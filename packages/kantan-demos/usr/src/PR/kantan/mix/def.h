/*============================================================================
  def.h
  ============================================================================*/

#ifndef DEF_H
#define DEF_H

/* segment for spec file */
#define STATIC_SEGMENT 	1
#define CFB_SEGMENT	2
#define DYNAMIC_SEGMENT	3

/* screen size */
#define SCREEN_HT        240
#define SCREEN_WD        320

#define STACKSIZE        0x2000
#define GLIST_LEN        2048
#define GFX_DLIST_BUF_SIZE 0x3000
#define GFX_DLIST_BUF_NUM  2
#define GFX_RDP_OUTPUT_SIZE 4096*16    /* RDP FIFO size */

/* thread priority */
#define IDLE_THREAD_PRI        1
#define MAIN_THREAD_PRI        5
#define RMON_THREAD_PRI        OS_PRIORITY_RMON
#define GRAPHIC_THREAD_PRI      10


/* thread id (for debug) */
#define IDLE_THREAD_ID         1
#define RMON_THREAD_ID         2
#define MAIN_THREAD_ID         3
#define CONTROLLER_THREAD_ID   4
#define GRAPHIC_THREAD_ID      5
/*#define CONTROLLER_THREAD_ID   5*/
#ifdef _LANGUAGE_C

#endif /* _LANGUAGE_C */
#endif /* DEF_H */
