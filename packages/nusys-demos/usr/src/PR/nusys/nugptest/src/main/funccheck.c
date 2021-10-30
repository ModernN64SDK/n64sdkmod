/*======================================================================*/
/*		NuSYS N64 Transfer Pak test  sample				*/
/*		funccheck.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*======================================================================*/  
#include <nusys.h>
#include "localdef.h"

extern NUContData 	contData[4];
extern NUContPakFile	file[];
extern s32		contNo;
extern u8		gbpakInit[];
extern OSGbpakId	gbpakId[];
extern u8		gbData[];
extern s32		dispFlag;
extern u8		menuSel[8];
extern u8		menuDeps;
extern s32		menuFlag;
extern s32		dispFlag;

/*------------------------------------------------------*/
/* N64 Transfer Pak error messages				*/
/*------------------------------------------------------*/
u8* pfsErrorMesg[] = {
    "GOOD",
    "PFS_ERR_NOPACK",
    "PFS_ERR_NEW_PACK",
    "PFS_ERR_INCONSISTENT",
    "PFS_ERR_CONTRFAIL",
    "PFS_ERR_INVALID",
    "PFS_ERR_BAD_DATA",
    "PFS_DATA_FULL",
    "PFS_DIR_FULL",
    "PFS_ERR_EXIST",
    "PFS_ERR_ID_FATAL",
    "PFS_ERR_DEVICE",
    "PFS_ERR_NO_GBCART",
    "PFS_ERR_NEW_GBCART"
};

/*------------------------------------------------------*/
/* GB cartridge type (Game Pak type) messages		*/
/*------------------------------------------------------*/
typedef struct st_CasrtType {
    u8	no;
    u8*	name1;
    u8*	name2;
}  CartType;

CartType cartTypeList[] = {
    {0x00, " ROM               ", "                   "},
    {0x01, " ROM+MBC1          ", "                   "},
    {0x02, " ROM+MBC1          ", "      +SRAM        "},
    {0x03, " ROM+MBC1          ", "      +SRAM+Battery"},
    {0x05, " ROM+MBC2          ", "                   "},
    {0x06, " ROM+MBC2          ", "           +Battery"},
    {0x08, " ROM               ", "      +SRAM        "},
    {0x09, " ROM               ", "      +SRAM+Battery"},
    {0x0F, " ROM+MBC3(clock)   ", "           +Battery"},
    {0x10, " ROM+MBC3(clock)   ", "      +SRAM+Battery"},
    {0x11, " ROM+MBC3(no clock)", "                   "},
    {0x12, " ROM+MBC3(no clock)", "      +SRAM        "},
    {0x12, " ROM+MBC3(no clock)", "      +SRAM+Battery"},
    {0xFF, " Unknown Cart Type.",""}
};

/*------------------------------------------------------*/
/* GB cartridge (Game Pak) ROM size messages		*/
/*------------------------------------------------------*/
u8* romSizeList[] = {
    "256Kbit",
    "512Kbit",
    "  1Mbit",
    "  2Mbit",
    "  4Mbit",
    "  8Mbit",
    " 16Mbit"
};

/*------------------------------------------------------*/
/* GB cartridge (Game Pak) RAM size messages		*/
/*------------------------------------------------------*/
u8* ramSizeList[] = {
    "NO | MBC2",
    "   -     ",
    "64Kbit   ",
    "256Kbit  "
};

/*------------------------------------------------------*/
/* GB cartridge (Game Pak) destination messages		*/
/*------------------------------------------------------*/
u8*	destList[] = {
    "Japan   ",
    "Overseas"
};
/*------------------------------------------------------*/
/* MBC Controller register addresses			*/
/*------------------------------------------------------*/
u16	mbcRegAddr[] = {
    NU_CONT_GBPAK_MBC_REG0_ADDR,
    NU_CONT_GBPAK_MBC_REG1_ADDR,
    NU_CONT_GBPAK_MBC_REG2_ADDR,
    NU_CONT_GBPAK_MBC_REG3_ADDR
};

/*------------------------------------------------------*/
/* CGB support code  (Byte 16 of game title)             */
/*------------------------------------------------------*/
#define CGB_COMPATIBLE	0x80 /* CGB support */
#define CGB_EXCLUSIVE	0xC0 /* Used for CGB only (planned) */

