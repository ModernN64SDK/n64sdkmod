/*============================================================================
  nnsched.h
  ============================================================================*/

#ifndef _NN_SCHED_
#define _NN_SCHED_
#include <ultra64.h>

#define NN_SC_STACKSIZE    0x2000  /* Thread stack size */
#define NN_SC_SWAPBUFFER   0x0040  /* Switch frame buffer */
#define NN_SC_RETRACE_MSG  1  /* Retrace message */
#define NN_SC_DONE_MSG     2  /* Task end message */
#define NN_SC_PRE_NMI_MSG  3  /* NMI message */
#define NN_SC_GTASKEND_MSG 4  /* Task end message
				(when task alone ended in graphic task) */
#define NN_SC_MAX_MESGS    8  /* Message buffer size */

/* Definition of thread priority */
#define NN_SC_PRI          120  /* scheduler */
#define NN_SC_AUDIO_PRI    110  /* audio */
#define NN_SC_GRAPHICS_PRI 100  /* graphics */

/* Definition of event message */
#define VIDEO_MSG       666  /* Retrace message */
#define RSP_DONE_MSG    667  /* End of RSP task */
#define RDP_DONE_MSG    668  /* End of RDP rendering */
#define PRE_NMI_MSG     669  /* NMI message */

/*
   For performance check
   When checking  RCP performance, define
   NN_SC_PERF 
 */
#define NN_SC_PERF
#define NN_SC_PERF_NUM    4
#define NN_SC_GTASK_NUM   8   /* Maximum number of graphic task */
#define NN_SC_AUTASK_NUM   4  /* Maximum number of audio task */


/*===========================================================================*/
typedef short NNScMsg;  /* Scheduler message type */

typedef struct SCClient_s {  /* Client list structure */
    struct SCClient_s   *next;  /* Pointer to next client */
    OSMesgQueue         *msgQ;  /* Message to send to client */
} NNScClient;

typedef struct SCTask_s {  /* Task structure */
    struct SCTask_s     *next;  /* Note: always set at the head of structure  */
    u32                 state;
    u32			flags;
    void		*framebuffer;  /* For graphic task */
    OSTask              list;
    OSMesgQueue         *msgQ;
    OSMesg              msg;
} NNScTask;

typedef struct {  /* Definition of scheduler structure */

  /* message */
  NNScMsg   retraceMsg;
  NNScMsg   prenmiMsg;

  /* Definition of task request queue */
  OSMesgQueue audioRequestMQ;
  OSMesg      audioRequestBuf[NN_SC_MAX_MESGS];
  OSMesgQueue graphicsRequestMQ;
  OSMesg      graphicsRequestBuf[NN_SC_MAX_MESGS];

  /* Definition of message queue */
  OSMesgQueue retraceMQ;
  OSMesg      retraceMsgBuf[NN_SC_MAX_MESGS];
  OSMesgQueue rspMQ;
  OSMesg      rspMsgBuf[NN_SC_MAX_MESGS];
  OSMesgQueue rdpMQ;
  OSMesg      rdpMsgBuf[NN_SC_MAX_MESGS];

  /* Use to wait for next retrace signal */
  OSMesgQueue waitMQ;
  OSMesg      waitMsgBuf[NN_SC_MAX_MESGS];

  /* Definition of thread */
  OSThread    schedulerThread;	/* main thread */
  OSThread    audioThread;	/* audio */
  OSThread    graphicsThread;	/* graphics */

  /* client list  */
  NNScClient  *clientList;

  /* graphic task that is running   */
  NNScTask    *curGraphicsTask;
  NNScTask    *curAudioTask;
  NNScTask    *graphicsTaskSuspended;
  

  /* others */
  u32         firstTime;  /* flag to control black out on the screen */

} NNSched;


/* Structure for performance check */
typedef struct {

  u32 gtask_cnt;                        /* graphic task counter */
  u32 autask_cnt;                       /* audio task counter */
  u64 retrace_time;                     /* time to create graphic */
  u64 gtask_stime[NN_SC_GTASK_NUM];   /* time to start task */
  u64 rdp_etime[NN_SC_GTASK_NUM];     /* ending time of RDP  */
  u64 rsp_etime[NN_SC_GTASK_NUM];     /* ending time of RSP */
  u64 autask_stime[NN_SC_AUTASK_NUM]; /* start time of audio task */
  u64 autask_etime[NN_SC_AUTASK_NUM]; /* ending time of audio task */
  u32 endflag;
} NNScPerf;

  

#ifdef NN_SC_PERF
extern NNScPerf* nnsc_perf_ptr;

#endif /* NN_SC_PERF */

/* Declare function prototype */
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
