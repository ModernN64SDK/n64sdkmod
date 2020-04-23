
#define MAXPLANEAXISDIVISIONS 20
#define MAXPLANEAXISPOINTS    2*(MAXPLANEAXISDIVISIONS + 1) 

#define MAXPLANEVERTICES      MAXPLANEAXISPOINTS * MAXPLANEAXISPOINTS 
#define MAXSUBPLANES          MAXPLANEVERTICES/4

void  CreatePlanes(void);
void  CreateSubPlanes(void);

typedef struct
{
  float Center[3];
} Plane;

extern int   PlaneWidth;
extern int   PlaneDivisions[];
extern int   CurrentPlaneAxis;
extern int   NumSubPlanes;
extern int   NumPlanePolys;
extern float PlaneMinMax[3][2];

