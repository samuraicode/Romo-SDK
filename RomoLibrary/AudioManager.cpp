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
//  AudioManager.cpp
//  C++ class to handle all Audio Thread commands.
//  http://romotive.com
//
//  Created by James Knight.
//  Last Revision Date: 1/28/2012
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//

#include "AudioManager.h"
#include "AudioThread.h"

AudioManager::AudioManager() 
{
    mAudioThread = new AudioThread();

    mAudioThread->start();
}

AudioManager::~AudioManager() 
{
    if (mAudioThread)
    {
        delete mAudioThread;
        mAudioThread = NULL;
    }
}

void AudioManager::playSoundFile(std::string filename)
{
    mAudioThread->playSoundFile(filename);
}

void AudioManager::playMotorCommand(uint8_t leftValue, uint8_t rightValue)
{
    mAudioThread->playMotorCommand(leftValue, rightValue);
}

void AudioManager::playAuxCommand(uint8_t auxValue)
{
    mAudioThread->playAuxCommand(auxValue);
}