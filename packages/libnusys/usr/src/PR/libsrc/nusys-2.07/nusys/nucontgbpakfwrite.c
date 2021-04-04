/*======================================================================*/
/*		NuSYS					            	*/
/*		nucontgbpakreadwrite.c		             		*/
/*							                */
/*		Copyright (C) 1997, NINTENDO Co,Ltd.          		*/
/*								        */
/*----------------------------------------------------------------------*/    
/* Ver 1.2	98/07/02		Created by Kensaku Ohki(SLANP) 	*/
/*======================================================================*/
/* $Id: nucontgbpakfwrite.c,v 1.4 1999/01/21 07:16:03 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>


/*----------------------------------------------------------------------*/
/*	nuContGBPakFwrite - Write to the GB Game Pak. 	          	*/
/* 	Writes data to a GB Game Pak inserted in the 64GB Pak.         	*/
/*	Neither the size nor the GB Game Pak addresses need to be      	*/
/*	multiples of 32. 		    		               	*/
/*	IN:	handle	64GB Pak handler. 		       		*/
/*		address	CPU address of GB Game Pak. 	      		*/
/*		buffer	Pointer to the buffer in RDRAM. 		*/
/*		size	      Data size. 				*/
/*	RET:	PFS_ERR_NOPACK				               	*/
/*		PFS_ERR_DEVICE				              	*/
/*		PFS_ERR_CONTRFAIL				        */
/*		PFS_ERR_NO_GBCART				        */
/*		PFS_ERR_NEW_GBCART			              	*/
/*----------------------------------------------------------------------*/
s32 nuContGBPakFwrite(NUContPakFile* handle, u16 address, u8* buffer, u16 size)
{
    s32		rtn;
    s32		writesize;
    s32		offset;
    s32		ram;
    u16		writeAdd;
    u8		data[32];
    u8    	status;
	
    if(nuContGBPakCallBack.func == NULL){
#ifdef NU_DEBUG
	osSyncPrintf("nuContGBPakReadWrite: no 64GBPack manager!!\n");
#endif /* NU_DEBUG */
	return 0;
    }

    /* Check for poor contact. 				         	*/
    /* Supports MBC1,2,3 and the case of no memory controller. 	*/
    rtn =  nuContGBPakCheckConnector(handle, &status);
    if(rtn) return rtn;


    /* Disable MBC1,2,3 SRAM write protection	*/
    ram = 0;
    if((0xa000 <= address) && (address < 0xc000)){
	bzero(data, 32);
	data[31] = NU_CONT_GBPAK_MBC_RAM_ENABLE_CODE;
	rtn = nuContGBPakWrite(handle, NU_CONT_GBPAK_MBC_RAM_REG0_ADDR, data, 32);
	if(rtn) return rtn;
	ram++;
    }
    
    while(size){
	/* Check whether address is a multiple of 32         */
	/* and whether size is a multiple of 32-bytes.       */
	/* If not, read data into buffer, and then write.    */
	offset = address & 0x001f;
	if(offset || size < 32){
	    writeAdd = address & 0xffe0;
	    rtn = nuContGBPakRead(handle, writeAdd, data, 32);
	    if(rtn) return rtn;
	    if(size < 32){
		writesize = size;
	    } else {
		writesize = 32 - offset;
	    }
	    bcopy(buffer, (data + offset), writesize);
	    rtn = nuContGBPakWrite(handle, writeAdd, data, 32);
	    if(rtn) return rtn;
	} else {
	    
	    /* If the write size is not a multiple of 32, make it     */
	    /* a multiple of 32 and then write it.  		        */
	    writesize = size & 0xffe0;
	    rtn = nuContGBPakWrite(handle, address, buffer, writesize);
	    if(rtn) return rtn;
	}
	address += writesize;
	buffer += writesize;
	size -= writesize;

    }
    
    /* MBC1,2,3 RAM write protection  */
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
