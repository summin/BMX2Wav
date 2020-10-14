//
//  Wave.cpp
//  BMX2Wav
//
//  Created by Aleksandr Leontjev on 16/09/2020.
//  Copyright © 2020 Aleksandr Leontjev. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "WaveFile.hpp"

void WaveFile::writeHeader(std::ofstream &outputfile) {
    std::cout << "\nwritingHEADER\n";
    outputfile << "RIFF";
    outputfile.write(reinterpret_cast<const char *>(&this->header_waveDataSizeA), sizeof(header_waveDataSizeA));
    outputfile << "WAVE";
    outputfile << "fmt ";
    outputfile.write(reinterpret_cast<const char *>(&this->header_some16), sizeof(header_some16));
    outputfile.write(reinterpret_cast<const char *>(&this->header_pcm), sizeof(header_pcm));
    outputfile.write(reinterpret_cast<const char *>(&this->header_channels), sizeof(header_channels));
    outputfile.write(reinterpret_cast<const char *>(&this->header_sampleRate), sizeof(header_sampleRate));
    outputfile.write(reinterpret_cast<const char *>(&this->header_sampleRateBitCh), sizeof(header_sampleRateBitCh));
    outputfile.write(reinterpret_cast<const char *>(&this->header_BitCh), sizeof(header_BitCh));
    outputfile.write(reinterpret_cast<const char *>(&this->header_Bits), sizeof(header_Bits));
    outputfile << "data";
    outputfile.write(reinterpret_cast<const char *>(&this->header_waveDataSize), sizeof(header_waveDataSize));
}

void WaveFile::writeSizesToHeader(std::ofstream &outputfile) {
    long long waveDataSizeLong = outputfile.tellp();
    outputfile.seekp(4);
    this->header_waveDataSizeA = (int32_t)waveDataSizeLong - 8;
    outputfile.write(reinterpret_cast<const char *>(&this->header_waveDataSizeA), sizeof(this->header_waveDataSizeA));
    outputfile.seekp(40);
    this->header_waveDataSize = (int32_t)waveDataSizeLong - 44;
    outputfile.write(reinterpret_cast<const char *>(&this->header_waveDataSize), sizeof(this->header_waveDataSize));
}

void WaveFile::setWaveIndex(std::ifstream &inputstream) {
    inputstream.read((char*)&this->waveIndex, sizeof(this->waveIndex));
}

void WaveFile::setWaveFormat(std::ifstream &inputstream) {
    inputstream.read((char*)&this->waveFormat, sizeof(this->waveFormat));
}

void WaveFile::writeData(std::ofstream &outputfile, std::ifstream &inputstream, u_int32_t waveSecSize) {

    u_int32_t waveSizeBytes;
    inputstream.read((char*)&waveSizeBytes, sizeof(waveSizeBytes));
    
    // waveSizeBytes = waveSecSize;
    std::cout << "\nwaveSize: " << waveSizeBytes << "\n";
    
    for (typeof waveSizeBytes i = 0; i < waveSizeBytes; ++i ){
        inputstream.read(&this->waveStreamBuffer, sizeof(this->waveStreamBuffer));
        outputfile << this->waveStreamBuffer;
    }
    
}
