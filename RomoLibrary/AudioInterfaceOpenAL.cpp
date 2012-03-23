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
//  AudioInterfaceOpenAL.cpp
//  C++ class to generate and play sounds with OpenAL.
//  http://romotive.com
//
//  Created by James Knight.
//  Last Revision Date: 2/28/2012
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//

#include "RomoInclude.h"
#include "AudioInterfaceOpenAL.h"
#include "WavFile.h"
#include "TargetConditionals.h"

AudioInterfaceOpenAL::AudioInterfaceOpenAL(): AudioInterface()
{
    mBufferQueue = std::queue<ALuint>();
    
#if TARGET_OS_IPHONE
    AudioSessionSetActive(TRUE);
#endif
    mAudioRoute = ROUTE_NONE;
    
    mAlDevice  = alcOpenDevice(NULL);
    mAlContext = alcCreateContext(mAlDevice, NULL);
    alcMakeContextCurrent(mAlContext);
    
    // Generate AL Sources:
    alGenSources(1, &mAlFxSource);
    alGenSources(1, &mAlCommandSource);
    
    // Initialize FX Source:
    alSourcef(mAlFxSource, AL_PITCH, 1.0f);
    alSourcef(mAlFxSource, AL_GAIN, 1.0f);
    alSourcei(mAlFxSource, AL_LOOPING, AL_FALSE);
    
    
    // Initialize Command Source:
    alSourcef(mAlCommandSource, AL_PITCH, 1.0f);
    alSourcef(mAlCommandSource, AL_GAIN, 1.0f);
    alSourcei(mAlCommandSource, AL_LOOPING, AL_FALSE);
    
    // Generate AL Buffers:
    alGenBuffers(1, &mAlFxBuffer);
    alGenBuffers(BUFFER_COUNT, mAlCommandBuffers);
    
    // Add command buffers to buffer queue.
    for (int i = 0; i < BUFFER_COUNT; i++)
        mBufferQueue.push(mAlCommandBuffers[i]);
}

AudioInterfaceOpenAL::~AudioInterfaceOpenAL()
{
    alDeleteSources(1, &mAlFxSource);
    alDeleteSources(1, &mAlCommandSource);
    
    alDeleteBuffers(1, &mAlFxBuffer);
    alDeleteBuffers(BUFFER_COUNT, mAlCommandBuffers);
    
    alcMakeContextCurrent(NULL);
    alcDestroyContext(mAlContext);
    alcCloseDevice(mAlDevice);
}

