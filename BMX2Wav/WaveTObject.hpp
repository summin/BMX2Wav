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
    char flags;
    bool stereo;
    bool envelopes;
    
    void setIndex(std::ifstream &inputstream);
    void setFileName(std::ifstream &inputstream);
    void setName(std::ifstream &inputstream);
    void setVolume(std::ifstream &inputstream);
    void setFlags(std::ifstream &inputstream);
    void setStereo(char flags);
    void setEnvelopes(char flags);
    void skipEnvelopes(std::ifstream &inputstream);
    void skipRemaining(std::ifstream &inputstream);
};

#endif /* WaveT_hpp */
