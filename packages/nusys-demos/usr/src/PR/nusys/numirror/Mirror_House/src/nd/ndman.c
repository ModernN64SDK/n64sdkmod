/*======================================================================*/
/*		NIFFprev						*/
/*		ndman.c							*/
/*									*/
/*		Copyright (C) 1997,1998, NINTENDO Co,Ltd.		*/
/*									*/
/*======================================================================*/
#define __NDMAN_C__

/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	Simple 3D Manager ND 

	Manager
 :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#include <string.h>
#include "pack.h"
#include "malloc.h"
#include "nd.h"

/* ===========================================================================
	defines
 =========================================================================== */

/* ===========================================================================
	variables
 =========================================================================== */

int ndinst_cnt;

NDD_SCENE cur_scene;		/* Current scene */
NDD_INSTANCE top_inst;		/* Top instance */
NDD_INSTANCE bottom_inst;	/* Terminator instance */
NDD_INSTANCE *root_instp;	/* Root instance pointer */
NDD_INSTANCE *obj_root_instp;	/* Root of object entries */
NDD_INSTANCE *anim_root_instp;	/* Root of animation entries */
NDD_INSTANCE *deform_root_instp; /* Root of deform entries */
NDD_INSTANCE *dynvtx_root_instp; /* Root of dynamic vertex entries */

u32 deform_flg;				/* Deform flag */

/* ===========================================================================
	functions
 =========================================================================== */

/* ----------------------------------------------------------------------------
	isp called to parent as the youngest child
 --------------------------------------------------------------------------- */
static void addChild(NDD_INSTANCE *parent, NDD_INSTANCE *isp)
{
    NDD_INSTANCE **pp;

    isp->parent = parent;
    for(pp=&parent->child; *pp; pp=&((*pp)->bros))
      ;

    *pp = isp;
    ++parent->nchild;
}

/* ----------------------------------------------------------------------------
	Sever link from parent to isp 
 --------------------------------------------------------------------------- */
static void delChild(NDD_INSTANCE *isp)
{

    if(isp->parent->child == isp){
	isp->parent->child = isp->bros;
    }else{
	NDD_INSTANCE **pp;

	for(pp=&isp->parent->child; *pp!=isp; pp=&((*pp)->bros))
	  ;
	*pp = isp->bros;
    }

    --isp->parent->nchild;
}

/* ----------------------------------------------------------------------------
name:
    ndGenInstance
synopsis:
    NDD_INSTANCE *ndGenInstance(NDD_INSTANCE *parent, u16 type, u16 prio, int gid);
description:
    Creates the instance structure and links to it.
parameters:
    NDD_INSTANCE *parent;
	Pointer to the parent instance.
    u16 type;
	Instance type.
    u16 prio;
	Priority code. Affects the evaluation order of display instances.
	The range that can be specified is determined by the specified type .
    int gid;
	Group ID for the instance.
returns:
    NDD_INSTANCE *;
	Pointer to the generated instances
notes:
    Creates instances, the characteristics of which are determined by type.
 --------------------------------------------------------------------------- */
NDD_INSTANCE *ndGenInstance(NDD_INSTANCE *parent, u16 type, u16 prio, int gid)
{
    NDD_INSTANCE *bp, *isp;

    /* Added beneath the root if NULL is specified as the parent */
    if(!parent)
      parent = root_instp;

    /* Search for insertion position */
    for(bp=top_inst.next; bp->prio>prio; bp=bp->next)
      ;

    isp = malloc( sizeof(NDD_INSTANCE) );
    isp->flag = 0;
    /* Reestablish link */
    isp->next = bp;
    isp->back = bp->back;
    bp->back->next = isp;
    bp->back = isp;

    /* Still childless */
    isp->bros = isp->child = NULL;
    isp->nchild = 0;

    /* Added beneath parent */
    addChild(parent, isp);

    /* No callback */
    isp->instCB = NULL;

    /* Copy the registers */
    isp->type = type;
    isp->prio = prio;
    isp->gid = gid;

    /* Sum the total number of instances */
    ++ndinst_cnt;

    return isp;
}

/* ----------------------------------------------------------------------------
name:
    ndDeleteInstance
synopsis:
    void ndDeleteInstance(NDD_INSTANCE *isp);
description:
    Deletes instances. Child instances are also deleted.
parameters:
returns:
notes:
    Note:  Cannot be used in an instance callback.
---------------------------------------------------------------------------- */
void ndDeleteInstance(NDD_INSTANCE *isp)
{
    NDD_INSTANCE *bp;

    /* Zombie dead -- do nothing */
    if(BITTEST1( isp->flag, NDC_IFLAG_ZONBIE ))
      return;

    /* Become a zombie */
    BITSET( isp->flag, NDC_IFLAG_ZONBIE );

    /* Communicate deletion to callback */
    if(isp->instCB)
      (*isp->instCB)(isp, NDC_ICB_DIE);

    /* Delete child */
    if(isp->child){
	for(bp=isp->child; bp; bp=bp->bros)
	  ndDeleteInstance(bp);
    }

    /* Separate from parent */
    delChild(isp);

    /* Remove from vertical link */
    bp = isp->next;
    bp->back = isp->back;
    bp->back->next = bp;

    /* Free memory */
    free(isp);
}

/* ----------------------------------------------------------------------------
name:
    ndChangePriority
synopsis:
    u16 ndChangePriority(NDD_INSTANCE *isp, u16 newprio);
description:
    Changes the priority of a single instance.
parameters:
    NDD_INSTANCE *isp;
	Instance pointer
    u16 newprio;
	New priority 
returns:
    u16;
	Old priority
notes:
    Links to the xxuppermost portionxx of the specified priority
 --------------------------------------------------------------------------- */
