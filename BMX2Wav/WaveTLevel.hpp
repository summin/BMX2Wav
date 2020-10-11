//
//  WaveTLevel.hpp
//  BMX2Wav
//
//  Created by Aleksandr Leontjev on 04/10/2020.
//  Copyright © 2020 Aleksandr Leontjev. All rights reserved.
//

#ifndef WaveTLevel_hpp
#define WaveTLevel_hpp

#include <stdio.h>


class WaveTLevel {
public:
    u_int32_t numberOfSamples;
    u_int32_t loopBegin;
    u_int32_t loopEnd;
    u_int32_t sampleRate;
    char rootNote;
};

#endif /* WaveTLevel_hpp */
