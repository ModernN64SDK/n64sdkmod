/*		AUTOMATIC SPRITE
	Generated: June 22, 2000 (Thursday)  19:57:57 JST 
	By command:  /exdisk2/home/n64os/mdev2/usr/sbin/mksprite ball ball.rgba 15 15 1
	In directory: /exdisk2/home/n64os/mdev2/PR/apps.released/spgame/Texture

Sprite Name = ball
Image Name = ball.rgba
Sprite xsiz = 15
Sprite ysiz = 15
Image xsiz = 61
Image ysiz = 61
Overlap = 1
subimg ball.rgba tmpimg18298.rgb 0 15 45 60
tmpimg18298.rgb:	SGI imagelib image (16  16)
subimg ball.rgba tmpimg18298.rgb 15 30 45 60
tmpimg18298.rgb:	SGI imagelib image (16  16)
subimg ball.rgba tmpimg18298.rgb 30 45 45 60
tmpimg18298.rgb:	SGI imagelib image (16  16)
subimg ball.rgba tmpimg18298.rgb 45 60 45 60
tmpimg18298.rgb:	SGI imagelib image (16  16)
subimg ball.rgba tmpimg18298.rgb 0 15 30 45
tmpimg18298.rgb:	SGI imagelib image (16  16)
subimg ball.rgba tmpimg18298.rgb 15 30 30 45
tmpimg18298.rgb:	SGI imagelib image (16  16)
subimg ball.rgba tmpimg18298.rgb 30 45 30 45
tmpimg18298.rgb:	SGI imagelib image (16  16)
subimg ball.rgba tmpimg18298.rgb 45 60 30 45
tmpimg18298.rgb:	SGI imagelib image (16  16)
subimg ball.rgba tmpimg18298.rgb 0 15 15 30
tmpimg18298.rgb:	SGI imagelib image (16  16)
subimg ball.rgba tmpimg18298.rgb 15 30 15 30
tmpimg18298.rgb:	SGI imagelib image (16  16)
subimg ball.rgba tmpimg18298.rgb 30 45 15 30
tmpimg18298.rgb:	SGI imagelib image (16  16)
subimg ball.rgba tmpimg18298.rgb 45 60 15 30
tmpimg18298.rgb:	SGI imagelib image (16  16)
subimg ball.rgba tmpimg18298.rgb 0 15 0 15
tmpimg18298.rgb:	SGI imagelib image (16  16)
subimg ball.rgba tmpimg18298.rgb 15 30 0 15
tmpimg18298.rgb:	SGI imagelib image (16  16)
subimg ball.rgba tmpimg18298.rgb 30 45 0 15
tmpimg18298.rgb:	SGI imagelib image (16  16)
subimg ball.rgba tmpimg18298.rgb 45 60 0 15
tmpimg18298.rgb:	SGI imagelib image (16  16)
*/ 

#include <PR/sp.h>

#include "Texture/ball_imgs.h"

static Bitmap  ball_bitmaps[] = {

#include "Texture/ball_bms.h"

};

#define NUM_ball_BMS  (sizeof(ball_bitmaps)/sizeof(Bitmap))

static Gfx      ball_dl[NUM_DL(NUM_ball_BMS)];

extern Sprite ball_sprite;

Sprite ball_sprite = {

	0,0,			/* Position: x,y */

	60,60,			/* Sprite size in texels (x,y) */

	1.0,1.0,		/* Sprite Scale: x,y */

	0,0,		/* Sprite Explosion Spacing: x,y */

	SP_TEXSHUF | SP_Z | SP_OVERLAP,			/* Sprite Attributes */
	0x1234,			/* Sprite Depth: Z */

	255,255,255,255,	/* Sprite Coloration: RGBA */

	0,0,NULL,		/* Color LookUp Table: start_index, length, address */

	0,1,			/* Sprite Bitmap index: start index, step increment */

	NUM_ball_BMS,		/* Number of bitmaps */
	NUM_DL(NUM_ball_BMS),	/* Number of display list locations allocated */

	15, 16,			/* Sprite Bitmap Height: Used_height, physical height */
	G_IM_FMT_RGBA,		/* Sprite Bitmap Format */
	G_IM_SIZ_16b,		/* Sprite Bitmap Texel Size */

	ball_bitmaps,		/* Pointer to bitmaps */

	ball_dl,			/* Display list memory */

	NULL			/* next_dl pointer */

};
