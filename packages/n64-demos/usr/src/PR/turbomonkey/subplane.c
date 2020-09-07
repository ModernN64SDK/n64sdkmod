
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

#include <ultra64.h>

#include "block.h"
#include "subplane.h"

int PlaneDivisions[2]   = {0, 0};
int PlaneAxisPoints[2]  = {0, 0};
int CurrentPlaneAxis    = 0;

int NumPlanePolys        = 2;

float PlaneDivisionArray[2][MAXPLANEAXISPOINTS];
Vtx Super_Planevtx[MAXPLANEVERTICES];

int PlaneVertexColor[8][3] =
{
  0xff,    0,    0, 
     0, 0x7f,    0,   
     0,    0, 0xff, 
     0,    0,    0, 
     0,    0, 0xff, 
     0,    0,    0,
  0xff,    0,    0,
     0, 0x7f,    0,
};

float PlaneMinMax[2][2] =
{
     20, 300,
  -1500,  -5
};

int NumPlaneVertices;
int NumSubPlanes;

struct PlaneVertexEntry
{
  float Coordinates[2];
} PlaneVertexArray[MAXPLANEVERTICES];

struct SubPlaneEntry
{
  int Index[4];
} SubPlaneArray[MAXSUBPLANES];

void CreatePlaneAxisDivisions(void)
{
  int i, j;

  for (i=0; i<2; i++)
    {
      PlaneAxisPoints[i] = 2 * (PlaneDivisions[i] + 1);
      
      /* Fill out endpoints first    */
      PlaneDivisionArray[i][0] = PlaneMinMax[i][0];
      PlaneDivisionArray[i][PlaneAxisPoints[i]-1] = PlaneMinMax[i][1];
      
      for (j=0; j<PlaneDivisions[i]; j++)
	{
	  PlaneDivisionArray[i][j*2+1] = 
	    PlaneDivisionArray[i][j*2+2] =
	    (PlaneMinMax[i][0] + 
	     (PlaneMinMax[i][1] - PlaneMinMax[i][0]) / 
	    (PlaneDivisions[i] + 1.0) * (j+1.0));
	}
    }
}

void CreatePlaneVertexArray(void)
{
  int i;

  /* Now we compute the vertex array */
  
  NumPlaneVertices = PlaneAxisPoints[0] * PlaneAxisPoints[1];
  
  for (i=0; i<2*NumPlaneVertices; i++)
    {
      PlaneVertexArray[i/2].Coordinates[i%2] = 
	PlaneDivisionArray[i%2][(i%2==0) ? (i/2) % PlaneAxisPoints[0] : 
			       (((i/2) / PlaneAxisPoints[0]) % PlaneAxisPoints[1])];	  
    }
}

void KnitSubPlanes(void)
{
  int i, j, k;
  int PlaneIndexPointer, BaseVertex;

  NumSubPlanes = NumPlaneVertices / 4;

  PlaneIndexPointer = 0;
  for (j=0; j<PlaneAxisPoints[1]; j+=2)
    {
      for (i=0; i<PlaneAxisPoints[0]; i+=2)
	{
	  BaseVertex = j*PlaneAxisPoints[0] + i;
	  
	  SubPlaneArray[PlaneIndexPointer].Index[0] = BaseVertex;
	  SubPlaneArray[PlaneIndexPointer].Index[1] = BaseVertex + 1;
	  SubPlaneArray[PlaneIndexPointer].Index[2] = BaseVertex + 1 + PlaneAxisPoints[0];
	  SubPlaneArray[PlaneIndexPointer].Index[3] = BaseVertex     + PlaneAxisPoints[0];
	  
	  PlaneIndexPointer++;	      
	}
    }
}

void CreatePlaneSuperVertices(void)
{
  int VertexIndexPointer, i, j;

  VertexIndexPointer = 0;
  for (i=0; i<NumSubPlanes; i++)
    {
      for (j=0; j<4; j++)
	{
	  V(Super_Planevtx + VertexIndexPointer,
	    (int) (PlaneVertexArray[SubPlaneArray[i].Index[j]].Coordinates[0] + 
		   (PlaneVertexArray[SubPlaneArray[i].Index[j]].Coordinates[0] > 0 ? 0.5 : -0.5)),
	    20, 
	    (int) (PlaneVertexArray[SubPlaneArray[i].Index[j]].Coordinates[1] + 
		   (PlaneVertexArray[SubPlaneArray[i].Index[j]].Coordinates[1] > 0 ? 0.5 : -0.5)),
	    0, 
	    ((j==0) | (j==3)) ? (0 << 6) : (63 << 6), 
	    ((j==0) | (j==1)) ? (0 << 6) : (255 << 6),
	    PlaneVertexColor[j][0], PlaneVertexColor[j][1], PlaneVertexColor[j][2], 0xff);

	  VertexIndexPointer++;
	}	  
    }
}

void CreateSubPlanes(void)
{
  CreatePlaneAxisDivisions();
  CreatePlaneVertexArray();
  KnitSubPlanes();
  CreatePlaneSuperVertices(); 
}

void PlaneDL(int Offset)
{
  gSP1Triangle(glistp++, Offset+2, Offset+1, Offset+0, Offset+2);
  gSP1Triangle(glistp++, Offset+3, Offset+2, Offset+0, Offset+3);    
}

void WriteSuperPlaneDisplayList(void)
{
  int PlaneIndexPointer, i;

  PlaneIndexPointer = 0;

  /*
   * The texture scaling parameters are .16 format. In order to
   * get an exact texture scale of 1.0, we scale above by 2.0, and
   * scale down here by 0.5
   */
  
  for (i=0; i<NumSubPlanes/4 ; i++)
    {
      
      gSPVertex(glistp++, Super_Planevtx + i*16, 16, 0);

      PlaneDL( 0);
      PlaneDL( 4);
      PlaneDL( 8);
      PlaneDL(12);

      PlaneIndexPointer += 4;      
    }

  if (PlaneIndexPointer != NumSubPlanes)
    {
      if (NumSubPlanes - PlaneIndexPointer == 1)
	{
	  gSPVertex(glistp++, Super_Planevtx + i*16, 4, 0);
	  PlaneDL(0); 
	}
      else if (NumSubPlanes - PlaneIndexPointer == 2)
	{
	  gSPVertex(glistp++, Super_Planevtx + i*16, 8, 0);
	  PlaneDL(0); 
	  PlaneDL(4);
	}
      else
	{
	  gSPVertex(glistp++, Super_Planevtx + i*16, 12, 0);
	  PlaneDL(0); 
	  PlaneDL(4);
	  PlaneDL(8);
	}
    }
}

#include "brick.h"

void CreatePlanes(void)
{
  
  if (!UseGlobalTransforms)
    {
      guTranslate(&dynamicp->TranslateIn, 0.0F, 0.0F, 0.0F); 
      gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamic.TranslateIn)),
		G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
    }

  if (UseTextureMode) {
    gDPLoadTextureBlock(glistp++, brick, G_IM_FMT_RGBA, G_IM_SIZ_16b, 
			32, 32, 0,
			G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR,
			5, 5, G_TX_NOLOD, G_TX_NOLOD);
  }
  
  WriteSuperPlaneDisplayList();
}

