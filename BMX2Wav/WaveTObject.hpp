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
#include <cstddef>

class WaveTObject {
public:
    u_int16_t index;
    std::string fileName;
    std::string folderName;
    std::string folderFileName;
    
    std::string name;
    float volume;
    u_int8_t flags;
    bool stereo;
    bool envelopes;
    bool floatingPointMem;
    bool loop;
    bool dontSave;
    
    void setFieldsDynamic(std::ifstream &inputstream, int index);
    void setFields(std::ifstream &inputstream, int index);
    void extractFolderName();
    void extractStereo();
    void extractEnvelopes();
    void printFieldsValues();
};

#endif /* WaveT_hpp */
