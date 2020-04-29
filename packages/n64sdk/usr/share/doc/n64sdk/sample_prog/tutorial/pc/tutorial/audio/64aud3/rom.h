/*******************************************************************
 *
 *  rom.h
 *
 *******************************************************************/

#ifndef   __ROM_HEADER__
#define   __ROM_HEADER__

#include    <ultra64.h>

void  DmaRomToRam(void * from_addr, void * to_addr, s32 size);

#endif /* __ROM_HEADER__ */