bool AudioInterfaceOpenAL::playSoundFile(std::string filename)
{    
    stopFxSource();
    
    
    WavFile wavFile = WavFile(filename);
        
    ALuint wavFormat = 0;
    switch (wavFile.getHeader()->bitsPerSample)
    {
        case 8:
            wavFormat = (wavFile.getHeader()->channels == 1) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
            break;
            
        case 16:
            wavFormat = (wavFile.getHeader()->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
            break;
            
        default:
            return 0;
    }
    
    loadAlBuffer(mAlFxBuffer, wavFile.getData(), wavFile.getHeader()->dataSize, wavFormat, wavFile.getHeader()->samplesPerSec);
    
    loadFxSource();
    setRoute(ROUTE_FX);
    
    startSource(mAlFxSource);
     
    return TRUE;
    
}

bool AudioInterfaceOpenAL::playCommand(uint8_t address, uint8_t command, uint32_t leadCount, uint32_t sampleRate)
{
    stopFxSource();
    
    if (mBufferQueue.size() > 0)
    {
        uint8_t *commandBuffer;
        
        uint8_t bitsPerMessage = BITS_PER_ADDRESS + BITS_PER_VALUE + BIT_PARITY;
        uint32_t bufferSize = ((bitsPerMessage * SAMPLES_PER_BIT) + (leadCount * 2)) * BYTES_PER_SAMPLE;
        
        commandBuffer = (uint8_t *) malloc(bufferSize);
        memset(commandBuffer, SIGNAL_LOW, bufferSize);
        
        uint32_t bufferIndex = 0;
        
        loadCommandBuffer(commandBuffer, &bufferIndex, address, command, leadCount);
        
        ALuint alBuffer = mBufferQueue.front();
        mBufferQueue.pop();
        
        loadAlBuffer(alBuffer, commandBuffer, bufferSize, COMMAND_FORMAT, sampleRate);
        queueBuffer(mAlCommandSource, alBuffer);
        
        setRoute(ROUTE_COMMAND);
        startSource(mAlCommandSource);
        
        return TRUE;
    }
    
    return FALSE;
}

bool AudioInterfaceOpenAL::playMotorCommand(MotorCommand command)
{    
    stopFxSource();
    
    if (mBufferQueue.size() > 0)
    {
        uint8_t *commandBuffer;
        
        ALuint buffer = mBufferQueue.front();
        mBufferQueue.pop();
        
        loadMotorCommandBuffer(&commandBuffer, command.leftValue, command.rightValue);
        loadAlBuffer(buffer, commandBuffer, mBufferSize * 2, COMMAND_FORMAT, DEFAULT_SAMPLE_RATE);
        queueBuffer(mAlCommandSource, buffer);
        
        setRoute(ROUTE_COMMAND);
        startSource(mAlCommandSource);
        
        return TRUE;
    }
    
    return FALSE;
}

bool AudioInterfaceOpenAL::playAuxCommand(AuxCommand command)
{
    stopFxSource();
    
    if (mBufferQueue.size() > 0)
    {
        uint8_t *commandBuffer;
        
        ALuint buffer = mBufferQueue.front();
        mBufferQueue.pop();
        
        loadAuxCommandBuffer(&commandBuffer, command.auxValue);
        loadAlBuffer(buffer, commandBuffer, mBufferSize, COMMAND_FORMAT, DEFAULT_SAMPLE_RATE);
        queueBuffer(mAlCommandSource, buffer);
        
        setRoute(ROUTE_COMMAND);
        startSource(mAlCommandSource);
        
        return TRUE;
    }
    
    return FALSE;
}

void AudioInterfaceOpenAL::reclaimBuffers()
{
    ALint bufferCount;
    ALuint buffer;
    alGetSourcei(mAlCommandSource, AL_BUFFERS_PROCESSED, &bufferCount);
    while (bufferCount--)
    {
        alSourceUnqueueBuffers(mAlCommandSource, 1, &buffer);
        mBufferQueue.push(buffer);
    }
}

void AudioInterfaceOpenAL::loadAlBuffer(ALuint alBuffer, uint8_t *buffer, uint32_t bufferSize, ALint format, ALint frequency)
{    
    alBufferData(alBuffer, format, buffer, bufferSize, frequency);
    free(buffer);
}

void AudioInterfaceOpenAL::queueBuffer(ALuint alSource, ALuint alBuffer)
{
    alSourceQueueBuffers(alSource, 1, &alBuffer);
}

void AudioInterfaceOpenAL::startSource(ALuint alSource)
{
    ALint state;
    alGetSourcei(alSource, AL_SOURCE_STATE, &state);
    
    if (state != AL_PLAYING)
        alSourcePlay(alSource);
}

void AudioInterfaceOpenAL::setRoute(AudioRoute route)
{
    
#if TARGET_OS_IPHONE
    
    UInt32 audioRouteOverride;
    
    switch (route) 
    {
        case ROUTE_FX:
            letCommandsFinish();
            audioRouteOverride = kAudioSessionOverrideAudioRoute_Speaker;
            mAudioRoute = ROUTE_FX;
            break;
            
        case ROUTE_COMMAND:
            audioRouteOverride = kAudioSessionOverrideAudioRoute_None;
            mAudioRoute = ROUTE_COMMAND;
            break;
            
        case ROUTE_NONE:
            // Shouldn't be set, just used as a default.
            break;
    }
    

    AudioSessionSetProperty(kAudioSessionProperty_OverrideAudioRoute, sizeof(audioRouteOverride), &audioRouteOverride);
#endif
}

void AudioInterfaceOpenAL::letCommandsFinish()
{
    ALint state;
    alGetSourcei(mAlCommandSource, AL_SOURCE_STATE, &state);
    while (state == AL_PLAYING)
    {
        usleep(500);
        alGetSourcei(mAlCommandSource, AL_SOURCE_STATE, &state);
    }
}

void AudioInterfaceOpenAL::stopFxSource()
{
    ALint state;
    alGetSourcei(mAlFxSource, AL_SOURCE_STATE, &state);
    
    if (state == AL_PLAYING)
        alSourceStop(mAlFxSource);
    
    alSourcei(mAlFxSource, AL_BUFFER, NULL);
}

void AudioInterfaceOpenAL::loadFxSource()
{
    alSourcei(mAlFxSource, AL_BUFFER, mAlFxBuffer);
}



