/*======================================================================*/
/*		NIFFprev						*/
/*		ndeval.c						*/
/*									*/
/*		Copyright (C) 1997,1998, NINTENDO Co,Ltd.		*/
/*									*/
/*======================================================================*/
#define __NDEVAL_C__

/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	Simple 3D Manager ND 

	Evaluation Functions
 :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#include <string.h>
#include "pack.h"
#include "nd.h"

/* ===========================================================================
	defines
 =========================================================================== */

/* ===========================================================================
	variables
 =========================================================================== */

/* ----------------------------------------------------------------------------
name:
    ndGetInstanceTRXPointer
synopsis:
    TRX *ndGetInstanceTRXPointer(NDD_INSTANCE *isp);
description:
    Returns a pointer for the transform buffer given the pointer for the instance.
parameters:
    NDD_INSTANCE *isp;
	Instance pointer.
returns:
notes:
    This type of function exists because the TRX location for the instance structure varies depending on the data type.
---------------------------------------------------------------------------- */
TRX *ndGetInstanceTRXPointer(NDD_INSTANCE *isp)
{
    if(isp->type == NDC_ITYPE_OBJECT){
	return &isp->u.obj.trx;
    }if(isp->type == NDC_ITYPE_NULL){
	return &isp->u.obj.trx;
    }

    return NULL;
}

/* ----------------------------------------------------------------------------
name:
    ndSetupIdentTRX
synopsis:
    void ndSetupIdentTRX(TRX *p)
description:
    Returns the unit string.
parameters:
    TRX *p;
	Pointer for the transform matrix.
returns:
notes:
	Assigns 0,0,0, 0,0,0, 1,1,1, which corresponds to the unit string for the 9 parameters for translational movement, rotation, and zooming.

---------------------------------------------------------------------------- */
void ndSetupIdentTRX(TRX *p)
{
    static TRX ident={0,0,0, 0,0,0, 1,1,1};

    memcpy(p, &ident, sizeof(TRX));
}

/* ----------------------------------------------------------------------------
name:
    ndMakeCameraMatrix
synopsis:
    void ndMakeCameraMatrix(float mf[4][4])
description:
    From the camera data, creates a camera matrix (LookAt) in mf .
parameters:
returns:
notes:
 	Please be aware that, unlike matrices such as Perspective, this matrix is unrelated to PROJECTION_VIEW.
---------------------------------------------------------------------------- */
void ndMakeCameraMatrix(float mf[4][4])
{
    TRX *ep, *up, *ap;

    if(cur_scene.enacam){
	ep = ndGetInstanceTRXPointer(cur_scene.cam[0]);
	up = ndGetInstanceTRXPointer(cur_scene.cam[1]);
	ap = ndGetInstanceTRXPointer(cur_scene.cam[2]);
	guLookAtF(mf, ep->tx, ep->ty, ep->tz,
	          ap->tx, ap->ty, ap->tz,
	          up->tx, up->ty, up->tz);
    }else{
	guMtxIdentF( mf );
    }
}

/* ---------------------------------------------------------------------------
name:
    ndProgressAnimTime
synopsis:
    int ndProgressAnimTime(NDD_INST_ANIMTK *ntkp);
description:
    Processes the progress of timekeeper time.
parameters:
    NDD_INST_ANIMTK *ntkp
	Pointer for the animation timekeeper instance.
returns:
    int;
	Result of execution;  currently always returns 1.
notes:
    Called by ndEvalInstance.
 --------------------------------------------------------------------------- */
int ndProgressAnimTime(NDD_INST_ANIMTK *ntkp)
{
    float time;
    int i;

    /* Advance the time */
    time = ntkp->time + ntkp->timescale;

    /* Check whether the ending time has been exceeded. */
    if(time > ntkp->end){
	/* reserve:  Now a forced loop */
	time = ntkp->start;
    }

    /* Determine the time */
    ntkp->time = time;

    /* Distributes the time value to the entered animation instances */
    for(i=0; i<ntkp->naniminst; ++i)
      ntkp->animinst[i]->u.anim.time = time;

    return TRUE;
}

