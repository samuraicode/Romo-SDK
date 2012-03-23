//
//  ListVC.m
//  RomoSampleApp
//
//  Created by Phu Nguyen on 3/22/12.
//  Copyright (c) 2012 Romotive, Inc. All rights reserved.
//

#import "ListVC.h"

@interface ListVC ()

@end

@implementation ListVC
@synthesize tblCommands, btnRun;

/*****************************
 Button Events
 *****************************/

- (IBAction)btnAddTap:(id)sender
{
    //Present Modal Dialog for the Cmd Options
    
    if (cmdOptionsVC == nil) {
        cmdOptionsVC = [[CmdOptionsVC alloc] init];
        cmdOptionsVC.delegate = self;
    }
    [self presentModalViewController:cmdOptionsVC animated:YES];

}
- (IBAction)btnRunTap:(id)sender
{
    if (moveQueue.count == 0) {
        return;
    }
    if(romo.isRunning) {
        [romo stopAllMotors];
    }
    
    if (![romo isInQueue]) {
        [romo setMoveQueue:[NSArray arrayWithArray:moveQueue]];
        [romo startMoveQueue];
        [btnRun setTitle:@"Stop"];
        //Here's a present for you, I'm disabling the Run button until the weird timing bug is fixed
        //TODO: fix timing bug in MoveQueues
        [btnRun setEnabled:NO];
        
    } else {
        [romo stopMoveQueue];
        [btnRun setTitle:@"Run"];
    }
}


/*****************************
 CmdOptions Delegate
 *****************************/
- (void)didSaveWithOptions:(NSDictionary *)options
{
    if (options) {
        [moveQueue addObject:options];
    }
    
    [tblCommands reloadData];
}

/*****************************
 RomoMovement Delegate
 *****************************/

- (void)didBeginMoveQueueStep:(NSInteger)stepNumber
{
    [tblCommands selectRowAtIndexPath:[NSIndexPath indexPathForRow:stepNumber inSection:0] animated:YES scrollPosition:UITableViewScrollPositionNone];
}
- (void)didFinishMoveQueue:(NSInteger)completedSteps inMoveQueue:(NSArray *)moveQueue
{
    [btnRun setTitle:@"Run"];
    [btnRun setEnabled:YES];
    
}
- (void)didFinishMoveQueueStep:(NSInteger) stepNumber
{
    [tblCommands deselectRowAtIndexPath:[NSIndexPath indexPathForRow:stepNumber inSection:0] animated:YES];
}
- (void)didFinishMoving
{

}
/*****************************
 UITableDelegate & DataSource
 *****************************/

- (void) tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    
    if ([romo isRunning]) {
        [romo stopAllMotors];
        [romo stopMoveQueue];
    }
    
    if ([[[moveQueue objectAtIndex:indexPath.row] objectForKey:@"mode"] intValue] == 0) {
        [romo moveRomoforTime:[[[moveQueue objectAtIndex:indexPath.row] objectForKey:@"time"] floatValue] 
                     withLeft:[[[moveQueue objectAtIndex:indexPath.row] objectForKey:@"left"] floatValue] 
                     andRight:[[[moveQueue objectAtIndex:indexPath.row] objectForKey:@"right"] floatValue]];
    } else {
        
        [romo moveAuxforTime:[[[moveQueue objectAtIndex:indexPath.row] objectForKey:@"time"] floatValue]
                    withAux1:[[[moveQueue objectAtIndex:indexPath.row] objectForKey:@"aux1"] intValue]  
                        aux2:[[[moveQueue objectAtIndex:indexPath.row] objectForKey:@"aux2"] intValue]   
                        aux3:[[[moveQueue objectAtIndex:indexPath.row] objectForKey:@"aux3"] intValue] ];
    }
   
    
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return YES if you want the specified item to be editable.
    return YES;
}


- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        [moveQueue removeObjectAtIndex:indexPath.row];
        [tblCommands reloadData];
        
    }    
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath 
{
    static NSString * CellIdentifier = @"Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) 
    {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:CellIdentifier];
    }
    
    if ([[[moveQueue objectAtIndex:indexPath.row] objectForKey:@"mode"] intValue] == 0) {
        cell.textLabel.text = [NSString stringWithFormat:@"Left:%i Right:%i",
                               [[[moveQueue objectAtIndex:indexPath.row] objectForKey:@"left"] intValue],
                               [[[moveQueue objectAtIndex:indexPath.row] objectForKey:@"right"]intValue] ];
    } else {
        
        cell.textLabel.text = [NSString stringWithFormat:@"Aux1:%@ Aux2:%@, Aux3:%@",
                               [self stringForAuxValue:[[[moveQueue objectAtIndex:indexPath.row] objectForKey:@"aux1"] intValue]],
                               [self stringForAuxValue:[[[moveQueue objectAtIndex:indexPath.row] objectForKey:@"aux2"] intValue]],
                               [self stringForAuxValue:[[[moveQueue objectAtIndex:indexPath.row] objectForKey:@"aux3"] intValue]] ];
    }
    
    
    cell.detailTextLabel.text = [NSString stringWithFormat:@"Time:%.1f s",
                                 [[[moveQueue objectAtIndex:indexPath.row] objectForKey:@"time"] floatValue]];
    return cell;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section 
{
    return [moveQueue count];
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}



/*****************************
 View Events
 *****************************/
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        self.title = NSLocalizedString(@"List", @"List");
        self.tabBarItem.image = [UIImage imageNamed:@"RSTabsGears"];
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    romo = [[RomoMovement alloc] init];
    
    [romo setDelegate:self];
    [cmdOptionsVC setDelegate:self];
    
    //Load moveQueue
    moveQueue = [[NSMutableArray alloc] init];
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

/*****************************
 View Events
 *****************************/

- (NSString *)stringForAuxValue:(int)auxValue
{
    switch (auxValue) {
        case 0: {
            return @"R";
        }   break;
        case 1: {
            return @"S";
        }   break; 
        case 2: {
            return @"F";
        }   break;
    }
    return @"";
}


@end
