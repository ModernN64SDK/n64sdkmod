/*======================================================================*/
/*		NuSYS high resolution sample		             		*/
/*		main.c					              		*/
/*								                  	*/
/*		Copyright (C) 1997, NINTENDO Co, Ltd.	         		*/
/*          97/12/08	Created by Kensaku Ohki(SLANP)			*/
/*======================================================================*/  
#include <nusys.h>
#include "localdef.h"

/*----------------------------------------------------------------------*/
/*	Memory mapping for high resolution and low resolution screens. 	*/
/*								                  	*/
/*              High resolution		Low resolution		      */
/*     0x80000000 +---------+              +----------+			*/
/*                | System  |              | System   |			*/
/*     0x80000400 +---------+              +----------+			*/
/*                |  Free   |              | Z-Buffer |			*/
/*     0x80025A00 +---------+              +----------+			*/
/*                |  Code   |              |  Code    |			*/
/*                | & Data  |              |  & Data  |			*/
/*                | & Heap  |              |  & Heap  |			*/
/*     0x801BE000 +---------+              |          |			*/
/*                |Z-Buffer |              |          |			*/
/*	            |FrameBuf |   0x8030F800 +----------+			*/
/*                |         |              | FrameBuf |			*/
/*                |         |              |          |			*/
/*     0x80380000 +---------+              +----------+			*/
/*                | Audio-  |              | Audio-   |			*/
/*                |  Heap   |              |  Heap    |			*/
/*     0x083FFFFF +---------+              +----------+			*/
/*								                    	*/
/*								                    	*/
/*	In high resolution, 0x80000400-0x80024FF becomes a free area. 	*/
/*	If you do not want to free this area, you can allocate it         */
/*    by dynamically securing the Z-Buffer.                             */
/*	However, the Z-Buffer and the FrameBuffer should be allocated	*/
/*	to another bank in RDRAM.  		                         	*/
/*	(This sample's high-resolution memory mapping is no good (^^;     */
/*	 but it is easy to manage the heap memory.) 			      */
/*----------------------------------------------------------------------*/

/* High resolution frame buffer address array. */
u16* HighFrameBuf[2] = {
    (u16*)CFB_HIGH_ADDR0,
    (u16*)CFB_HIGH_ADDR1
};

/* Low resolution frame buffer address array. */
u16*	LowFrameBuf[3] = {
    (u16*)CFB_LOW_ADDR0,
    (u16*)CFB_LOW_ADDR1,
    (u16*)CFB_LOW_ADDR2,
};

u32 mainNo;
void setupHigh(void);
void setupLow(void);

extern void graphic(u32 taskNum);
extern void audioInit(void);
extern u32 hireso;

/*----------------------------------------------------------------------*/
/*	Game startup. 					                    	*/
/*	IN:	Nothing in particular. 				              	*/
/*	RET:	None							                  */
/*----------------------------------------------------------------------*/
void mainproc(void* arg)
{
    /* Initialize graphics */
    nuGfxInit();

    /* Initialize the controller */
    nuContInit();

    
    /* High resolution at the start */
    mainNo = MAIN_HIGH;

    
    /* Game main */
    while(1){
	switch(mainNo){
	case MAIN_HIGH:	/* High resolution */
	    setupHigh();
	    mainNo = MAIN_DUMMY;
	    break;
	    
	case MAIN_LOW: 	/* Low resolution */
	    setupLow();
	    mainNo = MAIN_DUMMY;
	    break;
	    
	case MAIN_DUMMY:	/* Dummy */
	    break;
	    
	default:
	    break;
	}
    }
}

/*----------------------------------------------------------------------*/
/*	setupHigh - Set high resolution. 	          				*/
/*	IN:	Nothing in particular. 			        			*/
/*	RET:	None						                    	*/
/*----------------------------------------------------------------------*/
void setupHigh(void)
{
    
    /* Wait for all tasks to end */
    nuGfxTaskAllEndWait();
    
     /* Set VI */
    osViSetMode(&osViModeTable[OS_VI_NTSC_HAN1]);

    /* Since osViBlack becomes FALSE when the VI mode is changed, */
    /* set the screen display to OFF again. 		            */
    nuGfxDisplayOff();

    /* Set the frame buffer address */
    nuGfxSetCfb(HighFrameBuf, 2);

    
    /* Set the Z buffer address */
    nuGfxSetZBuffer((u16*)(ZBUFFER_ADDR));

    nuPiReadRom((u32)_himageSegmentRomStart, (u32*)_codeSegmentBssEnd,
		(u32)_himageSegmentRomEnd-(u32)_himageSegmentRomStart);


    nuDebConClear(0);
    nuDebConTextAttr(0, NU_DEB_CON_ATTR_BLINK);
    nuDebConTextPos(0, 13,25);
    nuDebConTextColor(0, NU_DEB_CON_TEXT_RED);
    nuDebConPuts(0, "High Resolution");
    nuDebConTextAttr(0, NU_DEB_CON_ATTR_NORMAL);

    /* Screen display ON */
    hireso  = 1;
    
    /* Register the callback function */
    nuGfxFuncSet(graphic);
    nuGfxDisplayOn();
}

/*----------------------------------------------------------------------*/
/*	setupHigh - Set low resolution. 			           		*/
/*	IN:	Nothing in particular. 					      	*/
/*	RET:	None							                  */
/*----------------------------------------------------------------------*/
void setupLow(void)
{
    /* Wait for all tasks to end */
    nuGfxTaskAllEndWait();

     /* Set VI */
    osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);

    /* Since osViBlack becomes FALSE when the VI mode is changed, */
    /* set the screen display to OFF again. 	             	*/
    nuGfxDisplayOff();
    
    /* Set the frame buffer address */
    nuGfxSetCfb(LowFrameBuf, 3);

    nuPiReadRom((u32)_limageSegmentRomStart, (u32*)_codeSegmentBssEnd,
		(u32)_limageSegmentRomEnd-(u32)_limageSegmentRomStart);

    /* Set the Z buffer address */
    nuGfxSetZBuffer((u16*)NU_GFX_ZBUFFER_ADDR);

    nuDebConClear(0);
    nuDebConTextAttr(0, NU_DEB_CON_ATTR_BLINK);
    nuDebConTextPos(0, 13,25);
    nuDebConTextColor(0, 1);
    nuDebConPuts(0, "Low Resolution");
    nuDebConTextAttr(0, NU_DEB_CON_ATTR_NORMAL);
    
    /* Screen display ON */
    nuGfxDisplayOn();
    hireso  = 0;

    /* Register the callback function */
    nuGfxFuncSet(graphic);
    nuGfxDisplayOn();
}
