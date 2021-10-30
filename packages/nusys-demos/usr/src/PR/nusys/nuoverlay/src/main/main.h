/*
   main.h

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.			
*/

#ifndef MAIN_H
#define MAIN_H

#ifdef _LANGUAGE_C

/* Definition of external variables */
/* Variables shared by the main routine and callbacks are  */
/* made volatile so they are optimized and not deleted.    */
extern volatile int stage;
extern NUContData	contdata[1]; /* Data read from Controller */
/* Value used for pending callback function task check -- 0 or 1  */
extern int pendflag;
/* Number of frames displayed per second */
extern int dspcount;
/* Console display buffer */
extern char conbuf[];

/* for stage00 and stage01 */
extern float triPos_x; /* Display coordinate X */
extern float triPos_y; /* Display coordinate Y */
extern float theta;  /* Angle of rotation of square */

#endif /* _LANGUAGE_C */
#endif /* MAIN_H */




