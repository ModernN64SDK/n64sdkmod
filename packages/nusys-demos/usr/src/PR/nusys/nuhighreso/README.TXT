------------------------------------------------------------------
NINTENDO64 NuSytem sample
highreso
Copyright (C) 1997, NINTENDO Co,Ltd.
------------------------------------------------------------------


[1] Introduction

This is a sample using both the high- and low-resolution mode of 
NuSystem.
Nintendo 64 supports the high-resolution mode.
 
When NuSystem uses the nuGfxInit() function the initial setting 
is for low-resolution mode. Thus, in order to change to high-
resolution mode, you must change the VI mode, the frame buffer 
address, and the Z buffer address.  You must also change the 
location of the audio heap. 

Please use this sample as a reference when using high-resolution 
mode, or when using both low-resolution mode and high-resolution 
mode. 

[2] How to use

Immediately after startup, the screen is in high-resolution 
mode.   Press the "A" button on the Controller to switch between 
high-resolution, and low-resolution mode.

[3] Memory mapping

The diagram below shows how high-resolution mode and low-
resolution mode are mapped in memory in this sample.


     Memory mapping for high resolution and low resolution 
     screens

	

              High resolution		    Low resolution
     0x80000000 +---------+              +----------+
                | System  |              | System   |
     0x80000400 +---------+              +----------+
                |  Free   |              | Z-Buffer |
     0x80025A00 +---------+              +----------+
                |  Code   |              |  Code    |
                | & Data  |              |  & Data  |
                | & Heap  |              |  & Heap  |
     0x801BE000 +---------+              |          |
                |Z-Buffer |              |          |
                |FrameBuf |   0x8030F800 +----------+
                |         |              | FrameBuf |
                |         |              |          |
     0x80380000 +---------+              +----------+
                | Audio-  |              | Audio-   |
                |  Heap   |              |  Heap    |
     0x083FFFFF +---------+              +----------+


	
In high resolution mode, 0x80000400-0x80024FF becomes a free 
area. If you do not want to free this area, you can allocate it 
by dynamically securing the Z-buffer.  However, the Z-Buffer and 
the FrameBuffer should be allocated to another bank in RDRAM.
(This sample's high-resolution memory mapping is no good, but it 
is easy to manage the heap memory.) 	

[4] Note

The S2DEX microcode BG function is used for the screen display, 
but this ends up taking more time than expected.  Actually, if 
the entire BG screen is drawn, the frame buffer does not have to 
be cleared and the process can be omitted.  This is great, since 
it takes so long to clear when in high-resolution mode.	
