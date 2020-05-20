/*
   main.h

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.	
*/

#ifndef MAIN_H
#define MAIN_H

#ifdef _LANGUAGE_C

/* Definition of the external variable  */
/* Affix "volatile" to the argument which is shared by the main routine and 
   call-back so that it cannot be cleared by the optimization.   */
extern volatile int stage;
extern NUContData	contdata[1]; /* Read data of the controller  */

#endif /* _LANGUAGE_C */
#endif /* MAIN_H */




