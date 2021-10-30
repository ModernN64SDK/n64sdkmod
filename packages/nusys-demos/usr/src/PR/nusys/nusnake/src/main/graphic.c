/*============================================================================
  NuSYSTEM sample program [SNAKE TAIL HACK]

  graphic.c
  
  Copyright (C) 1997, NINTENDO Co,Ltd.
  ============================================================================*/

#include <nusys.h>

#include "graphic.h"
#include "para.h"

Gfx          gfx_glist[GFX_GTASK_NUM][GFX_GLIST_LEN];
Dynamic      gfx_dynamic[GFX_GTASK_NUM];
Gfx*         glistp;
u32          gfx_gtask_no = 0;

/*----------------------------------------------------------------------------
  gfxRCPIinit
  
  The initialization of RCP 
  ----------------------------------------------------------------------------*/
void
gfxRCPInit(void)
{

    /* Setting the RSP segment register  */
    gSPSegment(glistp++, 0, 0x0); /* For the CPU virtual address  */

    /* Setting RSP  */
    gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(setup_rspstate));

    /* Setting RDP  */
    gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(setup_rdpstate));
}

/*----------------------------------------------------------------------------
  gfxClearCfb
  
  Clear the frame/Z-buffer 
  ----------------------------------------------------------------------------*/
void
gfxClearCfb(void)
{
    /* Clear the Z-buffer  */
    gDPSetDepthImage(glistp++, OS_K0_TO_PHYSICAL(nuGfxZBuffer));
    gDPPipeSync(glistp++);
    gDPSetCycleType(glistp++, G_CYC_FILL);
    gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b,SCREEN_WD,
		     OS_K0_TO_PHYSICAL(nuGfxZBuffer));
    gDPSetFillColor(glistp++,(GPACK_ZDZ(G_MAXFBZ,0) << 16 |
			      GPACK_ZDZ(G_MAXFBZ,0)));
    gDPFillRectangle(glistp++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
    gDPPipeSync(glistp++);
  
    /* Clear the frame buffer  */
    gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
		     osVirtualToPhysical(nuGfxCfb_ptr));
    gDPSetFillColor(glistp++, (GPACK_RGBA5551(0, 0, 0, 1) << 16 | 
			       GPACK_RGBA5551(0, 0, 0, 1)));
    gDPFillRectangle(glistp++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
    gDPPipeSync(glistp++);
}
