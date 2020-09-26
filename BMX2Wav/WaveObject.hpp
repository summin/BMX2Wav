//
//  WaveSection.hpp
//  BMX2Wav
//
//  Created by Aleksandr Leontjev on 20/09/2020.
//  Copyright © 2020 Aleksandr Leontjev. All rights reserved.
//

#ifndef WaveObject_hpp
#define WaveObject_hpp

#include <stdio.h>
#include <string>
#include <cstddef>

class WaveObject {
public:
    u_int16_t index;
    int8_t format;
    u_int32_t size;
    void setFields(std::ifstream &inputstream);
    void printFieldsValues();
};

#endif /* WaveObject_hpp */
