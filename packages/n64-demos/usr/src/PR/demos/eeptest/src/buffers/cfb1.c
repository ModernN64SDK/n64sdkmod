#ifdef	_EXTERNAL_CFB
#define _CFB_GLOBAL	extern	
#define _CFB_INIT(v)	/*  */
#else
#define _CFB_GLOBAL	/*  */
#define _CFB_INIT(v)	= (v)
#endif
/*---------------------------------------------------------------------
  File   : cfb.c

  Created  by  Yoshitaka Yasumoto.   Jul,13 1995.
  Modified by  
  
  Copyright by Nintendo, Co., Ltd. 1997.
  ---------------------------------------------------------------------*/
#include <ultra64.h>
#include "nu64sys.h"

#ifdef __MWERKS__
#pragma align(8)
#endif

_CFB_GLOBAL	u32	cfb_16_a[SCREEN_WD*SCREEN_HT];
