extern OSMesgQueue	controllerMsgQ;
extern OSMesg		controllerMsgBuf;

extern OSContStatus     statusdata[MAXCONTROLLERS];
extern OSContPad        controllerdata[MAXCONTROLLERS];

extern int initControllers(void);
extern void readController(int);

extern float	shadowScaleX;
extern float	shadowScaleZ;
extern float	shadowTranslateX;
extern float	shadowTranslateZ;

