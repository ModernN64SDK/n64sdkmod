
#define MAXCUBEAXISDIVISIONS 1
#define MAXCUBEAXISPOINTS    2*(MAXCUBEAXISDIVISIONS + 1) 

#define MAXCUBEVERTICES      MAXCUBEAXISPOINTS * MAXCUBEAXISPOINTS * MAXCUBEAXISPOINTS
#define MAXSUBBLOCKS         MAXCUBEVERTICES/8

void  CreateBlocks(void);
void  CreateSubCubes(void);
void  InitCubeRotations(void);

typedef struct
{
  float RotationAxis[3];
  float Center[3];
  float theta;
  float thetaDelta;
} Cube;

extern int BlockWidth;
extern int CubeDivisions[];
extern int CurrentCubeAxis;
extern int NumCubesPerAxis;
extern int NumCubePolys;
extern int NumSubCubes;


