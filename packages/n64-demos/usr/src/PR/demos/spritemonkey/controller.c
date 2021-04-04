
/*---------------------------------------------------------------------*
        Copyright (C) 1997 Nintendo. (Originated by SGI)
        
        $RCSfile: controller.c,v $
        $Revision: 1.7 $
        $Date: 1999/01/19 04:44:48 $
 *---------------------------------------------------------------------*/
#include <ultra64.h>

#include <ramrom.h>

/* app specific includes */
#include "sprite.h"

OSMesgQueue	controllerMsgQ;
OSMesg		controllerMsgBuf;

int initControllers(void);
OSContStatus     statusdata[MAXCONTROLLERS];
OSContPad        controllerdata[MAXCONTROLLERS];

static u16       lastbutton;

/*
 *
 * Return the lowest number controller connected to system
 */
int initControllers()
{
    OSMesgQueue     serialMsgQ;
    OSMesg          serialMsg;
    int             i;
    u8              pattern;

    osCreateMesgQueue(&serialMsgQ, &serialMsg, 1);
    osSetEventMesg(OS_EVENT_SI, &serialMsgQ, (OSMesg)1);

    osContInit(&serialMsgQ, &pattern, &statusdata[0]);

    osCreateMesgQueue(&controllerMsgQ, &controllerMsgBuf, 1);
    osSetEventMesg(OS_EVENT_SI, &controllerMsgQ, (OSMesg)0);

    for (i = 0; i < MAXCONTROLLERS; i++) {
        if ((pattern & (1<<i)) &&
                !(statusdata[i].errno & CONT_NO_RESPONSE_ERROR))
            return i;
    }
    return -1;
}

#define JUMPY  0
#define SMOOTH 1

static u16 button;
int ButtonCheck(int value, int Smooth)
{
  if (!Smooth) return ((button & value) && !(lastbutton & value));
  else return (button & value);
}

void ReadController1(void)
{
  static s16 StickX, StickY;      
  int MaxS, MaxT;
    
  (void)osRecvMesg(&controllerMsgQ, NULL, OS_MESG_BLOCK);
  osContGetReadData(controllerdata);

  button = controllerdata[ActiveController].button;
  StickX = controllerdata[ActiveController].stick_x;
  StickY = controllerdata[ActiveController].stick_y;

  MaxS = ImageWidth  - 1 - (RectangleWidth - 1);
  MaxT = ImageHeight - RectangleHeight;

  if (ButtonCheck(CONT_L, JUMPY))
    {
      FlipTextureX ^= 1;

      ControllerInput = 1;
    }

  if (ButtonCheck(CONT_R, JUMPY))
    {
      FlipTextureY ^= 1;
      
      ControllerInput = 1;
    }

  if (ButtonCheck(CONT_A, JUMPY))
    {
      TextureNumber++;

      TextureNumber %= MAXTEXTURES;

      NewTextureNumber = 1;

      ControllerInput  = 1;
    }

  if (ButtonCheck(CONT_C, SMOOTH))
    {
      RectangleWidth --;

      if (RectangleWidth < 1) RectangleWidth = 1;
      ControllerInput = 1;
    }

  if (ButtonCheck(CONT_E, SMOOTH))
    {
      RectangleHeight--;

      if (RectangleHeight < 1) RectangleHeight = 1;

      ControllerInput = 1;
    }

  if (ButtonCheck(CONT_F, SMOOTH))
    {
      RectangleWidth ++;

      if (RectangleWidth > ImageWidth) RectangleWidth = ImageWidth;

      ControllerInput = 1;
    }

  if (ButtonCheck(CONT_D, SMOOTH))
    {
      RectangleHeight++;

      if (RectangleHeight > ImageHeight) RectangleHeight = ImageHeight;

      ControllerInput = 1;
    }

  if (StickX > 15) 
    {
      TranslateHorizontal+=4;

      ControllerInput = 1;
    }
 
  else if (StickX < -15) 
    {
      TranslateHorizontal-=4;

      ControllerInput = 1;
    }

  if (StickY < -15)  
    {
      TranslateVertical+=4;

      ControllerInput = 1;
    }

  else if (StickY > 15)
    {
      TranslateVertical-=4;

      ControllerInput = 1;
    }

  if (ButtonCheck(CONT_RIGHT, SMOOTH))
    {
      TextureStartS++;

      if (TextureStartS > MaxS) TextureStartS = MaxS;

      ControllerInput = 1;
    }
  else if (ButtonCheck(CONT_LEFT, SMOOTH))
    {
      TextureStartS--;

      if (TextureStartS < 0) TextureStartS = 0;

      ControllerInput = 1;
    }

  if (ButtonCheck(CONT_DOWN, SMOOTH))
    {
      TextureStartT++;

      if (TextureStartT > MaxT) TextureStartT = MaxT;

      ControllerInput = 1;
    }
  else if (ButtonCheck(CONT_UP, SMOOTH))
    {
      TextureStartT--;
      
      if (TextureStartT < 0) TextureStartT = 0;

      ControllerInput = 1;
    }

  if (ButtonCheck(CONT_G, JUMPY))
    {
      ControllerMode++;

      if (PrintInstructions) 
	{
	    ClearScreen();
	}
      ControllerInput = 1;
    }

  if (ButtonCheck(CONT_START, JUMPY))
    {
      InitVariables();
      (*(TestStruct[0]).TestInitFunc)();

      ControllerInput       = 1;
    }
	    
  lastbutton = button;
}

