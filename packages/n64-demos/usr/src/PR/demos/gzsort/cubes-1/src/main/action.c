/*---------------------------------------------------------------------*
	Copyright (C) 1997, Nintendo.
	
	File		action.c
	Coded    by	Yoshitaka Yasumoto.	Sep 12, 1997.
	Modified by	
	
	$Id: action.c,v 1.1 1997/09/12 07:03:17 yasu Exp $
 *---------------------------------------------------------------------*/
#include	<ultra64.h>
#include        "system.h"
#include        "action.h"

/*---------------------------------------------------------------------------*
 * Action Structure
 *---------------------------------------------------------------------------*/
Action  Ac;

/*---------------------------------------------------------------------------*
 * Initialization of Action Structure
 *---------------------------------------------------------------------------*/
void    actionInit(void)
{
  int   i;

  for (i = 0; i < MAXCONTROLLERS; i ++){
    Ac.pad[i].c = contPad + i;
    Ac.pad[i].button = Ac.pad[i].push = Ac.pad[i].release = 0;
  }
  return;
}

/*---------------------------------------------------------------------------*
 * Update a Member of Action
 *---------------------------------------------------------------------------*/
void    actionUpdate(void)
{
  int   i;
  u16   button, change;
  
  /*---- Detect a trigger of Controller ----*/
  for (i = 0; i < MAXCONTROLLERS; i ++){
    button = Ac.pad[i].c->button;
    change = Ac.pad[i].button ^ button;
    Ac.pad[i].release = (Ac.pad[i].push = change & button) ^ change;
    Ac.pad[i].button = button;
  }
  return;
}

/*======== End of action.c ========*/
