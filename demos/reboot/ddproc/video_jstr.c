
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: video_jstr.c,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:01 $
 *---------------------------------------------------------------------*/
/*
     Routine to display the built-in ROM font for DD
     Also refer to the custom version of sprite.c 
 */
/* pre-process */

#include "hglobals.h"
#include "hvideo.h"
#include "hvideo_str.h"
#include "hdd.h"

#define  HORIZONAL_LINE_YOFFS 14

/* variables */

OSMesgQueue      jstrDmaMsgQ;
static OSMesg    jstrDmaMsgBuf[JSTR_DMA_NUM];
static OSIoMesg  jstr_iomsg[JSTR_DMA_NUM];

static TDmaListInfo  _dmalist_info;
static TDmaNode      _dma[ JSTR_DMA_NUM ];
static u8 * _empty_dma;

static s32	  _dma_cnt;
static s32 	  _jstr_framecnt=0;  

static OSPiHandle *_dd_dev;
static TDDToken _token;

/* local func */
s32     	_mkSprite( Gfx **dlp ,Sprite *sp, u8 *str );
s32         _setJFont(Bitmap *bm,s32 idx);
s32         _setAFont(Bitmap *bm,s32 idx);

/*  extern void str_setColor(Sprite *sp, u32 color); */

#if 0
/* public */
void    	jstr_clean(void);
void    	jstr_init(void);
void *  	jstr_getBmp(s32 offs);
s32     	video_setJStr(u8 *str, s32 xpos, s32 ypos, u32 color);
#endif

#ifdef  EUC_CODE
int e2s(int code);
#endif

/* implementation */

s32 
_setJFont(Bitmap *bm,s32 idx)
{ 
  s32 offs;

  offs = LeoGetKAdr(idx);

  if( offs >= 0 )
	{
	  bm->width = 16;				
	  bm->width_img = 16;			
	  bm->s = 0;					
	  bm->t = 0;					
	  bm->buf = jstr_getBmp(offs);				
	  bm->actualHeight = 16;		
	  bm->LUToffset = 0;			
	}
  else
	{
	  _setJFont(bm,0x8148);			/* "?" */
	}

  return -1;
}

s32
_setAFont(Bitmap *bm ,s32 idx)
{
  s32 offs ,align;
  int dx,dy,cy; 

  offs = LeoGetAAdr(idx,&dx,&dy,&cy);
  if( offs >= 0 ) 
	{
	  align = (dx & 0x1);
	  bm->width = dx;				
	  bm->width_img = dx + align;		
	  bm->s = 0;					
	  bm->t = 0;					
	  bm->buf = jstr_getBmp(offs);
	  bm->actualHeight = dy;		
	  bm->LUToffset = 0;			
	}
  else
	{
	  _setJFont(bm,LeoGetKAdr(0x8148));			/* "?" */
	}

  return  HORIZONAL_LINE_YOFFS - cy; 
}

void
jstr_clean(void)
{
  TDmaNode *p, *nextp;
  s32 i;

  OSIoMesg      *iomsg = (OSIoMesg *)0;

  FORI( _dma_cnt )
	{
	  if(osRecvMesg(&jstrDmaMsgQ, (OSMesg *)&iomsg, OS_MESG_NOBLOCK) == -1)
		  try_warning("video_jstr : Dma not done\n" ,1);
	}

/*  Free the right of use for the PI bus  */
  dd_freeToken(_token);
  _token = 0;

  p = _dmalist_info.usedlist_first;
  while( p ) {
    nextp = (TDmaNode *)p->node.next;

    /*
       Add the buffer which has exceeded JSTR_KEEP_COUNT
       to the link list of the empty buffer.
       When JSTR_KEEP_COUNT is increased, the buffer to be used will be
       increased, and when it is decreased the number of DMA call will be
       increased.    */

    if( p->frame + JSTR_KEEP_COUNT  < (u32)_jstr_framecnt )
	  {
		if(_dmalist_info.usedlist_first == p)
		  _dmalist_info.usedlist_first = (TDmaNode *)p->node.next;
      
		alUnlink( (ALLink *)p );

		if( _dmalist_info.freelist_first )
		  {
			g_dmaNode_add(p,_dmalist_info.freelist_first);
		  }
		else
		  {
			_dmalist_info.freelist_first = p;
			p->node.next = NULL;
			p->node.prev = NULL;
		  }
	  }
    p = nextp;
  }

  _dma_cnt = 0;
  _jstr_framecnt++;  
}

void
jstr_init(void)
{
  s32         i;
  extern u8 heap_dma[][JSTR_DMA_SIZE];

  _dd_dev = dd_getHandle();

  /* create message queue for DMA */
  osCreateMesgQueue(&jstrDmaMsgQ, jstrDmaMsgBuf, JSTR_DMA_NUM );

  _dmalist_info.freelist_first = &_dma[0];    /* Register the head empty */
  _dmalist_info.usedlist_first = NULL;        /* No MABuffer in use */

  FORI( JSTR_DMA_NUM-1 )
	{
	  alLink( (ALLink*)(&_dma[i+1]),(ALLink*)&_dma[i]);
	  _dma[i].dma = heap_dma[i];
	}

  _empty_dma = heap_dma[JSTR_DMA_NUM];

  _dmalist_info.initialized = 1;    /* a set of flag which has been initialized */

}

