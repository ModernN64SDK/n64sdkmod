/*======================================================================*/
/*		NuSYS 							*/
/*		nusys.h							*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/
/* Ver 1.0	97/10/09	Created  by Kensaku Ohki(SLANP)		*/
/* Ver 1.2	98/07/12	Modified by Kensaku Ohki(SLANP)		*/
/* Ver 2.0	90/01/23	Modified by Kensaku Ohki(SLANP)		*/
/*----------------------------------------------------------------------*/
/* $Id: nusys.h,v 1.30 2000/01/27 03:06:54 ohki Exp $		*/
/*======================================================================*/
#ifndef _NUSYS_H_
#define _NUSYS_H_

#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)

#ifndef F3DEX_GBI
#ifndef F3DEX_GBI_2
#define F3DEX_GBI_2
#endif	/* F3DEX_GBI_2	*/
#endif	/* F3DEX_GBI	*/

#include <ultra64.h>
#include <PR/gs2dex.h>
#endif

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

/*----------------------------------------------------------------------*/    
/*----------------------------------------------------------------------*/
/*	DEFINE								*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
#define NU_VERSION		"2.07"

/*--------------------------------------*/
/* NuSystem spec define			*/
/*--------------------------------------*/
#define NU_SPEC_BOOT_STACK_SIZE	0x2000
#define	NU_SPEC_BOOT_ADDR	0x80025c00
#define NU_SPEC_BOOT_STACK	nuMainStack + NU_SPEC_BOOT_STACK_SIZE

/*--------------------------------------*/
/* Thread ID for DEBUG (To make sure)	*/
/*--------------------------------------*/
#define NU_IDLE_THREAD_ID	1
#define NU_RMON_THREAD_ID	2	/* no use */
#define NU_MAIN_THREAD_ID	3

									
/*--------------------------------------*/
/* NuSYS Thread Priority		*/
/*--------------------------------------*/
#define NU_IDLE_THREAD_PRI	OS_PRIORITY_IDLE	/* Idle thread	*/
#define NU_RMON_THREAD_PRI	OS_PRIORITY_RMON	/* no use	*/
#define NU_MAIN_THREAD_PRI	10			/* main thread	*/
    
/*--------------------------------------*/
/* NUSYS STACK SIZE			*/
/*--------------------------------------*/
#define NU_IDLE_STACK_SIZE	0x2000			/* Idle thread */
#define	NU_RMON_STACK_SIZE	0x2000			/* Rmon thread */
#define NU_MAIN_STACK_SIZE	NU_SPEC_BOOT_STACK_SIZE	/* Main thread */




/*----------------------------------------------------------------------*/
/* SCHEDULER DEFINE							*/
/*----------------------------------------------------------------------*/
#define NU_SC_STACK_SIZE	0x2000		/* Thread stack size */
#define NU_SC_NOSWAPBUFFER	0x0000		/* No switch		*/
#define NU_SC_SWAPBUFFER	0x0001		/* Switching frame buffer*/
#define NU_SC_NORDP		0x0002		/* Don't wait for RDP end*/
#define	NU_SC_UCODE_XBUS	0x0004		/* XBUS Ucode		*/
#define	NU_SC_TASK_YIELDED	(OS_TASK_YIELEDE<<16)
#define	NU_SC_TASK_DP_WAIT	(OS_TASK_DP_WAIT<<16)	/* RDP WAIT	*/
#define	NU_SC_TASK_LODABLE	(OS_TASK_LOADBLE<<16)	/* LOADABLE(miss)*/
#define	NU_SC_TASK_LOADABLE	(OS_TASK_LOADABLE<<16)	/* LOADABLE	*/ 
#define	NU_SC_TASK_SP_ONLY	(OS_TASK_SP_ONLY<<16)	/* SP ONLY	*/

							   
#define NU_SC_RETRACE_MSG	0x0001		/* Retrace message */
#define NU_SC_PRENMI_MSG	0x0002		/* NMI message */
#define NU_SC_SWAPBUFFER_MSG	0x0004		/* Switching frame buffer */
						/* The message*/ 
#define NU_SC_GTASKEND_MSG	0x0008		/* Task end message */
#define NU_SC_MAX_MESGS		8		/* Message buffer size */

#define NU_SC_HANDLER_PRI	120	/* EVENT HANDLER THREAD PRORITY */
#define NU_SC_AUDIO_PRI		110	/* AUDIO DISPATCHER THREAD PRORITY */
#define NU_SC_GRAPHICS_PRI	100	/* GFX DISPATCHER THREAD PRORITY */
#define NU_SC_HANDLER_THREAD_ID	19
#define NU_SC_AUDIO_THREAD_ID	18
#define NU_SC_GRAPHICS_THREAD_ID 17   
#define	NU_SC_PRENMI_YET	0	/* PRE NMI has not occured */
#define	NU_SC_PRENMI_GET	1	/* Flag that received the PRE NMI */
#define	NU_SC_BEFORE_RESET 	2	/* Flag immediately preceding the frame before RESET */
#define	NU_SC_BEFORE_RESET_FRAME 2	/* Number of frames before the RESET that the BEFORE_RESET flag is set */
/*----------------------------------------------------------------------*/
/* GRAFIC THREAD DEFINE							*/
/*----------------------------------------------------------------------*/
#define NU_GFX_THREAD_ID	4	
#define NU_GFX_TASKMGR_THREAD_ID 5
#define NU_GFX_STACK_SIZE	0x2000		/* Thread stack size */
#define	NU_GFX_MESGS		8		/* GFX message queue */
#define NU_GFX_TASKMGR_STACK_SIZE 0x2000	/* Stack size */
#define	NU_GFX_TASKMGR_MESGS	8		/* Task manager queue */
#define NU_GFX_THREAD_PRI	50	/* GFX thread priority	*/
#define NU_GFX_TASKMGR_THREAD_PRI 60	/* Task Manager priority */

/*----------------------------------------------------------------------*/
/* GRAFIC MANEGER DEFINE						*/
/*----------------------------------------------------------------------*/
#ifdef F3DEX_GBI_2
#define	NU_GFX_UCODE_F3DEX2	0		/* F3DEX microcode  */
#define	NU_GFX_UCODE_F3DEX2_NON	1		/* F3DEX.NoN microcode  */
#define	NU_GFX_UCODE_F3DEX2_REJ	2		/* F3DEX.NoN microcode  */
#define	NU_GFX_UCODE_F3DLX2_REJ	3		/* F3DEX.ReJ microcode  */
#define	NU_GFX_UCODE_L3DEX2	4		/* L3DEX microcode  */
#define	NU_GFX_UCODE_S2DEX2	5		/* S2DEX microcode  */
    
