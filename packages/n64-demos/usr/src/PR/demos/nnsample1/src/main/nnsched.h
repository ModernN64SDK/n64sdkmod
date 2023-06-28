/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 1

		Copyright (C) 1997, NINTENDO Co., Ltd.

============================================================================*/

#ifndef _NN_SCHED_
#define _NN_SCHED_
#include <ultra64.h>

#define NN_SC_STACKSIZE    0x2000  /* thread stack size */
#define NN_SC_SWAPBUFFER   0x0040  /* frame buffer swap */
#define NN_SC_RETRACE_MSG  1  /* retrace message */
#define NN_SC_DONE_MSG     2  /* task end message */
#define NN_SC_PRE_NMI_MSG  3  /*  NMI message */
#define NN_SC_GTASKEND_MSG 4  /*  task end  message (in the case of a
				          graphic task,  task end only */ 
#define NN_SC_MAX_MESGS    8  /*  message buffer size */

/*  define thread priority */
#define NN_SC_PRI          120  /* scheduler */
#define NN_SC_AUDIO_PRI    110  /* audio */
#define NN_SC_GRAPHICS_PRI 100  /* graphics */

/* define event message */
#define VIDEO_MSG       666  /* retrace message  */
#define RSP_DONE_MSG    667  /* RSP task done */
#define RDP_DONE_MSG    668  /* RDP draw done  */
#define PRE_NMI_MSG     669  /* NMI message */

/*
    For performance check.
    Please define NN_SC_PERF in order to check PCP performance.
 */
#define NN_SC_PERF
#define NN_SC_PERF_NUM    4
#define NN_SC_GTASK_NUM   8   /* graphic task maximum number */
#define NN_SC_AUTASK_NUM   4  /* audio task maximum number */


/*===========================================================================*/
typedef short NNScMsg;  /*  scheduler message type */

typedef struct SCClient_s {  /*  client list structure */
    struct SCClient_s   *next;  /* pointer to next client */
    OSMesgQueue         *msgQ;  /* message sent to client */
} NNScClient;

typedef struct SCTask_s {  /* task structure */
    struct SCTask_s     *next;  /*  Note:  usually stationed at start of structure */
    u32                 state;
    u32			flags;
    void		*framebuffer;  /* for graphic task use */
    OSTask              list;
    OSMesgQueue         *msgQ;
    OSMesg              msg;
} NNScTask;

typedef struct {  /* define scheduler structure */

  /* message */
  NNScMsg   retraceMsg;
  NNScMsg   prenmiMsg;

  /* define task request */
  OSMesgQueue audioRequestMQ;
  OSMesg      audioRequestBuf[NN_SC_MAX_MESGS];
  OSMesgQueue graphicsRequestMQ;
  OSMesg      graphicsRequestBuf[NN_SC_MAX_MESGS];

  /*  define message queue */
  OSMesgQueue retraceMQ;
  OSMesg      retraceMsgBuf[NN_SC_MAX_MESGS];
  OSMesgQueue rspMQ;
  OSMesg      rspMsgBuf[NN_SC_MAX_MESGS];
  OSMesgQueue rdpMQ;
  OSMesg      rdpMsgBuf[NN_SC_MAX_MESGS];

  /* used for waiting for next retrace signal */
  OSMesgQueue waitMQ;
  OSMesg      waitMsgBuf[NN_SC_MAX_MESGS];

  /* define thread */
  OSThread    schedulerThread;	/* main thread */
  OSThread    audioThread;	/* audio */
  OSThread    graphicsThread;	/* graphics */

  /* client list */
  NNScClient  *clientList;

  /* executing graphic task  */
  NNScTask    *curGraphicsTask;
  NNScTask    *curAudioTask;
  NNScTask    *graphicsTaskSuspended;
  

  /* misc. */
  u32         firstTime;  /* flag for control of screen blackout */

} NNSched;


/* performance check structure */
typedef struct {

  u32 gtask_cnt;                        /* graphic task counter  */
  u32 autask_cnt;                       /* audio task counter */
  u64 retrace_time;                     /* graphic creation time */
  u64 gtask_stime[NN_SC_GTASK_NUM];   /* task start time */
  u64 rdp_etime[NN_SC_GTASK_NUM];     /* RDP end time */
  u64 rsp_etime[NN_SC_GTASK_NUM];     /* RSP end time */
  u64 autask_stime[NN_SC_AUTASK_NUM]; /* audio task start time */
  u64 autask_etime[NN_SC_AUTASK_NUM]; /* audio task end time */
  u32 endflag;
} NNScPerf;

  

#ifdef NN_SC_PERF
extern NNScPerf* nnsc_perf_ptr;

#endif /* NN_SC_PERF */

/* function prototype declarations */
extern void            nnScCreateScheduler(NNSched *sc, u8 videoMode, u8 
numFields);
extern void            nnScAddClient(NNSched *sc, NNScClient *, OSMesgQueue *mq);
extern void            nnScRemoveClient(NNSched *sc, NNScClient 
*client);
extern void            nnScEventHandler(NNSched *sc);
extern void            nnScEventBroadcast(NNSched *sc, NNScMsg *msg);
extern void            nnScExecuteAudio(NNSched *sc);
extern void            nnScExecuteGraphics(NNSched *sc);
extern void            nnScWaitTaskReady(NNSched *sc, NNScTask *task);
extern OSMesgQueue *   nnScGetGfxMQ(NNSched *sc);
extern OSMesgQueue *   nnScGetAudioMQ(NNSched *sc);


#endif

