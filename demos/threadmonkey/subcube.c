#include <ultra64.h>

#include "block.h"
#include "subcube.h"

int BlockWidth         = 30;
float BlockWidthVariation= 0;

int CubeDivisions[3]   = {0, 0, 0};
int CubeAxisPoints[3]  = {0, 0, 0};
int CurrentCubeAxis    = 0;
int NumCubesPerAxis   = 4;

int NumCubePolys      = 0;

float CubeDivisionArray[3][MAXCUBEAXISPOINTS];
Vtx ColorCube_vtx[MAXCUBEVERTICES];
Vtx NormalCube_vtx[MAXCUBEVERTICES];

int CubeVertexColor[8][3] =
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

int CubeVertexNormal[8][3] =
{
     0,  127,    0, /* 0 */
     0,    0, -128, /* 1 */
     0,    0,    0, /* 2 */
     0,    0,  127, /* 3 */
     0, -128,    0, /* 4 */
   127,    0,    0, /* 5 */
     0,    0,    0, /* 6 */
  -128,    0,    0, /* 7 */
};


int NumCubeVertices;
int NumSubCubes;

struct PlaneVertexEntry
{
  float Coordinates[3];
} CubeVertexArray[MAXCUBEVERTICES];

struct SubBlockEntry
{
  int Index[8];
} SubBlockArray[MAXSUBBLOCKS];

Vtx Block_vtx[8];

Cube CubeArray[MAXBLOCKS];

void CreateCubeAxisDivisions(void)
{
  int i, j;

  for (i=0; i<3; i++)
    {
      CubeAxisPoints[i] = 2 * (CubeDivisions[i] + 1);
      
      /* Fill out endpoints first    */
      CubeDivisionArray[i][0] = (i == 1 ?  BlockWidth/2 : -BlockWidth/2);
      CubeDivisionArray[i][CubeAxisPoints[i]-1] = (i == 1 ? -BlockWidth/2 : BlockWidth/2);
      
      for (j=0; j<CubeDivisions[i]; j++)
	{
	  CubeDivisionArray[i][j*2+1] = 
	    CubeDivisionArray[i][j*2+2] = 
	    (-BlockWidth/2 + BlockWidth / 
	    (CubeDivisions[i] + 1.0) * (j+1.0)) *
	    (i == 1 ? -1.0 : 1.0);
	}
    }
}

void CreateCubeVertexArray(void)
{
  int i;

  /* Now we compute the vertex array */
  
  NumCubeVertices = CubeAxisPoints[0] * CubeAxisPoints[1] * CubeAxisPoints[2];
  
  for (i=0; i<3*NumCubeVertices; i++)
    {
      CubeVertexArray[i/3].Coordinates[i%3] = 
	CubeDivisionArray[i%3][(i%3==0) ? (i/3) % CubeAxisPoints[0] : 
			  ((i%3==1) ? ((i/3) / CubeAxisPoints[0]) % CubeAxisPoints[1] : 
			   (i/3) / (CubeAxisPoints[0] * CubeAxisPoints[1]))];	  
    }
}

void KnitSubCubes(void)
{
  int i, j, k;
  int BlockIndexPointer, BaseVertex;

  NumSubCubes = NumCubeVertices / 8;

  BlockIndexPointer = 0;
  for (k=0; k<CubeAxisPoints[2]; k+=2)
    {
      for (j=0; j<CubeAxisPoints[1]; j+=2)
	{
	  for (i=0; i<CubeAxisPoints[0]; i+=2)
	    {
	      BaseVertex = k*CubeAxisPoints[1]*CubeAxisPoints[0] + j*CubeAxisPoints[0] + i;

	      SubBlockArray[BlockIndexPointer].Index[0] = BaseVertex                                                     ;
	      SubBlockArray[BlockIndexPointer].Index[1] = BaseVertex + 1                                                 ;
	      SubBlockArray[BlockIndexPointer].Index[2] = BaseVertex + 1                  + CubeAxisPoints[0] * CubeAxisPoints[1];
	      SubBlockArray[BlockIndexPointer].Index[3] = BaseVertex                      + CubeAxisPoints[0] * CubeAxisPoints[1];       
	      SubBlockArray[BlockIndexPointer].Index[4] = BaseVertex      + CubeAxisPoints[0]                                ;
	      SubBlockArray[BlockIndexPointer].Index[5] = BaseVertex + 1  + CubeAxisPoints[0]                                ;
	      SubBlockArray[BlockIndexPointer].Index[6] = BaseVertex + 1  + CubeAxisPoints[0] + CubeAxisPoints[0] * CubeAxisPoints[1];
	      SubBlockArray[BlockIndexPointer].Index[7] = BaseVertex      + CubeAxisPoints[0] + CubeAxisPoints[0] * CubeAxisPoints[1];

	      BlockIndexPointer++;	      
	    }
	}
    }
}

