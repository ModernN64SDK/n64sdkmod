/*---------------------------------------------------------------------
	Copyright (C) 1997, Nintendo.
	
	File		zdrawpoly_sh.c
	Coded    by	Yoshitaka Yasumoto.	Sep 10, 1997.
	Modified by	
	
	$Id: zdrawpoly_sh.c,v 1.5 1997/11/25 07:40:03 yasu Exp $
  ---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	"gzpoly.h"

extern	void	osDirtyDCache32(void *);
extern	void	osDirtyDCache40(void *);

static	zHeader *zDrawShTri_sub(zHeader *, s32, zVtxDest *, zVtxDest *,
				zVtxDest *, zColor, zColor, zColor);
static	zHeader	*zDrawShPoly_nearClip(zHeader *, s32, u32, zVtxDest *,
				      zVtxDest *, zVtxDest *, zVtxDest *,
				      zColor, zColor, zColor, zColor);
static	void	zDrawShNewVtx(zVtxDest *, zVtxDest *, zVtxDest *,
			      zColor, zColor, zColor *);


/*---------------------------------------------------------------------------*
 *	Draw smooth shaded triangle (phase 1)
 *---------------------------------------------------------------------------*/
zHeader	*zDrawShTri(zHeader *zheader,
		    zPoly *zpoly, s32 v0, s32 v1, s32 v2)
{
  /*=================================*
    > Polygon > Triangle > NoTexture
   *=================================*/
  zVtxDest	*v0p, *v1p, *v2p;
  zColor	 c0,   c1,   c2;
  s32		iw0, iw1, iw2, iw;
  s32		dx01, dy01, dx02, dy02;
#ifdef	REJECT_SMALL
  s32		dcross;
#endif

  /*--- Get pointers for vertices ---*/
  v0p = (zVtxDest *)(((u8 *)zpoly->intr.rsp_vtx) + (v0 << 4));
  v1p = (zVtxDest *)(((u8 *)zpoly->intr.rsp_vtx) + (v1 << 4));
  v2p = (zVtxDest *)(((u8 *)zpoly->intr.rsp_vtx) + (v2 << 4));  
  
  /*------ Check if outside screen ------*/
  /* If outside, reject it */
  if ((v0p->cc & v1p->cc & v2p->cc) != 0) return zheader;
  
  /*--- Get vector of 2 edges ---*/
  dx01 = v0p->sx - v1p->sx;  dy01 = v0p->sy - v1p->sy;
  dx02 = v0p->sx - v2p->sx;  dy02 = v0p->sy - v2p->sy;

#ifdef	REJECT_SMALL  
  /*--- Check if backface ---*/
  dcross = dx01 * dy02 - dx02 * dy01;  
  if (((zgc.polyMode & ZPM_CULLBACK) && dcross >= 0)){
    return zheader;
  }
  /*--- Check if too small (dcross = -256 to 255) ---*/
  if (((dcross + 256)>>9) == 0){
    return zheader;
  }
#else
  /*--- Check if backface ---*/
  if ((zgc.polyMode & ZPM_CULLBACK) && (dx01 * dy02 - dx02 * dy01 >= 0)){
    return zheader;
  }
#endif

  /*--- Get color0 ---*/
  c0 = *(zColor *)(((u8 *)zpoly->intr.rsp_color) + (v0 << 2));
  
  /*--- Check if smooth shaded ---*/
  if (zgc.polyMode & ZPM_SMOOTH){
    
    /*================================================*
      > Polygon > Triangle > NoTexture > Smooth shade
     *================================================*/
    /*------ Get color1,2 ------*/
    c1 = *(zColor *)(((u8 *)zpoly->intr.rsp_color) + (v1 << 2));
    c2 = *(zColor *)(((u8 *)zpoly->intr.rsp_color) + (v2 << 2));
    
  } else {
    
    /*==============================================*
      > Polygon > Triangle > NoTexture > Flat shade
     *==============================================*/
    /*--- Copy color0 to color1,2 ---*/
    c1 = c2 = c0;
  }
  
  /*------ Check if fog mode ------*/
  if (zgc.fogMode){
    /*------ Set fog parameter as alpha value ------*/
    c0.n.a = v0p->fog;
    c1.n.a = v1p->fog;
    c2.n.a = v2p->fog;
  }
  
  /*--- Get screen depth value from 1/W ---*/
  /*-----------------------------------------------------------------
    Use mips "sra" (shift 2) instruction instead of dividing by 3,
    Because DIVIDE instruction is very slow.
    Get 75% value of the average here.
    -----------------------------------------------------------------*/
  iw0 = v0p->invw / 4;		/* To avoid overflow */
  iw1 = v1p->invw / 4;	
  iw2 = v2p->invw / 4;
  iw  = iw0 + iw1 + iw2;
  
  /*------ Check if need near plane clipping ------*/
  if ((v0p->cc | v1p->cc | v2p->cc) & GZ_CC_NEAR){
    
    /*------ Clip at near plane ------*/
    return zDrawShPoly_nearClip(zheader, iw,
				3, v0p, v1p, v2p, v0p, c0,  c1,  c2, c0);
  }
  
  /*------ Go to phase 2 ------*/
  return zDrawShTri_sub(zheader, iw, v0p, v1p, v2p, c0, c1, c2);
}


