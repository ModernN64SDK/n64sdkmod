/*============================================================================
  NuSYSTEM sample program [SNAKE TAIL HACK]
  
  gamepakrwerr.c
  
  Copyright (C) 1997, NINTENDO Co,Ltd.
  ============================================================================*/

#include <assert.h>
#include <nusys.h>
#include <nualsgi_n.h>

#include "main.h"
#include "graphic.h"
#include "font.h"
#include "para.h"

int ReadFlg;
void DrawRwErr( void );
extern void ContPakOpenAll( void );
extern void NoteOpenAll( void );
extern int ReplayMode;
extern void SetViewMtx( Dynamic* dynamicp );
extern NUContData ContTriger[];
extern u32 nuScRetraceCounter;    /* The retrace counter  */
extern void InitGame( void );
volatile extern int stage;
extern int ContPakError;
extern NUContPakFile pakFile[];
extern int ContPakOpen[];
extern void SaveContPak( void );
extern int ChkPakStat( void );

/*--------------------------------------------------------------*/
/*  Make the display list and activate the graphic task 	*/
/*  IN:	None 							*/
/*  RET:None 							*/
/*--------------------------------------------------------------*/
void
makepakrwerrDL(void)
{
    Dynamic* dynamicp;

    /* Specify the dynamic buffer  */
    dynamicp = &gfx_dynamic[gfx_gtask_no];

    /* Specify the display list buffer  */
    glistp = &gfx_glist[gfx_gtask_no][0];

    /*  The initialization of RSP and RDP  */
    gfxRCPInit();

    /*Clear the frame and Z-buffer */
    gfxClearCfb();

    /* Setting the view-matrix  */
    SetViewMtx( dynamicp );

    DrawRwErr();

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
/*  The pak error progressing process 	*/
/*  IN:	None 			*/
/*  RET:None 			*/
/*------------------------------*/
void
updateRwPakErr(void)
{
    /* A BUTTON */
    if(ContTriger[0].trigger & A_BUTTON){
	/* Remove the call-back function. */
	nuGfxFuncRemove();
	/* Specify next stage to main  */
	stage = 0;
	/* Play effect of OK */
	nuAuSndPlayerPlay(EFF_OK);
	nuAuSndPlayerSetPitch(44100.0/32000);
    }

    /* B BUTTON */
    if(ContTriger[0].trigger & B_BUTTON){
	ContPakOpenAll();
	if(ChkPakStat() != -1){
	    /* Remove the call-back function. */
	    nuGfxFuncRemove();
	    /* To the pak error screen  */
	    stage = 4;
	}else{
	    NoteOpenAll();
	    /* Save data of the controller pak  */
	    SaveContPak();
	    if(ChkPakStat() == -1){
		/* Remove the call-back function. */
		nuGfxFuncRemove();
		/* Indicate next stage to main  */
		stage = 0;
	    }else{
		/* Remove the call-back function. */
		nuGfxFuncRemove();
		ReadFlg = 1;
		/* Indicate next stage to main */
		stage = 4;
	    }
	}

	/* Play effect of OK */
	nuAuSndPlayerPlay(EFF_OK);
	nuAuSndPlayerSetPitch(44100.0/32000);
    }

    
}

/*------------------------------*/
/*  Draw the pak error screen 	*/
/*  IN:	None 			*/
/*  RET:None 			*/
/*------------------------------*/
void
DrawRwErr( void )
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
	  case RUMBLE_PAK:
	    sprintf(outstring,"PAK%d:RUMBLE PAK DETECTED", idx+1);
	    break;
	  default:
	    if(ReadFlg){
		sprintf(outstring,"PAK%d:LOAD FAIL", idx+1);
	    }else{
		sprintf(outstring,"PAK%d:SAVE FAIL", idx+1);
	    }
	    break;
	}
	Draw8Font(50,90+idx*10, TEX_COL_WHITE, 0);
    }

    sprintf(outstring,"PLEASE CHECK CONTROLLER PAK");
    Draw8Font(50,160, TEX_COL_WHITE, 0);

    sprintf(outstring,"PUSH A RETURN TITLE");
    Draw8Font(50,180, TEX_COL_WHITE, 0);

    if(ReadFlg){
	sprintf(outstring,"PUSH B LOAD RETRY");
    }else{
	sprintf(outstring,"PUSH B SAVE RETRY");
    }
    Draw8Font(50,200, TEX_COL_WHITE, 0);
}
