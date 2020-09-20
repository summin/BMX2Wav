//
//  main.cpp
//  BMX2Wav
//
//  Created by Aleksandr Leontjev on 29/08/2020.
//  Copyright © 2020 Aleksandr Leontjev. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <regex>
#include <stdint.h>
#include <SectionUtility.hpp>

#include "WaveFile.hpp"

using namespace std;

static void extractWave(std::ifstream &bmxFile) {
    
    // std::string str = "/api/asd/";
    // std::string pattern = "/api/(.*)/";
    // std::cout << "Starting matching" << std::endl;
    // std::smatch matches;
    // if (std::regex_match(str, matches, std::regex(pattern, std::regex::egrep)))
    // {
    //     std::cout << "Found match!" << std::endl;
    //     std::cout << "All matches: ";
    //     for (auto& it : matches)
    //         std::cout << it << ", ";
    //     std::cout << std::endl;
    // }
    
    cout << "Writing Sample Data\n";

    regex r;
    
    // data vars
    ofstream outputfile;
    __fs::filesystem::create_directories("./samples1/asd/ds");
    outputfile.open("./samples1/test2.wav");

    WaveFile waveFile;
    waveFile.writeHeader(outputfile);
    waveFile.writeData(outputfile, bmxFile);
    waveFile.writeSizesToHeader(outputfile);
    
    outputfile.close();
}

int main(int argc, const char * argv[]) {
    
    uint32_t numberOfSections;

    uint16_t numberOfWaves;

    ifstream bmxFile;
    string buzzStr;
    
    array<char, 4> wavtSecName  {'W','A','V','T'};
    array<char, 4> waveSecName1 {'W','A','V','E'};
    array<char, 4> waveSecName2 {'C','W','A','V'};
    
    
    char buffer;
    char* bufferPtr;
    bufferPtr = &buffer;
    
    
    // bmxFile.open("./Bmx/2Samples.bmx", ios::binary);
    // bmxFile.open("./Bmx/2Samples1Tracker.bmx", ios::binary);
    bmxFile.open("./Bmx/bdr7.bmx", ios::binary);
    // bmxFile.open("./Bmx/Empty.bmx", ios::binary);
    // bmxFile.open("./Bmx/A story from the sea total1.bmx", ios::binary);

    
    // get number of sections
    
    for (int i = 0; i < 4; ++i) {
        bmxFile.read(&buffer, 1);
        buzzStr += buffer;
    }
    bmxFile.read(reinterpret_cast<char *>(&numberOfSections), sizeof(numberOfSections));
    
    // get wave and size of wave section
    

    array<uint32_t, 2> waveSectionOffsetAndSize;
    array<uint32_t, 2> wavTSectionOffsetAndSize;
    SectionUtility sectionUtility;
    sectionUtility.seekSectionOffsetAndSize(<#std::array<char, 4> sectionGauge#>, <#std::ifstream &inputstream#>)
    
    // get number of Waves

    bmxFile.clear();
    bmxFile.seekg(0);
    bmxFile.seekg(waveSectionOffset, ios::beg);
    
    bmxFile.read((char*)&numberOfWaves, sizeof(numberOfWaves));
    
    cout
        << "\nSizeOFSecOffst: "     << sizeof(waveSectionOffset)
        << "\nSizeOFSecSize: "      << sizeof(waveSectionSize)
        << "\n";
    
    cout
        << "\nNumber Of Sections: " << numberOfSections
        << "\nWaves Offset: "       << waveSectionOffset
        << "\nWaves Size: "         << waveSectionSize
        << "\nNumber Of Waves: "    << numberOfWaves
        << "\n";
    
    // read wave
    
    cout << "READING WAVE \n\n";
    
    extractWave(bmxFile);
    
    bmxFile.close();
    
    cout << "EXTRACTION COMPLETED \n\n";
    
    return 0;
}
