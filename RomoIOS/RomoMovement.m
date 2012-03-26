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
//  RomoMovement.m
//  An Objective C class with preset movement functions for controlling Romo, the Smart Phone Robot.
//   http://romotive.com
//
//  Created by Phu Nguyen.
//  Last Revision Date: 3/22/2012
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//

#import "RomoMovement.h"
#import <AudioToolbox/AudioToolbox.h>
#import <MediaPlayer/MediaPlayer.h>
#import <AVFoundation/AVFoundation.h>
#import "NativeManager.h"

@interface RomoMovement ()
{
    NSTimer *_moveTimer;

    MPMusicPlayerController *_mediaPlayer;
    float speakerVolume;
    
    NSInteger currentMoveStep;
    AVAudioPlayer *_AVPlayer;
    
    /* When the audio switches route, it has a lag so the first few commands need to be repeated after route switching */
    
    BOOL repeatCommandAfterFirstTime;
    NSTimer *_repeatTimer;
    uint8_t currentCmdLeft;
    uint8_t currentCmdRight;
    NSInteger repeatCount;
}
@property NSInteger currentMoveStep;

- (void)moveCurrentCommand:(NSInteger)queueStep;
- (void)moveQueueCallback:(NSTimer *)timer;
@end

@implementation RomoMovement
@synthesize isInQueue, moveQueue, currentMoveStep;
@synthesize isRunning, currentDirection, delegate;


/****************************************
 Core Motor Movements
 ****************************************/

- (void)moveLeftMotor:(uint8_t)leftValue andRight:(uint8_t)rightValue
{
    [self overrideToHeadphones:YES];
    [[NativeManager shared] playMotorCommandLeft:leftValue commandRight:rightValue];
}

- (void)moveAuxMotors:(uint8_t)auxValue
{
    [self overrideToHeadphones:YES];
    [[NativeManager shared] playAuxCommand:auxValue];
}

- (void)moveMotors:(NSInteger)motorIndex atSpeed:(NSInteger) speed direction:(NSInteger) direction
{
    UInt32 sessionCategory = kAudioSessionCategory_SoloAmbientSound;
    AudioSessionSetProperty(kAudioSessionProperty_AudioCategory,
                            sizeof(sessionCategory),
                            &sessionCategory);
    
    uint8_t leftValue = 0, rightValue = 0;
    uint8_t auxValue = 0;
    
    if (_mediaPlayer.volume < 1) _mediaPlayer.volume = 1;
    
    if ([_repeatTimer isValid]) {
        repeatCount = 0;
        [_repeatTimer invalidate];
        _repeatTimer = nil;
    }
    switch (motorIndex) 
    {
        case RMOutputDrive:
            switch (direction) {
                case RMDirectionForward:
                    //play forward sound
                    if (speed == RMSpeedFast) {
                        leftValue = 0xFF;
                        rightValue = 0xFF;
                    } else {
                        leftValue = 0xC0;
                        rightValue = 0xC0;
                    }
                    break;
                case RMDirectionRight:
                    //play right sound
                    if (speed == RMSpeedFast) {
                        leftValue = 0xFF;
                        rightValue = 0x00;
                    } else {
                        leftValue = 0xC0;
                        rightValue = 0x40;
                    }
                    break;
                case RMDirectionBack:
                    //play back sound
                    if (speed == RMSpeedFast) {
                        leftValue = 0x00;
                        rightValue = 0x00;
                    } else {
                        leftValue = 0x40;
                        rightValue = 0x40;
                    }
                    break;
                case RMDirectionLeft:
                    //play left sound
                    if (speed == RMSpeedFast) {
                        leftValue = 0x00;
                        rightValue = 0xFF;
                    } else {
                        leftValue = 0x40;
                        rightValue = 0xC0;
                    }
                    break;
                case RMDirectionForwardLeft:

                    leftValue = 0xC0;
                    rightValue = 0xFF;
                    break;
                case RMDirectionForwardRight:

                    leftValue = 0xFF;
                    rightValue = 0xC0;
                    break;
                case RMDirectionBackLeft:

                    leftValue = 0x40;
                    rightValue = 0x00;
                    break;
                case RMDirectionBackRight:

                    leftValue = 0x00;
                    rightValue = 0x40;
                    break;
                default:
                    leftValue = 0x80;
                    rightValue = 0x80; 
                    break;
            }
            break;
        case RMOutputAux1:
            switch (direction) {
                case RMDirectionForward:
                    auxValue = 0x10;
                    break;
                case RMDirectionBack:
                    auxValue = 0x20;
                    break;
                default:
                    auxValue = 0x00;
                    break;
            }
            break;
        case RMOutputAux2:
            switch (direction) {
                case RMDirectionForward:
                    auxValue = 0x04;
                    break;
                case RMDirectionBack:
                    auxValue = 0x08;
                    break;
                default:
                    auxValue = 0x00;
                    break;
            }
            break;
        case RMOutputAux3:
            switch (direction) {
                case RMDirectionForward:
                    auxValue = 0x01;
                    break;
                case RMDirectionBack:
                    auxValue = 0x02;
                    break;
                default:
                    auxValue = 0x00;
                    break;
            }
            break;
        case RMOutputAuxAll:
            switch (direction) {
                case RMDirectionForward:
                    auxValue = 0x15;
                    break;
                case RMDirectionBack:
                    auxValue = 0x2A;
                    break;
                default:
                    auxValue = 0x00;
                    break;
            }
            break;
    }
    
    if (motorIndex == RMOutputDrive) {
        //NSLog(@"left: %i right: %i", leftValue, rightValue);
        [[NativeManager shared] playMotorCommandLeft:leftValue commandRight:rightValue];
    } else {
        [[NativeManager shared] playAuxCommand:auxValue];
    }

    if(repeatCommandAfterFirstTime) {
        currentCmdLeft = leftValue;
        currentCmdRight = rightValue;
        
        repeatCount = 0;
        //if this is the first time after playing a sound outloud (switching audio routes)
        _repeatTimer = [NSTimer timerWithTimeInterval:0.1 target:self selector:@selector(repeatTimerCallback) userInfo:nil repeats:YES];
        [[NSRunLoop mainRunLoop] addTimer:_repeatTimer forMode:NSDefaultRunLoopMode];
    } 
}
- (void)stopMotors:(NSInteger) motorIndex
{
    switch (motorIndex) {
        case RMOutputDrive:
            [self moveMotors:RMOutputDrive atSpeed:RMSpeedFast direction:RMDirectionStop];
            [delegate didFinishMoving];
            break;
        case RMOutputAux1:
        case RMOutputAux2:
        case RMOutputAux3:
            [self moveMotors:RMOutputAuxAll atSpeed:RMSpeedFast direction:RMDirectionStop];
            break;
        default:
            [self stopAllMotors];
            break;
    }
    isRunning = NO;
}

