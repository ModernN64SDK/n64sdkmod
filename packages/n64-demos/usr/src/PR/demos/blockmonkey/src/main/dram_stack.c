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

/*---------------------------------------------------------------------*
        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
        
        $RCSfile: dram_stack.c,v $
        $Revision: 1.2 $
        $Date: 1998/12/24 14:25:14 $
 *---------------------------------------------------------------------*/

/*
 * buffers for RSP tasks:
 */

/*  for CodeWarrior compiler */
#ifdef __MWERKS__
#pragma align(16)
#endif

#if __GNUC__ /* { */

/* for GNU compiler */
u64 dram_stack[SP_DRAM_STACK_SIZE64] __attribute__((aligned (16))); /* used for matrix stack */

#else /* }{ */

/* for SGI compiler */
u64 dram_stack[SP_DRAM_STACK_SIZE64];	/* used for matrix stack */

#endif /* } */
