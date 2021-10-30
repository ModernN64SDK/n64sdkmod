/*======================================================================*/
/*	Mirror House							*/
/*	om_eval_draw.c							*/
/*									*/
/*	Copyright (C) 1998 MONEGI CORPORATION				*/
/*									*/
/*======================================================================*/
#include "pack.h"

#include "nd.h"
#include "nusys.h"

#include "om.h"
#include "om_background.h"

/* ===========================================================================
	Function Prototype Declarations 
 =========================================================================== */
Gfx *om_game_draw3d(Gfx *gp, OM_CHARACTER_ST *st, float um[4][4]);
void om_game_eval(OM_CHARACTER_ST *st, float mf[4][4]);

/* ===========================================================================
	External Reference Declarations 
 =========================================================================== */
/*** om_common.c ***/
extern OM_CHARACTER_DATA *om_com_charast_chain_search(OM_CHARACTER_ST *st,
                                                      int entry_number);

/* ---------------------------------------------------------------------------
	Rendering and Evaluation Functions 

    The four functions below generate a display list (GBI) for evaluating the object tree for rendering 3D images and for actual rendering.  The display list is issued to the N64 RSP. 
 --------------------------------------------------------------------------- */
Gfx *om_setup_SP_DP(Gfx *gp)
{
    /* Various settings */
    gSPClearGeometryMode( gp++, G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH
      | G_FOG | G_LIGHTING | G_TEXTURE_GEN
      | G_TEXTURE_GEN_LINEAR | G_LOD );
    gSPSetGeometryMode( gp++, G_SHADE | G_LIGHTING /*| G_SHADING_SMOOTH */
      | G_ZBUFFER | G_CULL_BACK );
    gSPFogPosition( gp++, 996, 1000);
    gSPClipRatio( gp++, FRUSTRATIO_2 );
    gDPSetCombineKey( gp++, G_CK_NONE );
    gSPTexture( gp++, 0, 0, 0, 0, G_OFF );
    gDPSetCycleType( gp++, G_CYC_1CYCLE );
    gDPSetCombineMode( gp++, G_CC_SHADE, G_CC_SHADE );
    gDPSetRenderMode( gp++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2 );
    gDPSetAlphaCompare( gp++, G_AC_NONE );
    gDPSetTextureFilter( gp++, G_TF_BILERP);
    gDPSetColorDither( gp++, G_CD_BAYER);
    gDPSetAlphaDither( gp++, G_AD_NOTPATTERN );
    gDPSetBlendMask( gp++, 0xff);
    gDPPipelineMode( gp++, G_PM_NPRIMITIVE);
    gDPSetTexturePersp( gp++, G_TP_PERSP);  /* Authorization to revise texture coordinates */
    return gp;
}

Gfx *om_game_draw3d(Gfx *gp, OM_CHARACTER_ST *st, float um[4][4])
{
    int i, max_entry;
    OM_CHARACTER_DATA *data;

    data = st->data;
    max_entry = st->header.character_entry;
    gp = om_setup_SP_DP( gp );

    for(i=0; i<max_entry; i++)
    {
        /* Character arrangement */
        if (data->dsp_status == OM_DISPLAY) {
            gp = ndBuildInstanceGfx(gp, data->cur_instp->obj_root_instp);
        }
        data = data->next;
    }
    return gp;
}

void om_game_eval(OM_CHARACTER_ST *st, float mf[4][4])
{
    u32 i, max_entry;
    float rm[4][4], um[4][4], tmpmf[4][4];
    OM_CHARACTER_DATA *data, *ch;

    data = st->data;
    max_entry = st->header.character_entry;
    for(i=0; i<max_entry; i++)
    {
        /* converted objects(scene) */
        if (data->dsp_status == OM_DISPLAY) {
            data->xb = data->x;
            data->yb = data->y;
            data->zb = data->z;
            guMtxIdentF(tmpmf);
            guTranslateF(um, data->x, data->y, data->z);
            guRotateRPYF(rm, data->rx, data->ry, data->rz);
            guMtxCatF(rm, um, tmpmf);
            ch = data;
            deform_flg = ch->deform_flg;
            ndEvalInstance(ch->cur_instp->root_instp, tmpmf, 1);
            ch->deform_flg = deform_flg;
        }
        data = data->next;
    }
}