#define	NU_GFX_UCODE_F3DEX	0		/* For compatibility	*/
#define	NU_GFX_UCODE_F3DEX_NON	1		/* For compatibility	*/
#define	NU_GFX_UCODE_F3DLX	0		/* For compatibility	*/
#define	NU_GFX_UCODE_F3DLX_NON	1		/* For compatibility	*/
#define	NU_GFX_UCODE_F3DLX_REJ	4		/* For compatibility	*/
#define	NU_GFX_UCODE_F3DLP_REJ	3		/* For compatibility	*/
#define	NU_GFX_UCODE_L3DEX	4		/* For compatibility	*/
#define	NU_GFX_UCODE_S2DEX	5		/* For compatibility	*/
#define	NU_GFX_UCODE_S2DEX_D	5		/* For compatibility	*/

#else    
#define	NU_GFX_UCODE_F3DEX	0		/* F3DEX microcode  */
#define	NU_GFX_UCODE_F3DEX_NON	1		/* F3DEX.NoN microcode  */
#define	NU_GFX_UCODE_F3DLX	2		/* F3DLX microcode  */
#define	NU_GFX_UCODE_F3DLX_NON	3		/* F3DLX.NoN microcode  */
#define	NU_GFX_UCODE_F3DLX_REJ	4		/* F3DEX.ReJ microcode  */
#define	NU_GFX_UCODE_F3DLP_REJ	5		/* F3DLP.Rej microcode  */
#define	NU_GFX_UCODE_L3DEX	6		/* L3DEX microcode  */
#define	NU_GFX_UCODE_S2DEX	7		/* S2DEX microcode  */
#define	NU_GFX_UCODE_S2DEX_D	8		/* S2DEX microcode  */
#endif /* F3DEX_GBI_2 */

    
/*--------------------------------------*/
/* Please set the number of graphic             */
/* task structures so it is larger than         */
/* the size of the Scheduler's message buffer.  */
/* Otherwise, task structures will be available */
/* for use while they are being used.	*/
/*--------------------------------------*/
#define NU_GFX_TASK_NUM		10	/* The number of graphic task structure	*/
#define	NU_GFX_RDP_OUTPUTBUFF_SIZE	0x20000	/* fifo buffer size	*/

/*--------------------------------------*/
/* Default frame buffer.                   */
/* By default, the frame buffer is mapped  */
/* to the highest location in RDRAM.       */
/* Set to 320x240 16bit triple buffer.	*/
/*--------------------------------------*/
#define NU_GFX_FRAMEBUFFER_NUM		3
#define NU_GFX_FRAMEBUFFER_ADDR		(0x80400000-320*240*2*3)
#define NU_GFX_FRAMEBUFFER0_ADDR	(NU_GFX_FRAMEBUFFER_ADDR)
#define NU_GFX_FRAMEBUFFER1_ADDR	(NU_GFX_FRAMEBUFFER0_ADDR+320*240*2)
#define NU_GFX_FRAMEBUFFER2_ADDR	(NU_GFX_FRAMEBUFFER1_ADDR+320*240*2)

#define NU_GFX_INIT_SCREEN_WD		320
#define NU_GFX_INIT_SCREEN_HT		240

/*--------------------------------------*/
/* By default, the Z buffer is mapped to  */
/* the lowest location in memory.	*/
/*--------------------------------------*/    
#define NU_GFX_ZBUFFER_ADDR		0x80000400
					   
#define	NU_GFX_DISPLAY_OFF		0	/* Display OFF */    
#define	NU_GFX_DISPLAY_ON		1	/* Display ON	*/
#define	NU_GFX_DISPLAY_ON_TRIGGER	0x80	/* Trigger	*/

#define NU_GFX_YIELD_BUF_SIZE		(OS_YIELD_DATA_SIZE + 0x10)

/*----------------------------------------------------------------------*/
/* SI MANAGER DEFINE							*/
/*----------------------------------------------------------------------*/
#define	NU_SI_STACK_SIZE		NU_CONT_STACK_SIZE
#define	NU_SI_THREAD_ID			NU_CONT_THREAD_ID
#define NU_SI_THREAD_PRI		NU_CONT_THREAD_PRI
#define NU_SI_MESG_MAX			NU_CONT_MESG_MAX
#define NU_SI_CALLBACK_END		1
#define NU_SI_CALLBACK_CONTINUE		0
#define	NU_SI_SC_MAJOR_NO		0x0000
#define	NU_SI_MSG_MAJOR_NO_MASK		0xff00
#define	NU_SI_MSG_MINOR_NO_MASK		0x00ff

#define	NU_SI_MAJOR_NO_CONT		0x0100
#define	NU_SI_MAJOR_NO_PAK		0x0200
#define	NU_SI_MAJOR_NO_RMB		0x0300
#define	NU_SI_MAJOR_NO_EEPROM		0x0400
#define	NU_SI_MAJOR_NO_GBPAK		0x0500
#define	NU_SI_MAJOR_NO_VRS		0x0600
#define	NU_SI_STOP_MGR_MSG		0x7F00

/*----------------------------------------------------------------------*/
/* CONTROLLER MANEGER DEFINE						*/
/*----------------------------------------------------------------------*/
#define NU_CONT_MAXCONTROLLERS		MAXCONTROLLERS
#define	NU_CONT_STACK_SIZE		0x2000
#define	NU_CONT_MESG_MAX		8
#define NU_CONT_THREAD_ID		6
#define NU_CONT_THREAD_PRI		115
#define	NU_CONT_DATA_UNLOCK		0
#define	NU_CONT_DATA_LOCK		1


/*----------------------------------------------------------------------*/
/* Definitions of messages to the controller manager			*/
/*----------------------------------------------------------------------*/
#define	NU_CONT_MSG_BASE		NU_SI_MAJOR_NO_CONT
#define	NU_CONT_RETRACE_MSG		(NU_CONT_MSG_BASE+0)
#define NU_CONT_READ_MSG		(NU_CONT_MSG_BASE+1)
#define NU_CONT_READ_NW_MSG		(NU_CONT_MSG_BASE+2)
#define NU_CONT_QUERY_MSG		(NU_CONT_MSG_BASE+3)


#define	NU_CONT_PAK_MSG_BASE		NU_SI_MAJOR_NO_PAK
#define	NU_CONT_PAK_RETRACE_MSG		(NU_CONT_PAK_MSG_BASE+0)
#define NU_CONT_PAK_OPEN_MSG		(NU_CONT_PAK_MSG_BASE+1)
#define NU_CONT_PAK_FREE_MSG		(NU_CONT_PAK_MSG_BASE+2)
#define NU_CONT_PAK_FOPEN_MSG		(NU_CONT_PAK_MSG_BASE+3)
#define NU_CONT_PAK_READWRITE_MSG	(NU_CONT_PAK_MSG_BASE+4)
#define NU_CONT_PAK_FILEDELETE_MSG	(NU_CONT_PAK_MSG_BASE+5)
#define NU_CONT_PAK_FILESTATE_MSG	(NU_CONT_PAK_MSG_BASE+6)
#define NU_CONT_PAK_FILENUM_MSG		(NU_CONT_PAK_MSG_BASE+7)
#define NU_CONT_PAK_REPAIRID_MSG	(NU_CONT_PAK_MSG_BASE+8)

