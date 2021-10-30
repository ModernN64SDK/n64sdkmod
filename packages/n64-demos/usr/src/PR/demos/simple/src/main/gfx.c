/*====================================================================
 * gfx.c
 *
 * Synopsis:
 *
 * This code implements the application graphics stuff
 * 
 * 
 * 
 * Copyright 1993, Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics,
 * Inc.; the contents of this file may not be disclosed to third
 * parties, copied or duplicated in any form, in whole or in part,
 * without the prior written permission of Silicon Graphics, Inc.
 *
 * RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to
 * restrictions as set forth in subdivision (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS
 * 252.227-7013, and/or in similar or successor clauses in the FAR,
 * DOD or NASA FAR Supplement. Unpublished - rights reserved under the
 * Copyright Laws of the United States.
 *====================================================================*/

#include <ultralog.h>

#include "gfx.h"
#include "simple.h"
#include "misc.h"

/*
 * graphics globals
 */

extern OSSched         sc;
extern OSMesgQueue     *sched_cmdQ;
extern GFXInfo         gInfo[];

extern s8    logoPos_x;
extern s8    logoPos_y;
extern s8    logoPos_z;
extern f32   logoScale_x;
extern f32   logoScale_y;
extern f32   logoScale_z;
extern f32   logoVeloc;

static u32          framecount;

static char         *staticSegment = 0;

#ifndef _FINALROM
extern OSTime      lastTime;
#endif

void doLogo(Dynamic *dynamicp);


void initGFX(void) 
{    
extern char _gfxdlistsSegmentEnd[];
    u32 len = (u32)(_staticSegmentRomEnd - _staticSegmentRomStart);

    staticSegment = _gfxdlistsSegmentBssStart;
    romCopy(_staticSegmentRomStart, staticSegment, len);
    
    gInfo[0].msg.gen.type = OS_SC_DONE_MSG;
    gInfo[0].cfb = cfb_16_a;
    gInfo[1].msg.gen.type = OS_SC_DONE_MSG;
    gInfo[1].cfb = cfb_16_b;

    /* The Vi manager was started by scheduler by this point in time */
    osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON);
}

void createGfxTask(GFXInfo *i) 
{
    static int firsttime = 1;
    Dynamic *dynamicp;
    OSScTask *t;
    
    /**** pointers to build the display list. ****/
    dynamicp = &i->dp;
    glistp   = i->dp.glist;

    /**** Tell RCP where each segment is  ****/
    gSPSegment(glistp++, 0, 0);	/* physical addressing segment */
    gSPSegment(glistp++, STATIC_SEGMENT,  osVirtualToPhysical(staticSegment));
    gSPSegment(glistp++, DYNAMIC_SEGMENT, osVirtualToPhysical(dynamicp));

    /**** Graphics pipeline state initialization ****/
    gSPDisplayList(glistp++, setup_rspstate);
    if (firsttime) {
        gSPDisplayList(glistp++, rdpstateinit_dl);
	firsttime = 0;
    }

    gSPDisplayList(glistp++, setup_rdpstate);
    
    /**** clear zbuffer, z = max z, dz = 0  ****/
    gDPSetDepthImage(glistp++, osVirtualToPhysical(zbuffer));
    gDPPipeSync(glistp++);
    gDPSetCycleType(glistp++, G_CYC_FILL);
    gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
		     osVirtualToPhysical(zbuffer));
    gDPSetFillColor(glistp++, (GPACK_ZDZ(G_MAXFBZ,0) << 16 |
			       GPACK_ZDZ(G_MAXFBZ,0)));
    gDPFillRectangle(glistp++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
	
    /**** Clear framebuffer cvg = FULL or 1  ****/
    gDPPipeSync(glistp++);
    gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
		     osVirtualToPhysical(i->cfb));
    /* clearcolor is 32-bits (2 pixels wide) because fill mode
     * does two pixels at a time.
     */
    gDPSetFillColor(glistp++, (GPACK_RGBA5551(0, 0, 0, 1) << 16 | 
			       GPACK_RGBA5551(0, 0, 0, 1)));
    gDPFillRectangle(glistp++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);

    gDPPipeSync(glistp++);
    gDPSetCycleType(glistp++, G_CYC_1CYCLE); 


    /**** Draw objects */
    doLogo(dynamicp);

    
    /**** Put an end on the top-level display list  ****/
    gDPFullSync(glistp++);
    gSPEndDisplayList(glistp++);

    /* Flush the dynamic segment */
    osWritebackDCache(&i->dp, (s32)glistp - (s32)&i->dp);

    /* build graphics task */

    t = &i->task;
    t->list.t.data_ptr = (u64 *) dynamicp->glist;
    t->list.t.data_size = (s32)(glistp - dynamicp->glist) * sizeof (Gfx);
    t->list.t.type = M_GFXTASK;
    t->list.t.flags = 0x0;
    t->list.t.ucode_boot = (u64 *)rspbootTextStart;
    t->list.t.ucode_boot_size = ((s32) rspbootTextEnd - 
				 (s32) rspbootTextStart);
    t->list.t.ucode = (u64 *) gspFast3DTextStart;
    t->list.t.ucode_data = (u64 *) gspFast3DDataStart;
    t->list.t.ucode_data_size = SP_UCODE_DATA_SIZE;
    t->list.t.dram_stack = (u64 *) dram_stack;
    t->list.t.dram_stack_size = SP_DRAM_STACK_SIZE8;
    t->list.t.output_buff = (u64 *) 0x0;
    t->list.t.output_buff_size = (u64 *) 0x0;
    t->list.t.yield_data_ptr = (u64 *) gfxYieldBuf;
    t->list.t.yield_data_size = OS_YIELD_DATA_SIZE;

    t->next     = 0;                   /* paranoia */
    t->flags	= (OS_SC_NEEDS_RSP | OS_SC_NEEDS_RDP | OS_SC_LAST_TASK |
		   OS_SC_SWAPBUFFER);
    t->msgQ     = &gfxFrameMsgQ;       /* reply to when finished */
    t->msg      = (OSMesg)&i->msg;     /* reply with this message */
    t->framebuffer = (void *)i->cfb;
