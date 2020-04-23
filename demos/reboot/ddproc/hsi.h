
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: hsi.h,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:00 $
 *---------------------------------------------------------------------*/
/*
   si.h  - serial interface
 */     

#ifndef	__SI_H__
#define	__SI_H__

#define SI_MSGQ_SIZE 	1
#define SI_DMAQ_SIZE    1
#define SI_CONT_STICK_BORDER 24

typedef struct{
  s32		available;
  u8		status;
  u16       store, now, trig;    
  s32       repeat[16];
}TContStat;

extern	void	si_create(void);
extern	TContStat  * si_getCont(s32 contNo);
extern  OSMesgQueue   siMsgQ;
extern  u16 si_getTrigger(s32 contNo);
extern  s32 si_setInterval(s32 i);
extern  void  si_ticker( void );

#endif /* __SI_H__ */

#if 0 
/*  tech note */

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

#endif