#define NU_CONT_PAK_MODE_NOCREATE	0
#define NU_CONT_PAK_MODE_CREATE		1
#define	NU_CONT_PAK_TYPE_NONE		0
#define	NU_CONT_PAK_TYPE_PAK		1	/* Controller pak	*/
#define	NU_CONT_PAK_TYPE_RUMBLE		2	/* Rumble pak 		*/
#define	NU_CONT_PAK_TYPE_GBPAK		3	/* 64GB pak		*/
#define NU_CONT_PAK_READ		PFS_READ
#define	NU_CONT_PAK_WRITE		PFS_WRITE

/*----------------------------------------------------------------------*/
/* Rumble Pak control 							*/
/*----------------------------------------------------------------------*/
#define NU_CONT_RMB_STATE_STOP		0x00
#define NU_CONT_RMB_STATE_STOPPING	0x01
#define NU_CONT_RMB_STATE_STOPPED	0x02
#define NU_CONT_RMB_STATE_RUN		0x03
#define NU_CONT_RMB_STATE_FORCESTOP	0x04
#define NU_CONT_RMB_MODE_DISABLE	0x00
#define	NU_CONT_RMB_MODE_ENABLE		0x01
#define NU_CONT_RMB_MODE_AUTORUN	0x02
#define NU_CONT_RMB_MODE_PAUSE		0x80
#define NU_CONT_RMB_AUTO_SEARCH		0x00
#define	NU_CONT_RMB_AUTO_FIND		0x01
#define NU_CONT_RMB_AUTO_SEARCHTIME	(60*5)
#define NU_CONT_RMB_MSG_BASE		NU_SI_MAJOR_NO_RMB
#define NU_CONT_RMB_RETRACE_MSG		(NU_CONT_RMB_MSG_BASE+0)
#define NU_CONT_RMB_CHECK_MSG		(NU_CONT_RMB_MSG_BASE+1)
#define NU_CONT_RMB_START_MSG		(NU_CONT_RMB_MSG_BASE+2)
#define NU_CONT_RMB_STOP_MSG		(NU_CONT_RMB_MSG_BASE+3)	   
#define	NU_CONT_RMB_FORCESTOP_MSG	(NU_CONT_RMB_MSG_BASE+4)
#define	NU_CONT_RMB_FORCESTOPEND_MSG	(NU_CONT_RMB_MSG_BASE+5)

/*----------------------------------------------------------------------*/
/* EEPROM								*/
/*----------------------------------------------------------------------*/
#define NU_EEPROM_MSG_BASE		NU_SI_MAJOR_NO_EEPROM
#define NU_EEPROM_RETRACE_MSG		(NU_EEPROM_MSG_BASE+0)
#define NU_EEPROM_CHECK_MSG		(NU_EEPROM_MSG_BASE+1)
#define	NU_EEPROM_READ_MSG		(NU_EEPROM_MSG_BASE+2)
#define NU_EEPROM_WRITE_MSG		(NU_EEPROM_MSG_BASE+3)


/*----------------------------------------------------------------------*/
/* 64GB Pak Manager							*/
/*----------------------------------------------------------------------*/
#define NU_CONT_GBPAK_MSG_BASE		NU_SI_MAJOR_NO_GBPAK
#define NU_CONT_GBPAK_RETRACE_MSG	(NU_CONT_GBPAK_MSG_BASE+0)
#define NU_CONT_GBPAK_OPEN_MSG		(NU_CONT_GBPAK_MSG_BASE+1)
#define NU_CONT_GBPAK_STATUS_MSG	(NU_CONT_GBPAK_MSG_BASE+2)
#define NU_CONT_GBPAK_POWER_MSG		(NU_CONT_GBPAK_MSG_BASE+3)
#define NU_CONT_GBPAK_READID_MSG	(NU_CONT_GBPAK_MSG_BASE+4)
#define NU_CONT_GBPAK_READWRITE_MSG	(NU_CONT_GBPAK_MSG_BASE+5)
#define NU_CONT_GBPAK_CHECKCONNECTOR_MSG (NU_CONT_GBPAK_MSG_BASE+6)
    
#define NU_CONT_GBPAK_POWER_OFF		OS_GBPAK_POWER_OFF
#define NU_CONT_GBPAK_POWER_ON		OS_GBPAK_POWER_ON
#define	NU_CONT_GBPAK_MBC_RAM_ENABLE_CODE	0x0a
#define	NU_CONT_GBPAK_MBC_RAM_DISABLE_CODE	0x00
#define	NU_CONT_GBPAK_MBC_RAM_REG0_ADDR		0x0000

#define NU_CONT_GBPAK_MBC_REG0_ADDR		0x0000	/* Register 0	*/
#define NU_CONT_GBPAK_MBC_REG1_ADDR		0x2100	/* Register 1	*/
#define NU_CONT_GBPAK_MBC_REG2_ADDR		0x4000	/* Register 2	*/
#define NU_CONT_GBPAK_MBC_REG3_ADDR		0x6000	/* Register 3	*/

/*----------------------------------------------------------------------*/
/* Voice Recognition System Manager					*/
/*----------------------------------------------------------------------*/
#define NU_VRS_MSG_BASE			NU_SI_MAJOR_NO_VRS
#define	NU_VRS_RETRACE_MSG		(NU_VRS_MSG_BASE+0)
#define	NU_VRS_OPEN_MSG			(NU_VRS_MSG_BASE+1)
#define	NU_VRS_CLEAR_DICTIONARY_MSG	(NU_VRS_MSG_BASE+2)
#define	NU_VRS_SET_WORD_MSG		(NU_VRS_MSG_BASE+3)
#define	NU_VRS_MASK_DICTIONARY_MSG	(NU_VRS_MSG_BASE+4)
#define	NU_VRS_START_READ_DATA_MSG	(NU_VRS_MSG_BASE+5)
#define	NU_VRS_GET_READ_DATA_MSG	(NU_VRS_MSG_BASE+6)
#define	NU_VRS_STOP_READ_DATA_MSG	(NU_VRS_MSG_BASE+7)
#define	NU_VRS_CONTROL_GAIN_MSG		(NU_VRS_MSG_BASE+8)
					   
/*----------------------------------------------------------------------*/
/* PI DEFINE								*/
/*----------------------------------------------------------------------*/
#define	NU_PI_MESG_NUM			50	/* PI Message buffer size */
#define	NU_PI_CART_BLOCK_READ_SIZE	0x4000	/* cart read block size	*/

