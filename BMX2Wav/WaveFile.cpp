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

void WaveFile::writeHeader(std::ofstream &outputfile, bool floatMem, bool stereo) {
    
    if (stereo) {
        header_channels = 2;
    }
    if (floatMem) {
        header_Bits = 24;
    }
        // 1-4
    outputfile << "RIFF";
    // 5-8
    outputfile.write(reinterpret_cast<const char *>(&this->header_waveDataSizeA), sizeof(header_waveDataSizeA));
    // 9-12
    outputfile << "WAVE";
    // 13-16
    outputfile << "fmt ";
    // 17-20
    outputfile.write(reinterpret_cast<const char *>(&this->header_some16), sizeof(header_some16));
    // 21-22
    outputfile.write(reinterpret_cast<const char *>(&this->header_pcm), sizeof(header_pcm));
    // 23-24
    outputfile.write(reinterpret_cast<const char *>(&this->header_channels), sizeof(header_channels));
    // 25-28
    outputfile.write(reinterpret_cast<const char *>(&this->header_sampleRate), sizeof(header_sampleRate));
    // 29-32
    this->header_sampleRateBitCh = (this->header_sampleRate*this->header_Bits*this->header_channels)/8;
    outputfile.write(reinterpret_cast<const char *>(&this->header_sampleRateBitCh), sizeof(header_sampleRateBitCh));
    // 33-34
    this->header_BitCh = (this->header_Bits*this->header_channels)/8;
    outputfile.write(reinterpret_cast<const char *>(&this->header_BitCh), sizeof(header_BitCh));
    // 35-36
    outputfile.write(reinterpret_cast<const char *>(&this->header_Bits), sizeof(header_Bits));
    // 37-40
    outputfile << "data";
    // 41-44
    outputfile.write(reinterpret_cast<const char *>(&this->header_waveDataSize), sizeof(header_waveDataSize));
}

void WaveFile::writeSizesToHeader(std::ofstream &outputfile, bool floatMem, bool stereo) {
    
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

int WaveFile::writeData(std::ofstream &outputfile, std::ifstream &inputstream, u_int32_t waveSecSize, bool floatMem, bool stereo, long fileSize) {

    u_int32_t waveSizeBytes;
    inputstream.read((char*)&waveSizeBytes, sizeof(waveSizeBytes));
    if (waveSizeBytes > (fileSize - 400)) {
        return 1;
    }
    // waveSizeBytes = waveSecSize;
    // std::cout << "\nwaveSize: " << waveSizeBytes << "\n";
    
    
    float bufferFloat;
    bufferFloat = 0.0;
    if (floatMem) {
        waveSizeBytes = waveSizeBytes/3;
    }
    
    for (typeof waveSizeBytes i = 0; i < waveSizeBytes; ++i ){
        
        inputstream.read(&this->waveStreamBuffer, sizeof(this->waveStreamBuffer));
        outputfile << this->waveStreamBuffer;
        
    }
    return 0;
}
