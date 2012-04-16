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
//  NativeManager.h
//  An Objective-C++ class to interface with ObjectiveC apps.
//  http://romotive.com
//
//  Created by James Knight.
//  Modified by Phu Nguyen.
//  Last Revision Date: 3/22/2012
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//
#import <Foundation/Foundation.h>

@class NativeManager;

struct NativeInterface;

typedef struct NativeInterface NativeInterface;

@interface NativeManager : NSObject
{
    NativeInterface        *_nativeInterface;
}

+ (NativeManager *)shared;

- (id)init;
- (void)shutdown;

- (void)playSoundFile:(NSString *)filename;
- (void)playMotorCommandLeft:(uint8_t)leftValue commandRight:(uint8_t)rightValue;
- (void)playAuxCommand:(uint8_t)auxValue;


@end
