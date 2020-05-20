/*======================================================================*/
/*	Mirror House							*/
/*	om_game_main.c							*/
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

/* ===========================================================================
	Global Variable Declarations
 =========================================================================== */
static u32 om_vsync_cnt = 0;          /* Counter variable for the timer */
static int om_game_over_flag = 0;     /* Game over flag */

/* ===========================================================================
	External Reference Declarations
 =========================================================================== */
extern EXCONTPAD *pad;

/* ===========================================================================
	External Function Reference Declarations
 =========================================================================== */
extern void om_game_over_begin(OM_CHARACTER_ST *st);
extern int om_game_over_main(OM_CHARACTER_ST *st);
extern void om_goal_goal_plate_main(OM_CHARACTER_ST *st);
extern void om_stage_cleartime_check_and_music_tempo_change(OM_BG_STAGE_ST *stage);
extern void om_camera_game_camera_action(OM_CAMERA_ST *cst, OM_CHARACTER_ST *st);
extern void om_camera_entry_delete(OM_CAMERA_ST *cst);
extern int om_camera_mode_check(OM_CAMERA_ST *cst, OM_BG_STAGE_ST *stage);
extern void om_camera_mode_on_game_view_mode(void);
extern int om_system_heap_memory_init(void);
extern u32 om_mycharacter_main(OM_CAMERA_ST *cst, OM_CHARACTER_ST *st,
                               OM_BG_STAGE_ST *stage);
extern void om_mycharacterst_init(OM_CHARACTER_ST *st, OM_CHARACTER_INFO *inf);
extern void om_myshadowst_init(OM_CHARACTER_ST *st, OM_CHARACTER_INFO *inf);
extern void om_goal_plate_init(OM_CHARACTER_ST *st);
extern void om_cornfrake_data_init(OM_BG_STAGE_ST *stage);
extern void om_cornfrake_main(OM_CHARACTER_ST *st, OM_BG_STAGE_ST *stage);
extern void om_bg_stage_entry_delete(OM_BG_STAGE_ST *stage);
extern OM_CHARACTER_DATA *om_com_charast_chain_search(OM_CHARACTER_ST *st, u32 entry_number);
extern void om_audio_seq_player_seq_stop(u32 seq_no);
extern void om_audio_snd_player_stop(void);

/* ===========================================================================
	Function Prototype Declarations
 =========================================================================== */
void om_game_init(OM_BG_STAGE_ST  *stage, OM_CAMERA_ST *cst,
                  OM_CHARACTER_ST *st,    OM_CHARACTER_INFO *inf);
void om_game_main_end(OM_BG_STAGE_ST  *stage, OM_CAMERA_ST *cst);
int  om_game_main_control(OM_CAMERA_ST *cst, OM_CHARACTER_ST *st,
                          OM_BG_STAGE_ST *stage);
static int om_game_cleartime_over_check(u32 *clear_time);
static void om_game_cleartime_and_help_num_display(OM_BG_STAGE_ST *stage);
static void om_game_stage_clear(OM_CHARACTER_ST *st);
static int om_game_game_over_procedure_excute_and_init(OM_CHARACTER_ST *st);

/* ===========================================================================
	Function Definitions
 =========================================================================== */
void om_game_init(OM_BG_STAGE_ST  *stage, OM_CAMERA_ST *cst,
                  OM_CHARACTER_ST *st,    OM_CHARACTER_INFO *inf)
{
    u32 i;
    OM_CHARACTER_DATA *data;

    om_audio_seq_player_seq_stop(0);       /* Stop sequence of character selection */
    data = st->data;
    /* not to display all character data */
    for(i=0; i<st->header.character_entry; i++)
    {
        if (data->dsp_status)
            data->dsp_status = 0;
        data = data->next;
    }
    om_mycharacterst_init(st, inf);        /* Initialize Flake-kun   */
    om_myshadowst_init(st, inf);           /* Initialize shadow data */
    om_goal_plate_init(st);                /* Initialize goal plate  */
    om_cornfrake_data_init(stage);         /* Initialize Cornflake data */

    /* register sequence data to bank */
    om_audio_seq_bank_set(OM_GAME_MAIN_MUSIC);
    om_audio_seq_player_play();            /* Play the sequence  */
    om_audio_se_start(FX_ONAKASUITA);
}

void om_game_main_end(OM_BG_STAGE_ST  *stage, OM_CAMERA_ST *cst)
{
    /* Begin completion of game main processing */
    om_bg_stage_entry_delete(stage);      /* Free map data memory */
    om_camera_entry_delete(cst);          /* Free camera data memory */
    om_audio_seq_player_seq_stop(0);      /* Stop the sequence player */
    om_audio_snd_player_stop();           /* Stop all sound */
    om_system_heap_memory_init();         /* Free and reserve system heap memory */
}

