/*
   main.h
Copyright (C) 1999 NINTENDO Co.,Ltd.
Copyright (C) 1999 MONEGI CORPORATION
*/

#ifndef MAIN_H
#define MAIN_H

#ifdef _LANGUAGE_C

/* External variable definitions  */
extern NUContData	contdata[1]; /* Controller read data */
extern u8 contPattern;		     /* Pattern of connected Controllers */
extern char _hvqdata1SegmentRomStart[], _hvqdata1SegmentRomEnd[];


#define DMA_QUEUE_SIZE  200
/*
 *  Compressed image data segment
 */
extern char _hvqdata1SegmentRomStart[], _hvqdata1SegmentRomEnd[];
extern char _hvqdata2SegmentRomStart[], _hvqdata2SegmentRomEnd[];
extern int useRSP;
extern int hvqno;

#endif /* _LANGUAGE_C */
#endif /* MAIN_H */




