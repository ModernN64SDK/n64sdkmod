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
                              マクロ定義
  --------------------------------------------------------------------*/

#define	SI_MSG_NUM	1
#define	RET_MSG_NUM	1

/*--------------------------------------------------------------------
                              構造体定義
  --------------------------------------------------------------------*/

typedef struct{				/* siproc へのメッセージ */
  u8    mode;
  u8	si_mode;
} sendMsg;

typedef struct{				/* siproc からのメッセージ */
  s32	ret;
  u8	mode;
  u8    si_mode;
} retMsg;

/*--------------------------------------------------------------------
                              外部宣言
  --------------------------------------------------------------------*/

extern 	OSMesgQueue	sendMsgQ;	/* siproc へのメッセージ渡し用 */
extern 	OSMesgQueue	retMsgQ;	/* siproc からのメッセージ返し用 */

extern	void	siproc(void);

#endif /* _siproc_h_ */


