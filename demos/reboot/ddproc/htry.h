
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: htry.h,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:00 $
 *---------------------------------------------------------------------*/

#ifndef __TRY_C__
#define __TRY_C__

#define TRY_MSGQ_SIZE           24

typedef struct {
  s32 id;

}TException;


extern  void  try_create(void);
extern  void  try_assert(u8 * str,s32 i);
extern  void * try_heapAlloc(ALHeap *hp ,s32 size, s32 align, u8 * str);

extern  s32 try_warning(u8 * str , s32 i);


#endif /* __TRY_C__ */