- (void)stopAllMotors
{
    [self stopMotors:RMOutputDrive];
    [self stopMotors:RMOutputAux1];
}

/****************************************
 Core Motor Movements with Presets
 ****************************************/

- (void)moveRomoforTime:(float) time atSpeed:(NSInteger) speed direction:(NSInteger) direction
{
    if (isRunning) {
        [self stopMotors:RMOutputDrive];
        isRunning = NO;
    }
    
    [self moveMotors:RMOutputDrive atSpeed:speed direction:direction];
    isRunning = YES;
    
    if (time > 0 && !isInQueue) {
        _moveTimer = [NSTimer timerWithTimeInterval:time 
                                             target:self 
                                           selector:@selector(moveTimerCallback:) 
                                           userInfo:nil 
                                            repeats:NO];
        [[NSRunLoop currentRunLoop] addTimer:_moveTimer forMode: NSDefaultRunLoopMode];
    }
}

- (void)moveRomoforTime:(float)time withLeft:(uint8_t)leftValue andRight:(uint8_t)rightValue;
{
    if (isRunning) {
        [self stopMotors:RMOutputDrive];
        isRunning = NO;
    }
    
    [[NativeManager shared] playMotorCommandLeft:leftValue commandRight:rightValue];
    
    isRunning = YES;

    if (time > 0 && !isInQueue) {
        _moveTimer = [NSTimer timerWithTimeInterval:time 
                                             target:self 
                                           selector:@selector(moveTimerCallback:) 
                                           userInfo:nil 
                                            repeats:NO];
        [[NSRunLoop currentRunLoop] addTimer:_moveTimer forMode: NSDefaultRunLoopMode];
    }
}

