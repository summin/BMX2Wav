//
//  Section.hpp
//  BMX2Wav
//
//  Created by Aleksandr Leontjev on 20/09/2020.
//  Copyright © 2020 Aleksandr Leontjev. All rights reserved.
//

#ifndef Section_hpp
#define Section_hpp

#include <stdio.h>
#include <iostream>
#include <array>

class SectionUtility {
public:
    bool sectionFound;
    std::array<uint32_t, 2> seekSectionOffsetAndSize(std::array<char, 4> sectionGauge, std::ifstream &inputstream);
};

#endif /* Section_hpp */
