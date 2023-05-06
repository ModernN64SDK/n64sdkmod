/*
   stage01.c 
*/

#include <nusys.h>
#include "main.h"
#include "hvqm.h"
#include "graphic.h"

/*
 * hvqm data you want to playback
 */
extern u8 _hvqmdataSegmentRomStart[];	/* spec segment information */
#define HVQM_DATA _hvqmdataSegmentRomStart /* Long, so use another name */

void initStage01(void)
{
    hvqmStart(HVQM_DATA);
}

/*
 * If drawing occurs only on the part of the screen, then drawing is allowed in other parts.
 * Clearly, if it's designed so HVQM images are drawn into another buffer,
 * then it could be regarded separately.
 * Set either NU_SC_SWAPBUFFER or NU_SC_NOSWAPBUFFER for the arguments of hvqmVideoMgr
 */
void makeDL01(void){
/*
 * If it's HVQM_PERMIT_OTHER_GRAPHICS, the following task that clears the screen
 * is needed. But if it isn't, then it doesn't matter.
 * It'd better be included if the image size is smaller than the size of the frame buffer
 */
/* #if HVQM_PERMIT_OTHER_GRAPHICS == YES */
#if HVQM_SWAPBUFFER_CALLBACK == YES
    /*
     * Make sure before you proceed from this point, verify that
     * the task of previously launched HVQM is finished and the frame buffer is switched.
     */
    while( hvqmStatus&HVQM_STATUS_WAIT_SWAPBUFFER );
#endif /* HVQM_SWAPBUFFER_CALLBACK */

    /* Set display list buffer */
    gfx_gtask_no ^= 1;
    glistp = gfx_glist[gfx_gtask_no];

    /*  Initialize RCP */
    gfxRCPInit();

    /* Clear frame buffer and z buffer */
#if HVQM_CFB_FORMAT == 1
    gfxClearCfb();
#else /* HVQM_CFB_FORMAT */
    gfxClearCfb32();
#endif /* HVQM_CFB_FORMAT */
    gDPFullSync(glistp++);
    gSPEndDisplayList(glistp++);

    /* Starts the task */
    nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0],
		   (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx),
		   NU_GFX_UCODE_F3DEX , NU_SC_NOSWAPBUFFER);
/* #endif /* HVQM_PERMIT_OTHER_GRAPHICS */

    /* HVQM task starts */
    hvqmVideoMgr(NU_SC_SWAPBUFFER);
}

/*
 * This function does nothing related to the frame buffer
 */
void updateGame01(void)
{
    /* Controller 1 data read */
    nuContDataGetEx(contdata,0);

    /* Change stage with START button */
    if(contdata[0].trigger & START_BUTTON)
	stage = 0;

    /* Restart with A button */
    if(contdata[0].trigger & A_BUTTON)
	hvqmStart(HVQM_DATA);

    /* Horrifying infinite loop!? */
/*
    if(!(hvqmStatus&HVQM_STATUS_PLAYING))
	hvqmStart(HVQM_DATA);
*/
}

