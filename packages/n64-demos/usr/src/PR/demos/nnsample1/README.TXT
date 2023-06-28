------------------------------------------------------------------
		NINTENDO64 TECHNICAL SUPPORT CENTER 
		    NINTENDO64 SAMPLE PROGRAM 1
		Copyright (C) 1997, NINTENDO Co,Ltd.
------------------------------------------------------------------


[0] Contents

	[1] Introduction
	[2] Using make
	[3] Control operations
	[4] Explanation of the screen
	[5] Copyright and reuse
	[6] History

[1] Introduction

This is a Scheduler sample program.  The scheduler used in this 
sample program was developed by the NINTENDO64 TECHNICAL SUPPORT 
CENTER, and it incorporates the general scheduler functions.  
This sample program contains imporvements and additions to the 
general scheduler that make it more efficient.

[2] Using make

Whether using the Indy emulator board or PARTNER-N64, please 
execute make without any additions.

[3] Control operations

	Control Stick        	Up/Down to rotate depth-wise, 
                            	Right/Left to rotate in the X-Y 
				plane.
	Control Pad Up    	Bring viewpoint toward origin.
	Control Pad Down     	Recede viewpoint from origin.  
	Control Pad Right    	Not used.
	Control Pad Left     	Not used.

	A button             	Shoot bullet.
	B button             	Not used.

	C button             	Rotate around center of viewpoint.
	
	START button         	Switch between fighter screen and 
                             	Controller model screen.

	Z button             	Change precedence of graphic 
				threads (only while pressing 
				button).  For a detailed 
				explanation, see the source file 
				graphic_01.c.  

	L/R buttons          	Not used.

* The above manipulations are for the fighter plane screen.  
Operations for the Controller model screen have been omitted.  


[4] Explanation of the screen

(1) Number of polygons for the model

Fighter No.1 (Blue) 314 Triangles
Fighter No.2 (Red)  314 Triangles

(2) How to read the performance meter

Starting from the left, the performance meter shows the first 
frame, second frame and third frame.  Measurements are made based 
on the time it takes to create the screen for 1 frame buffer.  
Each meter provides the following information: 

Red    ... Time for audio task processing. 
Green  ... Time for RSP processing of the graphic task.
Yellow ... Time for RDP processing of the graphic task.

* RSP processing time includes the time yielded by the audio task. 

(3) Screen rendering

To render the screen, these three graphic tasks are run in 
frame 1: 

	1. Clear the frame buffer and Z buffer (F3DEX microcode). 
	2. Render opaque polygons (F3DEX microcode). 
	3. Render lines (Linde3D microcode). 

The task of clearing the frame buffer and Z buffer is separated 
from the task of rendering opaque polygons because the former 
task makes use of time when the CPU is creating the audio task. 

[5] Copyright and reuse

The copyright to this sample program belongs to Nintendo Co., Ltd.  
The source code may be freely used and changed.  However, this 
sample program has not been fully debugged. Please be aware of 
this when using this sample program. 

[6] History

	1.0	1997/01/17 	Release of nnsample1.
	1.1	1997/02/03	Fixed problems with nnsched.c.
	1.2	1997/02/10	Restricted viewpoint movement in 
				main01.c so the viewpoint and 
				lookat point do not become the 
				same. 
	1.3	1997/12/09	Suppressed warning
                        	Fixed the RDP FIFO buffer size 
				error.  


