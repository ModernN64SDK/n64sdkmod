/*---------------------------------------------------------------------*
        Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
        
        $RCSfile: ctype.c,v $
        $Revision: 1.1 $
        $Date: 1999/04/14 07:18:39 $
 *---------------------------------------------------------------------*/
#include    "ctype.h"

unsigned char __MojiStat[0x101]={
    /* -1 (EOF) */
    0,

    /*  00 */
    _ISCNTRL,            _ISCNTRL,            _ISCNTRL,            _ISCNTRL,
    _ISCNTRL,            _ISCNTRL,            _ISCNTRL,            _ISCNTRL,
    _ISCNTRL,            _ISCNTRL|_ISSPACE,   _ISCNTRL|_ISSPACE,   _ISCNTRL|_ISSPACE,
    _ISCNTRL|_ISSPACE,   _ISCNTRL|_ISSPACE,   _ISCNTRL,            _ISCNTRL,

    /*  10 */
    _ISCNTRL,            _ISCNTRL,            _ISCNTRL,            _ISCNTRL,
    _ISCNTRL,            _ISCNTRL,            _ISCNTRL,            _ISCNTRL,
    _ISCNTRL,            _ISCNTRL,            _ISCNTRL,            _ISCNTRL,
    _ISCNTRL,            _ISCNTRL,            _ISCNTRL,            _ISCNTRL,

    /*  20 */
    _ISSPACE,           0,                  0,                  0,
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,

    /*  30 */
    _ISDIGIT|_ISXDIGIT,  _ISDIGIT|_ISXDIGIT, _ISDIGIT|_ISXDIGIT,  _ISDIGIT|_ISXDIGIT,
    _ISDIGIT|_ISXDIGIT,  _ISDIGIT|_ISXDIGIT, _ISDIGIT|_ISXDIGIT,  _ISDIGIT|_ISXDIGIT,
    _ISDIGIT|_ISXDIGIT,  _ISDIGIT|_ISXDIGIT, 0,                   0,
    0,                   0,                  0,                   0,

    /*  40 */
    0,                  _ISUPPER|_ISXDIGIT,  _ISUPPER|_ISXDIGIT,  _ISUPPER|_ISXDIGIT,
    _ISUPPER|_ISXDIGIT, _ISUPPER|_ISXDIGIT,  _ISUPPER|_ISXDIGIT,  _ISUPPER,
    _ISUPPER,           _ISUPPER,            _ISUPPER,            _ISUPPER,
    _ISUPPER,           _ISUPPER,            _ISUPPER,            _ISUPPER,

    /*  50 */
    _ISUPPER,           _ISUPPER,            _ISUPPER,            _ISUPPER,
    _ISUPPER,           _ISUPPER,            _ISUPPER,            _ISUPPER,
    _ISUPPER,           _ISUPPER,            _ISUPPER,            0,
    0,                  0,                   0,                   0,

    /*  60 */
    0,                  _ISLOWER|_ISXDIGIT,  _ISLOWER|_ISXDIGIT,  _ISLOWER|_ISXDIGIT,
    _ISLOWER|_ISXDIGIT, _ISLOWER|_ISXDIGIT,  _ISLOWER|_ISXDIGIT,  _ISLOWER|_ISXDIGIT,
    _ISLOWER,           _ISLOWER,            _ISLOWER,            _ISLOWER,
    _ISLOWER,           _ISLOWER,            _ISLOWER,            _ISLOWER,

    /*  70 */
    _ISLOWER,           _ISLOWER,            _ISLOWER,            _ISLOWER,
    _ISLOWER,           _ISLOWER,            _ISLOWER,            _ISLOWER,
    _ISLOWER,           _ISLOWER,            _ISLOWER,            0,
    0,                  0,                   0,                   _ISCNTRL,

    /*  80 */
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,

    /*  90 */
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,

    /*  A0 */
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,

    /*  B0 */
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,

    /*  C0 */
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,

    /*  D0 */
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,

    /*  E0 */
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,

    /*  F0 */
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,
    0,                  0,                  0,                  0,
};
