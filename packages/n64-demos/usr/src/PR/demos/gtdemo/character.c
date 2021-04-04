

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

/*---------------------------------------------------------------------*
        Copyright (C) 1997 Nintendo. (Originated by SGI)
        
        $RCSfile: character.c,v $
        $Revision: 1.12 $
        $Date: 1998/09/25 21:49:14 $
 *---------------------------------------------------------------------*/

/*
 * File:	character.c
 * Creator:	hsa@sgi.com
 * Create Date:	Wed Nov  1 10:39:29 PST 1995
 *
 */

#include <ultra64.h>
#include "gtdemo.h"
#include "controller.h"
#include "character.h"

#include "char_data.h"

/* we copy this into new objects to initialize them */
static gtState	dummyObj =
{
    GT_CULL_BACK,	/* renderState */
    0x0,	/* textureState */
    0,		/* vtxCount */
    0,		/* vtxV0 */
    0,		/* triCount */
    0x0, 	/* pad1 */
    NULL,	/* rdp commands */
    gsDPClearOtherMode(),				/* rdpOthermode */
    {	/* integer portion: */
	0x00010000, 0x00000000,			/* transform */
	0x00000001, 0x00000000,
	0x00000000, 0x00010000,
	0x00000000, 0x00000001,
	/* fractional portion: */
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
	0x00000000, 0x00000000,
    },
};

/*
 * Fill in a new character.
 * Doesn't assume all zeros at allocation-time
 */
static void
new_character(Node_t *obj, int id, char *name, int vcount, Vtx *vtx, int tricount, gtTriN *tris, float tx, float ty, float tz)
{
    gtState	*sp;
    Mtx		*mp;

    sp = &(obj->obj_state);
    obj->id = id;
    obj->flags = 0x0;
    bcopy((void *)&dummyObj, (void *)sp, sizeof(gtState));
    bcopy(name, obj->name, 24);
    obj->parent = (struct Node_t *) NULL;
    obj->children = (struct Node_t *) NULL;
    obj->sibling = (struct Node_t *) NULL;
    obj->xscl = 1.0;
    obj->yscl = 1.0;
    obj->zscl = 1.0;
    obj->xrot = 0.0;
    obj->yrot = 0.0;
    obj->zrot = 0.0;
    obj->xtrns = 0.0;
    obj->ytrns = 0.0;
    obj->ztrns = 0.0;
    guMtxIdentF(obj->my_transform);
    guMtxIdentF(obj->my_cumulative);
    obj->orig_centroid.x = 0.0;
    obj->orig_centroid.y = 0.0;
    obj->orig_centroid.z = 0.0;
    obj->centroid.x = 0.0;
    obj->centroid.y = 0.0;
    obj->centroid.z = 0.0;
    sp->sp.vtxCount = vcount;
    obj->vtx = vtx;
    sp->sp.triCount = tricount;
    obj->tri = tris;

    /* force translate elements into my_locate matix. */
    guTranslateF(obj->my_locate, tx, ty, tz);
}

/* find the centroid */
static void
find_centroid(Node_t *node)
{
    int		i, xcen = 0, ycen = 0, zcen = 0;

    for (i=0; i<node->obj_state.sp.vtxCount; i++) {
	xcen += node->vtx[i].v.ob[0];
	ycen += node->vtx[i].v.ob[1];
	zcen += node->vtx[i].v.ob[2];
    }

    if (node->obj_state.sp.vtxCount != 0) {
	xcen /= node->obj_state.sp.vtxCount;
	ycen /= node->obj_state.sp.vtxCount;
	zcen /= node->obj_state.sp.vtxCount;
    }

    node->orig_centroid.x = (float) xcen;
    node->orig_centroid.y = (float) ycen;
    node->orig_centroid.z = (float) zcen;
}

/* create a character, with initial translation into the modeling
 * matrix.
 *
 * Builds the character hierarchy for this character.
 *
 */
