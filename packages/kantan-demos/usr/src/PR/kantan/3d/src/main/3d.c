
#include "pack.h"

extern Gfx n64_model0[];

static Lights1 lightset = gdSPDefLights1(
      127, 127, 127,	/* ambient light color		*/
      255, 255, 255,	/* diffuse light color		*/
      24,  70,  66);	/* diffuse light position	*/

EXCONTPAD *pad;

static Gfx *setup_SP_DP(Gfx *gp)
{
    /*various settings */
    gSPClearGeometryMode( gp++, G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH
      | G_FOG | G_LIGHTING | G_TEXTURE_GEN
      | G_TEXTURE_GEN_LINEAR | G_LOD );
    gSPSetGeometryMode( gp++, G_SHADE | G_LIGHTING | G_SHADING_SMOOTH
      | G_ZBUFFER | G_CULL_BACK );
    gSPNumLights( gp++, 1);
    gSPSetLights1( gp++, lightset);
    gSPTexture( gp++, 0, 0, 0, 0, G_OFF );
    gDPSetCycleType( gp++, G_CYC_1CYCLE );
    gDPSetCombineMode( gp++, G_CC_SHADE, G_CC_SHADE);
    gDPSetRenderMode( gp++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2 );
    gDPSetAlphaCompare( gp++, G_AC_NONE );
    gDPSetColorDither( gp++, G_CD_MAGICSQ);
    gDPSetAlphaDither( gp++, G_AD_NOISE );
    return gp;
}

static Gfx *draw_polygon(Gfx *gp)
{
    /* Polygon rendering */
    gSPDisplayList(gp++, n64_model0);
    return gp;
}

void entry(void)
{
    Gfx *gp;
    float r,v,s;

    r=v=0.0;
    s=PI2/50000.0;
    while(1){
        /* Shift a camera by 1 unit */
        v += s;
        r += v;
        if(ABS(v)>PI2/100.0) s=-s;

        /* Start creating a display list */
        gp = gfxBegin( 1024 );
        /* Set rendering mode for RSP and RDP*/
        gp = setup_SP_DP( gp );
        /* Set projection matrix */
        gp = gfxPerspective( gp, 30, (float)SCREEN_WD/SCREEN_HT, 64, 1000, 1.0);
        /* Set model view matrix */
        gp = gfxLookAt( gp, sinf(r)*400, 220, cosf(r)*400, 0,0,0, 0,1,0);
        /*Render polygon */
        gp = draw_polygon( gp );
        /* End creating a display list */
        gfxEnd(gp);
        /* Enter the display list into RCP */
        gfxFlush();
        /* Retrace standby*/
        gfxWaitSync();
    }
}

