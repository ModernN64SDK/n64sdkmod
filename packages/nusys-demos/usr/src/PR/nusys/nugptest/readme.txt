------------------------------------------------------------------
NINTENDO64 NuSystem sample
gptest
------------------------------------------------------------------

                 
[1] Introduction 

This sample confirms the operation of the NuSystem N64 Transfer 
Pak functions and checks the N64 Transfer Pak. It can be used to 
learn how the N64 Transfer Pak functions behave, depending on 
the state of the N64 Transfer Pak and the Game Boy Game Pak. It 
can also be used to determine what types of errors are returned.

[2] Operation

Control the screen displaying the function list as follows:

	A Button		Select
	B Button		Go back one
	Control Pad Up/Down	Move the cursor 

You can also input the address and size for nuContGBPakRead, 
nuContGBPakWrite, nuContGBPakFread and nuContGBPakFwrite.
	
	Control Pad Up/Down      +32, -32
	Control Pad Right/Left	 +256, -256
	L,R Button(s)	         +1, -1
	START	                 Reset to 0

For nuContGBPakRegWrite:

	(Input address)
	Control Pad Up/Down      +256, -256
	Control Pad Right/Left	 +4096, -4096
	START	                 Reset to 0

	(Data input)
	Control Pad Up/Down      +1, -1
	Control Pad Right/Left	 +16, -16
	START	                 Reset to 0

[3] Game Title 
The Game Boy game title was originally 16 bytes, but the last 
byte is now used for a flag that represents the status of 
support for Game Boy Color. 

0x80 means that color is supported, and 0xC0 means color 
dedicated (no DMG/MGB).  This program also uses this information 
to display the color support status to the screen. If the (C) 
displayed at the right edge of the screen is bright blue, color 
is supported. If it is bright red, the status is color dedicated. 

In the future, the game title may be further shortened and that 
area used for other information. Therefore, when creating a 
program that uses the title, please ensure that no problems 
occur even if some characters cannot be displayed. 


[4] Note

The sample source is not easy to read because we tried to 
enhance the generality of the menu display.