/* ---------------------------------------------------------------------------
name:
    ndSetAnimChannel
synopsis:
    int ndSetAnimChannel(NDD_INST_ANIM *niap);
description:
    Processes the key frame animation channel.
parameters:
    NDD_INST_ANIM *niap;
	Pointer for the animation instance.
returns:
    Execution result.
notes:
    Called by ndEvalInstance.
 --------------------------------------------------------------------------- */
int ndSetAnimChannel(NDD_INST_ANIM *niap)
{
    int i;
    float c1, c2, time, v1, v2;
    float *fvp;

    /* Check the clock */
    time = niap->time;

    /* Exit if not in the xxplaybackxx range */
    if(time < niap->start || time > niap->end)
      return FALSE;

    /* -----------------------------------------------------------
       xxIf the time is within the playback range, pressing a key stamps the current time value (otherwise the data are abnormal).xx
       ----------------------------------------------------------- */

    /* Searches for the key to reference */
    i = niap->nowkeypos;
    if(time < niap->key[i]){
	/* Time reversed */
	for(--i; time < niap->key[i]; --i)
	  ;
    }else{
	/* Search permutationally beginning from the previously referenced key */
	for(; time > (float)niap->key[i]; ++i)
	  ;
	--i;
    }

    /* Obtain the TRX pointer for the xxhostxx instance */
    fvp = (float*)ndGetInstanceTRXPointer(niap->tarinst);

    /* Take the next Key and compare it with the current time */
    c2 = niap->key[i+1];
    if(time == c2){
	/* If they are the same, assign that position to nowkeypos and return value[nowkeypos] */
	niap->nowkeypos = i+1;
	fvp[niap->kind] = niap->value[niap->nowkeypos];
    }else{
	/* Interpolate between c1 and c2.  Currently only Linear interpolation.  */
	c1 = niap->key[i];
	v1 = niap->value[i];
	v2 = niap->value[i+1];
	fvp[niap->kind] = v1+(v2-v1)/(c2-c1)*(time-c1);
    }

    /* Designate matrix recalculation */
    BITRESET(niap->tarinst->flag, NDC_IFLAG_MTX_READY);
    return TRUE;
}

/* ---------------------------------------------------------------------------
name:
    ndReloadDynamicVertex
synopsis:
    void ndReloadDynamicVertex(NDD_INST_DYNAMICVTX *nidp);
description:
    References the xxmodulation tablexx for each type of deform instance and sets up Vtx .
parameters:
    NDD_INST_DYNAMICVTX *nidp;
	Pointer to the internal structure for dynamic vertex instances.
returns:
notes:
    Partly dependent on the rendering driver; two tables are provided for Vtx.
 --------------------------------------------------------------------------- */