int om_game_main_control(OM_CAMERA_ST    *cst,
                         OM_CHARACTER_ST *st,
                         OM_BG_STAGE_ST  *stage)
{
    int camera_mode;
    int ret = 0;

    if (om_game_over_flag) {
        /* Game-over main processing and initialization */
        ret = om_game_game_over_procedure_excute_and_init(st);
        return ret;
    }

/* ===========================================================================
	Game main processing
 =========================================================================== */

    camera_mode = om_camera_mode_check(cst, stage);   /* Camera mode check */
    /* Advance game processing if camera mode is the game mode */
    if (!camera_mode) {
        /* Main character main */
        if (om_mycharacter_main(cst, st, stage) == OM_GOAL) {
            om_game_over_flag = 1;
            om_game_stage_clear(st);      /* Stage clearing */
            return 0;
        }
	    om_cornfrake_main(st, stage);	/* Cornflake main */
    } else if (camera_mode == 2) {
        --stage->help_num;              /* Help number is -1 if in help mode */
    }

    /* Set camera action with the main character as the reference point */
    om_camera_game_camera_action(cst, st);     /* Camera action setting */

    /* Check whether clear time exceeded (if exceeded, game over) */
    if (om_game_cleartime_over_check(&stage->clear_time)) {
        om_game_over_flag = 1;            /* Game-over flag ON */
        om_game_over_begin(st);           /* Begin game-over initialization */
        return 0;
    }
    /* Display clear time and help number to screen */
    om_game_cleartime_and_help_num_display(stage);
    /* Control goal plate in front of goal*/
    om_goal_goal_plate_main(st);
    /* Change BG sound tempo according to remaining stage clear time*/
    om_stage_cleartime_check_and_music_tempo_change(stage);

    if (pad[0].trigger & CONT_B) {
        om_game_over_flag = 1;
        om_game_over_begin(st);           /* Begin intial game-over processing */
        return 0;
    }
    return 0;
}

static int om_game_cleartime_over_check(u32 *clear_time)
{
    if (*clear_time == 0)
        return 1;

    *clear_time -= 1;
    return 0;
}

static void om_game_cleartime_and_help_num_display(OM_BG_STAGE_ST *stage)
{
    u32  help_num;
    u32  min, sec;
    static s32 msec=60;
    char time_str[32], msec_str[3];
    char help_str[32];

    nuDebConClear(NU_DEB_CON_WINDOW0);
    nuDebConClear(NU_DEB_CON_WINDOW1);
    nuDebConClear(NU_DEB_CON_WINDOW2);
    nuDebConClear(NU_DEB_CON_WINDOW3);
    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_BLACK);
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 3, 2);
    min  = stage->clear_time / 3600;
    sec  = (stage->clear_time - (3600 * min)) / 60;

    if (msec < 0)
        msec = 60;

    if (msec < 10)
        sprintf(msec_str, "0%d", msec);
    else
        sprintf(msec_str, "%d", msec);

    if (sec < 10) {
        sprintf(time_str, "CLEAR TIME %d:0%d:%s", min, sec, msec_str);
    } else {
        sprintf(time_str, "CLEAR TIME %d:%d:%s", min, sec, msec_str);
    }
    --msec;

    nuDebConPuts(NU_DEB_CON_WINDOW0, time_str);
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 3, 3);
    if (stage->help_num < 0) {
        help_num = 0;
        stage->help_num = 0;
        /* Restore view mode for game mode */
        om_camera_mode_on_game_view_mode();
    } else 
        help_num = stage->help_num;

    sprintf(help_str, "HELP       %d", help_num);
    nuDebConPuts(NU_DEB_CON_WINDOW0, help_str);
}

static void om_game_stage_clear(OM_CHARACTER_ST *st)
{
    OM_CHARACTER_DATA *data;

    data = om_com_charast_chain_search(st, OM_FRAKE);
    data->time_cnt = -1;

    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_BLACK);
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 15, 15);
    nuDebConPuts(NU_DEB_CON_WINDOW0, "   GOAL   ");
}

static int om_game_game_over_procedure_excute_and_init(OM_CHARACTER_ST *st)
{
    static int ret = 0;

    /* Begin game-over main processing */
    if (ret) {
        if (!om_vsync_cnt) {
            /* Following 3-second wait (timer), prepare to resume game processing */
            om_game_over_flag = 0;
            om_vsync_cnt   = 0;
            ret            = 0;
            return 1;
        } else if (om_vsync_cnt > 0) {
            --om_vsync_cnt;
            return 0;
        }
    } else {
        if ((ret = om_game_over_main(st))) {
            /* Set WAIT timer for 3-sec following game-over processing */
            om_vsync_cnt = 180;
        }
    }
    return 0;
}

