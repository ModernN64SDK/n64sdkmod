
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

/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: cfb.c,v $
        $Revision: 1.1.1.1 $
        $Date: 2002/05/02 03:27:29 $
 *---------------------------------------------------------------------*/

/*
 * CPU view of the actual frame buffer memory.
 */

#include <ultra64.h>
#include "nu64sys.h"

u16 cfb[2][SCREEN_WD*SCREEN_HT];
