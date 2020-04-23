/**************************************************************************
  File   : sraminit.c
  Copyright by Nintendo, Co., Ltd. 1997.
 **************************************************************************/

#include "rcp.h"
#include "os.h"
#include "sram.h"

OSPiHandle SramHandle;
/*
 * Name:   SramInit
 * Description:
 *	Initialize SRAM.
 */

OSPiHandle *
SramInit(void)
{
	u32 saveMask;

	if (SramHandle.baseAddress == PHYS_TO_K1(SRAM_START_ADDR))
		return(&SramHandle);

	/* Fill basic information */
	SramHandle.type = DEVICE_TYPE_SRAM;
	SramHandle.baseAddress = PHYS_TO_K1(SRAM_START_ADDR);

	/* Get Domain parameters */
	SramHandle.latency = (u8)SRAM_LATENCY;
	SramHandle.pulse = (u8)SRAM_PULSE;
	SramHandle.pageSize = (u8)SRAM_PAGE_SIZE;
	SramHandle.relDuration = (u8)SRAM_REL_DURATION;
	SramHandle.domain = PI_DOMAIN2;

	/* Fill speed and transferInfo to zero */
	SramHandle.speed = 0;
	bzero((void *)&(SramHandle.transferInfo),
			sizeof(SramHandle.transferInfo));
	
	/* Put the SramHandle onto PiTable*/
	osEPiLinkHandle(&SramHandle);
	return(&SramHandle);

}  /* SramInit */
