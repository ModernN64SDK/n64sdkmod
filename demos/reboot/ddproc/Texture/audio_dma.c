/*
   audio_dma.c   --  DMA processing
 */

/* pre-process */

#include "hglobals.h"
#include "haudio.h"

#ifdef __NAUDIO__
#include <PR/n_libaudio.h>
#include <PR/n_libaudio_s_to_n.h>
#endif

/* variables */

OSMesgQueue      audioDmaMsgQ;
static OSMesg    audioDmaMsgBuf[AUDIO_DMA_NUM];
static OSIoMesg  audma_iomsg[AUDIO_DMA_NUM];

static TDmaListInfo  _dmalist_info;
static TDmaNode      _dma[ AUDIO_DMA_NUM ];
static s32	  _dma_cnt;
static u32 	  _audio_framecnt=0;  

s32 * look2 = &_dma_cnt;

f32 hits;
s32 dma_call_cnt,dma_use_cnt;

/* implementation */
/*-----------------------------------------------------------------

  DMA buffer cleanup

  Searches the DMA buffers and adds buffers with 2 or more unused frames
  to a linked list of free buffers. 

-----------------------------------------------------------------*/

static s32 dma_frame_max =0; /*  for debug */

void
audma_clean(void)
{
  TDmaNode *p, *nextp;
  s32 i;
  OSIoMesg      *iomsg = (OSIoMesg *)0;

  FORI( _dma_cnt )
	{
	  if(osRecvMesg(&audioDmaMsgQ, (OSMesg *)&iomsg, OS_MESG_NOBLOCK) == -1)
		osSyncPrintf("Dma not done");
	}

  p = _dmalist_info.usedlist_first;
  while( p ) {
    nextp = (TDmaNode *)p->node.next;

    /*
       Add buffers with more than AUDIO_BUFFER_USE_FRAME to the linked 
	list of free buffers. 
       Increasing AUDIO_BUFFER_USE_FRAME increases the number of usable 	buffers, and decreasing it increases the number of DMA calls. 
    */

    if( p->frame + AUDIO_DMA_CLEAR_COUNT  < _audio_framecnt )
	  {
		if(_dmalist_info.usedlist_first == p)
		  _dmalist_info.usedlist_first = (TDmaNode *)p->node.next;
      
		alUnlink( (ALLink *)p );

		if( _dmalist_info.freelist_first )
		  {
			alLink( (ALLink *)p , (ALLink *)_dmalist_info.freelist_first );
		  }
		else
		  {
			/*  alInsert ?   */
			_dmalist_info.freelist_first = p;
			p->node.next = NULL;
			p->node.prev = NULL;
		  }
	  }
    p = nextp;
  }

#ifdef _DEBUG
  {
	if( _dma_cnt > dma_frame_max ){
	  dma_frame_max = _dma_cnt;
	}
	
	if(dma_call_cnt == 0 )
	  {
		hits = 0;
	  }
	else
	  {
		hits = (((f32)dma_call_cnt-(f32)dma_use_cnt)*100.0)/(f32)dma_call_cnt;
	  }
	
	/* 		  osSyncPrintf("dma max = %d /f, dma_call = %d, dma_use = %d, Hits = %f\n", */
	/* 					   dma_frame_max, dma_call_cnt, dma_use_cnt,hits); */
	dma_call_cnt =0;
	dma_use_cnt =0;
  }
#endif

  _dma_cnt = 0;
  _audio_framecnt++;  
}

void
audma_init(void)
{
  /* create message queue for DMA */
  osCreateMesgQueue(&audioDmaMsgQ, audioDmaMsgBuf, AUDIO_DMA_NUM );

  /* Clear the DMABuffer initialization flag. */
  _dmalist_info.initialized = 0;

}

/*-------------------------------------------------------------
 
  Sequence player DMA callback set

 When it is registered in the sequence player, this callback routine is called only once,   to initialize the list.  The callback routine returned by return is then called. 

---------------------------------------------------------------*/

ALDMAproc
audma_new( void **state )
{
  int         i;

  /* Check whether the list has been initialized. If not, initialize it. */
  if( !_dmalist_info.initialized )
	{

	  _dmalist_info.freelist_first = &_dma[0];    /* Enter the start of the DMA buffer list */
	  _dmalist_info.usedlist_first = NULL;            /* No DMA buffers being used. */

	  FORI( AUDIO_DMA_NUM - 1 )
		{
		  alLink( (ALLink*)(&_dma[i+1]),(ALLink*)&_dma[i]);
		  _dma[i].dma = (u8 *)g_heapAlloc(&audio_heap, AUDIO_DMA_SIZE, 16);
		}
	  /* Set the 'initialized' flag */
	  _dmalist_info.initialized = 1;
	}

  *state = &_dmalist_info;  /* Passed to the callback routine as an argument 
										 (probably an unnecessary specification, however . . . ) */

  return audma_callback;
}

