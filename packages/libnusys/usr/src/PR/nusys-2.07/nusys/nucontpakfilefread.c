/*======================================================================*/
/*		NuSYS						        */
/*		nucontpakfilereadwrite.c		              	*/
/*								        */
/*		Copyright (C) 1997, NINTENDO Co,Ltd.	          	*/
/*								        */
/*----------------------------------------------------------------------*/    
/* Ver 1.2	98/07/03		Created by Kensaku Ohki(SLANP)  */
/*======================================================================*/
/* $Id: nucontpakfilefread.c,v 1.2 1999/01/21 07:15:34 ohki Exp $	*/
/*======================================================================*/
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	nuContPakFileFread - Read the Controller Pak file. 	        */
/* 	Reads the Controller Pak file.  OK even if offset and size      */
/*	are not multiples of 32. 	                           	*/
/*	IN:	*file 	Controller Pak structure.       		*/
/*		offset	Offset position. 		        	*/
/*		size		Size			                */
/*		buf	      Buffer to store the read data.       	*/
/*	RET:	None				                   	*/
/*----------------------------------------------------------------------*/
s32 nuContPakFileFread(NUContPakFile *file, s32 offset, s32 size, u8* buf)
{
    s32		rtn;
    s32		readsize;
    s32		offset32;
    u16		readAdd;
    u8		data[32];

    
    
    while(size){

	offset32 = offset & 0x001f;
	if(offset32 || size < 32){
	    readAdd = offset & 0xffe0;
	    rtn = nuContPakFileRead(file, readAdd, 32, data);
	    if(rtn) return rtn;
	    if(size < 32){
		readsize = size;
	    } else {
		readsize = 32 - offset32;
	    }
	    bcopy((data + offset32), buf, readsize);
	    
	} else {
	    
	    /* If the read size is not a multiple of 32, make it */
	    /* a multiple of 32 and then read it. 		   */
	    readsize = size & 0xffe0;
	    rtn = nuContPakFileRead(file, offset, readsize, buf);
	    if(rtn) return rtn;
	}
	offset += readsize;
	buf += readsize;
	size -= readsize;
    }
    return 0;
}
