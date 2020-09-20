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

class SectionUtility {
    void seekSection(char sectionGauge[], std::ifstream &inputstream);
};

#endif /* Section_hpp */
