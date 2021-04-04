/*======================================================================*/
/*	Mirror House							*/
/*	om_audio.h							*/
/*									*/
/*	Copyright (C) 1998 MONEGI CORPORATION				*/
/*									*/
/*======================================================================*/

#ifndef _OM_AUDIO_H_
#define _OM_AUDIO_H_

#ifdef NUALSTL
enum {
    OM_WBANK            = 0,
    OM_PBANK            = 2,
    OM_GAME_MAIN_MUSIC  = 4,
    OM_SELECT_MUSIC     = 6,
    OM_TITLE_MUSIC      = 8,
    OM_SFXBANK		= 10
};
#else
enum {
    OM_WBANK            = 2,  /* midibank  */
    OM_PBANK            = 4,  /* miditable */
    OM_GAME_MAIN_MUSIC  = 0,
    OM_SELECT_MUSIC     = 0,
    OM_TITLE_MUSIC      = 0,
    OM_SFXBANK          = 6,
    OM_SFXTABLE         = 8
};
#endif

/* ===========================================================================
	SGI Sound (Start)
 =========================================================================== */
#ifdef NUALSGI          /* <<< SGI Sound */

#ifdef N_AUDIO
#include <nualsgi_n.h>
#else
#include <nualsgi.h>
#endif

extern u8 _seqSegmentRomStart[];
extern u8 _seqSegmentRomEnd[];
extern u8 _midibankSegmentRomStart[];
extern u8 _midibankSegmentRomEnd[];
extern u8 _miditableSegmentRomStart[];
extern u8 _miditableSegmentRomEnd[];
extern u8 _sfxbankSegmentRomStart[];
extern u8 _sfxbankSegmentRomEnd[];
extern u8 _sfxtableSegmentRomStart[];
extern u8 _sfxtableSegmentRomEnd[];


/* ===========================================================================
	Function Prototype Declarations
 =========================================================================== */
void om_audio_music_start(void);
void om_audio_se_start(u32 se_no);
void om_stage_cleartime_check_and_music_tempo_change(OM_BG_STAGE_ST *stage);
void om_audio_music_init(void);
void om_audio_seq_player_play(void);
void om_audio_snd_bank_set(void);
void om_audio_seq_bank_set(u32 bank_no);
void om_audio_seq_player_seq_stop(u32 seq_no);

#define FX_NULL                 0
#define FX_KUTSU                1
#define FX_ONAKASUITA           2
#define FX_SELECTIT             3
#define FX_MOUDAMEDA            4
#define FX_OHYEAH               5
#define FX_GETIT                6

#endif  /* NUALSGI */


/* ===========================================================================
	SGI Sound (End)
 =========================================================================== */


/* ===========================================================================
	N64 Sound Tools (End)
 =========================================================================== */
#ifdef NUALSTL          /* >>> N64 Sound Tools */

#ifdef N_AUDIO
#include <nualstl_n.h>
#else
#include <nualstl.h>
#endif

#include <libmus.h>
#include <libmus_data.h>
#include "sample1.h"

#ifdef _OM_AUDIO_C_
u32 seqHandle = 0;
u32 sndHandle[7];
s32 om_volume;
#endif

extern u8 _wbankSegmentRomStart[];
extern u8 _pbankSegmentRomStart[];
extern u8 _pbankSegmentRomEnd[];
extern u8 _game_main_musicSegmentRomStart[];
extern u8 _game_main_musicSegmentRomEnd[];
extern u8 _select_musicSegmentRomStart[];
extern u8 _select_musicSegmentRomEnd[];
extern u8 _title_musicSegmentRomStart[];
extern u8 _title_musicSegmentRomEnd[];
extern u8 _sfxbankSegmentRomStart[];
extern u8 _sfxbankSegmentRomEnd[];

#define SEWBANK_START   _sewbankSegmentRomStart
#define SEPBANK_START   _sepbankSegmentRomStart
#define SEPBANK_END     _sepbankSegmentRomEnd

/* ===========================================================================
	Function Prototype Declarations
 =========================================================================== */
void om_audio_music_start(void);
void om_audio_se_start(u32 se_no);
void om_stage_cleartime_check_and_music_tempo_change(OM_BG_STAGE_ST *stage);
void om_audio_music_init(void);
void om_audio_seq_player_play(void);
void om_audio_snd_bank_set(void);
void om_audio_seq_bank_set(u32 bank_no);
void om_audio_seq_player_seq_stop(u32 seq_no);
void om_audio_snd_player_snd_stop(u32 se_no);
void om_audio_snd_player_stop(void);

#endif /* NUALSTL END */
/* ===========================================================================
	N64 Sound Tools (End)
 =========================================================================== */
#endif
