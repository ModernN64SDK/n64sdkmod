/*============================================================================
  NuSYSTEM sample program [SNAKE TAIL HACK]
  
  main.c
  
  Copyright (C) 1997, NINTENDO Co,Ltd.
  ============================================================================*/

#include <nusys.h>
#include <nualsgi_n.h>

#include "main.h"
#include "para.h"


/* The structure for creating trigger data of the controller  */
NUContData ContTriger[NU_CONT_MAXCONTROLLERS];
NUContPakFile*	file_chk;

/* For the game replay  */
ContTrace ConTrace[100*60][4];
int TraceIdx;
int ReplayMode;

/* The buffer for saving the controller data  */
u8 SaveBuf[100*60*4];

int ContPakError;
int ContPakOpen[4];
int ContPakFileOpen[4];

/* The stage number  */
volatile int stage;

/* Declaration of the prototype  */
void gamemain(int);
void gametitle(int);
void gameover(int);
void pakerr(int);
void pakrwerr(int);
void SavePadData( void );
void ReplayPadData( void );
void SaveContPak( void );
void LoadContPak( void );
void ContPakOpenAll( void );
void NoteOpenAll( void );
int ChkPakStat( void );


/* Declaration of extern  */
/* gamemain.c */
extern void makegameDL(void);
extern void updateGame(void);
extern void InitSnake( void );
extern void InitShotTail( void );
extern float View;



/* gametitle.c */
extern void maketitleDL(void);
extern void updateTitle(void);

/* gameover.c */
extern void makegameoverDL(void);
extern void updateGameOver(void);

/* gamepakerr.c */
extern void makepakerrDL(void);
extern void updatePakErr(void);
extern int NoSave;

/* gameparwkerr.c */
extern void makepakrwerrDL(void);
extern void updateRwPakErr(void);

/* main.c */
extern int Pause;

/* For n_audio */
extern long long int n_aspMainTextStart[],n_aspMainTextEnd[];
extern long long int n_aspMainDataStart[],n_aspMainDataEnd[];

extern u8 _midibankSegmentRomStart[],_midibankSegmentRomEnd[];
extern u8 _miditableSegmentRomStart[],_miditableSegmentRomEnd[];
extern u8 _seq1SegmentRomStart[],_seq1SegmentRomEnd[];

extern u8 _sfxbankSegmentRomStart[],_sfxbankSegmentRomEnd[];
extern u8 _sfxtableSegmentRomStart[],_sfxtableSegmentRomEnd[];

NUContPakFile pakFile[4];

/*----------------------*/
/*  The main process 	*/
/*  IN:	None 		*/
/*  RET:None 		*/
/*----------------------*/
void
mainproc(void)
{
    extern ALSynConfig    auSynConfig;

    /* The initialization of graphic  */
    nuGfxInit();

    /* The initialization of the controller manager  */
    nuContInit();

    /* The initialization of the audio manager  */
    nuAuInit();

    /*Register the bank to the sound player */
    nuAuSndPlayerBankSet(_sfxbankSegmentRomStart,
			 _sfxbankSegmentRomEnd - _sfxbankSegmentRomStart,
			 _sfxtableSegmentRomStart
			 );
    
    /*Register the bank to the sequence player */
    nuAuSeqPlayerBankSet(_midibankSegmentRomStart,
			 _midibankSegmentRomEnd - _midibankSegmentRomStart,
			 _miditableSegmentRomStart
			 );

    /*Register the sequence bank to the sequence player */
    nuAuSeqPlayerSeqSet(_seq1SegmentRomStart);

    /* Set the stage number to 0 */
    stage = 0;

    /* Select the call-back routine depending on stage numbers. 
       The call-back sets the value to stage for specifying the call-back
	   routine registering next when stage ends. */

    InitSnake();
    InitShotTail();
    View = 0.0;
    Pause = 1;
    TraceIdx = 0;
    ReplayMode = 0;
    while(1){
	switch(stage){
	  case 0:/*Title */
	    /* Set stage to the different value from the stage number first so
		   that you can notice the call-back has set the value */
	    stage = -1;
	    /* Play sequence of the title  */
	    nuAuSeqPlayerSetNo(0,0);
	    nuAuSeqPlayerPlay(0);
	    /* Call-back register  */
	    nuGfxFuncSet((NUGfxFunc)gametitle);
	    /* Start to display  */
	    nuGfxDisplayOn();
	    break;
	  case 1:/*Main of the game */
	    stage = -1;
	    /* Play sequence of this game volume  */
	    nuAuSeqPlayerSetNo(0,2);
	    nuAuSeqPlayerPlay(0);
	    /* Call-back register  */
	    nuGfxFuncSet((NUGfxFunc)gamemain);
	    /* Start to display  */
	    nuGfxDisplayOn();
	    break;
	  case 2:/*Game-over */
	    stage = -1;
	    /* Play sequence at game-over  */
	    nuAuSeqPlayerSetNo(0,1);
	    nuAuSeqPlayerPlay(0);
	    /* Call-back register  */
	    nuGfxFuncSet((NUGfxFunc)gameover);
	    /* Start to display  */
	    nuGfxDisplayOn();
	    break;
	  case 3:/*Pak error */
	    stage = -1;
	    /* Call-back register  */
	    nuGfxFuncSet((NUGfxFunc)pakerr);
	    /* Start to display  */
	    nuGfxDisplayOn();
	    break;
	  case 4:/*Pak error */
	    stage = -1;
	    /* Call-back register  */
	    nuGfxFuncSet((NUGfxFunc)pakrwerr);
	    /* Start to display  */
	    nuGfxDisplayOn();
	    break;
	  default:
	    break;
	}
      
	/* Wait for switching the scene  */
	while(stage == -1);
	/* Clear the display  */
	nuGfxDisplayOff();
    }
}

  
/*----------------------*/
/*  The process of the title 	*/
/*  IN:	pendingGfx	*/
/*  RET:None 		*/
/*----------------------*/
void
gametitle(int pendingGfx)
{
    /* Provide the display process if there is only one task which is processing or waiting for the process. */
    if(pendingGfx < 2){
	maketitleDL();		
	/*Create trigger data */
	nuContDataGetExAll(&ContTriger[0]);
    
	/* The title progressing process  */
	updateTitle();
    }
}

