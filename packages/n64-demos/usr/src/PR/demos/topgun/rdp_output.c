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
#include "topgun.h"

/*
 * buffers for RSP tasks:
 */

/*
 * buffers used by fifo microcode only
 */

/*  for CodeWarrior compiler */
#ifdef __MWERKS__
#pragma align(16)
#endif

#if __GNUC__ /* { */

/* for GNU compiler */
u64 rdp_output[RDP_OUTPUT_LEN] __attribute__((aligned (16))); /* buffer for RDP DL */

#else /* }{ */

/* for SGI compiler */
u64 rdp_output[RDP_OUTPUT_LEN];		/* buffer for RDP DL */

#endif /* } */
