#ifndef __bb_atb_h__
#define __bb_atb_h__

#include <PR/bbtypes.h>

#define BB_ATB_SUCCESS  0
#define BB_ATB_FAIL    -1

#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)

s32 osBbAtbSetup(u32 vAddrBase, u16 *fsBlockList, u32 maxListLen);
u32 osBbAtbGetVAddr();

#endif  /* defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS) */

#endif