u16 ndChangePriority(NDD_INSTANCE *isp, u16 newprio)
{
    u16 oldprio;
    NDD_INSTANCE *link;

    /* Remove from the location of the current link */
    isp->next->back = isp->back;
    isp->back->next = isp->next;

    if(isp->prio <= newprio){
	/* Search beginning from the instance's own location in order of ascending priority */
	for(link = isp->back; link!=&top_inst; link=link->back)
	  if( link->prio > newprio )
	    break;
	link = link->next;
    }else{
	/* Search beginning from the instance's own location in order of descending priority */
	for(link = isp->next; link!=&bottom_inst; link=link->next)
	  if( link->prio <= newprio )
	    break;
    }

    /* Reestablish the link */
    isp->back = link->back;
    isp->next = link;
    link->back->next = isp;
    link->back = isp;

    /* Set priority */
    oldprio = isp->prio;
    isp->prio = newprio;

    return oldprio;
}

/* ----------------------------------------------------------------------------
name:
    ndChangeParent
synopsis:
    NDD_INSTANCE ndChangeParent(NDD_INSTANCE *isp, NDD_INSTANCE *newparent);
description:
    Changes the parent of an instance
parameters:
    NDD_INSTANCE *isp;
	Pointer to the target instance
    NDD_INSTANCE *newparent;
	Pointer to the new parent instance
returns:
    NDD_INSTANCE *;
	Previous parent instance
notes:
    The value of newparent cannot be NULL.
 --------------------------------------------------------------------------- */
NDD_INSTANCE *ndChangeParent(NDD_INSTANCE *isp, NDD_INSTANCE *newparent)
{
    NDD_INSTANCE *oldparent;

    /* Save the old parent */
    oldparent = isp->parent;

    /* Detach from old parent */
    delChild(isp);

    /* Join to new parent */
    addChild(newparent, isp);

    return oldparent;
}

/* ----------------------------------------------------------------------------
name:
    ndDeleteInstanceGID
synopsis:
    void ndDeleteInstanceGID(NDD_INSTANCE *isp, int type, int gid, int mask);
description:
    Delete instances that match the specified type, gid, and mask parameters.
    Deleted are those for which <TYPE> == type and <GID> & mask == gid is true.  Specifying -1 for type causes type not to be checked.
    All isp child instances are evaluated.  When a parent matches the search parameters, all child instances below are unconditionally deleted.
parameters:
    NDD_INSTANCE *isp;
    int type;
    int gid;
    int mask;
returns:
notes:
    Note: Cannot be used in an instance callback.
---------------------------------------------------------------------------- */
void ndDeleteInstanceGID(NDD_INSTANCE *isp, int type, int gid, int mask)
{
    if((type<0 || isp->type == type) && (isp->gid & mask) == gid){
	/* Delete */
	ndDeleteInstance(isp);
    }else{
	/* Evaluate child for deletion */
	NDD_INSTANCE *bp;

	if(isp->child){
	    for(bp=isp->child; bp; bp=bp->bros)
	      ndDeleteInstanceGID(bp, type, gid, mask);
	}
    }
}

/* ----------------------------------------------------------------------------
name:
    ndInit
synopsis:
    void ndInit(void);
description:
    Initializes the module.
parameters:
returns:
notes:
    This function must always be called before using the API for nd as a whole.
    It prepares the current scene.  Even if the scene is not specified, the stage for displaying the object is prepared. 
---------------------------------------------------------------------------- */
void ndInit(void)
{
    NDD_INSTANCE *isp;
    int i;

    /* Initialize the instance table */
    top_inst.next = &bottom_inst;
    top_inst.back = NULL;
    top_inst.type = NDC_ITYPE_NULL;
    top_inst.prio = NDC_IPRIO_SYSTEM_TOP;
    bottom_inst.next = NULL;
    bottom_inst.back = &top_inst;
    bottom_inst.type = NDC_ITYPE_NULL;
    bottom_inst.prio = NDC_IPRIO_SYSTEM_BOTTOM;

    ndinst_cnt = 0;

    /* Initialize the current scene */
    cur_scene.top = 0;
    cur_scene.bottom = SCREEN_HT;
    cur_scene.left = 0;
    cur_scene.right = SCREEN_WD;
    cur_scene.fog.near=
      cur_scene.fog.far=0.0;	/* if near == far, fog off  */
    cur_scene.fog.color.r=
      cur_scene.fog.color.g=
	cur_scene.fog.color.b=
	  cur_scene.fog.color.a=0;
    cur_scene.camera.mode = 0;
    cur_scene.camera.fovy = 45.0;
    cur_scene.camera.aspect = 4.0/3.0;
    cur_scene.camera.near = 50;
    cur_scene.camera.far = 20000;
    cur_scene.camera.scale = 1.0;
    cur_scene.camera.top = -SCREEN_HT/2;
    cur_scene.camera.bottom = SCREEN_HT/2;
    cur_scene.camera.left = -SCREEN_WD/2;
    cur_scene.camera.right = SCREEN_WD/2;
    cur_scene.fill_col = GPACK_RGBA5551(0, 80, 100, 255);
    cur_scene.fill_depth = GPACK_ZDZ(G_MAXFBZ, 0);
    cur_scene.cam[0]=cur_scene.cam[1]=cur_scene.cam[2]=NULL;
    cur_scene.enacam = 0;
    cur_scene.reflect.right[0] = 1;
    cur_scene.reflect.right[1] = 0;
    cur_scene.reflect.right[2] = 0;
    cur_scene.reflect.upper[0] = 0;
    cur_scene.reflect.upper[1] = 1;
    cur_scene.reflect.upper[2] = 0;
    cur_scene.lightset.numlights = 1;
    cur_scene.lightset.ambient.r = 127;
    cur_scene.lightset.ambient.g = 127;
    cur_scene.lightset.ambient.b = 127;
    cur_scene.lightset.light[0].flag = NDC_LFLAG_ENABLE;
    cur_scene.lightset.light[0].color.r = 255;
    cur_scene.lightset.light[0].color.g = 255;
    cur_scene.lightset.light[0].color.b = 255;
    cur_scene.lightset.light[0].fnv[0] = 0.0;
    cur_scene.lightset.light[0].fnv[1] = 1.0;
    cur_scene.lightset.light[0].fnv[2] = 0.0;
    for(i=1; i<NUM_LIGHTSET_LIGHTS; ++i)
      cur_scene.lightset.light[i].flag = 0;

    /* Creation of the root object instance */
    root_instp = isp = malloc( sizeof(NDD_INSTANCE) );
    /* Reestablish the link */
    isp->next = &bottom_inst;
    isp->back = &top_inst;
    top_inst.next = isp;
    bottom_inst.back = isp;
    isp->bros = isp->child = NULL;
    isp->nchild= 0;
    isp->instCB = NULL;
    /* Copy the registers */
    isp->type = NDC_ITYPE_NULL;
    isp->prio = NDC_IPRIO_ROOT;
    /* Include the root in the count */
    ++ndinst_cnt;

    /* The order in which the following root instances are generated is important. */

    /* Root of Animation entries  */
    anim_root_instp =
      ndGenInstance( root_instp, NDC_ITYPE_NULL, NDC_IPRIO_ANIMROOT, 0 );
    deform_root_instp =
      ndGenInstance( anim_root_instp, NDC_ITYPE_NULL, NDC_IPRIO_DEFORM, 0 );
    dynvtx_root_instp =
      ndGenInstance( anim_root_instp, NDC_ITYPE_NULL, NDC_IPRIO_DYNVTX, 0 );

    /* Creates the root of object entries */
    obj_root_instp =
      ndGenInstance( root_instp, NDC_ITYPE_ROOT, NDC_IPRIO_OBJROOT, 0 );
}

