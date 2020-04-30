/*******************************************************************
 *
 *  rom.c
 *
 *******************************************************************/

#include  <nusys.h>

#define   MAX_DMA_SIZE    (16 * 1024)

void  DmaRomToRam(void * from_addr, void * to_addr, s32 size)
{
  u8 *  source;
  u8 *  dest;
  s32   length;
  
  /* If size is an even number, set it to an odd number since it cannot be sent by PI */
  if(size & 0x00000001)
    size++;
  
  source = from_addr;
  dest   = to_addr;
  
  while(size)
  {
    /* If size is too big, divide it and send */
    if (size > MAX_DMA_SIZE)
      length = MAX_DMA_SIZE;
    else
      length = size;
    
    nuPiReadRom((u32)source, dest, length);
    
    source += length;
    dest   += length;
    size   -= length;
  }
}