void ndReloadDynamicVertex(NDD_INST_DYNAMICVTX *nidp)
{
    int i, j, nvtx;
    NDR_SHAPE *dsp;
    Vtx *tar;
    s16 *vmap;
    s32 *mvp;
    s16 *mnp, *mcp;

    /* Replaces Vtx of the host object's shape. */
    dsp = nidp->dyn_shape;
    tar = nidp->tarvtx[building_gb];
    vmap = dsp->shaperef;
    mvp = (nidp->dfminst[0])?
      nidp->dfminst[0]->u.dfm_vtx.modify_vtx: NULL;
    mcp = (nidp->dfminst[1])?
      nidp->dfminst[1]->u.dfm_vtxcol.modify_vtxcol: NULL;
    mnp = (nidp->dfminst[2])?
      nidp->dfminst[2]->u.dfm_vtxnv.modify_vtxnv: NULL;
    nvtx = dsp->nvtx;
    for(i=0; i<nvtx; ++i){
	/* vertex */
	if(mvp && (*vmap>=0)){
	    j = *vmap + (*vmap<<1);
	    tar[i].n.ob[0] = mvp[j++] >> 16;
	    tar[i].n.ob[1] = mvp[j++] >> 16;
	    tar[i].n.ob[2] = mvp[j] >> 16;
	}
	++vmap;

	/* vertex color */
	if(mcp && (*vmap>=0)){
	    j = *vmap<<2;
	    tar[i].v.cn[0] = mcp[j++] >> 8;
	    tar[i].v.cn[1] = mcp[j++] >> 8;
	    tar[i].v.cn[2] = mcp[j++] >> 8;
	    tar[i].v.cn[3] = mcp[j] >> 8;
	}
	++vmap;

	/* vertex nv */
	if(mnp && (*vmap>=0)){
	    j = *vmap + (*vmap<<1);
	    tar[i].n.n[0] = mnp[j++] >> 8;
	    tar[i].n.n[1] = mnp[j++] >> 8;
	    tar[i].n.n[2] = mnp[j] >> 8;
	}
	++vmap;
    }

    /* Write the cache of the rewritten Vtx buffer */
    osWritebackDCache(tar, nvtx*sizeof(Vtx));

    /* Replaces Vtx for the host object's shape */
    dsp->vtx = tar;
}

/* ---------------------------------------------------------------------------
name:
    ndDeformVertex
synopsis:
    void ndDeformVertex(NDD_INST_DEFORM_VTX *nidvp);
description:
    Deforms the vertex.
parameters:
    NDD_INST_DEFORM_VTX *nidvp;
	Pointer to the internal structure for vertex deform instances.
returns:
notes:
 --------------------------------------------------------------------------- */
void ndDeformVertex(NDD_INST_DEFORM_VTX *nidvp)
{
    int i,nelem;
    s32 *mvp, *delta;

    nelem = nidvp->nelem+(nidvp->nelem<<1);  /* x3 */
    mvp = nidvp->modify_vtx;
    delta = nidvp->delta;

	/* Processing of the moment at which the deform direction changes */
	if( deform_flg & NDC_DEFORM_DIRECTION_TRIG )
	{
		int key , dc;

		key = nidvp->nowkey;
		if( deform_flg & NDC_DEFORM_DIRECTION )				/* xxPrevious time + time of direction xx*/
			dc = nidvp->key[key]- nidvp->key[key - 1];
		else dc = nidvp->key[key + 1] - nidvp->key[key];	/* xxPrevious time - time of directionxx */
		nidvp->downcounter = dc - nidvp->downcounter;
	}

    if(nidvp->downcounter){
	/* Modulate delta value and vertex position */
	if( deform_flg & NDC_DEFORM_DIRECTION )
		for( i = 0 ; i < nelem ; ++i ) mvp[i] -= delta[i];
	else
	for(i=0; i<nelem; ++i)
	  mvp[i] += delta[i];
	--nidvp->downcounter;
    }else{
	int key, dc;
	float div;
	s16 *s0, *s1;

	if( deform_flg & NDC_DEFORM_DIRECTION )
	{
		/* Restore key */
		key = --nidvp->nowkey;
		/* With the STOP flag, do nothing if the key is the final key */
		if( key <= 0 && (deform_flg & NDC_DEFORM_STOP) )
		{
			s1 = nidvp->rawvtx[0];
			for( i = 0 ; i < nelem ; ++i )
			{
				mvp[i] = (s32)s1[i] << 16;
				delta[i] = 0;
			}
			nidvp->downcounter = 1;
			nidvp->nowkey = 0;
			deform_flg |= NDC_DEFORM_END;
			return;
		}
		/* When looping */
		if( key <= 0 ) key = nidvp->nkey - 1;
		nidvp->nowkey = key;
		
		/* Initialize the modulated vertex position and calculate delta values up to the previous key */
		s0 = nidvp->rawvtx[key];
		s1 = nidvp->rawvtx[key + 1];
		dc = nidvp->key[key + 1]- nidvp->key[key];
		div = 65536.0 / dc;
		for( i = 0 ; i < nelem ; ++i )
		{
			mvp[i] = (s32)s0[i] << 16;
			delta[i] = (s32)(((float)s1[i] - s0[i]) * div);
		}
	}
	else
	{
	/* Advance the key */
	key = ++nidvp->nowkey;
	/* With the STOP flag, do nothing if the key is the final key */
	if( key >= nidvp->nkey && (deform_flg & NDC_DEFORM_STOP) )
	{
		s0 = nidvp->rawvtx[key-1];
		for( i = 0 ; i < nelem ; ++i )
		{
			mvp[i] = (s32)s0[i]<<16;
			delta[i] = 0;
		}
		nidvp->downcounter = 1;
		nidvp->nowkey = nidvp->nkey - 1;
		deform_flg |= NDC_DEFORM_END;
		return;
	}
	if(key >= nidvp->nkey)
	  key=1;
	nidvp->nowkey = key;

	/* Initialize the modulated vertex position and calculate delta values up to the next key */
	s0 = nidvp->rawvtx[key-1];
	s1 = nidvp->rawvtx[key];
	dc = nidvp->key[key] - nidvp->key[key-1];
	div = 65536.0/dc;
	for(i=0; i<nelem; ++i){
	    mvp[i] = (s32)s0[i]<<16;
	    delta[i] = (s32)(((float)s1[i]-s0[i])*div);
	}

	}
	/* Assign a new value to downcounter */
	nidvp->downcounter = dc;
    }
}

