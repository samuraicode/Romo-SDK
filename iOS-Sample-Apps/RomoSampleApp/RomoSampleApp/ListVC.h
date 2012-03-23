//
//  ListVC.h
//  RomoSampleApp
//
//  Created by Phu Nguyen on 3/22/12.
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//


#import <UIKit/UIKit.h>
#import "CmdOptionsVC.h"
#import "RomoMovement.h"

@interface ListVC : UIViewController
<RomoMovementDelegate, CmdOptionsDelegate, UITableViewDelegate, UITableViewDataSource>
{
    CmdOptionsVC *cmdOptionsVC;
    RomoMovement *romo;
    NSMutableArray *moveQueue;
}
@property (nonatomic, retain) IBOutlet UITableView *tblCommands;
@property (nonatomic, retain) IBOutlet UIBarButtonItem *btnRun;

- (IBAction)btnAddTap:(id)sender;
- (IBAction)btnRunTap:(id)sender;


@end
