This directory is a sample for reading a Controller for NuSystem.

The square can be moved using the Control Stick.

Use the A button to switch the direction of rotation.  

Use the B button to increase the rotation speed.

The sample source consists of the following files.

main.c		Main routine/Structure of game processing
		
main.h		Declaration of the argument to be used for a game

stage00.c	Create DL/Display processing and game processing

graphic.h	A definition of outside reference for graphics etc.

graphic.c	General routine for graphics

gfxinit.c	A static DL to initialize RSP/RDP

spec		Spec file for makerom