/* ---------------------------------------------------------------------------
name:
    ndEntryObjectRaw
synopsis:
    NDD_INSTANCE *ndEntryObjectRaw(NDD_INSTANCE *parent, NDR_OBJ *rop, int gid);
description:
    Creates a single instance from the object reference table.
    The function does not follow links or generate animation instances.
parameters:
    NDD_INSTANCE *parent;
	Pointer to the parent instance.  Added beneath the object's root if the value is NULL.
    NDR_OBJ *rop;
	Pointer to the object reference table.
    int gid;
	Base value of the instance ID.  xxThe item specified by NDR_OBJ,  which provides gid, is entered in the created object instance structure as the value of this parameter.xx
returns:
    NDD_INSTANCE *;
	Pointer to the created instance
notes:
    Cannot be used in an instance callback.
 --------------------------------------------------------------------------- */
NDD_INSTANCE *ndEntryObjectRaw(NDD_INSTANCE *parent, NDR_OBJ *rop, int gid)
{
    NDD_INSTANCE *isp;

    /* Create the structure */
    isp = ndGenInstance( parent, NDC_ITYPE_OBJECT,
			rop->prio | NDC_IPRIO_OBJ, gid+rop->gid );

    /* Copy the registers */
    isp->u.obj.order = rop->order;
    isp->u.obj.flag = NDC_OFLAG_ENABLE|NDC_OFLAG_ALLREG|rop->regflag;
    memcpy( &isp->u.obj.trx, &rop->trx, sizeof(TRX));

    isp->u.obj.geommode = rop->geommode;
    isp->u.obj.rdmode = rop->rdmode;
    memcpy( isp->u.obj.cc1, rop->cc1, sizeof(char)*8);
    memcpy( isp->u.obj.cc2, rop->cc2, sizeof(char)*8);
    isp->u.obj.primcol = rop->primcol;

#if 0
    isp->u.obj.bb_obj[0] = rop->bb_obj[0];
    isp->u.obj.bb_obj[1] = rop->bb_obj[1];
#else
    isp->u.obj.bb_obj[0] = NULL;
    isp->u.obj.bb_obj[1] = NULL;
#endif

    return isp;
}

/* ----------------------------------------------------------------------------
name:
    ndEntryAnimationRaw
synopsis:
    NDD_INSTANCE *ndEntryAnimationRaw(NDD_INSTANCE *parent, NDR_ANIM *anp, int gid, NDD_INSTANCE *tarinst);
description:
 Uses the animation reference table to create a timekeeper instance and animation instances corresponding to the number of channels.
parameters:
    NDD_INSTANCE *parent;
	Pointer to the parent instance
    NDR_ANIM *anp;
	Pointer to the animation reference table 
    int gid;
	Instance group ID
    NDD_INSTANCE *tarinst;
	Instance pointer that attaches an animation
returns:
    NDD_INSTANCE *;
	Pointer to the animation timekeeper instance
notes:
    Cannot be used in an instance callback. 
---------------------------------------------------------------------------- */
NDD_INSTANCE *ndEntryAnimationRaw(NDD_INSTANCE *parent, NDR_ANIM *anp, int gid, NDD_INSTANCE *tarinst)
{
    NDD_INSTANCE *tkp, *isp;
    NDR_ANIM_CHANNEL *ch;
    int i;

    /* Checks whether the parent is correct */
    if(!parent)
      parent = anim_root_instp;

    /* Create a timekeeper */
    tkp = ndGenInstance( parent, NDC_ITYPE_ANIMTK, NDC_IPRIO_ANIMTK, gid );

    /* Copy the registers */
    tkp->flag = 0;
    tkp->u.animtk.tarinst = tarinst;
    tkp->u.animtk.time = 0.0;
    tkp->u.animtk.timescale = 1.0;
    tkp->u.animtk.start = 0;
    tkp->u.animtk.end = anp->total;
    tkp->u.animtk.naniminst = anp->nch;

    ch = anp->chlist;
    for(i=0; i<anp->nch; ++i){
	/* Create an animation channel */
	isp = ndGenInstance( tkp, NDC_ITYPE_ANIM, NDC_IPRIO_ANIM, gid );

	tkp->u.animtk.animinst[i] = isp;

	/* Copy the registers */
	isp->u.anim.time = 0.0;
	isp->u.anim.tarinst = tarinst;
	isp->u.anim.kind = ch[i].kind;
	isp->u.anim.start = ch[i].start;
	isp->u.anim.end = ch[i].end;
	isp->u.anim.nowkeypos = 0;
	isp->u.anim.nkey = ch[i].nkey;
	isp->u.anim.key = ch[i].key;
	isp->u.anim.value = ch[i].value;
    }

    return tkp;
}

