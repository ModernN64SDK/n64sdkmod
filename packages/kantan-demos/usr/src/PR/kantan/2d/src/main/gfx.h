
#ifndef __GFX_H__
#define __GFX_H__

#ifndef _ULTRA64_H_
#include <ultra64.h>
#endif

/* ===========================================================================
	defines
 =========================================================================== */

#define ALIGNER(ptr,siz) ((u64*)((((u32)(ptr))+((siz)-1))&(~((siz)-1))))

/* ===========================================================================
	type definisions
 =========================================================================== */
typedef struct {
    float x,y,z;
} TripleF;

extern void *memcpy(void *d, const void *s, size_t len);

/* ===========================================================================
	public functions
 =========================================================================== */
void gfxInit(void);
Gfx *gfxBegin(int gfxcnt);
void gfxEnd(Gfx *gfx);
void gfxFlush(void);
void gfxWaitSync(void);

Gfx *gfxMatrix( Gfx *gp, Mtx *m, u32 flag );
Gfx *gfxPerspective( Gfx *gp, float fovy, float aspect, float near, float far, float scale );
Gfx *gfxOrtho( Gfx *gp, float left, float right, float bottom, float top, float near, float far, float scale );
Gfx *gfxLookAt( Gfx *gp, float ex, float ey, float ez, float ax, float ay, float az, float ux, float uy, float uz);
Gfx *gfxLoadMatrix( Gfx *gp, Mtx *m );
Gfx *gfxMultiMatrix( Gfx *gp, Mtx *m );
Gfx *gfxPushMatrix( Gfx *gp, Mtx *m );
Gfx *gfxPopMatrix( Gfx *gp );

#endif   /* #ifndef __GFX_H__ */
