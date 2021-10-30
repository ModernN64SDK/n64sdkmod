/*======================================================================*/
/*	Mirror House							*/
/*	om_game_main.c							*/
/*									*/
/*	Copyright (C) 1998 MONEGI CORPORATION				*/
/*									*/
/*======================================================================*/
#include "pack.h"

#include "nd.h"
#include "nusys.h"

#include "om.h"
#include "om_def.h"
#include "om_audio.h"

/* ===========================================================================
	External Reference Declarations
 =========================================================================== */
u32 om_continue_cnt = 9;

/* ===========================================================================
	External Reference Declarations
 =========================================================================== */

/* ===========================================================================
	External Function Reference Declarations
 =========================================================================== */
extern void om_com_character_cur_instance_change(OM_CHARACTER_DATA *data, int nd_no);
extern void om_com_character_globalresource_change(OM_CHARACTER_DATA *data);
extern OM_CHARACTER_DATA *om_com_charast_chain_search(OM_CHARACTER_ST *st, u32 entry_number);
extern void om_audio_se_start(u32 se_no);
void om_audio_snd_player_stop(void);

/* ===========================================================================
	Function Prototype Declarations
 =========================================================================== */
void om_game_over_begin(OM_CHARACTER_ST *st);
int om_game_over_main(OM_CHARACTER_ST *st);

/* ===========================================================================
	Function Definitions
 =========================================================================== */
void om_game_over_begin(OM_CHARACTER_ST *st)
{
    OM_CHARACTER_DATA *data;

    data = om_com_charast_chain_search(st, OM_FRAKE);

    if (data->walk_flg == OM_FRAKE_WALK) {
        om_com_character_cur_instance_change(data, OM_FRAKE_WALK_NODE);
        om_com_character_globalresource_change(data);
        ndDeleteInstanceGID(data->cur_instp->anim_root_instp, -1, 0x1000, 0x1000);
        om_audio_snd_player_stop();
    }
    om_com_character_cur_instance_change(data, OM_FRAKE_DOWN_NODE);
    om_com_character_globalresource_change(data);
    ndAttachAnimation2(data->cur_instp->anim_root_instp,
                       data->cur_instp->instp, 1, 0, 0x1001, 2, 1);
    data->time_cnt = 58;

    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_BLACK);
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 15, 15);
    nuDebConPuts(NU_DEB_CON_WINDOW0, "GAME  OVER");
    /* Set Flake-kun shadow to no-display */
    data = om_com_charast_chain_search(st, OM_FRAKE_SHADOW);
    data->dsp_status = OM_NODISPLAY;
    om_audio_se_start(FX_MOUDAMEDA);    /* Play sound effects when Flake-kun down */
}

int om_game_over_main(OM_CHARACTER_ST *st)
{
    OM_CHARACTER_DATA *data;

    data = st->data;
    if (data->time_cnt < 0) {
        ndDeleteInstanceGID(data->cur_instp->anim_root_instp, -1, 0x1001, 0x1001);
        return 1;
    }
    --data->time_cnt;
    return 0;
}