/* ---------------------------------------------------------------------------
name:
    ndDeformVertexNv
synopsis:
    void ndDeformVertexNv(NDD_INST_DEFORM_VTXNV *nidcp);
description:
    Deforms the vertex normal vector.
parameters:
    NDD_INST_DEFORM_VTXNV *nidcp;
	Pointer to the internal structure for vertex normal vector instances.
returns:
notes:
 --------------------------------------------------------------------------- */
void ndDeformVertexNv(NDD_INST_DEFORM_VTXNV *nidcp)
{
    int i,nelem;
    s16 *mcp, *delta;

    nelem = nidcp->nelem+(nidcp->nelem<<1);  /* x3 */
    mcp = nidcp->modify_vtxnv;
    delta = nidcp->delta;

    if(nidcp->downcounter){
	/* Modulate the delta value and the vertex color */
	for(i=0; i<nelem; ++i)
	  mcp[i] += delta[i];
	--nidcp->downcounter;
    }else{
	int key, dc;
	float div;
	s8 *s0, *s1;

	/* Advance the key */
	key = ++nidcp->nowkey;
	if(key >= nidcp->nkey)
	  key=1;
	nidcp->nowkey = key;

	/* Initialize the modulated vertex position and calculate delta values up to the next key  */
	s0 = nidcp->rawvtxnv[key-1];
	s1 = nidcp->rawvtxnv[key];
	dc = nidcp->key[key] - nidcp->key[key-1];
	div = 256.0/dc;
	for(i=0; i<nelem; ++i){
	    mcp[i] = (s16)s0[i]<<8;
	    delta[i] = (s16)(((float)s1[i]-s0[i])*div);
	}

	/* Assign a new value to downcounter */
	nidcp->downcounter = dc;
    }
}

/* ---------------------------------------------------------------------------
name:
    ndDeformVertexColor
synopsis:
    void ndDeformVertexColor(NDD_INST_DEFORM_VTXCOL *nidcp);
description:
    Deforms the vertex color. 
parameters:
    NDD_INST_DEFORM_VTXCOL *nidcp;
	Pointer to the internal structure for vertex color deform instances.
returns:
notes:
 --------------------------------------------------------------------------- */
