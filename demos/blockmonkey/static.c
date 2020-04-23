
/**************************************************************************
 *                                                                        *
 *               Copyright (C) 1995, Silicon Graphics, Inc.               *
 *                                                                        *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright  law.  They  may not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *                                                                        *
 *************************************************************************/

#include <ultra64.h>
#include "block.h"
#include "static.h"

static Vtx background_vtx[] =  
{
  {       20,   20, -1500, 0, 0, 0,    0, 0xff,    0, 0xff	},
  {      300,   20, -1500, 0, 0, 0,    0,    0,    0, 0xff	},
  {      300,   20,    -5, 0, 0, 0,    0,    0, 0xff, 0xff	},
  {       20,   20,    -5, 0, 0, 0, 0xff,    0,    0, 0xff	},
};

Gfx background_dl[] = 
{
  /* Setup display modes antialiased in 1 cycle */
  gsDPPipeSync(),

  gsSPVertex(&(background_vtx[0]), 4, 0),
  gsSP1Triangle(2, 1, 0, 2),
  gsSP1Triangle(3, 2, 0, 3),

  gsSPEndDisplayList(),
};


