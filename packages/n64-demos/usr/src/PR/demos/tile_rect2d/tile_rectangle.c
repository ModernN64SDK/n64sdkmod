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

/*
 *  This function will break up a large textured rectangle into 
 *  smaller rectangles and texture tiles that fit in Tmem.
 *
 *  This function creates 2D texture rectangles.
 *
 *  Assumptions:
 *		Mapping of texes to pixels is 1:1.
 *
 *	Limitations:
 *		Not all texture types are supported
 *		Smallest offset is 1-pixel.
 */
void
tileRectangle (	Gfx **glistp, 
					void *tex, u32 txlfmt, u32 txlsz,
					u32 txwd, u32 txht,
					u32 tilewd, u32 tileht,
					u32 s0, u32 t0,
					u32 s1, u32 t1,
					f32 x0, f32 y0 )
{
	Gfx				*gfx = *glistp;
	int				i, j;
	int				x, y;
	int				x1, y1, x2, y2;
	float			s, t;
	int				sld0, tld0, sld1, tld1;


	for(i = t0, y = y0; i < t1; i += tileht, y += tileht) {
		for(j = s0, x = x0; j < s1; j += tilewd, x += tilewd) {
			/* texture coordinates */
			s = (float)j - 0.5;
			t = (float)i - 0.5;

			/* load coordinates */
			sld0 = MAX(j - 1, 0);
			sld1 = MIN(j + tilewd, s1-1);
			tld0 = MAX(i - 1, 0);
			tld1 = MIN(i + tileht, t1-1);

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

			/* do texture rectangle */
			x1 = x;
			y1 = y;

			x2 = MIN(x+tilewd, x+(s1-j));
			x2 = MIN(x2, SCREEN_WD);
			y2 = MIN(y+tileht, y+(t1-i));
			y2 = MIN(y2, SCREEN_HT);
			
#ifdef DEBUG
			if(PrintEn) {
				osSyncPrintf("\n");
				osSyncPrintf("x1 = %d, y1 = %d, x2 = %d, y2 = %d\n", x1, y1, x2, y2);
				osSyncPrintf("\n");
			}
#endif

			x1 <<= 2;
			y1 <<= 2;
			x2 <<= 2;
			y2 <<= 2;

			gSPTextureRectangle ( gfx++,
				x1, y1,
            	x2, y2,
            	G_TX_RENDERTILE,
            	(int)(s*32) & 0xffff, (int)(t*32) & 0xffff, 
				1 << 10, 1 << 10);

		}
	}

	*glistp = gfx;
}