/*---------------------------------------------------------------------------*
 *	Draw smooth shaded triangle (phase 2)
 *---------------------------------------------------------------------------*/
static	zHeader *zDrawShTri_sub(zHeader *zheader, s32 iw,
				zVtxDest *v0p, zVtxDest *v1p, zVtxDest *v2p,
				zColor c0, zColor c1, zColor c2)
{
  zShTri	*zobj = (zShTri *)zheader; 

  /*--- Set geometry and color values ---*/
  osDirtyDCache32(zobj);
  zobj->t.v[0].x = v0p->sx;
  zobj->t.v[1].x = v1p->sx;
  zobj->t.v[2].x = v2p->sx;
  zobj->t.v[0].y = v0p->sy;
  zobj->t.v[1].y = v1p->sy;
  zobj->t.v[2].y = v2p->sy;
  zobj->w.clr0 = c0.w;
  zobj->w.clr1 = c1.w;
  zobj->w.clr2 = c2.w;

  /*--- Set rendering mode ---*/
  zobj->t.rdpcmd1 = zgc.rdpcmd1;
  
  /*--- Entry zobject into Z Sort Array ---*/
  zobj->t.header = zArray_link((zHeader *)zobj, ZH_SHTRI, iw,
			       0.750f / ZARRAY_FACTOR);
  
  /*--- Writeback D-cache ---*/
  osWritebackDCache(zobj, sizeof(zShTri));

  return (zHeader *)(zobj+1);
}


/*---------------------------------------------------------------------------*
 *	Draw smooth shaded quadrangle.
 *---------------------------------------------------------------------------*/
