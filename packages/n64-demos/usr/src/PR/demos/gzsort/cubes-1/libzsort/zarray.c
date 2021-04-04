/*---------------------------------------------------------------------
	Copyright (C) 1997, Nintendo.
	
	File		zarray.c
	Coded    by	Yoshitaka Yasumoto.	Sep 10, 1997.
	Modified by	
	
	$Id: zarray.c,v 1.4 1997/11/25 07:40:02 yasu Exp $
  ---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	"gzpoly.h"

extern	void	osDirtyDCache(void *, int);

/*---------------------------------------------------------------------------*
 *	zArray
 *---------------------------------------------------------------------------*/
static	struct	{
  union	{
    zHeader	*bucket[ZPACKET_LVL+2];		/* for Z Sort */
    Gfx		 gfx_dl[ZPACKET_LVL/2+1];	/* for RSP    */
  } t;
  float	 finvwMin;
  float	 finvwFct;
} zArray;

/*---------------------------------------------------------------------------*
 *	zArray_open
 *---------------------------------------------------------------------------*/
void	zArray_open(Gfx **gp, u16 near, u16 far)
{
  int	i;
  
  osDirtyDCache(zArray.t.bucket, sizeof(zHeader)*ZPACKET_LVL);
  
  /*------- Initialize zarray for zsort -------*/
  for (i = 0; i < ZPACKET_LVL; i += 4){
    zArray.t.bucket[i+0] = (zHeader *)G_ZOBJ_NONE;
    zArray.t.bucket[i+1] = (zHeader *)G_ZOBJ_NONE;
    zArray.t.bucket[i+2] = (zHeader *)G_ZOBJ_NONE;
    zArray.t.bucket[i+3] = (zHeader *)G_ZOBJ_NONE;
  }
  gSPZEndDisplayList(zArray.t.gfx_dl+ZPACKET_LVL/2);
  gSPZDisplayList((*gp) ++, zArray.t.gfx_dl);
  
  /*------- Save paramater to get z index -------*/
  zArray.finvwMin = ((float)(near + far)) / (float)(4 * (s32)far);
  zArray.finvwFct = ((float)ZPACKET_LVL) /
    (((float)(near + far)) / (float)(4 * (s32)near) - zArray.finvwMin);
}

/*---------------------------------------------------------------------------*
 *	zArray_close
 *---------------------------------------------------------------------------*/
void	zArray_close(void)
{
  osWritebackDCache(zArray.t.gfx_dl, sizeof(zArray.t.gfx_dl));
}

/*---------------------------------------------------------------------------*
 *	zArray_link
 *---------------------------------------------------------------------------*/
zHeader	*zArray_link(zHeader *zobj, u32 zid, s32 invW, float invfactor)
{
  float		finvW;
  s32		index;
  zHeader	*next;
  
  if (invW > 0){
    finvW = (float)invW * invfactor;
    index = (s32)((finvW - zArray.finvwMin) * zArray.finvwFct);
    if      (index <  0)           index = 0;
    else if (index >= ZPACKET_LVL) index = ZPACKET_LVL-1;
  } else {
    index = ZPACKET_LVL-1;
  }
  next = zArray.t.bucket[index];
  zArray.t.bucket[index] = (zHeader *)(((u32)zobj) + zid);
  return next;
}

/*======== End of zarray.c ========*/
