//
//  CmdOptionsVC.h
//  RomoSampleApp
//
//  Created by Phu Nguyen on 3/22/12.
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//


#import <UIKit/UIKit.h>

@protocol CmdOptionsDelegate
- (void)didSaveWithOptions:(NSDictionary *)options;
@end

@interface CmdOptionsVC : UIViewController
{
    id<CmdOptionsDelegate> delegate;
}

@property (nonatomic, retain) id<CmdOptionsDelegate> delegate;
@property (nonatomic, retain) IBOutlet UISlider *durationSlider;
@property (nonatomic, retain) IBOutlet UILabel *lblDuration;
@property (nonatomic, retain) IBOutlet UISegmentedControl *outputSegCntrl;
@property (nonatomic, retain) IBOutletCollection(UISlider) NSArray *auxSliders;
@property (nonatomic, retain) IBOutletCollection(UILabel) NSArray *auxSlidersLabels;
@property (nonatomic, retain) IBOutletCollection(UISlider) NSArray *driveSliders;
@property (nonatomic, retain) IBOutletCollection(UILabel) NSArray *driveSlidersLabels;
@property (nonatomic, retain) IBOutlet UIView *driveView;
@property (nonatomic, retain) IBOutlet UIView *auxView;

- (IBAction)durationSliderChanged:(id)sender;
- (IBAction)driveMotorSlidersChanged:(UISlider *)sender;
- (IBAction)auxSlidersChanged:(UISlider *)sender;
- (IBAction)outputSegCntrlChanged:(id)sender;

- (IBAction)btnSaveTap:(id)sender;
- (IBAction)btnCancelTap:(id)sender;

@end
