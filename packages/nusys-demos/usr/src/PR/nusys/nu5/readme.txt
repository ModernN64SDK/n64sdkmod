This directory is an audio sample for NuSYSTEM which uses N64 Sound Tools data.

To compile the program, the N64 Sound Tools audio library must be installed.

In this sample, 25 squares are drawn every time rotating.

Information will be displayed showing the number of frames that can be processed 
per second.

Stages can be changed by the Start button. However, in Stage 1 it is being 
processed by dividing graphics tasks.   Use the Z button to change from double 
frame buffer mode to triple frame buffer mode.

The sample source consists of the following files.

main.c		Main routine/Structure of game processing
		
main.h		Declaration of the argument to be used for a game

stage00.c	Create DL for Stage 0/Display processing and game processing

stage01.c	Create DL for Stage 1/Display processing and game processing

graphic.h	A definition of outside reference for graphics etc.

graphic.c	General routine for graphics

gfxinit.c	A static DL to initialize RSP/RDP

segment.h       Segment definition

nu5.ld		Linkerscript which will be preproccesed and linked with "ld"

wave.ctl	Waveform data for recreating sequence
wave.tbl
midi.sbk       	Sequence data

se.ctl		Sound effect data
se.tbl


Revision History
05/19/99
* drwa/sec calculation method has been improved to be more accurate

* In stage 1, gfxClearCfb() is moved over to be processed first, instead at the 
  end.
  (If the program is changed so that stage01 is to be called first, an image would 
  be drawn without executing gfxClearCfb() even once, and then the program would 
  become unstable.)

