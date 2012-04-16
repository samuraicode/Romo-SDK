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
//  RomoMovement.h
//  An Objective C class with preset movement functions for controlling Romo, the Smart Phone Robot.
//   http://romotive.com
//
//  Created by Phu Nguyen.
//  Last Revision Date: 3/22/2012
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol RomoMovementDelegate
@optional
- (void)didBeginMoveQueueStep:(NSInteger)stepNumber;
- (void)didFinishMoveQueue:(NSInteger)completedSteps inMoveQueue:(NSArray *)moveQueue;
- (void)didFinishMoveQueueStep:(NSInteger) stepNumber;
- (void)didFinishMoving;
@end

@interface RomoMovement : NSObject
{
    id<RomoMovementDelegate> delegate;
    NSArray *moveQueue;
    
}
@property BOOL isRunning;
@property BOOL isInQueue;
@property NSInteger currentDirection;
@property (nonatomic,retain) NSArray *moveQueue;
@property (nonatomic,retain) id<RomoMovementDelegate> delegate;

/****************************************
 Check to see if Romo is plugged in
 ****************************************/

- (BOOL)isPluggedIn;

/****************************************
 Check to see if Romo is plugged in
 ****************************************/

- (void)overrideToHeadphones:(BOOL)useHeadPhones;

/****************************************
 Core Motor Movements with Presets
 ****************************************/

- (void)moveMotors:(NSInteger)motorIndex atSpeed:(NSInteger)presetSpeed direction:(NSInteger)direction;       
- (void)stopMotors:(NSInteger)motorIndex;
- (void)stopAllMotors;

/****************************************
 The main movement function for Romo, 
 each motor value can be anything from 0 (full backwards) to 128 (stop) to 255 (full forward)
 ****************************************/

- (void)moveLeftMotor:(uint8_t)leftValue andRight:(uint8_t)rightValue;  
- (void)moveAuxMotors:(uint8_t)auxValue;

/****************************************
 Calculated movements
 ****************************************/

- (void)moveRomoforTime:(float)time atSpeed:(NSInteger)presetSpeed direction:(NSInteger)direction;
- (void)moveRomoforTime:(float)time withLeft:(uint8_t)leftValue andRight:(uint8_t)rightValue;
- (void)moveAuxforTime:(float)time withAux1:(uint8_t)aux1Value aux2:(uint8_t)aux2Value aux3:(uint8_t)aux3Value;

/****************************************
 Movement Queue
 ****************************************/

- (void)startMoveQueue;
- (void)stopMoveQueue;

/****************************************
 Playing sound file
 ****************************************/
- (void)playSound:(NSString *)filename withLoops:(NSInteger)loops;
- (void)stopAV;
- (void)pauseAV;
- (void)playAV;

/****************************************
 Deactivate
 ****************************************/
- (void)shutdown;

@end

/****************************************
 Romo Presets
 ****************************************/

typedef enum
{
    RMOutputDrive =     10,
    RMOutputAux1 =      11,
    RMOutputAux2 =      22,
    RMOutputAux3 =      33,
    RMOutputAuxAll =    44
    
}RMPresetOutput;

typedef enum
{
    RMDirectionForward =        111,
    RMDirectionBack =           100,
    RMDirectionLeft =           110,
    RMDirectionRight =          101,
    RMDirectionForwardLeft =    112,
    RMDirectionForwardRight =   121,
    RMDirectionBackLeft =       102,
    RMDirectionBackRight =      120,
    RMDirectionStop =           999
}RMPresetDirection;

typedef enum
{
    RMSpeedSlow = 1,
    RMSpeedFast = 2
}RMPresetSpeed;

