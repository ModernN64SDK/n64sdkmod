/*======================================================================*/
/*	Mirror House							*/
/*	om_mychar.c							*/
/*									*/
/*	Copyright (C) 1998 MONEGI CORPORATION				*/
/*									*/
/*======================================================================*/
#include "pack.h"

#include <math.h>
#include "nd.h"
#include "nusys.h"

#include "om.h"
#include "om_info.h"
#include "om_camera.h"
#include "om_def.h"
#include "om_bg_def.h"
#include "om_audio.h"
#include "om_common.h"

/* ===========================================================================
	External Reference Declarations
 =========================================================================== */
extern EXCONTPAD *pad;

/* ===========================================================================
	External Function Reference Declarations
 =========================================================================== */
extern void om_com_character_cur_instance_change(OM_CHARACTER_DATA *data, int nd_no);
extern void om_com_character_globalresource_change(OM_CHARACTER_DATA *data);
extern OM_CHARACTER_DATA *om_com_charast_chain_search(OM_CHARACTER_ST *st, u32 entry_number);
extern void om_bg_map_area_culling(u32 *area_dsp_flg, u32 area_number);
extern void om_bg_map_zone_culling(u32 *zone_dsp_flg, u32 area_number, u32 zone_x, u32 zone_z);
extern u32 om_bg_character_wall_hit_check(OM_CHARACTER_DATA *data, OM_BG_STAGE_ST *stage);
extern void om_characterst_init(OM_CHARACTER_DATA *st, OM_CHARACTER_INFO_DATA *stinf);
extern int om_com_circle_hit_check(OM_HIT_CHECK_ST *obj1, OM_HIT_CHECK_ST *obj2);
extern s32 om_expad_lookat_xydeg(s32 x, s32 y, f32 *dist);
extern void om_audio_se_start(u32 se_no);
extern void om_audio_snd_player_snd_stop(u32 se_no);

/* ===========================================================================
	Function Prototype Declarations
 =========================================================================== */
u32 om_mycharacter_main(OM_CAMERA_ST *cst, OM_CHARACTER_ST *st,
                        OM_BG_STAGE_ST *stage);
void om_mycharacterst_init(OM_CHARACTER_ST *st, OM_CHARACTER_INFO *inf);
void om_myshadowst_init(OM_CHARACTER_ST *st, OM_CHARACTER_INFO *inf);
static void om_on_walk(OM_CHARACTER_DATA *data, int nd_no);
static void om_off_walk(OM_CHARACTER_DATA *data, int nd_no);
static int om_character_cornfrake_get(OM_CHARACTER_ST *st, OM_BG_STAGE_ST *stage);
static void om_character_shadow_move(OM_CHARACTER_ST *st, float x, float z);

/* ===========================================================================
	Function Definitions
 =========================================================================== */
u32 om_mycharacter_main(OM_CAMERA_ST         *cst,
                        OM_CHARACTER_ST      *st,
                        OM_BG_STAGE_ST       *stage)
{
    #ifdef NUALSGI
    static u32 vsync_cnt = 0;
#endif
    OM_CHARACTER_DATA *data;
    u32 rr;
    f32 dumy;

    data = om_com_charast_chain_search(st, OM_FRAKE);

    /* Main character cannot move during attck */
    if (data->action_flg == OM_FRAKE_STOP) {
        rr = om_expad_lookat_xydeg(pad[0].about_x, pad[0].about_y, &dumy);
        if (rr != (u32)-1) {
            /* Chagne the direction of the character and move it by operationg
		the analog stick */
            data->ry =rr+90 + cst->ry*180/PI;
            data->x += sinf(cst->ry+ rr*PI/180+ PI/2)*dumy/2.0;
            data->z += cosf(cst->ry+ rr*PI/180+ PI/2)*dumy/2.0;
            /* Start walking animation of Flake-kun */
            om_on_walk(data, OM_FRAKE_WALK_NODE);
#ifdef NUALSGI
            if (vsync_cnt > 20) {
                om_audio_se_start(1);
                vsync_cnt = 0;
            } else {
                ++vsync_cnt;
            }
#endif
        } else {
            /* Stop walking animation of Flake-kun */
            om_off_walk(data, OM_FRAKE_WALK_NODE);
        }
    }

    /* Chack for collisions with walls on the map */
        if (om_bg_character_wall_hit_check(data, stage) == OM_GOAL) {
        om_audio_se_start(FX_OHYEAH);
        return OM_GOAL;
    }

    /* Synchronize Flake-kun's shadow with his movement */
    om_character_shadow_move(st, data->x, data->z);

    /* Process so that areas far from the area in which Flake-kun is located are	not rendered */
    om_com_memset((char*)stage->area_dsp_flg, NULL, sizeof(stage->area_dsp_flg));
    om_bg_map_area_culling(stage->area_dsp_flg, data->area_number);

    /* Set to that only the left and right portions of the zone where Flake-kun is located are rendered */
    om_com_memset((char*)stage->zone_dsp_flg, NULL, sizeof(stage->zone_dsp_flg));
    om_bg_map_zone_culling(stage->zone_dsp_flg, data->area_number,
                           data->zone_x, data->zone_z);

    /* Check for collisions with Lucky Cornflake */
    om_character_cornfrake_get(st, stage);

    return 0;
}

