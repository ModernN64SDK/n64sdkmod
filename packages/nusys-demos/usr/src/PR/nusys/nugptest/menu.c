/*======================================================================*/
/*		NuSYS N64 Transfer Pak test sample				            */
/*		menu.c							            */
/*									                  */
/*		Copyright (C) 1997, NINTENDO Co, Ltd.			      */
/*======================================================================*/  
#include <nusys.h>
#include "localdef.h"

extern Gfx*	gfxListPtr;
extern Gfx*	gfxListStartPtr;
extern void	gfxListBufferChange(void);
extern void	gfxClearCfb(u32 size);
extern NUContData contData[4];

extern void funccheck(s32 funcNo);

/*----------------------------------------------------------------------*/
/*  Define MENU character strings.         					*/
/*----------------------------------------------------------------------*/
struct st_menuList;

typedef struct st_menuLink {
    struct st_menuList **list;
} MenuLink;

typedef struct st_menuList {
    u8*			title;
    struct st_menuList*	next;
    s32			flag;
} MenuList;

MenuList funcTestPowerList[] = {
    {"N64 Transfer Pak Power OFF", NULL, MENU_FUNCPOWER},
    {"N64 Transfer Pak Power ON", NULL, MENU_FUNCPOWER},
    {NULL, NULL, 0}
};

MenuList funcTestList[] = {
    {"nuContGBPakOpen()", NULL, MENU_FUNCTEST},
    {"nuContGBPakGetStatus()", NULL, MENU_FUNCTEST},
    {"nuContGBPakPower()", funcTestPowerList, MENU_MODE},
    {"nuContGBPakReadID()", NULL, MENU_FUNCTEST},
    {"nuContGBPakRead()", NULL, MENU_FUNCTEST},
    {"nuContGBPakWrite()", NULL, MENU_FUNCTEST},
    {"nuContGBPakCheckConnector()", NULL, MENU_FUNCTEST},
    {"nuContGBPakFread()", NULL, MENU_FUNCTEST},
    {"nuContGBPakFwrite()", NULL, MENU_FUNCTEST},
    {"nuContGBPakRegWrite()", NULL, MENU_FUNCTEST},
    {NULL, NULL, 0}
};



MenuList* menuListPtr =  funcTestList;	/* The menu list to display. */
MenuList* menuListPrev[4];		/* History of the selected menu. */
u8	menuSel[8];
u8	menuDeps;			/* Depth of the menu hierarchy. */
s32	menuFlag;			/* Menu display control flag. */
s32	dispFlag;			/* Display flag. */

NUContPakFile	file[MAXCONTROLLERS];
s32		contNo = 0;
u8		gbpakInit[MAXCONTROLLERS] = {0, 0, 0, 0};
OSGbpakId	gbpakId[MAXCONTROLLERS];
u8		gbData[0x10000];



/*----------------------------------------------------------------------*/
/*	dispFrame - Display the screen frame. 					*/
/*	Display the screen frame in text characters. 				*/
/*	IN:	None										*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void dispFrame(void)
{
    s32 cnt;

    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_LIGHTBLUE);
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 1);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, " ==================================== ");
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 9, 1);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, "N64 Transfer Pak Test");

    for(cnt = 0; cnt < 26; cnt++){
	nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 2 + cnt);
	nuDebConCPuts(NU_DEB_CON_WINDOW0, "#");
	nuDebConTextPos(NU_DEB_CON_WINDOW0, 38, 2 + cnt);
	nuDebConCPuts(NU_DEB_CON_WINDOW0, "#");
    }
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 1, 28);
    nuDebConCPuts(NU_DEB_CON_WINDOW0, " ==================================== ");
    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_WHITE);
    
}

/*----------------------------------------------------------------------*/
/*	dispMenu - Display the menu. 							*/
/*	In the menu hierarchy, display the specified menu list. 		*/
/*	IN:	None										*/
/*	RET:	None										*/
/*----------------------------------------------------------------------*/
void dispMenu(void)
{
    s32 cnt;
  
    for(cnt = 0; menuListPtr[cnt].title != NULL; cnt++){
	if(cnt == menuSel[menuDeps]){
	    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_RED);
	    nuDebConTextPos(NU_DEB_CON_WINDOW0, 3, 3 + menuSel[menuDeps] * 2);
	    nuDebConCPuts(NU_DEB_CON_WINDOW0, ">");
	} else {
	    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_WHITE);
	}
	nuDebConTextPos(NU_DEB_CON_WINDOW0, 4, 3 + cnt * 2);
	nuDebConCPuts(NU_DEB_CON_WINDOW0, menuListPtr[cnt].title);
    }
}


