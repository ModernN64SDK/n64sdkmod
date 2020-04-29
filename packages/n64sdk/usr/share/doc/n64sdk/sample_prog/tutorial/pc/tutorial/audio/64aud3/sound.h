/*******************************************************************
 *
 *  sound.h
 *
 *******************************************************************/

#ifndef   __SOUND_HEADER__
#define   __SOUND_HEADER__

#include    <ultra64.h>

#define   AUDIO_HEAP_SIZE       0x40000

#define   BGM_VOLUME            0x5000

/* Parameters changed by user input */
extern s32            effectNo;

/* Parameters changed during sound synthesis */
extern s32            songChannels, seChannels, totalChannels;

/* Memory related information */
extern s32            totalHeapSize, usedHeapSize, initHeapSize, dataHeapSize;

/* Variable that tells whether it is option mode in stage0. */
extern u8             optionsMode;

/* Audio data pointer */
extern void *         pbank1Buffer;     /* Sample bank A and B */
extern void *         pbank2Buffer;     /* Sample bank C */
extern void *         pbank3Buffer;     /* Sample bank D and E */
extern void *         song1Buffer;      /* Song A and B */
extern void *         song2Buffer;      /* Song C */
extern void *         sebank1Buffer;    /* SE bank D and E */

/* Function initialization for Audio related */
void    InitializeAudio(void);
void    InitializeAudio0(void);
void    InitializeAudio1(void);

/* Function responsible for processing tying menu processing to MUS, etc. */
void    SoundMenu0(s32 index);
void    SoundMenu1(s32 index);

/* Function to return number of SE */
s32     GetNumberOfEffects(void * SEBankPtr);

#endif /* __SOUND_HEADER__ */
