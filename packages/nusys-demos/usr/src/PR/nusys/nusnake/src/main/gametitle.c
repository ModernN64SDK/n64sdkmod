/*============================================================================
  NuSYSTEM sample program [SNAKE TAIL HACK]
  
  gametitle.c
  
  Copyright (C) 1997, NINTENDO Co,Ltd.
  ============================================================================*/

#include <assert.h>
#include <nusys.h>
#include <nualsgi_n.h>

#include "main.h"
#include "graphic.h"
#include "font.h"
#include "para.h"

/*nusys*/
extern u32 nuScRetraceCounter;    /* The retrace counter  */

/* gamemain.c */
extern void InitGame( void );

/* main.c */
extern void LoadContPak( void );
extern void ContPakOpenAll( void );
extern void NoteOpenAll( void );
extern int ChkPakStat( void );
extern int ReplayMode;
extern NUContData ContTriger[];
volatile extern int stage;
extern int ContPakError;
extern NUContPakFile pakFile[4];

/* view.c */
extern void SetViewMtx( Dynamic* dynamicp );

/* gamepakrwerr.c */
extern int ReadFlg;

void DrawTitle( void );
/*--------------------------------------------------------------*/
/*  Make the display list and activate the graphic task 	*/
/*  IN:	None 							*/
/*  RET:None 							*/
/*--------------------------------------------------------------*/
void
maketitleDL(void)
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

    DrawTitle();

    gDPFullSync(glistp++);
    gSPEndDisplayList(glistp++);

    assert((glistp - gfx_glist[gfx_gtask_no]) < GFX_GLIST_LEN);

    /* Activate the RSP task and switch display buffers. */
    nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0],
		   (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx),
		   NU_GFX_UCODE_F3DEX , NU_SC_SWAPBUFFER);

    /* Indicate next graphic task number  */
    gfx_gtask_no++;
    gfx_gtask_no %= GFX_GTASK_NUM;
}

/*----------------------*/
/*  The title progressing process 	*/
/*  IN:	None 		*/
/*  RET:None 		*/
/*----------------------*/
void
updateTitle(void)
{

#if 0
    int idx;
    if(ContTriger[0].trigger & L_TRIG){
	for(idx = 0; idx < 4; idx++){
	    nuContPakOpen(&pakFile[idx], idx);
	    switch(pakFile[idx].error){
	      case 0:
		osSyncPrintf("PAK%d\tOK\n",idx);
		break;
	      case PFS_ERR_NOPACK:
		osSyncPrintf("PAK%d\tPFS_ERR_NOPACK\n",idx);
		break;
	      case PFS_ERR_NEW_PACK:
		osSyncPrintf("PAK%d\tPFS_ERR_NEW_PACK\n",idx);
		break;
	      case PFS_ERR_CONTRFAIL:
		osSyncPrintf("PAK%d\tPFS_ERR_CONTRFAIL\n",idx);
		break;
	      case PFS_ERR_ID_FATAL:
		osSyncPrintf("PAK%d\tPFS_ERR_ID_FATAL\n",idx);
		break;
	      case PFS_ERR_DEVICE:
		osSyncPrintf("PAK%d\tPFS_ERR_DEVICE\n",idx);
		break;
	      default:
		break;
	    }
	}
	osSyncPrintf("\n");
    }
#endif
    
    /* A BUTTON */
    if(ContTriger[0].trigger & A_BUTTON){
	ContPakOpenAll();
	if(ChkPakStat() == -1){
	    NoteOpenAll();
	    if(ChkPakStat() != -1){
		/* Remove the call-back function. */
		nuGfxFuncRemove();
		/* To the pak error screen  */
		stage = 3;
	    }else{
		/* Remove the call-back function. */
		nuGfxFuncRemove();
		/* Indicate next stage to main  */
		stage = 1;
		/*The initialization for this game volume */
		InitGame();
		ReplayMode = 0;
	    }
	}else{
	    /* Remove the call-back function. */
	    nuGfxFuncRemove();
	    /* Indicate next stage to main  */
	    stage = 3;
	}
	/* Play effect of OK  */
	nuAuSndPlayerPlay(EFF_OK);
	nuAuSndPlayerSetPitch(44100.0/32000);
    }

    if(ContTriger[0].trigger & B_BUTTON){
	ContPakOpenAll();
	if(ChkPakStat() != -1){
	    /* Remove the call-back function. */
	    nuGfxFuncRemove();
	    /* To the pak error screen  */
	    stage = 3;
	}else{
	    NoteOpenAll();
	    /*The initialization for this game volume */
	    InitGame();
	    ReplayMode = 1;
	    /* Play effect of OK  */
	    nuAuSndPlayerPlay(EFF_OK);
	    nuAuSndPlayerSetPitch(44100.0/32000);
	    /* Load data of the controller pak  */
	    LoadContPak();
	    if(ChkPakStat() == -1){
		/* Remove the call-back function. */
		nuGfxFuncRemove();
		/* Indicate next stage to main  */
		stage = 1;
	    }else{
		/* Remove the call-back function. */
		nuGfxFuncRemove();
		ReadFlg = 1;
		/* Indicate next stage to main  */
		stage = 4;
	    }
	}
	/* Play effect of OK  */
	nuAuSndPlayerPlay(EFF_OK);
	nuAuSndPlayerSetPitch(44100.0/32000);
    }
}

/*----------------------*/
/*  Draw the title screen 	*/
/*  IN:	None 		*/
/*  RET:None 		*/
/*----------------------*/
void
DrawTitle( void )
{
    sprintf(outstring,"SNAKE TAIL HACK");
    Draw8Font(100,70, TEX_COL_WHITE, 0);

    if(0x30 > (nuScRetraceCounter & 0x30)){
	sprintf(outstring,"PUSH A PADDATA SAVE MODE");
	Draw8Font(50,130, TEX_COL_WHITE, 0);
	
	sprintf(outstring,"PUSH B REPLAY MODE");
	Draw8Font(50,150, TEX_COL_WHITE, 0);
    }
}
