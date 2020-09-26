//
//  WaveT.cpp
//  BMX2Wav
//
//  Created by Aleksandr Leontjev on 20/09/2020.
//  Copyright © 2020 Aleksandr Leontjev. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "WaveTObject.hpp"

using namespace std;
void WaveTObject::setFields(std::ifstream &inputstream) {

    inputstream.read((char*)&this->index, sizeof(this->index));
    
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
    cout << "FLAGS: " << this->flags << "\n";
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
    

    cout << "FLAGS: " << this->flags << "\n";
}


void WaveTObject::extractStereo() {
    this->stereo = (bool)((this->flags >> 3) & 1);
}

void WaveTObject::extractEnvelopes() {
    this->envelopes = (bool)((this->flags >> 7) & 1);
    
    cout << "index: " << this->index << "\n";
    cout << "filename: " << this->fileName << "\n";
    cout << "name: " << this->name << "\n";
    cout << "volume: " << this->volume << "\n";
    cout << "stereo: " << this->stereo << "\n";
    cout << "envelopes: " << this->envelopes << "\n";
}

void WaveTObject::skipEnvelopes(std::ifstream &inputstream){
    char buffer;
    u_int16_t numberOfEnvs;
    inputstream.read((char*)&numberOfEnvs, 2);

    cout << "NUMBER OF ENVS: " << numberOfEnvs << "\n";
    
    for (int i = 0; i < numberOfEnvs || i < 3; ++i) {
        // u_int16_t attack;
        // u_int16_t decay;
        // u_int16_t sustain;
        // u_int16_t release;
        // u_int8_t adsrsbd;
        // u_int8_t adsrFlags;
        // inputstream.read((char*)&attack, sizeof(attack));
        // cout << "\n attack is: " << attack << "\n";
        // inputstream.read((char*)&decay, sizeof(decay));
        // cout << "\n decay is: " << decay << "\n";
        // inputstream.read((char*)&sustain, sizeof(sustain));
        // cout << "\n sustain is: " << sustain << "\n";
        // inputstream.read((char*)&release, sizeof(release));
        // inputstream.read((char*)&release, sizeof(release));
        // cout << "\n release is: " << release << "\n";
        // inputstream.read((char*)&adsrsbd, sizeof(adsrsbd));
        // cout << "\n adsrsbd is: " << adsrsbd << "\n";
        // inputstream.read((char*)&adsrFlags, sizeof(adsrFlags));
        // cout << "\n adsrFlags is: " << adsrFlags << "\n";
        
        
        // skip unneccessary data in env
        for(int j = 0; j < 10; ++j) {
            inputstream.read(&buffer, 1);
        }
        // endskip
        
        u_int16_t numberOfPoints;
        inputstream.read((char*)&numberOfPoints, 2);
        
        cout << "NUMBER OF POINTS: " << numberOfPoints << "\n";
        for (int k = 0; k < numberOfPoints || k < 2; ++k) {
            
            // skip unneccessary data in point
            for(int l = 0; l < 4; ++l) {
                inputstream.read(&buffer, 1);
            }
            // endskip
        }
    }
    inputstream.read(&buffer, 1);
}

void WaveTObject::skipRemaining(std::ifstream &inputstream){
    int8_t numberOfLevels;
    inputstream.read((char*)&numberOfLevels, sizeof(numberOfLevels));
    for (int i = 0; i < numberOfLevels; ++i){
        char buffer;
        
        // skip unneccessary data in levels
        for(int j = 0; j < 17; ++j) {
            inputstream.read(&buffer, 1);
        }
        // endskip
    }
}

void WaveTObject::printFieldsValues(){
    cout << "index: " << this->index << "\n";
    cout << "filename: " << this->fileName << "\n";
    cout << "name: " << this->name << "\n";
    cout << "volume: " << this->volume << "\n";
    cout << "stereo: " << this->stereo << "\n";
    cout << "envelopes: " << this->envelopes << "\n";
}
