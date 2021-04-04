
/**************************************************************************
 *                                                                        *
 *               Copyright (C) 1995, Silicon Graphics, Inc.               *
 *                                                                        *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright  law.  They  may not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *                                                                        *
 *************************************************************************/

/*---------------------------------------------------------------------*
        Copyright (C) 1997 Nintendo. (Originated by SGI)
        
        $RCSfile: controller.c,v $
        $Revision: 1.3 $
        $Date: 1998/09/25 21:51:47 $
 *---------------------------------------------------------------------*/

#include <ultra64.h>

#include <ramrom.h>

/* app specific includes */
#include "block.h"
#include "subcube.h"
#include "subplane.h"
/*
#include "timer.h"
*/

OSMesgQueue	controllerMsgQ;
OSMesg		controllerMsgBuf;

int initControllers(void);
OSContStatus     statusdata[MAXCONTROLLERS];
OSContPad        controllerdata[MAXCONTROLLERS];

static u16       lastbutton;

int ControllerMode = 0;

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

void ResetParameters(void)
{
  UseViModeX          =         0;
  ScreenWidthDelta    =         0;

  UseAAMode           =         0;
  BlockWidth          = CUBEWIDTH;
  CurrentCubeAxis     =         0;
  CurrentPlaneAxis    =         0;
  NumCubesPerAxis     =         4;
  TimerTicks          =         7;
  
  CubeDivisions[0]    =         0;
  CubeDivisions[1]    =         0;
  CubeDivisions[2]    =         0;

  PlaneDivisions[0]   =         0;
  PlaneDivisions[1]   =         0;
  
  PlaneMinMax[0][0]   = PLANEMINX;
  PlaneMinMax[0][1]   = PLANEMAXX;
  PlaneMinMax[1][0]   = PLANEMINZ;
  PlaneMinMax[1][1]   = PLANEMAXZ;
  
  EyeZPosition        =         0;
  
  TranslateHorizontal =         0;
  TranslateVertical   =         0;
  
  CreateNewCubes      =         1;
  CreateNewPlanes     =         1;
  
  RotateGlobalXAxis   =         0;
  RotateGlobalYAxis   =         0;

  UseTextureMode      =         0;
  UseGlobalTransforms =         1;
}