zHeader	*zDrawShQuad(zHeader *zheader,
		     zPoly *zpoly, s32 v0, s32 v1, s32 v2, s32 r3)
{
  /*===================================*
    > Polygon > Quadrangle > NoTexture
   *===================================*/
  zVtxDest	*v0p, *v1p, *v2p, *v3p, *vtmp;
  zColor	 c0,   c1,   c2,   c3,   ctmp;
  zShQuad	*zobj = (zShQuad *)zheader; 
  s32		iw0, iw1, iw2, iw3, iw;
  s32		v3;
  s32		dx02, dy02, dx13, dy13;
#ifdef	REJECT_SMALL
  s32		dcross;
#endif

  /*--- Get pointers for vertices ---*/
  v3  = r3 & 0x7f;
  v0p = (zVtxDest *)(((u8 *)zpoly->intr.rsp_vtx) + (v0 << 4));
  v1p = (zVtxDest *)(((u8 *)zpoly->intr.rsp_vtx) + (v1 << 4));
  v2p = (zVtxDest *)(((u8 *)zpoly->intr.rsp_vtx) + (v2 << 4));
  v3p = (zVtxDest *)(((u8 *)zpoly->intr.rsp_vtx) + (v3 << 4));
  
  /*--- Check if outside screen ---*/
  /* If outside, reject it */
  if ((v0p->cc & v1p->cc & v2p->cc & v3p->cc) != 0) return zheader;
  
  /*--- Get vectors along diagonal lines ---*/
  dx02 = v0p->sx - v2p->sx;  dy02 = v0p->sy - v2p->sy;
  dx13 = v1p->sx - v3p->sx;  dy13 = v1p->sy - v3p->sy;

#ifdef	REJECT_SMALL
  /*--- Check if backface ---*/
  dcross = dx02 * dy13 - dx13 * dy02;
  if ((zgc.polyMode & ZPM_CULLBACK) && dcross >= 0){
    return zheader;
  }
  /*--- Check if too small (dcross = -256 to 255) ---*/
  if (((dcross + 256)>>9) == 0){
    return zheader;
  }
#else
  /*--- Check if backface ---*/
  if ((zgc.polyMode & ZPM_CULLBACK) && (dx02 * dy13 - dx13 * dy02) >= 0){
    return zheader;
  }
#endif

  /*--- Get color0 ---*/
  c0 = *(zColor *)(((u8 *)zpoly->intr.rsp_color) + (v0 << 2));
  
  /*--- Check if smooth shaded ---*/
  if (zgc.polyMode & ZPM_SMOOTH){
    
    /*==================================================*
      > Polygon > Quadrangle > NoTexture > Smooth shade
     *==================================================*/
    /*------ Get color1,2,3 ------*/
    c1 = *(zColor *)(((u8 *)zpoly->intr.rsp_color) + (v1 << 2));
    c2 = *(zColor *)(((u8 *)zpoly->intr.rsp_color) + (v2 << 2));
    c3 = *(zColor *)(((u8 *)zpoly->intr.rsp_color) + (v3 << 2));

  } else {

    /*================================================*
      > Polygon > Quadrangle > NoTexture > Flat shade
     *================================================*/
    /*--- Copy color0 to color1,2,3 ---*/
    c1 = c2 = c3 = c0;    
  }
  
  /*--- Check if fog mode ---*/
  if (zgc.fogMode){
    /*--- Set fog parameter as alpha value ---*/
    c0.n.a = v0p->fog;
    c1.n.a = v1p->fog;
    c2.n.a = v2p->fog;
    c3.n.a = v3p->fog;
  }
  
  /*--- Get screen depth value from 1/W ---*/
  /*-----------------------------------------------------------------
    Use mips "sra" (shift 2) instruction instead of dividing by 4,
    Because DIVIDE instruction is very slow.
    -----------------------------------------------------------------*/
  iw0 = v0p->invw / 4;		/* To avoid overflow */
  iw1 = v1p->invw / 4;	
  iw2 = v2p->invw / 4;
  iw3 = v3p->invw / 4;
  iw  = iw0 + iw1 + iw2 + iw3;
  
  /*--- Check if need near plane clipping ---*/
  if ((v0p->cc | v1p->cc | v2p->cc | v3p->cc) & GZ_CC_NEAR){
    
    /*--- Clip at near plane ---*/
    return zDrawShPoly_nearClip(zheader, iw, 
				4, v0p, v1p, v2p, v3p, c0, c1, c2, c3);
  }
  
  /*--- Check if dynamic divide ---*/
  if (r3 < 0){
    
    /*============================================================*
      > Polygon > Quadrangle > NoTexture > Smooth shade > Dynamic
     *============================================================*/
    /*--- Get length of diagonal lines ---*/
    if (dy02 < 0) dy02 = -dy02;
    if (dy13 < 0) dy13 = -dy13;

    /*--- Select better diagonal line ---*/
    if (dy02 < dy13){

      /*--- Divide 1-3 (0-1-3 and 1-3-2) ---*/
      /*--- Swap 0<->1, 2<->3 ---*/
      vtmp = v1p; v1p = v0p; v0p = vtmp;
      vtmp = v3p; v3p = v2p; v2p = vtmp;
      ctmp = c1;  c1  = c0;  c0  = ctmp;
      ctmp = c3;  c3  = c2;  c2  = ctmp;
    }
  }
  
  /*--- Divide 0-2 (1-0-2 and 0-2-3) ---*/	
  /*--- Set geometry and color values ---*/
  osDirtyDCache40(zobj);
  zobj->t.v[0].x = v1p->sx;
  zobj->t.v[1].x = v0p->sx;
  zobj->t.v[2].x = v2p->sx;
  zobj->t.v[3].x = v3p->sx;
  zobj->t.v[0].y = v1p->sy;
  zobj->t.v[1].y = v0p->sy;
  zobj->t.v[2].y = v2p->sy;
  zobj->t.v[3].y = v3p->sy;
  zobj->w.clr0 = c1.w;
  zobj->w.clr1 = c0.w;
  zobj->w.clr2 = c2.w;
  zobj->w.clr3 = c3.w;
  
  /*--- Set rendering mode ---*/
  zobj->t.rdpcmd1 = zgc.rdpcmd1;
  
  /*--- Entry zobject into Z Sort Array ---*/
  zobj->t.header = zArray_link((zHeader *)zobj, ZH_SHQUAD, iw,
			       1.000f/ZARRAY_FACTOR);
  
  /*--- Writeback D-cache ---*/
  osWritebackDCache(zobj, sizeof(zShQuad));
  return (zHeader *)(zobj+1);
}


