/*
   graphic.c

   General-purpose graphics routines (initialize, clear frame buffer) and definitions of external variables. 

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.
*/

#include <nusys.h>
#include "graphic.h"

Gfx          gfx_glist[GFX_GLIST_LEN];
Dynamic      gfx_dynamic;
Gfx*         glistp;

/*----------------------------------------------------------------------------
  gfxRCPIinit

  Initialize the RSP/RDP. 
----------------------------------------------------------------------------*/
void gfxRCPInit(void)
{
  /* Set the RSP segment register. */
  gSPSegment(glistp++, 0, 0x0);  /* For CPU virtual address */

  /* Set the RSP. */
  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(setup_rspstate));

  /* Set the RDP. */
  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(setup_rdpstate));
}

/*----------------------------------------------------------------------------
  gfxClearCfb

  Clear the address settings of the frame buffer and Z buffer. 

  The NuSYSTEM global variables nuGfxZBuffer (Z buffer address) and 
  nuGfxCfb_ptr (frame buffer address) are used. 
----------------------------------------------------------------------------*/
void gfxClearCfb(void)
{
  /* Clear the Z buffer. */
  gDPSetDepthImage(glistp++, OS_K0_TO_PHYSICAL(nuGfxZBuffer));
  gDPSetCycleType(glistp++, G_CYC_FILL);
  gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b,SCREEN_WD,
		   OS_K0_TO_PHYSICAL(nuGfxZBuffer));
  gDPSetFillColor(glistp++,(GPACK_ZDZ(G_MAXFBZ,0) << 16 |
			       GPACK_ZDZ(G_MAXFBZ,0)));
  gDPFillRectangle(glistp++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
  gDPPipeSync(glistp++);
  
    /* Clear the frame buffer. */
  gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
		   osVirtualToPhysical(nuGfxCfb_ptr));
  gDPSetFillColor(glistp++, (GPACK_RGBA5551(0, 0, 0, 1) << 16 | 
				GPACK_RGBA5551(0, 0, 0, 1)));
  gDPFillRectangle(glistp++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
  gDPPipeSync(glistp++);
}