/*----------------------------------------------------------------------*/
/* DEBUG 								*/
/*----------------------------------------------------------------------*/
#define NU_DEB_PERF_BUF_NUM		3
#define	NU_DEB_PERF_GFXTASK_CNT		8	/* Graphic task count number */
#define	NU_DEB_PERF_AUTASK_CNT		4	/* Audio task count number */
#define NU_DEB_PERF_RUNNING		0	/* Sampling		*/
#define NU_DEB_PERF_STOP		1	/* Stop sampling	*/
#define NU_DEB_PERF_START		2	/* Start sampling	*/
#define NU_DEB_DP_CLOCK_CTR		0	/* RDP internal counter	*/
#define	NU_DEB_DP_CMD_CTR		1	/* CMD counter		*/
#define NU_DEB_DP_PIPE_CTR		2	/* PIPE counter		*/
#define	NU_DEB_DP_TMEM_CTR		3	/* TMEM counter		*/
#define NU_DEB_BAR_FRAME1		0	/* Maxium display number for */
#define NU_DEB_BAR_FRAME2		1	/* performance bar	*/
#define NU_DEB_BAR_FRAME3		2
#define NU_DEB_BAR_FRAME4		3
#define NU_DEB_BAR_FRAME5		4
#define NU_DEB_BAR_FRAME6		5
#define NU_DEB_BAR_FRAME7		6
#define	NU_DEB_CON_ROW_MAX		40	/* Number of console rows */
#define NU_DEB_CON_COLUMN_MAX		30 	/* Number of console columns */
#define NU_DEB_CON_TEXT_SIZE		(NU_DEB_CON_ROW_MAX*NU_DEB_CON_COLUMN_MAX)
#define NU_DEB_CON_WIDTH		320
#define NU_DEB_CON_WINDOW_NUM		4
#define NU_DEB_CON_WINDOW0		0
#define NU_DEB_CON_WINDOW1		1
#define NU_DEB_CON_WINDOW2		2
#define NU_DEB_CON_WINDOW3		3
#define	NU_DEB_CON_TEXT_BLACK		0	/* Black */
#define NU_DEB_CON_TEXT_BLUE		1	/* Blue */
#define NU_DEB_CON_TEXT_GREEN		2	/* Green */
#define NU_DEB_CON_TEXT_CYAN		3	/* Cyan */
#define NU_DEB_CON_TEXT_RED		4	/* Red */
#define NU_DEB_CON_TEXT_MAGENTA		5	/* Magenta */
#define NU_DEB_CON_TEXT_YELLOW		6	/* Yellow */
#define NU_DEB_CON_TEXT_WHITE		7	/* White */
#define	NU_DEB_CON_TEXT_LIGHTBLACK	8	/* Gray */
#define NU_DEB_CON_TEXT_LIGHTBLUE	9	/* Light blue */
#define NU_DEB_CON_TEXT_LIGTHBLUE	9	/* Light blue for spell miss*/
#define NU_DEB_CON_TEXT_LIGHTGREEN	10	/* Light green */
#define NU_DEB_CON_TEXT_LIGHTCYAN	11	/* Light cyan */
#define NU_DEB_CON_TEXT_LIGHTRED	12	/* Light red */
#define NU_DEB_CON_TEXT_LIGHTMAGENTA	13	/* Light magenta */
#define NU_DEB_CON_TEXT_LIGHTYELLOW	14	/* Light yellow */
#define NU_DEB_CON_TEXT_LIGHTWHITE	15	/* White */
#define NU_DEB_CON_ATTR_NORMAL		0x0	/* No attribute */
#define NU_DEB_CON_ATTR_BLINK		0x1	/* Blink	*/
#define	NU_DEB_CON_ATTR_REVERSE		0x2	/* Reversed character*/
#define NU_DEB_CON_SCROLL_ON		1
#define NU_DEB_CON_SCROLL_OFF		0
#define NU_DEB_CON_WINDOW_OFF		0
#define NU_DEB_CON_WINDOW_ON		1    
    
#define NU_DEB_MARKER_NUM		10

#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/* structer define 							*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*--------------------------------------*/
/* Scheduler sturcter			*/
/*--------------------------------------*/
typedef short	NUScMsg;		/* Scheduler message type */

typedef struct	st_SCClient {		/* Client list structure */
    struct st_SCClient*	next;		/* Pointer to the next client */
    OSMesgQueue*	msgQ;		/* Message sent to the client */
    NUScMsg		msgType;	/* Message type		*/
					/* NU_SC_RETRACE_MSG or	*/
    					/* NU_SC_PRENMI_MSG		*/
} NUScClient;

typedef struct st_SCTask {		/* Task structure */
    struct st_SCTask	*next;
    u32		state;
    u32		flags;
    void	*framebuffer;		/* For the graphic task */
    OSTask	list;
    OSMesgQueue	*msgQ;
    OSMesg	msg;
} NUScTask;

typedef struct st_Sched { /* Define the scheduler structure */

    /* Message */
    NUScMsg	retraceMsg;
    NUScMsg	prenmiMsg;

    /* Define the task request queue */
    OSMesgQueue	audioRequestMQ;
    OSMesg	audioRequestBuf[NU_SC_MAX_MESGS];
    OSMesgQueue graphicsRequestMQ;
    OSMesg	graphicsRequestBuf[NU_SC_MAX_MESGS];

    /* Define the message queue */
    OSMesgQueue	retraceMQ;
    OSMesg	retraceMsgBuf[NU_SC_MAX_MESGS];
    OSMesgQueue	rspMQ;
    OSMesg      rspMsgBuf[NU_SC_MAX_MESGS];
    OSMesgQueue rdpMQ;
    OSMesg	rdpMsgBuf[NU_SC_MAX_MESGS];

    /* Use for waiting for the next retrace-signal */
    OSMesgQueue	waitMQ;
    OSMesg	waitMsgBuf[NU_SC_MAX_MESGS];

    /* Define threads */
    OSThread	schedulerThread;	/* Main thread */
    OSThread	audioThread;		/* Audio */
    OSThread	graphicsThread;		/* Graphics */

    /* Client list */
    NUScClient	*clientList;

    /* Executing graphics task  */
    NUScTask	*curGraphicsTask;
    NUScTask 	*curAudioTask;
    NUScTask	*graphicsTaskSuspended;

    u32		retraceCount;
    u8		frameRate;
    u8		frameBufferNum;		/* The number of frame buffers */


} NUSched;


/*--------------------------------------*/
/* ucode sturcter			*/
/*--------------------------------------*/
typedef struct  st_Ucode {
    u64*	ucode;	
    u64*	ucode_data;
} NUUcode;

/*--------------------------------------*/
/* CALL BACK Function	typedef		*/
/*--------------------------------------*/
typedef void (*NUIdleFunc)(void);	/* Idle callback function pointer */
typedef void (*NUScPreNMIFunc)(void);	/* PRENMI callback function pointer */
typedef void (*NUGfxFunc)(u32);		/* PRENMI callback function pointer */
typedef void (*NUGfxPreNMIFunc)(void);	/* GFX PRENMI callback function pointer */

typedef void (*NUGfxSwapCfbFunc)(void*);/* SWAPBUF callback function pointer*/
typedef void (*NUGfxTaskEndFunc)(void*);/* TASKEND callback function pointer*/
typedef void (*NUContReadFunc)(u32);	/* Callback function pointer	*/
					/* for Controller read end	*/
typedef void (*NUContPakFunc)(void*);	/* Callback the Controller Pak control function */
typedef void (*NUContRmbFunc)(void*);	/* Callback the Rumble Pak control function */
typedef s32 (*NUCallBackFunc)(void*);	/* Callback function */



