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

namespace fs = std::__fs::filesystem;
using namespace std;

/**
 writes data to waveFile
 */
static int extractWaves(std::ifstream &bmxFile, WaveTObject wavTs[], uint16_t numberOfTs, u_int32_t waveSecSize)
{
    cout << "\nentered the wav extraction " << numberOfTs << "\n";
    for (int m = 0; m < numberOfTs; ++m)
    {
        ofstream outputfile;
        // outputfile.open("./1.wav");
        // // ofstream outputfile1;
        WaveFile waveFile;
        waveFile.setWaveIndex(bmxFile);
        cout << "\n_" << waveFile.waveIndex << "_ index \n";
        // waveFile.setWaveFormat(bmxFile);
        // cout << "\n_" << (int)waveFile.waveFormat << "_ format\n";
        

        for (uint16_t n = 0; n < numberOfTs; ++n)
        {
            if (wavTs[n].index == waveFile.waveIndex)
            {
                __fs::filesystem::create_directories("extractedSmp/" + wavTs[n].folderName);
                outputfile.open("extractedSmp/" + wavTs[n].fileName);
            }
        }

        cout << "\n CURRENT POS " << bmxFile.tellg() << "\n";

        waveFile.writeHeader(outputfile);
        waveFile.writeData(outputfile, bmxFile, waveSecSize);
        waveFile.writeSizesToHeader(outputfile);
        outputfile.close();
    }
    return 0;
}

/**
 process a BMX file
 */
static int processFile(const std::string &bmxFileName)
{
    uint32_t numberOfSections;
    uint16_t numberOfWaves;
    uint16_t numberOfWavTs;
    string bver = "";
    ifstream bmxFile;
    string buzzStr;

    array<char, 4> wavTSecName{'W', 'A', 'V', 'T'};
    array<char, 4> waveSecName1{'W', 'A', 'V', 'E'};
    array<char, 4> waveSecName2{'C', 'W', 'A', 'V'};
    array<char, 4> bverSecName{'B', 'V', 'E', 'R'};

    char buffer;
    char *bufferPtr;
    bufferPtr = &buffer;
    bmxFile.open(bmxFileName, ios::binary);

    /**
     get number of sections
     */
    for (int i = 0; i < 4; ++i)
    {
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
    array<uint32_t, 2> bverSectionOffsetAndSize;
    SectionUtility sectionUtility;
    wavTSectionOffsetAndSize = sectionUtility.seekSectionOffsetAndSize(wavTSecName, bmxFile);
    waveSectionOffsetAndSize = sectionUtility.seekSectionOffsetAndSize(waveSecName1, bmxFile);
    if (!sectionUtility.isSectionFound)
    {
        waveSectionOffsetAndSize = sectionUtility.seekSectionOffsetAndSize(waveSecName2, bmxFile);
    }
    bverSectionOffsetAndSize = sectionUtility.seekSectionOffsetAndSize(bverSecName, bmxFile);
    
    cout << sectionUtility.seekSectionStringValue(bverSecName, bmxFile) << "\n";
    cout << "WaveSecOffet: " << waveSectionOffsetAndSize[0] << "\n";
    cout << "WaveSecSize: " << waveSectionOffsetAndSize[1] << "\n";
    cout << "WavTSecOffet: " << wavTSectionOffsetAndSize[0] << "\n";
    cout << "WavTSecSize: " << wavTSectionOffsetAndSize[1] << "\n";

    /**
     Get version
     */
    bmxFile.clear();
    bmxFile.seekg(0);
    bmxFile.seekg(bverSectionOffsetAndSize[0], ios::beg);
    *bufferPtr = '-';
    while (buffer != '\0') {
        bmxFile.read(bufferPtr, 1);
        bver.push_back(buffer);
    }
    cout << "Buzz Version: " << bver << "\n";

    /**
     Get number of WavTs
     */
    bmxFile.clear();
    bmxFile.seekg(0);
    bmxFile.seekg(wavTSectionOffsetAndSize[0], ios::beg);
    bmxFile.read((char *)&numberOfWavTs, sizeof(numberOfWavTs));
    cout << "Number of WavTs in BMX: " << numberOfWavTs << "\n";

    WaveTObject waveTList[numberOfWavTs];

    for (int i = 0; i < numberOfWavTs; ++i)
    {
        WaveTObject waveTObject;
        waveTObject.setFields(bmxFile, i);
        waveTObject.extractFolderName();
        waveTObject.parseFlags();
        if (waveTObject.envelopes) {
            waveTObject.processEnvelopes(bmxFile);
        }
        waveTObject.processLevels(bmxFile);
        waveTObject.printFieldsValues();
        waveTList[i] = waveTObject;
    }

    /**
     Get number of Waves
     */
    bmxFile.clear();
    bmxFile.seekg(0);
    bmxFile.seekg(waveSectionOffsetAndSize[0], ios::beg);
    bmxFile.read((char *)&numberOfWaves, sizeof(numberOfWaves));
    cout << "Number of Waves in BMX: " << numberOfWaves << "\n";

    if (numberOfWaves != numberOfWavTs)
    {
        cout << "Unsupported file";
        return 1;
    }

    /**
     Process Waves
     */
    int extractResult;
    extractResult = extractWaves(bmxFile, waveTList, numberOfWavTs, waveSectionOffsetAndSize[1]);

    bmxFile.close();
    cout << "EXTRACTION COMPLETED \n\n";

    return 0;
}

/**
 main
 */
int main(int argc, const char *argv[])
{
    /**
     iterate through files in directory
     */
    std::string path = "./";
    for (const auto &entry : fs::directory_iterator(path))
    {
        string bmxFileName = entry.path();
        if ((bmxFileName.substr(bmxFileName.length() - 4, 4) == ".bmx") || (bmxFileName.substr(bmxFileName.length() - 4, 4) == ".BMX"))
        {
            cout << "\nProcessing file: " << bmxFileName << endl;
            processFile(bmxFileName);
        }
    }
}

/**
 code holder. inactive
 */
void someStuffUnused()
{

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