/*---------------------------------------------------------------------------*
 *	Clip smooth shaded polygon (Near clipping).
 *---------------------------------------------------------------------------*/
static	zHeader	*zDrawShPoly_nearClip(zHeader *zheader, s32 iw,
				      u32 num, zVtxDest *v0p, zVtxDest *v1p,
				      zVtxDest *v2p, zVtxDest *v3p, zColor c0,
				      zColor c1, zColor c2, zColor c3)
{
  u8             cc, ccsv;
  s32            i, n;
  zVtxDest      *vtxsv;
  zColor         clrsv;
  zVtxDest	*clipVtx[6];
  zVtxDest	 clipInOut, clipOutIn;
  zColor	 clipClr[6];
  
  clipVtx[0] = v0p; clipClr[0] = c0;
  clipVtx[1] = v1p; clipClr[1] = c1;
  clipVtx[2] = v2p; clipClr[2] = c2;
  clipVtx[3] = v3p; clipClr[3] = c3;
  ccsv  = clipVtx[0]->cc & 0x04;
  vtxsv = clipVtx[0];
  clrsv = clipClr[0];
  n = 5;
  
  for (i = num-1; i >= 0; i --){

    /*--- Check if inner or outer of clip box ---*/
    if ((cc = clipVtx[i]->cc & 0x04) == 0){
      if (ccsv != 0){
        /*--- Make new vertex (Out->In) and add in array ---*/
        zDrawShNewVtx(clipVtx[i], vtxsv, &clipOutIn,
		      clipClr[i], clrsv, &clipClr[n]);
	clipVtx[n] = &clipOutIn;
	n --;
      } 
      /*--- Add original vertex in array ---*/
      clipVtx[n] = clipVtx[i];
      clipClr[n] = clipClr[i];
      n --;
    } else {
      if (ccsv == 0){
        /*--- Make new vertex (In->Out) and add in array ---*/
        zDrawShNewVtx(vtxsv, clipVtx[i], &clipInOut,
		      clrsv, clipClr[i], &clipClr[n]);
	clipVtx[n] = &clipInOut;
	n --;
      }
    }
    /*--- Save current vertex ---*/
    ccsv  = cc;
    vtxsv = clipVtx[i];
    clrsv = clipClr[i];
  }
  
  /*--- Draw clipped polygon ---*/
  for (i = 3; i > n; i --){
    /*--- Draw triangle (5, i, i+1) ---*/
    zheader = zDrawShTri_sub(zheader, iw,
			     clipVtx[5], clipVtx[i], clipVtx[i+1],
			     clipClr[5], clipClr[i], clipClr[i+1]);
  }
  return zheader;
}
 

/*---------------------------------------------------------------------------*
 *	Create new vertex for smooth shaded polygon.
 *---------------------------------------------------------------------------*/
static	void	zDrawShNewVtx(zVtxDest *vin, zVtxDest *vout, zVtxDest *vnew,
			      zColor    cin, zColor    cout, zColor   *cnew)
{
  s32           n, x, y, near;
  
  /*--- Get position of intersection ---*/
  near = (s32)(zgc.nearPlane);
  n = (vin->wi - near) * 65536 / (vin->wi - vout->wi);
  x = (vin->xi << 16) + (vout->xi - vin->xi) * n;
  y = (vin->yi << 16) + (vout->yi - vin->yi) * n;
  
  /*--- Translate to screen coordinates ---*/
  vnew->sx  = zgc.vtrans[0] + x / near * zgc.vscale[0] / 65536;
  vnew->sy  = zgc.vtrans[1] + y / near * zgc.vscale[1] / 65536;
  vnew->sy &= 0xfffc;
  
  /*--- Get new color by interpolation ---*/
  cnew->n.r = (u8)((s32)cin.n.r + ((s32)cout.n.r - (s32)cin.n.r) * n / 65536);
  cnew->n.g = (u8)((s32)cin.n.g + ((s32)cout.n.g - (s32)cin.n.g) * n / 65536);
  cnew->n.b = (u8)((s32)cin.n.b + ((s32)cout.n.b - (s32)cin.n.b) * n / 65536);
  cnew->n.a = (u8)((s32)cin.n.a + ((s32)cout.n.a - (s32)cin.n.a) * n / 65536);
}

/*======== End of zdrawpoly_sh.c ========*/
