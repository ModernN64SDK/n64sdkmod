/*---------------------------------------------------------------------
        Copyright (C) 1998 Nintendo.
        
        File            cont.h
        Coded    by     Tetsuyuki Ootsuka.      July, 1998.
        Comments        Header file for SI Procedure
   
        $Id: 
   ---------------------------------------------------------------------*/
#ifndef	_siproc_h_
#define	_siproc_h_

/*--------------------------------------------------------------------
                              Macro Defnition
  --------------------------------------------------------------------*/

#define	SI_MSG_NUM	1
#define	RET_MSG_NUM	1

/*--------------------------------------------------------------------
                              Structure Defnition
  --------------------------------------------------------------------*/

typedef struct{				/* Message to siproc */
  u8    mode;
  u8	si_mode;
} sendMsg;

typedef struct{				/* Message from siproc */
  s32	ret;
  u8	mode;
  u8    si_mode;
} retMsg;

/*--------------------------------------------------------------------
                              External Declare
  --------------------------------------------------------------------*/

extern 	OSMesgQueue	sendMsgQ;	/* Message to siproc */
extern 	OSMesgQueue	retMsgQ;	/* Message from siproc */

extern	void	siproc(void);

#endif /* _siproc_h_ */