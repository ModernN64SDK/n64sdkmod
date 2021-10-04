/*---------------------------------------------------------------------*
	Copyright (C) 1998, Nintendo.
	
	File		action.h
	Coded    by	Tetsuyuki Ootsuka.	July, 1998.

	$Id: action.h,v 1.1.1.1 2002/05/02 03:27:29 blythe Exp $
 *---------------------------------------------------------------------*/

/*-- コントローラ構造体 --*/

typedef struct  {
  OSContPad     *c;
  u16           button;
  u16           push;
  u16           release;
} myContPad;

/*-- アクションデータ構造体 --*/

typedef struct  {
  myContPad     pad[MAXCONTROLLERS];
} Action;

/*-- プロトタイプ宣言 --*/

extern  Action  Ac;
extern  void    actionInit(void);
extern  void    actionUpdate(void);

/*======== End of action.h ========*/
