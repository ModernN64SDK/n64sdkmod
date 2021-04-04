This directory contains NuSystem audio samples that use N64SoundTools.
The N64SoundTools audio libraries (MUS libraries) must be installed to compile 
these samples. 

The samples were created with the purpose of using the MUS libraries directly. 
Only the NuSystem nuAuStl functions that are is particularly useful are used.
All initialization parameter settings and default values are set explicitly.

The order in which the sequence data are played can be determined using the upper 
and lower portions of the Control Pad. 

The order in which the sound effect data are played can be determined using the 
right and left sides of the Control Pad. 

The playback tempo is changed using the L and R buttons.

The Z button fades out a sequence.
 
The sample source consists of the following files.

main.c		Main routine/Structure of game processing
		
main.h		Declaration of the argument to be used for a game

stage00.c	Create DL/Display processing and game processing

graphic.h	A definition of outside reference for graphics etc.

graphic.c	General routine for graphics

gfxinit.c	A static DL to initialize RSP/RDP

spec		Spec file for makerom

drumtest.wbk 	Data output by N64SoundTools
drumtest.ptr  
dt_sfx1.bfx
dt_song1.bin
dt_song2.bin
dt_song3.bin
