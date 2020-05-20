This directory is an audio sample for NuSYSTEM which uses SGI tools.

Three sequence data are recreated in order by moving the Control Pad up and down.
Eleven audio data are recreated in order by moving the Control Pad right and left.

main.c	 	Main routine/Structure of game processing
		
main.h		Declaration of the argument to be used for a game

stage00.c	Create DL/Display processing and game processing

graphic.h	A definition of outside reference for graphics etc.

graphic.c	General routine for graphics

gfxinit.c	A static DL to initialize RSP/RDP

segment.h       Segment definition

spec		Spec file for makerom

wave.ctl	Wave form data for recreating sequence
wave.tbl
midi.sbk        Sequence data

se.ctl		Sound effect data
se.tbl