- (void)moveAuxforTime:(float)time withAux1:(uint8_t)aux1Value aux2:(uint8_t)aux2Value aux3:(uint8_t)aux3Value
{
    switch (aux1Value) {
        case 0:
            aux1Value = 0x20;
            break;
        case 1:
            aux1Value = 0x00;
            break;
        case 2:
            aux1Value = 0x10;
            break;
    }
    switch (aux2Value) {
        case 0:
            aux1Value = 0x08;
            break;
        case 1:
            aux1Value = 0x00;
            break;
        case 2:
            aux1Value = 0x04;
            break;
    }
    switch (aux3Value) {
        case 0:
            aux1Value = 0x02;
            break;
        case 1:
            aux1Value = 0x00;
            break;
        case 2:
            aux1Value = 0x01;
            break;
    }
    
    [[NativeManager shared] playAuxCommand:aux1Value];
    usleep(200);
    [[NativeManager shared] playAuxCommand:aux2Value];
    usleep(200);
    [[NativeManager shared] playAuxCommand:aux3Value];
    
    if (time > 0 && !isInQueue) {
        _moveTimer = [NSTimer timerWithTimeInterval:time 
                                             target:self 
                                           selector:@selector(moveTimerCallback:) 
                                           userInfo:nil 
                                            repeats:NO];
        [[NSRunLoop currentRunLoop] addTimer:_moveTimer forMode: NSDefaultRunLoopMode];
    }
}

- (void)moveTimerCallback:(NSTimer *) timer
{
    if (isRunning) {
        isRunning = NO;
        [self stopAllMotors];
    }
}

/****************************************
 Repeat Timer Call back, for picking up move commands after route switching
 ****************************************/
- (void) repeatTimerCallback
{
    if (repeatCount < 5) {
        NSLog(@"%i, %i",currentCmdLeft,currentCmdRight);
        [[NativeManager shared] playMotorCommandLeft:currentCmdLeft commandRight:currentCmdRight];
        repeatCount++;
        
    } else {
        repeatCount = 0;
        repeatCommandAfterFirstTime = NO;
        
        if ([_repeatTimer isValid]) {
            [_repeatTimer invalidate];
            _repeatTimer = nil;
            [_repeatTimer release];
        }
    }
}


/****************************************
 Move Queues
 ****************************************/

- (void)startMoveQueue
{

    if (isRunning) {
        isRunning = NO;
        [self stopMotors:RMOutputDrive];
    }
    if ([_moveTimer isValid]) {
        [_moveTimer invalidate];
    }
    
    _moveTimer = nil;
    
    currentMoveStep = 0;
    [self moveCurrentCommand:currentMoveStep];
    isInQueue = YES;
}
- (void)stopMoveQueue
{
    [delegate didFinishMoveQueue:currentMoveStep inMoveQueue:moveQueue];
    
    isInQueue = NO;
    currentMoveStep = 0;
    moveQueue = nil;
    
    if ([_moveTimer isValid]) {
        [_moveTimer invalidate];
    }
    
    _moveTimer = nil;
    [_moveTimer release];
}

- (void)moveQueueCallback:(NSTimer *)timer
{
    if(!isInQueue) {
        return;
    }
    
    [delegate didFinishMoveQueueStep:currentMoveStep];
    isRunning = NO;
    
    currentMoveStep++;
    
    if (currentMoveStep > ([moveQueue count] - 1)) {
        //Expired stop
        [self stopMoveQueue];
    } else {
        //Continue with queue
        isInQueue = YES;
        isRunning = NO;
        [self moveCurrentCommand:currentMoveStep];
    }
    
}
- (void)moveCurrentCommand:(NSInteger)queueStep
{
    
    [delegate didBeginMoveQueueStep:queueStep];
    
    int mode = [[[moveQueue objectAtIndex:queueStep] objectForKey:@"mode"]  intValue];
    float timeInterval = [[[moveQueue objectAtIndex:queueStep] objectForKey:@"time"]  floatValue];
    
    if (mode == 0) {
        uint8_t leftValue =  [[[moveQueue objectAtIndex:queueStep] objectForKey:@"left"] intValue];
        uint8_t rightValue = [[[moveQueue objectAtIndex:queueStep] objectForKey:@"right"] intValue];
        //Move the motors
        [self moveLeftMotor:leftValue andRight:rightValue];
    } else {
        
        uint8_t aux1Value = [[[moveQueue objectAtIndex:queueStep] objectForKey:@"aux1"] intValue];
        uint8_t aux2Value = [[[moveQueue objectAtIndex:queueStep] objectForKey:@"aux2"] intValue];
        uint8_t aux3Value = [[[moveQueue objectAtIndex:queueStep] objectForKey:@"aux3"] intValue];
        
        [self moveAuxforTime:0 withAux1:aux1Value aux2:aux2Value aux3:aux3Value];
    }

    _moveTimer = [NSTimer timerWithTimeInterval:timeInterval target:self selector:@selector(moveQueueCallback:) userInfo:nil repeats:NO];
    isRunning = YES;
    
    [[NSRunLoop currentRunLoop] addTimer:_moveTimer forMode: NSDefaultRunLoopMode];
}

