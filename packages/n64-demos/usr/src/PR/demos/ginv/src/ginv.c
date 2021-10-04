/**************************************************************************
 *                                                                        *
 *               Copyright (C) 1995, Silicon Graphics, Inc.               *
 *                                                                        *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright  law.  They  may not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *                                                                        *
 *************************************************************************/

/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo. (Originated by SGI)
        
        $RCSfile: ginv.c,v $
        $Revision: 1.18 $
        $Date: 1998/09/30 13:47:14 $
 *---------------------------------------------------------------------*/

#include <ultra64.h>
#include <assert.h>

#include "ginv.h"

#define VIDEO_MSG	411
#define MAX_MESGS	1

#ifndef ABS
#define ABS(x)		(((x) < 0) ? -(x) : (x))
#endif

#define GINVPRINTF	osSyncPrintf

u64	bootStack[STACKSIZE/sizeof(u64)];

static void	idle(void *);
static void	mainproc(void *);

static OSThread	idleThread;
static u64	idleThreadStack[STACKSIZE/sizeof(u64)];

static OSThread	mainThread;
static u64	mainThreadStack[STACKSIZE/sizeof(u64)];

static OSMesgQueue	interruptQ;
static OSMesg		intBuf[MAX_MESGS];

OSPiHandle      *handler;

void
boot(void)
{
    osInitialize();

    handler = osCartRomInit();

    osCreateThread(&idleThread, 1, idle, (void *)0,
		   idleThreadStack+STACKSIZE/sizeof(u64), 10);
    osStartThread(&idleThread);

    /* never reached */
}

static void
idle(void *arg)
{
    /* Initialize video for timing clock */
    osCreateMesgQueue(&interruptQ, intBuf, MAX_MESGS);
    osCreateViManager(OS_PRIORITY_VIMGR);
    osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);
    osViSetEvent(&interruptQ, (OSMesg)VIDEO_MSG, 1);

    osCreateThread(&mainThread, 3, mainproc, arg,
		   mainThreadStack+STACKSIZE/sizeof(u64), 10);
    
    osStartThread(&mainThread);


    osSetThreadPri(0, 0);

    for (;;);
}

static void mainproc(void *arg)
{
  u32 rcp_version;
  u32 r4300_config;
  u32 count0, count1, numer, denom;
  s32 clk, pclk, rcpclk;
#ifdef UCODE_VERSION
  u32 buff, i, ucode2 = 0;
#endif /* UCODE_VERSION */
  u32 __osGetConfig( void );
  u32 releasenumber;

  /*
   *  Determine clock frequency
   */
  if (MQ_IS_FULL(&interruptQ))
	(void) osRecvMesg(&interruptQ, NULL, OS_MESG_BLOCK);

  osRecvMesg(&interruptQ, 0, OS_MESG_BLOCK);
  count0 = osGetCount();
  osRecvMesg(&interruptQ, 0, OS_MESG_BLOCK);
  count1 = osGetCount();

  r4300_config = __osGetConfig();

  switch((r4300_config & CONFIG_EC) >> 28) {
	case CONFIG_EC_1_1:
		numer = denom = 1;
		break;
	case CONFIG_EC_3_2:
		numer = 2;
		denom = 3;
		break;
	case CONFIG_EC_2_1:
		numer = 1;
		denom = 2;
		break;
	case CONFIG_EC_3_1:
		numer = 1;
		denom = 3;
		break;
	default:
		numer = denom = 1;
		GINVPRINTF("illegal value\n");
		break;
  }

  /*
   *  Blank Video
   */
  osViBlack(TRUE);

  /* 
   * Print Release Number
   */

  osEPiReadIo(handler, RAMROM_RELEASE_OFFSET, &releasenumber);
  GINVPRINTF("RELEASE %4.1f", (float)(((releasenumber & 0xffff00)>>8)/10));
  GINVPRINTF("%1s\n", (int)(&releasenumber) + 3);

  /*
   *  Print Version Register
   */
  rcp_version = *((u32 *)OS_PHYSICAL_TO_K1(MI_VERSION_REG));
  if(rcp_version == 0x01010101) {
  	GINVPRINTF("RCP  Version 1.0\n");
  } 
  else if (rcp_version == 0x02020102) {
  	GINVPRINTF("RCP  Version 2.0\n");
  } else {
  	GINVPRINTF("IO  Version %d\n", rcp_version & 0xff);
  	GINVPRINTF("RAC Version %d\n", (rcp_version >> 8) & 0xff);
  	GINVPRINTF("RDP Version %d\n", (rcp_version >> 16) & 0xff);
  	GINVPRINTF("RSP Version %d\n", (rcp_version >> 24) & 0xff);
  }

#ifdef UCODE_VERSION
  for(i = 0; i < 20; i++) {
	buff = *((u32 *)gspFast3DTextEnd-20+i);
	if(buff == 0x2400beef) {
	  ucode2 = 1;
	  break;
	}
  }

  if(ucode2) {
	 GINVPRINTF("Microcode for RCP 2.0 <-- hack alert\n"); 
  } else {
	 GINVPRINTF("Microcode for RCP 1.0 <-- hack alert\n"); 
  }

#endif /* UCODE_VERSION */

  /*
   *  Set by Boot procedure
   */
  GINVPRINTF("PIF  Version %d\n", osVersion);
  GINVPRINTF("DRAM size %d bytes\n", osMemSize);


  /*
   *  Print clock speed
   */
  GINVPRINTF("System Clock Ratio = %d:%d\n", denom, numer);
  clk = count1 - count0;
  if (osTvType == 0)
  	clk = ABS(clk) * 50 / 100000;
  else
  	clk = ABS(clk) * 60 / 100000;
  pclk = clk * 2;
  rcpclk = pclk * numer / denom;
  GINVPRINTF("R4300 freq = %4.1f MHz\n", (float)pclk / 10.0);
  GINVPRINTF("RCP freq   = %4.1f MHz\n", (float)rcpclk / 10.0);

  /*
   *  Set by Boot procedure
   */
  if (osTvType == 0)
    {
      GINVPRINTF("Video Mode is PAL \n");
    }
  else if (osTvType == 1)
    {
      GINVPRINTF("Video Mode is NTSC \n");
    }
  else
    {
      GINVPRINTF("ERROR: Unknown Video Mode %d \n", osTvType);
    }

  GINVPRINTF("ROM base address of the game image is 0x%08x\n", (u32)osRomBase);

  if (osRomType == 0)
    {
      GINVPRINTF("ROM type is cartridge \n");
    }
  else if (osRomType == 1)
    {
      GINVPRINTF("Rom type is bulk media \n");
    }
  else
    {
      GINVPRINTF("ERROR: Unknown ROM Type %d \n", osRomType);
    }

  if (osResetType == 0)
    {
      GINVPRINTF("Reset type is cold reset \n");
    }
  else if (osResetType == 1)
    {
      GINVPRINTF("Reset type is NMI \n");
    }
  else
    {
      GINVPRINTF("ERROR: Unknown Reset Type %d \n", osResetType);
    }

	/* osExit interacts poorly with the GINVPRINTF */
#ifdef _HW_VERSION_1
    osExit();
#endif /* _HW_VERSION_1 */

}
