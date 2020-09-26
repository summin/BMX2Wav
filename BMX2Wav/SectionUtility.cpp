//
//  Section.cpp
//  BMX2Wav
//
//  Created by Aleksandr Leontjev on 20/09/2020.
//  Copyright © 2020 Aleksandr Leontjev. All rights reserved.
//

#include "SectionUtility.hpp"
#include <iostream>
#include <fstream>
#include <array>

using namespace std;

array<uint32_t, 2> SectionUtility::seekSectionOffsetAndSize(array<char, 4> sectionGauge, ifstream &inputstream) {
    
    array<uint32_t, 2> sectionOffsetAndSize;
    inputstream.clear();
    inputstream.seekg(0);
    
    char buffer;
    char* bufferPtr;
    bufferPtr = &buffer;
    
    std::array<char, 4> fantomGauge {'-','-','-','-'};
    int maxItr = 180000000;
    int counter = 0;
    while (counter < maxItr) {

        inputstream.read(bufferPtr, 1);
        fantomGauge[0] = fantomGauge[1];
        fantomGauge[1] = fantomGauge[2];
        fantomGauge[2] = fantomGauge[3];
        fantomGauge[3] = buffer;
        counter++;
        
        if (fantomGauge == sectionGauge) {
            break;
        }
        if (inputstream.eof()) {
            this->isSectionFound = false;
            return sectionOffsetAndSize;
        }
    }
    
    inputstream.read((char*)&sectionOffsetAndSize[0], sizeof(sectionOffsetAndSize[0]));
    inputstream.read((char*)&sectionOffsetAndSize[1], sizeof(sectionOffsetAndSize[1]));
    this->isSectionFound = true;
    
    return sectionOffsetAndSize;
}

string SectionUtility::seekSectionStringValue(array<char, 4> sectionGauge, ifstream &inputstream) {
    
    inputstream.clear();
    inputstream.seekg(0);
    string bver;
    char buf;
    char buffer;
    char* bufferPtr;
    bufferPtr = &buffer;
    
    std::array<char, 4> fantomGauge {'-','-','-','-'};
    int maxItr = 180000000;
    int counter = 0;
    while (counter < maxItr) {

        inputstream.read(bufferPtr, 1);
        fantomGauge[0] = fantomGauge[1];
        fantomGauge[1] = fantomGauge[2];
        fantomGauge[2] = fantomGauge[3];
        fantomGauge[3] = buffer;
        counter++;
        
        if (fantomGauge == sectionGauge) {
            break;
        }
        if (inputstream.eof()) {
            this->isSectionFound = false;
            return bver;
        }
    }
    
    while (buf != '\0') {
        cout << buf;
        inputstream.read(&buf, 1);
        bver.push_back(buf);
    }
    
    this->isSectionFound = true;
    
    return bver;
}
