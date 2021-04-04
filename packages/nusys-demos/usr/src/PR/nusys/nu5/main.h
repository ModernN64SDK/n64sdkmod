/*
   main.h

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.
*/

#ifndef MAIN_H
#define MAIN_H

#ifdef _LANGUAGE_C

/* Definition of the external variable  */
/* Affix "volatile" to the variable which is shared by the main routine 
   and the call-back so that it is not cleared by the optimization.  */
extern volatile int stage;
extern NUContData	contdata[1]; /* Read data of the controller  */
/* Values 0 or 1 used by the pending task check of the call-back function  */
extern int pendflag;
/* The number of frames displayed in a second  */
extern int dspcount;
/* The buffer for the console display  */
extern char conbuf[];

#endif /* _LANGUAGE_C */
#endif /* MAIN_H */




