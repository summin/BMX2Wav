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
#include <stdint.h>

using namespace std;

static void extractWave(std::ifstream &bmxFile) {
    
    int32_t buffer;
    bmxFile.read((char*)&buffer, 3);
    
    cout << "Writing Sample Data\n";
    
    ofstream outputfile;
    uint32_t waveSizeBytes;
    char* waveStreamBuffer = new char[1];
    int32_t waveDataSize;
    
    bmxFile.read((char*)&waveSizeBytes, sizeof(waveSizeBytes));
    outputfile.open("test.wav");
    
    for (typeof waveSizeBytes i = 0; i < waveSizeBytes; ++i ){
        bmxFile.read(waveStreamBuffer, sizeof(waveStreamBuffer));
        outputfile.write(waveStreamBuffer, sizeof(waveStreamBuffer));
    }
    
    long long waveDataSizeLong = outputfile.tellp();
    waveDataSize = (int32_t)waveDataSizeLong;
    
    // appending header
    cout << "APPENDING LOWER HEADER\n";
    
    outputfile.seekp(0);
    
    int32_t some16 = 16;
    int16_t pcm = 1;
    int16_t channels = 1;
    int32_t sampleRate = 44100;
    int32_t sampleRateBitCh = 0;
    int16_t BitCh = 0;
    int16_t Bits = 16;
    
    int32_t waveDataSizeA = waveDataSize + 36;
    
    outputfile << "RIFF";
    outputfile.write(reinterpret_cast<const char *>(&waveDataSizeA), sizeof(waveDataSizeA));
    outputfile << "WAVE";
    outputfile << "fmt ";
    outputfile.write(reinterpret_cast<const char *>(&some16), sizeof(some16));
    outputfile.write(reinterpret_cast<const char *>(&pcm), sizeof(pcm));
    outputfile.write(reinterpret_cast<const char *>(&channels), sizeof(channels));
    outputfile.write(reinterpret_cast<const char *>(&sampleRate), sizeof(sampleRate));
    outputfile.write(reinterpret_cast<const char *>(&sampleRateBitCh), sizeof(sampleRateBitCh));
    outputfile.write(reinterpret_cast<const char *>(&BitCh), sizeof(BitCh));
    outputfile.write(reinterpret_cast<const char *>(&Bits), sizeof(Bits));
    outputfile << "data";
    outputfile.write(reinterpret_cast<const char *>(&waveDataSize), sizeof(waveDataSize));

    cout << "Sample Sizes: " << waveDataSizeA << " and " << waveDataSize << "\n";
      
    outputfile.close();
}

int main(int argc, const char * argv[]) {
    
    uint32_t numberOfSections;
    uint32_t waveSectionOffset;
    uint32_t waveSectionSize;
    uint16_t numberOfWaves;

    ifstream bmxFile;
    string buzzStr;
    array<char, 4> wavtSecName  {'W','A','V','T'};
    array<char, 4> waveSecName1 {'W','A','V','E'};
    array<char, 4> waveSecName2 {'C','W','A','V'};
    array<char, 4> waveSecGauge {'-','-','-','-'};
    
    char buffer;
    char* bufferPtr;
    bufferPtr = &buffer;
    
    // bmxFile.open("./Bmx/2Samples.bmx", ios::binary);
    bmxFile.open("./Bmx/2Samples1Tracker.bmx", ios::binary);
    // bmxFile.open("./Bmx/Empty.bmx", ios::binary);
    // bmxFile.open("./Bmx/A story from the sea total1.bmx", ios::binary);

    
    // get number of sections
    
    for (int i = 0; i < 4; ++i) {
        bmxFile.read(&buffer, 1);
        buzzStr += buffer;
    }
    bmxFile.read(reinterpret_cast<char *>(&numberOfSections), sizeof(numberOfSections));
    
    // get wave and size of wave section
    
    int maxItr = 180000000;
    int counter = 0;
    while (counter < maxItr) {

        bmxFile.read(bufferPtr, 1);
        waveSecGauge[0] = waveSecGauge[1];
        waveSecGauge[1] = waveSecGauge[2];
        waveSecGauge[2] = waveSecGauge[3];
        waveSecGauge[3] = buffer;
        counter++;
        
        if (waveSecGauge == waveSecName1 || waveSecGauge == waveSecName2) {
            break;
        }
    }
    
    bmxFile.read((char*)&waveSectionOffset, sizeof(waveSectionOffset));
    bmxFile.read((char*)&waveSectionSize, sizeof(waveSectionSize));
    
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
