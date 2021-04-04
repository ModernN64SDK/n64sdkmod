extern OSMesgQueue	controllerMsgQ;
extern OSMesg		controllerMsgBuf;

extern int initControllers(void);
extern OSContStatus     statusdata[MAXCONTROLLERS];
extern OSContPad        controllerdata[MAXCONTROLLERS];

extern int initControllers(void);
extern void ReadController(void);

extern int ControllerMode;
