/*============================================================================
  NuSYSTEM sample program [SNAKE TAIL HACK]
  
  gameover.c
  
  Copyright (C) 1997, NINTENDO Co,Ltd.
  ============================================================================*/

#include <assert.h>
#include <nusys.h>
#include <nualsgi_n.h>

#include "main.h"
#include "graphic.h"
#include "font.h"
#include "para.h"

int SavePak;
int SavePakEnd;
void DrawGameOver( void );
void SetSnakePara( void );

/* nusys */
extern u32 nuScRetraceCounter;    /* The retrace counter  */

/* gamemain.c */
extern void DrawHead( Dynamic*, int );
extern int Winner;

/* main.c */
extern void SaveContPak( void );
extern void ContPakOpenAll( void );
extern void NoteOpenAll( void );
extern NUContData ContTriger[];
extern SnakePara Snake;
extern int TraceIdx;
extern ContTrace ConTrace[100*60][4];
volatile extern int stage;
extern int ReplayMode;
extern int ChkPakStat( void );

/* view.c */
extern void SetViewMtxGameOver( Dynamic* dynamicp );

/* gamepakerr.c */
extern int NoSave;

/* gamepakrwerr.c */
extern int ReadFlg;

/*--------------------------------------------------------------*/
/*  Make the display list and activate the graphic task 	*/
/*  IN:	None 							*/
/*  RET:None 							*/
/*--------------------------------------------------------------*/
void
makegameoverDL(void)
{
    int idx;
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
    SetViewMtxGameOver( dynamicp );

    /* Setting the snake position, etc.  */
    SetSnakePara();

    /* Draw 4 heads  */
    for(idx = 0; idx < 4; idx++){
	DrawHead( dynamicp, idx );
    }

    /* Draw the text character  */
    DrawGameOver();

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
/*  The gameover progressing process 	*/
/*  IN:	None 			*/
/*  RET:None 			*/
/*------------------------------*/
void
updateGameOver(void)
{
    /* START BUTTON */
    if(ContTriger[0].trigger & START_BUTTON ){
	/* Remove the call-back function. */
	nuGfxFuncRemove();
	/* Specify next stage to main  */
	stage = 0;
    }

    /* If it is not the NoSave mode  */
    if(NoSave == 0){
	/* Save in the controller pak only when it's the replay mode  */
	if(ReplayMode == 0 && SavePak == 1){
	    ContPakOpenAll();
	    if(ChkPakStat() != -1){/* The error  */
		/* Remove the call-back function. */
		nuGfxFuncRemove();
		/* Specify next stage to main  */
		stage = 4;
	    }else{
		NoteOpenAll();
		SaveContPak();
		SavePak = 0;
		SavePakEnd = 1;
		if(ChkPakStat() != -1){
		    /* Remove the call-back function. */
		    nuGfxFuncRemove();
		    /* Specify next stage to main  */
		    stage = 4;
		    ReadFlg = 0;
		}
	    }
		
	}
	/* Save after displaying the screen once  */
	if(SavePak == 0 && SavePakEnd == 0 ){
	    SavePak = 1;
	}
    }
}

/*------------------------------*/
/*  Draw the game-over screen 	*/
/*  IN:	None 			*/
/*  RET:None 			*/
/*------------------------------*/
void
DrawGameOver( void )
{
    int idx;
/*    sprintf(outstring,"TRACE:%d",TraceIdx);
    Draw8Font(20,20, TEX_COL_WHITE, 0);*/

    sprintf(outstring,"GAME OVER");
    Draw8Font(120,40, TEX_COL_WHITE, 0);

    if(Winner != -1){
	sprintf(outstring,"WINNER IS PLAYER NO.%d", Winner + 1);
	Draw8Font(80,80, TEX_COL_YELLOW, 0);
    }else{
	sprintf(outstring,"DRAW GAME");
	Draw8Font(120,80, TEX_COL_YELLOW, 0);
    }

    for(idx = 0; idx < 4; idx++){
	sprintf(outstring,"PLAYER %d : %2d TAIL", idx+1, Snake.tailcnt[idx]);
	if(idx == Winner){
	    Draw8Font(110,120+idx*22, TEX_COL_YELLOW, 0);
	}else{
	    Draw8Font(110,120+idx*22, TEX_COL_WHITE, 0);
	}
    }

    if(SavePak && ReplayMode == 0){
	sprintf(outstring,"NOW SAVING PLAY DATA");
	Draw8Font(80,210, TEX_COL_WHITE, 0);
    }else{
	if(0x30 > (nuScRetraceCounter & 0x30)){
	    sprintf(outstring,"PUSH START RETURN TITLE");
	    Draw8Font(80,210, TEX_COL_WHITE, 0);
	}
    }
}

/*------------------------------------------------------*/
/*  Set the model position, etc. of the game-over screen 	*/
/*  IN:	None 						*/
/*  RET:None 						*/
/*------------------------------------------------------*/

void
SetSnakePara( void )
{
    Snake.pos_x[0][0] = -260.0;
    Snake.pos_y[0][0] = 0.0;
    Snake.pos_z[0][0] = 0.0;
    Snake.rot_x[0][0] = 270.0;
    Snake.rot_y[0][0] = 0.0;
    Snake.rot_z[0][0] = 270.0;
    Snake.size_x[0][0] = 2.0;
    Snake.size_y[0][0] = 2.0;
    Snake.size_z[0][0] = 2.0;

    Snake.pos_x[1][0] = -260.0;
    Snake.pos_y[1][0] = -80.0;
    Snake.pos_z[1][0] = 0.0;
    Snake.rot_x[1][0] = 270.0;
    Snake.rot_y[1][0] = 0.0;
    Snake.rot_z[1][0] = 270.0;
    Snake.size_x[1][0] = 2.0;
    Snake.size_y[1][0] = 2.0;
    Snake.size_z[1][0] = 2.0;

    Snake.pos_x[2][0] = -260.0;
    Snake.pos_y[2][0] = -160.0;
    Snake.pos_z[2][0] = 0.0;
    Snake.rot_x[2][0] = 270.0;
    Snake.rot_y[2][0] = 0.0;
    Snake.rot_z[2][0] = 270.0;
    Snake.size_x[2][0] = 2.0;
    Snake.size_y[2][0] = 2.0;
    Snake.size_z[2][0] = 2.0;

    Snake.pos_x[3][0] = -260.0;
    Snake.pos_y[3][0] = -240.0;
    Snake.pos_z[3][0] = 0.0;
    Snake.rot_x[3][0] = 270.0;
    Snake.rot_y[3][0] = 0.0;
    Snake.rot_z[3][0] = 270.0;
    Snake.size_x[3][0] = 2.0;
    Snake.size_y[3][0] = 2.0;
    Snake.size_z[3][0] = 2.0;
}
