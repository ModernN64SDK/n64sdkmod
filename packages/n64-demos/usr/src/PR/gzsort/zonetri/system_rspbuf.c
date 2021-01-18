/*---------------------------------------------------------------------*
	Copyright (C) 1997, Nintendo.
	
	File		system_rspbuf.c
	Coded    by	Yoshitaka Yasumoto.	Sep 12, 1997.
	Modified by	
	
	$Id: system_rspbuf.c,v 1.1 1997/10/13 08:59:27 yasu Exp $
 *---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	"system.h"

u64	system_rdpfifo[RDPFIFO_SIZE];
u64	system_rspyield[OS_YIELD_DATA_SIZE/sizeof(u64)];

/*======== End of system_rspbuf.c ========*/
