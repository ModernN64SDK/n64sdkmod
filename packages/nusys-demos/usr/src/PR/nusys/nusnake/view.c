/*============================================================================
  NuSYSTEM sample program [SNAKE TAIL HACK]
  
  view.c
  
  Copyright (C) 1997, NINTENDO Co,Ltd.
  ============================================================================*/

#include <assert.h>
#include <nusys.h>

#include "main.h"
#include "graphic.h"
#include "para.h"

void SetViewMtx( Dynamic* );
void SetViewMtxGameOver( Dynamic* );
extern float View;

/*--------------------------------------*/
/*  Setting the view-matrix (at the normal time)*/
/*  IN:	dp The dynamic pointer 		*/
/*  RET:None				*/
/*--------------------------------------*/
void
SetViewMtx( Dynamic* dp )
{
    u16   perspNorm;
    static LookAt lookat;
    /* The calculation and set-up of the projection-matrix  */
    guPerspective(&dp->projection, &perspNorm,
		  33, (float)SCREEN_WD/(float)SCREEN_HT, 10, 10000, 1.0);

    guLookAtReflect(&dp->viewing, &lookat,
		    0,
		    cosf(2.0*M_PI/(360/(View+90.0)))*CAM_DISTANCE,
		    sinf(2.0*M_PI/(360/(View+90.0)))*CAM_DISTANCE,
		    0, 0, 0,
		    0, 1, 0);

    gSPPerspNormalize(glistp++, perspNorm);
    gSPLookAt(glistp++, &lookat);

    gSPMatrix(glistp++, &(dp->projection), 
	      G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(glistp++, &(dp->viewing), 
	      G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
}

/*----------------------------------------------*/
/*  Setting of the view-matrix (at the game-over)	*/
/*  IN:	dp The dynamic pointer 			*/
/*  RET:None					*/
/*----------------------------------------------*/
void
SetViewMtxGameOver( Dynamic* dp )
{
    u16   perspNorm;
    static LookAt lookat;
    /* The calculation and set-up of the projection-matrix  */
    guPerspective(&dp->projection, &perspNorm,
		  33, (float)SCREEN_WD/(float)SCREEN_HT, 10, 10000, 1.0);

    guLookAtReflect(&dp->viewing, &lookat,
		    0,
		    0,
		    1500,
		    0, 0, 0,
		    0, 1, 0);

    gSPPerspNormalize(glistp++, perspNorm);
    gSPLookAt(glistp++, &lookat);

    gSPMatrix(glistp++, &(dp->projection), 
	      G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(glistp++, &(dp->viewing), 
	      G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
}