void ndDeformVertexColor(NDD_INST_DEFORM_VTXCOL *nidcp)
{
    int i,nelem;
    s16 *mcp, *delta;

    nelem = nidcp->nelem<<2;  /* x4 */
    mcp = nidcp->modify_vtxcol;
    delta = nidcp->delta;

    if(nidcp->downcounter){
	/* Modulate the delta value and vertex color */
	for(i=0; i<nelem; ++i)
	  mcp[i] += delta[i];
	--nidcp->downcounter;
    }else{
	int key, dc;
	float div;
	u8 *s0, *s1;

	/* Advance the key */
	key = ++nidcp->nowkey;
	if(key >= nidcp->nkey)
	  key=1;
	nidcp->nowkey = key;

	/* Initialize the modulated vertex position and calculate delta values up to the next key  */
	s0 = (u8*)nidcp->rawvtxcol[key-1];
	s1 = (u8*)nidcp->rawvtxcol[key];
	dc = nidcp->key[key] - nidcp->key[key-1];
	div = 256.0/dc;
	for(i=0; i<nelem; ++i){
	    mcp[i] = (s16)s0[i]<<8;
	    delta[i] = (s16)(((float)s1[i]-s0[i])*div);
	}

	/* Assign a new value to downcounter */
	nidcp->downcounter = dc;
    }
}

/* ----------------------------------------------------------------------------
name:
    ndMultiTrx
synopsis:
    void ndMultiTrx(float mf[4][4], TRX *trxp, u32 order);
description:
    Evaluates the TRX structure and creates a 4x4 matrix.
parameters:
    float mf[4][4];
	Buffer that stores the matrix to be created
    TRX *trxp;
	The transform data structure that is the basis of the matrix
    u32 order;
	Evaluation order for the rotational component
returns:
    no return value
notes:
    The generated 4x4 matrix is placed in a buffer in the instance working area and used at the time of display.
---------------------------------------------------------------------------- */
void ndMultiTrx(float mf[4][4], TRX *trxp, u32 order)
{
    /* This speed of this portion can efficiently be increased by adjusting it to suit the contents of the games to be created. */
    ndguTranslateF(mf, trxp->tx, trxp->ty, trxp->tz);

    /* Reflect the rotational elements for each axis. */
    if(trxp->rx!=0.0 || trxp->ry!=0.0 || trxp->rz!=0.0){
	switch(order){
	  case NDC_ROTORDER_XYZ:
	    ndguRotateXYZF( mf, RAD2DEG(trxp->rx),
			   RAD2DEG(trxp->ry), RAD2DEG(trxp->rz) );
	    break;
	  case NDC_ROTORDER_ZYX:
	    ndguRotateZYXF( mf, RAD2DEG(trxp->rx),
			   RAD2DEG(trxp->ry), RAD2DEG(trxp->rz) );
	    break;
	  case NDC_ROTORDER_ZXY:
	    ndguRotateZXYF( mf, RAD2DEG(trxp->rx),
			   RAD2DEG(trxp->ry), RAD2DEG(trxp->rz) );
	    break;
	  default:
	    {
		/* Evaluate rotation in order */
		int i, ir;

		for(i=0; i<3; ++i, order>>=8){
		    ir = order & 0xff;
		    if(ir == 0x01)
		      ndguRotateXF( mf, RAD2DEG(trxp->rx));
		    if(ir == 0x02)
		      ndguRotateYF( mf, RAD2DEG(trxp->ry));
		    if(ir == 0x03)
		      ndguRotateZF( mf, RAD2DEG(trxp->rz));
		}
	    }
	    break;
	}
    }
    ndguScaleF(mf, trxp->sx, trxp->sy, trxp->sz);
}