Node_t *
create_character(Dynamic *dynamicp, int tx, int ty, int tz)
{
    Node_t	*character, *obj, *node_pool;
    int		i;

    node_pool = &(dynamicp->obj_nodes[dynamicp->free_node]);
    character = &(node_pool[BODY_NODE]);

    /* these will propagate to all objects: */
    gtStateSetOthermode(&(dummyObj.sp.rdpOthermode), GT_RENDERMODE,
			(G_RM_OPA_SURF | G_RM_OPA_SURF2));
    gtStateSetOthermode(&(dummyObj.sp.rdpOthermode), GT_CYCLETYPE, 
			G_CYC_1CYCLE);

    /* create body */
    obj = &(node_pool[BODY_NODE]);
    new_character(obj, BODY_NODE, "body", 
		  8, body_vtx, 12, cube_tris, tx, ty, tz);
    obj->children = (struct Node_t *) &(node_pool[HEAD_NODE]);
    find_centroid(obj);

    /* create head */
    obj = &(node_pool[HEAD_NODE]);
    new_character(obj, HEAD_NODE, "head", 
		  8, head_vtx, 12, cube_tris, 0, 22, 0);
    obj->parent = (struct Node_t *) &(node_pool[BODY_NODE]);
    obj->sibling = (struct Node_t *) &(node_pool[LEFT_ARM_NODE]);
    guRotateF(obj->my_transform, 5.0, 0.0, 1.0, 0.0);
    find_centroid(obj);

    /* create left arm */
    obj = &(node_pool[LEFT_ARM_NODE]);
    new_character(obj, LEFT_ARM_NODE, "left arm", 
		  8, left_arm_vtx, 12, cube_tris, 20, 20, 0);
    obj->parent = (struct Node_t *) &(node_pool[BODY_NODE]);
    obj->sibling = (struct Node_t *) &(node_pool[RIGHT_ARM_NODE]);
    obj->children = (struct Node_t *) &(node_pool[LEFT_WRIST_NODE]);
    guRotateF(obj->my_transform, 20.0, 0.0, 0.0, 1.0);
    find_centroid(obj);

    /* create right arm */
    obj = &(node_pool[RIGHT_ARM_NODE]);
    new_character(obj, RIGHT_ARM_NODE, "right arm", 
		  8, right_arm_vtx, 12, cube_tris, -20, 20, 0);
    obj->parent = (struct Node_t *) &(node_pool[BODY_NODE]);
    obj->sibling = (struct Node_t *) &(node_pool[LEFT_LEG_NODE]);
    obj->children = (struct Node_t *) &(node_pool[RIGHT_WRIST_NODE]);
    guRotateF(obj->my_transform, -45.0, 1.0, 0.0, 0.0);
    find_centroid(obj);

    /* create left leg */
    obj = &(node_pool[LEFT_LEG_NODE]);
    new_character(obj, LEFT_LEG_NODE, "left leg", 
		  8, left_leg_vtx, 12, cube_tris, 0, -20, 0);
    obj->parent = (struct Node_t *) &(node_pool[BODY_NODE]);
    obj->sibling = (struct Node_t *) &(node_pool[RIGHT_LEG_NODE]);
    obj->children = (struct Node_t *) &(node_pool[LEFT_SHIN_NODE]);
    guRotateF(obj->my_transform, -30.0, 1.0, 0.0, 0.0);
    find_centroid(obj);

    /* create right leg */
    obj = &(node_pool[RIGHT_LEG_NODE]);
    new_character(obj, RIGHT_LEG_NODE, "right leg", 
		  8, right_leg_vtx, 12, cube_tris, -0, -20, 0);
    obj->parent = (struct Node_t *) &(node_pool[BODY_NODE]);
    obj->children = (struct Node_t *) &(node_pool[RIGHT_SHIN_NODE]);
    find_centroid(obj);

    /* create left wrist */
    obj = &(node_pool[LEFT_WRIST_NODE]);
    new_character(obj, LEFT_WRIST_NODE, "left wrist", 
		  8, left_wrist_vtx, 12, cube_tris, 0, -20, 0);
    obj->parent = (struct Node_t *) &(node_pool[LEFT_ARM_NODE]);
    guRotateF(obj->my_transform, -120.0, 1.0, 0.0, 0.0);
    find_centroid(obj);

    /* create right wrist */
    obj = &(node_pool[RIGHT_WRIST_NODE]);
    new_character(obj, RIGHT_WRIST_NODE, "right wrist", 
		  8, right_wrist_vtx, 12, cube_tris, 0, -20, 0);
    obj->parent = (struct Node_t *) &(node_pool[RIGHT_ARM_NODE]);
    guRotateF(obj->my_transform, -90.0, 1.0, 0.0, 0.0);
    find_centroid(obj);

    /* create left shin */
    obj = &(node_pool[LEFT_SHIN_NODE]);
    new_character(obj, LEFT_SHIN_NODE, "left shin", 
		  8, left_leg_vtx, 12, cube_tris, 0, -20, 0);
    obj->parent = (struct Node_t *) &(node_pool[LEFT_LEG_NODE]);
    guRotateF(obj->my_transform, 60.0, 1.0, 0.0, 0.0);
    find_centroid(obj);

    /* create right shin */
    obj = &(node_pool[RIGHT_SHIN_NODE]);
    new_character(obj, RIGHT_SHIN_NODE, "right shin", 
		  8, right_leg_vtx, 12, cube_tris, 0, -20, 0);
    obj->parent = (struct Node_t *) &(node_pool[RIGHT_LEG_NODE]);
    find_centroid(obj);

    dynamicp->free_node += BODY_PART_COUNT;

    /* initialize sorted character nodes */
    for (i=0; i<dynamicp->free_node; i++) {
	dynamicp->char_order[i] = &(dynamicp->obj_nodes[i]);
    }

    return(character);
}