/*----------------------------------------------------------------------*/
/*	dispfuncError - Display error					*/
/*	Displays characters specified in the GB Pak function error code */
/*	IN:	error	Error number					*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void dispfuncError(s32 error)
{
    u8 text[40];
    if(error) {
	nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_RED);	
    }
    sprintf(text, "Resut: %s(%d)", pfsErrorMesg[error], error);
    nuDebConPuts(NU_DEB_CON_WINDOW1, text);	
    nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_WHITE); 
}


/*----------------------------------------------------------------------*/
/*	funcOpen - Check nuContGBPakOpen()				*/
/*	Execute nuContGBPakOpen() and display execution results		*/
/*	IN:	None							*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void funcOpen(void)
{
    s32 rtn;
    u32	startTime;
    u32	funcTime;
    u8	text[80];
    
    nuDebConCPuts(NU_DEB_CON_WINDOW1, "Exec nuContGBPakOpen...");
    
    startTime = osGetCount();
    rtn = nuContGBPakOpen(&file[contNo], contNo);
    funcTime = osGetCount() - startTime;
    nuDebConPuts(NU_DEB_CON_WINDOW1, "End\n");
    
    dispfuncError(rtn);
    
    funcTime = OS_CYCLES_TO_USEC(funcTime);
    sprintf(text, "time = %d [us]", funcTime);
    nuDebConPuts(NU_DEB_CON_WINDOW1, text);
    if(!rtn){
	gbpakInit[contNo] = 1;
    }

}

/*----------------------------------------------------------------------*/
/*	dispStatus - Display status					*/
/*	Display the status bit information				*/
/*	IN:	status							*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void dispStatus(u8 status)
{
    u8	text[40];
    sprintf(text, "---------< Status 0x%02X >----------");
    nuDebConCPuts(NU_DEB_CON_WINDOW1, text);
    
    if(status & OS_GBPAK_GBCART_ON){
	nuDebConPuts(NU_DEB_CON_WINDOW1, "GBCART_ON");
    } else {
	nuDebConPuts(NU_DEB_CON_WINDOW1, "NO GBCART");
    }
    if(status & OS_GBPAK_GBCART_PULL){
	nuDebConPuts(NU_DEB_CON_WINDOW1, "GBCART_PULL");
    }
    if(status & OS_GBPAK_POWER){
	nuDebConPuts(NU_DEB_CON_WINDOW1, "GBCART_POWER ON");
    } else {
	nuDebConPuts(NU_DEB_CON_WINDOW1, "GBCART_POWER OFF");
    }
    
}

/*----------------------------------------------------------------------*/
/*	funcGetStatus - Check nuContGBPakGetStatus()			*/
/*	Execute nuContGBPakGetStatus() and display execution results	*/
/*	IN:	None							*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void funcGetStatus(void)
{
    u8	text[40];
    u8	status;
    u32	startTime;
    u32	funcTime;
    s32 rtn;
    
    /* Check whether or not nuContGBPakOpen was executed	*/
    /* This has to be called first or it will hang	*/
    if(!gbpakInit[contNo]){
	nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_RED);
	nuDebConCPuts(NU_DEB_CON_WINDOW1, "Don't exec nuContGBPakOpen!!");
	nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_WHITE);
	return;
    }
    nuDebConCPuts(NU_DEB_CON_WINDOW1, "Exec nuContGBPakGetStatus...");    

    startTime = osGetCount();
    rtn = nuContGBPakGetStatus(&file[contNo], &status);
    funcTime = osGetCount() - startTime;
    nuDebConPuts(NU_DEB_CON_WINDOW1, "End\n");
    dispfuncError(rtn);
    
    funcTime = OS_CYCLES_TO_USEC(funcTime);
    sprintf(text, "time = %d [us]", funcTime);
    nuDebConPuts(NU_DEB_CON_WINDOW1, text);
    
    dispStatus(status);
}