void ReadController0(void)
{
  static u16 button;
  static s16 stickX, stickY;      
    
  (void)osRecvMesg(&controllerMsgQ, NULL, OS_MESG_BLOCK);
  osContGetReadData(controllerdata);

  button = controllerdata[ActiveController].button;
  stickX = controllerdata[ActiveController].stick_x;
  stickY = controllerdata[ActiveController].stick_y;

  if (button & CONT_UP)
    {
      BlockWidth+=3;

      CreateNewCubes  = 1;
      ControllerInput = 1;
    }
	    
  if (button & CONT_DOWN)
    {
      BlockWidth-=3;

      if (BlockWidth < 1)
	BlockWidth = 1;

      CreateNewCubes  = 1;
      ControllerInput = 1;
    }

  if (button & CONT_RIGHT)
    {
      /* Expand Plane */
/*
      PlaneMinMax[0][0]-=4;
      PlaneMinMax[0][1]+=4;

      PlaneMinMax[1][0]-= 4.0 * (float) (- PLANEMINZ +PLANEMAXZ) / (float) (PLANEMAXX - PLANEMINX);
      PlaneMinMax[1][1]+= 4.0 * (float) (- PLANEMINZ +PLANEMAXZ) / (float) (PLANEMAXX - PLANEMINX);

      CreateNewPlanes = 1;
*/
      ControllerInput = 1;
    }
	    
  if (button & CONT_LEFT)
    {
      /* Shrink Plane */
      
/*
      if (! (PlaneMinMax[0][0] >= PlaneMinMax[0][1]))
	{
	  PlaneMinMax[0][0]+=4;
	  PlaneMinMax[0][1]-=4;

	  PlaneMinMax[1][0]+= 4.0 * (float) (- PLANEMINZ +PLANEMAXZ) / (float) (PLANEMAXX - PLANEMINX);
	  PlaneMinMax[1][1]-= 4.0 * (float) (- PLANEMINZ +PLANEMAXZ) / (float) (PLANEMAXX - PLANEMINX);
	}

      CreateNewPlanes = 1;
*/
      ControllerInput = 1;
    }


  if ((button & CONT_A) && !(lastbutton & CONT_A))
    {
      UseAAMode = 1 - UseAAMode;
      ControllerInput = 1;
    }

  if ((button & CONT_B) && !(lastbutton & CONT_B))
    {
      UseZMode = 1 - UseZMode;
      ControllerInput = 1;
    }

  if ((button & CONT_C) && !(lastbutton & CONT_C))
    {
      if (CubeDivisions[(CurrentCubeAxis + 1) % 2] != 0)
	{	  
	  CurrentCubeAxis+=1;
	  CurrentCubeAxis %= 2; 

	  CubeDivisions[CurrentCubeAxis]--;
	}

      CreateNewCubes  = 1;
      ControllerInput = 1;
    }

  if ((button & CONT_F) && !(lastbutton & CONT_F))
    {      
      if (CubeDivisions[CurrentCubeAxis] != MAXCUBEAXISDIVISIONS)	
	{
	  CubeDivisions[CurrentCubeAxis]++;

	  CurrentCubeAxis++;
	  CurrentCubeAxis %= 2; 
	}

      CreateNewCubes  = 1;
      ControllerInput = 1; 
    }

  if ((button & CONT_D) && !(lastbutton & CONT_D))
    {
/*
      if (PlaneDivisions[(CurrentPlaneAxis + 1) % 2] != 0)
	{	  
	  CurrentPlaneAxis+=1;
	  CurrentPlaneAxis %= 2; 

	  PlaneDivisions[CurrentPlaneAxis]--;
	}

      CreateNewPlanes  = 1;
*/
      ControllerInput  = 1;
    }

  if ((button & CONT_E) && !(lastbutton & CONT_E))
    {      
/*
      if (PlaneDivisions[CurrentPlaneAxis] != MAXPLANEAXISDIVISIONS)	
	{
	  PlaneDivisions[CurrentPlaneAxis]++;

	  CurrentPlaneAxis++;
	  CurrentPlaneAxis %= 2; 
	}

      CreateNewPlanes  = 1;
*/
      ControllerInput  = 1; 
    }


  if ((button & CONT_L) && !(lastbutton & CONT_L))
    {      
      if (NumCubesPerAxis > 0)
	{
	  NumCubesPerAxis--;
	  ControllerInput = 1; 
	}
    }

  if ((button & CONT_R) && !(lastbutton & CONT_R))
    {      
      if (NumCubesPerAxis < MAXBLOCKSPERAXIS)
	{
	  NumCubesPerAxis++;
	  ControllerInput = 1; 
	}
    }

  if ((button & CONT_G) && !(lastbutton & CONT_G))
    {
      /* ControllerMode  = 1 - ControllerMode; */
      ControllerMode++;
      if (ControllerMode >= MAXCONTROLLERMODE)
        ControllerMode = 0;
      ControllerInput = 1;
    }

  if ((button & CONT_START) && !(lastbutton & CONT_START))
    {
      ResetParameters();
      ControllerInput = 1;
    }
  
  lastbutton = button;
}

