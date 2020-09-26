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
    std::string name;
    float volume;
    u_int8_t flags;
    bool stereo;
    bool envelopes;
    
    void setFields(std::ifstream &inputstream);
    void setFieldsDynamic(std::ifstream &inputstream, int index);

    void extractStereo();
    void extractEnvelopes();
    void skipEnvelopes(std::ifstream &inputstream);
    void skipRemaining(std::ifstream &inputstream);
    void seekNext(std::ifstream &inputstream);
    void printFieldsValues();
};

#endif /* WaveT_hpp */
