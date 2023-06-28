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
#include "main01.h"

/* Game subject parameters */
CFiter fiter[2];

/*
  Correction value for bullet direction.
   Correction value for when four bullets shot in order from different positions.
*/
short gun_pos[4][3] = {
  { 14,  4, 7},
  {-10, -4, 7},
  { 10, -4, 7},
  {-14,  4, 7},
};

u32 threadpri_flag = 0;


void InitFiter(CFiter *);
void fiter_move(CFiter *,int);
void bullet_move(CBullet *);
void lookat_move(int );

/* external variable declaration */
extern Gfx fiter_1_dl[];
extern Gfx fiter_2_dl[];

/*--------------------------
  main01 test stage

  return: next stage no
--------------------------*/
int main01(NNSched* sched)
{

  OSMesgQueue  msgQ;
  OSMesg       msgbuf[MAX_MESGS];
  NNScClient   client;
  u32          controller_num;
  u32          endflag;
  u32          cnt;


  if(numControllers > 2){
    controller_num = 2;
  } else {
    controller_num = numControllers;
  }

  /* create message queue for VI retrace */
  osCreateMesgQueue(&msgQ, msgbuf, MAX_MESGS);

  /* add client to scheduler */
  nnScAddClient(sched, &client, &msgQ);

  /* initialize game subject parameters */
  InitFiter(&fiter[0]);  /* #1 machine */
  fiter[0].pos.x = 45.0;
  fiter[0].dlist_ptr = fiter_1_dl;        /*set display list */
  fiter[0].dlist_buf = (Gfx*)gfx_dlist_buf[0]; /* display list buffer */

  /* read display list */
  auRomRead((u32)_fiter1SegmentRomStart, gfx_dlist_buf[0],
	    (u32)_fiter1SegmentRomEnd - (u32)_fiter1SegmentRomStart);

  InitFiter(&fiter[1]); /* #2 machine */
  fiter[1].pos.x = 360.0-45.0;
  fiter[1].dlist_ptr = fiter_2_dl;  /* set display list */
  fiter[1].dlist_buf = (Gfx*)gfx_dlist_buf[1]; /* display list buffer */

  /* read display list */
  auRomRead((u32)_fiter2SegmentRomStart, gfx_dlist_buf[1],
	    (u32)_fiter2SegmentRomEnd - (u32)_fiter2SegmentRomStart);

  /* viewpoint settings */
  lookat.eye_x = 00.0;
  lookat.eye_y = 0.0;
  lookat.eye_z = -20.0;
  lookat.at_x = 0.0;
  lookat.at_y = 0.0;
  lookat.at_z = 0.0;
  lookat.up_x = 0.0;
  lookat.up_y = 1.0;
  lookat.up_z = 0.0;
  lookat.pitch = 5.0;
  lookat.yaw =  0.0;
  lookat.roll = 0.0;
  lookat.dist = -40.0;

  /* read sequence data */
  auSeqPlayerSetFile(1,0);
  auSeqPlayerPlay(1);


  /* start graphic display */
  graphic_no = GFX_01;
  endflag =0;

  while(!endflag){
    (void) osRecvMesg(&msgQ, NULL, OS_MESG_BLOCK);

    for(cnt = 0; cnt < controller_num; cnt++){

      fiter_move(&fiter[cnt],cnt);
      lookat_move(cnt);

      /* end on Start button push */
      if(controllerdataTriger[cnt].button & START_BUTTON){
	 endflag++;
       }

      /* raise priority of graphic thread */
      if(controllerdata_ptr[cnt]->button & Z_TRIG){
	 threadpri_flag = 1;
      } else {
	threadpri_flag = 0;
      }
    }
  }

  /* stop sequence */
  auSeqPlayerStop(1);
  graphic_no = GFX_NULL;

  /* wait for end of graphic task /audio task */
  while((auSeqPlayerState(0) != AL_STOPPED) || (pendingGFX != 0)){
    (void) osRecvMesg(&msgQ, NULL, OS_MESG_BLOCK);
  };

  /* remove client to scheduler */
  nnScRemoveClient(sched, &client);

  return MAIN_00;
}

/*-----------------------------------------------------------------------------
  Initialize fiter parameters
  CFiter *fiter : fiter structure pointer  
-----------------------------------------------------------------------------*/
void InitFiter(CFiter *fiter)
{
  u32 cnt;

  fiter->pos.x = 0.0;
  fiter->pos.y = -40.0;
  fiter->pos.z = 0.0;
  fiter->pos.roll = 0.0;
  fiter->pos.pitch = 0.0;
  fiter->pos.yaw =  0.0;
  fiter->pos.size = 0.1;
  fiter->flag = FITER_FLAG_START;
  fiter->bullet_cnt = 0;

  /* initialize bullet status */
  for(cnt =0; cnt < FITER_BULLET_NUM; cnt++){
    fiter->bullet[cnt].flag = FITER_BULLET_FLAG_NONE;
  }
}

