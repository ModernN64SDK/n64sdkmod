
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: hddproc.h,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:00 $
 *---------------------------------------------------------------------*/

#ifndef __DDPROC__
#define __DDPROC__

/*
   Function to be used for DD
 */

#include "hdd.h" 

#if 0

extern  u8           	*_rwbuf[2];
#define READWRITE_BYTESIZE   		0x10000 /*    65k about 3 block   */
/*
   Default buffer and its size for DD
 */

#define ERRBUF_SIZE     			7
#define NUM_LEO_MESGS   			8
#define DISK_MSG_NUM 				1

#define DDSTAT_GOOD				0
#define DDSTAT_IDLE				0
#define DDSTAT_INSERTED			0
#define DDSTAT_CONTINUE    		0x10
#define DDSTAT_KILLED			0x20
#define DDSTAT_DEAD				0x40
#define DDSTAT_BUSY       		0x80
#define DDSTAT_DIALOGUE			0x100

#define ERRMES__DRIVE_NOT_READY		0x01
#define ERRMES__ACCESSLAMP			0x02
#define ERRMES__WRONG_DISK	 		0x04
#define ERRMES__NOW_IN_RESTORE      0x08 
#define ERRMES__ACCESS_TO_NOFORMAT_AREA 0x10
#define ERRMES__TORISETSU_YOME      0x11 
#define ERRMES__SET_CLOCK           0x12
#define ERRMES__RAMPACK_NOT_CONNECTED	0x14
#define ERRMES__MANUAL_YOME 		0x18

#define ERRMES__INSERT_AGAIN        0x100
#define ERRMES__DISK_WO_IRERO		0x200
#define ERRMES__DISK_WO_NUKE        0x400

#define ERRMES__UNEXPECTED			0x20
#define ERRMES__UNDEFINED	   		0x21

#define LEO_ERROR_DDROMCART_NOT_CONNECTED    105
#define ERRMES__DDROMCART_NOT_CONNECTED    0x22


#define LEOCMD_INITIALIZE 			61
#define LEOCMD_GETID 				13
#define LEOCMD_GETCORE 				14
#define LEOCMD_READ					3
#define LEOCMD_WRITE				4
#define LEOCMD_ACTIVE   			5
#define LEOCMD_STANDBY  			6
#define LEOCMD_SLEEP    			7
#define LEOCMD_BRAKE    			8
#define LEOCMD_EJECT    			32

#define LEOCMD_LBAREAD  			23
#define LEOCMD_LBAWRITE 			24
#define LEOCMD_BYTEREAD				25
#define LEOCMD_BYTEWRITE			26

#define LEOCMD_GETRTC			    30
#define LEOCMD_SETRTC			    31

#define LEOCMD_IDLE					0
#define LEOCMD_SEEK					2
#define LEOCMD_REZERO				1

#define LEOCMD_TESTUNIT    			28
#define LEOCMD_SETMOTORTIME			27

#define LEOCMD_THAW    			    40
#define LEOCMD_TEST  				99

#define UNITMODE_LBA 				0
#define UNITMODE_BYTE   			1

#define LEO_ERROR_USER_DEFINED     	101
#define LEO_ERROR_MODE_INCOMPLETE	100
#define LEO_ERROR_PROGRAM_FAULT		102
#define LEO_ERROR_UNEXPECTED_ACCIDENT	103
#define LEO_ERROR_CHECKSUM	        LEO_ERROR_UNRECOVERED_READ_ERROR

#define TRANSMIT_LOWRATE  			1   /* transfer efficiency conventionary under 1M byte */

#define SEC_SIZE_ZONE0        232    /* bytes/sector in zone-0 */

#define DD_MAX_LBA					4291
#define DD_MAX_BYTE					64458560

typedef struct {
  s32   mode;			        /* Status */
  s32   errno;					/* Error number */
  s32 	errmes;			        /* Message number. Please note it is notsynchronizing with the errno */
  s32   counter;				/* Number of reset */
  s32   current_buffer;			/* Numbers of buffer being used */
  u32   buffer_size;            /* It has already been transferred if the length of the read data is 0 */
  u32   position;               /* Position of the head (LBA) */
  u32   read_offset;            /* Offset the start reading from the
buffer  */
} TDDStatus;

typedef struct{
  s32   command;  /* 0 means available , non 0 means this hasnt reach yet */
  s32 	locked;
  struct{ 
    s32   hi;
    s32   lo;
  }param;
  OSMesgQueue *retQ;
  s32   retVal;
} TMessage4dd;