/****************************************
 Playing sounds
 ****************************************/

- (void)playSound:(NSString *)filename withLoops:(int)loops
{
    if ([_repeatTimer isValid]) {
        repeatCount = 0;
        [_repeatTimer invalidate];
        _repeatTimer = nil;
    }
    
    [self overrideToHeadphones:NO];
    
    repeatCommandAfterFirstTime = YES;
    
    if (speakerVolume == 1) {
        speakerVolume = _mediaPlayer.volume;
    } else {
        _mediaPlayer.volume = speakerVolume;
    }
    
    @autoreleasepool {
        
        NSArray *arrNameString = [filename componentsSeparatedByString:@"."];
        NSString *file = [NSString stringWithString:[arrNameString objectAtIndex:0]];
        NSString *ext;
        
        
        if ([arrNameString count] < 2) {
            ext = @"wav";
        } else {
            ext = [arrNameString objectAtIndex:1];
        }
        
        NSString *soundFilename = [NSString stringWithFormat:@"%@.%@",file,ext];
        
        if (loops < 1) {
            [[NativeManager shared] playSoundFile:soundFilename];
            
        } else {
                    
            NSURL *toneURLRef = [[NSBundle mainBundle] URLForResource:file withExtension:ext];
            NSError * err;
            if(_AVPlayer != nil) {
                [_AVPlayer release];
            }
            _AVPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:toneURLRef error:&err];
            
            _AVPlayer.numberOfLoops = loops;
            _AVPlayer.volume = 1;
            
            [_AVPlayer prepareToPlay];
            [_AVPlayer play];
        }
    }
}

- (void)stopAV
{
    [_AVPlayer stop];
    [_AVPlayer release];
    _AVPlayer = nil;
}

- (void)pauseAV
{
    [_AVPlayer pause];
}

- (void)playAV
{
    [self overrideToHeadphones:NO];
    [_AVPlayer play];
}

/****************************************
 Checks
 ****************************************/

- (BOOL)isPluggedIn
{
    UInt32 sessionCategory = kAudioSessionCategory_SoloAmbientSound;
    AudioSessionSetProperty(kAudioSessionProperty_AudioCategory,sizeof(sessionCategory),&sessionCategory);
    
    UInt32 routeSize = sizeof(CFStringRef);
    CFStringRef route;
    
    OSStatus error = AudioSessionGetProperty(kAudioSessionProperty_AudioRoute,&routeSize,&route);
    /* Known values of route:
     * "Headset"
     * "Headphone"
     * "Speaker"
     * "SpeakerAndMicrophone"
     * "HeadphonesAndMicrophone"
     * "HeadsetInOut"
     * "ReceiverAndMicrophone"
     * "Lineout"
     */
    if (!error && (route != NULL)) {
        NSString * routeStr = (__bridge NSString*)route;
        NSRange headphoneRange = [routeStr rangeOfString : @"Head"];
        if (headphoneRange.location != NSNotFound) return YES;
    }
    return NO;
}

- (void)overrideToHeadphones:(BOOL)useHeadPhones
{
    if (useHeadPhones) {
        
    } else {
        
        UInt32 sessionCategory = kAudioSessionCategory_PlayAndRecord;
        AudioSessionSetProperty(kAudioSessionProperty_AudioCategory,
                                sizeof(sessionCategory),
                                &sessionCategory);
        UInt32 audioRouteOverride = kAudioSessionOverrideAudioRoute_Speaker;         
        AudioSessionSetProperty (kAudioSessionProperty_OverrideAudioRoute,sizeof (audioRouteOverride),&audioRouteOverride);
        
    }
}


/****************************************
 Initialization
 ****************************************/

- (id)init 
{
    if (self = [super init]) {
        
        AudioSessionInitialize(NULL, NULL, NULL, NULL);
        UInt32 sessionCategory = kAudioSessionCategory_PlayAndRecord;
        OSStatus err = AudioSessionSetProperty(kAudioSessionProperty_AudioCategory,sizeof(sessionCategory),&sessionCategory);
        AudioSessionSetActive(TRUE);
        
        if (err) NSLog(@"AudioSessionSetProperty kAudioSessionProperty_AudioCategory failed: %@", err);
        
        isRunning = NO;
        
        _mediaPlayer.volume = speakerVolume = 1;
    }   
    
    return self;
}

- (void)turnUpSound
{
    _mediaPlayer.volume = 1;
}

- (void)shutdown
{
    [[NativeManager shared] shutdown];
    
}

@end
