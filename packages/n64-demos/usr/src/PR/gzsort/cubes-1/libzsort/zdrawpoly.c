/*---------------------------------------------------------------------
	Copyright (C) 1997, Nintendo.
	
	File		zdrawpoly.c
	Coded    by	Yoshitaka Yasumoto.	Sep 10, 1997.
	Modified by	
	
	$Id: zdrawpoly.c,v 1.4 1997/10/08 10:06:45 yasu Exp $
  ---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	"gzpoly.h"

/*=========================================================================*
  Comments:	CPU 2nd phase of ZSort ucode.
 *=========================================================================*/

/*---------------------------------------------------------------------------*
 *  Graphic context for ZSort library.
 *---------------------------------------------------------------------------*/
zGc	zgc;

/*---------------------------------------------------------------------------*
 * Setup paramater to get z index and init array.
 *---------------------------------------------------------------------------*/
void	zDrawSetup(Gfx **gp, Vp *vp, u16 near, u16 far)
{
  /*--- Save viewport and depth of clip planes ---*/
  zgc.vtrans[0] = vp->vp.vtrans[0];
  zgc.vtrans[1] = vp->vp.vtrans[1];
  zgc.vscale[0] = vp->vp.vscale[0];
  zgc.vscale[1] = vp->vp.vscale[1];
  zgc.nearPlane = near;
  zgc.farPlane  = far;
  zgc.invwMax   = ((s32)(near + far)) * 65536 / (4 * (s32)near);

  /*--- Initialize z bucket array ---*/
  zArray_open(gp, near, far);

  return;
}  

/*---------------------------------------------------------------------------*
 * Draw polygons
 *---------------------------------------------------------------------------*/
zHeader	*zDrawPoly(zHeader *zheader, zPoly *zpoly)
{
  s8	*face_ptr;
  s32	 v0,v1,v2,v3;
  u8	 fog_mode;
  
  face_ptr = zpoly->face_ptr;  
  zgc.fogMode = fog_mode = ((u8)zpoly->renderMode) & ZRM_FOG;
  
  /*--- Get param0 and Check if END of DATA ---*/
  while (-1 != (v0 = *(face_ptr ++))){
    
    /*--- Get param1 and Check if polygon ---*/
    v1 = *(face_ptr ++);
    if (v0 >= 0){
      
      /*==========*
	> Polygon
       *==========*/
      /*--- Get param2 and Check if triangle ---*/
      v2 = *(face_ptr ++);
      if (v1 >= 0){
	
	/*=====================*
	  > Polygon > Triangle
	 *=====================*/
	/*--- Check if textured ---*/
	if (zgc.polyMode & ZPM_TXTR){
	  
	  /*===============================*
	    > Polygon > Triangle > Texture
	   *===============================*/
	  zheader = zDrawTxTri(zheader, zpoly, v0, v1, v2);

	} else {

	  /*=================================*
	    > Polygon > Triangle > NoTexture
	   *=================================*/
	  zheader = zDrawShTri(zheader, zpoly, v0, v1, v2);
	}
      } else {

	/*=======================*
	  > Polygon > Quadrangle
	 *=======================*/
	/*--- Reset flag of param1 and Get param3 ---*/
	v1 &= 0x7f;		/* Clear Bit7 */
	v3 = *(face_ptr ++);
	
	/*--- Check if textured ---*/
	if (zgc.polyMode & ZPM_TXTR){
	  
	  /*=================================*
	    > Polygon > Quadrangle > Texture
	   *=================================*/
	  zheader = zDrawTxQuad(zheader, zpoly, v0, v1, v2, v3);

	} else {

	  /*===================================*
	    > Polygon > Quadrangle > NoTexture
	   *===================================*/
	  zheader = zDrawShQuad(zheader, zpoly, v0, v1, v2, v3);
	}
      }
    } else {
      
      /*==========*
	> SetMode
       *==========*/
      
      /*--- Update poly type and RDP command 1 ---*/
      zgc.polyMode = v0;
      zgc.rdpcmd1  = *(zpoly->gfx_ptr + v1);      
      
      /*--- Set fog mode ---*/
      zgc.fogMode = (zgc.polyMode ^ ZPM_FOG_OFF) & fog_mode;
      
      /*--- Check if textured and update RDP command 2,3 ---*/
      if (v0 & ZPM_TXTR){
	v2 = *(face_ptr ++);
	v3 = *(face_ptr ++);
	zgc.rdpcmd2 = *(zpoly->gfx_ptr + v2);
	zgc.rdpcmd3 = *(zpoly->gfx_ptr + v3);
      }
    }
  }
  return zheader;
}

/*======== End of zdrawpoly.c ========*/
