//
//  CmdOptionsVC.m
//  RomoSampleApp
//
//  Created by Phu Nguyen on 3/22/12.
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//


#import "CmdOptionsVC.h"

@interface CmdOptionsVC ()
{
    NSInteger commandMode;
}
@end

@implementation CmdOptionsVC
@synthesize delegate;
@synthesize durationSlider,lblDuration,outputSegCntrl,auxSliders,auxSlidersLabels,driveSliders, driveSlidersLabels, driveView, auxView;


/*****************************
 Button methods
 *****************************/
- (IBAction)btnSaveTap:(id)sender
{
    NSDictionary *cmdInfo;
    
    if (commandMode==0) {
        
        UISlider *leftSlider = [driveSliders objectAtIndex:0];
        UISlider *rightSlider = [driveSliders objectAtIndex:1];
        
        cmdInfo = [NSDictionary dictionaryWithObjectsAndKeys:
                   [NSNumber numberWithInt:commandMode], @"mode",
                   [NSNumber numberWithFloat:(durationSlider.value/10)], @"time",
                   [NSNumber numberWithInt:leftSlider.value], @"left",
                   [NSNumber numberWithInt:rightSlider.value], @"right", nil];
    } else {
        
        UISlider *aux1Slider = [auxSliders objectAtIndex:0];
        UISlider *aux2Slider = [auxSliders objectAtIndex:1];
        UISlider *aux3Slider = [auxSliders objectAtIndex:2];
        
        cmdInfo = [NSDictionary dictionaryWithObjectsAndKeys:
                   [NSNumber numberWithInt:commandMode], @"mode",
                   [NSNumber numberWithFloat:(durationSlider.value/10)], @"time",
                   [NSNumber numberWithInt:aux1Slider.value], @"aux1",
                   [NSNumber numberWithInt:aux2Slider.value], @"aux2",
                   [NSNumber numberWithInt:aux3Slider.value], @"aux3", nil];
    }
    
    [delegate didSaveWithOptions:cmdInfo];
    [self dismissModalViewControllerAnimated:YES];
}
- (IBAction)btnCancelTap:(id)sender
{
    [self dismissModalViewControllerAnimated:YES];
}

/*****************************
 Slider methods
 *****************************/

- (IBAction)durationSliderChanged:(id)sender
{
    //Update label
    lblDuration.text = [NSString stringWithFormat:@"Time Duration: %.1f",(durationSlider.value/10)];
}
- (IBAction)driveMotorSlidersChanged:(UISlider *)sender
{
    
    if (sender.tag == 11) {
        [[driveSlidersLabels objectAtIndex:0] 
         setText:[NSString stringWithFormat:@"Left Motor Command: %i",(int)(sender.value+0.5f)]];
    } else if (sender.tag == 12) {
        [[driveSlidersLabels objectAtIndex:1] setText:[NSString stringWithFormat:@"Right Motor Command: %i",(int)(sender.value+0.5f)]];
    }
}
- (IBAction)auxSlidersChanged:(UISlider *)sender
{
    NSString *readableCommand;
    
    if (sender.value == 0) {
        readableCommand = @"Back";
    } else if (sender.value == 1) {
        readableCommand = @"Stop";
    } else {
        readableCommand = @"Forward";
    }
    
    switch (sender.tag) {
        case 21: {
            [[auxSlidersLabels objectAtIndex:0] setText:[NSString stringWithFormat:@"AUX1 Command: %@",readableCommand]];
        }   break;
        case 22: {
            [[auxSlidersLabels objectAtIndex:1] setText:[NSString stringWithFormat:@"AUX2 Command: %@",readableCommand]];
        }   break;
        case 23: {
            [[auxSlidersLabels objectAtIndex:2] setText:[NSString stringWithFormat:@"AUX3 Command: %@",readableCommand]];
        }   break;
    }
}

/*****************************
 Segmented Control methods
 *****************************/

- (IBAction)outputSegCntrlChanged:(id)sender
{
    commandMode = outputSegCntrl.selectedSegmentIndex;
    
    if (outputSegCntrl.selectedSegmentIndex == 0) {
        //Show the drive view
        auxView.hidden = YES;
        driveView.hidden = NO; 
    } else {
        //Show the aux view
        auxView.hidden = NO;
        driveView.hidden = YES;  
    }
}

/*****************************
 View Events
 *****************************/

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    [outputSegCntrl setSelectedSegmentIndex:0];
    commandMode = 0;
    driveView.hidden = NO;
    
    auxView.hidden = YES;
    auxView.frame = CGRectMake(0, 128, 320, 216);
    [self.view addSubview:auxView];
    
    
}

- (void)viewDidAppear:(BOOL)animated
{
    //reset all uisliders to original
    for (UISlider *slider in auxSliders) {
        slider.value = 1;
        [self auxSlidersChanged:slider];
    }
    
    
    //reset all uisliders to original
    for (UISlider *slider in driveSliders) {
        slider.value = 128;
        [self driveMotorSlidersChanged:slider];
    }
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