/* 
 * update animation paramters for this frame 
 */
void
character_update(Node_t *obj, update_t type, float m[4][4], 
		 float x, float y, float z)
{
    float	m1[4][4];

    switch (type) {

      case OBJ_MTX_LOAD:
	bcopy((void *)m, (void *)obj->my_transform, sizeof(float)*16);
	break;

      case OBJ_MTX_MUL:
	guMtxCatF(obj->my_transform, m, obj->my_transform);
	break;

      case OBJ_SCALE:
	guScaleF(m1, x, y, z);
	guMtxCatF(obj->my_transform, m1, obj->my_transform);
	break;

      case OBJ_ROTATE:
	break;

      case OBJ_TRANSLATE:
	guTranslateF(m1, x, y, z);
	guMtxCatF(obj->my_transform, m1, obj->my_transform);
	break;

    }
}

/* 
 * Flatten matrices, computing current cumulative for each.
 * Processes the tree recursively in hierarchical order.
 */
void
character_flatten(Dynamic *dynamicp, Node_t *obj)
{
    Node_t	*parent;
    gtState	*pstate, *ostate;
    float	m1[4][4], m2[4][4];

    parent = (Node_t *) obj->parent;
    ostate = &(obj->obj_state);
    pstate = &(parent->obj_state);

    /* fix the rendermode, based on z-buffering or not */
    if (do_turbo_z) {
        gtStateSetOthermode(&(ostate->sp.rdpOthermode), GT_RENDERMODE,
			    (G_RM_ZB_OPA_SURF | G_RM_ZB_OPA_SURF2));
	ostate->sp.renderState |= GT_ZBUFFER;
    } else {
        gtStateSetOthermode(&(ostate->sp.rdpOthermode), GT_RENDERMODE,
			    (G_RM_OPA_SURF | G_RM_OPA_SURF2));
	ostate->sp.renderState &= ~GT_ZBUFFER;
    }

    guMtxCatF(obj->my_transform, obj->my_locate, m1);
    if (parent == (Node_t *) NULL) {
	guMtxCatF(dynamicp->viewing, dynamicp->projection, m2);
	guMtxCatF(m1, m2, obj->my_cumulative);
    } else {
	guMtxCatF(m1, parent->my_cumulative, obj->my_cumulative);
    }
    guMtxF2L(obj->my_cumulative, &(ostate->sp.transform));

    /* follow sibling chain recursively: */
    if (obj->sibling != (struct Node_t *) NULL)
	character_flatten(dynamicp, (Node_t *)obj->sibling);

    /* follow children recursively: */
    if (obj->children != (struct Node_t *) NULL)
	character_flatten(dynamicp, (Node_t *)obj->children);

}

/* 
 * Sort character nodes based on transformed centroid z.
 * Choice of sorting algorithm is careful, we should use
 * one which is linear, o(n) for a nearly-sorted list, since
 * the sorted order shouldn't change much, frame-to-frame.
 *
 */
void
character_sort(Dynamic *dynamicp)
{
    Node_t	*obj, *tmp;
    gtState	*ostate;
    int		i, j;

    if (do_turbo_z)
	return;	/* skip the sort for z-buffer case */

    /* Tranform centroids. This is sloppy; we only care about
     * the z, so there is no need for a full matrix multiply.
     */
    for (i=0; i<dynamicp->free_node; i++) {
	obj = dynamicp->char_order[i];
	ostate = &(obj->obj_state);
	guMtxXFML(&(ostate->sp.transform), obj->orig_centroid.x,
		  obj->orig_centroid.y, obj->orig_centroid.z,
		  &obj->centroid.x, &obj->centroid.y, &obj->centroid.z);
    }

    /* insertion sort character nodes, o(n) for nearly-sorted list */
    for (i=1; i<dynamicp->free_node; i++) {
	tmp = dynamicp->char_order[i];
	for (j=i; 
	     ((j >= 1) && 
	      (dynamicp->char_order[j-1]->centroid.z < tmp->centroid.z)); 
	     j--) {
	    /* move all of them down */
	    dynamicp->char_order[j] = dynamicp->char_order[j-1];
	}
	dynamicp->char_order[j] = tmp;
    }
}

void
character_draw(Dynamic *dynamicp, gtGfx	**gtlistp)
{
    Node_t	*obj;
    int		i;

    for (i=0; i<dynamicp->free_node; i++) {

	obj = dynamicp->char_order[i];

	(*gtlistp)->obj.gstatep = (gtGlobState *) NULL;
	(*gtlistp)->obj.statep = &(obj->obj_state);
	(*gtlistp)->obj.vtxp = obj->vtx;
	(*gtlistp)->obj.trip = obj->tri;
	(*gtlistp)++;
    }
}
