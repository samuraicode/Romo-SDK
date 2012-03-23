//
//  SlidersVC.h
//  RomoSampleApp
//
//  Created by Phu Nguyen on 3/22/12.
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//


#import <UIKit/UIKit.h>
#import "RomoMovement.h"

@interface SlidersVC : UIViewController
{
    RomoMovement *romo;
}

@property (nonatomic, retain) IBOutlet UISlider *leftSlider;
@property (nonatomic, retain) IBOutlet UISlider *rightSlider;
@property (nonatomic, retain) IBOutlet UIButton *btnStop;
@property (nonatomic, retain) IBOutlet UILabel  *lblLeftValue;
@property (nonatomic, retain) IBOutlet UILabel  *lblRightValue;

- (IBAction)sliderChanged:(UISlider *)slider;
- (IBAction)sliderTouchDown:(UISlider *)slider;
- (IBAction)sliderTouchUp:(UISlider *)slider;

- (IBAction)btnStopTap:(id)sender;

@end
