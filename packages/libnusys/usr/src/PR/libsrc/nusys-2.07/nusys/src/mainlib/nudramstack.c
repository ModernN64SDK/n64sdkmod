/*======================================================================*/
/*		NuSYS							*/
/*		nudramstack.c						*/
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

/*----------------------------------------------------------------------------*/
/* The DRAM Stack buffer 						      */
/* It is set as the separate object for making the 16-byte boundary sort.     */
/*----------------------------------------------------------------------------*/
u8 nuDramStack[SP_DRAM_STACK_SIZE8] __attribute__((aligned (16)));
