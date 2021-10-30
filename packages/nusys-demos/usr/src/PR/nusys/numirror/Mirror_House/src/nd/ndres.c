/*======================================================================*/
/*		NIFFprev						*/
/*		ndres.c							*/
/*									*/
/*		Copyright (C) 1997,1998, NINTENDO Co,Ltd.		*/
/*									*/
/*======================================================================*/
#define __NDRES_C__

/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	Simple 3D Manager ND 

	Resource Manager
 :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#include <pack.h>
#include <nd.h>

/* ===========================================================================
	defines
 =========================================================================== */

/* ===========================================================================
	variables
 =========================================================================== */

/* Global resource list */
NDR_OBJ **objlist;
u16 objlist_top;
u16 objlist_lim;

NDR_SHAPE **shapelist;
u16 shapelist_top;
u16 shapelist_lim;

NDR_ANIM **animlist;
u16 animlist_top;
u16 animlist_lim;

s16 **rawvtxlist;
u16 rawvtxlist_top;
u16 rawvtxlist_lim;

u32 **rawvtxcollist;
u16 rawvtxcollist_top;
u16 rawvtxcollist_lim;

s8 **rawvtxnvlist;
u16 rawvtxnvlist_top;
u16 rawvtxnvlist_lim;

/* ===========================================================================
	functions
 =========================================================================== */

static void initPointerList(void *vp, int vpplen)
{
    void **vpp = vp;
    int i;

    for(i=0; i<vpplen; ++i)
      vpp[i] = NULL;
}

/* ----------------------------------------------------------------------------
name:
    ndResInit
synopsis:
    void ndResInit(int olplim, int slplim, int alplim, int rvlplim, int rvclplim, int rvnlplim);
description:
    Initializes resource modules.
parameter:
    int olplim;
    int slplim;
    int alplim;
    int rvlplim;
    int rvclplim;
    int rvnlplim;
	List management pointers and limits for the data classes.
return:
notes:
    Memory is reserved for the pointer list using the system function malloc().
---------------------------------------------------------------------------- */
void ndResInit(int olplim, int slplim, int alplim, int rvlplim, int rvclplim, int rvnlplim)
{
    objlist = malloc(olplim << 2);
    objlist_lim = olplim;
    objlist_top = 0;
    initPointerList(objlist, objlist_lim);

    shapelist = malloc(slplim << 2);
    shapelist_lim = slplim;
    shapelist_top = 0;
    initPointerList(shapelist, shapelist_lim);

    animlist = malloc(alplim << 2);
    animlist_lim = alplim;
    animlist_top = 0;
    initPointerList(animlist, animlist_lim);

    rawvtxlist = malloc(rvlplim << 2);
    rawvtxlist_lim = rvlplim;
    rawvtxlist_top = 0;
    initPointerList(rawvtxlist, rawvtxlist_lim);

    rawvtxcollist = malloc(rvclplim << 2);
    rawvtxcollist_lim = rvclplim;
    rawvtxcollist_top = 0;
    initPointerList(rawvtxcollist, rawvtxcollist_lim);

    rawvtxnvlist = malloc(rvnlplim << 2);
    rawvtxnvlist_lim = rvnlplim;
    rawvtxnvlist_top = 0;
    initPointerList(rawvtxnvlist, rawvtxnvlist_lim);
}

/* ----------------------------------------------------------------------------
name:
    ndResEntry
synopsis:
    NDD_RESINFO *ndResEntry(NDR_RESINFO *rrp, int usemask);
description:
    Adds the specified resource to the global resource list.  
parameter:
    NDR_RESINFO *rrp;
	Pointer to the resource information structure used for referencing.
    int usemask;
	Bit specification of the type of data to be entered.  Based on the contents of rrp .
return:
    NDR_RESINFO *drp;
	Pointer to the resource information structure.
notes:
---------------------------------------------------------------------------- */
static int entry_resource( void *dlp, void *slp, int ofs, int cnt, NDD_RESOFFSET *ofsp)
{
    void **dlpp = dlp;
    void **slpp = slp;
    int i;

    for(i=0; i<cnt; ++i)
      dlpp[ofs+i] = slpp[i];

    ofsp->ofs = ofs;
    ofsp->count = cnt;

    return ofs+cnt;
}