/*--------------------------------------*/
/* PI Common Message 			*/
/*--------------------------------------*/
typedef struct st_PiOverlaySegment {
    u8* romStart;	/* The starting offset of ROM for the segment*/
    u8* romEnd;		/* The ending offset of ROM for the segment */
    u8*	ramStart;	/* The starting offset of CPU for the segment */    
    u8* textStart;	/* The starting offset of DRAM with text attribute */
    u8* textEnd;	/* The ending offset of DRAM with text attribute */
    u8* dataStart;	/* The starting offset of DRAM with data attribute */
    u8* dataEnd;	/* The ending offset of DRAM with data attribute */
    u8* bssStart;	/* The starting offset of DRAM with bss attribute */
    u8*	bssEnd;		/* The ending offset of DRAM with bss attribute */
} NUPiOverlaySegment;

/*--------------------------------------*/
/* SI Common Message 			*/
/*--------------------------------------*/
typedef struct st_SiCommonMesg {
    NUScMsg		mesg;
    OSMesgQueue*	rtnMesgQ;
    s32			error;
    void*		dataPtr;
} NUSiCommonMesg;

/*--------------------------------------*/
/* controller typedef			*/
/*--------------------------------------*/
typedef struct st_ContRead {
    NUScMsg	mesg;			/* Message type */
    OSMesgQueue* rtnMesgQ;
    OSContPad*	pad;
} NUContReadMesg;

typedef struct st_ContData {
    u16	button;			/* A,B,C,Z,L,R,START,PAD ,button 	*/
    s8	stick_x;		/* Actual used range -61 <= stick X <= 61*/
    s8	stick_y;		/* Actual used range -63 <= stick Y <= 63*/
    u8	errno;			/* Error				*/
    u16	trigger;		/* Button's trigger data		*/
} NUContData;

typedef struct st_ContQuery {
    NUScMsg	mesg;
    OSMesgQueue* rtnMesgQ;
} NUContQueryMesg;


typedef struct st_ContPakFile {
    OSPfs	*pfs;    
    s32		file_no;
    s32		error;
    u32		type;
} NUContPakFile;


typedef struct st_ContPakFreeMesg {
    NUContPakFile*	file;
    s32			size;
} NUContPakFreeMesg;

typedef struct st_ContPakFileOpenMesg {
    NUContPakFile*	file;
    u8*			noteName;
    u8*			extName;
    s32			size;
    s32			mode;
} NUContPakFileOpenMesg;

typedef struct st_ContPakFileRWMesg {
    NUContPakFile*	file;
    s32			offset;
    s32			size;
    u8			mode;
    u8*			buf;
} NUContPakFileRWMesg;

typedef struct st_ContPakFileStateMesg {
    NUContPakFile*	file;
    OSPfsState*		state;
} NUContPakFileStateMesg;

typedef struct st_ContPakFileNumMesg {
    NUContPakFile*	file;
    s32*		max_files;
    s32*		used_files;
} NUContPakFileNumMesg;

typedef struct st_ContPakIsPlugMesg {
    NUScMsg		mesg;
    OSMesgQueue*	rtnMesgQ;
    s32			error;
    u8			bitPattern;
} NUContPakIsPlugMesg;

typedef struct st_ContRmbCtl {
    u16			freq;		/* Rumble frequency */
    u16			frame;
    u16			counter;
    u8			state;
    u8			mode;
    u8			autorun;
    u8			type;
} NUContRmbCtl;

typedef struct st_ContRmbMesg {
    u8			contNo;
    s32			error;
    void*		data;
} NUContRmbMesg;

typedef struct st_CallBackList {
    struct st_CallBackList* 	next;
    s32 			(**func)(NUSiCommonMesg*);
    u16				majorNo;
    u8    			funcNum;
} NUCallBackList;

typedef struct st_EepromMesg {
    void*		dataPtr;
    s32			error;
    u8			address;
    u16			dataLen;
} NUEepromMesg;

typedef struct st_ContGBPakMesg {
    NUContPakFile*	handle;
    s32			data[4];
} NUContGBPakMesg;

typedef OSVoiceHandle NUVrsHandle;

typedef OSVoiceData NUVrsData;

typedef struct stVrsMesg {
    NUVrsHandle*	vrsHandle;
    s32			data[2];
} NUVrsMesg;

/*--------------------------------------*/
/* Debug				*/
/*--------------------------------------*/
typedef struct st_GfxTaskTime {
    s64		rspStart;		/* RSP task start time*/
    s64		rspEnd;			/* RSP task end time*/
    s64		rdpEnd;			/* RDP end time		*/
    u32		dpCnt[4];		/* RDP internal counter	*/
} NUDebTaskTime;

typedef struct st_DebTaskPerf {
    s64		retraceTime;		/* Retrace event time*/
    s64		markerTime[NU_DEB_MARKER_NUM];
    u8		gfxTaskCnt;		/* The number of graphic tasks */
    u8		auTaskCnt;		/* The number of audio tasks */
    u8		gfxTaskStart;
    NUDebTaskTime	gfxTaskTime[NU_DEB_PERF_GFXTASK_CNT];
    NUDebTaskTime	auTaskTime[NU_DEB_PERF_AUTASK_CNT];
} NUDebTaskPerf;

/* Console window structure */
typedef struct st_DebConWindow {
    u8	windowFlag;	/* Console window show/hide flag*/
    u16	scroll;		/* Scroll value 			*/
    u8	scrollFlag;	/* Scroll enable/disable 		*/
    u8	textColor;	/* Text color 				*/
    u8	attribute;	/* Attribute				*/
    u16	posX;		/* Row position for writing text	*/
    u16	posY;		/* Column position for writing text	*/
    u16	index;		/* Position for writing text buffer	*/
    u16	winX;		/* X coordinate for console window display*/
    u16	winY;		/* Y coordinate for console window display*/
    u16	winW;		/* Number of rows in console display	*/
    u16	winH;		/* Number of columns in console display	*/
    u16	text[NU_DEB_CON_TEXT_SIZE];	/*Text buffer */    
} NUDebConWindow;

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/* extern variables 							*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/    

extern u8	nuRDPOutputBuf[];
extern u8	nuDramStack[];
extern u8	nuYieldBuf[];
extern NUSched	nusched;		/* Scheduler structure */
extern OSMesgQueue nuGfxMesgQ;		/* graphic thread  queue */
extern u32	nuScRetraceCounter;    /* Retrace counter */
extern u8	nuScPreNMIFlag;
/*--------------------------------------*/
/*  graphics variables 			*/
/*--------------------------------------*/
extern NUUcode*		nuGfxUcode;	/* Pointer to GFX microcode structure*/
extern u16**		nuGfxCfb;/* Pointer to the frame buffer pointer array*/
extern u16*		nuGfxCfb_ptr;	/* Frame buffer pointer */
extern u32		nuGfxCfbNum;		/* The number of frame buffers*/
extern u16*		nuGfxZBuffer;		/* Z buffer pointer*/
extern volatile u32	nuGfxTaskSpool;		/* Number of tasks in the queue */
extern u32		nuGfxDisplay;		/* Screen display ON/OFF flag*/
extern u32		nuGfxCfbCounter;	/* For switching fame buffers*/
extern OSMesgQueue	nuGfxMesgQ;
extern OSThread		nuGfxThread;			/* graphic thread */
extern s32		nuGfxUcodeFifoSize; 	/* FIFO buffer size -1: Size undetermined */
extern u64*		nuGfxUcodeFifoPtr;	/* Pointer to FIFO buffer */

