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
#include "SectionUtility.hpp"
#include "WaveFile.hpp"
#include "WaveTObject.hpp"
#include "WaveObject.hpp"

using namespace std;

/**
 writes data to waveFile
 */
static void extractWave(std::ifstream &bmxFile) {
    
    ofstream outputfile;
    outputfile.open("./samples1/test2.wav");

    WaveFile waveFile;
    waveFile.writeHeader(outputfile);
    waveFile.writeData(outputfile, bmxFile);
    waveFile.writeSizesToHeader(outputfile);
    
    outputfile.close();
}

/**
 process a BMX file
 */
static int processFile(const std::string &bmxFileName) {
    uint32_t numberOfSections;
    uint16_t numberOfWaves;
    uint16_t numberOfWavTs;
    ifstream bmxFile;
    string   buzzStr;
    
    array<char, 4> wavTSecName  {'W','A','V','T'};
    array<char, 4> waveSecName1 {'W','A','V','E'};
    array<char, 4> waveSecName2 {'C','W','A','V'};
    array<char, 4> bverSecName {'B','V','E','R'};
    
    char buffer;
    char* bufferPtr;
    bufferPtr = &buffer;
    bmxFile.open(bmxFileName, ios::binary);
    
    /**
     get number of sections
     */
    for (int i = 0; i < 4; ++i) {
        bmxFile.read(&buffer, 1);
        buzzStr += buffer;
    }
    bmxFile.read(reinterpret_cast<char *>(&numberOfSections), sizeof(numberOfSections));
    cout << "Number Of Sections: " << numberOfSections << "\n";
    
    /**
     get WAVE and WAVT section offset and size
     */
    array<uint32_t, 2> waveSectionOffsetAndSize;
    array<uint32_t, 2> wavTSectionOffsetAndSize;
    SectionUtility sectionUtility;
    wavTSectionOffsetAndSize = sectionUtility.seekSectionOffsetAndSize(wavTSecName, bmxFile);
    waveSectionOffsetAndSize = sectionUtility.seekSectionOffsetAndSize(waveSecName1, bmxFile);
    if (!sectionUtility.isSectionFound) {
        waveSectionOffsetAndSize = sectionUtility.seekSectionOffsetAndSize(waveSecName2, bmxFile);
    }
    cout << sectionUtility.seekSectionStringValue(bverSecName, bmxFile) << "\n";
    cout << "WaveSecOffet: " << waveSectionOffsetAndSize[0] << "\n";
    cout << "WaveSecSize: " << waveSectionOffsetAndSize[1] << "\n";
    cout << "WavTSecOffet: " << wavTSectionOffsetAndSize[0] << "\n";
    cout << "WavTSecSize: " << wavTSectionOffsetAndSize[1] << "\n";
    
    
    /**
     Get number of Waves
     */
    bmxFile.clear();
    bmxFile.seekg(0);
    bmxFile.seekg(waveSectionOffsetAndSize[0], ios::beg);
    bmxFile.read((char*)&numberOfWaves, sizeof(numberOfWaves));
    cout << "Number of Waves in BMX: " << numberOfWaves << "\n";
    
    /**
     Get number of WavTs
     */
    bmxFile.clear();
    bmxFile.seekg(0);
    bmxFile.seekg(wavTSectionOffsetAndSize[0], ios::beg);
    bmxFile.read((char*)&numberOfWavTs, sizeof(numberOfWavTs));
    cout << "Number of WavTs in BMX: " << numberOfWavTs << "\n";
    
    if (numberOfWaves != numberOfWavTs) {
        cout << "Unsupported file";
        return 1;
    }
    
    WaveObject waveList[numberOfWaves];
    WaveTObject waveTList[numberOfWavTs];
    
    for(int i = 0; i < numberOfWavTs; ++i){
        WaveTObject waveTObject;
        waveTObject.setFieldsDynamic(bmxFile, i);
        waveTObject.extractStereo();
        // waveTObject.extractEnvelopes();
        // if (waveTObject.envelopes == 1) {
        //     waveTObject.skipEnvelopes(bmxFile);
        // }
        // waveTObject.skipRemaining(bmxFile);
        waveTObject.printFieldsValues();
        waveTList[i] = waveTObject;
    }
    
    // for(int i = 0; i < numberOfWaves; ++i){
    //     WaveObject waveObject;
    //     waveList[i] = waveObject;
    // }
    
    /**
     read wave
     */
    cout << "READING WAVE \n\n";
    
    // extractWave(bmxFile);
    
    bmxFile.close();
    
    cout << "EXTRACTION COMPLETED \n\n";
    
    return 0;
}

/**
 main
 */
int main(int argc, const char * argv[]) {
    
    // string bmxFileName = "./Bmx/2Samples.bmx";
    // string bmxFileName = "./Bmx/2Samples1Tracker.bmx";
    // string bmxFileName = "./Bmx/bdr7.bmx";
    // string bmxFileName = "./Bmx/Empty.bmx";
    
    string bmxFileName = "./Bmx/A story from the sea total1.bmx";
    
    processFile(bmxFileName);
}

/**
 code holder. inactive
 */
void someStuff() {
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
    __fs::filesystem::create_directories("./samples1/asd/ds");
}
