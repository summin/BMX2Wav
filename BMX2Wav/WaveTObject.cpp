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

void WaveTObject::extractFolderName() {
    size_t found = this->fileName.find_last_of("/");
    this->folderName = this->fileName.substr(0,found);
    this->folderFileName = this->fileName.substr(found+1);
    // cout << "_" << this->folderFileName << " " << this->folderName << "_\n";
}

void WaveTObject::extractStereo() {
    this->stereo = (bool)((this->flags >> 3) & 1);
}

void WaveTObject::extractEnvelopes() {
    this->envelopes = (bool)((this->flags >> 7) & 1);
    this->floatingPointMem = (bool)((this->flags >> 2) & 1);
    this->loop = (bool)((this->flags >> 0) & 1);
    this->dontSave = (bool)((this->flags >> 1) & 1);
}

void WaveTObject::printFieldsValues(){
    cout << "index: " << this->index << "\n";
    cout << "filename: " << this->fileName << "\n";
    cout << "name: " << this->name << "\n";
    cout << "volume: " << this->volume << "\n";
    cout << "stereo: " << this->stereo << "\n";
    cout << "envelopes: " << this->envelopes << "\n";
    cout << "floatingPointMem: " << this->floatingPointMem << "\n";
    cout << "loop: " << this->loop << "\n";
    cout << "dontSave: " << this->dontSave << "\n";
}