/*----------------------------------------------------------------------*/
/*	funcPower - Check nuContGBPakPower()				*/
/*	Execute nuContGBPakPower()and display execution results		*/
/*	IN:	None							*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void funcPower(void)
{
    u8	text[40];
    u32	startTime;
    u32	funcTime;
    s32 rtn;
    
    /* Check whether or not nuContGBPakOpen was executed	*/
    /* This has to be called first or it will hang	*/
    if(!gbpakInit[contNo]){
	nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_RED);
	nuDebConCPuts(NU_DEB_CON_WINDOW1, "Don't exec nuContGBPakOpen!!");
	nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_WHITE);
	return;
    }
    nuDebConCPuts(NU_DEB_CON_WINDOW1, "Exec nuContGBPakPower...");
    startTime = osGetCount();
    rtn = nuContGBPakPower(&file[contNo], menuSel[menuDeps]);
    funcTime = osGetCount() - startTime;
    nuDebConPuts(NU_DEB_CON_WINDOW1, "End\n");
    
    sprintf(text, "Resut: %s(%d)", pfsErrorMesg[rtn], rtn);
    nuDebConPuts(NU_DEB_CON_WINDOW1, text);
    
    funcTime = OS_CYCLES_TO_USEC(funcTime);
    sprintf(text, "time = %d [us]", funcTime);
    nuDebConPuts(NU_DEB_CON_WINDOW1, text);
    
    nuDebConPuts(NU_DEB_CON_WINDOW1, "\n");
    if(menuSel[menuDeps] == OS_GBPAK_POWER_OFF){
	nuDebConPuts(NU_DEB_CON_WINDOW1, "N64 Transfer Pak Power OFF");
    } else {
	nuDebConPuts(NU_DEB_CON_WINDOW1, "N64 Transfer Pak Power ON");
    }
}