/*--------------------------------------*/
/*  controller manager variables 	*/
/*--------------------------------------*/
extern OSContStatus	nuContStatus[];
extern OSContPad	nuContData[];
extern u32		nuContNum;	/* The number of connected Controllers */
extern u32		nuContDataLockKey; /* Lock the Controller data */
extern OSMesgQueue	nuContWaitMesgQ;/* Wait for Controller read */
extern OSPfs		nuContPfs[];
extern NUCallBackList	nuContCallBack;
extern u16		nuContPakCompanyCode;	/* Company code */
extern u32		nuContPakGameCode;	/* Game code */
extern NUCallBackList	nuContPakCallBack;

/*--------------------------------------*/
/*  RUMBUL manager variables 		*/
/*--------------------------------------*/
extern NUContRmbCtl	nuContRmbCtl[];
extern u32		nuContRmbSearchTime;
extern NUCallBackList	nuContRmbCallBack;


/*--------------------------------------*/
/*  EEPROM manager variables 		*/
/*--------------------------------------*/
extern NUCallBackList	nuEepromCallBack;

/*--------------------------------------*/
/*  64GBPak  manager variables 		*/
/*--------------------------------------*/
extern NUCallBackList	nuContGBPakCallBack;

/*--------------------------------------*/
/*  VRS  manager variables 		*/
/*--------------------------------------*/
extern NUCallBackList	nuVrsCallBack;
					   
/*--------------------------------------*/
/*  si variables 			*/
/*--------------------------------------*/
extern OSMesgQueue	nuSiMesgQ;	/* SI event message queue*/
extern OSMesgQueue	nuSiMgrMesgQ;	/* SI manager queue*/
extern NUCallBackList*	nuSiCallBackList;	/* Callback function list */

/*--------------------------------------*/
/*  pi variables 			*/
/*--------------------------------------*/
extern OSPiHandle*	nuPiCartHandle;
extern OSPiHandle*	nuPiSramHandle;
extern OSPiHandle*	nuPiDDRomHandle;

/*--------------------------------------*/
/* CALL BACK Function pointer 		*/
/*--------------------------------------*/
extern NUIdleFunc	nuIdleFunc;		/* callback func pointer */
extern NUScPreNMIFunc	nuScPreNMIFunc;		/* PRE NMI callback pointer  */
extern NUGfxSwapCfbFunc	nuGfxSwapCfbFunc; 	/* swapbuf callback pointer  */
extern NUGfxFunc	nuGfxFunc;		/* callback pointer 	*/
extern NUGfxPreNMIFunc	nuGfxPreNMIFunc;	/* callback pointer 	*/
extern NUGfxTaskEndFunc	nuGfxTaskEndFunc; 	/* task end callback  ptr */
extern NUContReadFunc	nuContReadFunc;		/* Callback function when*/
						/* Controller read ends	*/
/*--------------------------------------*/
/* Debug 		 		*/
/*--------------------------------------*/

extern NUDebTaskPerf*	nuDebTaskPerfPtr;
extern NUDebConWindow	nuDebConWin[];
extern NUDebTaskPerf	nuDebTaskPerf[];
extern u32		nuDebTaskPerfInterval;
extern volatile u32	nuDebTaskPerfCnt;
extern volatile u32	nuDebTaskPerfEnd;


/*----------------------------------------------------------------------*/    
/*----------------------------------------------------------------------*/
/* Global Function 							*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*--------------------------------------*/
/* Boot function			*/
/*--------------------------------------*/
extern void nuIdleFuncSet(void (*func)(void));
extern void nuPreNMIFuncSet(NUScPreNMIFunc func);
/*--------------------------------------*/
/* Scheduler function			*/
/*--------------------------------------*/
extern void nuScCreateScheduler(u8 videoMode, u8 numFields);
extern void nuScAddClient(NUScClient *c, OSMesgQueue *mq, NUScMsg msgType);
extern void nuScRemoveClient(NUScClient *client);
extern void nuScResetClientMesgType(NUScClient* client, NUScMsg msgType);
extern OSMesgQueue* nuScGetGfxMQ(void);
extern OSMesgQueue* nuScGetAudioMQ(void);
extern void nuScSetFrameBufferNum(u8 frameBufferNum);
extern s32 nuScGetFrameRate(void);
					   
/*--------------------------------------*/
/* graphic(GFX) manager function	*/
/*--------------------------------------*/
extern void nuGfxInit(void);
extern void nuGfxInitEX2(void);
extern void nuGfxThreadStart(void);
extern void nuGfxTaskMgrInit(void);
extern void nuGfxTaskStart(Gfx *gfxList_ptr, u32 gfxListSize, u32 ucode, u32 flag);
extern void nuGfxFuncSet(NUGfxFunc func);
extern void nuGfxPreNMIFuncSet(NUGfxPreNMIFunc func);
extern void nuGfxSwapCfbFuncSet(NUGfxSwapCfbFunc func);
extern void nuGfxSetCfb(u16** framebuf, u32 framebufnum);
extern void nuGfxSwapCfb(void* framebuffer);

extern void nuGfxTaskEndFuncSet(NUGfxTaskEndFunc func);
extern void nuGfxTaskAllEndWait(void);
extern void nuGfxRetraceWait(u32 retrace_num);
extern void nuGfxDisplayOff(void);
extern void nuGfxDisplayOn(void);
extern void nuGfxSetUcodeFifo(void* fifoBufPtr, s32 size);
#ifdef F3DEX_GBI_2
#define	nuGfxInit()	nuGfxInitEX2()
#endif /* F3DEX_GBI_2 */
/*--------------------------------------*/
/* controller manager function		*/
/*--------------------------------------*/
extern u8   nuContInit(void);
extern s32  nuContMgr(void* mesg);
extern u8   nuContMgrInit(void);
extern void nuContMgrRemove(void);
extern void nuContDataLock(void);
extern void nuContDataUnLock(void);
extern void nuContDataRead(OSContPad* pad);
extern void nuContDataReadWait(void);
extern void nuContDataReadStart(void);
extern void nuContDataGet(OSContPad* contdata, u32 padno);
extern void nuContDataGetAll(OSContPad* contdata);
extern void nuContDataGetEx(NUContData *contdata, u32 padno);
extern void nuContDataGetExAll(NUContData *contdata);
extern void nuContReadFuncSet(NUContReadFunc func);
extern void nuContQueryRead(void);
extern void nuContDataClose(void);
extern void nuContDataOpen(void);
    
