/*---------------------------------------------------------------------
 	Copyright (C) 1998 Nintendo.
 	
 	File		main.c
 	Coded    by	Tetsuyuki Ootsuka.	July, 1998.
 	Comments	Voice Recognition System Demo Program
   
 	$Id: main.c,v 1.1.1.2 2002/10/29 08:06:05 blythe Exp $
   ---------------------------------------------------------------------*/
/**************************************************************************
 *
 *  $Revision: 1.1.1.2 $
 *  $Date: 2002/10/29 08:06:05 $
 *  $Source: 
 *
 **************************************************************************/
#include <ultra64.h>
#include "nu64sys.h"
#include "graph.h"
#include "action.h"
#include "siproc.h"

#include "src/main/mes.h"

#define TITLE_MES_X     40
#define TITLE_MES_Y     20
#define STATUS_X        40
#define STATUS_Y        60
#define WORD_X          48
#define WORD_Y          64
#define READY_X         40
#define READY_Y         180
#define ERR_X           8
#define ERR_Y           100
#define WARN_X          40
#define WARN_Y          200

OSVoiceData	result;
OSVoiceHandle	vhd;

u16		*gp;
u8		draw_buffer = 1;

static OSThread siThread;
static u64      siStack[STACKSIZE/sizeof(u64)];
static OSMesg   sendMsgBuf[SI_MSG_NUM];
OSMesgQueue     sendMsgQ;

     
/*---------------------------------------------------------------------
                  main
  ---------------------------------------------------------------------*/