void ReadController2(void)
{
  static s16 StickX, StickY;      
    
  (void)osRecvMesg(&controllerMsgQ, NULL, OS_MESG_BLOCK);
  osContGetReadData(controllerdata);

  button = controllerdata[ActiveController].button;
  StickX = controllerdata[ActiveController].stick_x;
  StickY = controllerdata[ActiveController].stick_y;

  if (StickX > 15) 
    {
      TranslateHorizontal+=4;

      ControllerInput = 1;
    }
 
  else if (StickX < -15) 
    {
      TranslateHorizontal-=4;

      ControllerInput = 1;
    }

  if (StickY < -15)  
    {
      TranslateVertical+=4;

      ControllerInput = 1;
    }

  else if (StickY > 15)
    {
      TranslateVertical-=4;

      ControllerInput = 1;
    }

  if (ButtonCheck(CONT_L, JUMPY))
    {
      FlipTextureX ^= 1;

      ControllerInput = 1;
    }

  if (ButtonCheck(CONT_R, JUMPY))
    {
      FlipTextureY ^= 1;
      
      ControllerInput = 1;
    }
  if (ButtonCheck(CONT_C, SMOOTH))
    {
      RectangleWidth --;

      if (RectangleWidth < 1) RectangleWidth = 1;
      ControllerInput = 1;
    }

  if (ButtonCheck(CONT_E, SMOOTH))
    {
      RectangleHeight--;

      if (RectangleHeight < 1) RectangleHeight = 1;

      ControllerInput = 1;
    }

  if (ButtonCheck(CONT_F, SMOOTH))
    {
      RectangleWidth ++;

      if (RectangleWidth > ImageWidth) RectangleWidth = ImageWidth;

      ControllerInput = 1;
    }

  if (ButtonCheck(CONT_D, SMOOTH))
    {
      RectangleHeight++;

      if (RectangleHeight > ImageHeight) RectangleHeight = ImageHeight;

      ControllerInput = 1;
    }


/** Swap controller RIGHT <-> LEFT **/
/*  if (ButtonCheck(CONT_RIGHT, SMOOTH)) */
  if (ButtonCheck(CONT_LEFT, SMOOTH))
    {
      TextureScaleX += (TextureScaleX >> 5 ? TextureScaleX >> 5 : 1);
      if (TextureScaleX > 32767) TextureScaleX = 32767;

      ControllerInput = 1;
    }
/*  else if (ButtonCheck(CONT_LEFT, SMOOTH)) */
  else if (ButtonCheck(CONT_RIGHT, SMOOTH))
    {
      TextureScaleX -= (TextureScaleX >> 5 ? TextureScaleX >> 5 : 1);

      if (TextureScaleX < 1) TextureScaleX = 1;

      ControllerInput = 1;
    }

  if (ButtonCheck(CONT_DOWN, SMOOTH))
    {
      TextureScaleY -= (TextureScaleY >> 5 ? TextureScaleY >> 5 : 1);

      if (TextureScaleY < 1) TextureScaleY = 1;

      ControllerInput = 1;
    }
  else if (ButtonCheck(CONT_UP, SMOOTH))
    {
      TextureScaleY += (TextureScaleY >> 5 ? TextureScaleY >> 5 : 1);
      if (TextureScaleY > 32767) TextureScaleY = 32767;

      ControllerInput = 1;
    }


  if (ButtonCheck(CONT_G, JUMPY))
    {
      ControllerMode= 1;

      if (PrintInstructions) 
	{
	    ClearScreen();
	}
      ControllerInput = 1;
    }

  if (ButtonCheck(CONT_START, JUMPY))
    {
      InitVariables();
      (*(TestStruct[0]).TestInitFunc)();

      ControllerInput       = 1;
    }
	    
  lastbutton = button;
}

void ReadController()
{
  if (ControllerMode == 1)
    ReadController1();
  else if (ControllerMode == 2)
    ReadController2();
  else 

#ifdef DEBUG
    PRINTF("ERROR: invalid ControllerMode %d \n", ControllerMode);
#else
    ;
#endif

}
