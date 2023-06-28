/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 2

		Copyright (C) 1997, NINTENDO Co,Ltd.

============================================================================*/


#include <ultra64.h>
#include <PR/ramrom.h>
#include <assert.h>

#define G_CC_MULPRIMSHADE PRIMITIVE,0,SHADE,0,PRIMITIVE,0,SHADE,0

#include "inc/near_.c"
#include "inc/middle_.c"
#include "inc/far_.c"


static Vtx testvtx = {
  { 0,    0,    0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
};

Gfx lodmodel_dl[] = {
  gsSPVertex(&testvtx, 1, 0),
  gsSPBranchLessZ(near__dl,   0,  500, 10, 2000, G_BZ_PERSP),
  gsSPBranchLessZ(middle__dl, 0, 1000, 10, 2000, G_BZ_PERSP),
  gsSPBranchLessZ(far__dl,    0, 1600, 10, 2000, G_BZ_PERSP),
  gsSPEndDisplayList(),
};

