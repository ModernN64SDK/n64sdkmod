/*---------------------------------------------------------------------
  File   : nu64sys.h

  Coded    by     Tetsuyuki Ootsuka.      July, 1998.
	
  Copyright by Nintendo, Co., Ltd. 1998.
  ---------------------------------------------------------------------*/
#ifndef	_nu64sys_h_
#define	_nu64sys_h_

#define	public
#define	private	static

#define SCREEN_LOW      1
#define SCREEN_HIGH     !SCREEN_LOW

#if     SCREEN_LOW
#define SCREEN_WD       320
#define SCREEN_HT       240
#else
#define SCREEN_WD       640
#define SCREEN_HT       480
#endif

#define STACKSIZE	0x2000

#define CONTR_START     0
#define CONTR_END       1
#define VOICE_START     2
#define VOICE_END       3

#define InitMode        0
#define ClrDicMode      1
#define SetWordMode     2
#define StartReadMode   3
#define GetReadMode     4
#define StopReadMode    5
#define NoContMode      6

#ifdef _LANGUAGE_C

extern  OSMesgQueue     piMessageQ;
extern  OSMesgQueue     siMessageQ;
extern  OSMesgQueue     dmaMessageQ;
extern  OSMesgQueue     rspMessageQ;
extern  OSMesgQueue     rdpMessageQ;
extern  OSMesgQueue     retraceMessageQ;

extern  OSContStatus    contStatus[MAXCONTROLLERS];
extern  OSContPad       contPad[MAXCONTROLLERS];
extern  u8              contExist;
extern	u8              contNo;

extern	OSVoiceData     result;
extern	OSVoiceHandle   vhd;

extern  u16             cfb[][SCREEN_WD*SCREEN_HT];
extern  u8      	draw_buffer;

#endif

#endif