#define dd_reset        LeoReset

extern  OSMesgQueue     ddflushQ;


extern  TDDStatus       dd_stat;
/*
   The current status of DD is stored.
   read only and nothing can be recorded
 */

extern  s32 			dd_create(void);
/*
   Create the DD object
 */

extern  s32 			dd_exec( TMessage4dd * msg );
/*
   Send the instruction to DD object
   _rwbuf is used to read and write
   Become the synchronous function if msg->retQ is not set (thread is
   suspended)
 */

extern  s32      		dd_checkGate( TMessage4dd * msg );
/*
   Receive the notification of the completion of execution of dd_exec to
   msg->retQ 
   Use dd_checkGate as the filter when using dd_exec as the asynchronous
   function
   
   if( dd_checkGate(msg) )
   {
     dd_exec(msg);
   }

   as stated above.
 */

extern  s32 			dd_saveID(LEODiskID * det);
extern  u32				dd_getVersion(void);
extern  s32 			dd_getBlockSize(s32 lba,s32 size);

extern  void            dd_change_mode_USA(void);
extern  void            dd_errFook(s32 func_no , s32 err_no , s32 count );
extern  s32             dd_thaw(s32 target_err);

/*
  Function for checking (dd_checksum.c)
  Function for checking at the time of read and write 
  Modify the contents of the following functions according to the application
 */

extern  s32             dd_checkSum(void);
extern  s32             dd_makeSum(void);
extern  s32             dd_restoreData(void);

extern OSPiHandle   *HANDLE_DD; 
#define dd_romRead(src_addr,dest_addr,size) g_dmaRead( HANDLE_DD ,src_addr,dest_addr,size)

#endif


/*
	Global variable and function
 */
#include "hglobals.h" 

#ifndef true
#define true -1
#define false 0
#endif 

#if 0

#define  abs(x)                  ( (x >= 0) ? (x) : (-(x)))

#define  FORI(j)    for(i=0;i<j;i++) 
#define  FORJ(size) for(j=0;j<size;j++)
#define  CEIL(f) ((((s32)f) < f ) ? ((s32)f + 1) : (s32)f )

#define  CEIL16(i)   ( (i & 0x0f) ? ((i & ~0xf) + 0x10) : i )
#define  FLOOR16(i)  ( i & ~0xf )

#define  PXL32_R(rgba)  ((rgba>>24) & 0xFF)
#define  PXL32_G(rgba)  ((rgba>>16) & 0xFF)
#define  PXL32_B(rgba)  ((rgba>>8) & 0xFF)
#define  PXL32_A(rgba)  ( rgba & 0xFF )

typedef struct {  s32 x,y,w,h;} TRect;

typedef struct {  u8 r,g,b,a;} TRGBA;
 
typedef struct {
  OSTask      *cur_task;
  OSMesgQueue *suspendQ;
  s32         yield_type;
  s32         first_done;
} TRspStat;

extern TOption 		opt;
extern OSId			g_thread_id;

extern OSPiHandle   *HANDLE_ROM; 
extern TRspStat     rsp_stat;
extern s16			resetflag;
extern u8  			rdbSendBuf[RDB_SEND_BUF_SIZE];
extern OSMesgQueue  rspMsgQ, rdpMsgQ, siMessageQ;

extern s32  g_dmaRead(OSPiHandle *handle, void *src_addr, void *dest_addr, u32 size);
#define g_romRead(src_addr,dest_addr,size) g_dmaRead( HANDLE_ROM ,src_addr,dest_addr,size)

#endif


#include "hvideo.h" 

/*
	Related to the VI(Video Interface) and drawing
 */

#if 0

#define VIDEO_DLMODE_IDLE 		NULL
#define VIDEO_DLMODE_SPRITE 	1
#define VIDEO_DLMODE_DRAWRECT   2
#define VIDEO_DLMODE_CUSTOM     99

typedef struct{
  void  *romstart;
  u32   *romend ;
  void  *addr ;
}TSegInfo;

extern TSegInfo dlist_statics;

extern Gfx *   	video_getDL(void);
extern s32     	video_setDL( Gfx * pgfx );
extern s32     	video_getCurrentBuffer(void);
extern void    	video_flush(void);

