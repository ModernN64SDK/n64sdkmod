This directory includes a sample of switching screens (stages) for NuSystem.

Use the Start button to switch screens (stages).

Stage 0 - use the Control Stick to move a square.

Stage 1 - use the A button to switch the direction of rotation.
          Use the B button to increase the rotation speed. 

The sample source consists of the following files.

main.c		Main routine/Structure of game processing
		
main.h		Declaration of the argument to be used for a game

stage00.c	Create DL for Stage 0/Display processing and game processing

stage01.c	Create DL for Stage 1/Display processing and game processing

graphic.h	A definition of outside reference for graphics etc.

graphic.c	General routine for graphics

gfxinit.c	A static DL to initialize RSP/RDP

nu2.ld		Linkerscript which will be preproccesed and linked with "ld"
