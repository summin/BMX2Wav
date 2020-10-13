//
//  WaveT.hpp
//  BMX2Wav
//
//  Created by Aleksandr Leontjev on 20/09/2020.
//  Copyright © 2020 Aleksandr Leontjev. All rights reserved.
//

#ifndef WaveT_hpp
#define WaveT_hpp

#include <stdio.h>
#include <string>
#include <vector>

#include <cstddef>
#include "WaveTLevel.hpp"

class WaveTObject {
public:
    u_int16_t index;
    std::string fileName;
    std::string folderName;
    std::string folderFileName;
    
    
    std::string name;
    float volume;
    char flags;
    
    bool stereo;
    bool envelopes;
    bool floatingPointMem;
    bool loop;
    bool dontSave;
    
    u_int8_t numberOfLevels;
    std::vector<WaveTLevel> waveTLevelList;
    
    void setFieldsDynamic(std::ifstream &inputstream, int index);
    void setFields(std::ifstream &inputstream, int index);
    void processLevels(std::ifstream &inputstream);
    
    void processEnvelopes(std::ifstream &inputstream);
    
    void parseFlags();
    void extractFolderName();
    void printFieldsValues();
};

#endif /* WaveT_hpp */
