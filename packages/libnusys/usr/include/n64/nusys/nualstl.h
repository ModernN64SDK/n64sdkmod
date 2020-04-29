/*======================================================================*/
/*		NuSYS	Audio Library for SoundTools Library & n_audio	*/
/*		nualstl_n.h						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nualstl.h,v 1.5 1999/06/11 07:58:37 ohki Exp $			*/
/*======================================================================*/
#ifndef _NUALSTL_H_
#define _NUALSTL_H_

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

/*----------------------------------------------------------------------*/    
/*----------------------------------------------------------------------*/
/*	DEFINE								*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
#define	NU_AU_SEQPLAYER_NUM	2
    
#define NU_AU_FIFO_LENGTH	64
#define NU_AU_CHANNELS		24
#define NU_AU_SYN_UPDATE_MAX	256
    
/*--------------------------------------*/
/* AUDIO DMA DEFINE			*/
/*--------------------------------------*/
#define	NU_AU_DMA_BUFFER_NUM	64	/* The number of DMA transfer buffers */
#define	NU_AU_DMA_BUFFER_SIZE	1024    /* The size of the DMA buffer */

/*--------------------------------------*/
/* SOUND DEFINE				*/
/*--------------------------------------*/
#define	NU_AU_MGR_THREAD_PRI	70
#define	NU_AU_MGR_THREAD_ID	6
#define	NU_AU_CLIST_LEN		0x800   /* The buffer size of the command list */
#define	NU_AU_OUTPUT_RATE	32000	/* Setting the frequency	*/
#define	NU_AU_MESG_MAX		4


#define NU_AU_HEAP_SIZE		0x50000	/* The HEAP size of default */
#define NU_AU_HEAP_ADDR		(NU_GFX_FRAMEBUFFER_ADDR - NU_AU_HEAP_SIZE)
#define NU_AU_SONG_SIZE		0x4000
#define NU_AU_SAMPLE_SIZE	0x4000
#define NU_AU_SE_SIZE		0x4000
    
#define NU_AU_TASK_STOP		0	/* Stop running the audio task */
#define NU_AU_TASK_RUN		1	/* Possible to run the audio task */
    
#define NU_AU_SEQ_PLAYER0	0
#define NU_AU_SEQ_PLAYER1	1

#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)
#include <ultra64.h>
    
#include <PR/libmus.h>
#include <PR/libmus_data.h>
    
#ifdef N_AUDIO
#include <PR/n_libaudio_sc.h>
#include <PR/n_libaudio_sn_sc.h>
#endif	/* N_AUDIO */
    
/*--------------------------------------*/
/* audio typedef			*/
/*--------------------------------------*/
/* The sequence player structure */
typedef struct st_Seqence {
    musHandle	handle;
    u8*		data_ptr;       /* Sequence data */
} NUAuSeqPlayer;


/*--------------------------------------*/
/* CALL BACK Function	typedef		*/
/*--------------------------------------*/
typedef void (*NUAuPreNMIFunc)(NUScMsg,u32);	/* The PRENMI call-back function */

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/* extern variables 							*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/    
/*--------------------------------------*/
/*  audio variables 			*/
/*--------------------------------------*/
extern u8		nuAuTaskStop;
extern u8		nuAuPreNMI;
extern NUAuPreNMIFunc	nuAuPreNMIFunc;
extern void*		nuAuEffect_ptr;
extern void*		nuAuPtrBank_ptr;
extern NUScTask		nuAuTask;
extern OSMesgQueue	nuAuMesgQ;
extern NUAuSeqPlayer	nuAuSeqPlayer[];
extern musConfig	nuAuStlConfig;
extern NUScClient	nuAuClient;

/*----------------------------------------------------------------------*/    
/*----------------------------------------------------------------------*/
/* Global Function 							*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*--------------------------------------*/
/* audio manager function		*/
/*--------------------------------------*/
extern s32	nuAuStlInit(void);
extern s32	nuAuStlMgrInit(musConfig* config);
extern void	nuAuStlPtrBankInit(u32 pbk_size);
extern void	nuAuStlPtrBankSet(u8* pbk_addr, u32 pbk_size, u8* wbk_addr);
extern void	nuAuStlSeqPlayerInit(u32 player_no, u32 size);
extern void	nuAuStlSeqPlayerDataSet(u32 player_no, u8 *seq_addr, u32 seq_size);
extern musHandle nuAuStlSeqPlayerPlay(u32 player_no);

