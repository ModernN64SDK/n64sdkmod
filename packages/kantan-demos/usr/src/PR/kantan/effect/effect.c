/*============================================================================
  effect.c
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
 Main for effect reproduction
  --------------------------*/
int main00(NNSched* sched)
{
    OSMesgQueue msgQ;
    OSMesg      msgbuf[MAX_MESGS];
    NNScClient client;
    u32  sndno;

    /* create message queue for VI retrace */
    osCreateMesgQueue(&msgQ, msgbuf, MAX_MESGS);

    /* add client to shceduler */
    nnScAddClient(sched, &client, &msgQ);

    sndno = 0;
    
    while(1){
		(void) osRecvMesg(&msgQ, NULL, OS_MESG_BLOCK);
		/*Make the next sound once per every 120 frames (2 seconds approximately)*/
		if((framecont % 120) == 0) {
	    	/*If the sound was stopped,  go to next data*/
	    	if(auSndGetState(sndno) == AL_STOPPED){
				if((sndno++) >= snd_alloc_num-2){
		    		sndno = 0;
				}
			auSndPlay(sndno);
	    	}
		}
    }

    /* exit label */
    return MAIN_01;
}
