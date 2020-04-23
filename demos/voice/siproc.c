/*---------------------------------------------------------------------
        Copyright (C) 1998 Nintendo.
        
        File            siproc.c
        Coded    by     Tetsuyuki Ootsuka.      July, 1998.
        Comments        SI Device Procedure
   
        $Id: 
   ---------------------------------------------------------------------*/
/**************************************************************************
 *
 *  $Revision: 1.3 $
 *  $Date: 1998/10/02 09:02:38 $
 *  $Source: 
 *
 **************************************************************************/

#include <ultra64.h>
#include "nu64sys.h"
#include "siproc.h"
#include "action.h"

#include "message.c"

static OSMesg   retMsgBuf[RET_MSG_NUM];
OSMesgQueue     retMsgQ;

OSContStatus    contStatus[MAXCONTROLLERS];
OSContPad       contPad[MAXCONTROLLERS];
u8              contExist;
u8              contNo = 0;

u8              voice_size = sizeof(voice_data)/sizeof(u8 *);


/*---------------------------------------------------------------------
                  Serial Interface Procedure
  ---------------------------------------------------------------------*/
void
siproc(void)
{
  u32  		i;
  u8		mode, si_mode;
  s32		ret;
  retMsg 	retMesgBuff;
  retMsg 	*retMesg = &retMesgBuff;
  sendMsg 	dummyBuff;
  sendMsg 	*dummy = &dummyBuff;
  
  osCreateMesgQueue(&retMsgQ, retMsgBuf, RET_MSG_NUM);

  /*-- Initializing Controller  --*/

  osContInit(&siMessageQ, &contExist, contStatus);
  actionInit();


  while(1){


    osRecvMesg(&sendMsgQ, (OSMesg *)&dummy, OS_MESG_BLOCK);

    mode = dummy->mode;
    si_mode = dummy->si_mode;
    
    
    if(si_mode == CONTR_START){
      
      /***** Obtaining Controller Data *****/

      osContStartReadData(&siMessageQ);
      osRecvMesg(&siMessageQ, NULL, OS_MESG_BLOCK);
      osContGetReadData(contPad);
      actionUpdate();

      si_mode = CONTR_END;
      
    }
    else if(si_mode == VOICE_START){

      /***** Starting Voice Recognition Process  *****/
      
      switch(mode){
	
	/*-- Initialization --*/
	
      case	InitMode:

	osContStartQuery(&siMessageQ);
	osRecvMesg(&siMessageQ, NULL, OS_MESG_BLOCK);
	osContGetQuery(contStatus);
	
	for(i = 0; i < MAXCONTROLLERS; i++){
	  if((contStatus[i].errno == 0) && ((contStatus[i].type & CONT_TYPE_MASK) == CONT_TYPE_VOICE)){
	    contNo = i;
	    break;
	  }
	}
	
	ret = osVoiceInit(&siMessageQ, &vhd, contNo);
	
#ifdef CHECKGAIN
	ret = osVoiceControlGain(&vhd, 1, 7);
#endif
	
	mode = ClrDicMode;
	break;
	
	/*-- Clearing Dictionary --*/
	
      case	ClrDicMode:
	
	ret = osVoiceClearDictionary(&vhd, voice_size);
	
	mode = SetWordMode;
	break;
	
	/*-- Setting Words for Dictionary --*/
	
      case	SetWordMode:
      
	i = 0;
    
	while(i < voice_size){

#ifdef CHECKWORD
	  if((ret = osVoiceCheckWord(voice_data[i])) != 0){
	    break;
	  }
#endif
		  
	  if((ret = osVoiceSetWord(&vhd, voice_data[i])) != 0){
	    break;
	  }
	   
	  i++;
	}
	 
	mode = StartReadMode;
	break;

	/*-- Start Voice Recognition --*/

      case	StartReadMode:
      
	ret = osVoiceStartReadData(&vhd);
	 
	mode = GetReadMode;
	break;

	/*-- Obtaining Recognition Result --*/

      case	GetReadMode:

	ret = osVoiceGetReadData(&vhd, &result);

	if(ret == 0){
	  mode = StartReadMode;
	}
      
	break;

	/*-- Halt the Process --*/

      case	StopReadMode:
      
	ret = osVoiceStopReadData(&vhd);

	mode = NoContMode;
	
        result.warning = 0;
	break;
      
      }

      si_mode = VOICE_END;

    }
    
    retMesg->ret = ret;
    retMesg->mode = mode;
    retMesg->si_mode = si_mode;
    
    osSendMesg(&retMsgQ, (OSMesg)retMesg, OS_MESG_BLOCK);
    
  }
  
}