/* ---------------------------------------------------------------------------
name:
    ndEntryObject
synopsis:
    NDD_INSTANCE *ndEntryObject(NDD_INSTANCE *parent, int objno, int shapeno, int animno, int gid, int flag);
description:
    Creates instances from the object reference table.
parameters:
    NDD_INSTANCE *parent;
	Pointer to the parent instance.  Added beneath the object root if the value is NULL.
    int objno;
	The object reference table instance
    int shapeno;
	Offset of the shape reference table.  xxshape from the object reference table and the list of positions provided by the numeric value of this parameter are entered as the shapes for the object.xx
    int animno;
	Offset of the animation reference table.  xxWhen animations are entered at the same time, the animations in the table to which this value is added are entered as values of NDR_OBJ.geom_anim.xx
    int gid;
	The base value of the instance ID.  xxThe item that provides the gid, which is specified by NDR_OBJ, is entered for this value in the created object instance structure.xx
    int flag;
	A bit-assigned production flag.  On when the bit is set to 1. 
	  bit 0 ... Set the NDR_OBJ animation links together.
	  bit 1 ... Evaluate by following the links toward the end of the parent-child relation.
returns:
    NDD_INSTANCE *;
	Pointer to the first parent among the created instances.
notes:
    Calls ndEntryObjectRaw internally.
    Note that this function cannot be used in an instance callback. 
 --------------------------------------------------------------------------- */
NDD_INSTANCE *ndEntryObject(NDD_INSTANCE *parent, int objno, int shapeno, int animno, int gid, int flag)
{
    NDD_INSTANCE *isp;
    NDR_OBJ *rop;

    if(!parent)
      parent = obj_root_instp;

    /* Create the object */
    rop = objlist[objno];
    isp = ndEntryObjectRaw(parent, rop, gid);

    /* Take the shape */
    if(rop->shape >= 0){
	isp->u.obj.shape = shapelist[shapeno + rop->shape];
 	isp->u.obj.flag |= NDC_OFLAG_VISIBLE;
    }else{
	isp->u.obj.shape = NULL;
    }

    /* Enter any animations together */
    if((flag & 0x01) && (rop->geom_anim >= 0)){
	NDD_INSTANCE *tkp;
	tkp = ndEntryAnimationRaw(NULL, animlist[animno + rop->geom_anim],
			       gid, isp);
	tkp->u.animtk.loop = rop->loop;
	tkp->u.animtk.timescale = 1.0 / rop->framerate;
    }

    /* Generate and link child instances */
    if((flag & 0x02) && rop->nchild){
	int i;
	NDD_INSTANCE **pp;
	for(pp=&isp->child, i=0; i<rop->nchild; pp=&((*pp)->bros), ++i)
	  *pp = ndEntryObject(isp, objno+rop->child[i],
			      shapeno, animno, gid, flag);
    }

    return isp;
}

/* ---------------------------------------------------------------------------
name:
    ndAttachAnimation
synopsis:
    void ndAttachAnimation(NDD_INSTANCE *parent, NDD_INSTANCE *isp, int objno, int animno, int gid, int flag)
description:
    References the animation links for the object reference table and attaches animations to existing objects.
parameters:
    NDD_INSTANCE *parent;
	Pointer to the parent instance.  Added beneath the animation root if the value is NULL .
    NDD_INSTANCE *isp;
	An object instance pointer that attaches animations.
    int objno;
	The object number in the global resource list.
    int animno;
	Offset of the animation reference table.  xxWhen animations are entered at the same time, the animations in the table to which this value is added are entered as values of NDR_OBJ.geom_anim.xx 
    int gid;
	The base value of the instance group ID. xxThe item that provides the gid, which is specified by NDR_OBJ, is entered for this value in the created object instance structure.xx 
    int flag;
	A bit-assigned production flag.  On when the bit is set to 1. 
	  bit 0 ... Not used
	  bit 1 ... Evaluate by following the links toward the end of the parent-child relation.
 
returns:
    NDD_INSTANCE *;
	Pointer to the first parent among the created instances.
 notes:
    The object instances passed by isp are handled as instances generated by the NDR_OBJ structure list specified by objno. 
    Note that this function cannot be used in an instance callback. 
--------------------------------------------------------------------------- */
void ndAttachAnimation(NDD_INSTANCE *parent, NDD_INSTANCE *isp, int objno, int animno, int gid, int flag)
{
    NDR_OBJ *rop;

    rop = objlist[objno];

    /* If the object has animations, enter them together. */
    if(rop->geom_anim >= 0){
	NDD_INSTANCE *tkp;
	tkp = ndEntryAnimationRaw(parent, animlist[animno+rop->geom_anim],
			       gid, isp);
	tkp->u.animtk.loop = rop->loop;
	tkp->u.animtk.timescale = 1.0 / rop->framerate;
    }

    /* Generate and link child instances */
    if(flag & 0x02){
	int i;
	NDD_INSTANCE *cp;

	for(cp=isp->child, i=0; cp && i<rop->nchild; cp=cp->bros, ++i)
	  ndAttachAnimation(parent, cp, objno+rop->child[i], animno,
			    gid, flag);
    }
}

