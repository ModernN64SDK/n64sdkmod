#include <ultra64.h>

#include "block.h"
#include "subcube.h"

int BlockWidth         = 30;

int CubeDivisions[3]   = {0, 0, 0};
int CubeAxisPoints[3]  = {0, 0, 0};
int CurrentCubeAxis    = 0;
int NumCubesPerAxis   = 4;

int NumCubePolys      = 0;

float CubeDivisionArray[3][MAXCUBEAXISPOINTS];
Vtx SuperCube_vtx[MAXCUBEVERTICES];

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
	  V(SuperCube_vtx + VertexIndexPointer,
	    (int) (CubeVertexArray[SubBlockArray[i].Index[j]].Coordinates[0] + 
		   (CubeVertexArray[SubBlockArray[i].Index[j]].Coordinates[0] > 0 ? 0.5 : -0.5)),
	    (int) (CubeVertexArray[SubBlockArray[i].Index[j]].Coordinates[1] +
		   (CubeVertexArray[SubBlockArray[i].Index[j]].Coordinates[1] > 0 ? 0.5 : -0.5)),
	    (int) (CubeVertexArray[SubBlockArray[i].Index[j]].Coordinates[2] + 
		   (CubeVertexArray[SubBlockArray[i].Index[j]].Coordinates[2] > 0 ? 0.5 : -0.5)),
	    0, 
	    ((j==3) || (j==7) || (j==0) || (j==4)) ? (31 << 5) : (0 << 5), 
	    ((j==0) || (j==1) || (j==2) || (j==3)) ? (31 << 5) : (0 << 5), 

	    CubeVertexColor[j][0], CubeVertexColor[j][1], CubeVertexColor[j][2], 0xff);

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
    }
}

void WriteSuperBlockDisplayList(int ArrayIndex)
{
    int		i = 0, vtx_count = 0, tri_count = 0;
    gtState	*gtp;

    switch (NumSubCubes) {

      case 1:
	vtx_count = 8;
	tri_count = 12;
	break;

      case 2:
	vtx_count = 16;
	tri_count = 24;
	break;

      case 4:
	vtx_count = 32;
	tri_count = 48;
	break;

      case 8:
	vtx_count = 8;
	tri_count = 12;
	break;

      case 12:
	vtx_count = 8;
	tri_count = 12;
	break;

      default:
	vtx_count = 8;
	tri_count = 12;
	break;

    }


    gtp = &(dynamicp->objState[ArrayIndex]);
    gtp->sp.renderState = GT_CULL_BACK | GT_SHADING_SMOOTH;
    gtp->sp.vtxV0 = 0;
    gtp->sp.vtxCount = vtx_count;
    gtp->sp.triCount = tri_count;
    if (UseAAMode) {
	gtStateSetOthermode(&(gtp->sp.rdpOthermode), GT_RENDERMODE,
			    (G_RM_AA_OPA_SURF | G_RM_AA_OPA_SURF2));
    } else {
	gtStateSetOthermode(&(gtp->sp.rdpOthermode), GT_RENDERMODE,
			    (G_RM_OPA_SURF | G_RM_OPA_SURF2));
    }
    gtStateSetOthermode(&(gtp->sp.rdpOthermode), GT_CYCLETYPE, 
			G_CYC_1CYCLE);
    if (UseTextureMode) {
	gtp->sp.renderState |= GT_TEXTURE;
	gtStateSetOthermode(&(gtp->sp.rdpOthermode), GT_TEXTFILT,
			    G_TF_BILERP);
	gtStateSetOthermode(&(gtp->sp.rdpOthermode), GT_TEXTCONV,
			    G_TC_FILT);
	gtStateSetOthermode(&(gtp->sp.rdpOthermode), GT_TEXTPERSP,
			    G_TP_PERSP);
	/* gDPLoadTextureBlock() happened in the global state */
    }
    
    gtlistp->obj.gstatep = (gtGlobState *) NULL;
    gtlistp->obj.statep = gtp;
    gtlistp->obj.vtxp = (Vtx *) (SuperCube_vtx + i * 16),
    gtlistp->obj.trip = (gtTriN *) cube0_tris;
    gtlistp++;

}

void CreateBlocks(void)
{
    float 	XTranslateAmount, YTranslateAmount, ZTranslateAmount;
    int 	ArrayIndex, i, j, k;
    Mtx 	*mp;
    gtState	*gtp;
    float 	XBinHalfWidth = (XMAX - XMIN)/(NumCubesPerAxis * 2.0);
    float 	YBinHalfWidth = (YMAX - YMIN)/(NumCubesPerAxis * 2.0);
    float 	ZBinHalfWidth = (ZMAX - ZMIN)/(NumCubesPerAxis * 2.0);


    for (i=0; i< NumCubesPerAxis; i++) {		/* Z var */
	for (j=0; j < NumCubesPerAxis; j++) { 		/* Y var */
	    for (k=0; k < NumCubesPerAxis; k++) { 	/* X var */

		XTranslateAmount = XMIN + (2*k+1)*XBinHalfWidth;
		YTranslateAmount = YMIN + (2*j+1)*YBinHalfWidth;
		ZTranslateAmount = ZMIN - (2*(NumCubesPerAxis-1 - i)+1)*ZBinHalfWidth;

		ArrayIndex = (i*NumCubesPerAxis*NumCubesPerAxis + 
			      j*NumCubesPerAxis + k +1);
	  
		gtp = &(dynamicp->objState[ArrayIndex]);
		
		guRotate(&gtp->sp.transform,
			 CubeArray[ArrayIndex-1].theta, 
			 CubeArray[ArrayIndex-1].RotationAxis[0],
			 CubeArray[ArrayIndex-1].RotationAxis[1],
			 CubeArray[ArrayIndex-1].RotationAxis[2]); 
		
		/* force translate elements into rotate matix.
		 */
		mp = (Mtx *) &(gtp->sp.transform);
		mp->m[1][2] = (((short int)XTranslateAmount << 16) | 
			       ((short int)YTranslateAmount));
		mp->m[1][3] = ((short int)ZTranslateAmount << 16) | (1);
		
		guMtxCatL(&gtp->sp.transform, &dynamicp->viewing,
			  &gtp->sp.transform);
		
		CubeArray[ArrayIndex-1].theta += CubeArray[ArrayIndex-1].thetaDelta;

		WriteSuperBlockDisplayList(ArrayIndex);
	    }
	}
    }
}
