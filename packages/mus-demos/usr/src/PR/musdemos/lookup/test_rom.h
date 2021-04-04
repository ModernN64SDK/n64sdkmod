
/*************************************************************

  test_rom.h : Nintendo 64 Music Tools Library Sample
  (c) Copyright 1998, Software Creations (Holdings) Ltd.

  Version 3.14

  LOOKUP table main source file. This demo illustrates the
  effect of changing the sample lookup table located in
  a song file.

**************************************************************/

#ifndef _TEST_ROM_H_
#define _TEST_ROM_H_

/* Stacksize in bytes */
#define STACKSIZE 0x2000

/* RSP segments */
#define CFB_SEG		1

/* don't include following section in makerom */
#ifndef SPECFILE

#ifndef F3DEX_GBI
#define F3DEX_GBI
#endif

#include <ultra64.h>
#include <sched.h>

/* Message queue lengths */
#define MAX_MESGS               8
#define PI_MSGQUEUE_SIZE        8
#define DMA_MSGQUEUE_SIZE     	1
#define RDPDONE_MSGQUEUE_SIZE 	1
#define RETRACE_MSGQUEUE_SIZE 	1

/* Thread priorities */
#define MAIN_PRIORITY           10
#define GAME_PRIORITY           10
#define SCHEDULER_PRIORITY      127

/* screen size */
#define SCREEN_XSIZE            320
#define SCREEN_YSIZE            240

/* fifo buffer size */
#define FIFO_DATA_SIZE         (16*1024)

/* externals from 'test_workspace.c' */
extern unsigned short	cfb_A[];
extern unsigned short	cfb_B[];
extern u64              yield_buffer[];
extern u64              fifo_buffer[];

/* externals necessary for 'test_music.c' */
extern OSSched     	sc;
extern OSMesgQueue	dma_queue;

/* function prototypes from 'test_music.c' */
void	InitMusicDriver	(void);

/* vars from 'test_music.c' */
extern unsigned int	tune_buf1[];


#endif  /* SPECFILE */


#endif	/* _TEST_ROM_H_ */


/* end of file */