/*--------------------------------------*/
/* controller pak function		*/
/*--------------------------------------*/
extern s32  nuContPakMgr(void* mesg_type);
extern void nuContPakMgrInit(void);
extern void nuContPakMgrRemove(void);
extern void nuContPakJisToN64(u8* src, u8* dest, u32 len);
extern void nuContPakN64ToJis(u8* src, u8* dest, u32 len);
extern s32  nuContPakOpen(NUContPakFile *file, u32 contNo);
extern s32  nuContPakGetFree(NUContPakFile *file);
extern void nuContPakCodeSet(u8* companyCode, u8* gameCode);
extern s32  nuContPakRepairId(NUContPakFile *file);
extern s32  nuContPakFileOpen(NUContPakFile *file,
			      u8* noteName,u8* extName, u32 mode, s32 size);
extern s32  nuContPakFileOpenJis(NUContPakFile *file,
				 u8* noteName,u8* extName, u32 mode, s32 size);
extern s32  nuContPakFileReadWrite(NUContPakFile *file,
			    s32 offset, s32 size,u8* buf, u32 mode);
extern s32  nuContPakFileDelete(NUContPakFile *file, u8* noteName, u8* extName);
extern s32  nuContPakFileDeleteJis(NUContPakFile *file, u8* noteName, u8* extName);
extern s32  nuContPakFileState(NUContPakFile *file, OSPfsState* state);
extern s32  nuContPakFileNum(NUContPakFile *file,
			     s32 *max_files, s32 *used_files);
extern s32 nuContPakFileFread(NUContPakFile *file, s32 offset, s32 size, u8* buf);

/*--------------------------------------*/
/* rumble pak function			*/
/*--------------------------------------*/
extern void nuContRmbMgrInit(void);
extern s32  nuContRmbMgr(void* mesg);
extern void nuContRmbMgrRemove(void);
extern void nuContRmbStart(u32 contNo, u16 freq, u16 frame);
extern void nuContRmbStop(u32 contNo);
extern s32  nuContRmbCheck(u32 contNo);
extern void nuContRmbModeSet(u32 contNo, u8 mode);
extern void nuContRmbForceStop(void);
extern void nuContRmbForceStopEnd(void);
extern void nuContRmbSearchTimeSet(u32 frame);

/*--------------------------------------*/
/* 64GB Pak function			*/
/*--------------------------------------*/
extern s32 nuContGBPakMgr(void* mesg);
extern void nuContGBPakMgrInit(void);
extern void nuContGBPakMgrRemove(void);
extern s32 nuContGBPakOpen(NUContPakFile* handle, s32 contNo);
extern s32 nuContGBPakGetStatus(NUContPakFile* handle, u8* status);
extern s32 nuContGBPakReadID(NUContPakFile* handle, OSGbpakId* id, u8* status);
extern s32 nuContGBPakReadWrite(NUContPakFile* handle, u16 flag , u16 address, u8* buffer, u16 size);
extern s32 nuContGBPakPower(NUContPakFile* handle, s32 flag);
extern s32 nuContGBPakCheckConnector(NUContPakFile* handle, u8* status);
extern s32 nuContGBPakFread(NUContPakFile* handle, u16 address, u8* buffer, u16 size);

extern s32 nuContGBPakFwrite(NUContPakFile* handle, u16 address, u8* buffer, u16 size);
extern s32 nuContGBPakRegWrite(NUContPakFile* handle, u16 addr, u8 data);

/*--------------------------------------*/
/* eeprom function			*/
/*--------------------------------------*/
extern void nuEepromMgrInit(void);
extern s32  nuEepromMgr(void* mesg);
extern s32  nuEepromCheck(void);
extern void nuEepromMgrRemove(void);
extern s32 nuEepromRead(u8 address, u8* buffer,u16 len);
extern s32 nuEepromWrite(u8 address, u8* buffer,u16 len);

    
/*--------------------------------------*/
/* pi function				*/
/*--------------------------------------*/
extern void nuPiInit(void);
extern void nuPiReadRom(u32 rom_addr, void* buf_ptr, u32 size);
extern void nuPiInitSram(void);
extern void nuPiInitDDrom(void);
extern void nuPiReadWriteSram(u32 addr, void* buf_ptr, u32 size, s32 flag);
extern void nuPiReadRomOverlay(NUPiOverlaySegment* segment);

/*--------------------------------------*/
/* si function				*/
/*--------------------------------------*/
extern u8 nuSiMgrInit(void);
extern void nuSiCallBackAdd(NUCallBackList* list);
extern void nuSiCallBackRemove(NUCallBackList* list);
extern s32 nuSiSendMesg(NUScMsg mesg, void* dataPtr);
extern void nuSiSendMesgNW(NUScMsg mesg, void* dataPtr);
extern void nuSiMgrStop(void);
extern void nuSiMgrRestart(void);
/*--------------------------------------*/
/* si function				*/
/*--------------------------------------*/
extern s32 nuVrsMgr(void *mesg);
extern void nuVrsMgrInit(void);
extern s32 nuVrsOpen(NUVrsHandle* handle, s32 contNo);
extern s32 nuVrsClearDictionary(NUVrsHandle* handle, u8 words);
extern s32 nuVrsSetWord(NUVrsHandle* handle, u8* word);
extern s32 nuVrsStartReadData(NUVrsHandle* handle);
extern s32 nuVrsGetReadData(NUVrsHandle* handle, NUVrsData* data);
extern s32 nuVrsStopReadData(NUVrsHandle* handle);
extern s32 nuVrsControlGain(NUVrsHandle* handle, s32 analog, s32 digital);
extern s32 nuVrsMaskDictionary(NUVrsHandle* handle, u8* maskpattern, s32 size);

/*--------------------------------------*/
/* dubug function			*/
/*--------------------------------------*/
#ifdef NDEBUG
#define nuDebTaskPerfBar0(EX0 ,EX1 ,EX2)	((void)0)
#define nuDebTaskPerfBar1(EX0 ,EX1 ,EX2)	((void)0)
#define nuDebTaskPerfBar0EX2(EX0 ,EX1 ,EX2)	((void)0)
#define nuDebTaskPerfBar1EX2(EX0 ,EX1 ,EX2)	((void)0)
#define	nuDebPerfMarkSet(EX0)			((void)0)
#define nuDebTaskPerfIntervalSet(EX0)		((void)0)
#else
extern void nuDebTaskPerfBar0(u32 frameNum, u32 y, u32 flag);
extern void nuDebTaskPerfBar1(u32 frameNum, u32 y, u32 flag);
extern void nuDebTaskPerfBar0EX2(u32 frameNum, u32 y, u32 flag);
extern void nuDebTaskPerfBar1EX2(u32 frameNum, u32 y, u32 flag);
extern u32 nuDebPerfMarkSet(s32 markNo);
extern void nuDebTaskPerfIntervalSet(u32 interval);
#ifdef F3DEX_GBI_2
#define nuDebTaskPerfBar0(a, b, c)	nuDebTaskPerfBar0EX2(a, b, c)
#define nuDebTaskPerfBar1(a, b, c)	nuDebTaskPerfBar1EX2(a, b, c)
#endif	/* F3DEX_GBI_2 */
					   
