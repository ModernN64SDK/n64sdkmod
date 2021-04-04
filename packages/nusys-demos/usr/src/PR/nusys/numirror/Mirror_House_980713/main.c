/*======================================================================*/
/*	Mirror House							*/
/*	main.c								*/
/*									*/
/*	Copyright (C) 1998 MONEGI CORPORATION				*/
/*									*/
/*======================================================================*/
#include <pack.h>

#include "nd.h"
#include "nusys.h"

#define _MAIN_
#include "om.h"

#include "om_info.h"
#include "om_data.h"
#include "om_audio.h"
#include "om_segment.h"

extern char mem_heep[1024*512*1];

/* ========================================================================== 
	define
 ========================================================================== */

/* ===========================================================================
	Global Structure Declarations
 =========================================================================== */

/* ===========================================================================
	Global Variable Declarations
 =========================================================================== */
static int om_reset_flg=0;
int nCharacterNumber = 0xff;

/* ===========================================================================
	Data Definitions
 =========================================================================== */

/* ===========================================================================
	Function Protorype Declarations
 =========================================================================== */
static void om_game_main(int pedingGfx);
static void om_system_reset(void);
void om_game_data_entry(void);
int om_system_heap_memory_init(void);
static void om_system_init(void);
static void om_opening_init(void);

/* ===========================================================================
	Declarations of External Reference Funcrion Prototypes
 =========================================================================== */
/*** om_game_main.c ***/
extern void om_game_init(OM_BG_STAGE_ST  *stage, OM_CAMERA_ST *cst,
                         OM_CHARACTER_ST *st,    OM_CHARACTER_INFO *inf);
extern void om_game_main_end(OM_BG_STAGE_ST  *stage, OM_CAMERA_ST *cst);
extern int om_game_main_control(OM_CAMERA_ST    *cst, OM_CHARACTER_ST *st,
                                OM_BG_STAGE_ST  *stage);

/*** om_camera.c ***/
extern void om_camera_init(OM_CAMERA_ST *cst);
extern OM_CAMERA_ST *om_camera_entry(void);
extern void om_camera_entry_delete(OM_CAMERA_ST *cst);
extern Gfx *om_camera_perspective_draw(Gfx *gp, OM_CAMERA_ST *cst, float um[4][4]);
extern Gfx *om_camera_ortho_draw(Gfx *gp);

/*** om_eval_draw.c ***/
extern Gfx *om_game_draw3d(Gfx *gp, OM_CHARACTER_ST *st, float um[4][4]);
extern void om_game_eval(OM_CHARACTER_ST *st, float mf[4][4]);

/*** om_char_entry.c ***/
extern void om_chara_anim_entry(OM_CHARACTER_ST *st, OM_CHARACTER_INFO *inf);

/*** om_bg_entry.c ***/
extern int om_bg_stage_entry(OM_BG_STAGE_ST **stage);
extern void om_bg_stage_entry_delete(OM_BG_STAGE_ST *stage);
extern Gfx *om_bg_backgroundst_draw(Gfx *gp, OM_CHARACTER_ST *st, OM_BG_STAGE_ST *stage);

/*** om_select_chara.c ***/
extern void om_select_character_main(int pedingGfx);

/*** om_title.c ***/
extern void om_title_main(int pedingGfx);

/*** om_audio.c ***/
extern void om_audio_music_start(void);
void om_audio_seq_bank_set(u32 bank_no);

/*** om_rom_data_read.c ***/
extern void om_pi_dma_data_read(u32 seg_no);

/* ---------------------------------------------------------------------------
	Main Function
	Main function deallocated by the user; no exit here
 --------------------------------------------------------------------------- */
