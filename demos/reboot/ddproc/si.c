
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: si.c,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:01 $
 *---------------------------------------------------------------------*/

/* pre-process */

#include "hglobals.h"
#include "hsi.h"

/* declarerations */
OSMesgQueue     siMsgQ, siDmaMsgQ;
OSMesg          siMsgBuf[SI_MSGQ_SIZE], siDmaMsgBuf[SI_DMAQ_SIZE];

static OSThread siThread;
static u64      siThreadStack[STACKSIZE/sizeof(u64)];
static void     objectOfsi(void * );
static s32      si_readCont(void);
static s32      si_initCont(void);
static s32      si_interval;

static TContStat	_contstat[MAXCONTROLLERS];

/* implmentations */

void
si_create(void)
{
  osCreateMesgQueue(&siMsgQ, siMsgBuf, SI_MSGQ_SIZE);
  osCreateMesgQueue(&siDmaMsgQ, siDmaMsgBuf, SI_DMAQ_SIZE);
  osSetEventMesg(OS_EVENT_SI, &siDmaMsgQ, (OSMesg)1);        /* SI */

  si_initCont();       /* Initialize controllers */

  osCreateThread(&siThread, ++g_thread_id,(void *)objectOfsi, 0
                                 ,siThreadStack + STACKSIZE/sizeof(u64), SI_THREAD_PRIORITY);
  osStartThread(&siThread);
}

void
objectOfsi(void * arg)
{

  arg = arg; /* no use */

  while(1)
	{
	  osRecvMesg(&siMsgQ, (OSMesg *)NULL, OS_MESG_BLOCK);
	  si_readCont();
	}
}

void
si_ticker( void )
{
  static s32 cnt=1;

  if( cnt < 0 )
	{
	  osSendMesg(&siMsgQ, (OSMesg)NULL, OS_MESG_BLOCK); 
	  cnt = si_interval; 
	}
  else
	{
	  cnt--;
	}
}

s32
si_readCont(void)
{
  int	i;
  OSContPad	contdata[MAXCONTROLLERS];
  TContStat * p;
  u16       bits,mask,trigmask;
  u8        j;

  osContStartReadData(&siDmaMsgQ); /* inside of the function : wait for SI_EVENTafter requesting the DMA
								   
 (Reading is completed at this point after it has been blocked) */
  osContGetReadData(contdata);     /* only to read the contents of RDRAM */


  for(i = 0 ; i < MAXCONTROLLERS ; i ++)
	{
	  p = &_contstat[i];
	  if (!(contdata[i].errno & CONT_NO_RESPONSE_ERROR)) 
		{
		  
		  bits = contdata[i].button;
		  if( contdata[i].stick_x > SI_CONT_STICK_BORDER )   bits |= R_JPAD;
		  if( contdata[i].stick_y > SI_CONT_STICK_BORDER )   bits |= U_JPAD;
		  if( contdata[i].stick_x < - SI_CONT_STICK_BORDER ) bits |= L_JPAD;
		  if( contdata[i].stick_y < - SI_CONT_STICK_BORDER ) bits |= D_JPAD;
		  
		  p->available = 1;
		  p->store = p->now;
		  p->now   = bits;
		  p->trig =  p->now & ~p->store;
		  
		  trigmask=0;
		  mask=0;
		  
		  for(j=0;j<16;j++)
			{
			  bits = p->now & p->store;  /* repeat bit */   
			  mask = 1<<j;
			  if(bits & mask)
				{
				  ++p->repeat[j];
				  if ( (p->repeat[j] > 32) && (p->repeat[j] & 3) ) 
					trigmask |= mask;
				}
			  else
				{
				  p->repeat[j]=0;
				}
			}
		  p->trig |= trigmask;
		}
	  else
		{
		  bzero(p,sizeof(TContStat));
		}
	}
  return i;
}


s32
si_initCont(void)
{
  int		i,j=0;
  u8		pattern;
  OSContStatus	stat[MAXCONTROLLERS];
  TContStat * p;

  osContInit(&siDmaMsgQ, &pattern, stat);

  for (i = 0 ; i < MAXCONTROLLERS ; i ++)
	{
	  p = &_contstat[i];

	  p->status = stat[i].status;
	  p->now = 0;
	  if ( ((pattern >> i) & 1) != 0 &&
		  ( stat[i].type & CONT_JOYPORT ) != 0 &&
		  ( stat[i].type & CONT_ABSOLUTE ) != 0 ) {
		p->available = 1;
		j++;
	  } else {
		p->available = 0;
	  }
	}

  si_interval = 0;

  return j;
}

TContStat *
si_getCont(s32 contNo)
{
	return &_contstat[contNo];
}

u16
si_getTrigger(s32 contNo)
{
  u16 ret;
  ret = _contstat[contNo].trig;
  _contstat[contNo].trig = 0;             /*  Clear and call trigger
simultaneously */
  return ret;	
}

s32
si_setInterval(s32 i)
{
  if (i<0) return 0;
  si_interval = i;
  return -1;
}


