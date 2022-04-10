/*======================================================================*/
/*		NuSYS										*/
/*		nugfxswapwait.c								*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/  
#include <nusys.h>


/*----------------------------------------------------------------------*/
/*	nuGfxSwapWait - Wait for all graphic tasks end 				*/
/*	IN:	None 										*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void nuGfxTaskAllEndWait(void)
{
    /* Wait for all graphic tasks end  */
    while(nuGfxTaskSpool);
}