void mainproc(void *arg)
{
    om_program_seq_st.seq_no = OM_SYSTEM_INIT;

    /* Set callback function called when reset buton pushed */
    nuPreNMIFuncSet((NUScPreNMIFunc)om_system_reset);

    while(1)
    {
	switch(om_program_seq_st.seq_no)
	{
	case OM_SYSTEM_INIT:      /* System initialization */
	    om_system_init();
	    break;
	case OM_OPENING_INIT:
	    om_opening_init();
	    break;
	case OM_GAME_TITLE:       /* Game title */
	    om_program_seq_st.seq_no = OM_STANDBY;
	    nuGfxFuncSet((NUGfxFunc)om_title_main);
	    break;
	case OM_GAME_DATA_ENTRY:  /* Register game data */
	    nuGfxFuncRemove();
	    om_game_data_entry();
	    break;
	case OM_GAME_SELECT:     /* Select character */
	    om_program_seq_st.seq_no = OM_STANDBY;
	    nuGfxFuncSet((NUGfxFunc)om_select_character_main);
	    break;
	case OM_GAME_INIT:       /* Initialize game data */
	    nuGfxFuncRemove();
	    om_game_init(om_bg_stage_st, om_camera_st,
			 &om_character_st, &om_character_info);
	    om_program_seq_st.seq_no = OM_GAME_MAIN;
	    break;
	case OM_GAME_MAIN:       /* Game main */
	    om_program_seq_st.seq_no = OM_STANDBY;
	    nuGfxFuncSet((NUGfxFunc)om_game_main);
	    break;
	case OM_STANDBY:
	    break;
	}
	nuGfxDisplayOn();
        while(om_reset_flg);
    }
}

static void om_game_main(int pedingGfx)
{
    Gfx *gp;
    float um[4][4];
    OM_CHARACTER_ST *st;
    OM_CAMERA_ST         *cst;
    OM_BG_STAGE_ST       *stage;

    st   = &om_character_st;
    cst  = om_camera_st;
    stage = om_bg_stage_st;

    /* Game main */
    if (om_game_main_control(cst, st, stage)) {
        om_program_seq_st.seq_no = OM_OPENING_INIT;
        /* Initialize game data when game over */
#if 0
        om_game_init(stage, cst, st, &om_character_info);
#endif
        /* Start processing of game main end */
        om_game_main_end(stage, cst);
        nuGfxFuncRemove();
        return;
    }

    /* Evaluate frame for each character */
    om_game_eval(st, um);

    /* Build rendering data (generate display list) */
    if (pedingGfx < 2) {
        if (gp = gfxBegin()){
            /* Build scene using perspective projection */
            gp = om_camera_perspective_draw(gp, cst, um);
            gp = om_game_draw3d(gp, st, um);
            gp = om_bg_backgroundst_draw(gp, st, stage);
            gfxEnd(gp);
            gfxFlush(NU_SC_NOSWAPBUFFER, NU_GFX_UCODE_F3DEX_NON);
        }
    }

    /* Updata pad data */
    pad = expadReadController();
    gfxWaitSync();
}

static void om_system_reset(void)
{
    om_program_seq_st.seq_no = OM_SYSTEM_INIT;
    om_reset_flg = 1;
}

void om_game_data_entry(void)
{
    om_audio_seq_player_seq_stop(0);
    /* register character data */
    om_chara_anim_entry(&om_character_st, &om_character_info);
    /* Generate and initialize table for camera */
    om_camera_st = om_camera_entry();
    /* register stage data */
    om_bg_stage_st = (OM_BG_STAGE_ST*)NULL;
    om_bg_stage_entry(&om_bg_stage_st);

    /* Read character select BG sprite data */
    om_pi_dma_data_read(OM_SELECT_BG_SEGMENT);
    om_audio_seq_bank_set(OM_SELECT_MUSIC);
    om_audio_seq_player_play();

    /* Transit the program sequence to the character selections */
    om_program_seq_st.seq_no = OM_GAME_SELECT;
}

int om_system_heap_memory_init(void)
{
    /* Reserve system heap memory */
    if (InitHeap(mem_heep, sizeof(mem_heep)) == -1) {
        osSyncPrintf("Heep Memory Alloc Error\n");
        return -1;
    }
    return 0;
}

static void om_system_init(void)
{
    gfxInit();                           /* Initialize the graphics module */
    om_system_heap_memory_init();        /* Reserve heap memory            */
    om_audio_music_start();              /* Initialize sound               */
    expadInitControllers( 4, 20, 4 );    /* Initialize the controllers     */
    pad = expadReadController();

    /* Enable to use debug console */
    nuDebConWindowShow( NU_DEB_CON_WINDOW0, NU_DEB_CON_WINDOW_ON );
    nuDebConWindowShow( NU_DEB_CON_WINDOW1, NU_DEB_CON_WINDOW_ON );
    nuDebConWindowShow( NU_DEB_CON_WINDOW2, NU_DEB_CON_WINDOW_ON );
    nuDebConWindowShow( NU_DEB_CON_WINDOW3, NU_DEB_CON_WINDOW_ON );

    om_program_seq_st.seq_no = OM_OPENING_INIT;
}

static void om_opening_init(void)
{
    om_program_seq_st.seq_no = OM_GAME_TITLE;
}
