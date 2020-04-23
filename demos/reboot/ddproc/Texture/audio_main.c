/*
   audio_main.c
 */

/* pre-process */

#include "hglobals.h"
#include "haudio.h"
#include "haudio_osc.h"

#ifdef __NAUDIO__
#include <PR/n_libaudio.h>
#include <PR/n_libaudio_s_to_n.h>
#endif

/* variables */

static OSThread	audioMainThread;
static u64      audioMainThreadStack[STACKSIZE/sizeof(u64)];
static s32      __active,_seq_num;

static s32             curBuf = 0;
/*  static s16      audioSamples[3] = {0, 0, 0}; */

/* functions */
void    	objectOfaumain(void);

extern OSMesgQueue      audioMsgQ ,atskMsgQ,atskWaitMsgQ;
extern TAudioSegInfo *audio_info;
extern ALInstrument *_inst;

void
aumain_create( s32 seq_num )
{
  osCreateThread(&audioMainThread, ++g_thread_id, (void *)objectOfaumain, 0
				 ,audioMainThreadStack + STACKSIZE/sizeof(u64), AUDIO_THREAD_PRIORITY);
  osStartThread(&audioMainThread);
  _seq_num = seq_num;
  __active = 1;
}

void 
objectOfaumain(void)
{
  static s32             seqNo = 0;

  s32        clcount        ,length;
  s16        *audioOp;

    /*
     * Message queue for PI manager return messages
     */
/*   auplyr_create(); */
/*   ausnd_create(); */

    seqNo = 3;

    while (1){
        if (seqNo >= _seq_num)
            seqNo = 0;

		auplyr_play(seqNo);
		
       do {

/* 		if( ausnd_play(sndNo) ) sndNo++; */
/* 		if( sndNo == _inst->soundCount ) sndNo = 0; */

/*             buf = curAudioBuf % 3; */

            audioOp = audio_getAfbPhysAddr(curBuf);
			length  = audio_getSampleNum(curBuf);
/* 			(s16 *) osVirtualToPhysical(_afb[curBuf].buf); */

            /*
             * Call the frame handler
             */

             _al[curBuf].last =
			   alAudioFrame( _al[curBuf].acmd, &clcount, audioOp, length);

			osSendMesg(&atskMsgQ, (OSMesg)curBuf, OS_MESG_BLOCK);
			osRecvMesg(&atskWaitMsgQ, (OSMesg *)NULL, OS_MESG_BLOCK);

            audma_clean();
            curBuf ^= 1; 

        } while ( auplyr_getState() != AL_STOPPED);

        seqNo++;
    }
/*
#ifdef COMP_SEQ_PLAY    
    alCSPStop(seqp);
#else
    alSeqpStop(seqp);
#endif
*/  

#if __2
    alClose(&g);
#endif
}


s32
aumain_stopThread(void)
{
  if ( __active ) 
	{
	  osStopThread( &audioMainThread );
	  __active = 0;
	  return -1;
	}
  return 0;
}

s32
aumain_startThread(void)
{
  if ( !__active ) 
	{
	  osStartThread( &audioMainThread );
	  __active = 1;
	  return -1;
	}
  return 0;
}


/*
  Discussion
   
  The audio command list (AL) must be created with a higher priority than even the DL.  So it cannot be created in the main thread but must run on a separate thread.  
alAudioFrame is a very deep process.   
 */

