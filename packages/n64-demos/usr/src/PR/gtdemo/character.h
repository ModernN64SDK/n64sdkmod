
/*
 * Copyright 1995, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 *
 * UNPUBLISHED -- Rights reserved under the copyright laws of the United
 * States.   Use of a copyright notice is precautionary only and does not
 * imply publication or disclosure.
 *
 * U.S. GOVERNMENT RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to restrictions
 * as set forth in FAR 52.227.19(c)(2) or subparagraph (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS 252.227-7013 and/or
 * in similar or successor clauses in the FAR, or the DOD or NASA FAR
 * Supplement.  Contractor/manufacturer is Silicon Graphics, Inc.,
 * 2011 N. Shoreline Blvd. Mountain View, CA 94039-7311.
 *
 * THE CONTENT OF THIS WORK CONTAINS CONFIDENTIAL AND PROPRIETARY
 * INFORMATION OF SILICON GRAPHICS, INC. ANY DUPLICATION, MODIFICATION,
 * DISTRIBUTION, OR DISCLOSURE IN ANY FORM, IN WHOLE, OR IN PART, IS STRICTLY
 * PROHIBITED WITHOUT THE PRIOR EXPRESS WRITTEN PERMISSION OF SILICON
 * GRAPHICS, INC.
 *
 */

/*
 * File:	character.h
 * Creator:	hsa@sgi.com
 * Create Date:	Wed Nov  1 17:23:19 PST 1995
 *
 */


#define BODY_NODE		0
#define HEAD_NODE		1
#define LEFT_LEG_NODE		2
#define RIGHT_LEG_NODE		3
#define LEFT_SHIN_NODE		4
#define RIGHT_SHIN_NODE		5
#define LEFT_ARM_NODE		6
#define RIGHT_ARM_NODE		7
#define LEFT_WRIST_NODE		8
#define RIGHT_WRIST_NODE	9
#define BODY_PART_COUNT		10

typedef enum {
    OBJ_MTX_LOAD,
    OBJ_MTX_MUL,
    OBJ_SCALE,
    OBJ_ROTATE,
    OBJ_TRANSLATE
} update_t;

extern Node_t	*create_character(Dynamic *dynamicp, int tx, int ty, int tz);
extern void	character_update(Node_t *obj, update_t type, 
				 float m[4][4], float x, float y, float z);
extern void	character_flatten(Dynamic *dynamicp, Node_t *character);
extern void	character_sort(Dynamic *dynamicp);
extern void	character_draw(Dynamic *dynamicp, gtGfx	**gtlistp);




