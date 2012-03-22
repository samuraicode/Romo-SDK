# Romo Library R1
- - -
by Romotive, Inc.

A collection of classes for moving and controlling Romo, the Smart Phone Robot. Please visit http://romotive.com/store to get your own smart phone robot.

![Romo Image](http://romotive.com/images/220/romo-white-neon-whitebg-sq.jpg)

With the Romo library, you can:

* Send move commands to drive the motors
* Send commands to drive the aux ports

# Getting Started
- - -

To get started with the library, download this entire Repository as a ZIP file (link above).

## iOS & XCode

Once you have all the files, drag the RomoLibrary folder into your project.

![Dragging the folder into Project]()

In your ViewController.h or AppDelegate.h file, import the RomoMovement.h file:

````ObjC
#import "RomoMovement.h"
````
To instantiate, in your viewDidLoad or applicationDidFinishLaunch function:

````ObjC
RomoMovement *romo = [[RomoMovement alloc] init];
````

(Optional) If you want an object (viewcontroller/appdelegate) to receive delegate actions from RomoMovement, declare the RomoMovementDelegate in your header file:

````ObjC
@interface ViewController : UIViewController <RomoMovementDelegate>
````
And make sure to set the Romo delegate to that object.

````ObjC
[romo setDelegate:self];
````

That's it, now you can call the many preset functions in RomoMovement.h that will make Romo move.

````ObjC
//Move Romo forward for 1.3 seconds at the fastest speed
[romo moveRomoforTime:1.3 atSpeed:RMSpeedFast direction:RMDirectionForward];
````

# How Romo Inteprets Your Move Comamnds
- - -
Each drive motor on Romo can receive 255 speeds. The value of the integer sent will also determine direction.

* 0 = Full power backward
* 1-127 = Partial power backward, depending on the integer passed
* 128 = Stop
* 129-254 = Partial power forward, depending on the integer passed
* 255 = Full power forward

In RomoMovement, there are preset speeds and directions, such as:

````ObjC
//Turn Romo left in place for 1.3 seconds at the halfway slow speed
[romo moveRomoforTime:1.3 atSpeed:RMSpeedSlow direction:RMDirectionLeft];
````

But you just as easily do:

````ObjC
//Turn Romo left in place for 1.3 seconds at the halfway slow speed
[romo moveRomoforTime:1.3 withLeft:64 andRight:192];
````

# Why do this instead of an SDK or static library?
- - -
We wanted our work to be easily portable and shareable with our Romo developers. Romo, in his current form, was built to be hackable. We believe that our developers will awesome things with Romo and contribute back to the Romo Library.

# Issues / Help
- - -
If you have any issues using the library, please let us know at support@romotive.com.

Please log bugs/feature requests in Git Issues for the project.