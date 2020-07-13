/*======================================================================*/
/*		NuSYS					                */
/*		nucontpakfilereadwrite.c		              	*/
/*								        */
/*		Copyright (C) 1997, NINTENDO Co,Ltd.	          	*/
/*								        */
/*----------------------------------------------------------------------*/    
/* Ver 1.2	98/07/03		Created by Kensaku Ohki(SLANP)  */
/*======================================================================*/
/* $Id: nucontpakfilefwrite.c,v 1.2 1999/01/21 07:15:41 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakFileFwrite - Write to the Controller Pak file.      	*/
/* 	Writes to the Controller Pak file. 	OK even if offset and	*/
/*	size are not multiples of 32.                    		*/
/*	IN:	*file  	Controller Pak structure.          		*/
/*		offset	Offset position. 		         	*/
/*		size		Size				        */
/*		buf     	Data to write. 		       		*/
/*	RET:	None					                */
/*----------------------------------------------------------------------*/
s32 nuContPakFileFwrite(NUContPakFile *file, s32 offset, s32 size, u8* buf)
{
    s32		rtn;
    s32		writesize;
    s32		offset32;
    s32		orgsize;
    u16		writeAdd;
    u8		data[32];
        
    orgsize = size;
    while(size){
	/* Check whether the address is a multiple of 32 and whether the  */ 
	/* size is a multiple of 32-bytes.  If not, read the data into    */ 
	/* the buffer and then write. 			      		*/
	offset32 = offset & 0x001f;
	if(offset32 || size < 32){
	    writeAdd = offset & 0xffe0;
	    if(orgsize < 32){
		bzero(data, 32);
	    } else {
		rtn = nuContPakFileRead(file, writeAdd, 32, data);
		if(rtn) return rtn;
	    }

	    if(size < 32){
		writesize = size;
	    } else {
		writesize = 32 - offset32;
	    }
	    bcopy(buf, (data + offset32), writesize);
	    rtn = nuContPakFileWrite(file, writeAdd, 32, data);
	    if(rtn) return rtn;
	    
	} else {
	    
	    /* If the write size is not a multiple of 32, make it   */ 
	    /* a multiple of 32 and then write.				*/
	    writesize = size & 0xffe0;
	    rtn = nuContPakFileWrite(file, offset, writesize, buf);
	    if(rtn) return rtn;
	}
	offset += writesize;
	buf += writesize;
	size -= writesize;
    }
    return 0;
}