/* ----------------------------------------------------------------------------
name:
    ndEvalInstanceOne
synopsis:
    int ndEvalInstanceOne(NDD_INSTANCE *isp, float mf[4][4], float cmf[4][4], int forceupd);
description:
    Performs frame evaluation for the instance specified by isp.
parameters:
    NDD_INSTANCE *isp;
	Pointer to the instance
    float mf[4][4];
	Model view matrix
    float cmf[4][4];
	Model view matrix updated by the instance
    int forceupd;
	Flag to force generation of a matrix.  Forced with 1.
returns:
    int;
	Matrix update flag. When the value of the forceudp argument and the function's own matrix is updated,  1 is returned xxas the or valuexx.
notes:
    mf is used as the basic model view matrix. mf is also returned for cmf when there is no matrix evaluation.
---------------------------------------------------------------------------- */
int ndEvalInstanceOne(NDD_INSTANCE *isp, float mf[4][4], float cmf[4][4], int forceupd)
{
    int child_update = 0;

    /* Callback call */
    if(isp->instCB)
      (*isp->instCB)(isp, NDC_ICB_EVAL);

    /* Copy the matrix */
    memcpy(cmf, mf, sizeof(float[4][4]));
    switch(isp->type){
      case NDC_ITYPE_ANIMTK:
	/* Advance time */
	if(!BITTEST1(isp->u.animtk.tarinst->u.obj.flag, NDC_OFLAG_PAUSE_ANIM))
	  ndProgressAnimTime( &isp->u.animtk );
	break;

      case NDC_ITYPE_ANIM:
	/* Animation progress 
	 Note: This instance must also be working when paused with NDC_OFLAG_PAUSE_ANIMATION. */
	ndSetAnimChannel( &isp->u.anim );
	break;

      case NDC_ITYPE_OBJECT:
	if(forceupd || !BITTEST1(isp->flag, NDC_IFLAG_MTX_READY)){
	    /* Object expansion */
	    ndMultiTrx(cmf, &isp->u.obj.trx, isp->u.obj.order);
	    memcpy(isp->u.obj.modelview, cmf, sizeof(float[4][4]));
	    BITSET(isp->flag, NDC_IFLAG_MTX_READY);
	    child_update = 1;
	}
	break;

      case NDC_ITYPE_DYNVTX:
	ndReloadDynamicVertex(&isp->u.dynvtx);
	break;

      case NDC_ITYPE_DEFORM_VTX:
	ndDeformVertex(&isp->u.dfm_vtx);
	break;

      case NDC_ITYPE_DEFORM_VTXNV:
	ndDeformVertexNv(&isp->u.dfm_vtxnv);
	break;

      case NDC_ITYPE_DEFORM_VTXCOL:
	ndDeformVertexColor(&isp->u.dfm_vtxcol);
	break;

      case NDC_ITYPE_NULL:
      case NDC_ITYPE_ROOT:
	child_update = forceupd;
	break;
    }

    return child_update;
}

/* ----------------------------------------------------------------------------
name:
    ndEvalInstance
synopsis:
    void ndEvalInstance(NDD_INSTANCE *isp, float mf[4][4], int forceupd);
description:
    Performs frame evaluation for all instances under the instance specified by isp.
parameters:
    NDD_INSTANCE *isp;
	Pointer to the instance.
    float mf[4][4];
	Model view matrix.
    int forceupd;
	Flag for forced generation of a matrix.  Forced with 1.
returns:
notes:
    mf is used as the basic model view matrix. 
    With recursion, a 64-byte buffer is prepared, consuming a good portion of the stack.
---------------------------------------------------------------------------- */
void ndEvalInstance(NDD_INSTANCE *isp, float mf[4][4], int forceupd)
{
    NDD_INSTANCE *bros;
    float cmf[4][4];

    /* Self-expansion */
    forceupd |= ndEvalInstanceOne(isp, mf, cmf, forceupd);

    /* Expand the child link */
    if(isp->nchild)
      for(bros=isp->child; bros; bros=bros->bros)
	ndEvalInstance(bros, cmf, forceupd);
}

