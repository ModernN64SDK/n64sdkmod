
/**************************************************************************
 *                                                                        *
 *               Copyright (C) 1994, Silicon Graphics, Inc.               *
 *                                                                        *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright  law.  They  may not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *                                                                        *
 *************************************************************************/

/*
 * Put the Color Frame Buffer below zbuffer.  Place it so that 16 bit or
 * 32 bit buffer will fit
 * This gives space for 2 16 bit or 1 32 bit buffers
 */

#include <ultra64.h>
#include "gfx.h"

unsigned short	cfb_16_a[SCREEN_WD*SCREEN_HT];
unsigned short	cfb_16_b[SCREEN_WD*SCREEN_HT];