/*----------------------------------------------------------------------*/
/*	func - Check nuContGBPakReadID()				*/
/*	Execute nuContGBPakReadID() and display the execution results	*/
/*	IN:	None							*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void funcReadID(void)
{
    u8	text[40];
    u32	startTime;
    u32	funcTime;
    s32 rtn;
    u8	status;
    s32 cnt;
    u8	title[20], cgbFlag;
    s32	addr;
    
    /* Check whether or not nuContGBPakOpen was executed	*/
    /* This has to be called first or it will hang	*/
    if(!gbpakInit[contNo]){
	nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_RED);
	nuDebConCPuts(NU_DEB_CON_WINDOW1, "Don't exec nuContGBPakOpen!!");
	nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_WHITE);
	return;
    }
    nuDebConCPuts(NU_DEB_CON_WINDOW1, "Exec nuContGBPakReadID...");
    
    startTime = osGetCount();
    rtn = nuContGBPakReadID(&file[contNo], &gbpakId[contNo], &status);
    funcTime = osGetCount() - startTime;
    
    nuDebConPuts(NU_DEB_CON_WINDOW1, "End\n");
    sprintf(text, "Resut: %s(%d)", pfsErrorMesg[rtn], rtn);
    nuDebConPuts(NU_DEB_CON_WINDOW1, text);
    
    funcTime = OS_CYCLES_TO_USEC(funcTime);
    sprintf(text, "time = %d [us]", funcTime);
    nuDebConPuts(NU_DEB_CON_WINDOW1, text);
    if(rtn) return;
    
    dispStatus(status);

    /*--------------------------------------------------*/
    /* 	Display the ID area					*/
    /*--------------------------------------------------*/
    nuDebConCPuts(NU_DEB_CON_WINDOW1, "\no=========( GameBoy ID )=========o");

    /*--------------------------*/
    /* Display the start address*/
    /*--------------------------*/
    addr = gbpakId[contNo].start_address/0x100
	+ ((gbpakId[contNo].start_address & 0xff) << 8);
    sprintf(text, "| StartAddr  | 0x%04X            |", addr);
    nuDebConCPuts(NU_DEB_CON_WINDOW1, text);

    /*--------------------------*/
    /* Display the game title	*/
    /*--------------------------*/
    for(cnt = 0; cnt < 15; cnt++){
	title[cnt] = gbpakId[contNo].game_title[cnt];
	if(title[cnt] == 0x00)
	    title[cnt] = ' ';
	else if(title[cnt]&0x80)
	    /* Characters after this one may be used */
	    /* for a different purpose, so do not display them. */
	    title[cnt] = 0x00; 
    }
    /* Special code if the last byte of the title is 0x80 of greater;*/
    /* otherwise, part of the title.             */
    cgbFlag = gbpakId[contNo].game_title[cnt];
    title[cnt] = 0x00;

    sprintf(text, "| Game Title | %15s",title);
    nuDebConCPuts(NU_DEB_CON_WINDOW1, text);

    switch(cgbFlag)
    {
    case CGB_COMPATIBLE: /* CGB support */
	nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_LIGTHBLUE);
	nuDebConCPuts(NU_DEB_CON_WINDOW1, "(C)");
	nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_WHITE);
	break;
    case CGB_EXCLUSIVE: /* For CGB only */
	nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_LIGHTRED);
	nuDebConCPuts(NU_DEB_CON_WINDOW1, "(C)");
	nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_WHITE);
	break;
    default:
	if(cgbFlag&0x80) /* Undefined type*/
	{
	    nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_LIGHTBLACK);
	    nuDebConCPuts(NU_DEB_CON_WINDOW1, "(U)");
	    nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_WHITE);
	}
	else /* Part of title*/
	{
	    if(cgbFlag==0x00)
		cgbFlag=' ';
	    sprintf(text, "%c  ", cgbFlag);
	    nuDebConCPuts(NU_DEB_CON_WINDOW1, text);
	}
	break;
    }
    nuDebConCPuts(NU_DEB_CON_WINDOW1, "|");

    /*--------------------------*/
    /* Display the company code	*/
    /*--------------------------*/
    sprintf(text, "| CompanyCode| 0x%02X 0x%02X "
	    , gbpakId[contNo].company_code >> 8
	    , gbpakId[contNo].company_code & 0xff);
    nuDebConCPuts(NU_DEB_CON_WINDOW1, text);
    nuDebConCPuts(NU_DEB_CON_WINDOW1, "(");
    if((gbpakId[contNo].company_code >> 8) & 0x70){
	sprintf(text, "%c", gbpakId[contNo].company_code >> 8);
	nuDebConCPuts(NU_DEB_CON_WINDOW1, text);
    } else {
	nuDebConCPuts(NU_DEB_CON_WINDOW1, "-");
    }
    
    if((gbpakId[contNo].company_code & 0xff) & 0x70){
	sprintf(text, "%c", gbpakId[contNo].company_code &0xff);
	nuDebConCPuts(NU_DEB_CON_WINDOW1, text);
    } else {
	nuDebConCPuts(NU_DEB_CON_WINDOW1, "-");
    }
    nuDebConCPuts(NU_DEB_CON_WINDOW1, ")    |");

    /*--------------------------*/
    /* Display the Body code	*/
    /*--------------------------*/
    sprintf(text, "| Body Code  | 0x%02X              |", gbpakId[contNo].body_code);
    nuDebConCPuts(NU_DEB_CON_WINDOW1, text);
    for(cnt = 0; cartTypeList[cnt].no != 0xff; cnt++){
	if(gbpakId[contNo].cart_type == cartTypeList[cnt].no) break;
    }

    /*--------------------------*/
    /* Display the cartridge (Game Pak) type	*/
    /*--------------------------*/
    sprintf(text, "| Cart Type  |%19s|", cartTypeList[cnt].name1);
    nuDebConCPuts(NU_DEB_CON_WINDOW1, text);
    sprintf(text, "|            |%19s|", cartTypeList[cnt].name2);
    nuDebConCPuts(NU_DEB_CON_WINDOW1, text);

    
    /*--------------------------*/
    /* Display the ROM/RAM size	*/
    /*--------------------------*/
    sprintf(text, "|ROM/RAM Size| %7s/%8s |",
	    romSizeList[gbpakId[contNo].rom_size],
	    ramSizeList[gbpakId[contNo].ram_size]);
    nuDebConCPuts(NU_DEB_CON_WINDOW1, text);

    /*--------------------------*/
    /* Display the destination code*/
    /*--------------------------*/
    sprintf(text, "| Destination| %8s          |", destList[gbpakId[contNo].country_code]);
    nuDebConCPuts(NU_DEB_CON_WINDOW1, text);

    
    /*--------------------------*/
    /* Display the version	*/
    /*--------------------------*/
    sprintf(text, "| Version    | 0x%02X              |", gbpakId[contNo].version);
    nuDebConCPuts(NU_DEB_CON_WINDOW1, text);

    
    /*--------------------------*/
    /* Display the checksum	*/
    /*--------------------------*/
    sprintf(text, "| iSum/Sum   | 0x%02X/0X%04X       |",
	    gbpakId[contNo].isum,
	    gbpakId[contNo].sum);
    nuDebConCPuts(NU_DEB_CON_WINDOW1, text);
    nuDebConCPuts(NU_DEB_CON_WINDOW1, "o============o===================o");
}
/*----------------------------------------------------------------------*/
/*	funcreadwrite - Check functions related to access		*/
/*	IN:	funcNo	Function to check				*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void funcreadwrite(s32 funcNo)
{
    u16 value[2];
    u16	valueIndex;
    s32 cnt;
    s32 cnt2;
    s32 rtn;
    u8	text[80];
    u32	startTime;
    u32	funcTime;
    
    /* Check whether or not nuContGBPakOpen was executed	*/
    /* This has to be called first or it will hang		*/
    if(!gbpakInit[contNo]){
	nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_RED);
	nuDebConCPuts(NU_DEB_CON_WINDOW1, "Don't exec nuContGBPakOpen!!");
	nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_WHITE);
	return;
    }

    /* Input process for read or write address and size	*/
    dispFlag = DISP_FUNCINPUTADDR;
    valueIndex = 0;
    value[0] = 0;
    value[1] = 0;
    cnt = 0;
    while(valueIndex < 2){
	nuGfxRetraceWait(1);
	nuContDataGetExAll(contData);
	cnt++;
	nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_WHITE);
	nuDebConTextPos(NU_DEB_CON_WINDOW1, 2, valueIndex + 1);
	if(!valueIndex){
	    if(funcNo == 4 || funcNo == 7){
		sprintf(text, "Input Read Address:  0X%04X\r", value[valueIndex]);
	    } else {
		sprintf(text, "Input Write Address: 0X%04X\r", value[valueIndex]);
	    }
	} else {
	    if((funcNo == 4) || (funcNo == 7)){
		sprintf(text, "Input Read Size:     0X%04X\r", value[valueIndex]);
	    } else {
		sprintf(text, "Input Write Size:    0X%04X\r", value[valueIndex]);
	    }
	}
	nuDebConCPuts(NU_DEB_CON_WINDOW1, text);
	if(!(cnt & 0x03)){
	    
	    if(contData[0].button & L_JPAD){
		value[valueIndex] +=0x100;
	    }
	    if(contData[0].button & R_JPAD){
		value[valueIndex] -=0x100;
	    }
	    if(contData[0].button & U_JPAD){
		value[valueIndex] +=0x20;
	    }
	    if(contData[0].button & D_JPAD){
		value[valueIndex] -=0x20;
	    }
	}
	if(contData[0].trigger & START_BUTTON ){
	    value[valueIndex] = 0x0000;
	}
	if(contData[0].trigger & A_BUTTON ){
	    valueIndex++;
	}
	if(contData[0].trigger & L_TRIG){
	    value[valueIndex] +=0x1;
	}
	if(contData[0].trigger & R_TRIG){
	    value[valueIndex] -=0x1;
	}
    }
    
    switch(funcNo){
    case 4:
	nuDebConCPuts(NU_DEB_CON_WINDOW1, "\n\nExec nuContGBPakRead...");
	startTime = osGetCount();
	rtn = nuContGBPakRead(&file[contNo], value[0], gbData, value[1]);
	funcTime = osGetCount() - startTime;
	break;
    case 5:
	nuDebConCPuts(NU_DEB_CON_WINDOW1, "\n\nExec nuContGBPakWrite...");
	startTime = osGetCount();
	rtn = nuContGBPakWrite(&file[contNo], value[0], gbData, value[1]);
	funcTime = osGetCount() - startTime;
	break;
    case 7:
	nuDebConCPuts(NU_DEB_CON_WINDOW1, "\n\nExec nuContGBPakFread...");
	startTime = osGetCount();
	rtn = nuContGBPakFread(&file[contNo], value[0], gbData, value[1]);
	funcTime = osGetCount() - startTime;
	break;
    case 8:
	nuDebConCPuts(NU_DEB_CON_WINDOW1, "\n\nExec nuContGBPakFwrite...");
	startTime = osGetCount();
	rtn = nuContGBPakFwrite(&file[contNo], value[0], gbData, value[1]);
	funcTime = osGetCount() - startTime;
	break;
    default:
	return;
    }
    
    nuDebConPuts(NU_DEB_CON_WINDOW1, "End\n");
    sprintf(text, "Resut: %s(%d)", pfsErrorMesg[rtn], rtn);
    nuDebConPuts(NU_DEB_CON_WINDOW1, text);	
    funcTime = OS_CYCLES_TO_USEC(funcTime);
    sprintf(text, "time = %d [us]", funcTime);
    nuDebConPuts(NU_DEB_CON_WINDOW1, text);
    
    nuDebConClear(NU_DEB_CON_WINDOW2);	
    nuDebConWindowPos(NU_DEB_CON_WINDOW2,  3 * 8, 15 * 8);
    nuDebConWindowSize(NU_DEB_CON_WINDOW2, 40 - 6 , 13);
    nuDebConWindowShow(NU_DEB_CON_WINDOW2, NU_DEB_CON_WINDOW_ON);
    
    
    for(cnt = 0 ; cnt < value[1]; cnt += 8){
	nuGfxRetraceWait(1);
	sprintf(text, "%04X|", value[0] + cnt);
	nuDebConCPuts(NU_DEB_CON_WINDOW2, text);
	for(cnt2 = 0; cnt2 < 8; cnt2++){
	    sprintf(text, " %02X", gbData[cnt+cnt2]);
	    nuDebConCPuts(NU_DEB_CON_WINDOW2, text);
	    if((cnt + cnt2) >value[1]) break;
	}
	nuDebConCPuts(NU_DEB_CON_WINDOW2, "\n");
    }
    
}
/*----------------------------------------------------------------------*/
/*	funccheckconnector - Check nuContGBPakCheckConnector		*/
/*	IN:	None							*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void funccheckconnector(void)
{
    s32 rtn;
    u8	text[80];
    u32	startTime;
    u32	funcTime;
    u8	status;
    
    /* Check whether or not nuContGBPakOpen was executed	*/
    /* This has to be called first or it will hang	*/
    if(!gbpakInit[contNo]){
	nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_RED);
	nuDebConCPuts(NU_DEB_CON_WINDOW1, "Don't exec nuContGBPakOpen!!");
	nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_WHITE);
	return;
    }
    
    nuDebConCPuts(NU_DEB_CON_WINDOW1, "Exec nuContGBPakCheckConnector...");
    startTime = osGetCount();
    rtn = nuContGBPakCheckConnector(&file[contNo], &status);
    funcTime = osGetCount() - startTime;
    nuDebConPuts(NU_DEB_CON_WINDOW1, "End\n");
    
    sprintf(text, "Resut: %s(%d)", pfsErrorMesg[rtn], rtn);
    nuDebConPuts(NU_DEB_CON_WINDOW1, text);	
    funcTime = OS_CYCLES_TO_USEC(funcTime);
    sprintf(text, "time = %d [us]", funcTime);
    nuDebConPuts(NU_DEB_CON_WINDOW1, text);
    
    nuDebConCPuts(NU_DEB_CON_WINDOW1, "------------< Status >------------");
    if(status & OS_GBPAK_GBCART_ON){
	nuDebConPuts(NU_DEB_CON_WINDOW1, "GBCART_ON");
    } else {
	nuDebConPuts(NU_DEB_CON_WINDOW1, "NO GBCART");
    }
    if(status & OS_GBPAK_GBCART_PULL){
	nuDebConPuts(NU_DEB_CON_WINDOW1, "GBCART_PULL");
    }
    if(status & OS_GBPAK_POWER){
	nuDebConPuts(NU_DEB_CON_WINDOW1, "GBCART_POWER ON");
    } else {
	nuDebConPuts(NU_DEB_CON_WINDOW1, "GBCART_POWER OFF");
    }
}