/*------------------------------*/
/*  The process of game main 	*/
/*  IN:	pendingGfx		*/
/*  RET:None 			*/
/*------------------------------*/
void
gamemain(int pendingGfx)
{
    /* Provide the display process if there is only one task which is processing or waiting for the process. */
    if(pendingGfx < 2){
	makegameDL();		

	/*In the case of selecting the REPLAY MODE at the title */
	if(ReplayMode){
	    ReplayPadData();
	    Pause = 0;
	}
	/*Create trigger data */
	nuContDataGetExAll(&ContTriger[0]);
	
	/*In the case of selecting PADDATA SAVE at the title */
	if(NoSave == 0){
	    if(ReplayMode == 0 && Pause == 0){
		SavePadData();
	    }
	}
    
	/* The game progressing process  */
	updateGame();
    }
}

/*------------------------------*/
/*  The process of game-over  	*/
/*  IN:	pendingGfx		*/
/*  RET:None  			*/
/*------------------------------*/
void
gameover(int pendingGfx)
{
    /* Provide the display process if there is only one task which is processing or waiting for the process. */
	if(pendingGfx < 2){
	makegameoverDL();

	/*Create trigger data */
	nuContDataGetExAll(&ContTriger[0]);
    
	/* The game-over progressing process */
	updateGameOver();
    }
}


/*----------------------------------------------*/
/*  The process of the controller pak error 	*/
/*  IN:	pendingGfx				*/
/*  RET:None					*/
/*----------------------------------------------*/
void
pakerr(int pendingGfx)
{
    /* Provide the display process if there is only one task which is processing or waiting for the process. */
    if(pendingGfx < 2){
	makepakerrDL();

	/*Create tigger data*/
	nuContDataGetExAll(&ContTriger[0]);
    
	/* The game-over progressing process  */
	updatePakErr();
    }
}


/*----------------------------------------------*/
/*  The process of the controller pak error 	*/
/*  IN:	pendingGfx				*/
/*  RET:None 					*/
/*----------------------------------------------*/
void
pakrwerr(int pendingGfx)
{
    /* Provide the display process if there is only one task which is processing or waiting for the process. */
    if(pendingGfx < 2){
	makepakrwerrDL();

	/*Create trigger data */
	nuContDataGetExAll(&ContTriger[0]);
    
	/* The game-over progressing process  */
	updateRwPakErr();
    }
}


/*--------------------------------------*/
/*  Record pad data to the buffer 	*/
/*  IN:	None 				*/
/*  RET:None 				*/
/*--------------------------------------*/
void
SavePadData( void )
{
    int idx;
    
    for(idx = 0; idx < 4; idx++){
	ConTrace[TraceIdx][idx].button = nuContData[idx].button;
	ConTrace[TraceIdx][idx].stick_x = nuContData[idx].stick_x;
	ConTrace[TraceIdx][idx].stick_y = nuContData[idx].stick_y;
    }
    TraceIdx++;
}


/*------------------------------------------------------*/
/*  Record pad data to the controller pak 	*/
/*  IN:	None 						*/
/*  RET:None 						*/
/*------------------------------------------------------*/
void
SaveContPak( void )
{
    int idx1, idx2, idx3;
    u8 _al, _ah, stx, sty;
    /*100*60*/
    for(idx1 = 0; idx1 < 4; idx1++){
	for(idx2 = 0, idx3 = 0; idx3 < 100*60; idx2+=4, idx3++){
	    /* To each work once with the u8 size from the buffer  */
	    _al = (u8)ConTrace[idx3][idx1].button;
	    _ah = (u8)(ConTrace[idx3][idx1].button >> 8);
	    stx = (u8)ConTrace[idx3][idx1].stick_x;
	    sty = (u8)ConTrace[idx3][idx1].stick_y;

	    SaveBuf[idx2] = _al;
	    SaveBuf[idx2 + 1] = _ah;
	    SaveBuf[idx2 + 2] = stx;
	    SaveBuf[idx2 + 3] = sty;
	}
	/* Save to the pak, practically */
	nuContPakFileWrite(&pakFile[idx1], 0, 100*60*4,SaveBuf);
	ContPakOpen[idx1] = pakFile[idx1].error;
    }
}

