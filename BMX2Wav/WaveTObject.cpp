//
//  WaveT.cpp
//  BMX2Wav
//
//  Created by Aleksandr Leontjev on 20/09/2020.
//  Copyright © 2020 Aleksandr Leontjev. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <regex>
#include "WaveTObject.hpp"

using namespace std;


void WaveTObject::setFields(std::ifstream &inputstream, int maxIndex) {
    
    // INDEX
    inputstream.read((char*)&this->index, sizeof(this->index));
    
    // FILENAME
    char buffer = ' ';
    while (buffer != '\0') {
        inputstream.read(&buffer, 1);
        this->fileName.push_back(buffer);
    }
    
    // NAME
    buffer = ' ';
    while (buffer != '\0') {
        inputstream.read(&buffer, 1);
        this->name.push_back(buffer);
    }
    
    this->fileName = this->fileName.substr(3);
    this->fileName = regex_replace(this->fileName, regex("\\\\"), "/");
    
    inputstream.read((char*)&this->volume, sizeof(this->volume));
    inputstream.read((char*)&this->flags, sizeof(this->flags));
    
}

void WaveTObject::parseFlags() {
    this->stereo = (bool)((this->flags >> 3) & 1);
    this->envelopes = (bool)((this->flags >> 7) & 1);
    this->floatingPointMem = (bool)((this->flags >> 2) & 1);
    this->loop = (bool)((this->flags >> 0) & 1);
    this->dontSave = (bool)((this->flags >> 1) & 1);
}

void WaveTObject::processLevels(std::ifstream &inputstream) {
    inputstream.read((char*)&this->numberOfLevels, sizeof(this->numberOfLevels));
    cout << "NUMBER OF LEVELS: " << (int)this->numberOfLevels << "\n";
    for (int iNLev = 0; iNLev < this->numberOfLevels; ++iNLev) {
        WaveTLevel waveTLevel;
        inputstream.read((char*)&waveTLevel.numberOfSamples, sizeof(waveTLevel.numberOfSamples));
        inputstream.read((char*)&waveTLevel.loopBegin, sizeof(waveTLevel.loopBegin));
        inputstream.read((char*)&waveTLevel.loopEnd, sizeof(waveTLevel.loopEnd));
        inputstream.read((char*)&waveTLevel.sampleRate, sizeof(waveTLevel.sampleRate));
        inputstream.read((char*)&waveTLevel.rootNote, sizeof(waveTLevel.rootNote));
        
        waveTLevelList.push_back(waveTLevel);
        cout << "Level numberOfSamples: " << waveTLevel.numberOfSamples << "\n";
        //cout << "Level loopBegin: " << waveTLevel.loopBegin << "\n";
        //cout << "Level loopEnd: " << waveTLevel.loopEnd << "\n";
        cout << "Level sampleRate: " << waveTLevel.sampleRate << "\n";
        cout << "Level rootNote: " << waveTLevel.rootNote << "\n";
        
    }
}

void WaveTObject::processEnvelopes(std::ifstream &inputstream){
    // cout << "PROCENV\n";
    u_int16_t numberOfEnvs;
    inputstream.read((char*)&numberOfEnvs, sizeof(numberOfEnvs));
    // cout << "NumEnvs: " << numberOfEnvs << "\n";
    
    for (int iNEnv = 0; iNEnv < numberOfEnvs; ++iNEnv) {
        
        u_int16_t attack;
        u_int16_t decay;
        u_int16_t sustain;
        u_int16_t release;
        char adsrSubDivide;
        char adsrFlags;
        
        inputstream.read((char*)&attack, sizeof(attack));
        inputstream.read((char*)&decay, sizeof(decay));
        inputstream.read((char*)&sustain, sizeof(sustain));
        inputstream.read((char*)&release, sizeof(release));
        inputstream.read((char*)&adsrSubDivide, sizeof(adsrSubDivide));
        inputstream.read((char*)&adsrFlags, sizeof(adsrFlags));
        
        // cout << "attack: " << attack << "\n";
        // cout << "decay: " << decay << "\n";
        // cout << "sustain: " << sustain << "\n";
        // cout << "release: " << release << "\n";
        
        u_int16_t numberOfPointsAndEnvOff;
        int numberOfPoints;
        bool envOff;
        inputstream.read((char*)&numberOfPointsAndEnvOff, sizeof(numberOfPointsAndEnvOff));
        numberOfPoints = numberOfPointsAndEnvOff & 0xeff;
        envOff = (bool)((numberOfPointsAndEnvOff >> 1) & 1);
        // cout << "NumPoints: " << numberOfPoints << "\n";
        
        for (int iNPoints = 0; iNPoints < numberOfPoints; ++iNPoints) {
            u_int16_t pointX;
            u_int16_t pointY;
            char pointFlags;
            inputstream.read((char*)&pointX, sizeof(pointX));
            inputstream.read((char*)&pointY, sizeof(pointY));
            inputstream.read((char*)&pointFlags, sizeof(pointFlags));
            // cout << "pointX: " << pointX << "\n";
            // cout << "pointY: " << pointY << "\n";
            // cout << "pointFlags: " << pointFlags << "\n";
        }
    }
}

void WaveTObject::printFieldsValues(){
    cout << "index: " << this->index << "\n";
    cout << "filename: " << this->fileName << "\n";
    // cout << "name: " << this->name << "\n";
    // cout << "volume: " << this->volume << "\n";
    // cout << "stereo: " << this->stereo << "\n";
    // cout << "flags: " << (int)this->flags << "\n";
    // cout << "flagEnvelopes: " << this->envelopes << "\n";
    // cout << "flagFloatingPointMem: " << this->floatingPointMem << "\n";
    // cout << "flagLoop: " << this->loop << "\n";
    // cout << "flagDontSave: " << this->dontSave << "\n";
    cout << "\n";
}

void WaveTObject::extractFolderName() {
    size_t found = this->fileName.find_last_of("/");
    this->folderName = this->fileName.substr(0,found);
    this->folderFileName = this->fileName.substr(found+1);
    cout << "_" << this->folderFileName << " " << this->folderName << "_\n";
    
}

void WaveTObject::setFieldsDynamic(std::ifstream &inputstream, int maxIndex) {

    long streamPos = inputstream.tellg();
    
    char diskName;
    char colon;
    char backslash;
    
    while (true) {
        inputstream.seekg(streamPos);
        inputstream.read((char*)&this->index, sizeof(this->index));
        inputstream.read(&diskName, 1);
        inputstream.read(&colon, 1);
        inputstream.read(&backslash, 1);
        
        if (this->index < 500
            && diskName >= 'C'
            && diskName < 'L'
            && colon == ':'
            && backslash == '\\'
            )
        {
            char buffer = ' ';
            while (buffer != '\0') {
                inputstream.read(&buffer, 1);
                this->fileName.push_back(buffer);
            }
            
            buffer = ' ';
            while (buffer != '\0') {
                inputstream.read(&buffer, 1);
                this->name.push_back(buffer);
            }
            
            inputstream.read((char*)&this->volume, sizeof(this->volume));
            inputstream.read((char*)&this->flags, sizeof(this->flags));
            break;
        } else {
            ++streamPos;
        }
    }
    this->fileName = regex_replace(this->fileName, regex("\\\\"), "/");

}
