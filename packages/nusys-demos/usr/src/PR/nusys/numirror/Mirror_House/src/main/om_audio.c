/*======================================================================*/
/*	Mirror House							*/
/*	om_audio.c							*/
/*									*/
/*	Copyright (C) 1998 MONEGI CORPORATION				*/
/*									*/
/*======================================================================*/
#include "pack.h"

#include "nusys.h"
#include "libmus.h"
#include "om.h"
#include "om_background.h"
#include "om_bg.h"

#define _OM_AUDIO_C_
#include "om_audio.h"

/* ===========================================================================
	N64 Sound Tools
 =========================================================================== */
#ifdef NUALSTL
u8 *om_audio_rom_data[] = {_wbankSegmentRomStart, NULL,
                           _pbankSegmentRomStart, _pbankSegmentRomEnd,
                           _game_main_musicSegmentRomStart, _game_main_musicSegmentRomEnd,
                           _select_musicSegmentRomStart, _select_musicSegmentRomEnd,
                           _title_musicSegmentRomStart, _title_musicSegmentRomEnd,
			   _sfxbankSegmentRomStart, _sfxbankSegmentRomEnd,
};
/* Pointers to the data to be read */
u32 om_audio_pbank_ptr[NU_AU_SAMPLE_SIZE]__attribute__((aligned(16)));
u32 om_audio_seq_ptr[NU_AU_SONG_SIZE]__attribute__((aligned(16)));
u32 om_audio_snd_ptr[NU_AU_SE_SIZE]__attribute__((aligned(16)));

#else
u8 *om_audio_rom_data[] = {_seqSegmentRomStart, _seqSegmentRomEnd,
                           _midibankSegmentRomStart, _midibankSegmentRomEnd,
                           _miditableSegmentRomStart, _miditableSegmentRomEnd,
                           _sfxbankSegmentRomStart, _sfxbankSegmentRomEnd,
                           _sfxtableSegmentRomStart, _sfxtableSegmentRomEnd
};
#endif

/* ===========================================================================
	Function Definitions
 =========================================================================== */
void om_audio_music_start(void)
{
    u32 rom_size;
#ifdef NUALSTL
    musConfig c;

    c.control_flag	= 0;
    c.channels		= NU_AU_CHANNELS;
    c.sched		= NULL;
    c.thread_priority	= NU_AU_MGR_THREAD_PRI;
    c.heap		= (unsigned char*)NU_AU_HEAP_ADDR;
    c.heap_length	= NU_AU_HEAP_SIZE;
    c.ptr		= NULL;
    c.wbk		= NULL;
    c.default_fxbank	= NULL;
    c.fifo_length	= NU_AU_FIFO_LENGTH;
    c.syn_updates	= NU_AU_SYN_UPDATE_MAX;
    c.syn_output_rate	= 44100;
    c.syn_rsp_cmds	= NU_AU_CLIST_LEN;
    c.syn_retraceCount	= 1;
    c.syn_num_dma_bufs	= NU_AU_DMA_BUFFER_NUM;
    c.syn_dma_buf_size	= NU_AU_DMA_BUFFER_SIZE;

    /* Initialize audio manager */
    nuAuStlMgrInit(&c);

    /* Register PRENMI function */
    nuAuPreNMIFuncSet(nuAuPreNMIProc);

    rom_size = om_audio_rom_data[OM_PBANK+1] - om_audio_rom_data[OM_PBANK];
    if(rom_size&1) rom_size++; /* Check fraction */
    nuPiReadRom((u32)om_audio_rom_data[OM_PBANK], om_audio_pbank_ptr, rom_size);
    MusPtrBankInitialize(om_audio_pbank_ptr, om_audio_rom_data[OM_WBANK]);
    om_audio_snd_bank_set();
    om_audio_music_init();
#else
    nuAuInit();
    rom_size = om_audio_rom_data[OM_WBANK+1] - om_audio_rom_data[OM_WBANK];
    /* Register sequence data (BG sound data) to the bank  */
    nuAuSeqPlayerBankSet(om_audio_rom_data[OM_WBANK], rom_size,
                         om_audio_rom_data[OM_PBANK]);
    nuAuSeqPlayerSeqSet(om_audio_rom_data[OM_GAME_MAIN_MUSIC]);
    /* Register the sound effect data to the bank */
    om_audio_snd_bank_set();
    om_audio_music_init();
#endif
}

