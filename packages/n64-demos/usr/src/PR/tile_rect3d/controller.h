
#ifndef _CONTROLLER_INCLD
#define _CONTROLLER_INCLD

extern OSMesgQueue	controllerMsgQ;
extern OSMesg		controllerMsgBuf;

extern int initControllers(void);
extern OSContStatus     statusdata[MAXCONTROLLERS];
extern OSContPad        controllerdata[MAXCONTROLLERS];

extern float CursorX, CursorY;
extern int  ButtonA;
extern int  FilterEn;

extern int initControllers(void);
extern void ReadController(void);


#endif /* _CONTROLLER_INCLD */
