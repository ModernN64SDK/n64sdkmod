/*******************************************************************
 *
 *  sound.h
 *
 *******************************************************************/

#ifndef   __SOUND_HEADER__
#define   __SOUND_HEADER__

#include    <ultra64.h>

#define   AUDIO_HEAP_SIZE       0x40000

#define   BGM_VOLUME            0x6000

/* Parameters changed by user input */
extern s32            songNo, seNo;
extern u32            fadeFrames;

/* Parameters changed during sound synthesis */
extern s32            songChannels, seChannels, totalChannels;

/* Memory related information */
extern s32            totalHeapSize, usedHeapSize, initHeapSize, dataHeapSize;

/* Audio related initialization function */
void    InitializeAudio(void);

/* Function responsible for processing tying menu processing to MUS, etc. */
void    SoundMenu(s32 index);

/* Function to return number of SE */
s32     GetNumberOfEffects(void);

#endif /* __SOUND_HEADER__ */
