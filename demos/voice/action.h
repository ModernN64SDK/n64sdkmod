/*---------------------------------------------------------------------*
	Copyright (C) 1998, Nintendo.
	
	File		action.h
	Coded    by	Tetsuyuki Ootsuka.	July, 1998.

	$Id: action.h,v 1.1 1998/07/23 03:36:10 otsu Exp $
 *---------------------------------------------------------------------*/

/*-- Controller Structure --*/

typedef struct  {
  OSContPad     *c;
  u16           button;
  u16           push;
  u16           release;
} myContPad;

/*-- Action Data Structure --*/

typedef struct  {
  myContPad     pad[MAXCONTROLLERS];
} Action;

/*-- Declare Prototype --*/

extern  Action  Ac;
extern  void    actionInit(void);
extern  void    actionUpdate(void);

/*======== End of action.h ========*/