void
mainproc(void) {

  u32		i;
  u8    	mode, si_mode;
  s32   	ret;
  u16		color;
  u16		delta_x;
  u8		tmp[16];
  sendMsg 	sendMesgBuff;
  sendMsg	*sendMesg = &sendMesgBuff;
  retMsg 	dummyBuff;
  retMsg 	*dummy = &dummyBuff;


  /*-- SIスレッドの初期化 --*/

  osCreateMesgQueue(&sendMsgQ, sendMsgBuf, SI_MSG_NUM);
  osCreateThread(&siThread, 8, (void *)siproc, 0,
		 siStack+STACKSIZE/sizeof(u64), 12);
  osStartThread(&siThread);

  ret = 0;
  mode = InitMode;
  si_mode = VOICE_END;

  delta_x = 0;
  result.answer_num = 0;
  vhd.cmd_status = 0xff;


  /*-- メインループ --*/

  while(1){


    /*-- フレームバッファをライドバック --*/
    
    osWritebackDCache(cfb[draw_buffer], 2*SCREEN_WD*SCREEN_HT);

    /*-- フレームバッファへのポインタを登録 --*/

    osViSwapBuffer(cfb[draw_buffer]);
    
    /*-- 垂直リトレース待ち --*/

    while(osRecvMesg(&retraceMessageQ, NULL, OS_MESG_NOBLOCK) == 0);
    osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);

    /*-- フレームバッファポインタの設定 --*/

    draw_buffer ^= 1;
    gp = (u16 *)cfb[draw_buffer];
    gcls(gp);



    /***** コントローラデータの取得(SIスレッドを実行) *****/

    if((si_mode == VOICE_END) || (mode == NoContMode && si_mode == CONTR_END)){
      si_mode = CONTR_START;
      sendMesg->mode = mode;
      sendMesg->si_mode = si_mode;
      osSendMesg(&sendMsgQ, (OSMesg)sendMesg, OS_MESG_NOBLOCK);
    }
    
    /*-- コントローラの状態取得 --*/

    if(si_mode == CONTR_START){
      if(osRecvMesg(&retMsgQ, (OSMesg *)&dummy, OS_MESG_NOBLOCK) == 0){
	si_mode = dummy->si_mode;
      }
    }
    
    if(si_mode == CONTR_END){
      
      for(i = 0;i < MAXCONTROLLERS;i++){

	/*-- 再度、初期化からスタート --*/

	if(Ac.pad[i].push & A_BUTTON){
	  mode = InitMode;
	  result.answer_num = 0;
	  break;
	}

	/*-- 認識処理を停止 --*/

	if((Ac.pad[i].push & B_BUTTON) && (mode == GetReadMode)){
	  mode = StopReadMode;
	  break;
	}
      
      }
    
    }
    

    /***** 音声認識処理開始(SIスレッドを実行) *****/

    if((si_mode == CONTR_END) && (mode != NoContMode)){
      si_mode = VOICE_START;
      sendMesg->mode = mode;
      sendMesg->si_mode = si_mode;
      osSendMesg(&sendMsgQ, (OSMesg)sendMesg, OS_MESG_NOBLOCK);
    }
    
    /*-- 音声認識処理終了 --*/

    if(si_mode == VOICE_START){
      if(osRecvMesg(&retMsgQ, (OSMesg *)&dummy, OS_MESG_NOBLOCK) == 0){
	ret = dummy->ret;
	mode = dummy->mode;
	si_mode = dummy->si_mode;
      }
    }
    

    /***** 音声認識結果表示 *****/

    printkanji(gp, TITLE_MES_X, TITLE_MES_Y, WHITE, mes0);


    /*-- 処理の停止表示 --*/

    if(mode == NoContMode){
      printkanji(gp, WARN_X, WARN_Y, YELLOW, mes1);
      prints(WARN_X+16*4, WARN_Y+16, RED, "-- Push A Button ! --");
    }

    
    /*-- エラー結果表示 --*/

    if(ret == CONT_ERR_NO_CONTROLLER){
      printkanji(gp, ERR_X+80, ERR_Y, RED, mes2);
      printkanji(gp, ERR_X+80, ERR_Y+16, RED, mes3);
      prints(ERR_X+56, ERR_Y+48, RED, "(CONT_ERR_NO_CONTROLLER)");
      mode = NoContMode;
      continue;
    }
    else if(ret == CONT_ERR_DEVICE){
      printkanji(gp, ERR_X+40, ERR_Y, RED, mes4);
      printkanji(gp, ERR_X+88, ERR_Y+16, RED, mes5);
      prints(ERR_X+84, ERR_Y+48, RED, "(CONT_ERR_DEVICE)");
      mode = NoContMode;
      continue;
    }
    else if(ret == CONT_ERR_VOICE_NO_RESPONSE){
      printkanji(gp, ERR_X+64, ERR_Y, RED, mes6);
      printkanji(gp, ERR_X+88, ERR_Y+16, RED, mes7);
      prints(ERR_X+40, ERR_Y+48, RED, "(CONT_ERR_VOICE_NO_RESPONSE)");
      mode = NoContMode;
      continue;
    }
    else if(ret == CONT_ERR_CONTRFAIL){
      printkanji(gp, ERR_X+72, ERR_Y, RED, mes8);
      prints(ERR_X+72, ERR_Y+32, RED, "(CONT_ERR_CONTRFAIL)");
      mode = NoContMode;
      continue;
    }
    else if(ret == CONT_ERR_INVALID){
      printkanji(gp, ERR_X+80, ERR_Y, RED, mes9);
      prints(ERR_X+80, ERR_Y+32, RED, "(CONT_ERR_INVALID)");
      mode = NoContMode;
      continue;
    }
    else if(ret == CONT_ERR_VOICE_WORD){
      printkanji(gp, ERR_X+80, ERR_Y, RED, mes10);
      printkanji(gp, ERR_X+72, ERR_Y+16, RED, mes11);
      prints(ERR_X+68, ERR_Y+48, RED, "(CONT_ERR_VOICE_WORD)");
      mode = NoContMode;
      continue;
    }
    else if(ret == CONT_ERR_VOICE_MEMORY){
      printkanji(gp, ERR_X+48, ERR_Y, RED, mes12);
      prints(ERR_X+60, ERR_Y+32, RED, "(CONT_ERR_VOICE_MEMORY)");
      mode = NoContMode;
    }
    else if(ret == CONT_ERR_NOT_READY){

      if((vhd.cmd_status == VOICE_STATUS_START) || (vhd.cmd_status == VOICE_STATUS_CANCEL)){
	printkanji(gp, READY_X, READY_Y, CYAN, mes13);
	mode = GetReadMode;
      }
      
    }

    
    /*-- 認識結果表示 --*/

    if(result.answer_num != 0){

      /*-- ウォーニング --*/

      if(result.warning != 0){

	delta_x = 0;
	
	if(result.warning & VOICE_WARN_TOO_NOISY){
	  printkanji(gp, WARN_X, WARN_Y, YELLOW, mes14);
	  delta_x += 64;
	}
	if(result.warning & VOICE_WARN_NOT_FIT){
	  printkanji(gp, WARN_X+delta_x, WARN_Y, YELLOW, mes15);
          delta_x += 64;
	}
	if(result.warning & VOICE_WARN_TOO_LARGE){
	  printkanji(gp, WARN_X+delta_x, WARN_Y, YELLOW, mes16);
          delta_x += 64;
	}
	if(result.warning & VOICE_WARN_TOO_SMALL){
	  printkanji(gp, WARN_X+delta_x, WARN_Y, YELLOW, mes17);
	}

      }
      
      /*-- 認識単語表示 --*/

      color = WHITE;
      
      for(i = 0;i < 5;i++){

	if(result.answer[i] == 0x7fff){
	  break;
	}
	
        if(i >= result.answer_num){
	  color = YELLOW;
	}
	
	printkanji(gp, WORD_X, (int)(WORD_Y+16*(i+1)), color, mes[result.answer[i]]);
	sprintf(tmp, "%4d", result.distance[i]);
	prints(WORD_X+16*12, (int)(WORD_Y+16*(i+1)), color, tmp);

      }

    }
    

    /*-- ステータス表示 --*/

    switch(vhd.cmd_status){
      
    case	VOICE_STATUS_READY:
      
      printkanji(gp, STATUS_X, STATUS_Y, WHITE, mes18);
      break;
      
    case        VOICE_STATUS_START:

      printkanji(gp, STATUS_X, STATUS_Y, WHITE, mes19);
      break;

    case        VOICE_STATUS_CANCEL:

      result.answer_num = 0;
      
      printkanji(gp, STATUS_X, STATUS_Y, WHITE, mes20);
      break;

    case        VOICE_STATUS_BUSY:

      printkanji(gp, STATUS_X, STATUS_Y, WHITE, mes21);
      break;

    case        VOICE_STATUS_END:

      printkanji(gp, STATUS_X, STATUS_Y, WHITE, mes22);
      break;
      
    }

  }
  
}

