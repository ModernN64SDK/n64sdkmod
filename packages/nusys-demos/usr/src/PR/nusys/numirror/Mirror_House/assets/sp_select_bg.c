/*		AUTOMATIC SPRITE
	Generated: Fri Jun 26 04:51:55 PDT 1998
	By command:  /usr/sbin/mksprite sp_SelectBG select_bg.rgb 64 32 0
	In directory: /usr/people/ohnishi

Sprite Name = sp_SelectBG
Image Name = select_bg.rgb
Sprite xsiz = 64
Sprite ysiz = 32
Image xsiz = 320
Image ysiz = 240
Overlap = 0
subimg select_bg.rgb tmpimg1642.rgb 0 63 208 239
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 64 127 208 239
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 128 191 208 239
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 192 255 208 239
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 256 319 208 239
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 0 63 176 207
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 64 127 176 207
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 128 191 176 207
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 192 255 176 207
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 256 319 176 207
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 0 63 144 175
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 64 127 144 175
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 128 191 144 175
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 192 255 144 175
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 256 319 144 175
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 0 63 112 143
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 64 127 112 143
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 128 191 112 143
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 192 255 112 143
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 256 319 112 143
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 0 63 80 111
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 64 127 80 111
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 128 191 80 111
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 192 255 80 111
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 256 319 80 111
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 0 63 48 79
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 64 127 48 79
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 128 191 48 79
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 192 255 48 79
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 256 319 48 79
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 0 63 16 47
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 64 127 16 47
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 128 191 16 47
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 192 255 16 47
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 256 319 16 47
tmpimg1642.rgb:	SGI imagelib image (64 x 32)
subimg select_bg.rgb tmpimg1642.rgb 0 63 0 15
tmpimg1642.rgb:	SGI imagelib image (64 x 16)
subimg select_bg.rgb tmpimg1642.rgb 64 127 0 15
tmpimg1642.rgb:	SGI imagelib image (64 x 16)
subimg select_bg.rgb tmpimg1642.rgb 128 191 0 15
tmpimg1642.rgb:	SGI imagelib image (64 x 16)
subimg select_bg.rgb tmpimg1642.rgb 192 255 0 15
tmpimg1642.rgb:	SGI imagelib image (64 x 16)
subimg select_bg.rgb tmpimg1642.rgb 256 319 0 15
tmpimg1642.rgb:	SGI imagelib image (64 x 16)
*/ 

#include <PR/sp.h>

#include "Texture/sp_selectbg_imgs.h"

Bitmap  sp_SelectBG_bitmaps[] = {
#include "Texture/sp_selectbg_bms.h"
};

#define NUM_sp_SelectBG_BMS  (sizeof(sp_SelectBG_bitmaps)/sizeof(Bitmap))

static Gfx      sp_SelectBG_dl[NUM_DL(NUM_sp_SelectBG_BMS)];

extern Sprite sp_SelectBG_sprite;

Sprite sp_SelectBG_sprite = {

	0,0,			/* Position: x,y */

	320,240,			/* Sprite size in texels (x,y) */

	1.0,1.0,		/* Sprite Scale: x,y */

	0,0,		/* Sprite Explosion Spacing: x,y */

	SP_TEXSHUF | SP_Z | SP_FASTCOPY,			/* Sprite Attributes */
	0x1234,			/* Sprite Depth: Z */

	255,255,255,255,	/* Sprite Coloration: RGBA */

	0,0,NULL,		/* Color LookUp Table: start_index, length, address */

	0,1,			/* Sprite Bitmap index: start index, step increment */

	NUM_sp_SelectBG_BMS,		/* Number of bitmaps */
	NUM_DL(NUM_sp_SelectBG_BMS),	/* Number of display list locations allocated */

	32, 32,			/* Sprite Bitmap Height: Used_height, physical height */
	G_IM_FMT_RGBA,		/* Sprite Bitmap Format */
	G_IM_SIZ_16b,		/* Sprite Bitmap Texel Size */

	sp_SelectBG_bitmaps,		/* Pointer to bitmaps */

	sp_SelectBG_dl,			/* Display list memory */

	NULL			/* next_dl pointer */

};
