/*======================================================================*/
/*	Mirror House							*/
/*	om_camera.c							*/
/*									*/
/*	Copyright (C) 1998 MONEGI CORPORATION				*/
/*									*/
/*======================================================================*/
#include "pack.h"
#include <string.h>

#include "nd.h"
#include "nusys.h"

#include "om.h"
#include "om_camera.h"
#include "om_common.h"


/* ========================================================================== 
	define
 ========================================================================== */
#define OM_CAMERA_ZMOVE   0
#define OM_CAMERA_ZMOVE1  1

/* ===========================================================================
	Global Variable Declarations
 ========================================================================== */
static int om_camera_mode = 0;

/* ===========================================================================
	Global Structure Declarations 
 ========================================================================== */

/* ========================================================================== 
	External Function Reference Declarations 
 ========================================================================== */

/* ========================================================================== 
	External Reference Declarations 
 ========================================================================== */
extern EXCONTPAD *pad;

/* ===========================================================================
	Function Prototype Declarations 
 =========================================================================== */
void om_camera_init(OM_CAMERA_ST *cst);
OM_CAMERA_ST *om_camera_entry(void);
void om_camera_entry_delete(OM_CAMERA_ST *cst);
Gfx *om_camera_perspective_draw(Gfx *gp, OM_CAMERA_ST *cst, float um[4][4]);
void om_camera_game_camera_action(OM_CAMERA_ST *cst, OM_CHARACTER_ST *st);
int om_camera_mode_check(OM_CAMERA_ST *cst, OM_BG_STAGE_ST *stage);
Gfx *om_camera_ortho_draw(Gfx *gp);
void om_camera_mode_on_game_view_mode(void);

/* ===========================================================================
	Data Definitions 
 =========================================================================== */
/*** Initial data of the camera data structure */
static OM_CAMERA_ST om_camera_info = {
    0, 500, 1100,                         /* Eye position */
    0.0F, 0.0F, 0.0F,                     /* Focal point */
    0.0F, 1.0F, 0.0F,                     /* Up-direction vector */
    3500,                                 /* Display area  */
    1000.0F, 15000.0F,                    /* near,far       */
    30.0F,                                /* fovy           */
    ((float)SCREEN_WD/(float)SCREEN_HT),  /* aspect         */
    1.0F,                                 /* scale          */
    GPACK_ZDZ(G_MAXFBZ,0),                /* fill_depth     */
    0x8420,                               /* fill_col       */
    {0,0,	0,0,0},                   /* fog            */
    0.0F, 0.0F, 0.0F,                     /* Camera rotation angle  */
    2500.0F,                              /* Camera height   */
    2500.0F                               /* Distance between camera and main character */
};


/* ===========================================================================
	Function Definitions 
 =========================================================================== */
void om_camera_init(OM_CAMERA_ST *cst)
{
    /* Initialize the camera data structure */
    /* The initial data is copied to the global structure om_camera_st, which is declared in om.h.  */
    memcpy((char*)cst, (char*)&om_camera_info, sizeof(OM_CAMERA_ST));
    om_camera_mode = 0;
}

OM_CAMERA_ST *om_camera_entry(void)
{
    OM_CAMERA_ST *cst;

    cst = (OM_CAMERA_ST*)malloc(sizeof(OM_CAMERA_ST));

    if (cst != (OM_CAMERA_ST*)NULL) {
        /* Initialize the camera data structure */
        memcpy((char*)cst, (char*)&om_camera_info, sizeof(OM_CAMERA_ST));
        return cst;
    }
    osSyncPrintf("OM_CAMERA_ST Memory Alloc Error\n");
    return (OM_CAMERA_ST*)NULL;
}

void om_camera_entry_delete(OM_CAMERA_ST *cst)
{
    /* Free memory reserved for the camera */
    free(cst);
}

Gfx *om_camera_perspective_draw(Gfx *gp, OM_CAMERA_ST *cst, float um[4][4])
{
    /* Calculate the perspective projection viewing matrix */
    guLookAtF(um, cst->x, cst->y, cst->z, cst->xl, cst->yl, cst->zl,
                  cst->xup, cst->yup, cst->zup);

    cur_scene.camera.fovy   = cst->fovy;
    cur_scene.camera.aspect = cst->aspect;
    cur_scene.camera.near   = cst->near;
    cur_scene.camera.far    = cst->far;
    cur_scene.camera.scale  = cst->scale;
    /* Set the light color for the scene */
    cur_scene.lightset.light[0].color.r = 255;
    cur_scene.lightset.light[0].color.g = 255;
    cur_scene.lightset.light[0].color.b = 255;
    /* Set the background color */
    cur_scene.fill_col = GPACK_RGBA5551(0, 100, 70, 255);
    /* Build the current scene based on the perspective projection matrix */
    gp = ndBuildSceneGfx(gp, um);
    return gp;
}

void om_camera_game_camera_action(OM_CAMERA_ST *cst, OM_CHARACTER_ST *st)
{
    OM_CHARACTER_DATA *data;

    data = st->data; /* Obtain the pointer to the character data for the player- operated character. */

    /* Calculate the coordinates of the camera so that it follows Flake-kun. */
    cst->x = cst->distance*sinf(cst->ry)+data->x;
    cst->y = cst->high+data->y;
    cst->z = cst->distance*cosf(cst->ry)+data->z;
    cst->xl = data->x;
    cst->yl = data->y;
    cst->zl = data->z;
}

int om_camera_mode_check(OM_CAMERA_ST *cst, OM_BG_STAGE_ST *stage)
{
    if (stage->help_num < 0) {
        return 0;
    }

    if (pad[0].trigger & CONT_R) {
        if (om_camera_mode == 0)
            om_camera_mode = 1;
        else
            om_camera_mode = 0;
    }

    if (om_camera_mode == 0) {
        if (cst->high > 2000.0F) {
            cst->high -= 50.0F;
        } else {
            cst->high = 2000.0F;
        }
    } else {
        if (cst->high < 8000.0F) {
            cst->high += 50.0F;
        } else {
            cst->high = 8000.0F;
        }
        ++om_camera_mode;
    }
    return om_camera_mode;
}

Gfx *om_camera_ortho_draw(Gfx *gp)
{
    /* Build scene using parallel projection */
    cur_scene.fill_col = GPACK_RGBA5551( 0 , 0 , 0 , 1 );	/* Black background */
    /* Frame buffer area */
    gp = ndClearFB(gp, cur_scene.fill_depth, cur_scene.fill_col);
    /* Calculate the parallel projection matrix  */
    gp = gfxOrtho( gp, -(float)SCREEN_WD/2.0F, (float)SCREEN_WD/2.0F,
                   -(float)SCREEN_HT/2.0F, (float)SCREEN_HT/2.0F,
                   1.0F, 10.0F, 1.0F );
    return gp;
}

void om_camera_mode_on_game_view_mode(void)
{
    om_camera_mode = 0;
}