void *
jstr_getBmp(s32 offs)
{
  TDmaNode    *p,*lastp;
  OSIoMesg    *iomsg;
  void * 	  romaddr;

  lastp = 0;
  p = _dmalist_info.usedlist_first;

  romaddr = (void *)(DDROM_FONT_START + offs);  

  /* Check if there is any data desired in the buffer */
  while(p)
	{  
	  /* romaddr is sorted in ascending order for TDmaNode */
	  if( romaddr < p->romaddr ) break;                   
	  if( romaddr == p->romaddr ) 		  /* Data exist in the buffer */
		{
		  p->frame = _jstr_framecnt; /* Set the value of current frame
  counter */
		  return p->dma;             /* Return the physical address of
  buffer */ 
		}
      lastp = p;
      p = (TDmaNode *)p->node.next;
    }
  
  /*There is no data desired in the buffer */

  /*  Obtain the right of use for PI bus   */
  if( !_token ) _token = dd_getToken(DD_TOKENKEY_JSTR,0);

  if( _token )
	{
	  p = _dmalist_info.freelist_first;   /* Take out one free buffer from DMABuffer */
          
	  try_assert("dmabuffer not exist",(s32)p); /* Check to make sure whether buffer exists or not */
	  
	  _dmalist_info.freelist_first = (TDmaNode *)p->node.next;
	  alUnlink((ALLink *)p);
	  
	  /* Insert the obtained DMA buffer to the link list that is being used so that romaddr will be in ascending order. */
	  if( lastp ) 
		{
		  /* Insert it next to lastp */
		  g_dmaNode_add(p,lastp);
		} 
	  else if( _dmalist_info.usedlist_first ) 
		{
		  /* Insert it to the head of link list */
		  lastp = _dmalist_info.usedlist_first;
		  _dmalist_info.usedlist_first = p;
		  
		  g_dmaNode_insert(p,lastp);
		}
	  else
		{
		  /* All the DMABuffer can be used */
		  _dmalist_info.usedlist_first = p;
		  p->node.next = 0;
		  p->node.prev = 0;
		}
	  
	  try_assert("video_jstr : not 2 byte align",!((u32)romaddr & 0x1) ); /* Restriciton on DMA */
	  
	  p->romaddr  = (void *)romaddr;
	  p->frame = _jstr_framecnt;     /* Set the current frame counter */
	  
	  /* Transfer DMA */
	  iomsg = &jstr_iomsg[_dma_cnt++];
	  iomsg->hdr.pri       = OS_MESG_PRI_NORMAL;
	  iomsg->hdr.retQueue  = &jstrDmaMsgQ;
	  iomsg->dramAddr      = p->dma;
	  iomsg->devAddr       = (u32)romaddr;
	  iomsg->size          = JSTR_DMA_SIZE;
	  
	  _dd_dev->transferInfo.cmdType = OS_OTHERS;
	  
	  osEPiStartDma(_dd_dev, iomsg, OS_READ);  
	  
	  return p->dma;
	}
  else
	return _empty_dma;
}

extern Sprite str_sprite;
extern s32 str_charsum;

s32
video_setJStr(u8 *str, s32 xpos, s32 ypos, u32 color)
{
  Gfx     *dl;
  Sprite  *sp;

  sp = &str_sprite;
  sp->bmsiz = G_IM_SIZ_DD;

  if( str[0] == 0 ) return 0;
  try_assert("drive not connected",dd_driveExist()); 
 
  if( VIDEO_DLMODE_SPRITE != video_getCurrentDLMode() )
	  video_startDLMode(VIDEO_DLMODE_SPRITE);

  dl = (Gfx *)video_getDL();
  if ( dl )
	{
	  str_setColor( sp, color );
	  spMove( sp, xpos , ypos );
	  str_charsum += _mkSprite( &dl ,sp, str);
	  video_setDL(dl);
	}  

  return -1;
}


s32
_mkSprite( Gfx **dlp ,Sprite *sp, u8 *str )
{
    s32  i , idx , yalign = 0;
    Bitmap * bm;
	Gfx * dl = *dlp ,*dl2;

    bm = sp->bitmap;
	sp->width = 0;

	for(i=0; i < STR_MAXLENGTH;++i)
	  {
	    if( str[i] == 0 )
		  {
			_setJFont(bm,0x8140);
			bm->width = -1;
			break;
		  };
		
		if( str[i] & 0x80 )
		  {
			idx = (s32)((str[i]<<8)+str[i+1]);
#ifdef EUC_CODE
  idx = e2s(idx);
#endif
			_setJFont(bm,idx);
			yalign = 0;
			++i;
		  }
		else
		  {
			idx = 0x00A0 + str[i];
			yalign = _setAFont(bm,idx);
		  }
		sp->width  = bm->width;
		sp->y += yalign;
		dl2 = spDraw( sp );

		if( dl2 )  gSPDisplayList(dl++,dl2);

		sp->y -= yalign;
		sp->x += sp->width;
	  };
	
    sp->nbitmaps = i;
    sp->height = bm->actualHeight;

	*dlp = dl;
	
    return i;
}

#ifdef EUC_CODE
int 
e2s(int code)
{
  int c1,c2;
  
  c1 = (code >> 8) & 0xff;
  c2 = (code >> 0) & 0xff;

  if (c1 % 2)
        {
          c1 = ( c1>>1 ) + ( c1 < 0xdf ? 0x31 : 0x71);
          c2 -= 0x60 + (c2 < 0xe0);
    }
  else
        {
          c1 = (c1>>1) + (c1 < 0xdf ? 0x30 : 0x70);
          c2 -= 2;
    }

  return (int)( ((c1<<8) & 0xff00) + ( c2 & 0xff ) );
}
#endif



#if 0
/*
   tech note

   dd_jstr might have been a more appropriate name for module.

   DMA buffer clear (jstr_clean()) needs to be called every frame.

   It is rather intense process since _mkSprite calls spDraw by character.
   ( Since the upper and lower positions need to be adjusted by each character
   for English font.) 
   
*/
#endif