/*-----------------------------------------------------------------------------
  Fitr movements
  CFiter *fiter  :   fiter structure pointer
  int pad_no    :   controller number
-----------------------------------------------------------------------------*/
void fiter_move(CFiter *fiter,int pad_no)
{

  int pos_x;
  float pos_z;
  int cnt;
  int bullet_cnt;


  switch(fiter->flag){
    
  case FITER_FLAG_START:   /*  From out of screen to within  */
    if(fiter->pos.z < 40.0){
      fiter->pos.z += 3.0;
    } else {
      fiter->pos.z = 40.0;
      fiter->flag = FITER_FLAG_PLAYER;
    }
    break;

  case FITER_FLAG_PLAYER:  /* Normal state.  Controller process */

    /* Fiter's Z axis movement */
    pos_z = fiter->pos.z;
    pos_z += controllerdata_ptr[pad_no]->stick_y/10;
    if(pos_z < 0.0) {
      fiter->pos.z = 0.0;
    } else if( pos_z >500.0){
      fiter->pos.z = 500.0;
    } else {
      fiter->pos.z = pos_z;
    }

    /* Fiter's X axis movement (rotation angle) */
    pos_x = fiter->pos.x;
    pos_x -= controllerdata_ptr[pad_no]->stick_x/10;
    fiter->pos.x = (float)(pos_x % 360);
    
    /* Fiter's bullet movement */
    for(cnt = 0; cnt < FITER_BULLET_NUM; cnt++){
      bullet_move(&fiter->bullet[cnt]);
    }

    /* bullet shot */
    if(controllerdataTriger[pad_no].button & A_BUTTON){
      /* 
	 Free buffer search.   More efficient with list structure, but
               when bullet number is small, a round robin search is sufficient.
      */
      for(cnt = 0; cnt < FITER_BULLET_NUM; cnt++){
	if(fiter->bullet[cnt].flag == FITER_BULLET_FLAG_NONE){
	  /* */
	  bullet_cnt = fiter->bullet_cnt;
	  fiter->bullet_cnt++;
	  fiter->bullet_cnt &= 0x03;
	  fiter->bullet[cnt].flag = FITER_BULLET_FLAG_FIRE;
	  fiter->bullet[cnt].pos.x = fiter->pos.x + gun_pos[bullet_cnt][0];
	  fiter->bullet[cnt].pos.y = fiter->pos.y + gun_pos[bullet_cnt][1];
	  fiter->bullet[cnt].pos.z = fiter->pos.z + gun_pos[bullet_cnt][2];

	  /* Bullet sound.  Controller can change sound in easy-to-understand way */
	  auSndPlay(pad_no+2);
	  break;
	}
      } 
    }
    break;
  } /* end of switch */
}

/*-----------------------------------------------------------------------------
  Fiter's bullet movement
  CBullet* bullet :   bullet structure pointer
-----------------------------------------------------------------------------*/
void bullet_move(CBullet *bullet)
{

  /* Check bullet status flag, do each process */
  switch(bullet->flag){
  case FITER_BULLET_FLAG_NONE:   /* bullet is not being fired */
    return;
  case FITER_BULLET_FLAG_FIRE:   /* bullet is flying */

    bullet->pos.z += 20.0;

    /* Check bullet position.  If  over 1000.0 delete. */
    if(bullet->pos.z >1000.0 ){
      bullet->flag = FITER_BULLET_FLAG_NONE;
    }
    break;
  }
}
/*-----------------------------------------------------------------------------
Movement of viewpoint.
Calculate viewpoint movement for each pad.
-----------------------------------------------------------------------------*/
void lookat_move(int pad_no)
{

  /* C-UP button */
  if(controllerdata_ptr[pad_no]->button & U_CBUTTONS){
    lookat.pitch++;
    if(lookat.pitch > 89.0) {
      lookat.pitch = 89.0;
    }
  }

  /* C-DOWN button */
  if(controllerdata_ptr[pad_no]->button & D_CBUTTONS){
    lookat.pitch--;
    if(lookat.pitch < -89.0) {
      lookat.pitch = -89.0;
    }
  }

  /* C-RIGHT button */
  if(controllerdata_ptr[pad_no]->button & R_CBUTTONS){
    lookat.yaw++;
    if(lookat.yaw > 360.0) {
      lookat.yaw = 0.0;
    }
  }

  /* C-LEFT button */
  if(controllerdata_ptr[pad_no]->button & L_CBUTTONS){
    lookat.yaw--;
    if(lookat.yaw < 0.0) {
      lookat.yaw = 360.0;
    }
  }

  /* + key UP button*/
  if(controllerdata_ptr[pad_no]->button & U_JPAD){
    if(lookat.dist < -20.0) {
      lookat.dist++;
    }
  }

  /* + key DOWN button */
  if(controllerdata_ptr[pad_no]->button & D_JPAD){
    lookat.dist--;
  }

}


