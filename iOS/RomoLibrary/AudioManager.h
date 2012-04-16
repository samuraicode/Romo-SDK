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
//  AudioManager.h
//  C++ class to handle all Audio Thread commands.
//  http://romotive.com
//
//  Created by James Knight.
//  Last Revision Date: 1/28/2012
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//

#ifndef AUDIOMANAGER_H
#define	AUDIOMANAGER_H

#include "RomoInclude.h"
#include <string>
    
class AudioThread;        // Production

class AudioManager 
{
public:
    AudioManager();
    virtual ~AudioManager();
    
    void playSoundFile(std::string filename);
    void playMotorCommand(uint8_t leftValue, uint8_t rightValue);
    void playAuxCommand(uint8_t auxValue);
    
private:
    AudioThread *mAudioThread;        // Production
};

#endif	/* AUDIOMANAGER_H */