void CreateCubeSuperVertices(void)
{
  int VertexIndexPointer, i, j;
  
  VertexIndexPointer = 0;
  for (i=0; i<NumSubCubes; i++)
    {
      for (j=0; j<8; j++)
	{
	  V(ColorCube_vtx + VertexIndexPointer,
	    (int) (CubeVertexArray[SubBlockArray[i].Index[j]].Coordinates[0] + 
		   (CubeVertexArray[SubBlockArray[i].Index[j]].Coordinates[0] > 0 ? 0.5 : -0.5)),
	    (int) (CubeVertexArray[SubBlockArray[i].Index[j]].Coordinates[1] +
		   (CubeVertexArray[SubBlockArray[i].Index[j]].Coordinates[1] > 0 ? 0.5 : -0.5)),
	    (int) (CubeVertexArray[SubBlockArray[i].Index[j]].Coordinates[2] + 
		   (CubeVertexArray[SubBlockArray[i].Index[j]].Coordinates[2] > 0 ? 0.5 : -0.5)),
	    0, 
	    ((j==3) || (j==7) || (j==0) || (j==4)) ? (31 << 6) : (0 << 6), 
	    ((j==0) || (j==1) || (j==2) || (j==3)) ? (31 << 6) : (0 << 6), 
	    CubeVertexColor[j][0], CubeVertexColor[j][1], CubeVertexColor[j][2], 0xff);


	  V(NormalCube_vtx + VertexIndexPointer,
	    (int) (CubeVertexArray[SubBlockArray[i].Index[j]].Coordinates[0] + 
		   (CubeVertexArray[SubBlockArray[i].Index[j]].Coordinates[0] > 0 ? 0.5 : -0.5)),
	    (int) (CubeVertexArray[SubBlockArray[i].Index[j]].Coordinates[1] +
		   (CubeVertexArray[SubBlockArray[i].Index[j]].Coordinates[1] > 0 ? 0.5 : -0.5)),
	    (int) (CubeVertexArray[SubBlockArray[i].Index[j]].Coordinates[2] + 
		   (CubeVertexArray[SubBlockArray[i].Index[j]].Coordinates[2] > 0 ? 0.5 : -0.5)),
	    0, 
	    ((j==3) || (j==7) || (j==0) || (j==4)) ? (31 << 6) : (0 << 6), 
	    ((j==0) || (j==1) || (j==2) || (j==3)) ? (31 << 6) : (0 << 6), 
	    CubeVertexNormal[j][0], CubeVertexNormal[j][1], CubeVertexNormal[j][2], 0xff);

	  VertexIndexPointer++;
	}	  
    }
}

void CreateSubCubes(void)
{
  CreateCubeAxisDivisions();
  CreateCubeVertexArray();
  KnitSubCubes();
  CreateCubeSuperVertices(); 
}

void InitCubeRotations(void)
{
  int i, j;
  float AxisLength, RandomValue, FlipAxis;

  for (i=0; i<MAXBLOCKSPERAXIS*MAXBLOCKSPERAXIS*MAXBLOCKSPERAXIS; i++)
    {
      AxisLength = 0;
      for (j=0; j<3; j++)
	{
	  RandomValue = frand();
	  CubeArray[i].RotationAxis[j] = RandomValue;
	  AxisLength += RandomValue * RandomValue;
	}

      FlipAxis = (frand() > 0.5) ? -1.0 : 1.0;

      AxisLength = sqrtf(AxisLength) * FlipAxis;

      for (j=0; j<3; j++)
	{
	  CubeArray[i].RotationAxis[j] /= AxisLength;
	}

      CubeArray[i].theta = 0;
      CubeArray[i].thetaDelta = 1.0 + 6.5*frand();

      CubeArray[i].scale = frand();
    }
}

void BlockDL(int Offset)
{
  /* Top */
  gSP1Triangle(glistp++, Offset+2, Offset+1, Offset+0, 2);
  gSP1Triangle(glistp++, Offset+3, Offset+2, Offset+0, 2);    
		       	         		       
  /* Bottom */	       	         		       
  gSP1Triangle(glistp++, Offset+4, Offset+5, Offset+6, 0);    
  gSP1Triangle(glistp++, Offset+4, Offset+6, Offset+7, 0);    
		       	         		       
  /* Left */	       	         		       
  gSP1Triangle(glistp++, Offset+7, Offset+3, Offset+0, 0);    
  gSP1Triangle(glistp++, Offset+4, Offset+7, Offset+0, 1);    
		       	         		       
  /* Right */	       	         		       
  gSP1Triangle(glistp++, Offset+5, Offset+1, Offset+2, 0);    
  gSP1Triangle(glistp++, Offset+6, Offset+5, Offset+2, 1);    
		       	         		       
  /* Near */	       	         		       
  gSP1Triangle(glistp++, Offset+6, Offset+2, Offset+3, 2);    
  gSP1Triangle(glistp++, Offset+7, Offset+6, Offset+3, 2);    
		       	         		       
  /* Far */	       	         		       
  gSP1Triangle(glistp++, Offset+4, Offset+0, Offset+1, 2);    
  gSP1Triangle(glistp++, Offset+5, Offset+4, Offset+1, 2);    
}

