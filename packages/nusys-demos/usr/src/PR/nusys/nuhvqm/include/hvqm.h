/*
 * HVQM-related functions
 *         1999/5/6 Yutaka Murakami
 */
#ifndef _HVQM_H_
#define _HVQM_H_

/* Environment settings (initial settings: can be changed with Makefile) */
#ifndef NO
#define NO 0
#endif /* NO */
#ifndef YES
#define YES 1
#endif /* YES */
/*
 * Size of image data (Depends on contents of hvqm file)
 */
#ifndef HVQM_MAXWIDTH
#define HVQM_MAXWIDTH 320
#endif /* HVQM_MAXWIDTH */
#ifndef HVQM_MAXHEIGHT
#define HVQM_MAXHEIGHT 240
#endif /* HVQM_MAXWIDTH */
/*
 * Size of buffer for original image data (Depends on contents of hvqm file)
 */
#ifndef HVQM_DATASIZE_MAX
#define HVQM_DATASIZE_MAX 30000
#endif /* HVQM_DATASIZE_MAX */
/*
 * Size of buffer for original audio data (Depends on contents of hvqm file)
 */
#ifndef HVQM_AUDIO_DATASIZE_MAX
#define HVQM_AUDIO_DATASIZE_MAX 3500
#endif /* HVQM_AUDIO_DATASIZE_MAX */
/*
 * Size of frame buffer (depends on value in NuSystem)
 */
#ifndef HVQM_CFB_WIDTH
#define HVQM_CFB_WIDTH NU_GFX_INIT_SCREEN_WD
#endif /* HVQM_CFB_WIDHT */
#ifndef HVQM_CFB_HEIGHT
#define HVQM_CFB_HEIGHT NU_GFX_INIT_SCREEN_HT
#endif /* HVQM_CFB_HEIGHT */
/*
 * Resolution for frame buffer (1:16bit, 2:32bit)
 */
#ifndef HVQM_CFB_FORMAT
#define HVQM_CFB_FORMAT 1
#endif /* HVQM_CFB_FORMAT */
/*
 * If decoding for the last frame is delayed
 *     YES:  Enable decoding
 *     NO:   Disable decoding
 */
#ifndef HVQM_LAST_FRAME
#define HVQM_LAST_FRAME YES
#endif /* HVQM_LAST_FRAME */
/*
 * Allow other images be displayed while decoding HVQM (YES/NO)
 *     If allowed, drawing is allowed only outside the HVQM image area.
 *     Note: If YES, the frame buffer needs to be initialized
 *            before starting hvqmVideoMgr.
 */
#ifndef HVQM_PERMIT_OTHER_GRAPHICS
#define HVQM_PERMIT_OTHER_GRAPHICS YES
#endif /* HVQM_PERMIT_OTHER_GRAPHICS */
/*
 * If HVQM_PERMIT_OTHER_GRAPHICS is YES, drawing isn't needed.
 * Frame copies the previous image that was used. Specify
 * whether S2DEX microcode is used for that process. (YES/NO)
 *     YES : Use S2DEX2
 *     NO  : Use F3DEX2
 */
#if HVQM_PERMIT_OTHER_GRAPHICS == YES
#ifndef HVQM_USE_SPRITE_UCODE
#define HVQM_USE_SPRITE_UCODE YES
#endif /* HVQM_PERMIT_OTHER_GRAPHICS */
#endif /* HVQM_USE_SPRITE_UCODE */
/*
 * When switching frame buffers, use the dedicated callback function? (YES/NO)
 *     HVQM_STATUS_WAIT_SWAPBUFFER flag is turned on in hvqmStatus when
 *     the dedicated callback function launches the HVQM task until
 *     the first switching of frame buffers occur.
 */
#ifndef HVQM_SWAPBUFFER_CALLBACK
#define HVQM_SWAPBUFFER_CALLBACK YES
#endif /* HVQM_SWAPBUFFER_CALLBACK */

/* You may not make changes below. */
/*
 * Size of FIFO when RSP is used
 */
#ifndef HVQM_SPFIFO_SIZE
#define HVQM_SPFIFO_SIZE 20000
#endif /* HVQM_SPFIFO_SIZE */
/*
 * PCM specifications
 */
