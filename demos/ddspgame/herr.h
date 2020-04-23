
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: herr.h,v $
        $Revision: 1.1 $
        $Date: 1998/11/13 12:39:08 $
 *---------------------------------------------------------------------*/

#ifndef __ERR_H__
#define __ERR_H__

extern void scr_error(Gfx **gpp);
extern void err_print(void);
extern void err_set(s32 no);
extern void rtc_noReadMode(void);


enum ERROR_MESSAGE {
/* 0 */   ES_SEIJO_SHURYO,
/* 1 */   ES_IJO_SHURYO,
/* 2 */   ES_KYOSEI_SHURYO,
/* 3 */   ES_ERROR_NUMBER,
/* 4 */   ES_TORIATUKAI_SETUMEISHO,
/* 5 */   ES_ACCESS_LAMP,
/* 6 */   ES_DISK_WO_NUKANAIDE,
/* 7 */   ES_DISK_GA_SOUNYUUSARETE,
/* 8 */   ES_KUDASAI,
/* 9 */   ES_DISK_GA_HAITTEMASUKA,
/* 10 */  ES_DISK_WO_SASHIKONDE,
/* 11 */  ES_MACHIGATTA_DISK,
/* 12 */  ES_KANOSEI_GA_ARIMASU,
/* 13 */  ES_JIKOKU_WO_SETTEI,
/* 14 */  ES_KUWASIKU_HA_SETUMEISHO,
/* 15 */  ES_OYOMIKUDASAI,
/* 16 */  ES_KAKCHO_RAM_PAK,
/* 17 */  ES_SASHIKONNDE_ARIMASUKA,
/* 18 */  ES_GENZAI_SAKUSEICHU,
/* 19 */  ES_ZENKAI_DATA_GA,
/* 20 */  ES_SAVE_DEKIMASEN,
/* 21 */  ES_DATA_WO_SHOKIKA,
/* 22 */  ES_A_BUTTON_WO_OSHITE,
/* 23 */  ES_DISK_WO_TORIDASHITE,
/* 24 */  ES_TADASHII_DISK_NI,
/* 25 */  ES_DISK_WO_SASHIKOMINAOSHITE
};

 
#endif
