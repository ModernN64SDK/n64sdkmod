/*======================================================================*/
/*		NuSYS							*/
/*		nurdpoutput.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>

#ifdef __MWERKS__
#pragma align(16)
#endif /* __MWERKS__ */

/*----------------------------------------------------------------------*/
/* The RSP fifo buffer 							*/
/* It is set as the separate object for making the 16-byte boundary sort*/
/*----------------------------------------------------------------------*/
#if defined(__GNUC__)
u8 nuRDPOutputBuf[NU_GFX_RDP_OUTPUTBUFF_SIZE] __attribute__((aligned (16)));
#else
u8 nuRDPOutputBuf[NU_GFX_RDP_OUTPUTBUFF_SIZE];
#endif

