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
//  AudioThread.cpp
//  C++ class to create an audio thread.
//  http://romotive.com
//
//  Created by James Knight.
//  Last Revision Date: 2/28/2012
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//

#include "AudioThread.h"

AudioThread::AudioThread()
{
    pthread_mutex_init(&mEffectMutex, NULL);
    pthread_mutex_init(&mMotorMutex, NULL);
    pthread_mutex_init(&mAuxMutex, NULL);
    
    mEffectDirty = FALSE;
    mEffectFilename = std::string();
    
    mMotorDirty = FALSE;
    mMotorCommand = MotorCommand();
    
    mAuxDirty = FALSE;
    mAuxCommand = AuxCommand();
    
    mAudioInterface = AudioInterface::create();
}

AudioThread::~AudioThread()
{
    pthread_mutex_destroy(&mEffectMutex);
    pthread_mutex_destroy(&mMotorMutex);
    pthread_mutex_destroy(&mAuxMutex);
    
    if (mAudioInterface)
    {
        delete mAudioInterface;
        mAudioInterface = NULL;
    }
}

void AudioThread::run()
{
    while (mState == THREAD_RUNNING)
    {
        if (mEffectDirty)
        {
            pthread_mutex_lock(&mEffectMutex);
            mEffectDirty = !(mAudioInterface->playSoundFile(mEffectFilename));
            pthread_mutex_unlock(&mEffectMutex);
        }
        
        if (mMotorDirty)
        {
            pthread_mutex_lock(&mMotorMutex);
            mMotorDirty = !(mAudioInterface->playMotorCommand(mMotorCommand));
            pthread_mutex_unlock(&mMotorMutex);
        }
        
        if (mAuxDirty)
        {
            pthread_mutex_lock(&mAuxMutex);
            mAuxDirty = !(mAudioInterface->playAuxCommand(mAuxCommand));
            pthread_mutex_unlock(&mAuxMutex);
        }
        
        mAudioInterface->reclaimBuffers();
    }
}



void AudioThread::playSoundFile(std::string filename)
{
    pthread_mutex_lock(&mEffectMutex);
    
    mEffectFilename = filename;
    mEffectDirty = TRUE;
    
    pthread_mutex_unlock(&mEffectMutex);
}

void AudioThread::playMotorCommand(uint8_t leftValue, uint8_t rightValue)
{
    pthread_mutex_lock(&mMotorMutex);
    
    mMotorCommand.leftValue = leftValue;
    
    if (rightValue == 0x80)
        mMotorCommand.rightValue = 0x80;
    else
        mMotorCommand.rightValue = 0xFF - rightValue;
    
    mMotorDirty = TRUE;
    
    pthread_mutex_unlock(&mMotorMutex);
}

void AudioThread::playAuxCommand(uint8_t auxValue)
{
    pthread_mutex_lock(&mAuxMutex);
    
    mAuxCommand.auxValue = auxValue;
    mAuxDirty = TRUE;
    
    pthread_mutex_unlock(&mAuxMutex);
}