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

/*
 * File:	shared.h
 * Create Date:	Thu Mar  7 14:23:20 PST 1996
 *
 */

typedef struct VertS {
    s16 altitude;
    s8  normal[3];
    u8  texture[2];
    u8  flags;
    s16 offset[2];
    s16 s,t;
} VertS;

#define WORLD_X 1024
#define WORLD_Z 1024
#define TESSEL  5
#define SIDE    (2*2*2*2*2)
#define SIDEMAX (SIDE*8)
#define SIDE2X  ((float)WORLD_X/(dimx-1))
#define SIDE2Z  ((float)WORLD_Z/(dimz-1))

#define VERT_FLAG_SWAPDIAG 1
