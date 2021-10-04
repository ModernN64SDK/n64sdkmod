#include <ultra64.h>

#include "fill.h"


unsigned int xseed = (7789<<16)+13399;

unsigned int xrand(void)
{
    unsigned int x;

    x = (xseed<<2) + 2;

    x *= (x+1);
    x = x >> 2;

    xseed = x;

    return( x );
}

float frand(void)
{
  return((xrand()%32767) / 32768.0);
}


/*
 *  Create list of randomly placed triangles, with a certain aspect ratio
 *  and a certain area.  The aspect ratio is defined as the longest x edge
 *  divided by the longest y edge.  The triangle always has the general shape:
 *
 * (x0,y0)
 *  +------------+(x1,y1)
 *  |           /
 *  |         /
 *  |       /
 *  |     /
 *  |   /
 *  | /
 *  + (x2,y2)
 */
void
makeTriList(int ntri, float area , float aspect)
{
  int tri;
  float xl, yl;
  float x0, y0, z0;
  float x1, y1, z1;
  float x2, y2, z2;
  float tc;
  Gfx *tlistp = dynamicp->trilist;

  yl = sqrtf(2.0*area/aspect);
  xl = yl*aspect;

  for(tri = 0; tri < ntri; tri++) {
    do {
      x0 = SCREEN_WD * frand();
      y0 = SCREEN_HT * frand();
    } while (((x0+xl) > SCREEN_WD) || ((y0+yl) > SCREEN_HT));

    x1 = x0 + xl;
    y1 = y0;
    x2 = x0;
    y2 = y0 + yl;

	z0 = 500.0 * frand() + 10.0;
	z1 = 500.0 * frand() + 10.0;
	z2 = 500.0 * frand() + 10.0;

    V(&(dynamic.vtxlist[tri * 3]), (short)x0, (short)y0, (short)z0, 0, 0, 0, 
	0xff, 0xff, 0xff, 0xff);
    V(&(dynamic.vtxlist[tri*3+1]), (short)x1, (short)y1, (short)z1, 0, 0, 0,
	0xff, 0xff, 0xff, 0xff);
    V(&(dynamic.vtxlist[tri*3+2]), (short)x2, (short)y2, (short)z2, 0, 0, 0,
	0xff, 0xff, 0xff, 0xff);

	/* assign random index for color */
	tc = 255.0 * frand();

	gDPSetPrimColor(tlistp++, 0, 0, (u8)tc, (u8)tc, (u8)tc, (u8)tc);
    gSPVertex(tlistp++, &dynamic.vtxlist[tri*3], 3, 0);	
    gSP1Triangle(tlistp++, 0, 1, 2, 0);
  }

  gSPEndDisplayList(tlistp++);
}


