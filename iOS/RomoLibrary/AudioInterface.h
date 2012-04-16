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
//  AudioInterface.h
//  C++ class to manage generating and playing audio from either OpenSL (Android) or OpenAL (iPhone).
//  http://romotive.com
//
//  Created by James Knight.
//  Last Revision Date: 2/28/2012
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//


#ifndef AUDIOINTERFACE_H
#define AUDIOINTERFACE_H

#include <string>
#include "RomoInclude.h"

struct MotorCommand {
    uint8_t leftValue;
    uint8_t rightValue;
};

struct AuxCommand {
    uint8_t auxValue;
};

class AudioInterface
{
public:
    static const uint8_t  ADDRESS_LEFT       = 1;
    static const uint8_t  ADDRESS_RIGHT      = 2;
    static const uint8_t  ADDRESS_AUX        = 3;
    
    static const uint32_t DEFAULT_SAMPLE_RATE  = 8000;
    static const uint8_t  DEFAULT_LEAD_SIZE  = 48;
    
    static const uint8_t  SAMPLES_PER_BIT    = 8;
    static const uint8_t  BYTES_PER_SAMPLE   = 2;
    
    static const uint8_t  BITS_PER_ADDRESS   = 3;
    static const uint8_t  BITS_PER_VALUE     = 8;
    static const uint8_t  BIT_PARITY         = 1;
    
    static const uint8_t  SIGNAL_HIGH        = 0xFF;
    static const uint8_t  SIGNAL_BASE        = 0x80;
    static const uint8_t  SIGNAL_LOW         = 0x00;
    
    static const uint8_t  LEAD_SIGNAL        = SIGNAL_LOW;
    
    static const uint8_t  BUFFER_COUNT       = 10;
    static const uint8_t  SOURCE_COUNT       = 1;
    
    static AudioInterface *create();
    
    AudioInterface();
    virtual ~AudioInterface() {};
    
    virtual bool playCommand(uint8_t address, uint8_t command, uint32_t sampleRate, uint32_t leadCount)  = 0;
    
    virtual bool playSoundFile(std::string filename)            = 0;
    virtual bool playMotorCommand(MotorCommand command)         = 0;
    virtual bool playAuxCommand(AuxCommand command)             = 0;
    
    virtual void reclaimBuffers() = 0;
    
protected:
    uint32_t mBufferSize;
    
    void loadMotorCommandBuffer(uint8_t **buffer, uint8_t leftValue, uint8_t rightValue);
    void loadAuxCommandBuffer(uint8_t **buffer, uint8_t auxValue);
    
    void loadCommandBuffer(uint8_t *buffer, uint32_t *bufferIndex, uint8_t address, uint8_t value);
    void loadCommandBuffer(uint8_t *buffer, uint32_t *bufferIndex, uint8_t address, uint8_t value, uint32_t leadSize);
    
    void loadLeadSample(uint8_t *buffer, uint32_t *bufferIndex);
    void loadLeadSample(uint8_t *buffer, uint32_t *bufferIndex, uint32_t leadSize);
    
    void loadBit(uint8_t *buffer, uint32_t *index, uint8_t value);
};

#endif
