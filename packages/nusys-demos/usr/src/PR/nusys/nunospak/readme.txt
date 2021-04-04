This directory contains samples implemented in NuSystem that 
perform that same functions as the function nonPakMenu in 
demos/nosPak. 

Refer to main.c to see how the functions are called.
The following 5 files are needed to use the function nosPakMenu. 

	nospakmenu.c
	nospakmenuloadfont.c
	fontbmp.h
	nospakmenu.h
	pakmenu.h

All the necessary features are included, so it is recommended that 
these be customized and used. 

When these samples are used with hardware other than NTSC, 
arguments to osViSetMode in nupakmenu.c should be changed to suit 
the image mode (PAL, MPAL).
