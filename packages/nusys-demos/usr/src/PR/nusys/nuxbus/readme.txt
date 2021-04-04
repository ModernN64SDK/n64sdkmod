------------------------------------------------------------------
This directory contains a sample that changes the FIFO buffer of 
microcode.
------------------------------------------------------------------

It draws the same square 25 times and rotates them.  The number 
of frames to be processed per second will be displayed.

It can not be less than 0x410, which is the minimum value for 
gspF3DEX2.fifo.oFIFO buffer.  XBUS microcode can regard the FIFO 
buffer as 0.  If the amount of FIFO buffer is reduced, then the 
area can be used for another use.  If FIFO buffer is redeced 
down to 0x1400, pending RSP may occur.

When a huge polygon is displayed just like in this sample, the 
number of frames displayed per second will be stable and 
consistent, since NBUS microcode has an advantage (FIFO buffer 
size can be small).

This sample stops processing every once in a while when it 
changes the FIFO buffer size, consecutively. This is because 
this sample does not continue to process until one task 
completes everytime FIFO buffer changes.

How to play

Z trigger		Switches between FIFO microcode and XBUS 
			microcode
C button unit UP	Increases the size of FIFO buffer by  
			0x10 bytes
C button unit RIGHT	Decrease the size of FIFO buffer by  
			0x10 bytes
C button unit LEFT	Increase the size of FIFO buffer by  
			0x100 bytes
C button unit DOWN	Decrease the size of FIFO buffer by  
			0x100 bytes

A button            	Changes the rotating direction of a  
			square
B button            	Increases the rotating speed of a square  
			while it is pressed down