/*----------------------------------------------------------------------*/
/*	funcregwrite - Check the nuContGBPakRegWrite function		*/
/*	IN:	None							*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void funcregwrite(void)
{
    s32 rtn;
    u8	text[80];
    u32	startTime;
    u32	funcTime;
    u16	value[2];
    u16	valueIndex;
    u16	valueScale;
    s32	cnt;
    
    /* Check whether or not nuContGBPakOpen was executed	*/
    /* This has to be called first or it will hang	*/
    if(!gbpakInit[contNo]){
	nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_RED);
	nuDebConCPuts(NU_DEB_CON_WINDOW1, "Don't exec nuContGBPakOpen!!");
	nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_WHITE);
	return;
    }

    cnt = 0;
    value[0] = 0;
    value[1] = 0;
    valueIndex = 0;

    while(valueIndex < 2){
	nuGfxRetraceWait(1);
	nuContDataGetExAll(contData);
	cnt++;
	nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_WHITE);
	nuDebConTextPos(NU_DEB_CON_WINDOW1, 2, valueIndex + 1);
	if(!valueIndex){
	    valueScale = 0x1;
	    sprintf(text, "Input Regster Address:  0X%04X\r", value[valueIndex]);
	} else {
	    valueScale = 0x100;
	    value[1] &= 0xff;
	    sprintf(text, "Input Write Data:       0X%02X\r", value[valueIndex]);
	}
	nuDebConCPuts(NU_DEB_CON_WINDOW1, text);
	if(!(cnt & 0x03)){
	    
	    if(contData[0].button & L_JPAD){
		value[valueIndex] +=0x1000/valueScale;
	    }
	    if(contData[0].button & R_JPAD){
		value[valueIndex] -=0x1000/valueScale;
	    }
	    if(contData[0].button & U_JPAD){
		value[valueIndex] +=0x100/valueScale;
	    }
	    if(contData[0].button & D_JPAD){
		value[valueIndex] -=0x100/valueScale;
	    }
	}
	if(contData[0].trigger & START_BUTTON ){
	    value[valueIndex] = 0x0000;
	}
	if(contData[0].trigger & A_BUTTON ){
	    valueIndex++;
	}

    }
    
    nuDebConCPuts(NU_DEB_CON_WINDOW1, "\n\nExec nuContGBPakRegWrite...");
    startTime = osGetCount();
    rtn = nuContGBPakRegWrite(&file[contNo], value[0], value[1]);
    funcTime = osGetCount() - startTime;
    nuDebConPuts(NU_DEB_CON_WINDOW1, "End\n");
    sprintf(text, "Resut: %s(%d)", pfsErrorMesg[rtn], rtn);
    nuDebConPuts(NU_DEB_CON_WINDOW1, text);	
    funcTime = OS_CYCLES_TO_USEC(funcTime);
    sprintf(text, "time = %d [us]", funcTime);
    nuDebConPuts(NU_DEB_CON_WINDOW1, text);
    
}

