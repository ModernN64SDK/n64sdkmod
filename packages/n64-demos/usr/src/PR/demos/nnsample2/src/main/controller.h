/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 2

		Copyright (C) 1997, NINTENDO Co,Ltd.

============================================================================*/
#ifndef CONTROLLER_H
#define CONTROLLER_H
#define CONTROLLER_THREAD_PRI  115
extern OSContStatus statusdata[];
extern OSContPad    controllerdata[];   /* normal Pad data */
extern OSContPad    controllerdataTriger[];  /* toriger Pad data */
extern OSContPad    *controllerdata_ptr[];    /* valid normal Pad data */
extern int          numControllers;


extern int controllerInit(int );        /* initialize controller */
extern void controllerReadStart(void);  /* controller read start */
extern void controllerThreadMake(NNSched*);  /* make controller thread     */

#endif /* CONTROLLER_H */