/* ----------------------------------------------------------------------------
name:
    ndEntryDeformVtx
synopsis:
    NDD_INSTANCE *ndEntryDeformVtx(NDD_INSTANCE *isp, NDR_DEFORM *dmfp, int ofs, int gid);
description:
    Enters vertex deform instances.
parameters:
    NDD_INSTANCE *isp;
	Pointer to a dynamic vertex instance
    NDR_DEFORM *dmfp;
	Pointer to the vertex deform structure
    int ofs;
	Offset used when referencing the list of raw vertex data
    int gid;
	Instance group ID
returns:
    NDD_INSTANCE *;
	Pointer to the created vertex deform instance
notes:
    These instances are added beneath a dedicated parent instance (deform_root_instp).
---------------------------------------------------------------------------- */
static void exitDeformVtx(NDD_INSTANCE *isp, int type)
{
    NDD_INST_DEFORM_VTX *nidp;

    if(type == NDC_ICB_DIE){
	nidp = &isp->u.dfm_vtx;
	free(nidp->rawvtx);
	free(nidp->modify_vtx);
	free(nidp->delta);
    }
}

NDD_INSTANCE *ndEntryDeformVtx(NDD_INSTANCE *isp, NDR_DEFORM *dmfp, int ofs, int gid)
{
    int i, alloclen;
    NDD_INSTANCE *new_isp;
    NDD_INST_DEFORM_VTX *nidp;

    /* Ignore if there is no animation setting */
    if(isp->type != NDC_ITYPE_DYNVTX || !dmfp)
      return NULL;

    /* Create an instance of an interpolated vertex */
    new_isp = ndGenInstance( deform_root_instp, NDC_ITYPE_DEFORM_VTX,
			    NDC_IPRIO_DEFORM, gid );
    new_isp->instCB = exitDeformVtx;
    nidp = &new_isp->u.dfm_vtx;
    nidp->tarinst = isp;

    /* Create an animation table and copy the pointers and counters */
    nidp->key = dmfp->key;
    nidp->nkey = dmfp->nkey;
    nidp->nelem = dmfp->nelem;
    nidp->rawvtx = malloc(sizeof(s16*) * nidp->nkey);
    for(i=0; i<nidp->nkey; ++i)
      nidp->rawvtx[i] = rawvtxlist[dmfp->tblno[i]+ofs];

    /*  Reserve a modulation buffer for rawvtx  */
    alloclen = nidp->nelem * sizeof(s32) * 3;
    nidp->modify_vtx = malloc( alloclen );
    nidp->delta = malloc( alloclen );
	if( deform_flg & NDC_DEFORM_DIRECTION ) nidp->nowkey = nidp->nkey - 1;
	else nidp->nowkey = 0;
	nidp->downcounter = 0;
#if 0
    nidp->nowkey = nidp->downcounter = 0;
#endif
    return new_isp;
}

/* ----------------------------------------------------------------------------
name:
    ndEntryDeformVtxcol
synopsis:
    NDD_INSTANCE *ndEntryDeformVtxcol(NDD_INSTANCE *isp, NDR_DEFORM *dmfp, int ofs, int gid);
description:
    Enters vertex color deform instances.
parameters:
    NDD_INSTANCE *isp;
	Pointer to a dynamic vertex instance
    NDR_DEFORM *dmfp;
	Pointer to the vertex color deform structure
    int ofs;
	Offset when referencing a list of raw vertex data.
    int gid;
	Instance group ID
returns:
    NDD_INSTANCE *;
	Pointer to the created vertex color deform instance.
notes:
    These instances are added to a dedicated parent instance (deform_root_instp).
    The system function malloc() is used internally to reserve 3 segments of memory.  
    In addition, a callback is set to run on exiting; the callback routine frees memory reserved by malloc().
---------------------------------------------------------------------------- */
static void exitDeformVtxcol(NDD_INSTANCE *isp, int type)
{
    NDD_INST_DEFORM_VTXCOL *nidp;

    if(type == NDC_ICB_DIE){
	nidp = &isp->u.dfm_vtxcol;
	free(nidp->rawvtxcol);
	free(nidp->modify_vtxcol);
	free(nidp->delta);
    }
}

NDD_INSTANCE *ndEntryDeformVtxcol(NDD_INSTANCE *isp, NDR_DEFORM *dmfp, int ofs, int gid)
{
    int i, alloclen;
    NDD_INSTANCE *new_isp;
    NDD_INST_DEFORM_VTXCOL *nidp;

    /* Ignore if no animation setting  */
    if(isp->type != NDC_ITYPE_DYNVTX || !dmfp)
      return NULL;

    /* Create a vertex color interpolation instance */
    new_isp = ndGenInstance( deform_root_instp, NDC_ITYPE_DEFORM_VTXCOL,
			    NDC_IPRIO_DEFORM, gid );
    new_isp->instCB = exitDeformVtxcol;
    nidp = &new_isp->u.dfm_vtxcol;
    nidp->tarinst = isp;

    /* Create an animation table and copy the pointers and counters */
    nidp->key = dmfp->key;
    nidp->nkey = dmfp->nkey;
    nidp->nelem = dmfp->nelem;
    nidp->rawvtxcol = malloc(sizeof(u32*) * nidp->nkey);
    for(i=0; i<nidp->nkey; ++i)
      nidp->rawvtxcol[i] = rawvtxcollist[dmfp->tblno[i]+ofs];

    /* Reserve the modulation buffer for rawvtxcol */
    alloclen = nidp->nelem * sizeof(s16) * 4;
    nidp->modify_vtxcol = malloc( alloclen );
    nidp->delta = malloc( alloclen );
    nidp->nowkey = nidp->downcounter = 0;

    return new_isp;
}

