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
//  WavFile.cpp
//  C++ class load a wave file to play.
//  http://romotive.com
//
//  Created by James Knight.
//  Last Revision Date: 3/5/2012
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//

#include "WavFile.h"
#include "ResourceManager.h"

WavFile::WavFile(std::string filename)
{
    mHeader = (WAVHeader *) malloc(sizeof(WAVHeader));
    
    std::string fileUri = ResourceManager::getInstance()->getResourcePath() + filename;    
    
    FILE *wavFile = fopen(fileUri.c_str(), "rb");
    
    if (fread(mHeader, sizeof(WAVHeader), 1, wavFile))
    {
        mData = (uint8_t *) malloc(mHeader->dataSize);
        fread(mData, mHeader->dataSize, 1, wavFile);
    }
    
    fclose(wavFile);
}

WavFile::~WavFile()
{
    if (mHeader)
        free(mHeader);
}

WAVHeader *WavFile::getHeader()
{
    return mHeader;
}

uint8_t *WavFile::getData()
{
    return mData;
}