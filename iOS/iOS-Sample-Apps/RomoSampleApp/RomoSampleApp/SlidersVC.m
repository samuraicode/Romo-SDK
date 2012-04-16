//
//  SlidersVC.m
//  RomoSampleApp
//
//  Created by Phu Nguyen on 3/22/12.
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//

#import "SlidersVC.h"

@interface SlidersVC ()
{
    BOOL touchActive;
}

@end

@implementation SlidersVC
@synthesize leftSlider, rightSlider, btnStop, lblLeftValue, lblRightValue;

/*****************************
 Button Events
 *****************************/

- (IBAction)btnStopTap:(id)sender
{
    [romo stopAllMotors];
    [leftSlider setValue:128];
    [rightSlider setValue:128];
    [self sliderChanged:leftSlider];
    [self sliderChanged:rightSlider];
    
    
    btnStop.hidden = YES;
}

/*****************************
 Slider Events
 *****************************/

- (IBAction)sliderTouchDown:(UISlider *)slider
{
    touchActive = YES;
    btnStop.hidden = NO;
}

- (IBAction)sliderTouchUp:(UISlider *)slider
{
    touchActive = NO;
    //Stop the motors
}

- (IBAction)sliderChanged:(UISlider *)slider
{
    //Update labels
    if (slider.tag == 12) {
        lblRightValue.text = [NSString stringWithFormat:@"%i",(int)(slider.value+0.5f)];
    } else {
        lblLeftValue.text = [NSString stringWithFormat:@"%i",(int)(slider.value+0.5f)];
    }
    [romo moveLeftMotor:leftSlider.value andRight:rightSlider.value];
}

/*****************************
 View Events
 *****************************/

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        self.title = NSLocalizedString(@"Sliders", @"Sliders");
        self.tabBarItem.image = [UIImage imageNamed:@"RSTabsAdjust"];
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    //Arrange the sliders vertically
    CGAffineTransform rotate = CGAffineTransformMakeRotation(-M_PI/2);
    leftSlider.transform = rotate;
    rightSlider.transform = rotate;
    
    //leftSlider.minimumTrackTintColor = [UIColor clearColor];
    //rightSlider.minimumTrackTintColor = [UIColor clearColor];
    
    leftSlider.frame = CGRectMake(30, 40, 40, 320);
    rightSlider.frame = CGRectMake(250, 40, 40, 320);
    
    //Clear the label values
    [lblLeftValue setText:@"128"];
    [lblRightValue setText:@"128"];
    
    btnStop.hidden = YES;
    
    //Initiate Romo
    romo = [[RomoMovement alloc] init];
    
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

@end
