/*======================================================================*/
/*		NuSYS					            	*/
/*		nucontgbpakreadwrite.c			              	*/
/*								        */
/*		Copyright (C) 1997, NINTENDO Co,Ltd.	        	*/
/*								        */
/*----------------------------------------------------------------------*/    
/* Ver 1.2	98/06/08		Created by Kensaku Ohki(SLANP)  */
/*======================================================================*/
/* $Id: nucontgbpakreadwrite.c,v 1.4 1999/01/21 07:16:23 ohki Exp $ */
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContGBPakReadWrite - Read and write to the GB Game Pak. 	*/
/* 	Read/write data to the GB Game Pak inserted in the 64GB Pak. 	*/
/*	IN:	handle	64GB Pak handler. 	        		*/
/*		flag	Read/write flag. 		                */
/*			NU_CONT_GBPAK_READ		             	*/
/*			NU_CONT_GBPAK_WRITE		               	*/
/*			or					        */
/*			OS_READ,OS_WRITE			        */
/*		address	GB Game Pak's CPU address. 	        	*/
/*		buffer	Pointer to the buffer in RDRAM. 		*/
/*		size	      Byte size to read/write. 			*/
/*	RET:	Value returned by osGbpakReadWrite(). 			*/
/*----------------------------------------------------------------------*/
s32 nuContGBPakReadWrite(NUContPakFile* handle, u16 flag , u16 address, u8* buffer, u16 size)
{
    NUContGBPakMesg 	gbpakMesg;

#ifdef NU_DEBUG
    if(address & 0x001f){
	osSyncPrintf("nuContGBPakReadWrite: address(0x%04X) error!!\n", address);
    }
    if(size & 0x001f){
	osSyncPrintf("nuContGBPakReadWrite: size(%d) error!!\n", size);
    }
#endif /* NU_DEBUG */
    
    gbpakMesg.handle   = handle;
    gbpakMesg.data[0]  = flag;
    gbpakMesg.data[1]  = address;
    gbpakMesg.data[2]  = (u32)buffer;
    gbpakMesg.data[3]  = size;
    handle->error = nuSiSendMesg(NU_CONT_GBPAK_READWRITE_MSG, (void*)&gbpakMesg);
    return handle->error;
}
