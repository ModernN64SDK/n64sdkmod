
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: herr.h,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:20:04 $
 *---------------------------------------------------------------------*/

#ifndef __ERR_H__
#define __ERR_H__

extern void scr_error(void);
extern void err_print(void);
extern void err_setString(u8 * message);
extern void rtc_noReadMode(void);
 
#endif