NDD_RESINFO *ndResEntry(NDR_RESINFO *rrp, int usemask)
{
    NDD_RESINFO *drp;

    drp = malloc(sizeof(NDD_RESINFO));
    drp->resinfo = rrp;

    if(BITTEST(usemask, NDC_RES_USED_OBJ) && rrp->nobj){
	objlist_top =
	  entry_resource( objlist, rrp->obj, objlist_top, rrp->nobj,
			 &drp->obj);
    }else{
	BITRESET(usemask, NDC_RES_USED_OBJ);
	drp->obj.ofs = drp->obj.count = 0;
    }

    if(BITTEST(usemask, NDC_RES_USED_SHAPE) && rrp->nshape){
	shapelist_top =
	  entry_resource( shapelist, rrp->shape, shapelist_top, rrp->nshape,
			 &drp->shape);
    }else{
	BITRESET(usemask, NDC_RES_USED_SHAPE);
	drp->shape.ofs = drp->shape.count = 0;
    }

    if(BITTEST(usemask, NDC_RES_USED_ANIM) && rrp->nanim){
	animlist_top =
	  entry_resource( animlist, rrp->anim, animlist_top, rrp->nanim,
			 &drp->anim);
    }else{
	BITRESET(usemask, NDC_RES_USED_ANIM);
	drp->anim.ofs = drp->anim.count = 0;
    }

    if(BITTEST(usemask, NDC_RES_USED_RAWVTX) && rrp->nrawvtx){
	rawvtxlist_top =
	  entry_resource( rawvtxlist, rrp->rawvtx, rawvtxlist_top,
			 rrp->nrawvtx, &drp->rawvtx);
    }else{
	BITRESET(usemask, NDC_RES_USED_RAWVTX);
	drp->rawvtx.ofs = drp->rawvtx.count = 0;
    }

    if(BITTEST(usemask, NDC_RES_USED_RAWVTXCOL) && rrp->nrawvtxcol){
	rawvtxcollist_top =
	  entry_resource( rawvtxcollist, rrp->rawvtxcol, rawvtxcollist_top,
			 rrp->nrawvtxcol, &drp->rawvtxcol);
    }else{
	BITRESET(usemask, NDC_RES_USED_RAWVTXCOL);
	drp->rawvtxcol.ofs = drp->rawvtxcol.count = 0;
    }

    if(BITTEST(usemask, NDC_RES_USED_RAWVTXNV) && rrp->nrawvtxnv){
	rawvtxnvlist_top =
	  entry_resource( rawvtxnvlist, rrp->rawvtxnv, rawvtxnvlist_top,
			 rrp->nrawvtxnv, &drp->rawvtxnv);
    }else{
	BITRESET(usemask, NDC_RES_USED_RAWVTXNV);
	drp->rawvtxnv.ofs = drp->rawvtxnv.count = 0;
    }

    drp->usemask = usemask;

    return drp;
}

/* ----------------------------------------------------------------------------
name:
    ndResRelease
synopsis:
    void ndResRelease(NDD_RESINFO *drp);
description:
    Removes the specified resource from the global list.
parameter:
    NDD_RESINFO *drp;
	Pointer to the resource information structure.
return:
notes:
    The parameter drp is released.
    Only resources entered immediately beforehand can be released.
---------------------------------------------------------------------------- */
void ndResRelease(NDD_RESINFO *drp)
{
    objlist_top = drp->obj.ofs;
    shapelist_top = drp->shape.ofs;
    animlist_top = drp->anim.ofs;
    rawvtxlist_top = drp->rawvtx.ofs;
    rawvtxcollist_top = drp->rawvtxcol.ofs;
    rawvtxnvlist_top = drp->rawvtxnv.ofs;

    free(drp);
}