/*---------------------------------------------------------------

   DMA CallBack for sequence player 

   A DMA callback routine called from the sequence player
   
   If a needed values are in a DMA buffer, this routine returns the value of that	buffer. 	Otherwise, it loads data into the buffer by DMA. 
   This design reduces the number of DMA calls.  However, the unused portion is 	deleted at two frames.
   This control may be more efficient if changed to LIFO, as with the CPU cache.

---------------------------------------------------------------*/

s32
audma_callback(s32 addr, s32 len, void *state)
{
  void        *target_dma;
  TDmaNode    *p,*lastp;
  s32         delta,offs;
  OSIoMesg    *iomsg;

  *(s32 *)state = *(s32 *)state; /* stete : no use */

  lastp = 0;
  p = _dmalist_info.usedlist_first;

#ifndef __NO_DEBUG__
  dma_call_cnt++;
#endif /* _AUDIO_DEBUG_DMA_ */

  /* Check whether desired data are in the buffer. */
  while(p)
	{  
      
	  /* TDmaNode is aligned in ascending order of addr */
	  if( ( offs = (void *)addr - p->romaddr) < 0 ) break;                   
	
	  if( offs + len <= AUDIO_DMA_SIZE )
		{
		  /* Data are present in the buffer. */
		  p->frame = _audio_framecnt; /* Set the value of the current frame counter. */
     
		  /* Calculate the addresses that actually have needed data. */
		  target_dma = p->dma + offs;

		  /* Return the physical address of the buffer. */
		  return (s32) osVirtualToPhysical( target_dma );
		}
      lastp = p;
      p = (TDmaNode *)p->node.next;
    }
  
  /*  Buffer contains no desired data. */

  p = _dmalist_info.freelist_first;   /* Retrieve 1 of the free DMA buffers */

  try_assert("dmabuffer not exist",(s32)p); /* Check whether the buffer exists. */

  _dmalist_info.freelist_first = (TDmaNode *)p->node.next;
  alUnlink((ALLink *)p);

  /* 
     Insert the obtained DMA buffer into the linked list in ascending order of 
     addr 
  */
  if( lastp ) 
	{
	  /* Insert following lastp */
	  g_dmaNode_add(p,lastp);
	} 
  else if( _dmalist_info.usedlist_first ) 
	{
	  /* Insert at start of linked list. */
	  lastp = _dmalist_info.usedlist_first;
	  _dmalist_info.usedlist_first = p;

	  g_dmaNode_insert(p,lastp);
	}
  else
	{
	  /* All DMA buffers available. */
	  _dmalist_info.usedlist_first = p;
	  p->node.next = 0;
	  p->node.prev = 0;
	}
  
  target_dma = p->dma;           /* Target buffer address for transfer */  
  addr -= ( delta = addr & 0x1 );/* Put the address location at the 2-byte boundary for DMA. */
  p->romaddr  = (void *)addr;
  p->frame = _audio_framecnt;     /* Set the current frame counter. */

  /* DMA transfer */
  iomsg = &audma_iomsg[_dma_cnt++];
  iomsg->hdr.pri       = OS_MESG_PRI_NORMAL;
  iomsg->hdr.retQueue  = &audioDmaMsgQ;
  iomsg->dramAddr      = target_dma;
  iomsg->devAddr       = (u32)addr;
  iomsg->size          = AUDIO_DMA_SIZE;

  osEPiStartDma(HANDLE_ROM, iomsg, OS_READ);  

#ifndef __NO_DEBUG__
  dma_use_cnt++;
#endif /* AUDIO_DEBUG_DMA_ */
  
  return (int) osVirtualToPhysical(target_dma) + delta;
}

/*
void
_dmaList_insert( TDmaNode *_new, TDmaNode *_dest )
{
	  _new->next  = _dest;
	  _new->prev  = 0;
	  _dest->prev = _new;
}

void
_dmaList_splice( TDmaNode *ln, TDmaNode *to )
{                                       
    ln->next = to->next;     
    ln->prev = to;           
    if (to->next)            
        to->next->prev = ln; 
    to->next = ln;           
}

void
_dmaList_remove( TDmaNode *ln )                       
{                                       
    if (ln->next)                   
        ln->next->prev = ln->prev;  
    if (ln->prev)                   
        ln->prev->next = ln->next;  
}
*/



