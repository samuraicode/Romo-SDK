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
//  AudioInterfaceOpenAL.h
//  C++ class to generate and play sounds with OpenAL.
//  http://romotive.com
//
//  Created by James Knight.
//  Last Revision Date: 2/28/2012
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//


#ifndef AUDIOINTERFACE_OPENAL_H
#define AUDIOINTERFACE_OPENAL_H

#include "AudioInterface.h"
#include <AudioToolbox/AudioToolbox.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <queue>

typedef enum {
    ROUTE_NONE      = 0,
    ROUTE_COMMAND   = 1,
    ROUTE_FX        = 2
} AudioRoute;

class AudioInterfaceOpenAL: public AudioInterface
{
public:    
    static const ALenum COMMAND_FORMAT = AL_FORMAT_STEREO8;
    
    AudioInterfaceOpenAL();
    virtual ~AudioInterfaceOpenAL();
    
    virtual bool playSoundFile(std::string filename);
    virtual bool playCommand(uint8_t address, uint8_t command, uint32_t leadCount, uint32_t sampleRate);
    virtual bool playMotorCommand(MotorCommand command);
    virtual bool playAuxCommand(AuxCommand command);
    
    virtual void reclaimBuffers();
    
private:
    AudioRoute mAudioRoute;
    
    uint32_t mBufferIndex;
    
    ALCdevice  *mAlDevice;
    ALCcontext *mAlContext;
    
    // Sources:
    ALuint mAlFxSource;
    ALuint mAlCommandSource;
    
    // Buffers:
    ALuint mAlFxBuffer;
    ALuint mAlCommandBuffers[BUFFER_COUNT];
    
    std::queue<ALuint> mBufferQueue;
    
    void loadAlBuffer(ALuint alBuffer, uint8_t *commandBuffer, uint32_t bufferSize, ALint format, ALint frequency);
    void setRoute(AudioRoute route);
    void queueBuffer(ALuint alSource, ALuint alBuffer);
    void startSource(ALuint alSource);
    
    void stopFxSource();
    void letCommandsFinish();
    void loadFxSource();
};

#endif