/* ----------------------------------------------------------------------------
name:
    ndEntryDeformVtxnv
synopsis:
    NDD_INSTANCE *ndEntryDeformVtxnv(NDD_INSTANCE *isp, NDR_DEFORM *dmfp, int ofs, int gid);
description:
    Enters vertex normal vector deform instances.
parameters:
    NDD_INSTANCE *isp;
	Pointer to a dynamic vertex instance
    NDR_DEFORM *dmfp;
	Pointer to the vertex normal vector deform structure
    int ofs;
	Offset when referencing the list of raw vertex normal vector data
    int gid;
	Instance group ID
returns:
    NDD_INSTANCE *;
	Pointer to a created vertex normal vector deform instance.
notes:
 These instances are added to a dedicated parent instance (deform_root_instp).
    The system function malloc() is used internally to reserve 3 segments of memory. In addition, a callback is set to run on exiting; the callback routine frees memory reserved by malloc().
---------------------------------------------------------------------------- */
static void exitDeformVtxnv(NDD_INSTANCE *isp, int type)
{
    NDD_INST_DEFORM_VTXNV *nidp;

    if(type == NDC_ICB_DIE){
	nidp = &isp->u.dfm_vtxnv;
	free(nidp->rawvtxnv);
	free(nidp->modify_vtxnv);
	free(nidp->delta);
    }
}

NDD_INSTANCE *ndEntryDeformVtxnv(NDD_INSTANCE *isp, NDR_DEFORM *dmfp, int ofs, int gid)
{
    int i, alloclen;
    NDD_INSTANCE *new_isp;
    NDD_INST_DEFORM_VTXNV *nidp;

    /* Ignore if no animation is set */
    if(isp->type != NDC_ITYPE_DYNVTX || !dmfp)
      return NULL;

    /* Create a vertex color interpolation instance  */
    new_isp = ndGenInstance( deform_root_instp, NDC_ITYPE_DEFORM_VTXNV,
			    NDC_IPRIO_DEFORM, gid );
    new_isp->instCB = exitDeformVtxnv;
    nidp = &new_isp->u.dfm_vtxnv;
    nidp->tarinst = isp;

    /* Create an animation table and copy the pointers and counters */
    nidp->key = dmfp->key;
    nidp->nkey = dmfp->nkey;
    nidp->nelem = dmfp->nelem;
    nidp->rawvtxnv = malloc(sizeof(u32*) * nidp->nkey);
    for(i=0; i<nidp->nkey; ++i)
      nidp->rawvtxnv[i] = rawvtxnvlist[dmfp->tblno[i]+ofs];

    /* Reserve the modulation buffer for rawvtxnv */
    alloclen = nidp->nelem * sizeof(s16) * 3;
    nidp->modify_vtxnv = malloc( alloclen );
    nidp->delta = malloc( alloclen );
    nidp->nowkey = nidp->downcounter = 0;

    return new_isp;
}

/* ----------------------------------------------------------------------------
name:
    ndEntryDeformShape
synopsis:
    NDD_INSTANCE *ndEntryDeformShape(NDD_INSTANCE *isp, int ofs_rawvtx, int ofs_rawvtxcol, int ofs_rawvtxnv, int gid, int flag);
description:
    Takes over shapes referenced by object instances and substitutes modulated vertex data.
parameters:
    NDD_INSTANCE *isp;
	Pointer to the object to be modulated
    int ofs_rawvtx;
    int ofs_rawvtxcol;
    int ofs_rawvtxnv;
	Offset when referencing data lists
    int gid;
	Instance group ID
    int flag;
	A bit-assignment generation flag.  On when the bit is set to 1. 
	  bit 0 ... Enter the vertex deform 
	  bit 1 ... Enter the vertex color deform
	  bit 2 ... Enter the vertex normal vector deform
returns:
    NDD_INSTANCE *;
	Pointer to a created dynamic vertex instance
notes:
      Creates instances corresponding to the xxupper portionsxx of ndEntryDeformVtx(), ndEntryDeformVtxnv(), ndEntryDeformVtxcol() .
      The function internally monitors the parameter flag while evaluating generation of the deform instances.
      Two Vtx buffers are used to match the asynchronicity of Gfx construction.
    The system function malloc() is used internally to reserve 3 segments of memory. In addition, a callback is set to run on exiting; the callback routine frees the memory reserved by malloc(). 
---------------------------------------------------------------------------- */
static void exitDynamicVtx(NDD_INSTANCE *isp, int type)
{
    NDD_INST_DYNAMICVTX *dvip;

    if(type == NDC_ICB_DIE){
	dvip = &isp->u.dynvtx;
	/* Restores object of the target to its original shape. */
	dvip->tarinst->u.obj.shape = dvip->src_shape;
	/* Note:  Pointers below are freed regardless of whether they are being used for rendering, so please use caution in deleting instances. */
	free(dvip->tarvtx[0]);
	free(dvip->tarvtx[1]);
	free(dvip->dyn_shape);
    }
}

