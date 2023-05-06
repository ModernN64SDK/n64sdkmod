/*============================================================================
  mix.c
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
#include "graphic.h"

extern 	float rotate;

/*--------------------------
  sequence & square
  --------------------------*/
int main00(NNSched* sched)
{
    OSMesgQueue msgQ;
    OSMesg      msgbuf[MAX_MESGS];
    NNScClient client;

    /* create message queue for VI retrace */
    osCreateMesgQueue(&msgQ, msgbuf, MAX_MESGS);

    /* Set sequence 0 to sequence player 0 */
    auSeqPlayerSetFile(0,2);

    /* add client to shceduler */
    nnScAddClient(sched, &client, &msgQ);

    auSeqPlayerPlay(0);
    graphic_no = GFX_00;

    while(1){
	(void) osRecvMesg(&msgQ, NULL, OS_MESG_BLOCK);
	/* If sequence stops, regenerate. */
	if(auSeqPlayerState(0) == AL_STOPPED){
	    auSeqPlayerPlay(0);
	}
	rotate++;
    }
    
    /* stop sequence */
    auSeqPlayerStop(0);
    graphic_no = GFX_NULL;

    /* graphic task/audio task end stand by */
    while((auSeqPlayerState(0) != AL_STOPPED) || (pendingGFX != 0)){
	(void) osRecvMesg(&msgQ, NULL, OS_MESG_BLOCK);
    };

    /* remove client to shceduler */
    nnScRemoveClient(sched, &client);
  
    /* exit label */
    return MAIN_01;
}