void ReadController1(void)
{
  static u16 button;
  static s16 stickX, stickY;      
    
  (void)osRecvMesg(&controllerMsgQ, NULL, OS_MESG_BLOCK);
  osContGetReadData(controllerdata);

  button = controllerdata[ActiveController].button;
  stickX = controllerdata[ActiveController].stick_x;
  stickY = controllerdata[ActiveController].stick_y;

  if (stickX > 15)
    {
      if (UseGlobalTransforms) TranslateHorizontal+=3;      
    }
  else if (stickX < -15)
    {
      if (UseGlobalTransforms) TranslateHorizontal-=3;
    }

  if (stickY > 15)
    {
      if (UseGlobalTransforms) TranslateVertical+=3;      
    }
  else if (stickY < -15)
    {
      if (UseGlobalTransforms) TranslateVertical-=3;
    }

  if (button & CONT_UP)
    {
      EyeZPosition+=8;
    }
	    
  if (button & CONT_DOWN)
    {
      EyeZPosition-=8;
    }

  if (button & CONT_E)
    {
      if (UseGlobalTransforms) RotateGlobalXAxis--;
    }

  if (button & CONT_D)
    {
      if (UseGlobalTransforms) RotateGlobalXAxis++;
    }

  if (button & CONT_F)
    {
      if (UseGlobalTransforms) RotateGlobalYAxis++;
    }

  if (button & CONT_C)
    {
      if (UseGlobalTransforms) RotateGlobalYAxis--;
    }

  if ((button & CONT_G) && !(lastbutton & CONT_G))
    {
      /* ControllerMode  = 1 - ControllerMode; */
      ControllerMode++;
      if (ControllerMode >= MAXCONTROLLERMODE)
        ControllerMode = 0;
      ControllerInput = 1;
    }

  if ((button & CONT_A) && !(lastbutton & CONT_A))
    {
      UseGlobalTransforms = 1 - UseGlobalTransforms;
      ControllerInput = 1;      
    }

  if ((button & CONT_B) && !(lastbutton & CONT_B))
    {
      UseTextureMode  = 1 - UseTextureMode;
      ControllerInput = 1;      
    }

  if ((button & CONT_L) && !(lastbutton & CONT_L))
    {      
      if (ClipRatio > 1)
	{
	  ClipRatio--;
	  ControllerInput = 1;      
	}
    }

  if ((button & CONT_R) && !(lastbutton & CONT_R))
    {      
      if (ClipRatio < MAXCLIPRATIO)
	{
	  ClipRatio++;
	  ControllerInput = 1;      
	}
    }


  if ((button & CONT_START) && !(lastbutton & CONT_START))
    {
      ResetParameters();
      ControllerInput = 1;
    }
  
  lastbutton = button;
}

void ReadController2(void)
{
  static u16 button;
    
  (void)osRecvMesg(&controllerMsgQ, NULL, OS_MESG_BLOCK);
  osContGetReadData(controllerdata);

  button = controllerdata[ActiveController].button;

  /* Toggle between Xpn1 and Lpn1 */
  /* Arrow Up */
  if ((button & CONT_E) && !(lastbutton & CONT_E))
    {
      UseViModeX  = 1 - UseViModeX;
      ControllerInput = 1;      
    }

  /* Arrow Down */
  if (button & CONT_D)
    {
    }

  /* Arrow Right */
  if ((button & CONT_F) && !(lastbutton & CONT_F))
    {
      ScreenWidthDelta += 4;
      if (ScreenWidthDelta > (SCREEN_WD_MAX-SCREEN_WD_MIN))
          ScreenWidthDelta = SCREEN_WD_MAX-SCREEN_WD_MIN;
      ControllerInput = 1;
    }

  /* Arrow Left */
  if ((button & CONT_C) && !(lastbutton & CONT_C))
    {
      ScreenWidthDelta -= 4;
      if (ScreenWidthDelta < 0)
          ScreenWidthDelta = 0;
      ControllerInput = 1;
    }

  if ((button & CONT_A) && !(lastbutton & CONT_A))
    {
    }

  if ((button & CONT_B) && !(lastbutton & CONT_B))
    {
    }

  if ((button & CONT_G) && !(lastbutton & CONT_G))
    {
      /* ControllerMode  = 1 - ControllerMode; */
      ControllerMode++;
      if (ControllerMode >= MAXCONTROLLERMODE)
        ControllerMode = 0;
      ControllerInput = 1;
    }

  if ((button & CONT_START) && !(lastbutton & CONT_START))
    {
      ResetParameters();
      ControllerInput = 1;
    }
  
  lastbutton = button;
}


void ReadController(void)
{
  if (ControllerMode == 0)
    ReadController0();
  else if (ControllerMode == 1)
    ReadController1();
  else
    ReadController2();
}
