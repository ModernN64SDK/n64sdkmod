/*---------------------------------------------------------------------*
	Copyright (C) 1997, Nintendo.
	
	File		cube.c
	Coded    by	Yoshitaka Yasumoto.	Sep 12, 1997.
	Modified by	
	
	$Id: cube.c,v 1.5 1997/10/08 10:06:30 yasu Exp $
 *---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	"gzpoly.h"
#include	"system.h"
#include	"action.h"
#include	"cube.h"

s16		rot  = 0;
s16		zpos = 800;
Dynamic		dynamic;
u16		perspNorm;
u64		outbuffer[SIZEOF_CUBEDEST*CUBE_NUM/sizeof(u64)+256];
/* Max size of cube object is 3 clipped face(each face consist of 3 tri) and
   3 Quadrangle. (=64*3*3+80*3) */
u64		zobjbuf[(64*3*3+80*3)*CUBE_NUM/sizeof(u64)];

void	cubeInit(void)
{
  s32	i, n;

  n = 0;
  for (i = CUBE_TYPE; i < CUBE_NUM; i ++){
    cube_poly[i] = cube_poly[n];
    n ++;
    if (n == CUBE_TYPE) n = 0;
  }
#ifdef	FOG
  for (i = 0; i < CUBE_NUM; i ++){
    cube_poly[i].renderMode |= ZRM_FOG;
  }
#endif
}

#define	Move(mtx,x,y,z)	{ (mtx).m[1][2] = ((x)<<16)|((u32)(y)&0xffff);	\
			  (mtx).m[1][3] = ((z)<<16)| 1 ;		\
			  (mtx).m[3][2] = (mtx).m[3][3] = 0; }

void	cubePass1(Gfx **gp)
{
  u32	i, m, n, outp;
  s32	z;
  Mtx	rotate[4];
  float	 deg;
  static s32 dx[] =
  {0,-200,-100,   0, 100,  200, 200, 200, 100,   0, -100,-200,-200,-100, 100 };
  static s32 dy[] =
  {0,-120,-120,-120,-120, -120,   0, 120, 120, 120,  120, 120,   0,   0,   0 };
  static s32 flag = 0;
  
  /*------ Controller Operation ------*/
  if (Ac.pad[0].push & START_BUTTON) flag ^= 1;
  if (flag) rot ++;
  if (Ac.pad[0].button & L_TRIG) rot ++;
  if (Ac.pad[0].button & R_TRIG) rot --;
  rot %= 360;
  if (Ac.pad[0].button & U_JPAD) zpos -= 2;
  if (Ac.pad[0].button & D_JPAD) zpos += 2;
  
  /*------ Set Perspective Array ------*/
  guPerspective(&dynamic.projection, &perspNorm, 33,
		(float)SCREEN_WD/(float)SCREEN_HT,
		(float)CLIP_NEAR, (float)CLIP_FAR, 1.0);
#if 0
  guLookAt(&dynamic.viewing, 0, 0, zpos,  0, 0, zpos-10,  0, 1, 0);
#else
  guLookAtReflect(&dynamic.viewing, &dynamic.lookat,
		  0, 0, zpos,  0, 0, zpos-10,  0, 1, 0);
  guZFixLookAt(&dynamic.lookat);
#endif
  
  /*------ Set Model Array ------*/
  deg = (float)rot;
  for (i = 0; i < 4; i ++){
    guRotate(rotate+i, deg, 0.2, 1.0, 0.7);
    deg += 22.5f;
  }
  m = n = 0;
  z = 50;
  for (i = 0; i < CUBE_NUM; i ++){
    dynamic.modeling[i] = rotate[m];
    Move(dynamic.modeling[i], dx[n], dy[n], z);
    z -= 20;
    if (m ++ == 3) m = 0;
    if (n ++ == (sizeof(dx)/sizeof(dx[0])-1)) n = 0;
  }
  
  /*--- WriteBack Dcache ---*/
  osWritebackDCache(&dynamic, sizeof(Dynamic));
  
  /*--- Set ViewPort Projection Viewing ---*/
  gSPZViewport((*gp) ++, &viewport);
  gSPZPerspNormalize((*gp) ++, perspNorm);  
  gSPZSetMtx((*gp) ++, GZM_MMTX, &dynamic.projection);
  gSPZSetMtx((*gp) ++, GZM_PMTX, &dynamic.viewing);
  gSPZMtxCat((*gp) ++, GZM_PMTX, GZM_MMTX, GZM_PMTX);
  
  /*--- Load light parameter ---*/
  gSPZSetAmbient((*gp)++, UMEM_LIGHT,    &scene_light.a);
  gSPZSetDefuse ((*gp)++, UMEM_LIGHT, 0, &scene_light.l[0]);

  /*--- Load reflection parameter ---*/
  gSPZSetLookAt ((*gp)++, UMEM_LIGHT, 1, &dynamic.lookat);
  
  /*--- Generate Display List for vertices of each cube ---*/
  outp = (u32)outbuffer;
  for (i = 0; i < CUBE_NUM; i ++){

    /*--- Set Modeling Matrix ---*/
    zSetMMtx(gp, &dynamic.modeling[i], 1, cube_poly[i].renderMode);
    
    /*--- zPoly calculation ---*/
    zCalcPoly(gp, cube_poly+i, (u8 **)&outp);
  }
  
  /*--- Invalid cache ---*/
  osInvalDCache(outbuffer, outp - (u32)outbuffer);
}


void	cubePass2(Gfx **gp)
{
  zHeader	*zobj = (zHeader *)zobjbuf;
  u32		i;
  
  /*--- Initialize Drawing Controller  ---*/
  /*--- + Set   viewport for Clipping  ---*/
  /*--- + Setup bucket array for ZSort ---*/
  zDrawSetup(gp, &viewport, CLIP_NEAR, CLIP_FAR);
  
  /*--- Register ZObject ----*/
  for (i = 0; i < CUBE_NUM; i ++){
    zobj = zDrawPoly(zobj, cube_poly+i);
  }

  /* Close a bucket for Z-sorting */
  zArray_close();
}

/*======== End of cube.c ========*/
