/*
 * audio.c
 *
 *  Output of sound sequences.  Higher priority than graphics.
 *
 */

/* pre-process */

#include "hglobals.h"
#include "haudio.h"

#ifdef __NAUDIO__
#include <PR/n_libaudio.h>
#include <PR/n_libaudio_s_to_n.h>
#endif

/* variables */

static OSThread	audioFlushThread;
static u64      audioFlushThreadStack[STACKSIZE/sizeof(u64)];

OSMesgQueue      audioMsgQ ,atskMsgQ,atskWaitMsgQ;
static OSMesg    audioMsgBuf[1],atskMsgBuf[1],atskWaitMsgBuf[1];

ALHeap audio_heap;                          /* audio heap structure. */
u8     audio_heap_buf[AUDIO_HEAP_SIZE];     /* audio heap buffer.  */
TAudioSegInfo       audioinfodata , *audio_info = &audioinfodata;

TAudioList 			_al[2];
TAudioFrameBuffer	_afb[3];

OSTask  *_atsk;
OSTask_t  atsk_bone =
{
    M_AUDTASK,                  /* type  : audio */
    0x0,                        /* flags : (audio task not use this) */
    NULL ,                      /* ucode_boot       (fill in later) */
	NULL ,						/* ucode_boot_size  (fill in later) */
	NULL ,						/* ucode: type of task processing microcode. */
    SP_UCODE_SIZE,              /* ucode_size: microcode size */
	NULL ,						/* ucode_data: (fill in later) */
    SP_UCODE_DATA_SIZE,         /* ucode_data_size : task ucode data size.. */
    NULL,           			/* dram_stack: audio task doesn't use */
    0,        				/* dram_stack_size: audio task doesn't use */
    NULL,                       /* output_buff : audio task doesn't use */
    0,                          /* output_buff_size : audio task doesn't use */
    NULL,                       /* data_ptr: audio frame (fill in later) */
    0,                          /* data_size : (fill in later) */
    NULL,                       /* yield_data_ptr: (not used here) */
    0                           /* yield_data_size: (not used here) */
};

s32 _samplesLeft = 0;   
s32 audio_surplus_samples = 16;

/* protected functions */

void    	objectOfaudioflush(void);
OSTask *    audio_makeTask(s32 cur);
OSTask *    _atsk_create(void);

/* public functions

   void    	audio_create(TAudioSegInfo *info);
 */

/* implmentations */

void
audio_create(TAudioSegInfo *info)
{
  s32 i,afbsize;

  rsp_stat.first_done = 0;
  bcopy(info,audio_info,sizeof(TAudioSegInfo));
  osCreateMesgQueue(&audioMsgQ, audioMsgBuf, 1);
  osCreateMesgQueue(&atskMsgQ , atskMsgBuf , 1);          /*  1 is the buffer size. */
  osCreateMesgQueue(&atskWaitMsgQ, atskWaitMsgBuf, 1);    /* 1 is the buffer size. */

  /* flush thread start */ 
  osCreateThread(&audioFlushThread, ++g_thread_id, (void *)objectOfaudioflush, 0
				 ,audioFlushThreadStack + STACKSIZE/sizeof(u64), AUDIO_THREAD_PRIORITY + 1 );
  osStartThread(&audioFlushThread);

  /*initialize audio heap */
  alHeapInit(&audio_heap, audio_heap_buf, AUDIO_HEAP_SIZE);

  /* create Acmd buffer */
  FORI(2) _al[i].acmd = (Acmd *)try_heapAlloc( &audio_heap, AL_SIZE_MAX * sizeof(Acmd) ,16 ,"_al[].acmd");

  /* create task struct */
  _atsk = _atsk_create();

  /* dma init */
  audma_init();

  /* create synthethizer */
  afbsize = ausyn_create();

  /* create players */
  auplyr_create(audio_info);
  ausnd_create(audio_info);

  /* create audio frame buffer */
  FORI(2)
	{
	  _afb[i].size    = CEIL16(afbsize);
	  _afb[i].minsize = FLOOR16(afbsize) - 16;
/*       _afb[i].buf = alHeapAlloc(&audio_heap, sizeof(u8), AFB_SIZE * sizeof(s32)); */
      _afb[i].buf = (s16 *)try_heapAlloc(&audio_heap, _afb[i].size * sizeof(s32) * 2 ,16 ,"_afb[].buf");
	}

  /* Acmd creation thread */
  aumain_create( auplyr_loadSeqFile( audio_info->midiseq.handle ) ) ;

}

OSTask *
_atsk_create(void)
{
  OSTask * p;
  p = alHeapAlloc(&audio_heap, sizeof(u8), sizeof(OSTask));
  bcopy(&atsk_bone,p,sizeof(OSTask));

  p->t.ucode_boot = audio_info->bootcode.handle;
  p->t.ucode_boot_size = audio_info->bootcode.size,      
     
#ifdef NAUDIO
  p->t.ucode      = (u64 *) n_aspMainTextStart;
  p->t.ucode_data = (u64 *) n_aspMainDataStart;
#else
  p->t.ucode      = audio_info->audiocode.text;
  p->t.ucode_data = audio_info->audiocode.data;
#endif /* NAUDIO */

  return p;
}

