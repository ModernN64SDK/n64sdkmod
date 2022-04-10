/*======================================================================*/
/*		NuSYS	                    				*/
/*		nucontgbpakfread.c	             			*/
/*					                     		*/
/*		Copyright (C) 1997, NINTENDO Co, Ltd.  			*/
/*							                */
/*----------------------------------------------------------------------*/    
/* Ver 1.2	98/07/02		Created by Kensaku Ohki(SLANP)  */
/*======================================================================*/
/* $Id: nucontgbpakfread.c,v 1.5 1999/01/21 07:15:52 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContGBPakFread - Reads the GB Game Pak              		*/
/* 	  Reads data from a GB Game Pak inserted in the 64GB Pak.   	*/
/*	  Neither the size nor the GB Game Pak addresses need to be 	*/
/*	multiples of 32. 					       	*/
/*	IN:	handle	64GB Pak handler.			       	*/
/*		address	GB Game Pak CPU address. 		       	*/
/*		buffer	Pointer to the buffer in RDRAM.			*/
/*		size	      Byte size. 				*/
/*	RET:	PFS_ERR_NOPACK				              	*/
/*		PFS_ERR_DEVICE				              	*/
/*		PFS_ERR_CONTRFAIL				        */
/*		PFS_ERR_NO_GBCART				        */
/*		PFS_ERR_NEW_GBCART				        */
/*----------------------------------------------------------------------*/
s32 nuContGBPakFread(NUContPakFile* handle, u16 address, u8* buffer, u16 size)
{
    s32		rtn;
    s32		readsize;
    s32		offset;
    s32		ram;
    u16		readAdd;
    u8		data[32];
    u8    	status;
        
    /* Check for poor contact. 				      	*/
    /* Supports MBC1,2,3 and cases of no memory controller.       */
    rtn =  nuContGBPakCheckConnector(handle, &status);
    if(rtn) return rtn;


    /* Disables MBC1,2,3 RAM protection.	*/
    ram = 0;
    if((0xa000 <= address) && (address < 0xc000)){
	bzero(data, 32);
	data[31] = NU_CONT_GBPAK_MBC_RAM_ENABLE_CODE;
	rtn = nuContGBPakWrite(handle, NU_CONT_GBPAK_MBC_RAM_REG0_ADDR, data, 32);
	if(rtn) return rtn;
	ram++;
    }


    /* Check while reading whether GB Game Pak and read size are */
    /* both multiples of 32. 					           */
    while(size){
	
	/* Check whether address is a multiple of 32.   */
	/* If not, read into the buffer. 			*/
	offset = address & 0x001f;
	if(offset || size < 32){
	    readAdd = address & 0xffe0;
	    rtn = nuContGBPakRead(handle, readAdd, data, 32);
	    if(rtn) return rtn;
	    if(size < 32){
		readsize = size;
	    } else {
		readsize = 32 - offset;
	    }
	    bcopy((data + offset), buffer, readsize);
	    
	} else {
	    
	    /* If read size is not a multiple of 32, make it a  */
	    /* multiple of 32 and read that in.                 */
	    readsize = size & 0xffe0;
	    rtn = nuContGBPakRead(handle, address, buffer, readsize);
	    if(rtn) return rtn;
	}
	address += readsize;
	buffer += readsize;
	size -= readsize;
    }
    /* MBC1,2,3 RAM protection */
    if(ram){
	bzero(data, 32);
	rtn = nuContGBPakWrite(handle, NU_CONT_GBPAK_MBC_RAM_REG0_ADDR, data, 32);
	if(rtn) return rtn;
    }

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