/*----------------------------------------------------------------------------*/
/*	dispFuncTest - Display the selected function as a subtitle. 	      */
/*	Center and display the selected function name in the subtitle position. */
/*	IN:	None										      */
/*	RET:	None										      */
/*----------------------------------------------------------------------------*/
void dispFuncTest(void)
{
    s32 len;
    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_GREEN);

    
    for(len = 0; menuListPtr[menuSel[menuDeps]].title[len] != '\0'; len++)
	;
    nuDebConTextPos(NU_DEB_CON_WINDOW0, 20-len/2, 3);
	
    nuDebConCPuts(NU_DEB_CON_WINDOW0, menuListPtr[menuSel[menuDeps]].title);
    nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_WHITE);
}

/*----------------------------------------------------------------------*/
/*	dispConsole - Control the screen display. 				*/
/*	Note current screen display state and control character output.   */
/*	IN:	None							                  */
/*	RET:	None						                   	*/
/*----------------------------------------------------------------------*/
void dispConsole(u32 taskNum)
{
    gfxListBufferChange();
    gfxClearCfb(0);

        /* end top-level display list */
    gDPFullSync(gfxListPtr++);
    gSPEndDisplayList(gfxListPtr++);
    
    nuGfxTaskStart(gfxListStartPtr, gfxListPtr - gfxListStartPtr,
		   NU_GFX_UCODE_S2DEX,NU_SC_NOSWAPBUFFER);

    nuDebConDisp(NU_SC_SWAPBUFFER);
    nuDebConClear(NU_DEB_CON_WINDOW0);
    dispFrame();
    switch(dispFlag){
    case DISP_MENU:
	dispMenu();
	break;
    case DISP_FUNCTEST:
	dispFuncTest();
	break;
    case DISP_FUNCPOWER:
	nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_GREEN);
	nuDebConTextPos(NU_DEB_CON_WINDOW0, 10, 3);
	nuDebConCPuts(NU_DEB_CON_WINDOW0, "nuContGBPakPower");
	break;
    case DISP_FUNCREADWRITE:
	break;
    case DISP_FUNCINPUTADDR:
	dispFuncTest();
	break;
    }
}

/*----------------------------------------------------------------------*/
/*	mainMenu - Input process						      */
/*	Check the pad input and perform menu list control process.        */
/*	IN:	None							                  */
/*	RET:	None							                  */
/*----------------------------------------------------------------------*/
void mainMenu(void)
{

    
    if(contData[0].trigger & D_JPAD){
	if(menuListPtr[menuSel[menuDeps]+1].title != NULL){
	    menuSel[menuDeps]++;
	}
    }
    
    if(contData[0].trigger & U_JPAD){
	if(menuSel[menuDeps] > 0){
	    menuSel[menuDeps]--;
	}
    }
    if(contData[0].trigger & A_BUTTON){
	MenuList* nextListPtr;
	nextListPtr = menuListPtr[menuSel[menuDeps]].next;
	if(nextListPtr){
	    menuListPrev[menuDeps] = menuListPtr;
	    menuListPtr = nextListPtr;
	    menuDeps++;
	    menuSel[menuDeps] = 0;
	} else {
	    menuFlag = menuListPtr[menuSel[menuDeps]].flag;
	}
    }
    
    if(contData[0].trigger & B_BUTTON){
	if(menuDeps){
	    menuDeps--;
	    menuListPtr = menuListPrev[menuDeps];
	}
    }

}

/*----------------------------------------------------------------------*/
/*	Menu operation process. 					      	*/
/*	IN:	Nothing in particular. 					        	*/
/*	RET:	None					                    		*/
/*----------------------------------------------------------------------*/
void menu(void)
{

    nuDebConScroll(NU_DEB_CON_WINDOW0, NU_DEB_CON_SCROLL_OFF);
    nuDebConWindowShow(NU_DEB_CON_WINDOW1, NU_DEB_CON_WINDOW_OFF);
    nuDebConWindowShow(NU_DEB_CON_WINDOW2, NU_DEB_CON_WINDOW_OFF);
    nuDebConWindowShow(NU_DEB_CON_WINDOW3, NU_DEB_CON_WINDOW_OFF);


    menuDeps = 0;
    menuFlag = MENU_MODE;
    dispFlag = DISP_MENU;
    nuGfxDisplayOn();
    nuGfxFuncSet(dispConsole);
    
    while(1){
	nuGfxRetraceWait(1);
	
	/* Read the controller data. */
	nuContDataGetExAll(contData);
	switch(menuFlag){
	case MENU_MODE:
	    dispFlag = DISP_MENU;
	    mainMenu();
	    break;
	case MENU_FUNCTEST:
	    dispFlag = DISP_FUNCTEST;
	    funccheck(menuSel[menuDeps]);
	    menuFlag = MENU_MODE;
	    break;
	case MENU_FUNCPOWER:
	    dispFlag = DISP_FUNCPOWER;
	    funccheck(menuSel[menuDeps-1]);
	    menuFlag = MENU_MODE;
	    menuDeps--;
	    menuListPtr = menuListPrev[menuDeps];
	    break;
	}
    }
}
