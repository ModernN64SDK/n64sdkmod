
/**************************************************************************
 *                                                                        *
 *               Copyright (C) 1995, Silicon Graphics, Inc.               *
 *                                                                        *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright  law.  They  may not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *                                                                        *
 *************************************************************************/

#include <ultra64.h>
#include "block.h"
#include "static.h"

/* Remember, viewport structures have 2 bits of fraction in them */
static Vp vp = 
{
  SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* scale */
  SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* translate */
};

/* initialize the RSP state: */
Gfx rspinit_dl[] = 
{
  gsSPViewport(&vp),
  gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
			G_FOG | G_LIGHTING | G_TEXTURE_GEN |
			G_TEXTURE_GEN_LINEAR | G_LOD | G_ZBUFFER),
  gsSPTexture(0, 0, 0, 0, G_OFF),
  gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK),
  gsSPEndDisplayList(),
};

/* initialize the RDP state: */
Gfx rdpinit_dl[] = 
{
  gsDPSetCycleType(G_CYC_1CYCLE),
  gsDPPipelineMode(G_PM_1PRIMITIVE),
  gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),
  gsDPSetTextureLOD(G_TL_TILE),
  gsDPSetTextureLUT(G_TT_NONE),
  gsDPSetTextureDetail(G_TD_CLAMP),
  gsDPSetTexturePersp(G_TP_PERSP),
  gsDPSetTextureFilter(G_TF_BILERP),
  gsDPSetTextureConvert(G_TC_FILT),
  gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
  gsDPSetCombineKey(G_CK_NONE),
  gsDPSetAlphaCompare(G_AC_NONE),
  gsDPSetColorDither(G_CD_BAYER),
  gsDPSetMaskImage(zbuffer),
  gsDPPipeSync(),
  gsSPEndDisplayList(),
};

static Vtx background_vtx[] =  
{
  {       20,   20, -1500, 0, 0, 0,    0, 0xff,    0, 0xff	},
  {      300,   20, -1500, 0, 0, 0,    0,    0,    0, 0xff	},
  {      300,   20,    -5, 0, 0, 0,    0,    0, 0xff, 0xff	},
  {       20,   20,    -5, 0, 0, 0, 0xff,    0,    0, 0xff	},
};

Gfx background_dl[] = 
{
  /* Setup display modes antialiased in 1 cycle */
  gsDPPipeSync(),

  gsSPVertex(&(background_vtx[0]), 4, 0),
  gsSP1Triangle(2, 1, 0, 2),
  gsSP1Triangle(3, 2, 0, 3),

  gsSPEndDisplayList(),
};

/*
 * Turbo Data
 *
 */

/* this init's the DP and clears the screen: */
gtState	dpInitClearObj =
{
    0x0,	/* renderState */
    0x0,	/* textureState */
    0,		/* vtxCount */
    0,		/* vtxV0 */
    0,		/* triCount */
    0x0, 0x0,	/* pad1, pad2 */
    gsDPClearOtherMode(),				/* rdpOthermode */
    {	/* integer portion: */
	0x00010000, 0x00000000,			/* transform */
	0x00000001, 0x00000000,
	0x00000000, 0x00010000,
	0x00000000, 0x00000001,
	/* fractional portion: */
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
    },
};


/* this finishes the DP with a sync: */
static Gfx finalDP[] =
{				/* rdpCmds */
    gsDPFullSync(),
    gsDPEndDisplayList()
};

gtState	dpFinalObj =
{
    0x0,	/* renderState */
    0x0,	/* textureState */
    0,		/* vtxCount */
    0,		/* vtxV0 */
    0,		/* triCount */
    0x0, 	/* pad1 */
    (Gfx *)&finalDP,
    gsDPClearOtherMode(),				/* rdpOthermode */
    {	/* integer portion: */
	0x00010000, 0x00000000,			/* transform */
	0x00000001, 0x00000000,
	0x00000000, 0x00010000,
	0x00000000, 0x00000001,
	/* fractional portion: */
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
    }
};

gtTriN	cube0_tris[] = 
{
    { 2, 1, 0, 0, },	/* top */
    { 3, 2, 0, 0, },	
    { 4, 5, 6, 0, },	/* bottom */
    { 4, 6, 7, 0, },
    { 7, 3, 0, 0, },	/* left */
    { 4, 7, 0, 0, },	
    { 5, 1, 2, 0, },	/* right */
    { 6, 5, 2, 0, },
    { 6, 2, 3, 0, },	/* near */
    { 7, 6, 3, 0, },	
    { 4, 0, 1, 0, },	/* far */
    { 5, 4, 1, 0, },

    { 2+8, 1+8, 0+8, 0, },	/* top */
    { 3+8, 2+8, 0+8, 0, },	
    { 4+8, 5+8, 6+8, 0, },	/* bottom */
    { 4+8, 6+8, 7+8, 0, },
    { 7+8, 3+8, 0+8, 0, },	/* left */
    { 4+8, 7+8, 0+8, 0, },	
    { 5+8, 1+8, 2+8, 0, },	/* right */
    { 6+8, 5+8, 2+8, 0, },
    { 6+8, 2+8, 3+8, 0, },	/* near */
    { 7+8, 6+8, 3+8, 0, },	
    { 4+8, 0+8, 1+8, 0, },	/* far */
    { 5+8, 4+8, 1+8, 0, },

    { 2+16, 1+16, 0+16, 0, },	/* top */
    { 3+16, 2+16, 0+16, 0, },	
    { 4+16, 5+16, 6+16, 0, },	/* bottom */
    { 4+16, 6+16, 7+16, 0, },
    { 7+16, 3+16, 0+16, 0, },	/* left */
    { 4+16, 7+16, 0+16, 0, },	
    { 5+16, 1+16, 2+16, 0, },	/* right */
    { 6+16, 5+16, 2+16, 0, },
    { 6+16, 2+16, 3+16, 0, },	/* near */
    { 7+16, 6+16, 3+16, 0, },	
    { 4+16, 0+16, 1+16, 0, },	/* far */
    { 5+16, 4+16, 1+16, 0, },

    { 2+24, 1+24, 0+24, 0, },	/* top */
    { 3+24, 2+24, 0+24, 0, },	
    { 4+24, 5+24, 6+24, 0, },	/* bottom */
    { 4+24, 6+24, 7+24, 0, },
    { 7+24, 3+24, 0+24, 0, },	/* left */
    { 4+24, 7+24, 0+24, 0, },	
    { 5+24, 1+24, 2+24, 0, },	/* right */
    { 6+24, 5+24, 2+24, 0, },
    { 6+24, 2+24, 3+24, 0, },	/* near */
    { 7+24, 6+24, 3+24, 0, },	
    { 4+24, 0+24, 1+24, 0, },	/* far */
    { 5+24, 4+24, 1+24, 0, },

};


Vtx q0[5] =  {
        {        -40,  40, 0, 0, 0, 0, 0, 0, 0xff, 0xff		},
        {         40,  40, 0, 0, (31 << 5), 0, 0xff, 0xff, 0xff, 0xff	},
        {         40, -40, 0, 0, (31 << 5), (31 << 5), 0, 0xff, 0, 0xff		},
        {        -40, -40, 0, 0, 0, (31 << 5), 0xff, 0, 0, 0xff		},
        {          0,   0, 0, 0, 0, 0, 0xff, 0xff, 0xff, 0xff	},
};

gtTriN	tris[] = 
{
    { 0, 1, 2, 0, },
    { 0, 2, 3, 0, },
    { 0, 1, 2, 0, },
    { 0, 2, 3, 0, },
};