extern void	nuAuStlSndPlayerInit(u32 size);
extern u32	nuAuStlSndPlayerPlay(u32 sndNo);
extern u32	nuAuStlSndPlayerPlay2(u32 sndNo, s32 volume, s32 pan, s32 restartflag, s32 priority);
extern void	nuAuStlSndPlayerDataSet(u8* snd_addr, u32 snd_size);
extern void	nuAuPreNMIFuncSet(NUAuPreNMIFunc func);
extern void	nuAuPreNMIProc(NUScMsg mesg_type, u32 frameCounter);
extern void	nuAuStlSndPlayerSetData(u8 *snd_addr, u32 snd_size);
extern void*	nuAuStlHeapAlloc(s32 length);
extern s32	nuAuStlHeapGetFree(void);
extern s32	nuAuStlHeapGetUsed(void);

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/* MACRO	 							*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

/* Delete the call-back function */
#define nuAuPreNMIFuncRemove() nuAuPreNMIFuncSet(NULL)

/*----------------------------------------------------------------------*/
/* nuAuStlSeqPlayerGetState - Get the state of the sequence player	*/
/*	IN:	None							*/
/*	RET:	The channel number that the sequence player is using	*/
/*----------------------------------------------------------------------*/
#define nuAuStlSeqPlayerGetState()					\
	MusAsk(MUSFLAG_SONGS)

/*----------------------------------------------------------------------*/
/* nuAuStlSeqPlayerStop - Stop the performance of all sequence		*/
/*	IN:	speed		The frame number before the stop 	*/
/*----------------------------------------------------------------------*/
#define nuAuStlSeqPlayerStop(speed)					\
	MusStop(MUSFLAG_SONGS, speed)

/*----------------------------------------------------------------------*/
/* nuAuStlSeqPlayerSetMasterVol - Setting volume of the whole sequence	*/
/*	IN:	vol		Volume (0 - 0x7fff)			*/
/*----------------------------------------------------------------------*/
#define nuAuStlSeqPlayerSetMasterVol(vol)				\
	MusSetMasterVolume(MUSFLAG_SONGS, vol)


/*----------------------------------------------------------------------*/
/* nuAuStlSeqPlayerGetSeqState - Get the state of sequence		*/
/*	IN:	player_no	The sequence player number		*/
/*	RET:	The channel number which is currently playing		*/
/*----------------------------------------------------------------------*/
#define nuAuStlSeqPlayerGetSeqState(player_no)				\
	MusHandleAsk(nuAuSeqPlayer[player_no].handle)

/*----------------------------------------------------------------------*/
/* nuAuStlSeqPlayerSeqStop - Stop the performance of sequence		*/
/*	IN:	player_no	The sequence player number		*/
/*		speed		The frame number before the stop	*/
/*----------------------------------------------------------------------*/
#define nuAuStlSeqPlayerSeqStop(player_no, speed)			\
	MusHandleStop(nuAuSeqPlayer[player_no].handle, speed)

/*----------------------------------------------------------------------*/
/* nuAuStlSeqPlayerSetSeqTempo - Setting tempo of sequence		*/
/*	IN:	player_no	The sequence player number		*/
/*		tempo		Tempo (0 - 0x100)			*/
/*----------------------------------------------------------------------*/
#define nuAuStlSeqPlayerSetSeqTempo(player_no, tempo)			\
	MusHandleSetTempo(nuAuSeqPlayer[player_no].handle, tempo)

/*----------------------------------------------------------------------*/
/* nuAuStlSeqPlayerSetSeqVol - Setting volume of sequence		*/
/*	IN:	player_no	The sequence player number		*/    
/*		vol		The volume scale (0 - 0x100)		*/
/*----------------------------------------------------------------------*/
#define nuAuStlSeqPlayerSetSeqVol(player_no, vol)			\
	MusHandleSetVolume(nuAuSeqPlayer[player_no].handle, vol)

/*----------------------------------------------------------------------*/
/* nuAuStlSeqPlayerSetSeqPan - Setting the pan position of sequence	*/
/*	IN:	player_no	The sequence player number		*/    
/*		pan		The pan scale				*/
/*				0=Left 0x80=Center 0x100=Right		*/
/*----------------------------------------------------------------------*/
#define nuAuStlSeqPlayerSetSeqPan(player_no, pan)			\
	MusHandleSetPan(nuAuSeqPlayer[player_no].handle, pan )

