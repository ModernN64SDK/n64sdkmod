/*======================================================================*/
/*		NuSYS N64 Transfer Pak test sample				*/
/*		main.c							*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*======================================================================*/  
#include <nusys.h>
#include "localdef.h"


u32 mainNo;
extern void menu(void);

/*----------------------------------------------------------------------*/
/*	Game start up							*/
/*	IN:	Noting for special					*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void mainproc(void* arg)
{
    /* The initialization for graphic */
    nuGfxInit();

    /* The initialization for controller */
    nuContInit();

    nuContGBPakMgrInit();
    
    /* Highreso for first time */
    mainNo = MAIN_MENU;

    
    /* Game main */
    while(1){
	switch(mainNo){
	case  MAIN_MENU:
	    menu();
	    break;
	    
	case MAIN_DUMMY:	/* Dummy */
	    break;
	    
	default:
	    break;
	}
    }
}