void om_mycharacterst_init(OM_CHARACTER_ST *st, OM_CHARACTER_INFO *inf)
{
    OM_CHARACTER_DATA      *data;
    OM_CHARACTER_INFO_DATA *infdata;

    infdata = (inf->data + OM_FRAKE);
    data = om_com_charast_chain_search(st, OM_FRAKE);
    om_characterst_init(data, infdata);
    om_com_character_cur_instance_change(data, OM_FRAKE_WALK_NODE);
    om_com_character_globalresource_change(data);
    ndDeleteInstanceGID(data->cur_instp->anim_root_instp, -1, 0x1000, 0x1000);
}

void om_myshadowst_init(OM_CHARACTER_ST *st, OM_CHARACTER_INFO *inf)
{
    OM_CHARACTER_DATA      *data;
    OM_CHARACTER_INFO_DATA *infdata;

    infdata = (inf->data + OM_FRAKE_SHADOW);
    data = om_com_charast_chain_search(st, OM_FRAKE_SHADOW);
    om_characterst_init(data, infdata);
}

static void om_on_walk(OM_CHARACTER_DATA *data, int nd_no)
{
    if (!data->walk_flg) {
        om_com_character_cur_instance_change(data, nd_no);
        om_com_character_globalresource_change(data);
        ndAttachAnimation2(data->cur_instp->anim_root_instp, data->cur_instp->instp, 1, 0, 0x1000, 2, 2);
        data->walk_flg = OM_FRAKE_WALK;
        om_audio_se_start(FX_KUTSU);
    }
}

static void om_off_walk(OM_CHARACTER_DATA *data, int nd_no)
{
    if (data->walk_flg) {
        om_com_character_cur_instance_change(data, nd_no);
        om_com_character_globalresource_change(data);
        ndDeleteInstanceGID(data->cur_instp->anim_root_instp, -1, 0x1000, 0x1000);
        data->walk_flg = OM_FRAKE_STOP;
        om_audio_snd_player_snd_stop(FX_KUTSU);
    }
}

static int om_character_cornfrake_get(OM_CHARACTER_ST *st, OM_BG_STAGE_ST *stage)
{
    u32 corn_num;
    OM_CHARACTER_DATA *mychar, *cornfrake;
    OM_CORNFRAKE_ST   *corn;
    OM_HIT_CHECK_ST   myhit, cornhit;

    mychar    = st->data;
    cornfrake = st->data->next;
    myhit.x   = mychar->x;
    myhit.y   = mychar->y;
    myhit.z   = mychar->z;
    myhit.r   = mychar->r;

    corn = stage->corn;
    for(corn_num=0; corn_num<stage->cornfrake_num; corn_num++)
    {
        /* Skip hit evaluation if Corn has already been taken */
        if (corn[corn_num].dsp_status == 2)
            continue;
        /* Skip hit evaluation if Cornflake is not in the rendering area */
        if (!stage->area_dsp_flg[corn[corn_num].area_number])
            continue;

        cornhit.x = corn[corn_num].x;
        cornhit.y = corn[corn_num].y;
        cornhit.z = corn[corn_num].z;
        cornhit.r = corn[corn_num].r;

        /* Check for collisions between Cornflake and the main character */
        if (om_com_circle_hit_check(&myhit, &cornhit)) {
            /* Lucky Cornflake taken */
            stage->help_num += 1;
            cornfrake->dsp_status = 0;
            corn[corn_num].dsp_status = 2;
            om_audio_se_start(FX_GETIT);
            return 1;
        }
    }
    return 0;
}

static void om_character_shadow_move(OM_CHARACTER_ST *st, float x, float z)
{
    OM_CHARACTER_DATA *data;

    /* Search for shadow character data */
    data = om_com_charast_chain_search(st, OM_FRAKE_SHADOW);
    data->x = x;
    data->z = z;
}