extern s32  	video_setSprite(Sprite *sp ,s32 xpos,s32 ypos);
extern void 	video_startDLMode(s32 mode);
extern void 	video_finishDLMode(void);
extern s32     	video_getCurrentDLMode(void);

extern void    	video_clearSprite(s32 cur);
extern void    	video_initSprite(ALHeap *hp);
extern s32      video_drawRect(s32 x,s32 y,s32 w,s32 h ,u32 rgba);

extern Gfx      dlist_rspinit[];
extern Gfx      dlist_rdpinit[];
extern Gfx      dlist_clearcfb[];
extern Gfx      dlist_clearzbuf[];

#endif

/*
   Related to the character drawing
 */

#include "hvideo_str.h" 

#if 0

#define STR_MAXLENGTH   64		/*  maximum length of the single line of letters */
#define STR_MAXSUM   	512	    /*  total number of letters */

#define STR_DL_BUFSIZE   NUM_DL( STR_MAXSUM )    

#define JSTR_DMA_NUM     400    
#define JSTR_KEEP_COUNT  1
#define JSTR_DMA_SIZE    128

/*  Structure for the fixed size font */

typedef struct {
    u8 		*ascii;
	s16 	char_width;
	s16		char_height;
    Bitmap  *bmp;
	s32		bmsize;
    u8 		*img;
} TFont;

extern s32 		video_setString(u8 *str, s32 xpos, s32 ypos, u32 color);
extern void		video_clearString(s32 cur);
extern void 	video_initString(ALHeap *hp);
extern s32     	video_setJStr(u8 *str, s32 xpos, s32 ypos, u32 color);

/*
   dd_setJStr might be more appropriate
   Character with 2 bytes is displayed using the font for dd
 */

extern void 	str_setColor(Sprite *sp, u32 color);
extern s32      str_getWidth(u8 *str);
extern s32      str_getFontWidth(void);
extern s32      str_getFontHeight(void);

#endif

/*
   SI - Serial Interface
   Controller and so on
 */     

#include "hsi.h" 

#if 0

#define SI_MSGQ_SIZE 	1
#define SI_DMAQ_SIZE    1
#define SI_CONT_STICK_BORDER 24

typedef struct{
  s32		available;
  u8		status;
  u16       store, now, trig;    
  s32       repeat[16];
}TContStat;

extern	TContStat  * 	si_getCont(s32 contNo);
extern  u16 			si_getTrigger(s32 contNo);
extern  s32 			si_setInterval(s32 i);

/* 
typedef struct {
        u16     type;                   /*  Controller Type  */
        u8      status;                 /*  Controller status  */
        u8      errno;
} OSContStatus;

typedef struct {
        u16     button;
        s8      stick_x;                /*  -80 <= stick_x <= 80  */
        s8      stick_y;                /*  -80 <= stick_y <= 80  */
        u8      errno;
} OSContPad;

*/

#endif

/*
   optional function
 */

#include "hsb.h" 

#if 0

extern s32     	sb_clip(s32 val ,s32 from ,s32 to);
extern u16     	sb_jpad( u16 trig );
extern s32     	sb_length(u8 *s);

/*  character bar */

extern s32     	sbar_create(TStringBar * bar,u8 * str, s32 xpos ,s32 ypos );
extern s32    	sbar_show(TStringBar * bar);
extern s32     	sbar_setBase(TStringBar * bar,s32 x ,s32 y);
extern s32     	sbar_setClip(TStringBar * bar,s32 left ,s32 top ,s32 right ,s32 bottom );

extern s32 		sbar_setSubst(TStringBar * bar,void * src, u8 **src_sub ,s32 length );
extern s32 		sbar_setSrc(TStringBar * bar,void * src, s32 type ,s32 length );
extern s32     	sbar_moveCenter(TStringBar * bar ,s32 left ,s32 right );
extern s32     	sbar_getWidth(TStringBar * bar);

/*  tile */

extern s32     	btile_create(TBackTile *bt ,s32 xpos ,s32 ypos ,s32 width ,s32 height ,u32 color);
extern s32     	btile_setBase(TBackTile * bt ,s32 x ,s32 y);
extern s32     	btile_moveCenter(TBackTile * bt ,s32 left ,s32 right );
extern s32     	btile_setClip(TBackTile * bt,s32 left ,s32 top ,s32 right ,s32 bottom );
extern s32     	btile_show(TBackTile * bt);

#endif

#include "hreset.h"

#endif /* __DDPROC__ */
