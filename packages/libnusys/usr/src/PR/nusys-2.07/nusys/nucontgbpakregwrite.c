/*======================================================================*/
/*		NuSYS					       		*/
/*		nucontgbpakregwrite.c			             	*/
/*								        */
/*		Copyright (C) 1997, NINTENDO Co,Ltd.             	*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.2	98/07/11		Created by Kensaku Ohki(SLANP)  */
/*======================================================================*/
/* $Id: nucontgbpakreadwrite.c,v 1.4 1999/01/21 07:16:23 ohki Exp $ */
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContGBPakRegWrite - Write to the memory bank controller register.*/
/*	Writes data into the register of the memory bank controller.   	*/
/*	IN:	handle	64GB Pak handler. 				*/
/*		addr	      The GB Game Pak address to write to. 	*/
/*		data	      The data to write. 			*/
/*	RET:	None					                */
/*----------------------------------------------------------------------*/
s32 nuContGBPakRegWrite(NUContPakFile* handle, u16 addr, u8 data)
{
    u8	buf[32];
    u8  status;
    s32	rtn;

    bzero(buf, 32);
    buf[31] = data;

    
    /* Check for poor contact. */
    /* Supports MBC1,2,3 and cases of no memory controller */
    rtn =  nuContGBPakCheckConnector(handle, &status);
    if(rtn) return rtn;

    /* Write data to the register. */
    rtn = nuContGBPakWrite(handle, addr, buf, 32);
    if(rtn) return rtn;

    /* When getting the status, if the power is OFF there is a chance that  */
    /* the 64GB Pak or the controller has been removed or inserted.         */
    /* Since this has the same meaning as PFS_ERR_CONTRFAIL, the same thing */
    /* has been done here.  In the manual, where it talks about "If the     */
    /* power is OFF..." the messages are different in the flow.  If this    */
    /* matters to you, please make a change so the status is returned.      */
    rtn = nuContGBPakGetStatus(handle, &status);
    if(rtn) return rtn;
    if(!(status & OS_GBPAK_POWER)){
	return PFS_ERR_CONTRFAIL;
    }
    return rtn;

}