#ifndef HVQM_PCM_ALIGN
#define HVQM_PCM_ALIGN 4	/* 16bit X 2ch */
#endif /* HVQM_PCM_ALIGN */
#ifndef HVQM_PCM_ALIGN_SHIFT
#define HVQM_PCM_ALIGN_SHIFT 2	/* log2(PCM_ALIGN) */
#endif /* HVQM_PCM_ALIGN_SHIFT */
/*
 * Size of PCM buffer
 */
#ifndef HVQM_PCM_BUF_NUM
#define HVQM_PCM_BUF_NUM 3
#endif /* HVQM_PCM_BUF_NUM */
#ifndef HVQM_PCM_SAMPLES_MAX
#define HVQM_PCM_SAMPLES_MAX ((HVQM_AUDIO_DATASIZE_MAX - sizeof(HVQM2Audio)) * HVQM_PCM_ALIGN)
#endif /* HVQM_PCM_SAMPLES_MAX */

/* Thread-related */
#define HVQM_AUDIO_THREAD_ID		30
#define HVQM_AUDIO_THREAD_PRI		71 /* NU_AU_MGR_THREAD_PRI+1 */
#define HVQM_AUDIO_THREAD_STACKSIZE	0x2000 /* NU_AU_STACK_SIZE */

/* Status */
#define HVQM_STATUS_INITIALIZED		0x00000001
#define HVQM_STATUS_VIDEO_STARTED	0x00000002
#define HVQM_STATUS_PLAYING		0x00000004
#define HVQM_STATUS_THREAD_CREATED	0x00000008
#if HVQM_SWAPBUFFER_CALLBACK == YES
#define HVQM_STATUS_WAIT_SWAPBUFFER	0x00000010
#endif /* HVQM_SWAPBUFFER_CALLBACK */
/* Microcode */
#define HVQM_UCODE_HVQM2SP	6
#if HVQM_CFB_FORMAT == 1
#define HVQM_UCODE_HVQM2SP1	HVQM_UCODE_HVQM2SP
#else /* HVQM_CFB_FORMAT */
#define HVQM_UCODE_HVQM2SP2	HVQM_UCODE_HVQM2SP
#endif /* HVQM_CFB_FORMAT */

#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)

#include <nusys.h>
#include <HVQM2File.h>
#include <hvqm2dec.h>
#include <adpcmdec.h>

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif /* _LANGUAGE_C_PLUS_PLUS */

/* hvqm.c */
extern volatile u32 hvqmStatus;	/* Status variable */
extern u32 hvqmOffset;		/* Position of image */
extern u32 hvqmTotalFrame;	/* HVQM data's total number of frames */
extern void *hvqmVideoStream;	/* HVQM data's video playback position */
extern void *hvqmAudioStream;	/* HVQM data's audio playback position */
extern u32 hvqmFrameRate;	/* HVQM data's frame rate per usec */
extern u32 hvqmAudioRate;	/* HVQM data's audio playback frequency */
extern u32 hvqmTotalAudio;	/* HVQM data's total number of audio records */
extern u32 hvqmVideoRemain;	/* HVQM data's number of remaining frames */
extern u32 hvqmAudioRemain;	/* HVQM data's number of remaining frames */

extern void hvqmInit(void);
extern void hvqmStart(void *hvqmData);
extern void hvqmVideoMgr(u32 hvqmFlag);
extern void hvqmDelete(void);

/* hvqmaudio.c */
extern void hvqmAudioInit(void);
extern void hvqmAudioDelete(void);

/* hvqmvideobuf.c */
extern u8 hvqmRecordBody[HVQM_DATASIZE_MAX];

/* hvqmaudiobuf.c */
extern u8 hvqmAudioRecordBody[HVQM_AUDIO_DATASIZE_MAX];

/* hvqmwork.c */
extern u16 hvqmWorkspace[(HVQM_MAXWIDTH/8)*(HVQM_MAXHEIGHT/8)*6];
extern s16 hvqmPcmBuf[HVQM_PCM_BUF_NUM][HVQM_PCM_SAMPLES_MAX];

/* hvqmgfxinit.c */
extern void hvqmGfxInit(void);

#if HVQM_PERMIT_OTHER_GRAPHICS == YES
extern void hvqmCopyFrameBuffer(HVQM2Header *hvqmHeader, u32 hvqmFlag);
#endif /* HVQM_PERMIT_OTHER_GRAPHICS */

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif /* _LANGUAGE_C_PLUS_PLUS */
#endif /* defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS) */
#endif /* _HVQM_H_ */
