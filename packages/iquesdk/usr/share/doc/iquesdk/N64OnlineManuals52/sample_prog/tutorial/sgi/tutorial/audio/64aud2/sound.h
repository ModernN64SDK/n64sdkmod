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

#define   SE1_ID                0
#define   SE2_ID                1
#define   SE3_1_ID              2
#define   SE3_2_ID              3

#define   MARK_COUNT            4

/* Parameters changed by user input */
extern s32            effectType, markerNo;
extern s32            engine;
extern s32            effectX, effectZ;
extern s32            markerDone;

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
