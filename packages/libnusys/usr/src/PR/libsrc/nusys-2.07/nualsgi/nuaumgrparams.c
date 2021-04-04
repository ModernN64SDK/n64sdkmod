/*======================================================================*/
/*		NuSYS							*/
/*		nuaumgrparams.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*	--/--/--	Created  by K.Ohki(SLANP)			*/
/*	98/12/10	Modified by K.Ohki(SLANP)			*/
/*======================================================================*/
/* $Id: nuaumgrparams.c,v 1.3 1999/06/10 05:02:51 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>
#include <nualsgi.h>



u64*		nuAuMgrStack;
NUScTask	nuAuTask;		/* Audio task buffer */
NUScClient	nuAuClient;
OSThread	nuAuMgrThread;
u32		nuAuFrameCounter = 0;	/* Frame counter */
ALGlobals	nuAuGlobal;
NUAuSeqPlayer	nuAuSeqPlayer[2];
ALBankFile*	nuAuSeqBank_ptr;
ALSeqFile*	nuAuSeqFile_ptr;
ALSndPlayer	nuAuSndPlayer;
ALBankFile*	nuAuSndBank_ptr = NULL;
ALSndId*       	nuAuSndId = NULL;
NUAuPreNMIFunc	nuAuPreNMIFunc = NULL;
NUAuMgrFunc	nuAuMgrFunc = NULL;
OSMesgQueue	nuAuMesgQ;
Acmd*		nuAuCmdListBuf; 		/* Command list pointer */
u32		nuAuAcmdLen = NU_AU_CLIST_LEN;
s16		nuAuFrameSampleSize;	/* Number of samples that can be created at one time */
s16		nuAuExtraSampleSize = 0;	/* Number of extra samples */
u8		nuAuTaskStop = NU_AU_TASK_RUN;
u8		nuAuPreNMI = 0;
u8		nuAuRetraceCount = 1;
#ifdef	NU_DEBUG
/*	For debugging and tuning	*/
u32		nuAuDebFlag = NU_AU_DEBUG_NORMAL;
u32		nuAuDebStatus = 0;
u32		nuAuDebAcmdLenMax = 0;	/* Maximum length of the audio command list */
u32		nuAuDebDmaCount = 0;	/* Total number of PI-DMAs		*/
u32		nuAuDebDmaFrame = 0;	/* DMA counter	*/
u32		nuAuDebDmaMax = 0;	/* Maximum number of PI-DMAs	*/
u32		nuAuDebDmaBufMaxUse = 0;/* Maximum number of DMA buffers used*/
u32		nuAuDebDmaCallbackCount = 0;/* Total number of DMA callbacks	*/
u32		nuAuDebDmaCallbackFrame = 0;/* Number of DMA callbacks	*/
u32		nuAuDebDmaCallbackMax= 0; /* Maximum number of DMA callbacks */
#endif	/* NU_DEBUG */
