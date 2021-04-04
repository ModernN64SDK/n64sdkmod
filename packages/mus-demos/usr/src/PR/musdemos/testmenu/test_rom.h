
/*************************************************************

  test_rom.h : Nintendo 64 Music Tools Library Sample
  (c) Copyright 1998, Software Creations (Holdings) Ltd.

  Version 3.14

  TESTMENU main header file.

**************************************************************/

#ifndef _TEST_ROM_H_
#define _TEST_ROM_H_


/* Stacksize in bytes */
#define STACKSIZE 0x2000

/* RSP segments */
#define CFB_SEG		1

/* end of 'makerom' section */
#ifndef SPECFILE

#include <ultra64.h>

/* Message queue lengths */
#define MAX_MESGS    		8
#define PI_MSGQUEUE_SIZE   	8
#define DMA_MSGQUEUE_SIZE     	1
#define RDPDONE_MSGQUEUE_SIZE 	1
#define RETRACE_MSGQUEUE_SIZE 	1

/* Thread priorities */
#define MAIN_PRIORITY           10
#define GAME_PRIORITY           10
#define SCHEDULER_PRIORITY      127

/* size of screen */
#define SCREEN_XSIZE    320
#define SCREEN_YSIZE    240

/* fifo buffer size */
#define FIFO_DATA_SIZE	(16*1024)

/* externals from 'test_workspace.c' */
extern unsigned short	cfb_A[];
extern unsigned short	cfb_B[];
extern u64				yield_buffer[];
extern u64				fifo_buffer[];

/* drawing pointers */
extern Vtx	*vertp;
extern u64	*glistp;

/* tune buffers in RAM */
extern int tune_buffer1[];
extern int tune_buffer2[];
extern int tune_buffer3[];
extern int tune_buffer4[];

/* sound effect buffer in RAM */
extern int sfx_buffer1[];

/* controller related vars */
extern OSMesgQueue serial_queue;
extern OSContPad   controllerdata[];


/* --------------- FUNCTION PROTOTYPES ------------------ */


/* functions from 'test_main.c' */
void	DmaRomToRam			(void *rom, void *ram, int len);
void	DownloadSongs		(void);


/* functions from 'test_menu.c' */
void	MenuProcess		(void);
void	MenuInit		(void);
void	MenuProcessController	(void);


/* functions from 'test_draw.c' */
void	DrawString		(char *text, int x, int y);
void	DrawChar		(unsigned short *letter, int x, int y);

#endif  /* SPECFILE */


#endif	/* _TEST_ROM_H_ */


/* end of file */