void WriteSuperBlockDisplayList(void)
{
  int BlockIndexPointer, i;

  BlockIndexPointer = 0;

  for (i=0; i<NumSubCubes/2 ; i++)
    {
      gSPVertex(glistp++, ((ShadingMode == LIGHTING) ? NormalCube_vtx : ColorCube_vtx) + i*16, 16, 0)

      BlockDL(0);
      BlockDL(8);

      BlockIndexPointer += 2;      
    }
  
  if (BlockIndexPointer != NumSubCubes)
    {      
      gSPVertex(glistp++, ((ShadingMode == LIGHTING) ? NormalCube_vtx : ColorCube_vtx) + i*16, 8, 0);
      BlockDL(0); 
    }
}

#include "../Texture/RGBA16mario.h"
#include "../Texture/RGBA16marioMM.h"

void CreateBlocks(int n)
{
  float XTranslateAmount, YTranslateAmount, ZTranslateAmount;
  int ArrayIndex, i, j, k;
  Mtx *mp;
  float rotmat[4][4], scalemat[4][4], scale;
  int   totalcubes,cubespertask,cubenum;

  float XBinHalfWidth = (XMAX - XMIN)/(NumCubesPerAxis * 2.0);
  float YBinHalfWidth = (YMAX - YMIN)/(NumCubesPerAxis * 2.0);
  float ZBinHalfWidth = (ZMAX - ZMIN)/(NumCubesPerAxis * 2.0);

  if (UseTextureMode && n==1) 
    {
      if (UseTextureMode == 2)
	{    
	  gSPDisplayList(glistp++, RGBA16marioMM_dl);
	}
      else
	{	
	  gDPLoadTextureBlock(glistp++, RGBA16mario, G_IM_FMT_RGBA, 
			      G_IM_SIZ_16b, 32, 32, 0,
			      G_TX_WRAP | G_TX_MIRROR, G_TX_WRAP | G_TX_MIRROR,
			      5, 5, G_TX_NOLOD, G_TX_NOLOD);
      }
    }

  totalcubes = NumCubesPerAxis*NumCubesPerAxis*NumCubesPerAxis;
  cubespertask = totalcubes/((numtasks==1)?1:(numtasks-1));
  for (i=0; i< NumCubesPerAxis; i++) /* Z var */
    for (j=0; j < NumCubesPerAxis; j++) /* Y var */
      for (k=0; k < NumCubesPerAxis; k++) /* X var */
	{
	  cubenum = k + (j + i*NumCubesPerAxis)*NumCubesPerAxis;
	  if (numtasks<3 || 
		(cubenum < cubespertask && n==2) ||
		(cubenum >= cubespertask * (numtasks-1) && n==numtasks) ||
		(cubenum >= cubespertask * (n-2) && 
		 cubenum < cubespertask * (n-1))) {
	    XTranslateAmount = XMIN + (2*k+1)*XBinHalfWidth;
	    YTranslateAmount = YMIN + (2*j+1)*YBinHalfWidth;
	    ZTranslateAmount = ZMIN - (2*(NumCubesPerAxis-1 - i)+1)*ZBinHalfWidth;
	  
	    ArrayIndex = i*NumCubesPerAxis*NumCubesPerAxis + j*NumCubesPerAxis + k +1;
	  
	    scale = 1 + BlockWidthVariation * (CubeArray[ArrayIndex-1].scale -1);
	    guRotateF(rotmat,
		   CubeArray[ArrayIndex-1].theta, 
		   CubeArray[ArrayIndex-1].RotationAxis[0],
		   CubeArray[ArrayIndex-1].RotationAxis[1],
		   CubeArray[ArrayIndex-1].RotationAxis[2]); 
	    guScaleF(scalemat,
		   scale,scale,scale);
	    guMtxCatF(rotmat,scalemat,scalemat);
	    guMtxF2L(scalemat,&dynamicp->modeling4[ArrayIndex]);
  
	  
	    CubeArray[ArrayIndex-1].theta += CubeArray[ArrayIndex-1].thetaDelta;

	    /* force translate elements into rotate matix so we cut 
	     * out an extra matrix-matrix multiply in the rsp */

            mp = (Mtx *) &(dynamic.modeling4[ArrayIndex]);
            mp->m[1][2] = (((short int)XTranslateAmount << 16) | 
			 ((short int)YTranslateAmount));
            mp->m[1][3] = ((short int)ZTranslateAmount << 16) | (1);

	    if (UseGlobalTransforms)
	      {
	        gSPMatrix(glistp++, 
			OS_K0_TO_PHYSICAL(&(dynamic.modeling4[ArrayIndex])),
			G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_PUSH);
	      }
	    else
	      {
	        gSPMatrix(glistp++, 
			OS_K0_TO_PHYSICAL(&(dynamic.modeling4[ArrayIndex])),
			G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
	      }
	  
	    WriteSuperBlockDisplayList();

	    if (UseGlobalTransforms)
	      {
	        /* Now pop back to the global transformation state */

	        gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
	      }
          }

	}
}
