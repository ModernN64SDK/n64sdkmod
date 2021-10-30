/*======================================================================*/
/*		NIFFprev						*/
/*		gfx.h							*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
#ifndef __GFX_H__
#define __GFX_H__

#ifndef _ULTRA64_H_
#include <ultra64.h>
#endif

/* ===========================================================================
	defines
 =========================================================================== */

#define	GFXLEN_GLIST	2048
#define GFXLEN_MTX	80
#define GFXLEN_LIGHT	50

#define ALIGNER(ptr,siz) ((u64*)((((u32)(ptr))+((siz)-1))&(~((siz)-1))))

/* ===========================================================================
	type definisions and prototypes
 =========================================================================== */
#ifndef _WIN32
extern void *memcpy(void *d, const void *s, int  len);
#endif

/* ===========================================================================
	public functions
 =========================================================================== */
extern u32 v_count;
extern int perfmonflg;
extern int building_gb;
extern int drawing_gb;

void gfxInit(void);
Gfx *gfxBegin( void );
void gfxEnd(Gfx *gfx);
void gfxFlush(u32 flag, u32 ucode);
void gfxWaitSync(void);

Gfx *gfxMatrix( Gfx *gp, Mtx *m, u32 flag );
Gfx *gfxPerspective( Gfx *gp, float fovy, float aspect, float near, float far, float scale );
Gfx *gfxMulPerspective( Gfx *gp, float fovy, float aspect, float near, float far, float scale, float mul[4][4] );
Gfx *gfxOrtho( Gfx *gp, float left, float right, float bottom, float top, float near, float far, float scale );
Gfx *gfxLookAtReflect( Gfx *gp, s8 *right, s8 *upper);
Gfx *gfxLoadMatrix( Gfx *gp, Mtx *m );
Gfx *gfxMultiMatrix( Gfx *gp, Mtx *m );
Gfx *gfxPushMatrix( Gfx *gp, Mtx *m );
Gfx *gfxPopMatrix( Gfx *gp );

Gfx *ndgSPLightColor(Gfx *gp, int lnum, u32 color);
Gfx *gfxSetLight( Gfx *gp, int num, u32 color, s8 *normal );
Gfx *gfxSetAmbient( Gfx *gp, int num, u32 color );

#endif   /* #ifndef __GFX_H__ */
