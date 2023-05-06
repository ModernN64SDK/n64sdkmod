/*
   graphic.c

   General-use graphic routine (initialize and clear frame buffer)
   and external variable definitions

*/

#include <nusys.h>
#include "graphic.h"
#include "hvqm.h"

Gfx          gfx_glist[2][GFX_GLIST_LEN];
Dynamic      gfx_dynamic[2];
Gfx*         glistp;
u32          gfx_gtask_no = 0;

/*----------------------------------------------------------------------------
  gfxRCPIinit

  Initialize RSP/RDP
----------------------------------------------------------------------------*/
void gfxRCPInit(void)
{
  /* RSP segment register setting */
  gSPSegment(glistp++, 0, 0x0);  /* For CPU virtual address */

  /* RSP setting */
  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(setup_rspstate));

  /* RDP setting */
  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(setup_rdpstate));
}

/*----------------------------------------------------------------------------
  gfxClearCfb

  Set address and clear frame buffer/Z buffer 

  Uses the NuSYSTEM global variables nuGfxZBuffer (Z buffer address) and
  nuGfxCfb_ptr (frame buffer adrdess)
----------------------------------------------------------------------------*/
void gfxClearCfb(void)
{
  /* Clear the Z buffer */
  gDPSetDepthImage(glistp++, OS_K0_TO_PHYSICAL(nuGfxZBuffer));
  gDPSetCycleType(glistp++, G_CYC_FILL);
  gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b,SCREEN_WD,
		   OS_K0_TO_PHYSICAL(nuGfxZBuffer));
  gDPSetFillColor(glistp++,(GPACK_ZDZ(G_MAXFBZ,0) << 16 |
			       GPACK_ZDZ(G_MAXFBZ,0)));
  gDPFillRectangle(glistp++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
  gDPPipeSync(glistp++);
  
    /* Clear the frame buffer */
  gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
		   osVirtualToPhysical(nuGfxCfb_ptr));
  gDPSetFillColor(glistp++, (GPACK_RGBA5551(0, 0, 0, 1) << 16 | 
				GPACK_RGBA5551(0, 0, 0, 1)));
  gDPFillRectangle(glistp++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
  gDPPipeSync(glistp++);
}
#if HVQM_CFB_FORMAT == 2
/* frame buffer for 32bit */
void gfxClearCfb32(void)
{
  /* Clear the Z buffer */
  gDPSetDepthImage(glistp++, OS_K0_TO_PHYSICAL(nuGfxZBuffer));
  gDPSetCycleType(glistp++, G_CYC_FILL);
  gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b,SCREEN_WD,
		   OS_K0_TO_PHYSICAL(nuGfxZBuffer));
  gDPSetFillColor(glistp++,(GPACK_ZDZ(G_MAXFBZ,0) << 16 |
			       GPACK_ZDZ(G_MAXFBZ,0)));
  gDPFillRectangle(glistp++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
  gDPPipeSync(glistp++);
  
  /* Clear the frame buffer */
  gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_32b, SCREEN_WD,
		   osVirtualToPhysical(nuGfxCfb_ptr));
//  gDPSetFillColor(glistp++, (GPACK_RGBA5551(0, 0, 0, 1) << 16 | 
//				GPACK_RGBA5551(0, 0, 0, 1)));
  gDPSetFillColor(glistp++, 0x000000ff);
  gDPFillRectangle(glistp++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
  gDPPipeSync(glistp++);
}
#endif /* HVQM_CFB_FORMAT */
