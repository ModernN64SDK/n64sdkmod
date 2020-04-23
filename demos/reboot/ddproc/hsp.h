
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: hsp.h,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:00 $
 *---------------------------------------------------------------------*/

#ifndef _HSP_H_
#define _HSP_H_

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

#include <PR/mbi.h>
#include <PR/ultratypes.h>
#include <PR/sp.h>

Gfx * spFillRect(Gfx * dl ,s32 x,s32 y,s32 x2,s32 y2 ,u8 r,u8 g,u8 b,u8 a);

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif

#endif /* _HSP_H_ */
