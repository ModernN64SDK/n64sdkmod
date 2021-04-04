/*======================================================================*/
/*		NuSYS							*/
/*		nugfxsetcfb.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/  
#include <nusys.h>

/*----------------------------------------------------------------------*/
/*	Framebuffer Settings						*/
/*	IN:	**framebuf 	Pointer to an array of pointers to frame buffers */
/*		framebufnum 	Number of frame buffers			*/
/*----------------------------------------------------------------------*/
void nuGfxSetCfb(u16** framebuf, u32 framebufnum)
{

    
    nuGfxCfb	= framebuf;
    nuGfxCfbNum	= framebufnum;
    nuGfxCfbCounter = 0;
    nuGfxCfb_ptr = nuGfxCfb[0];

    /* Wait 1 frame so that the values in VI Manager's current frame buffer */
    /* and next frame buffer are the same 				    */
    nuGfxRetraceWait(1);
    nuScSetFrameBufferNum(framebufnum);


    if(framebufnum > 1){
	s32	cnt;
	
	/* Check whether frame buffer addresses are the same. */	
	for(cnt = 0; cnt < framebufnum - 1; cnt++){
	    if(nuGfxCfb[cnt] != nuGfxCfb[cnt+1]){
		break;
	    }
	}
	if(cnt == (framebufnum - 1)){
	    nuScSetFrameBufferNum(1);
	} else {

	    /* Make the current frame buffer address different from the address */
	    /* to be the subject of the next rendering 				*/
	    for(cnt = 0; cnt < framebufnum; cnt++){
		if(osViGetCurrentFramebuffer() != nuGfxCfb[cnt]){
		    nuGfxCfbCounter = cnt;
		    nuGfxCfb_ptr = nuGfxCfb[cnt];
		    break;
		}
	    }

#ifdef NU_DEBUG
	    if(cnt == framebufnum){
		osSyncPrintf("nuGfxSetCfb: FrameBuffer Addres Error!!\n");
	    }
#endif
	}
    }

}
