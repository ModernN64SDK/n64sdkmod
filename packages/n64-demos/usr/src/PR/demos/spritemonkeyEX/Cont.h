/*
  Cont.h
    Copyright (C) 1999, NINTENDO Co,Ltd., MONEGI CORPORATION.
    Jan. 28, 1999.
*/

#ifndef _Cont_h_
#define _Cont_h_

#ifdef __cplusplus
extern "C" {
#endif

#define MAXCONTROLLERS 4

typedef struct _Control {
    s16 stick_x;
    s16 stick_y;
    u16 button;
    u16 trigger;
    u16 lastbutton;
} Control;

extern int init_controlers( Control * );
extern void read_controler( Control * );

#ifdef __cplusplus
}
#endif

#endif
