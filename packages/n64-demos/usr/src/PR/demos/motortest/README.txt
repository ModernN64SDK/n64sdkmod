------------------------------------------------------------------
Rumble Pak Programming Manual                                     
------------------------------------------------------------------


1.  Introduction

There are two ways to control the Rumble Pak: by operating the 
motor and by stopping the motor.   And there are three processes 
needed to control the motor: 'prepare'  'move' and 'stop.'   
Accordingly, these three functions are available to control the 
Rumble Pak.


2.  Basic usage

---- Initializing the Controller ----
It is first necessary to initialize the Controller, and to confirm
that the Controller is inserted in the Controller port.  The 
method is the same as for a normal Controller; call the 
osContInit() function, and the osContStartQuery() and 
osContGetQuery functions.

---- Initializing the Rumble Pak ----
Once the Controller has been properly connected and recognized, it
is necessary to initialize the Rumble Pak inserted in that 
Controller port.  The function used for this is osMotorInit().
At this time it is necessary to confirm the value returned by the 
function to check whether the Rumble Pak has been inserted 
properly.  When the error code is 0 that means the Rumble Pak is 
inserted properly.  The error code PFS_ERR_NOPACK means nothing is
inserted in the Controller.  PFS_ERR_DEVICE means some device 
other than the Rumble Pak is inserted in the Controller.  If some 
other error is returned, the Rumble Pak is either inserted 
incorrectly, or there is some abnormality with the Controller or 
the Rumble Pak.

---- Stopping and starting the Rumble Pak motor ----
To start the Rumble Pak motor, call the function osMotorStart().  
If the function returns the value 0, then the motor has 
successfully started and there are no problems.  If a nonzero 
value is returned, then the motor cannot be started for some 
reason regarding PIf and transmission errors.  In this case, 
please call the osMotorStart() function again.
To stop the Rumble Pak motor, call the function osMotorStop().  
The return values have the same meanings as with osMotorStart().

One point of caution: It is possible for the Rumble Pak and the 
connector to develop a poor connection during operation.  When 
this occurs, there is no way with software to determine whether 
the Rumble Pak is operating correctly.  In other words, it is 
possible to make a successful call to osMotorStart() (or to 
osMotorStop() ), when in actuality the Rumble Pak has not begun to
move (or has not stopped).

To avoid a situation where the Rumble Pak might continue to run 
without stopping during a game, make three successive calls to 
osMotorStop() at around 1 fr. intervals at places where the Rumble
Pak ought to stop.

The Rumble Pak must be stopped during pauses and when reset is 
pushed.   In these situations, call osMototInit() once and then 
make three successive calls to osMotorStop() at around 1 fr. 
intervals.  Please refer to the programming notes for the reason 
for this.  The demo sample does not incorporate this ability, so 
please deal with it on the user side.

3.  Explanation of Rumble Pak functions



NAME
    osMotorInit   ----  initialize Rumble Pak function
    osMotorStart  ---  start motor
    osMotorStop  ---   stop motor


SYNOPSIS
    #include "motor.h"

    s32  osMotorInit(OSMesgQueue *siMessegeQ, OSPfs *pfs, int 
	 channel);
    s32  osMotorStart(OSPfs *pfs);
    s32  osMotorStop(OSPfs *pfs);


DESCRIPTION

osMotorInit() is the Rumble Pak initialization function.  Its 
arguments are the SI message queue, the OSPfs structure, and the 
Controller channel number.  Make sure to call this function first.
The error codes returned by the function are PFS_ERR_NOPACK, 
PFS_ERR_CONTRFAIL, and PFS_ERR_DEVICE.

PFS_ERR_NOPACK indicates that nothing is inserted in the 
Controller.

PFS_ERR_CONTRFAIL indicates that there has been a data transfer 
failure with the Controller.  When this error code is returned, 
either the Controller is not connected properly, or there is some 
abnormality with the Controller connector. 

PFS_ERR_DEVICE  indicates some device other than the Rumble Pak is
inserted in the Controller.

osMotorStart() is the function that starts the motor.  Its 
argument is the OSPfs structure.  When this function is called, 
the motor starts to move.  Internally, it merely writes data for 
the Controller Pak.  For the sake of speed, other functions like a
status check are not performed.  For this reason, if the Rumble 
Pak and Controller have been taken out and put back in, then it 
will not operate unless osMotorInit() is called again.

If PFS_ERR_CONTRFAIL is returned as the error code, then there has
been a transmission failure.  In other words, the motor has not 
started up.  Since errors during transmission can occur, it is 
necessary to confirm with the error codes whether actions have 
proceeded correctly.

OsMotorStop() is the function that stops the motor.   Its argument
is the OSPfs structure.  When this function is called, the motor 
stops moving.

Internally, this function performs the same processes as 
osMotorStart().


4.  The Sample Program

Sample programs with which to confirm Rumble Pak actions are 
included in the Rumble Pak library archive.  The procedure for 
manipulating these sample programs is described below:

The A button turns the motor ON, while the B button turns it OFF.  
Pushing the Z trigger changes the strength of the vibrations 
according to the extent to which the Control Stick is pulled down.
	

5.  Troubleshooting

Q1)  How do you change the strength of the vibrations?

A1) There are only two ways to control the motor: keep it running 
or turn it off.   Thus, in order to achieve the right vibration 
strength, the application program must cycle between on and off 
with the appropriate periodicity.  For a specific example, please 
refer to the sample programs.

Q2) The Rumble Pak was pulled out and put back in during a game, 
and now it does not work right.

A2) If the Rumble Pak is reinserted, then it will not operate 
properly unless the function osMotorInit() is called again.  
However, repeatedly calling osMotorInit() will lower performance.  
For this reason, please make some effort to warn the user not to 
remove the Rumble Pak during a game, for example placing a warning 
in the game software directions, or displaying it on the game screen.

Q3)  Something strange happened to the data read from the 
Controller.

A3)  Did you call a Rumble Pak function between the 
osContStartQuery() function and the osContGetQuery() function?  
When a Rumble Pak function is called while another thread is 
accessing SI, it will act strangely.  Some kind of exclusive 
process is necessary when using a Rumble Pak function together 
with a Controller read.