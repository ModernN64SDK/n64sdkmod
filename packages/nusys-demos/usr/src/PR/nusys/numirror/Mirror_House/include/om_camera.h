/*======================================================================*/
/*         OBJECT MANAGER                                               */
/*          om_camera.h                                                 */
/*                                                                      */
/*      Copyright (C) 1998, NINTENDO Co,Ltd.                            */
/*                                                                      */
/*======================================================================*/
#include    "nd.h"

#ifndef _OM_CAMERA_H_
#define _OM_CAMERA_H_

/* ----------------------------------------------------------------------------
    Camera Data Structure 
---------------------------------------------------------------------------- */
typedef struct {
    float   x, y, z;                    /* Eye position */
    float   xl, yl, zl;                 /* xxfocal viewpointxx */
    float   xup, yup, zup;              /* Camera xxup-directionxx vector */
    float   s;                          /* Display area (the radius of the display circumference around the viewpoint) */
    float   near, far;                   /* The distances to the near and far clipping planes from the viewpoint (always positive) */
    float   fovy;                       /* The viewing angle in the Y direction */
    float   aspect;                     /* Aspect ratio defining the field of view in the x direction.  The aspect ratio is the ratio of y to x for the field of view. */
    float   scale;                      /* Scale */
    int     fill_depth;                 /* Initial value of the Z-buffer */
    int     fill_col;                   /* Background color */
    struct {                            /* Fog */
        float   near, ffar;             /* Near clip and far clip */
        int     r,g,b,a;
    } fog;
    float   rx, ry, rz;                 /* x, y, and z camera rotation angles */
    float   high;                       /* Camera height */
    float   distance;                   /* Distance between the camera and the player-operated xxcharacterxx */
} OM_CAMERA_ST;

#endif