/*
 * Audio output thread. 
 * Rendering directive --> task generation --> RCP --> DMA
 */

s32 audio_current=0;

void
objectOfaudioflush(void)
{
	OSMesgQueue *q;
	OSTask *p, *yldtsk;

	while(1)
	  { 
        osRecvMesg(&atskMsgQ, (OSMesg *)&audio_current, OS_MESG_BLOCK);

		/* task generation */				 // location A
		p = audio_makeTask(audio_current);	/*  */

		while (!osRecvMesg(&audioMsgQ, NULL, OS_MESG_NOBLOCK)); /*  <- Semicolon IS required! */
        osRecvMesg(&audioMsgQ, (OSMesg *)NULL, OS_MESG_BLOCK);

		/*  Examine the remainder of the previous AI task. */
		_samplesLeft = osAiGetLength()>>2;

		/*  Specify the AI output buffer. */
		osAiSetNextBuffer(_afb[audio_current^1].buf, _afb[audio_current^1].sample_num<<2);

		/*  Send retrace timing. */
        osSendMesg(&atskWaitMsgQ, (OSMesg)NULL, OS_MESG_BLOCK);

		/*  If executing a gfx task, yield. */
		rsp_stat.yield_type = 0;
		if( yldtsk = rsp_stat.cur_task )
		  {
			try_assert("audio_core : rsp type conflict" , (yldtsk->t.type == M_GFXTASK));
			osSpTaskYield();
			osRecvMesg( &rspMsgQ, (OSMesg *)NULL, OS_MESG_BLOCK );
			rsp_stat.yield_type = osSpTaskYielded(yldtsk) ? 1:2;
		  }
		
		/* Start rsp processing. */
		rsp_stat.cur_task = p;
		osSpTaskStart( p );

		/* Terminate rsp processing. */
		osRecvMesg( &rspMsgQ, (OSMesg *)NULL, OS_MESG_BLOCK );

		try_assert("audi_core : rsp type conflict 2" , (rsp_stat.cur_task->t.type == M_AUDTASK));
		rsp_stat.cur_task = (OSTask *)NULL;

		/* Resume the gfx task. */

		if( q = rsp_stat.suspendQ )
		  {
			try_assert("audio_core : yeild type conflict" , (rsp_stat.yield_type == 0));
			rsp_stat.suspendQ = (OSMesgQueue *)0;
			osSendMesg( q, (OSMesg)NULL, OS_MESG_BLOCK );

		  }
		
		if( rsp_stat.yield_type == 1 )
		  {
			rsp_stat.cur_task = yldtsk;
			osSpTaskStart( yldtsk );
		  }
		else if( rsp_stat.yield_type == 2 )
		  {
			rsp_stat.cur_task = yldtsk;
			osSendMesg( &rspMsgQ, (OSMesg)NULL, OS_MESG_BLOCK );
		  }

		/* DMA output. */
/*         osAiSetNextBuffer(_afb[audio_current].buf, _afb[audio_current].sample_num<<2); */

		rsp_stat.first_done = 1;
	  }
  }

/*
 * Generating audio tasks
 */

OSTask *
audio_makeTask(s32 cur)
{
    OSTask      *p;
	p = (OSTask *)_atsk;          /* task list pointer */
	p->t.data_ptr  = (u64 *)( _al[cur].acmd );
	p->t.data_size = ( _al[cur].last - _al[cur].acmd ) * sizeof(Acmd);

	try_assert( "datasize over \n", ( p->t.data_size < (AL_SIZE_MAX * sizeof(Acmd)) ) );

	return p; 

}

s16 *
audio_getAfbPhysAddr(s32 cur)
{
  return (s16 *)osVirtualToPhysical(_afb[cur].buf);
}

/*  #define AUDIO_EXTRA_SAMPLES      80 */

s32
audio_getSampleNum(s32 cur)
{
  _afb[cur].sample_num =
	(_afb[cur].size - _samplesLeft + audio_surplus_samples) & ~0xf;
  if (_afb[cur].sample_num < _afb[cur].minsize)
	_afb[cur].sample_num = _afb[cur].minsize;

  return _afb[cur].sample_num;
}

/*
   Discussion:

   The audio library has several buffers that require 16-bit alignment.  So because they don't seem to be divided into separate modules, these are (probably) for alignment by functions.  
   (heapInit and heapAlloc)
   But with GNU C, it is like sufficient to use __attribute__ ((aligned (16))) for all.

   The content of both functions is very simple -- one look at them is enough.
   
   Acmd is the equivalent of Gfx with vi.
   The audio command list is the display list.
   Making the variable names a bit more distinctive from one another would probably make the functions easier to understand.
   If we were to use the heap design, something like vi could also be used . . .
   The plan is in disarray.

   Acmd only executes alAudioFrame(), so it's easy to use.

   The required frame buffer size = extra + (frequency * sizeof(s16) * stereo (2) ) / frames per second (60)

 */


