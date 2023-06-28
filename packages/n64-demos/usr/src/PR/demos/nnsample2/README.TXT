------------------------------------------------------------------
		NINTENDO64 TECHNICAL SUPPORT CENTER 
		    NINTENDO64 SAMPLE PROGRAM 2
		Copyright (C) 1997, NINTENDO Co,Ltd.
------------------------------------------------------------------


[0] Contents

	[1] Introduction
	[2] Using make
	[3] Control operations
	[4] Explanation of the program
	[5] Copyright and reuse
	[6] History

[1] Introduction

This is a F3DEX Ver1.21 microcode sample program.  It has the 
same base as nnsample1, but uses the BranchDisplayList function 
newly added since F3DEX Ver1.21 and the direct load of microcode.
Since in F3DEX Ver1.21, ucode performs BranchDisplayList, it is 
easy to conduct the LOD (Level Of Detail) process.  And direct 
loading of the microcode simplifies the scheduling of graphic 
tasks.  Depending on the process, it is easy to switch microcode.  

[2] Using make

Whether using the Indy emulator board or PARTNER-N64, please 
execute make without any additions.

[3] Control operations

	Control pad	Moves object right, left and to the rear.
	
	START button	Initializes the screen.


[4] Explanation of the program

(1) LOD process

Draws the following character models, based on the LOD and on the
position of the model from the viewpoint. 

	From the viewpoint to 500      "NEAR"
	From 501 to 1000               "MIDDLE"
	From 1001 to 1600              "FAR"
	Nothing drawn above 1600.

Indicating with this DisplayList is as follows:  (See lodmodel.c)

       gsSPBranchLessZ(near__dl, 0, 500, 10, 2000, G_BZ_PERSP),
       gsSPBranchLessZ(middle__dl, 0, 1000, 10, 2000, G_BZ_PERSP),
       gsSPBranchLessZ(far__dl, 0, 1600, 10, 2000, G_BZ_PERSP),
       gsSPEndDisplayList(),

In this way, the ucode side calculates the distance from the 
viewpoint to the model and performs the LOD process.


(2) Direct load of the microcode

It is possible to dynamically change the microcode by setting

	gSPLoadUcodeL(glist_ptr++, gspL3DEX_fifo);

in the display list creation part, like line 84 in graphic.c.  By
doing this, the microcode can be easily switched, depending on 
the subject being rendered, and scheduling becomes easy.  
However, loading microcode is overhead, so it is best not to 
switch the microcode too frequently.


(3) Notes regarding use of the fifo microcode

RDP OUTPUT SIZE

When using the fifo graphic microcode, you must enter the fifo 
buffer's maximum pointer + 1 in "*t.output_buff_size" of the 
OSTask structure when the task is started.  For example, in this 
sample, the pointer is assigned and used as shown below:

	---def.h---
	#define GFX_RDP_OUTPUT_SIZE 1024    /* RDP FIFO size. */
	
	---rdp_output.c---
	u64 rdp_output[GFX_RDP_OUTPUT_SIZE];

	---graphic.c---
	gtask->list.t.output_buff = (u64 *)&rdp_output[0];
	gtask->list.t.output_buff_size = 
		(u64 *)&rdp_output[GFX_RDP_OUTPUT_SIZE];

Please be careful, since the role of this pointer variable 
differs in the dram microcode and xbus.  
	
	
[5] Copyright and reuse

The copyright to this sample program belongs to Nintendo Co., Ltd.  
The source code may be freely used and changed.  However, this 
sample program has not been fully debugged, so please be sure to 
debug it for your own use. 


[6] History

1.0	1997/01/30 	Release of nnsample2.
1.1	1997/02/03	Fixed problems with nnsched.c.
1.2	1997/12/09	* Suppressed warning.
                        * Fixed the RDP FIFO buffer size error. 
