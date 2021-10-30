/*============================================================================
  NuSYSTEM sample program [SNAKE TAIL HACK]
  
  gamepakerr.c
  
  Copyright (C) 1997, NINTENDO Co,Ltd.
  ============================================================================*/

#include <assert.h>
#include <nusys.h>
#include <nualsgi_n.h>

#include "main.h"
#include "graphic.h"
#include "font.h"
#include "para.h"

int NoSave;
void DrawErr( void );
extern int ReplayMode;
extern void SetViewMtx( Dynamic* dynamicp );
extern NUContData ContTriger[];
extern u32 nuScRetraceCounter;    /* The retrace counter  */
extern void InitGame( void );
volatile extern int stage;
extern int ContPakError;
extern NUContPakFile pakFile[];
extern int ContPakOpen[];

/*--------------------------------------------------------------*/
/*  Make the display list and activate the graphic task 	*/
/*  IN:	None 							*/
/*  RET:None 							*/
/*--------------------------------------------------------------*/
void
makepakerrDL(void)
{
    Dynamic* dynamicp;

    /* Specify the dynamic buffer  */
    dynamicp = &gfx_dynamic[gfx_gtask_no];

    /* Specify the display list buffer  */
    glistp = &gfx_glist[gfx_gtask_no][0];

    /*  The initialization of RSP and RDP */
    gfxRCPInit();

    /*Clear the frame and Z-buffer */
    gfxClearCfb();

    /* Setting the view-matrix  */
    SetViewMtx( dynamicp );

    DrawErr();

    gDPFullSync(glistp++);
    gSPEndDisplayList(glistp++);

    assert((glistp - gfx_glist[gfx_gtask_no]) < GFX_GLIST_LEN);

    /* Activate the RSP task and 
       switch display buffers. */
    nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0],
		   (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx),
		   NU_GFX_UCODE_F3DEX , NU_SC_SWAPBUFFER);

    /* Specify next graphic task number  */
    gfx_gtask_no++;
    gfx_gtask_no %= GFX_GTASK_NUM;
}

/*------------------------------*/
/*  The pack error progressing process 	*/
/*  IN:	None 			*/
/*  RET:None 			*/
/*------------------------------*/
void
updatePakErr(void)
{
    /* A BUTTON */
    if(ContTriger[0].trigger & A_BUTTON){
	/* Remove the call-back function. */
	nuGfxFuncRemove();
	/* Indicate next stage to main  */
	stage = 0;
	NoSave = 0;
	/* Play effect of OK  */
	nuAuSndPlayerPlay(EFF_OK);
	nuAuSndPlayerSetPitch(44100.0/32000);
    }
    
    /* B BUTTON */
    if(ContTriger[0].trigger & B_BUTTON){
	/* Remove the call-back function. */
	nuGfxFuncRemove();
	/* Indicate next stage to main  */
	stage = 1;
	NoSave = 1;
	ReplayMode = 0;
	InitGame();
	/* Play effect of OK */
	nuAuSndPlayerPlay(EFF_OK);
	nuAuSndPlayerSetPitch(44100.0/32000);
    }
    
}

/*------------------------------*/
/*  Draw the pack error screen 	*/
/*  IN:	None 			*/
/*  RET:None 			*/
/*------------------------------*/
void
DrawErr( void )
{
    int idx;
    
    sprintf(outstring,"CONTROLLER PAK ERROR!!");
    Draw8Font(60,70, TEX_COL_RED, 0);

    for(idx = 0; idx < 4; idx++){
	switch(ContPakOpen[idx]){
	  case 0:
	    sprintf(outstring,"PAK%d:OK", idx+1);
	    break;
	  case PFS_ERR_NOPACK:
	    sprintf(outstring,"PAK%d:NOPAK", idx+1);
	    break;
	  case PFS_ERR_NEW_PACK:
	    sprintf(outstring,"PAK%d:NEW_PAK", idx+1);
	    break;
	  case PFS_ERR_CONTRFAIL:
	    sprintf(outstring,"PAK%d:CONTROLLER PAK FAIL", idx+1);
	    break;
	  case PFS_ERR_ID_FATAL:
	    sprintf(outstring,"PAK%d:PAK ID FATAL", idx+1);
	    break;
	  case PFS_ERR_DEVICE:
	    sprintf(outstring,"PAK%d:DEVICE ERROR", idx+1);
	    break;
	  case NOT_CREAT:
	    sprintf(outstring,"PAK%d:FILE FULL OR PAK FULL", idx+1);
	    break;
	  case RUMBLE_PAK:
	    sprintf(outstring,"PAK%d:RUMBLE PAK DETECTED", idx+1);
	    break;
	  default:
	    break;
	}
	Draw8Font(50,90+idx*10, TEX_COL_WHITE, 0);
    }

    sprintf(outstring,"PLEASE CHECK CONTROLLER PAK");
    Draw8Font(50,160, TEX_COL_WHITE, 0);

    sprintf(outstring,"PUSH A RETURN TITLE");
    Draw8Font(50,180, TEX_COL_WHITE, 0);

    sprintf(outstring,"PUSH B PLAY NOSAVE");
    Draw8Font(50,200, TEX_COL_WHITE, 0);
}