/*------------------------------------------------------*/
/*  Read pad data from the controller pak 	*/
/*  IN:	None 						*/
/*  RET:None 						*/
/*------------------------------------------------------*/
void
LoadContPak( void )
{
    int idx1, idx2, idx3;
    u8 _al, _ah, stx, sty;

    /* Register the company code and game code  */
    nuContPakCodeSet("64", "snake");
    
    for(idx1 = 0; idx1 < 4; idx1++){
	/* Open the note */
	nuContPakFileOpenJis(&pakFile[idx1], "snake tail hack", ""
			     ,NU_CONT_PAK_MODE_NOCREATE, GAME_NOTE_SIZE + 31);
	/* Load from the pak, practically  */
	nuContPakFileRead(&pakFile[idx1], 0, 100*60*4,SaveBuf);
	ContPakOpen[idx1] = pakFile[idx1].error;

	if(pakFile[idx1].error == 0){
	    for(idx2 = 0, idx3 = 0; idx3 < 100*60; idx2+=4, idx3++){
		/* To each work once with the u8 size from the buffer  */
		_al = SaveBuf[idx2];
		_ah = SaveBuf[idx2 + 1];
		stx = SaveBuf[idx2 + 2];
		sty = SaveBuf[idx2 + 3];
	    
		ConTrace[idx3][idx1].button = (_ah << 8) + _al;
		ConTrace[idx3][idx1].stick_x = stx;
		ConTrace[idx3][idx1].stick_y = sty;
	    }
	}
    }
}


/*--------------------------------------*/
/*  Play back saved pad data 	*/
/*  IN:	None 				*/
/*  RET:None 				*/
/*--------------------------------------*/
void
ReplayPadData( void )
{
    int idx;
    
    for(idx = 0; idx < 4; idx++){
	nuContData[idx].button = ConTrace[TraceIdx][idx].button;
	nuContData[idx].stick_x = ConTrace[TraceIdx][idx].stick_x;
	nuContData[idx].stick_y = ConTrace[TraceIdx][idx].stick_y;
    }
    TraceIdx++;
}


/*--------------------------------------*/
/*  Open the controller pak 	*/
/*  IN:	None 				*/
/*  RET:None 				*/
/*--------------------------------------*/
void
ContPakOpenAll( void )
{
    int idx;
    for(idx = 0; idx < 4; idx++){
	/* Recognize the controller pak  */
	nuContPakOpen(&pakFile[idx], idx);
	if(pakFile[idx].type == NU_CONT_PAK_TYPE_RUMBLE){
	    ContPakOpen[idx] = RUMBLE_PAK;
	}else{
	    ContPakOpen[idx] = pakFile[idx].error;
	}
	/*PFS_ERR_NOPACK, PFS_ERR_NEW_PACK, PFS_ERR_CONTRFAIL*/
	/*The error process about PFS_ERR_ID_FATAL and PFS_ERR_DEVICE*/
#if 0
	switch(ContPakOpen[idx]){
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
	  case RUMBLE_PAK:
	    osSyncPrintf("PAK%d\tRUMBLE_PACK DETECTED\n",idx);
	    break;
	  default:
	    break;
	}
#endif
    }
}


/*--------------------------------------------------------------*/
/*  Check the status of the opened controller pak 	*/
/*  IN:	None 							*/
/*  RET:None 							*/
/*--------------------------------------------------------------*/
int
ChkPakStat( void )
{
    int idx;
    for(idx = 0; idx < 4; idx++){
	if(ContPakOpen[idx] != 0){
	    return(idx);
	}
    }
    return(-1);
}


/*--------------------------------------*/
/*  Open the game note of the opened 	*/
/*  controller pak 	*/
/*  IN:	None 				*/
/*  RET:None				*/
/*--------------------------------------*/
void
NoteOpenAll( void )
{
    int idx;
    /* Register the company and game codes */
    nuContPakCodeSet("64", "snake");
    for(idx = 0; idx < 4; idx++){
	/* Open the note  */
	nuContPakFileOpenJis(&pakFile[idx], "snake tail hack", ""
			     ,NU_CONT_PAK_MODE_CREATE, GAME_NOTE_SIZE + 31);

	if(pakFile[idx].error == PFS_DATA_FULL || pakFile[idx].error == PFS_DIR_FULL){
	    pakFile[idx].error = NOT_CREAT;
	    ContPakOpen[idx] = pakFile[idx].error;
	}else{
	    ContPakOpen[idx] = 0;
	}
    }
}