#ifndef _FINALROM
    t->totalTime = 0;
#endif
    osSendMesg(sched_cmdQ, (OSMesg) t, OS_MESG_BLOCK); 
    
    framecount++;
}

/*
 * Draw the SGI Logo
 *
 * You can mix the gu and the gSP commands.  The important item to 
 * remember is that the cache must be flushed of any dynamic data
 * before the RSP starts reading the command list.
 */
void doLogo(Dynamic *dynamicp)
{
    u16		   perspNorm;
    static float   logo_theta = 0;
#ifndef _FINALROM
    u32      timeLen;
#endif
    /*
     * You must make the call to gSPPerspNormalize() in addition to 
     * using the perspective projection matrix.
     */
    guPerspective(&dynamicp->projection, &perspNorm,
		  33, 320.0/240.0, 400, 2000, 1.0);
    gSPPerspNormalize(glistp++, perspNorm);

    guLookAt(&dynamicp->viewing, 
	     0, 0, 1000,
	     0, 0, 0,
	     0, 1, 0);
    

    /* draw the background first */
    guScale(&dynamicp->bg_model, 12.0, 7.0, 1.0);
    gSPMatrix(glistp++, &dynamicp->projection, 
	       G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(glistp++, &dynamicp->viewing, 
	       G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
    gSPMatrix(glistp++, &dynamicp->bg_model, 
	       G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(glistp++, bg_dl);

    /* Position the logo: */
    guTranslate(&dynamicp->logo_trans, logoPos_x*4, logoPos_y*2, logoPos_z);
    /* Scale the logo */
    guScale(&dynamicp->logo_scale, logoScale_x, logoScale_y, logoScale_z);
    /* Rotate the logo */
    guRotate(&dynamicp->logo_rotate, logo_theta, 0.0, 1.0, 0.0);

    /* Setup model matrix */
    gSPMatrix(glistp++, &dynamicp->projection, 
	       G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(glistp++, &dynamicp->viewing, 
	       G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
    gSPMatrix(glistp++, &dynamicp->logo_trans, 
	      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(glistp++, &dynamicp->logo_scale, 
	      G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
    gSPMatrix(glistp++, &dynamicp->logo_rotate, 
	      G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
    
    /* Draw the logo */
    gSPDisplayList(glistp++, logo_dl);

    /* calculate theta for next frame */
    logo_theta += logoVeloc;

#ifndef _FINALROM   /* draw the performance bar */

#define USECS_PER_FRAME    16666  /* assuming a frame rate of 60fps */
#define USECS_PER_PIXEL    150
#define PIXELS_PER_FRAME   USECS_PER_FRAME/USECS_PER_PIXEL
#define LEFT_OFFSET        40
#define MARK_TOP           210
#define BAR_TOP            215
#define BAR_BOTTOM         220
#define MARK_BOTTOM        225
#define MARK_WIDTH         2

    gDPPipeSync(glistp++);

    gDPSetCycleType(glistp++, G_CYC_1CYCLE);
    gDPSetPrimColor(glistp++, 0, 0, 200, 0, 120, 255);
    gDPSetCombineMode(glistp++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetRenderMode(glistp++, G_RM_OPA_SURF, G_RM_OPA_SURF2);

    timeLen =(u32) (OS_CYCLES_TO_USEC(lastTime) / USECS_PER_PIXEL);
    gDPFillRectangle(glistp++, LEFT_OFFSET, BAR_TOP, 
		     LEFT_OFFSET + timeLen, BAR_BOTTOM);
    gDPFillRectangle(glistp++, LEFT_OFFSET+PIXELS_PER_FRAME, MARK_TOP, 
		     LEFT_OFFSET+PIXELS_PER_FRAME+MARK_WIDTH, MARK_BOTTOM);
    gDPFillRectangle(glistp++, LEFT_OFFSET+(2*PIXELS_PER_FRAME), MARK_TOP, 
		     LEFT_OFFSET+(2*PIXELS_PER_FRAME)+MARK_WIDTH, MARK_BOTTOM);
#endif
}
