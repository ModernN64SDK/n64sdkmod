
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: video_draw.c,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:01 $
 *---------------------------------------------------------------------*/

/*
    Drawing function to hide the display list
 */


/* pre-process */

#include "hglobals.h"
#include "hvideo.h"
#include "hsp.h"

/* variables */

static s32      video_DLmode;
static Gfx      *_pfrom ,*_pdl;

static Gfx      *sp_dlb[2];

/* implmentations */

s32
video_setSprite(Sprite *sp ,s32 xpos,s32 ypos)
{
  Gfx     *dl,*dl2;

#ifdef DEBUG
  if( !_pdl || ((_pdl - _pfrom) > ( DL_BUFSIZE - 1024 )) )
	{
	  osSyncPrintf("video_draw alert :DL limit %d \n",(_pdl - _pfrom));
	  return 0;
	}
#endif

  if( video_DLmode != VIDEO_DLMODE_SPRITE )
	  video_startDLMode(VIDEO_DLMODE_SPRITE);

  dl = (Gfx *)video_getDL();
  if ( dl )
	{
	   sp->rsp_dl_next = _pdl;
	   spMove( sp, xpos , ypos );
	   dl2 = spDraw( sp );
	   if ( dl2 )
		 {
		   gSPDisplayList(dl++,dl2);
		   if ( video_setDL(dl) ) _pdl = sp->rsp_dl_next;
		 }
	}  

  return -1;
}

void 
video_initSprite(void)
{
  s32 i;
  extern Gfx heap_sp_dlb[][DL_BUFSIZE];

  for(i=0;i<2;i++)
	{
	  sp_dlb[i] = heap_sp_dlb[i];
	  bzero( (void *)sp_dlb[i],DL_BUFSIZE * 8 );
	}

  video_clearSprite(0);

}

void
video_clearSprite(s32 cur)
{
  _pfrom = (Gfx *)(&sp_dlb[cur][0]);
  _pdl   = _pfrom;
}

void
video_finishDLMode(void)
{
  Gfx     *dl;

  switch(video_DLmode)
	{
	case VIDEO_DLMODE_SPRITE:
	  dl = (Gfx *)video_getDL();
	  if(dl)
		{
		  spFinish(&dl);
		  dl--;
		  video_setDL(dl);
		  video_DLmode = VIDEO_DLMODE_IDLE;
		}
	  break;
	default:
	  break;
	}
}

void
video_startDLMode(s32 mode)
{
  Gfx     *dl;

  if ( video_DLmode != VIDEO_DLMODE_IDLE )
	video_finishDLMode() ;

  switch(mode)
	{
	case VIDEO_DLMODE_SPRITE:
	  dl = (Gfx *)video_getDL();
	  if(dl){
		spInit( &dl );
		video_setDL(dl);
		video_DLmode = VIDEO_DLMODE_SPRITE;
	  }
	  break;
	default:
	  break;
	}
}


s32
video_getCurrentDLMode(void)
{
  return video_DLmode;
}

s32
video_drawRect(s32 x,s32 y,s32 w,s32 h ,u32 rgba)
{
  Gfx     *dl,*dl2;

  if( try_warning( "video_drawrect :DL limit \n" ,
			  ( _pdl && ((_pdl - _pfrom) < ( DL_BUFSIZE - 1024 ))) ) )
	return 0;
	  
  if( video_DLmode != VIDEO_DLMODE_DRAWRECT )
	  video_startDLMode( VIDEO_DLMODE_DRAWRECT );

  dl = (Gfx *)video_getDL();
  if ( dl )
	{
	  dl2 = spFillRect( _pdl , x,y,x + w ,y + h 
					   ,PXL32_R(rgba) ,PXL32_G(rgba), PXL32_B(rgba),PXL32_A(rgba) );
	  gSPDisplayList( dl++ , _pdl );
	  if ( video_setDL(dl) ) _pdl = dl2;
	}
  return -1;
}



/*

 */