#endif /* NDEBUG */

extern void nuDebConDisp(u32 flag);
extern void nuDebConDispEX2(u32 flag);
extern void nuDebConCPuts(u32 wndNo, const char *s);
extern void nuDebConInc(NUDebConWindow* conWin);
extern void nuDebConRtn(NUDebConWindow* conWin);
extern void nuDebConEsc(NUDebConWindow* conWin, char esc);
extern void nuDebConWindowSet(u32 wndNo, u32 winX , u32 winY, u32 width, u32 height);
extern void nuDebConWindowSize(u32 wndNo,  u32 width, u32 height);
extern void nuDebConWindowPos(u32 wndNo, u32 winX , u32 winY);
extern void nuDebConTextColor(u32 wndNo, u32 color);
extern void nuDebConTextAttr(u32 wndNo, u32 attr);
extern void nuDebConTextPos(u32 wndNo, u32 posX, u32 posY);
extern void nuDebConScroll(u32 wndNo, u32 flag);
extern void nuDebConWindowShow(u32 wndNo, u32 flag);
extern void nuDebConClear(u32 wndNo);
extern void nuDebConPutc(u32 wndNo, u32  c);
extern void nuDebTaskPerfLoad(void);
extern void nuDebConPrintf(u32 wndNo, const char* fmt, ...);


#ifdef F3DEX_GBI_2
#define	nuDebConDisp(flag)		nuDebConDispEX2(flag)
#endif	/* F3DEX_GBI_2 */

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/* MACRO	 							*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*	Register the graphics microcode					*/
/*	IN:	Pointer to the graphics microcode NUUcode array		*/
/*----------------------------------------------------------------------*/
#define nuGfxSetUcode(ucode)						\
{									\
    nuGfxUcode = ucode;							\
}
/*----------------------------------------------------------------------*/
/*	Set the Z buffer						*/
/*	IN:	Z buffer pointer					*/
/*----------------------------------------------------------------------*/
#define nuGfxSetZBuffer(ZBuf_p)						\
{									\
    nuGfxZBuffer = ZBuf_p;						\
}
/*----------------------------------------------------------------------*/
/*	Remove the callback function					*/
/*	IN:	None							*/
/*----------------------------------------------------------------------*/
#define	nuPreNmiFuncRemove()		nuPreNmiFuncSet(NULL)
#define nuGfxFuncRemove()		nuGfxFuncSet(NULL)
#define nuGfxPreNMIFuncRemove()		nuGfxPreNMIFuncSet(NULL)
#define	nuGfxSwapCfbFuncRemove()	nuGfxSwapCfbFuncSet(NULL)
#define	nuGfxTaskEndFuncRemove()	nuGfxTaskEndFuncSet(NULL)
#define nuContReadFuncRemove()		nuContReadFuncSet(NULL)

/*----------------------------------------------------------------------*/
/*	nuContDataGet - Get the Controller data				*/
/*	Get the read data from the Controller manager			*/
/*									*/
/*	IN:	*contpad Controller structure pointer			*/
/* 		cont_no  Controller number				*/
/*----------------------------------------------------------------------*/
#if 0
#define	nuContDataGet(contpad, cont_no)					\
{									\
    bcopy(&nuContData[cont_no], contpad, sizeof(OSContPad));		\
}

/*----------------------------------------------------------------------*/
/*	nuContDataGetAll - Get the Controller data			*/
/*	Get the read data from the Controller manager. 			*/
/*	Copy all four buffers						*/
/*	IN:	*contpad Controller structure pointer			*/
/* 		cont_no  Controller number				*/
/*----------------------------------------------------------------------*/
#define nuContDataGetAll(contpad)					\
{									\
    bcopy(&nuContData[0], contpad, sizeof(OSContPad)*MAXCONTROLLERS);\
}
#endif
/*----------------------------------------------------------------------*/
/*	nuContPakFileFind - Find the node				*/
/*----------------------------------------------------------------------*/
#define nuContPakFileFind(file, nodeName, extName)			\
    nuContPakFileOpen(file, nodeNname, extName, NU_CONT_PAK_NOCREAT, 0)\


/*----------------------------------------------------------------------*/
/*	nuContPakFileFindJis - Find the note				*/
/*----------------------------------------------------------------------*/
#define nuContPakFileFindJis(file, nodeName, extName)			\
    nuContPakFileOpenJis(file, nodeNname, extName, NU_CONT_PAK_NOCREAT, 0)\

/*----------------------------------------------------------------------*/
/*	nuContPakFileRead - Read to the note				*/
/*----------------------------------------------------------------------*/
#define nuContPakFileRead(file, offset, size, buf)			\
    nuContPakFileReadWrite(file, offset, size, buf, PFS_READ)		\

/*----------------------------------------------------------------------*/
/*	nuContPakFileWrite - Write to the note				*/
/*----------------------------------------------------------------------*/
#define nuContPakFileWrite(file, offset, size, buf)			\
    nuContPakFileReadWrite(file, offset, size, buf, PFS_WRITE)		\

/*----------------------------------------------------------------------*/
/*	nuDebConPuts - Output character string (with line feeds)	*/
/*----------------------------------------------------------------------*/
#define  nuDebConPuts(wndNo, s)						\
{									\
    nuDebConCPuts(wndNo, s);						\
    nuDebConRtn(&nuDebConWin[wndNo]);					\
}

/*----------------------------------------------------------------------*/
/*	nuContGBPakRead - Read the GB Game Pak				*/
/*----------------------------------------------------------------------*/
#define nuContGBPakRead(file, address, buf, size)			\
    nuContGBPakReadWrite(file, OS_READ, address, buf, size)

/*----------------------------------------------------------------------*/
/*	nuContGBPakWrite - Write to the GB Game Pak			*/
/*----------------------------------------------------------------------*/
#define nuContGBPakWrite(file, address, buf, size)			\
    nuContGBPakReadWrite(file, OS_WRITE, address, buf, size)

/*----------------------------------------------------------------------*/
/*	nuPiReadSram - Read from SRAM					*/
/*----------------------------------------------------------------------*/
#define nuPiReadSram(addr, buf_ptr, size)				\
	nuPiReadWriteSram(addr, buf_ptr, size, OS_READ)
    
/*----------------------------------------------------------------------*/
/*	nuPiWriteSram - Write to SRAM					*/
/*----------------------------------------------------------------------*/
#define nuPiWriteSram(addr, buf_ptr, size)				\
    nuPiReadWriteSram(addr, buf_ptr, size, OS_WRITE)
	
/*----------------------------------------------------------------------*/
/*	Voice Recognition Macro Definitions				*/
/*----------------------------------------------------------------------*/
#define	nuVrsCheckWord		osVoiceCheckWord
#define	nuVrsCountSyllables	osVoiceCountSyllables



#endif  /* defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS) */
#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif
#endif /* _NUSYS_H_ */


