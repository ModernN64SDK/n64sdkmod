/*---------------------------------------------------------------------*
	Copyright (C) 1997, Nintendo.
	
	File		cube.h
	Coded    by	Yoshitaka Yasumoto.	Sep 12, 1997.
	Modified by	
	
	$Id: cube.h,v 1.3 1997/10/08 10:06:31 yasu Exp $
 *---------------------------------------------------------------------*/
#include	"gzpoly.h"

#define	CUBE_VTX	8
#define	CUBE_FACE	6
#ifndef	xDEBUG
#define	CUBE_NUM	100
#else
#define	CUBE_NUM	1
#endif
#define	CUBE_TYPE	15
#define	CUBE_ARRAY_MAX	(CUBE_NUM>CUBE_TYPE ? CUBE_NUM : CUBE_TYPE)

#define	SIZEOF_VTXSRC	6
#define	SIZEOF_VTXDEST	16
#define	SIZEOF_CDEST	4
#define	SIZEOF_TDEST	4
#define	SIZEOF_CUBEDEST	(CUBE_VTX*(SIZEOF_VTXDEST+SIZEOF_CDEST+SIZEOF_TDEST))

#define	DMEM_VTXSRC	0
#define	DMEM_VTXDEST	((CUBE_VTX*SIZEOF_VTXSRC+7)&~7)
#define	DMEM_MAX_VTX	((2048-DMEM_VTXDEST)/SIZEOF_VTXDEST)
#define	DMEM_MAX_CUBE	(DMEM_MAX_VTX/CUBE_VTX)

typedef	struct	{
  Mtx		projection;
  Mtx		viewing;
  LookAt	lookat;
  Mtx		modeling[CUBE_NUM];
} Dynamic;

extern	Dynamic	dynamic;
extern	zPoly	cube_poly[];
extern	Vp	viewport;
extern	Lights1	scene_light;

/*======== End of cube.h ========*/
