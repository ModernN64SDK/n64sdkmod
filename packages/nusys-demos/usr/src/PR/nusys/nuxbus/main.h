/*
   main.h

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.			
*/

#ifndef MAIN_H
#define MAIN_H

/* from appgfxinit.c */
#define GFX_UCODE_F3DEX_XBUS 1

#ifdef _LANGUAGE_C

/* Define external variables */
/* Use volatile to avoid optimization of
	the variables shared with the main routine and the callback */
extern volatile int stage;
extern NUContData	contdata[1]; /* Data to be read for a controller */

/* Whether the microcode utilizes XBUS or not */
extern volatile int now_xbusflag;
extern volatile int next_xbusflag;
/* FIFO size after modification (the initial value's set as minimum) */
extern volatile int now_fifosize;
extern volatile int next_fifosize;

/* Number of frames per second */
extern int dspcount;
/* Buffer for console display */
extern char conbuf[];

/* from appgfxinit.c */
void appGfxInit(void);

#endif /* _LANGUAGE_C */
#endif /* MAIN_H */




