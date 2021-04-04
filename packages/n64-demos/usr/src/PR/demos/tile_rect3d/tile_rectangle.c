/**************************************************************************
 *									  *
 *		 Copyright (C) 1994, Silicon Graphics, Inc.		  *
 *									  *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright law.  They  may  not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *									  *
 **************************************************************************/

#include "ultra64.h"
#include "tile_rect.h"
#include "controller.h"

#define V(p, x, y, z, f, s, t, r, g, b, a) {                \
    (p)->v.ob[0] = (x)&0xffff;              \
    (p)->v.ob[1] = (y)&0xffff;              \
    (p)->v.ob[2] = (z)&0xffff;              \
    (p)->v.flag  = (f);                     \
    (p)->v.tc[0] = (s)&0xffff;              \
    (p)->v.tc[1] = (t)&0xffff;              \
    (p)->v.cn[0] = (r);                     \
    (p)->v.cn[1] = (g);                     \
    (p)->v.cn[2] = (b);                     \
    (p)->v.cn[3] = (a);                     \
}

extern int PrintEn;

#ifdef DEBUG
static void
print_vert(s16 x, s16 y, s16 z, s16 s, s16 t)
{
	osSyncPrintf("x = %3d, y = %3d, z = %3d, ", x, y, z);
	osSyncPrintf("s = %4x (%3.5f), t = %4x (%3.5f)\n", 
		s & 0xffff, (float)s/0x20, t & 0xffff, (float)t/0x20);
}
#endif


/*
 *  This function will break up a large textured rectangle into 
 *  smaller rectangles (two triangles) and texture tiles that fit in Tmem.
 *  Extra border texels are loaded in order to prevent seams in the texture
 *  tiles.
 *
 *  Assume rectangle lies on one of the x/y/z planes.
 *  Assume the rectangle is the same scale as the texture, i.e. 1:1 mapping.
 *  Assume integer number of tiles in texture
 *  Assume a texture scale of 0x8000 is used, so texture coordinates will
 *		have twice the magnitude.
 */
void
tileRectangle (	Vtx **vlistp, Gfx **glistp, 
					void *tex, u32 txlfmt, u32 txlsz,
					u32 txwd, u32 txht,
					u32 tilewd, u32 tileht,
					f32 x0, f32 y0, f32 z0, char plane )
{
	Vtx				*vtx = *vlistp;
	Gfx				*gfx = *glistp;
	int				i, j;
	int				x, y, xi, yi;
	int				x1, y1, z1, x2, y2, z2;
	int				x3, y3, z3, x4, y4, z4;
	int				s1, t1, s2, t2, s3, t3, s4, t4;
	float			s, t;
	int				sld0, tld0, sld1, tld1;

#ifdef DEBUG
	if(PrintEn)
		osSyncPrintf("=============================\n");
#endif

	if(plane == 'x') {
		xi = z0;
		yi = y0;
	} else if(plane == 'y') {
		xi = x0;
		yi = z0;
	} else if(plane == 'z') {
		xi = x0;
		yi = y0;
	}
		
	for(i = 0, y = yi; i < txht; i += tileht, y += tileht) {
		for(j = 0, x = xi; j < txwd; j += tilewd, x += tilewd) {
			/* texture coordinates */
			s = (float)j - 0.5;
			t = (float)i - 0.5;

			/* load coordinates */
			sld0 = MAX(j - 1, 0);
			sld1 = MIN(j + tilewd, txwd-1);
			tld0 = MAX(i - 1, 0);
			tld1 = MIN(i + tileht, txht-1);

			/* do load command */
			if(txlsz == G_IM_SIZ_4b) {	
				gDPLoadTextureTile_4b ( gfx++, tex, txlfmt,
					txwd, txht, 
					sld0, tld0, sld1, tld1,
					0,
					G_TX_CLAMP, G_TX_CLAMP, 
					G_TX_NOMASK, G_TX_NOMASK,
					G_TX_NOLOD, G_TX_NOLOD);
			} else {
				gDPLoadTextureTile ( gfx++, tex, txlfmt, G_IM_SIZ_16b,
					txwd, txht, 
					sld0, tld0, sld1, tld1,
					0,
					G_TX_CLAMP, G_TX_CLAMP, 
					G_TX_NOMASK, G_TX_NOMASK,
					G_TX_NOLOD, G_TX_NOLOD);
			}

			/* create verts and triangles */
			switch(plane) {
				case 'X':
				case 'x':
					x1 = x2 = x3 = x4 = x0;
					y1 = y;
					z1 = x;
					y2 = y+tileht;
					z2 = x+tilewd;
					y3 = y1;	
					z3 = z2;	
					y4 = y2;	
					z4 = z1;	
					break;

				case 'Y':
				case 'y':
					x1 = x;
					y1 = y2 = y3 = y4 = y0;
					z1 = y;
					x2 = x+tilewd;
					z2 = y+tileht;
					x3 = x2;	
					z3 = z1;	
					x4 = x1;	
					z4 = z2;	
					break;

				case 'Z':
				case 'z':
					x1 = x;
					y1 = y;
					z1 = z2 = z3 = z4 = z0;
					x2 = x+tilewd;
					y2 = y+tileht;
					x3 = x2;
					y3 = y1;
					x4 = x1;
					y4 = y2;
					break;
			}

			/* tex coords are scaled x2 because rsp scale is set to 0.5 */
			s1 = (int)(s*64) & 0xffff;
			t1 = (int)(t*64) & 0xffff;
			s2 = (int)((s+tilewd)*64) & 0xffff;
			t2 = (int)((t+tileht)*64) & 0xffff;
			s3 = s2;
			t3 = t1;
			s4 = s1;
			t4 = t2;

#ifdef DEBUG
			if(PrintEn) {
				osSyncPrintf("\n");
				osSyncPrintf("vert 0: "); print_vert(x1, y1, z1, s1, t1);
				osSyncPrintf("vert 1: "); print_vert(x2, y2, z2, s2, t2);
				osSyncPrintf("vert 2: "); print_vert(x3, y3, z3, s3, t3);
				osSyncPrintf("vert 3: "); print_vert(x4, y4, z4, s4, t4);
				osSyncPrintf("\n");
			}
#endif

			/* around rectangle in clockwise order */
			V(vtx, x1, y1, z1, 0, s1, t1, 255, 0, 255, 255);
			vtx++;
	
			V(vtx, x3, y3, z3, 0, s3, t3, 255, 255, 0, 255);
			vtx++;
	
			V(vtx, x2, y2, z2, 0, s2, t2, 0, 0, 255, 255);
			vtx++;
	
			V(vtx, x4, y4, z4, 0, s4, t4, 255, 255, 255, 255);
			vtx++;

			gSPVertex(gfx++, (vtx-4), 4, 0);	
			gSP1Triangle(gfx++, 0, 1, 2, 2);
			gSP1Triangle(gfx++, 0, 2, 3, 2);

		}
	}

	*vlistp = vtx;
	*glistp = gfx;
}

