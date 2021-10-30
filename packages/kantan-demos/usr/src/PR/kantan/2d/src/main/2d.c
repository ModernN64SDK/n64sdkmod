
/* ==========================================================================

        KAN TAN Manual. sample program.

        chapter 2

        2d

=========================================================================== */

#include "pack.h"

extern unsigned char texture[];
extern unsigned short texturetlut[];

static Gfx *setup_SP_DP(Gfx *gp)
{
    /*Various settings*/
    gSPTexture( gp++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON );
    gDPPipeSync( gp++ );
    gDPSetCycleType( gp++, G_CYC_COPY );
    gDPSetRenderMode( gp++, G_RM_NOOP, G_RM_NOOP2 );
    gDPSetTextureLOD( gp++, G_TL_TILE );
    gDPSetTexturePersp( gp++, G_TP_NONE );
    gDPSetTextureDetail( gp++, G_TD_CLAMP );
    gDPSetTextureFilter( gp++, G_TF_BILERP );
    gDPSetTextureConvert( gp++, G_TC_FILT );
    gDPSetAlphaCompare( gp++, G_AC_NONE );
    gDPSetColorDither( gp++, G_CD_DISABLE);
    gDPSetAlphaDither( gp++, G_AD_NOISE );

    return gp;
}

static Gfx *load_texture(Gfx *gp, u16 w, u16 h)
{
    /* Read TLUT */
    gDPSetTextureLUT( gp++, G_TT_RGBA16 );
    gDPLoadTLUT_pal16( gp++, 0, texturetlut);

    /* Read Bitmap pattern */
    gDPLoadTextureTile_4b (gp++, texture, G_IM_FMT_CI, w, h,
        0, 0, w-1, h-1,
        0,
        G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR,
        G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    return gp;
}

static Gfx *draw_texture(Gfx *gp, u16 left, u16 top, u16 w, u16 h)
{
    /*Render Bitmap pattern */
    gSPTextureRectangle( gp++,
      left<<2, top<<2, ((left+w)<<2)-1, ((top+h)<<2)-1, G_TX_RENDERTILE,
      0, 0, 4<<10, 1<<10 );
    return gp;
}

void entry(void)
{
    Gfx *gp;
    u16 w, h;

    w=64; h=64;
    while(1){
        /* Start creating a display list */
        gp = gfxBegin( 1024 );
        /* Set rendering mode for RSP and RDP */
        gp = setup_SP_DP( gp );
        /* Read texture pattern */
        gp = load_texture( gp, w, h );
        /* Write texture pattern */
        gp = draw_texture( gp, 124, 92, w, h);
        /* End creating a display list */
        gfxEnd(gp);
        /* Enter the display list into RCP */
        gfxFlush();
        /* Retrace standby */
        gfxWaitSync();
    }
}
