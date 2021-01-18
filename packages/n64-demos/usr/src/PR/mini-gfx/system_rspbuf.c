/*---------------------------------------------------------------------
  $Id: system_rspbuf.c,v 1.1.1.1 2002/05/02 03:27:18 blythe Exp $
  
  File : system_rspbuf.c

  Coded     by Yoshitaka Yasumoto.   Jun 17, 1997.
  Copyright by Nintendo, Co., Ltd.           1997.
  ---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	"system.h"

u64	system_rdpfifo[RDPFIFO_SIZE];
u64	system_rspyield[OS_YIELD_DATA_SIZE/sizeof(u64)];

/*======== End of system_rspbuf.c ========*/
