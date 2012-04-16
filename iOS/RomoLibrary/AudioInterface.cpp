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
//  AudioInterface.cpp
//  C++ class to manage generating and playing audio from either OpenSL (Android) or OpenAL (iPhone).
//  http://romotive.com
//
//  Created by James Knight.
//  Last Revision Date: 2/28/2012
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//


#include "AudioInterface.h"
#ifdef ANDROID
#include "AudioInterfaceOpenSL.h"
#elif defined (__APPLE__)
#include "AudioInterfaceOpenAL.h"
#endif

AudioInterface *AudioInterface::create()
{
    AudioInterface *interface = NULL;
    
    #ifdef ANDROID
    interface = new AudioInterfaceOpenSL();
    #elif defined (__APPLE__)
    interface = new AudioInterfaceOpenAL();
    #endif
    
    return interface;
}

AudioInterface::AudioInterface()
{
    uint8_t bitsPerMessage = BITS_PER_ADDRESS + BITS_PER_VALUE + BIT_PARITY;
    mBufferSize = ((bitsPerMessage * SAMPLES_PER_BIT) + (DEFAULT_LEAD_SIZE * 2)) * BYTES_PER_SAMPLE;
}

void AudioInterface::loadMotorCommandBuffer(uint8_t **buffer, uint8_t leftValue, uint8_t rightValue)
{
    *buffer = (uint8_t *) malloc(mBufferSize * 2);
    memset(*buffer, SIGNAL_LOW, mBufferSize * 2);
    
    uint32_t bufferIndex = 0;
    loadCommandBuffer(*buffer, &bufferIndex, ADDRESS_LEFT, leftValue);
    loadCommandBuffer(*buffer, &bufferIndex, ADDRESS_RIGHT, rightValue);
}

void AudioInterface::loadAuxCommandBuffer(uint8_t **buffer, uint8_t auxValue)
{
    *buffer = (uint8_t *) malloc(mBufferSize);
    memset(*buffer, SIGNAL_LOW, mBufferSize);
    
    uint32_t bufferIndex = 0;
    loadCommandBuffer(*buffer, &bufferIndex, ADDRESS_AUX, auxValue);
}

void AudioInterface::loadCommandBuffer(uint8_t *buffer, uint32_t *bufferIndex, uint8_t address, uint8_t value)
{
    loadLeadSample(buffer, bufferIndex);
    
    uint8_t currentBit;
    uint8_t normalizedValue;
    
    uint16_t i;
    uint16_t highBitCount = 0;
    
    i = BITS_PER_ADDRESS;
    while (i != 0)
    {
        i--;
        currentBit = (address & (0x1 << i)) >> i;
        highBitCount += currentBit;
        normalizedValue = (currentBit) ? SIGNAL_HIGH : SIGNAL_LOW;
        loadBit(buffer, bufferIndex, normalizedValue);
    }
    
    i = BITS_PER_VALUE;
    while (i != 0)
    {
        i--;
        currentBit = (value & (0x1 << i)) >> i;
        highBitCount += currentBit;
        normalizedValue = (currentBit) ? SIGNAL_HIGH : SIGNAL_LOW;
        loadBit(buffer, bufferIndex, normalizedValue);
    }
    
    // Parity bit:
    uint8_t parityBit = highBitCount & 0x1;
    normalizedValue = (parityBit) ? SIGNAL_HIGH : SIGNAL_LOW;
    
    loadBit(buffer, bufferIndex, normalizedValue);
    
    loadLeadSample(buffer, bufferIndex);
}

void AudioInterface::loadCommandBuffer(uint8_t *buffer, uint32_t *bufferIndex, uint8_t address, uint8_t value, uint32_t leadSize)
{
    loadLeadSample(buffer, bufferIndex, leadSize);
    
    uint8_t currentBit;
    uint8_t normalizedValue;
    
    uint16_t i;
    uint16_t highBitCount = 0;
    
    i = BITS_PER_ADDRESS;
    while (i != 0)
    {
        i--;
        currentBit = (address & (0x1 << i)) >> i;
        highBitCount += currentBit;
        normalizedValue = (currentBit) ? SIGNAL_HIGH : SIGNAL_LOW;
        loadBit(buffer, bufferIndex, normalizedValue);
    }
    
    i = BITS_PER_VALUE;
    while (i != 0)
    {
        i--;
        currentBit = (value & (0x1 << i)) >> i;
        highBitCount += currentBit;
        normalizedValue = (currentBit) ? SIGNAL_HIGH : SIGNAL_LOW;
        loadBit(buffer, bufferIndex, normalizedValue);
    }
    
    // Parity bit:
    uint8_t parityBit = highBitCount & 0x1;
    normalizedValue = (parityBit) ? SIGNAL_HIGH : SIGNAL_LOW;
    
    loadBit(buffer, bufferIndex, normalizedValue);
    
    loadLeadSample(buffer, bufferIndex, leadSize);
}

void AudioInterface::loadLeadSample(uint8_t *buffer, uint32_t *index)
{
    loadLeadSample(buffer, index, DEFAULT_LEAD_SIZE);
}


void AudioInterface::loadLeadSample(uint8_t *buffer, uint32_t *index, uint32_t leadSize)
{
    uint32_t i = *index;
    
    u_int sampleCount = leadSize * BYTES_PER_SAMPLE;
    memset(buffer + i, LEAD_SIGNAL, sampleCount);
    
    *index += sampleCount;
}

void AudioInterface::loadBit(uint8_t *buffer, uint32_t *index, uint8_t value)
{
    int i = *index;
    
    buffer[i + 0]  = SIGNAL_HIGH;
    buffer[i + 1]  = value;
    buffer[i + 2]  = SIGNAL_HIGH;
    buffer[i + 3]  = value;
    buffer[i + 4]  = SIGNAL_HIGH;
    buffer[i + 5]  = value;
    buffer[i + 6]  = SIGNAL_HIGH;
    buffer[i + 7]  = value;
    buffer[i + 8]  = SIGNAL_LOW;
    buffer[i + 9]  = value;
    buffer[i + 10] = SIGNAL_LOW;
    buffer[i + 11] = value;
    buffer[i + 12] = SIGNAL_LOW;
    buffer[i + 13] = value;
    buffer[i + 14] = SIGNAL_LOW;
    buffer[i + 15] = value;
    
    *index += 16;
}