NDD_INSTANCE *ndEntryDeformShape(NDD_INSTANCE *isp, int ofs_rawvtx, int ofs_rawvtxcol, int ofs_rawvtxnv, int gid, int flag)
{
    int i;
    NDD_INSTANCE *new_isp;
    NDD_INST_DYNAMICVTX *dvip;
    NDR_SHAPE *src_shape, *new_shape;

    /* Ignore if not an object instance or if shaperef absent */
    if(isp->type != NDC_ITYPE_OBJECT || !isp->u.obj.shape ||
       !isp->u.obj.shape->shaperef)
      return NULL;

    /* Create a dynamic vertex instance (added beneath the object) */
    new_isp = ndGenInstance( dynvtx_root_instp, NDC_ITYPE_DYNVTX,
			NDC_IPRIO_DYNVTX, gid );
    new_isp->instCB = exitDynamicVtx;
    dvip = &new_isp->u.dynvtx;
    dvip->tarinst = isp;

    /* Reserve a dynamic vertex buffer and copy contents */
    src_shape = isp->u.obj.shape;
    i = sizeof(Vtx)*src_shape->nvtx;
    dvip->tarvtx[0] = malloc(i);
    memcpy(dvip->tarvtx[0], src_shape->vtx, i);
    dvip->tarvtx[1] = malloc(i);
    memcpy(dvip->tarvtx[1], src_shape->vtx, i);

    /* Create new NDR_SHAPE and copy contents */
    dvip->dyn_shape = new_shape = malloc(sizeof(NDR_SHAPE));
    memcpy(new_shape, isp->u.obj.shape, sizeof(NDR_SHAPE));

    /* Rewrite NDR_SHAPE for the host object */
    dvip->src_shape = isp->u.obj.shape;  /* Backup */
    isp->u.obj.shape = dvip->dyn_shape;

    /* Initialize each table modulation instance */
    for(i=0; i<4; ++i)
      dvip->dfminst[i] = NULL;

    /* Construct the table modulation instances */
    if(flag & 0x01)
      dvip->dfminst[0] = ndEntryDeformVtx(new_isp, new_shape->vtx_deform,
			   ofs_rawvtx, gid);
    if(flag & 0x02)
      dvip->dfminst[1] = ndEntryDeformVtxcol(new_isp,
			   new_shape->vtxcol_deform, ofs_rawvtxcol, gid);
    if(flag & 0x04)
      dvip->dfminst[2] = ndEntryDeformVtxnv(new_isp, new_shape->vtxnv_deform,
			   ofs_rawvtxnv, gid);

    return new_isp;
}

/* ---------------------------------------------------------------------------
name:
    ndAttachDeform
synopsis:
    void ndAttachDeform(NDD_INSTANCE *isp, int rawvtxno, int rawvtxcolno, int rawvtxnvno, int gid, int flag);
description:
    Enters deforms provided for stratified object instances.  
parameters:
    NDD_INSTANCE *isp;
	xxPointer to the object instances for which dynamic vertex instances are to be generated.xx
    int rawvtxno;
    int rawvtxcolno;
    int rawvtxnvno;
	Offset when data lists are referenced.
    int gid;
	Instance group ID
    int flag;
	Bit-assigned flags.  On when set to 1.
	  bit 0 ... Not used
	  bit 1 ... Evaluate by following link toward end of parent-child relation
	  bit 2 ... Enter vertex deform
	  bit 3 ... Enter vertex color deform
	  bit 4 ... Enter vertex normal vector deform
returns:
notes:
 --------------------------------------------------------------------------- */
void ndAttachDeform(NDD_INSTANCE *isp, int rawvtxno, int rawvtxcolno, int rawvtxnvno, int gid, int flag)
{
    if(isp->type == NDC_ITYPE_OBJECT && isp->u.obj.shape &&
       isp->u.obj.shape->shaperef)
      ndEntryDeformShape(isp, rawvtxno, rawvtxcolno, rawvtxnvno, gid,
			 (flag&0x1c) >> 2);

    /* Generate and link child instances */
    if(flag & 0x02){
	NDD_INSTANCE *cp;

	for(cp=isp->child; cp; cp=cp->bros)
	  ndAttachDeform(cp, rawvtxno, rawvtxcolno, rawvtxnvno, gid, flag);
    }
}

