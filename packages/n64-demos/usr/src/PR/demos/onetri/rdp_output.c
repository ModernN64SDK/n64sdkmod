
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
 * The "output_buff" field of the RCP task structure is set to this address.
 * It is placed in its own .c, and thus its own .o, since the linker aligns
 * individual relocatables to data cache line size (16 byte) boundaries.
 *
 * This avoids the problem where the output buffer data is accidentally
 * scribbled over during a writeback for data sharing the same line.
 */

#include <ultra64.h>
#include "onetri.h"

/*
 * buffers for RSP tasks:
 */

/*
 * buffers used by fifo microcode only
 */


/*  for CodeWarior compiler */
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
