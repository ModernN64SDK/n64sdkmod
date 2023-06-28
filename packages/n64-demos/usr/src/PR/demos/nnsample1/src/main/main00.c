/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 1

		Copyright (C) 1997, NINTENDO Co., Ltd.

============================================================================*/

#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>

#include "def.h"
#include "vram.h"
#include "segment.h"
#include "message.h"
#include "nnsched.h"
#include "main.h"
#include "audio.h"
#include "controller.h"
#include "graphic.h"


ModelMtx mtx_cont[4];


/*--------------------------
  main00 test stage

  return: next stage no
--------------------------*/
int main00(NNSched* sched)
{

  OSMesgQueue msgQ;
  OSMesg      msgbuf[MAX_MESGS];
  NNScClient client;
  u32  sndno=0;
  u32  pad_no = 0;
  u32  cnt;
  u32  controller_num;
  u32  endFlag = 0;
  /* create message queue for VI retrace */
  osCreateMesgQueue(&msgQ, msgbuf, MAX_MESGS);

  /* set sequence 0 in sequence player 0*/
  auSeqPlayerSetFile(0,0);

  /* add client to shceduler */
  nnScAddClient(sched, &client, &msgQ);

  controller_num = numControllers;

  /* initialize variables */
  for(cnt = 0; cnt < controller_num; cnt++){
    mtx_cont[cnt].pos_x = 80.0-cnt*80.0;
    mtx_cont[cnt].pos_y = -80.0+cnt*80.0;
    mtx_cont[cnt].pos_z = 0.0+cnt*80.0;
    mtx_cont[cnt].size =  0.3;
    mtx_cont[cnt].rot_x = 0.0;
    mtx_cont[cnt].rot_y = 0.0;
    mtx_cont[cnt].rot_z = 0.0;
  }

  /* read display list */
  auRomRead((u32)_contSegmentRomStart, gfx_dlist_buf[0],
	    (u32)_contSegmentRomEnd - (u32)_contSegmentRomStart);


  /* Display only the number of controllers that are connected */
  auSeqPlayerPlay(0);
  graphic_no = GFX_00;


  while(!endFlag){
    (void) osRecvMesg(&msgQ, NULL, OS_MESG_BLOCK);

    /* Loop only the number of controllers being pushed */
    for(pad_no = 0; pad_no < controller_num ; pad_no++){
      if(controllerdataTriger[pad_no].button & B_BUTTON){
	/* playback if sequence has stopped */
	if(auSeqPlayerState(0) == AL_STOPPED){
	  auSeqPlayerPlay(0);
	}
      }
      /* Play a sound */
      if(controllerdataTriger[pad_no].button & A_BUTTON){
	auSndPlay(sndno);
	sndno=(sndno++) & 0x07;
      }

      /*  + key up */
      if(controllerdata_ptr[pad_no]->button & U_JPAD){
	mtx_cont[pad_no].pos_z-=1.0;
      }

      /*  + key down */
      if(controllerdata_ptr[pad_no]->button & D_JPAD){
	mtx_cont[pad_no].pos_z+=1.0;
      }

      /* + key left */
      if(controllerdata_ptr[pad_no]->button & L_JPAD){
	mtx_cont[pad_no].pos_x-=1.0;
      }

      /* + key right */
      if(controllerdata_ptr[pad_no]->button & R_JPAD){
	mtx_cont[pad_no].pos_x+=1.0;
      }

      /* C-UP */
      if(controllerdata_ptr[pad_no]->button & U_CBUTTONS){
	mtx_cont[pad_no].size+=0.01;
      }

      /* C-DOWN */
      if(controllerdata_ptr[pad_no]->button & D_CBUTTONS){
	mtx_cont[pad_no].size-=0.01;
      }

      /* START BUTTON */
      if(controllerdataTriger[pad_no].button & START_BUTTON){
	  endFlag++;
      }

      /* 3D Stick */
      mtx_cont[pad_no].rot_x += controllerdata_ptr[pad_no]->stick_y/8;
      mtx_cont[pad_no].rot_y += controllerdata_ptr[pad_no]->stick_x/8;

      /* steady rotation */
      mtx_cont[pad_no].rot_x += 1;
      mtx_cont[pad_no].rot_y += 1;
    }

  }



  /* stop sequence */
  auSeqPlayerStop(0);
  graphic_no = GFX_NULL;

  /* wait for end of graphic task and audio task */
  while((auSeqPlayerState(0) != AL_STOPPED) || (pendingGFX != 0)){
    (void) osRecvMesg(&msgQ, NULL, OS_MESG_BLOCK);
  };

  /* remove client to scheduler */
  nnScRemoveClient(sched, &client);
  
  /* exit label */
  return MAIN_01;
}