/* ----------------------------------------------------------------------------
name:
    ndSetupScene
synopsis:
    void ndSetupScene(NDD_RESINFO *rip, int gid, int flag);
description:
    Builds a scene from the scene reference table.
parameters:
    NDD_RESINFO *rip;
	Resource information
    int gid;
	Group ID for entering object instances
    int flag;
	Generation status flag
	A production flag. Setting the bit to 1 enables the following.
	  bit 0 ... Establish all of the NDR_OBJ animation links at the same time.
	  bit 1 ... Evaluate by following link toward end of parent-child relation
	  bit 2 ... Enter vertex deform 
	  bit 3 ... Enter vertex color deform
	  bit 4 ... Enter vertex normal vector deform
returns:
notes:
    The scene data structure used for the settings references rip->resinfo->scene.
---------------------------------------------------------------------------- */
void ndSetupScene(NDD_RESINFO *rip, int gid, int flag)
{
    NDR_SCENE *scene;
    NDR_SCN_INST_OBJ *sio;
    NDR_SCN_LIGHT *lp;
    int i,nlight;

    scene = rip->resinfo->scene;
    cur_scene.fog.near=scene->fog.near;
    cur_scene.fog.far=scene->fog.far;
    cur_scene.fog.color.r=scene->fog.r;
    cur_scene.fog.color.g=scene->fog.g;
    cur_scene.fog.color.b=scene->fog.b;
    cur_scene.fog.color.a=scene->fog.a;

    if(scene){
	/* If a scene is enabled */
	cur_scene.camera.mode = scene->cam.mode;
	cur_scene.camera.fovy = scene->cam.fovy;
	cur_scene.camera.aspect = scene->cam.aspect;
	cur_scene.camera.scale = 1.0;  /* scene->cam.scale; */
	cur_scene.camera.top = scene->cam.top;
	cur_scene.camera.bottom = scene->cam.bottom;
	cur_scene.camera.left = scene->cam.left;
	cur_scene.camera.right = scene->cam.right;
	if((cur_scene.camera.near = scene->cam.near)<50)
	  cur_scene.camera.near = 50;
	cur_scene.camera.far = scene->cam.far;

	cur_scene.fill_col = scene->fill_col;
	cur_scene.fill_depth = scene->fill_depth;

	/* Make the objects into instances */
	sio = scene->entryobj;
	cur_scene.cam[0]=cur_scene.cam[1]=cur_scene.cam[2]=NULL;
	cur_scene.enacam = 0;
	for(i=0; i<scene->nentobj; ++i){
	    NDD_INSTANCE *isp;
	    isp = ndEntryObject(NULL, rip->obj.ofs + sio[i].objno,
		    rip->shape.ofs, rip->anim.ofs, gid, flag & 0x03);
	    memcpy(ndGetInstanceTRXPointer(isp), &sio[i].trx, sizeof(TRX));

	    if(flag & 0x1c)
	      ndAttachDeform(isp, rip->rawvtx.ofs, rip->rawvtxnv.ofs,
	        rip->rawvtxcol.ofs, gid, (flag &0x1c)+2);
#if 0
	    if(i == scene->cam.eye_inst)
	      cur_scene.cam[0]=isp;
	    else if(i == scene->cam.upper_inst)
	      cur_scene.cam[1]=isp;
	    else if(i == scene->cam.lookat_inst)
	      cur_scene.cam[2]=isp;
#endif
	}

	/* Load the lighting data */
	memcpy(&cur_scene.lightset.ambient, &scene->light.amb_color,
	       sizeof(RGBAQUAD));
	cur_scene.lightset.numlights = nlight = scene->light.nlight;
	lp = scene->light.light;
	for(i=0; i<nlight; ++i){
	    /* xxSet the provided light source normallyxx */
	    memcpy(&cur_scene.lightset.light[i].color, &lp->color,
		   sizeof(RGBAQUAD));
	    if(lp->type){
		/* Point light source */
		cur_scene.lightset.light[i].flag =
		  NDC_LFLAG_ENABLE | NDC_LFLAG_HAVEPOS;
		cur_scene.lightset.light[i].fpos[0] = lp->x;
		cur_scene.lightset.light[i].fpos[1] = lp->y;
		cur_scene.lightset.light[i].fpos[2] = lp->z;
	    }else{
		/* Directional light source */
		cur_scene.lightset.light[i].flag = NDC_LFLAG_ENABLE;
		cur_scene.lightset.light[i].fnv[0] = lp->x;
		cur_scene.lightset.light[i].fnv[1] = lp->y;
		cur_scene.lightset.light[i].fnv[2] = lp->z;
	    }
	    cur_scene.lightset.light[i].start = lp->a1;
	    cur_scene.lightset.light[i].end = lp->a2;
	}
#if 0
    }else{
	/* Set the default camera data in cases where the scene is disabled */
	cur_scene.camera.mode = 0;
	cur_scene.camera.fovy = 45.0;
	cur_scene.camera.aspect = 1.0*SCREEN_WD/SCREEN_HT;
	cur_scene.camera.scale = 1.0;
	cur_scene.camera.top = -SCREEN_HT/2;
	cur_scene.camera.bottom = SCREEN_HT/2;
	cur_scene.camera.left = -SCREEN_WD/2;
	cur_scene.camera.right = SCREEN_WD/2;
	cur_scene.camera.near = 50;
	cur_scene.camera.far = 20000;
	cur_scene.fill_col = GPACK_RGBA5551(120, 0, 0, 255);
	cur_scene.fill_depth = GPACK_ZDZ(G_MAXFBZ, 0);
	cur_scene.cam[0]=cur_scene.cam[1]=cur_scene.cam[2]=NULL;
	cur_scene.enacam = 0;
#endif
    }

    /* Enable camera if xxthere are 3 camera-controlled objectsxx */
    if(cur_scene.cam[0] && cur_scene.cam[1] && cur_scene.cam[2]){
	TRX *ep, *up, *ap;

	cur_scene.enacam = 1;
	/* Check whether the positions of the 3 objects attached to the camera are coordinated. */
	ep = ndGetInstanceTRXPointer(cur_scene.cam[0]);
	up = ndGetInstanceTRXPointer(cur_scene.cam[1]);
	ap = ndGetInstanceTRXPointer(cur_scene.cam[2]);
	if( ep->tx == up->tx && ep->tx == ap->tx &&
	    ep->ty == up->ty && ep->ty == ap->ty &&
	    ep->tz == up->tz && ep->tz == ap->tz ){
	    ep->tx = ep->ty = 0.0; ep->tz = -500;
	    ap->tx = ap->ty = ap->tz = 0.0;
	    up->tx = ep->tx; up->ty = ep->ty+1.0; up->tz = ep->tz;
	}
    }
}

/* ----------------------------------------------------------------------------
name:
    ndSetScreen
synopsis:
    void ndSetScreen(int top, int bottom, int left, int right);
description:
    Sets the screen width and height for the display area.
parameters:
    int top;
    int bottom;
    int left;
    int right;
	The display area in pixels
returns:
notes:
---------------------------------------------------------------------------- */
void ndSetScreen(int top, int bottom, int left, int right)
{
    cur_scene.top = top;
    cur_scene.bottom = bottom;
    cur_scene.left = left;
    cur_scene.right = right;
}

void ndAttachAnimation2(NDD_INSTANCE *parent, NDD_INSTANCE *isp, int objno, int animno, int gid, int flag, u8 basetime)
{
    NDR_OBJ *rop;

    rop = objlist[objno];

    /* Enter any animations at the same time */
    if(rop->geom_anim >= 0){
	NDD_INSTANCE *tkp;
	tkp = ndEntryAnimationRaw(parent, animlist[animno+rop->geom_anim],
			       gid, isp);
	tkp->u.animtk.loop = rop->loop;
	tkp->u.animtk.timescale = 1.0 / rop->framerate * basetime;
    }

    /* Generate and link child instances */
    if(flag & 0x02){
	int i;
	NDD_INSTANCE *cp;

	for(cp=isp->child, i=0; cp && i<rop->nchild; cp=cp->bros, ++i)
	  ndAttachAnimation2(parent, cp, objno+rop->child[i], animno,
			    gid, flag, basetime);
    }
}

