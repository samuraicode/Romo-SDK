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
//  NativeManager.mm
//  An Objective-C++ class to interface with ObjectiveC apps.
//  http://romotive.com
//
//  Created by James Knight.
//  Modified by Phu Nguyen.
//  Last Revision Date: 3/22/2012
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//


#import "NativeManager.h"
#import "AudioManager.h"
#import "NativeInterface.h"
#include "ResourceManager.h"

@implementation NativeManager

static NativeManager *sharedInstance = nil;

+ (NativeManager *)shared
{
    @synchronized(sharedInstance)
    {
        if (!sharedInstance)
            sharedInstance = [[NativeManager alloc] init];
        
        return sharedInstance;
    }
}

- (id)init
{
    self = [super init];
    
    if (self)
    {
        _nativeInterface = new NativeInterface();
        _nativeInterface->initialize();
        _nativeInterface->setManager(self);
        std::string resourcePath = std::string([[[NSBundle mainBundle] resourcePath] cStringUsingEncoding:NSUTF8StringEncoding]);
        
        _nativeInterface->setResourcePath(resourcePath);
    }
    
    return self;
}

- (void)dealloc
{
    _nativeInterface->shutdown();
    [super dealloc];
    
}

- (void)shutdown
{
    _nativeInterface->shutdown();
}

- (void)playSoundFile:(NSString *)filename
{
    std::string stdFilename = std::string([filename cStringUsingEncoding:NSUTF8StringEncoding]);
    _nativeInterface->playSoundFile(stdFilename);
}

- (void)playMotorCommandLeft:(uint8_t)leftValue commandRight:(uint8_t)rightValue
{
    _nativeInterface->playMotorCommand(leftValue, rightValue);
}

- (void)playAuxCommand:(uint8_t)auxValue
{
    _nativeInterface->playAuxCommand(auxValue);
}


@end

// ------------------------------------------------------------------------------ //

void NativeInterface::initialize()
{
    mAudioManager = new AudioManager();
}

void NativeInterface::shutdown()
{
    if (mAudioManager)
    {
        delete mAudioManager;
        mAudioManager = NULL;
    }
}

void NativeInterface::setResourcePath(std::string resourcePath)
{
    ResourceManager::getInstance()->setResourcePath(resourcePath);
}

void NativeInterface::playSoundFile(std::string filename)
{
    mAudioManager->playSoundFile(filename);
}

void NativeInterface::playMotorCommand(uint8_t leftValue, uint8_t rightValue)
{
    mAudioManager->playMotorCommand(leftValue, rightValue);
}

void NativeInterface::playAuxCommand(uint8_t auxValue)
{
    mAudioManager->playAuxCommand(auxValue);
}



