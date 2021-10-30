/*======================================================================*/
/*		NIFFprev						*/
/*		nddisp.c						*/
/*									*/
/*		Copyright (C) 1997,1998, NINTENDO Co,Ltd.		*/
/*									*/
/*======================================================================*/
#define __NDDISP_C__

/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	Simple 3D Manager ND

	Display Driver
 :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
#include <string.h>
#include "pack.h"
#include "nd.h"

/* ===========================================================================
	defines
 =========================================================================== */

/* ===========================================================================
	static variables
 =========================================================================== */
static Gfx ForceAlignmentPadding0[] = { gsSPEndDisplayList() };

/* ---------------------------------------------------------------------------
	Viewport
 --------------------------------------------------------------------------- */
static Vp vp = {
	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* scale */
	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,	/* translate */
};

/* ---------------------------------------------------------------------------
	RDP full initialize
 --------------------------------------------------------------------------- */
Gfx rdpstateinit_dl[] = {

    /* set all of the attribute registers to zero */
    gsDPSetEnvColor(0,0,0,0),
    gsDPSetPrimColor(0,0,0,0,0,0),
    gsDPSetBlendColor(0,0,0,0),
    gsDPSetFogColor(0,0,0,0),
    gsDPSetFillColor(0),
    gsDPSetPrimDepth(0,0),
    gsDPSetConvert(0,0,0,0,0,0),
    gsDPSetKeyR(0,0,0),
    gsDPSetKeyGB(0,0,0,0,0,0),

    /* set combine mode */
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),

    /* initialize the scissor box */
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),

    /* initialize all the texture tile descriptors to zero */
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0),

    gsDPSetTileSize(0, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(1, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(2, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(3, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(4, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(5, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(6, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(7, 0, 0, (1 - 1) << G_TEXTURE_IMAGE_FRAC, (1 - 1) <<
		G_TEXTURE_IMAGE_FRAC),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

/* ---------------------------------------------------------------------------
	RCP basic initialize
 --------------------------------------------------------------------------- */
static Gfx gfxinit_dl[]={
    /* init state */
    gsSPViewport(&vp),
    gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
			  G_FOG | G_LIGHTING | G_TEXTURE_GEN |
			  G_TEXTURE_GEN_LINEAR | G_LOD),
    gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH),
    gsSPTexture(0, 0, 0, 0, G_OFF),

    /* buffer fill */
    gsDPPipeSync(),
    gsSPDisplayList(rdpstateinit_dl),

    gsSPEndDisplayList(),
};

/* ---------------------------------------------------------------------------
	Z buffer initialize
 --------------------------------------------------------------------------- */
static Gfx clearzbuffer_dl[] = {
    gsDPSetCycleType(G_CYC_FILL),
    gsDPSetRenderMode(G_RM_NOOP, G_RM_NOOP2),
    gsDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
		      SEGTOP(ZBUF_SEGMENT)),
    gsDPFillRectangle(0, 0, SCREEN_WD-1, SCREEN_HT-1),
    gsDPSetDepthImage(SEGTOP(ZBUF_SEGMENT)),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

/* ---------------------------------------------------------------------------
	CFB initialize
 --------------------------------------------------------------------------- */
static Gfx clearcfb_dl[] = {
    gsDPSetCycleType(G_CYC_FILL),
    gsDPSetRenderMode(G_RM_NOOP, G_RM_NOOP2),
    gsDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
		      SEGTOP(CFB_SEGMENT)),
    gsDPFillRectangle(0, 0, SCREEN_WD-1, SCREEN_HT-1),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

/* ===========================================================================
	public functions
 =========================================================================== */

/* ----------------------------------------------------------------------------
name:
    ndClearFB
synopsis:
    Gfx *ndClearFB(Gfx *gp, u16 fill_depth, u16 fill_col);
description:
parameters:
    Gfx *gp;
	Display list pointer
    u16 fill_depth;
	Value for the Z-buffer fill
    u16 fill_col;
	Value for the color fill
returns:
    Gfx *gp;
	Display list pointer
notes:
---------------------------------------------------------------------------- */
Gfx *ndClearFB(Gfx *gp, u16 fill_depth, u16 fill_col)
{
    u32 value;

    /* Assign the z-buffer initialization value and initialize */
    value = ((u32)fill_depth)&0xffff;
    gDPSetFillColor( gp++, (value|(value<<16)));
    gSPDisplayList( gp++, clearzbuffer_dl );

    /* Assign the CFB initialization value and initialize */
    value = ((u32)fill_col)&0xffff;
    gDPSetFillColor( gp++, (value|(value<<16)));
    gSPDisplayList( gp++, clearcfb_dl );

    return gp;
}

/* ----------------------------------------------------------------------------
name:
    ndEvalLight
synopsis:
    int ndEvalLight(float *tarpos, NDD_LIGHT *lp, RGBAQUAD *color, s8 *nv);
description:
    Updates the light data based on the initialized light structure and position data for the target object.  Then returns the color value and the light vector.

parameters:
    float *tarpos;
	Position of the target that the light hits.  Three values: x, y, and z. 
	If NULL, the origin (0,0,0) is used in the calculations.
    NDD_LIGHT *lp;
	Pointer to the light structure. 
    RGBQUAD *color;
	Pointer to the RGBAQUAD structure, which stores the color values.
    s8 *nv;
	Pointer that stores the light vector. Three S0.7-format values: x,y, and z (3 bytes).
returns:
    int;
	When light is evaluated as invalid, 0 is returned.
notes:
    The NDD_LIGHT structure has the following limitations.
    - xxxx provided by fnv is normalized.
    - When the flag is set to NDC_LFLAG_ATTENUATION, valid values must first be assigned for start and end.
    - NDC_LFLAG_CUTOFF is valid only with NDC_LFLAG_HAVEPOS.
    Although the execution speed of these can be checked xxin principlexx, it is ignored because it is a game manager.
When 0 is returned, color and nv are undefined.
---------------------------------------------------------------------------- */
int ndEvalLight(float *tarpos, NDD_LIGHT *lp, RGBAQUAD *color, s8 *nv)
{
    int i;
    u32 flag;
    float d, dist, power, *fnv, *ltv, tmpnv[3];
    static float pos_zero[]={ 0, 0, 0 };

    flag = lp->flag;

    /* Prepare light source vector for fnv */
    if( BITTEST1(flag, NDC_LFLAG_HAVEPOS) ){
	/* xxFalse pointxx light source */
	dist = ndguCalcNormal(tmpnv, lp->fpos, (tarpos)? tarpos: pos_zero);
	fnv = tmpnv;
	if( BITTEST1(flag, NDC_LFLAG_CUTOFF) ){
	    /* Using CUTOFF, evaluate whether light source angle is outside the valid range */
	    ltv = lp->fnv;
	    d = fnv[0]*ltv[0] + fnv[1]*ltv[1] + fnv[2]*ltv[2];  /* Inner product */
	    d = (1.0 - lp->cutoff) + lp->cutoff * d;
	    if(d <= 0) return FALSE;	/*  This light invalid */
	    power = d;
	}
    }else{
	/* Bidirectional light source */
	fnv = lp->fnv;
	dist = power = 1.0;
    }

    /* Create parameters for setting the registers */
    d = 127.0/dist;
    nv[0] = fnv[0]*d;
    nv[1] = fnv[1]*d;
    nv[2] = fnv[2]*d;

    /* Recalculate color level using light point and model 
     Note: Values for start and end are always treated as valid.
            Assigning strange values will cause a runaway.
    */
    if( BITTEST(flag, NDC_LFLAG_ATTENUATION) ){
	if(dist < lp->start) return FALSE;  /* Invalid because too near */
	if(dist > lp->end) return FALSE;  /* Invalid because too far */
	power *=
	  1.0 - ((dist - lp->start) / (lp->end - lp->start));
    }

    /* Calculation and assignment of color data */
    memcpy(color, &lp->color, sizeof(RGBAQUAD));
    i = power * 255.0;
    if(i < 1) return FALSE;	/* Light color is invalid */

    /* Color level adjustment */
    ndguCalcFadeColor( color, 0, (i>255)? 255: i, 0 );

    /* This light is valid  */
    return TRUE;
}

/* ----------------------------------------------------------------------------
name:
    ndReloadLightReg
synopsis:
    Gfx *ndReloadLightReg(Gfx *gp, NDD_LIGHT *lp, u32 newcolor, s8 *newlvec);
description:
    Establishes an association with the light structure and updates the light data. 
    Called by the ndSetupLightset function. 
parameters:
    Gfx *gp;
	Display list pointer.
    NDD_LIGHT *lp;
	Pointer to the light structure.
    u32 newcolor;
	New color setting.  Stored in 0xRRGGBB00 format in the upper bits; the lower 8 bits are treated as 0x00 internally.
    s8 *newlvec;
	New vector pointer.  Three values in S0.7 format are recognized: x,y, and z (3 bytes).
	NULL value not accepted. 
returns:
    Gfx *gp;
	Display list pointer.
notes:
    The following three members of the NDD_LIGHT structure are referenced: color, power, and stored_nv.
    The required heap portions are defined, and RSP commands are loaded into the buffer.  
    This function is optimized for internal use.  Thus, the user should understand its contents well when calling it.
---------------------------------------------------------------------------- */
Gfx *ndReloadLightReg(Gfx *gp, NDD_LIGHT *lp, u32 newcolor, s8 *newlvec)
{
    /* Define the undefined portion of the color value */
    newcolor &= 0xffffff00;

    /* If this is the second or a subsequent setting, update the setting by the difference from the previous setting. */
    if( BITTEST1(lp->flag, NDC_LFLAG_ALREADY_SETS) ){
	/* Compare with the previous light vector setting */
	if(lp->stored_nv[0] == newlvec[0] && lp->stored_nv[1] == newlvec[1] &&
	   lp->stored_nv[2] == newlvec[2]){

	    /* The light vector is the same.  What about the color? */
	    if(lp->stored_color == newcolor)
	      return gp;	/* No resetting necessary if it is the same */

	    /* Update the color registers only */
	    lp->stored_color = newcolor;
	    return ndgSPLightColor(gp, lp->lnum, newcolor);
	}
    }

    /* Save to the registers */
    memcpy(lp->stored_nv, newlvec, 3);
    lp->stored_color = newcolor;

    /* Generate the display list */
    gp = gfxSetLight( gp, lp->lnum, newcolor, newlvec);
    BITSET(lp->flag, NDC_LFLAG_ALREADY_SETS);

    return gp;
}

/* ----------------------------------------------------------------------------
name:
    ndSetupLightset
synopsis:
    Gfx *ndSetupLightset(Gfx *gp, float *tarpos, NDD_LIGHTSET *lightset);
description:
    Evaluates the initialized lightset passed by lightset and the target object. Loads only the changed portions to the display list.
parameters:
    Gfx *gp;
	Display list pointer.
    float *tarpos;
	Position of the target that the light hits.  Three values:  x,y, and z .
	If NULL, the origin (0,0,0) is used in the calculations.
    NDD_LIGHTSET *lightset;
	Pointer to the light set structure.
returns:
    Gfx *gp;
	Display list pointer.
notes:
    The argument tarpos is passed unmodified to ndEvalLight() and evaluated.  Consequently, when its value is NULL, the origin is substituted and evaluated
---------------------------------------------------------------------------- */
Gfx *ndSetupLightset(Gfx *gp, float *tarpos, NDD_LIGHTSET *lightset)
{
    int i;
    union {
	RGBAQUAD rq;
	u32 val;
    } newcolor;
    s8 newlvec[3];
    static s8 dummy_lvec[]={ 0, 0, 127 };
    NDD_LIGHT *lp;
    int lcnt = lightset->numlights;

    if(lcnt){
	/* Usually set when there is even 1 valid light present. */
	for(i=0; i<lcnt; ++i){
	    lp = &lightset->light[i];
	    if( BITTEST1(lp->flag, NDC_LFLAG_ENABLE) ){
		if(ndEvalLight( tarpos, lp, &newcolor.rq, newlvec ))
		  gp = ndReloadLightReg(gp, lp, newcolor.val, newlvec);
		else
		  gp = ndReloadLightReg(gp, lp, 0, dummy_lvec);
	    }
	}
    }else{
	/* If all lights are invalid, construct an achromatic light. */
	gp = gfxSetLight( gp, 1, 0, dummy_lvec );
	lcnt = 1;
    }

    /* Set the ambient color */
    memcpy(&i, &lightset->ambient, 4);
    gp = gfxSetAmbient( gp, lcnt+1, i&0xffffff00);

    gSPNumLights(gp++, lcnt);

    return gp;
}

/* ----------------------------------------------------------------------------
name:
    ndInitLights
synopsis:
    int ndInitLights( NDD_LIGHTSET *lightset );
description:
    Resets the RDP light setting, determines the number of lights, and allocates light numbers for each light structure.
parameters:
    NDD_LIGHTSET *lightset;
	Ponter to the light set structure.
returns:
    int;
	Number of valid lights.
notes:
    References the _ENABLE bit of each light structure flag, calculates the number of lights required, and allocates each light number. 
    This number is the light number actually evaluated by RSP, but the user need not be aware of it.  It is referenced and used when a reset is performed by an API function.
---------------------------------------------------------------------------- */
int ndInitLights( NDD_LIGHTSET *lightset )
{
    int i, lcnt;
    NDD_LIGHT *lp;

    /* Show the number of valid lights in the light structure while placing light numbers in the structure. */
    lp = &lightset->light[0];
    for( i=lcnt=0; i<NUM_LIGHTSET_LIGHTS; ++i ){
	if( BITTEST1(lp[i].flag, NDC_LFLAG_ENABLE) ){
	    /* Assign the light numbers */
	    lp[i].lnum = 1+lcnt;
	    /* Attempt to set register */
	    BITRESET(lp[i].flag, NDC_LFLAG_ALREADY_SETS);
	    ++lcnt;
	}
    }

    return lightset->numlights = lcnt;
}

/* ----------------------------------------------------------------------------
name:
    ndSetupObjectRegister
synopsis:
    Gfx *ndSetupObjectRegister( Gfx *gp, NDD_INST_OBJ *niop );
description:
    References the object instance structure and resets registers for which resetting is specified.
parameters:
    Gfx *gp;
	Pointer to the display list buffer.
    NDD_INST_OBJ *niop;
	Pointer to the object instance structure.
returns:
    Gfx *;
	Post-update Gfx buffer pointer.
notes:
    Called from ndDrawObjectInstance.
---------------------------------------------------------------------------- */
Gfx *ndSetupObjectRegister( Gfx *gp, NDD_INST_OBJ *niop )
{
    int mode;
    union{
	u32 i;
	u8 c[4];
    }col;
    int flag = niop->flag;

    /* Pipe sync entered by default. */
    gDPPipeSync(gp++);

    /* Set RenderMode  */
    if( BITTEST1(flag, NDC_OFLAG_RENDERMODE) )
      gp = ndgDPSetRenderMode(gp, niop->rdmode);

    /* Set AlphaCompare */
    if( BITTEST1(flag, NDC_OFLAG_ALPHACOMPARE) ){
	switch( niop->rdmode & NDC_RM_ALPHA_COMPARE){
	  case NDC_RM_ALPHA_COMPARE_THRESHOLD:
	    mode = G_AC_THRESHOLD;
	    break;
	  case NDC_RM_ALPHA_COMPARE_DITHER:
	    mode = G_AC_DITHER;
	    break;
	  case NDC_RM_ALPHA_COMPARE_NONE:
	  default:
	    mode = G_AC_NONE;
	    break;
	}
	gDPSetAlphaCompare(gp++, mode);
    }

    /* Set AlphaDither */
    if( BITTEST1(flag, NDC_OFLAG_ALPHADITHER) ){
	switch( niop->rdmode & NDC_RM_DITHER_ALPHA){
	  case NDC_RM_DITHER_ALPHA_PATTERN:
	    mode = G_AD_PATTERN;
	    break;
	  case NDC_RM_DITHER_ALPHA_NOPATTERN:
	    mode = G_AD_NOTPATTERN;
	    break;
	  case NDC_RM_DITHER_ALPHA_NOISE:
	    mode = G_AD_NOISE;
	    break;
	  case NDC_RM_DITHER_ALPHA_DISABLE:
	  default:
	    mode = G_AD_DISABLE;
	    break;
	}
	gDPSetAlphaDither(gp++, mode);
    }

    /* Set CycleType */
    if( BITTEST1(flag, NDC_OFLAG_CYCLETYPE) ){
	if(BITTEST1(niop->flag, NDC_OFLAG_2CYC)){
	    /* 2cycle mode */
	    gDPSetCycleType(gp++, G_CYC_2CYCLE);
	}else{
	    /* 1cycle mode */
	    gDPSetCycleType(gp++, G_CYC_1CYCLE);
	}
    }

    /* Set ColorCombineMode */
    if( BITTEST1(flag, NDC_OFLAG_CCMODE) )
      gp = ndgDPSetCombineMode(gp, niop->cc1, niop->cc2);

    /* Set GeomMode */
    if( BITTEST1(flag, NDC_OFLAG_GEOMMODE) ){
	gSPClearGeometryMode(gp++, -1);
	gSPSetGeometryMode(gp++, niop->geommode);
    }

    /* Set PrimColor */
    if( BITTEST1(flag, NDC_OFLAG_PRIMCOLOR) ){
	col.i = niop->primcol;
	gDPSetPrimColor(gp++, niop->prim_m, niop->prim_l,
			col.c[0], col.c[1], col.c[2], col.c[3]);
    }

    /* Set EnvColor */
    if( BITTEST1(flag, NDC_OFLAG_ENVCOLOR) ){
	col.i = niop->envcol;
	gDPSetEnvColor(gp++, col.c[0], col.c[1], col.c[2], col.c[3]);
    }

    /* Set FogColor */
    if( BITTEST1(flag, NDC_OFLAG_FOGCOLOR) ){
	col.i = niop->fogcol;
	gDPSetFogColor(gp++, col.c[0], col.c[1], col.c[2], col.c[3]);
    }

    /* Set BleandColor */
    if( BITTEST1(flag, NDC_OFLAG_BLEANDCOLOR) ){
	col.i = niop->blcol;
	gDPSetBlendColor(gp++, col.c[0], col.c[1], col.c[2], col.c[3]);
    }

    /* Create a Mtx structure from a 4x4 matrix and enter it in GBI as the MODELVIEW matrix. */
    if( BITTEST1(flag, NDC_OFLAG_MATRIX) ){
	Mtx tmpmtx;

	if(BITTEST1(niop->flag, NDC_OFLAG_BILLBOARD)){
	    /* When a billboard is processed, produce a LookAt matrix.
	       Note:  There is no error handling.  Improper settings will 		cause guLookAt to hang.
	       */
	    float *eye, *at;
	    float nv[3];
	    eye = &niop->modelview[3][0];
	    at = &niop->bb_obj[0]->u.obj.modelview[3][0];
	    ndguCalcNormal( nv, eye,
			 &niop->bb_obj[1]->u.obj.modelview[3][0]);
	    guLookAt( &tmpmtx,
		     eye[0], eye[1], eye[2],
		     at[0], at[1], at[2],
		     nv[0], nv[1], nv[2]);
	}else{
	    guMtxF2L(niop->modelview, &tmpmtx);
	}
	gp = gfxLoadMatrix(gp, &tmpmtx);
    }

    return gp;
}

/* ----------------------------------------------------------------------------
name:
    ndDrawShape
synopsis:
    Gfx *ndDrawShape( Gfx *gp, NDR_SHAPE *shp );
description:
    Loads the Gfx structure in the rendering buffer according to contents of the shape structure.
parameters:
    Gfx *gp;
	Pointer to the Display list buffer.
    NDR_SHAPE *shp;
	Pointer to the shape structure used for referencing.
returns:
    Gfx *;
	Post-update Gfx buffer pointer.
notes:
    Called from ndDrawObjectInstance.
---------------------------------------------------------------------------- */
Gfx *ndDrawShape( Gfx *gp, NDR_SHAPE *shp )
{
    /* Set the segment registers. */
    if(shp->vtx)
      gSPSegment(gp++, shp->vtxsegno, shp->vtx);
    if(shp->img)
      gSPSegment(gp++, shp->imgsegno, shp->img);
    if(shp->tlut)
      gSPSegment(gp++, shp->tlutsegno, shp->tlut);
    gSPDisplayList(gp++, shp->gfx);

    return gp;
}

/* ----------------------------------------------------------------------------
name:
    ndDrawObjectInstance
synopsis:
    int ndDrawObjectInstance( NDD_INST_OBJ *niop );
description:
    Displays one object.
parameters:
    Gfx *gp;
	Pointer to the display list buffer.
    NDD_INST_OBJ *niop;
	Pointer to the object instance structure. 
returns:
    Gfx *;
	Post-update Gfx buffer pointer. 
notes:
    Called from ndBuildInstanceGfx. 
---------------------------------------------------------------------------- */
Gfx *ndDrawObjectInstance( Gfx *gp, NDD_INST_OBJ *niop )
{
    int flag = niop->flag;

    if(BITTEST(flag, NDC_OFLAG_ENABLE|NDC_OFLAG_VISIBLE)){

	/* Set each register
	 Note:  This is constructed so that updating is possible even without shape.  Depending on the needs of the user, there may be cases in which the if statements below should be combined. */
	if( flag & NDC_OFLAG_ALLREG )
	  gp = ndSetupObjectRegister( gp, niop );

	/* Display list entry */
	if(niop->shape)
	  gp = ndDrawShape( gp, niop->shape );
    }
    return gp;
}

/* ----------------------------------------------------------------------------
name:
    ndBuildInstanceGfxAtPrio
synopsis:
    Gfx *ndBuildInstanceGfxAtPrio(Gfx *gp, NDD_INSTANCE *isp, u16 start, u16 end);
description:
    Renders instances of a specific priority from the specified instance pointer onward. 
parameters:
    Gfx *gp;
	Display list pointer.
    NDD_INSTANCE *isp;
	The target instance pointer. Providing NULL causes everything from the object's root and lower to be evaluated.
    u16 start;
	Priority of the rendering target. start is also included as a rendering target.
    u16 end;
	Priority other than that of the rendering target. Note that end is not rendered. 
returns:
    Gfx *;
	Updated Display list pointer.
notes:
    Set start and end so that start has the higher value (higher priority).
---------------------------------------------------------------------------- */
Gfx *ndBuildInstanceGfxAtPrio(Gfx *gp, NDD_INSTANCE *isp, u16 start, u16 end)
{
    /* All instances if isp == NULL */
    if(!isp)
      isp = obj_root_instp;

    /* Evaluate display beginning from the specified instance */
    for(; isp!=&bottom_inst; isp=isp->next)
      if(isp->type == NDC_ITYPE_OBJECT && isp->prio <= start)
	break;

    /* Do nothing if no corresponding instance */
    if(isp==&bottom_inst)
      return gp;

    /* Evaluate display beginning from the specified instance */
    for(; isp!=&bottom_inst; isp=isp->next){
	if(isp->prio <= end)
	  break;
	if(isp->type == NDC_ITYPE_OBJECT)
	  gp = ndDrawObjectInstance( gp, &isp->u.obj );
    }

    return gp;
}

/* ----------------------------------------------------------------------------
name:
    ndBuildInstanceGfx
synopsis:
    Gfx *ndBuildInstanceGfx(Gfx *gp, NDD_INSTANCE *isp);
description:
    From the current scene structure, loads the display list for the scene.
parameters:
    Gfx *gp;
	Display list pointer.
    NDD_INSTANCE *isp;
	The target instance pointer. Providing NULL causes everything from the object's root and lower to be evaluated.
returns:
    Gfx *;
	Updated Display list pointer.

notes:
    Internally, ndBuildInstanceGfxAtPrio() is called to provide all object priorities. 
---------------------------------------------------------------------------- */
Gfx *ndBuildInstanceGfx(Gfx *gp, NDD_INSTANCE *isp)
{
    return ndBuildInstanceGfxAtPrio( gp, isp,
	    NDC_IPRIO_OBJROOT, NDC_IPRIO_OBJEND+1);
}

/* ----------------------------------------------------------------------------
name:
    ndBuildSceneGfx
synopsis:
    Gfx *ndBuildSceneGfx(Gfx *gp, float mul[4][4]);
description:
From the current scene structure, loads the display list for the scene.
parameters:
    Gfx *gp;
	Display list pointer.
    float mul[4][4];
	Camera matrix.
returns:
    Gfx *;
	Display list pointer.
notes:
    The camera matrix, mul, is multiplied by the projection matrix.
---------------------------------------------------------------------------- */
Gfx *ndBuildSceneGfx(Gfx *gp, float mul[4][4])
{
    /* Start RCP rendering*/
    gSPDisplayList( gp++, gfxinit_dl);

    /* Initialize z-buffer and CFB */
    gp = ndClearFB(gp, cur_scene.fill_depth, cur_scene.fill_col);

    /* Reflect the fog parameters */
    if(cur_scene.fog.near != cur_scene.fog.far){
	gSPFogPosition(gp++, cur_scene.fog.near, cur_scene.fog.far);
	gDPSetFogColor(gp++, cur_scene.fog.color.r, cur_scene.fog.color.g,
		       cur_scene.fog.color.b, cur_scene.fog.color.a);
    }

    /* Reflect the camera */
    gp = gfxMulPerspective( gp,
	   cur_scene.camera.fovy,
	   cur_scene.camera.aspect,
	   cur_scene.camera.near,
	   cur_scene.camera.far,
	   cur_scene.camera.scale,
	   mul);

    /* Create the reflection structure */
    /* reserve: Here LookAt is generated as a reflection with a value such that it is parallel to the camera matrix mul.  However, it actually can be set according to the preferences of the user. */
    {
	float d;
	s8 rv[3], uv[3];

	d = 127.0/sqrtf(mul[0][0]*mul[0][0]+mul[1][0]*mul[1][0]+
			mul[2][0]*mul[2][0]);
	rv[0] = mul[0][0]*d;
	rv[1] = mul[1][0]*d;
	rv[2] = mul[2][0]*d;

	d = 127.0/sqrtf(mul[0][1]*mul[0][1]+mul[1][1]*mul[1][1]+
			mul[2][1]*mul[2][1]);
	uv[0] = mul[0][1]*d;
	uv[1] = mul[1][1]*d;
	uv[2] = mul[2][1]*d;

	gp = gfxLookAtReflect( gp, rv, uv);
    }
#if 0
    gp = gfxLookAtReflect( gp, &cur_scene.reflect.right[0],
			  &cur_scene.reflect.upper[0]);	
#endif

    /* Construction of lights */
    ndInitLights( &cur_scene.lightset );
    gp = ndSetupLightset(gp, NULL, &cur_scene.lightset);

    return gp;
}


