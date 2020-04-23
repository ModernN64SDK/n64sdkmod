
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: hglobals.h,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:00 $
 *---------------------------------------------------------------------*/
#ifndef __GLOBALS__
#define __GLOBALS__

#include <ultra64.h>
#include "sysassigned.h"
#include "hspfix.h"
#include "htry.h"

#define  abs(x)                  ( (x >= 0) ? (x) : (-(x)))

#define  FORI(j)    for(i=0;i<j;i++) 
#define  FORJ(size) for(j=0;j<size;j++)
#define  CEIL(f) ((((s32)f) < f ) ? ((s32)f + 1) : (s32)f )

#define  CEIL16(i)   ( (i & 0x0f) ? ((i & ~0xf) + 0x10) : i )
#define  FLOOR16(i)  ( i & ~0xf )

#define  PXL32_R(rgba)  ((rgba>>24) & 0xFF)
#define  PXL32_G(rgba)  ((rgba>>16) & 0xFF)
#define  PXL32_B(rgba)  ((rgba>>8) & 0xFF)
#define  PXL32_A(rgba)  ( rgba & 0xFF )

typedef struct {
  s32 x,y,w,h;
} TRect;

typedef struct {
  u8 r,g,b,a;
} TRGBA;

typedef struct TDmaNode_s {
    ALLink      node;          /* !!important!! position fixed. inherited
member from ALLink */ 
    void 		* romaddr;	   /* Address for the data inside of theROM */
    u32         frame;         /* value of frame counter used last */
    u8          * dma;         /* pointer value for buffer */
} TDmaNode;

typedef struct {
    u8          initialized;  /* flag for whether or not initialized already    */
    TDmaNode    *usedlist_first;   /* head of the DMABuffer link list which is
being used */
    TDmaNode    *freelist_first;   /* head of the DMABuffer link list which is
being freed     */
} TDmaListInfo;

extern OSId			g_thread_id;

extern OSPiHandle   *HANDLE_ROM; 
extern s16			resetflag;
extern u8  			rdbSendBuf[RDB_SEND_BUF_SIZE];
extern OSMesgQueue  rspMsgQ, rdpMsgQ, siMessageQ;
extern void g_create(void);

extern s32  g_dmaRead(OSPiHandle *handle, void *src_addr, void *dest_addr, u32 size);
#define g_romRead(src_addr,dest_addr,size) g_dmaRead( HANDLE_ROM ,src_addr,dest_addr,size)

extern void g_dmaNode_insert( TDmaNode *_new, TDmaNode *_dest );
#define  g_dmaNode_add(_new,_dest) alLink( &(_new->node) , &(_dest->node))

#endif /* __GLOBALS__ */
