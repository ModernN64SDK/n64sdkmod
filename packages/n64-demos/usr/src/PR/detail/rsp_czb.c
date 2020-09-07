
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

/*
 * RSP view of the z buffer.  It exists to create an RSP address for
 * the z buffer, which is remapped on each z to either of two
 * regions of physical memory that store that actual bits.
 */

#include <ultra64.h>
#include "app.h"

u16     rsp_czb[SCREEN_WD*SCREEN_HT];
