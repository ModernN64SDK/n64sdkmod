/*---------------------------------------------------------------------*
	Copyright (C) 1997, Nintendo.
	
	File		action.h
	Coded    by	Yoshitaka Yasumoto.	Sep 12, 1997.
	Modified by	
	
	$Id: action.h,v 1.1 1997/09/12 07:03:18 yasu Exp $
 *---------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * Controller Structure   
 *---------------------------------------------------------------------------*/
typedef struct  {
  OSContPad     *c;
  u16           button;
  u16           push;
  u16           release;
} myContPad;

/*---------------------------------------------------------------------------*
 * Action Data Structure 
 *---------------------------------------------------------------------------*/
typedef struct  {
  myContPad     pad[MAXCONTROLLERS];
} Action;

/*---------------------------------------------------------------------------*
 * Declare Prototype
 *---------------------------------------------------------------------------*/
extern  Action  Ac;
extern  void    actionInit(void);
extern  void    actionUpdate(void);

/*======== End of action.h ========*/
