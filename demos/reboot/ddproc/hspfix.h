
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: hspfix.h,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:00 $
 *---------------------------------------------------------------------*/
#ifndef __SPFIX__
#define __SPFIX__

#ifdef DL_BM_OVERHEAD
#undef DL_BM_OVERHEAD
#endif

#ifdef DL_SPRITE_OVERHEAD
#undef DL_SPRITE_OVERHEAD
#endif

#define DL_BM_OVERHEAD     (16)     /*  prev (32) */
#define DL_SPRITE_OVERHEAD (32)     /*  prev (128) */

#endif
