/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 2

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

  u32  pad_no = 0;
  u32  endFlag = 0;

  /* create message queue for VI retrace */
  osCreateMesgQueue(&msgQ, msgbuf, MAX_MESGS);

  /* add client to scheduler */
  nnScAddClient(sched, &client, &msgQ);



  /* initialize variables */
  mtx_cont[0].pos_x = 0.0;
  mtx_cont[0].pos_y = 0.0;
  mtx_cont[0].pos_z = 400.0;
  mtx_cont[0].size =  2.0;
  mtx_cont[0].rot_x = 0.0;
  mtx_cont[0].rot_y = 0.0;
  mtx_cont[0].rot_z = 0.0;


  /* read display list */
  auRomRead((u32)_lodmodelSegmentRomStart, gfx_dlist_buf[0],
	    (u32)_lodmodelSegmentRomEnd - (u32)_lodmodelSegmentRomStart);

  /* read sequence data */
  auSeqPlayerSetFile(1,0);
  auSeqPlayerPlay(1);

  /* Display only the number of controllers that are connected */
  graphic_no = GFX_00;




  while(!endFlag){
    (void) osRecvMesg(&msgQ, NULL, OS_MESG_BLOCK);

    /*
      Move object.
      Move object in direction away from viewpoint.
      If out of  field, return to original position.
     */
    mtx_cont[0].pos_z -=5.0;
    if(mtx_cont[0].pos_z <( 600-2000)){
      mtx_cont[0].pos_z = 600.0;
    }

    /* + key UP*/
    if(controllerdata_ptr[0]->button & U_JPAD){
      mtx_cont[0].pos_z-=10.0;
    }
    
    /*  + key DOWN */
    if(controllerdata_ptr[0]->button & D_JPAD){
      mtx_cont[0].pos_z+=10.0;
      
    }

    /*  + key LEFT */
    if(controllerdata_ptr[0]->button & L_JPAD){
      mtx_cont[0].pos_x-=1.0;
    }
    
    /* + key RIGHT */
    if(controllerdata_ptr[0]->button & R_JPAD){
      mtx_cont[0].pos_x+=1.0;
    }
    
    /* START BUTTON */
    if(controllerdataTriger[0].button & START_BUTTON){
	endFlag++;
    }

    /* 3D Stick */
    mtx_cont[0].rot_x += controllerdata_ptr[pad_no]->stick_y/8;
    mtx_cont[0].rot_y += controllerdata_ptr[pad_no]->stick_x/8;
    mtx_cont[0].rot_x += 1.0;
    mtx_cont[0].rot_y += 1.0;


  }


  /* stop sequence */
  auSeqPlayerStop(1);

  graphic_no = GFX_NULL;


  /*  wait for end of graphics task/audio task */
  while((auSeqPlayerState(0) != AL_STOPPED) || (pendingGFX != 0)){
    (void) osRecvMesg(&msgQ, NULL, OS_MESG_BLOCK);
  };

  /* remove client to scheduler */
  nnScRemoveClient(sched, &client);
  
  /* exit label */
  return MAIN_00;
}




