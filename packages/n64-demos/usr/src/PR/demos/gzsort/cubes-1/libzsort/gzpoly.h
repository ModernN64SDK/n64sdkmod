/*---------------------------------------------------------------------
	Copyright (C) 1997, Nintendo.
	
	File		zpoly.h
	Coded    by	Yoshitaka Yasumoto.	Sep  3, 1997.
	Modified by	
	
	$Id: gzpoly.h,v 1.5 1997/10/08 10:06:43 yasu Exp $
  ---------------------------------------------------------------------*/
#include	<PR/gzsort.h>

#ifndef	_ZPOLY_H_
#define	_ZPOLY_H_

/*===========================================================================

  Tri   +-+--------+ +-+--------+ +-+--------+
        |0|  Vtx0  | |0|  Vtx1  | |0|  Vtx2  |
        +-+--------+ +-+--------+ +-+--------+

  Quad  +-+--------+ +-+--------+ +-+--------+ +-+--------+
        |0|  Vtx0  | |1|  Vtx1  | |0|  Vtx2  | |*|  Vtx3  |
        +-+--------+ +-+--------+ +-+--------+ +-+--------+
	      * 0:Static 1:Dynamic divide

  Mode1 +-+-+------+ +----------+
        |1|0| Mode | |  zGfx1   |
        +-+-+------+ +----------+

  Mode3 +-+-+------+ +----------+ +----------+ +----------+
        |1|1| Mode | |  zGfx1   | |  zGfx2   | |  zGfx3   |
        +-+-+------+ +----------+ +----------+ +----------+
	Mode[1:0] 00:NoShade 01:Flat 10:Smooth

  End   +-+-+------+
        |1|1|111111|
        +-+-+------+
  ===========================================================================*/
#define	zsV(v,f)                   ((u8)((v)|((f)<<7)))
#define	zsM(v,f)                   ((u8)((v)|((f)<<6)))
#define	zsTriangle(v0,v1,v2)       zsV(v0,0),zsV(v1,0),zsV(v2,0)
#define	zsQuadrangleS(v0,v1,v2,v3) zsV(v0,0),zsV(v1,1),zsV(v2,0),zsV(v3,0)
#define	zsQuadrangleD(v0,v1,v2,v3) zsV(v0,0),zsV(v1,1),zsV(v2,0),zsV(v3,1)
#define	zsPoly(md,g1)	           zsM(md,2),zsV(g1,0)
#define	zsPolyTxtr(md,g1,g2,g3)    zsM(md,3),zsV(g1,0),zsV(g2,0),zsV(g3,0)
#define	zsEnd()                    ((u8)0xff)

#define	ZPM_NULL		0x00
#define	ZPM_FLAT		0x01
#define	ZPM_SMOOTH		0x02
#define	ZPM_FOG_OFF		0x04
#define	ZPM_CULLBACK		0x08
#define	ZPM_TXTR		0x40

#define	ZRM_NULL		0x00
#define	ZRM_TEX_GEN_LINEAR	0x01
#define	ZRM_TEX_GEN		0x02
#define	ZRM_FOG			0x04	/* FOG=ZRM_FOG & (!ZPM_FOG_OFF) */
#define	ZRM_MATERIAL		0x08
#define	ZRM_LIGHT		0x10

typedef struct	{  u8	v0, v1, v2;	} zTri;
typedef struct  {  u8	v0, v1, v2, v3;	} zQuad;
typedef	s8	zFace;

typedef	struct	{
  u16		 renderMode;	/* Rendering status       */
  u8		 vnum;		/* Num of vertices        */
  u8		 nnum;		/* Num of normals         */
  zVtxSrc	*vtx_ptr;	/* Ptr for vertices       */
  zColor	*color_ptr;	/* Ptr for colors         */
  zNorm		*norm_ptr;	/* Ptr for normals        */
  zTxtr		*txtr_ptr;	/* Ptr for textures       */
  zFace		*face_ptr;	/* Ptr for faces          */
  Gfx	       **gfx_ptr;	/* Ptr for rdpcmd arrays  */
  struct {
    zVtxDest	*rsp_vtx;
    zColor	*rsp_color;
    zTxtr	*rsp_txtr;
  } intr;
} zPoly;

typedef	struct	{
  s16		vtrans[2];	/* Save viewPort           */
  s16		vscale[2];	/* Save viewPort           */
  u16		nearPlane;	/* Save near plane         */
  u16		farPlane;	/* Save far  plane         */
  s32		invwMax;	
  Gfx		*rdpcmd1;
  Gfx		*rdpcmd2;
  Gfx		*rdpcmd3;
  u8		polyMode;	/* Save polygon mode       */
  u8		fogMode;	/* Fog mode Enable/Disable */
} zGc;

/*---------------------------------------------------------------------------*
 *  Ambient =  8Bytes
 *  Defuse  = 24Bytes/Light
 *
 *	MAX_DEFUSE     MAX_VTX
 *	No Ambient      128
 *          0           127
 *	    1           126
 *	    2           124
 *	    3           123
 *	    4           121
 *	    5           120
 *	    6           118
 *	    7           117
 *	    8           115
 *---------------------------------------------------------------------------*/
#define	MAX_DEFUSE	 5			 /* 1L=1.5V -> 5L=7.5V */
#define	UMEM_LIGHT	(2048-MAX_DEFUSE*ZSIZE_LIGHT-ZSIZE_AMBIENT)
#define	MAX_VTX		(UMEM_LIGHT/ZSIZE_VDEST) /* if L=5, Vtx=120. */
#define	UMEM_VSRC	((UMEM_LIGHT-MAX_VTX*ZSIZE_VSRC)&(~7))
#define	UMEM_VDEST	0
#define	UMEM_NSRC	0
#define	UMEM_CSRC	512
#define	UMEM_CDEST	512
#define	UMEM_TDEST	1024
#define	ZARRAY_FACTOR	65536.0f	/* Fixed */
#define	ZPACKET_LVL	1024		/* multiple of 4 */

/*---------------------------------------------------------------------*
  E X T E R N A L S
 *---------------------------------------------------------------------*/
extern	zGc	 zgc;
extern	void	 zDrawSetup(Gfx **, Vp *, u16, u16);
extern	zHeader	*zDrawPoly(zHeader *, zPoly *);
extern	void	 zSetMMtx(Gfx **, Mtx *, u32, u16);
extern	void	 zCalcPoly(Gfx **, zPoly *, u8 **);
extern	zHeader	*zDrawShTri(zHeader *, zPoly *, s32, s32, s32);
extern	zHeader	*zDrawShQuad(zHeader *, zPoly *, s32, s32, s32, s32);
extern	zHeader	*zDrawTxTri(zHeader *, zPoly *, s32, s32, s32);
extern	zHeader	*zDrawTxQuad(zHeader *, zPoly *, s32, s32, s32, s32);
extern	void	 zArray_open(Gfx **, u16, u16);
extern	void	 zArray_close(void);
extern	zHeader	*zArray_link(zHeader *, u32, s32, float);

#endif		/* _ZPOLY_H_ */

/*======== End of zpoly.h ========*/
