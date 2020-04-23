/*---------------------------------------------------------------------*
	Copyright (C) 1998, Nintendo.
	
	File		action.c
	Coded    by	Tetsuyuki Ootsuka.	July, 1998.

	$Id: action.c,v 1.1 1998/07/23 03:36:09 otsu Exp $
 *---------------------------------------------------------------------*/
#include	<ultra64.h>
#include        "action.h"
#include	"nu64sys.h"

/*-- Action Structure --*/

Action  Ac;

/*-- Initializing Action Structure  --*/

void    actionInit(void)
{
  int   i;

  for (i = 0; i < MAXCONTROLLERS; i ++){
    Ac.pad[i].c = contPad + i;
    Ac.pad[i].button = Ac.pad[i].push = Ac.pad[i].release = 0;
  }
  return;
}

/*-- Renewal of Action Structure Member --*/

void    actionUpdate(void)
{
  int   i;
  u16   button, change;
  
  /*-- Calculating controller triger (poll)  --*/
  for (i = 0; i < MAXCONTROLLERS; i ++){
    button = Ac.pad[i].c->button;
    change = Ac.pad[i].button ^ button;
    Ac.pad[i].release = (Ac.pad[i].push = change & button) ^ change;
    Ac.pad[i].button = button;
  }
  return;
}

/*======== End of action.c ========*/
