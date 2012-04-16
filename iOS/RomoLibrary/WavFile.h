//      
//       o      
//       | 
//  //=======\\
//  || o   o ||
//  ||       ||=>
//  ||  ---  ||
//  \\=======//
//
//  ROMOTIVE, INC
//  WavFile.h
//  C++ class load a wave file to play.
//  http://romotive.com
//
//  Created by James Knight.
//  Last Revision Date: 3/5/2012
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//

#ifndef WAVFILE_H
#define WAVFILE_H

#include "RomoInclude.h"
#include <string>

struct WAVHeader
{
    char  riff[4];//'RIFF'
    unsigned int riffSize;
    char  wave[4];//'WAVE'
    char  fmt[4];//'fmt '
    unsigned int fmtSize;
    unsigned short format;
    unsigned short channels;
    unsigned int samplesPerSec;
    unsigned int bytesPerSec;
    unsigned short blockAlign;
    unsigned short bitsPerSample;
    char  data[4];//'data'
    unsigned int dataSize;
};

class WavFile
{
private:
    WAVHeader *mHeader;
    uint8_t *mData;
    
public:
    WavFile(std::string filename);
    ~WavFile();
    
    WAVHeader *getHeader();
    uint8_t *getData();
};

#endif
