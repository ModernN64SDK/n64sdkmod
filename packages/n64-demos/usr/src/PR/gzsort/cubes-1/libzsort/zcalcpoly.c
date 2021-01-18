/*---------------------------------------------------------------------
	Copyright (C) 1997, Nintendo.
	
	File		zcalcpoly.c
	Coded    by	Yoshitaka Yasumoto.	Sep 10, 1997.
	Modified by	
	
	$Id: zcalcpoly.c,v 1.5 1997/11/25 07:40:03 yasu Exp $
  ---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	"gzpoly.h"

extern	void	osDirtyDCache48(void *);

/*=========================================================================*
  Comments:	CPU 1st phase of ZSort ucode.
 *=========================================================================*/

/*---------------------------------------------------------------------------*
 *	Construct DL for calculation of vertices.
 *---------------------------------------------------------------------------*/
void	zCalcPoly(Gfx **gpp, zPoly *zpoly, u8 **wpp)
{  
  /*-----------------------------------------*
    Vertex calculation
   *-----------------------------------------*/
#ifdef	DEBUG
  if (!zpoly->vtx_ptr){
    osSyncPrintf("%s %d: zpoly->vtx_ptr is zero.\n", __FILE__, __LINE__);
  }
  if (((u32)zpoly->vtx_ptr)&7){		/* Check 8byte alignment */
    osSyncPrintf("%s %d: zpoly->vtx_ptr [%08x] is not aligned by 8.\n",
		 __FILE__, __LINE__, zpoly->vtx_ptr);
  }
  if (zpoly->vnum == 0){
    osSyncPrintf("%s %d: zpoly->vnum is zero.\n", __FILE__, __LINE__);
  }
#endif
  osDirtyDCache48(*gpp);  
  gSPZSetUMem((*gpp)++, UMEM_VSRC, zpoly->vnum * ZSIZE_VSRC, zpoly->vtx_ptr);
  gSPZMultMPMtx((*gpp)++, GZM_MPMTX, UMEM_VSRC, zpoly->vnum, UMEM_VDEST);
  
  /*-----------------------------------------*
    Set vertices ptr
   *-----------------------------------------*/
  gSPZGetUMem((*gpp)++, UMEM_VDEST, zpoly->vnum * ZSIZE_VDEST, *wpp);
  zpoly->intr.rsp_vtx = (zVtxDest *)(*wpp);	/* Save ptr */
  (*wpp) += zpoly->vnum * ZSIZE_VDEST;		/* Advance ptr for work area */
  
  /*-----------------------------------------*
    Lighting/Texture generating calculation
   *-----------------------------------------*/
  if (zpoly->renderMode & (ZRM_LIGHT|ZRM_TEX_GEN|ZRM_TEX_GEN_LINEAR)){
#ifdef	DEBUG
    if (!zpoly->norm_ptr){
      osSyncPrintf("%s %d: zpoly->norm_ptr is zero.\n", __FILE__, __LINE__);
    }
    if (((u32)zpoly->norm_ptr)&7){		/* Check 8byte alignment */
      osSyncPrintf("%s %d: zpoly->norm_ptr [%08x] is not aligned by 8.\n",
		   __FILE__, __LINE__, zpoly->norm_ptr);
    }
    if (zpoly->nnum == 0){
      osSyncPrintf("%s %d: zpoly->nnum is zero.\n", __FILE__, __LINE__);
    }
#endif
    gSPZSetUMem((*gpp)++, UMEM_NSRC, zpoly->nnum*ZSIZE_NSRC, zpoly->norm_ptr);
    if (zpoly->renderMode & ZRM_MATERIAL){
      /*====== Load materials and use them ======*/
#ifdef	DEBUG
      if (!zpoly->color_ptr){
	osSyncPrintf("%s %d: zpoly->color_ptr is zero.\n", __FILE__, __LINE__);
      }
      if (((u32)zpoly->color_ptr)&7){		/* Check 8byte alignment */
	osSyncPrintf("%s %d: zpoly->color_ptr [%08x] is not aligned by 8.\n",
		     __FILE__, __LINE__, zpoly->color_ptr);
      }
#endif
      gSPZSetUMem((*gpp)++,
		  UMEM_CSRC, zpoly->nnum * ZSIZE_CSRC, zpoly->color_ptr);
      gSPZLightMaterial((*gpp)++, UMEM_CSRC, UMEM_NSRC, zpoly->nnum,
			UMEM_CDEST, UMEM_TDEST);
    } else {
      /*====== Use default material ======*/
      gSPZLight((*gpp)++, UMEM_NSRC, zpoly->nnum, UMEM_CDEST, UMEM_TDEST);
    }
  }
  
  /*-----------------------------------------*
    Set colors 
   *-----------------------------------------*/
  if (zpoly->renderMode & ZRM_LIGHT){
    gSPZGetUMem((*gpp)++, UMEM_CDEST, zpoly->nnum * ZSIZE_CDEST, *wpp);
    zpoly->intr.rsp_color = (zColor *)(*wpp);		/* Save ptr     */
    (*wpp) += ALIGN(zpoly->nnum*ZSIZE_CDEST, 8);	/* Get next ptr */
  } else {
    zpoly->intr.rsp_color = zpoly->color_ptr;		/* Set ptr      */
  }
  
  /*-----------------------------------------*
    Set textures
   *-----------------------------------------*/
  if (zpoly->renderMode & ZRM_TEX_GEN){
    gSPZGetUMem((*gpp)++, UMEM_TDEST, zpoly->nnum * ZSIZE_TDEST, *wpp);
    zpoly->intr.rsp_txtr = (zTxtr *)(*wpp);		/* Save ptr     */
    (*wpp) += ALIGN(zpoly->nnum*ZSIZE_TDEST, 8);	/* Get next ptr */
  } else {
    zpoly->intr.rsp_txtr = zpoly->txtr_ptr;		/* Set ptr      */
  }
}

/*======== End of zcalcpoly.c ========*/