/*----------------------------------------------------------------------*/
/*	funccheck - Check functions related to NuSystem 64GBPak		*/
/*	IN:	funcNo	Function to check				*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void funccheck(s32 funcNo)
{
    
    nuDebConWindowShow(NU_DEB_CON_WINDOW1, NU_DEB_CON_WINDOW_ON);
    nuDebConWindowPos(NU_DEB_CON_WINDOW1,  3 * 8, 5 * 8);
    nuDebConWindowSize(NU_DEB_CON_WINDOW1, 40 - 6 , 22);

    /* Execute the selected function	*/
    switch(funcNo){
    case 0:	/* nuContGBPakOpen */
	funcOpen();
	break;
	
    case 1:	/* nuContGBPakGetStatus */
	funcGetStatus();
	break;
	
    case 2:	/* Power Control*/
	funcPower();
	break;
    case 3:	/* Read ID	*/
	funcReadID();
	break;
    case 4:	/* nuContGBPakRead 	*/
    case 5:	/* nuContGBPakWrite 	*/
    case 7:	/* nuContGBPakFread	*/
    case 8:	/* nuCOntGBPakFwrite	*/
	funcreadwrite( funcNo);
	break;
    case 6: /* nuContGBPakCheckConnector */
	funccheckconnector();
	break;
    case 9: /* nuContGBPakRegWrite */
	funcregwrite();
	break;
    }	
    
    nuDebConTextAttr(NU_DEB_CON_WINDOW1, NU_DEB_CON_ATTR_BLINK);
    nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_RED);
    nuDebConTextPos(NU_DEB_CON_WINDOW1, 7, 20);
    nuDebConPuts(NU_DEB_CON_WINDOW1, "PRESS A or B BUTTON");
    nuDebConTextColor(NU_DEB_CON_WINDOW1, NU_DEB_CON_TEXT_WHITE);
    nuDebConTextAttr(NU_DEB_CON_WINDOW1, NU_DEB_CON_ATTR_NORMAL);
    while(1){
	nuGfxRetraceWait(1);
	/* Read the controller data */
	nuContDataGetExAll(contData);
	if(contData[0].trigger & (A_BUTTON | B_BUTTON)){
	    break;
	}
   }
    nuDebConClear(NU_DEB_CON_WINDOW1);
    nuDebConWindowShow(NU_DEB_CON_WINDOW1, NU_DEB_CON_WINDOW_OFF);
    nuDebConWindowShow(NU_DEB_CON_WINDOW2, NU_DEB_CON_WINDOW_OFF);
}
