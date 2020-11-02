//
//  Wave.hpp
//  BMX2Wav
//
//  Created by Aleksandr Leontjev on 16/09/2020.
//  Copyright © 2020 Aleksandr Leontjev. All rights reserved.
//

#ifndef Wave_hpp
#define Wave_hpp

#include <stdio.h>
#include <string>

class WaveFile {
public:
    
    u_int32_t waveSizeBytes;
    u_int16_t waveIndex;
    char waveFormat;
    
    char waveStreamBuffer;
    std::string name;
    std::string path;
    
    int32_t header_waveDataSizeA = 0;
    int32_t header_some16 = 16;
    int16_t header_pcm = 1;
    int16_t header_channels = 1;
    int32_t header_sampleRate = 44100;
    int32_t header_sampleRateBitCh = 0;
    int16_t header_BitCh = 0;
    int16_t header_Bits = 16;
    int32_t header_waveDataSize = 0;
    
    void setWaveIndex(std::ifstream &inputstream);
    void setWaveFormat(std::ifstream &inputstream);
    int writeData(std::ofstream &outputfile, std::ifstream &inputstream, u_int32_t waveSecSize, bool floatMem, bool stereo, long fileSize);
    void writeHeader(std::ofstream &outputfile, bool floatMem, bool stereo);
    void writeSizesToHeader(std::ofstream &outputfile, bool floatMem, bool stereo);
    
};

#endif /* Wave_hpp */
