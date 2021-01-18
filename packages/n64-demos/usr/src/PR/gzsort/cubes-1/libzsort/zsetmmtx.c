/*---------------------------------------------------------------------*
	Copyright (C) 1997, Nintendo.
	
	File		zsetmmtx.c
	Coded    by	Yoshitaka Yasumoto.	Sep 26, 1997.
	Modified by	
	
	$Id: zsetmmtx.c,v 1.2 1997/10/08 10:06:47 yasu Exp $
 *---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	"gzpoly.h"

/*=========================================================================*
  Comments:	Set modelview matrix
 *=========================================================================*/

/*---------------------------------------------------------------------------*
 *	Set modelview matrix
 *	Set lighting vector
 *---------------------------------------------------------------------------*/
void	zSetMMtx(Gfx **gpp, Mtx *mmtx, u32 num_light, u16 renderMode)
{
  /*-----------------------------------------*
    Set modelview matrix and make MP matrix
   *-----------------------------------------*/
  gSPZSetMtx((*gpp) ++, GZM_MMTX, mmtx);
  gSPZMtxCat((*gpp) ++, GZM_MMTX, GZM_PMTX, GZM_MPMTX);

  /*-----------------------------------------*
    Light calculation
   *-----------------------------------------*/
  if (renderMode & (ZRM_LIGHT|ZRM_TEX_GEN)){
    if (renderMode & ZRM_TEX_GEN){
      num_light += 2;
    }
    gSPZMtxTrnsp3x3((*gpp) ++, GZM_MMTX);
    gSPZXfmLights((*gpp) ++, GZM_MMTX, UMEM_LIGHT, num_light);
  }
}

/*======== End of zsetmmtx.c ========*/
