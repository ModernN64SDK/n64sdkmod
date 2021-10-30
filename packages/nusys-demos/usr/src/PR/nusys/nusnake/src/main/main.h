/*============================================================================
  NuSYSTEM sample program [SNAKE TAIL HACK]
  
  main.h
  
  Copyright (C) 1997, NINTENDO Co,Ltd.
  ============================================================================*/

#ifndef MAIN_H
#define MAIN_H

#ifdef _LANGUAGE_C

typedef struct{
    u16	button;			/* A,B,C,Z,L,R,START,PAD ,button 	*/
    s8	stick_x;		/* The range of the practical use  -61 <= stick X <= 61	*/
    s8	stick_y;		/* The range of the practical use  -63 <= stick Y <= 63	*/
} ContTrace;

#endif /* _LANGUAGE_C */
#endif /* MAIN_H */
