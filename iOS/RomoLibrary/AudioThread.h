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
//  AudioThread.h
//  C++ class to handle all Audio Thread commands.
//  http://romotive.com
//
//  Created by James Knight.
//  Last Revision Date: 2/28/2012
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//

#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

#include <string>
#include "RomoInclude.h"
#include "Thread.h"
#include "AudioInterface.h"

class AudioThread: public Thread
{
public:
    AudioThread();
    virtual ~AudioThread();
    
    virtual void run();
    
    void playSoundFile(std::string filename);
    void playMotorCommand(uint8_t leftValue, uint8_t rightValue);
    void playAuxCommand(uint8_t auxValue);
    
private:
    pthread_mutex_t mEffectMutex;
    pthread_mutex_t mMotorMutex;
    pthread_mutex_t mAuxMutex;
    
    bool mEffectDirty;
    bool mMotorDirty;
    bool mAuxDirty;
    
    std::string mEffectFilename;
    MotorCommand mMotorCommand;
    AuxCommand mAuxCommand;
    AudioInterface *mAudioInterface;
    
};

#endif