/*----------------------------------------------------------------------*/
/* nuAuStlSndPlayerGetState - Setting the pan position of sequence	*/
/*	IN:	None							*/
/*	RET:	The channel number which is currently playing		*/
/*----------------------------------------------------------------------*/
#define nuAuStlSndPlayerGetState()					\
	MusAsk(MUSFLAG_EFFECTS)

/*----------------------------------------------------------------------*/
/* nuAuStlSndPlayerStop - Stop playback of all sound			*/
/*	IN:	speed		The frame number before the stop	*/
/*----------------------------------------------------------------------*/
#define nuAuStlSndPlayerStop(speed)					\
	MusStop(MUSFLAG_EFFECTS, speed)

/*----------------------------------------------------------------------*/
/* nuAuStlSndPlayerSetMasterVol - Setting volume of the whole sound	*/
/*	IN:	vol		Volume (0 - 0x7fff)			*/
/*----------------------------------------------------------------------*/
#define nuAuStlSndPlayerSetMasterVol(vol)				\
	MusSetMasterVolume(MUSFLAG_EFFECTS, vol)


/*----------------------------------------------------------------------*/
/* nuAuStlSndPlayerGetSndState - Get the state of sound			*/
/*	IN:	handle		The sound handler			*/
/*		None							*/
/*	RET:	The channel number which is currently playing		*/
/*----------------------------------------------------------------------*/
#define nuAuStlSndPlayerGetSndState(handle)				\
	MusHandleAsk(handle)

/*----------------------------------------------------------------------*/
/* nuAuStlSndPlayerSndStop - Stop the sound performance			*/
/*	IN:	handle		The sound handle			*/
/*		speed		The frame number before the stop	*/
/*----------------------------------------------------------------------*/
#define nuAuStlSndPlayerSndStop(handle, speed)				\
	MusHandleStop(handle, speed)

/*----------------------------------------------------------------------*/
/* nuAuStlSndPlayerSetSndVol - Setting volume of sound			*/
/*	IN:	handle		The sound handle			*/
/*		vol		The volume scale (0 - 0x100)		*/
/*----------------------------------------------------------------------*/
#define nuAuStlSndPlayerSetSndVol(handle, vol)				\
	MusHandleSetVolume(handle, vol)

/*----------------------------------------------------------------------*/
/* nuAuStlSndPlayerSetSndPan - Setting the pan position of sound	*/
/*	IN:	handle		The sound handler			*/
/*		pan		The pan scale				*/
/*				0=Left 0x80=Center 0x100=Right		*/
/*----------------------------------------------------------------------*/
#define nuAuStlSndPlayerSetSndPan(handle, pan)				\
	MusHandleSetPan(handle, pan )

/*----------------------------------------------------------------------*/
/* nuAuStlSndPlayerSetSndPitch - Setting sound pitch			*/
/*	IN:	handle		The sound handler			*/
/*		pitch		The pitch value(-6.0 - +6.0)		*/
/*----------------------------------------------------------------------*/
#define nuAuStlSndPlayerSetSndPitch(handle, pitch)			\
	MusHandleSetFreqOffset(handle, pitch)
	    
/************************************************************************/
/*	Incorporate name changes from the beta version (not recommended).				*/
/************************************************************************/
#define	nuAuStlBankSet(pbk_addr, pbk_size, wbk_addr)			\
{									\
    nuAuStlPtrBankInit(pbk_size);					\
    nuAuStlPtrBankSet(pbk_addr, pbk_size, wbk_addr);			\
}

/*****************************************************************************/
/*This macro is included to provided compatibility with the previous version,*/
/* however, is scheduled to be removed. Make sure to change the names to use.*/
/*****************************************************************************/
#define	nuAuStlSeqPlayerSetData(player_no, seq_addr, seq_size)		\
	nuAuStlSeqPlayerDataSet(player_no, seq_addr, seq_size)

#define	nuAuStlSndPlayerSetData(snd_addr, snd_size)			\
	nuAuStlSndPlayerDataSet(snd_addr, snd_size)

#define	nuAuStlPlayerInit(c, size) ((void)0);
	    
#endif  /* defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS) */
#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif
#endif /* _NUALSTL_H__ */
