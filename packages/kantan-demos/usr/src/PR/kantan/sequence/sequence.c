/*============================================================================
  sequence.c
  ============================================================================*/

#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>

#include "def.h"
#include "segment.h"
#include "message.h"
#include "nnsched.h"
#include "main.h"
#include "audio.h"

/*--------------------------
  Main sequence recreation
  --------------------------*/
int main00(NNSched* sched)
{
    OSMesgQueue msgQ;
    OSMesg      msgbuf[MAX_MESGS];
    NNScClient client;
    u32  seqno;

    /* create message queue for VI retrace */
    osCreateMesgQueue(&msgQ, msgbuf, MAX_MESGS);

    /* Set sequence 0 to sequence player 0 */
    auSeqPlayerSetFile(0,2);

    /* add client to shceduler */
    nnScAddClient(sched, &client, &msgQ);

    seqno = 0;
    
    auSeqPlayerPlay(seqno);

    while(1){
		(void) osRecvMesg(&msgQ, NULL, OS_MESG_BLOCK);
		/* If sequence stops, start once again*/
		if(auSeqPlayerState(0) == AL_STOPPED){
	    	auSeqPlayerPlay(seqno);
		}
    }
  
    /* exit label */
    return MAIN_01;
}