void om_audio_se_start(u32 se_no)
{
#ifdef NUALSTL
    /*** Play the sound effect specified by se_no ***/
    sndHandle[se_no] = MusStartEffect(se_no);
    MusSetMasterVolume(MUSFLAG_EFFECTS, 0x7fff);
    MusHandleSetFreqOffset(sndHandle[se_no], -6.0);
    MusHandleSetVolume(sndHandle[se_no], 0x100);
#else
#endif
}

void om_stage_cleartime_check_and_music_tempo_change(OM_BG_STAGE_ST *stage)
{
#ifdef NUALSTL
    static u32 tempo = 0x75;

    /*** Check the stage clear time and change the sound tempo */
    if (stage->clear_time < OM_STAGE_CLEAR_TIME) {
	MusHandleSetTempo(seqHandle, tempo);
        tempo += 0x05;
    } else {
        tempo = 0x75;
    }
#else
    static u32 tempo;

    /*** Check the clear time for the stage and change the sound tempo. */
    if (stage->clear_time < OM_STAGE_CLEAR_TIME) {
	tempo = nuAuSeqPlayerGetTempo(0);
	tempo -= 1;
	nuAuSeqPlayerSetTempo(0, tempo);
    }
#endif
}

void om_audio_music_init(void)
{
    /*** Initialize the sound tempo. */
#ifdef NUALSTL
    MusHandleSetTempo(seqHandle, 0x70);
#else
    s32 tempo;
    /*** Initialize the sound tempo. */
    tempo = nuAuSeqPlayerGetTempo(0);
#if 0
    nuAuSeqPlayerSetTempo(0, tempo);
#endif
#endif
}

void om_audio_seq_player_play(void)
{
#ifdef NUALSTL
    om_volume = 0x7fff;
    /* Set the Sequence Player volume. */
    MusSetMasterVolume(MUSFLAG_SONGS, om_volume);
    /* Play the sequence. */
    seqHandle = MusStartSong(om_audio_seq_ptr);
#else
    nuAuSeqPlayerSetNo(0, 0);
    /* Play the sequence. */
    nuAuSeqPlayerPlay(0);
#endif
}

void om_audio_snd_bank_set(void)
{
    u32 bank_size;
#ifdef NUALSTL
    bank_size = om_audio_rom_data[OM_SFXBANK+1] - om_audio_rom_data[OM_SFXBANK];
    if(bank_size&1) bank_size++; /* Check fraction */
    nuPiReadRom((u32)om_audio_rom_data[OM_SFXBANK], om_audio_snd_ptr, bank_size);
    MusFxBankInitialize((void*)om_audio_snd_ptr);
#else
    bank_size = om_audio_rom_data[OM_SFXBANK+1] - om_audio_rom_data[OM_SFXBANK];
    /* Register sound effect data in the bank. */
    nuAuSndPlayerBankSet(om_audio_rom_data[OM_SFXBANK], bank_size,
                         om_audio_rom_data[OM_SFXTABLE]);
#endif
}

void om_audio_seq_bank_set(u32 bank_no)
{
    u32 bank_size;
#ifdef NUALSTL
    bank_size = om_audio_rom_data[bank_no+1] - om_audio_rom_data[bank_no];
    if(bank_size&1) bank_size++; /* Check fraction */
    /* Register the sequence data (BG Sound data) in the bank. */
    nuPiReadRom((u32)om_audio_rom_data[bank_no], om_audio_seq_ptr, bank_size);
#else
#endif
}

void om_audio_seq_player_seq_stop(u32 seq_no)
{
#ifdef NUALSTL
    MusStop(MUSFLAG_SONGS, 0);
#else
    /* Stop the Sequence Player. */
    nuAuSeqPlayerStop(seq_no);
#endif
}

void om_audio_snd_player_snd_stop(u32 se_no)
{
#ifdef NUALSTL
    MusHandleStop(sndHandle[se_no], 0);
#else
#endif
}

void om_audio_snd_player_stop(void)
{
#ifdef NUALSTL
    MusStop(MUSFLAG_EFFECTS, 0);
#else
#endif
}

