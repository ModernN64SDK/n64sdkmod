/*
 * static.c
 *
 * Contains static display lists for morphdemo.c
 *
 */

#include "morphdemo.h"

/* Texture definition to use.
 * How to make it:
 * Use xv to create a 225x225 .rgb file, then convert to .ppm using "toppm",
 * then run ppm2cTex -m -s, as done in makefile.
 */

/* Multi-tile texture of mario */
#include "mariotex.h"

/* Z-buffer.  Defined in zbuf.c */
extern unsigned short zbuffer[];

/*
 * Remember, viewport structures have 2 bits of fraction in them.
 */
static Vp vp = {
	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* scale */
	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* translate */
};

/*
 * Display list to initialize the RSP state
 */
Gfx rspinit_dl[] = {
    gsSPViewport(&vp),
    gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
			  G_FOG | G_LIGHTING | G_TEXTURE_GEN |
			  G_TEXTURE_GEN_LINEAR | G_LOD ),
    gsSPTexture(0, 0, 0, 0, G_OFF),
    gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_ZBUFFER),
    gsSPEndDisplayList(),
};

/*
 * Light -- only needed if lighting is turned on
 */
Lights1 thelight = gdSPDefLights1(0x20, 0x20, 0x20,  /* Ambient */
				  0xcf, 0xcf, 0xcf,  /* Color? */
				  -90, 0x00, 90   /* norm'd Normal */
				  );

/*
 * Display list to initialize the RDP state
 * Some init is also done in the screen clear display list, and in
 * morphdemo.c
 */
 
Gfx rdpinit_dl[] = {
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPPipelineMode(G_PM_1PRIMITIVE), /* Don't pipeline primitives */
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),
    /*
     * Various texture commands.  They're harmless if we don't do texturing
     */
#ifdef DOMM
    gsDPSetTextureLOD(G_TL_LOD),       /* Choose tile # based on LOD */
#else
    gsDPSetTextureLOD(G_TL_TILE),      /* Use tile # as is */
#endif
    gsDPSetTextureLUT(G_TT_NONE),      /* Disable color lookup table */
    gsDPSetTextureDetail(G_TD_CLAMP),  /* No sharpen or detail */
    gsDPSetTexturePersp(G_TP_PERSP),   /* Use s,t persp correction */
    gsDPSetTextureFilter(G_TF_BILERP), /* Bilinear interpolation   */
    gsDPSetTextureConvert(G_TC_FILT),  /* No YUV conversion stuff  */
    gsDPSetCombineKey(G_CK_NONE),      /* ?? */
    gsDPSetAlphaCompare(G_AC_NONE),    /* No alpha compare */
    /*
     * Turn dithering on or off.  Note that on HW2 where dithering works
     * better it might make sense to turn in on even when lighting is off.
     * Right now it doesn't look so good, but it's necessary when you're
     * lighting to avoid horrible banding effects.
     */
#ifdef DO_LIGHTING
    gsDPSetColorDither(G_CD_ENABLE),   /* Turn on dithering */
#else
    gsDPSetColorDither(G_CD_DISABLE),  /* Turn off dithering */
#endif
    /*
     * Set color combiner modes and load light if appropriate
     */
#ifdef DO_LIGHTING
    /* Load the light */
    gsSPSetLights1(thelight),
#ifdef DOTEX
#ifdef DOMM
    /* lighting; texture w/trilinear mipmapping */
    gsDPSetCombineMode(G_CC_TRILERP, G_CC_MODULATERGB2),
#else
    /* lighting; texture w/o mipmapping */
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
#endif
#else
    /* lighting; no texture */
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
#endif

#else
    /* No lighting */
#ifdef DOTEX
#ifdef DOMM
    /* no lighting; texture w/trilinear mipmapping */
    gsDPSetCombineMode   (G_CC_TRILERP,
	                  G_CC_DECALRGB2),/* RGBA texture -> out */
#else
    /* no lighting; texture w/o mipmapping */
    gsDPSetCombineMode   (G_CC_DECALRGBA,
	                  G_CC_DECALRGBA),/* RGBA texture -> out */
#endif
#else
    /* no lighting; no texture */
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
#endif
#endif
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

/*
 * Display list to Clear the CFB and Zbuf
 * A gDPSetColorImage must be done before this display list is called,
 * (and afterwards as well before drawing).
 * We also set the cycle mode and rendering mode to use for drawing later
 */
Gfx scrnclr_dl[] = {
  gsDPPipeSync(), /* Not sure if necessary */
  gsDPSetCycleType(G_CYC_FILL),
  /* When in fill mode, must not have AA or ZBUF bits set */
  gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
  /* Clear color; coverage = 1 */
  gsDPSetFillColor(GPACK_RGBA5551(0x40,0x40,0x80,1) << 16 |
		   GPACK_RGBA5551(0x40,0x40,0x80,1)),
  gsDPFillRectangle(0, 0, SCREEN_WD-1, SCREEN_HT-1),
  gsDPNoOp(),    /* Necessary ? */
  gsDPPipeSync(), 
  /* Clear Z, z = max z, dz = 0 */
  gsDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
		    OS_K0_TO_PHYSICAL(zbuffer)),
  gsDPSetFillColor(GPACK_ZDZ(G_MAXFBZ, 0) << 16 | GPACK_ZDZ(G_MAXFBZ, 0)),
  gsDPFillRectangle(0, 0, SCREEN_WD-1, SCREEN_HT-1),
  gsDPNoOp(),  /* Necessary ? */
  gsDPPipeSync(),
  /*
   * Set cycle mode to use for rendering
   */
#ifdef DOMM
  gsDPSetCycleType(G_CYC_2CYCLE),  /* Put into 2-cycle mode */
#else
  gsDPSetCycleType(G_CYC_1CYCLE),  /* Put into 1-cycle mode */
#endif
  /*
   * Set rasterization mode for triangles
   * Mode = Anti-aliased, Z-buffered, opaque surfaces
   */
#ifdef DOMM
#ifdef DOZBUF
  /* ..._TERR2 looks a little better than ..._SURF2 because the non-silhouette
   * edges are anti-aliased better.
   */
  gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_OPA_TERR2),
#else
  /* No Z-buffering: Doesn't look quite as good, but runs faster */
  gsDPSetRenderMode(G_RM_PASS, G_RM_AA_OPA_SURF2),
#endif
#else
  gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
#endif
  gsSPEndDisplayList()
};
