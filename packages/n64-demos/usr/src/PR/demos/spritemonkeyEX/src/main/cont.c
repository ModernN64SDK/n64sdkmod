/*
  cont.c
    Copyright (C) 1999, NINTENDO Co,Ltd., MONEGI CORPORATION.
    Jan. 28, 1999.
*/

#include <ultra64.h>
#include "Cont.h"

static u16 gActiveController;
OSMesgQueue	controllerMsgQ;
OSMesg		controllerMsgBuf;
OSContStatus    gStatusData[ MAXCONTROLLERS ];
OSContPad       gControllerData[ MAXCONTROLLERS ];

int init_controlers( Control *cont )
{
    OSMesgQueue     serialMsgQ;
    OSMesg          serialMsg;
    int             i;
    u8              pattern;

    cont->stick_x = 0;
    cont->stick_y = 0;
    cont->button = 0;
    cont->lastbutton = 0;

    osCreateMesgQueue( &serialMsgQ, &serialMsg, 1 );
    osSetEventMesg( OS_EVENT_SI, &serialMsgQ, (OSMesg)1 );

    osContInit( &serialMsgQ, &pattern, &gStatusData[ 0 ] );

    osCreateMesgQueue( &controllerMsgQ, &controllerMsgBuf, 1 );
    osSetEventMesg( OS_EVENT_SI, &controllerMsgQ, (OSMesg)0 );

    for( i = 0; i < MAXCONTROLLERS; i++ )
    {
        if ( ( pattern & ( 1 << i ) ) &&
                !( gStatusData[ i ].errno & CONT_NO_RESPONSE_ERROR ) )
        {
	    gActiveController = (u16)i;
            return i;
	}
    }
    return -1;
}

void read_controler( Control *cont )
{
    u16 press, press_up, press_down;

    osContStartReadData( &controllerMsgQ );
    (void)osRecvMesg(&controllerMsgQ, NULL, OS_MESG_BLOCK);
    osContGetReadData( gControllerData);

    cont->button = gControllerData[ gActiveController ].button;
    cont->stick_x = gControllerData[ gActiveController ].stick_x;
    cont->stick_y = gControllerData[ gActiveController ].stick_y;

    press = (u16)(cont->lastbutton ^ cont->button);
    press_up = (u16)(cont->lastbutton & press);
    press_down = (u16)(cont->button & press);
    cont->lastbutton = cont->button;

    cont->button = press_up;
    cont->trigger = press_down;
}







