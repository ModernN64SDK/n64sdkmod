/*---------------------------------------------------------------------*
	Copyright (C) 1997, Nintendo.
	
	File		main.c
	Coded    by	Yoshitaka Yasumoto.	Sep 12, 1997.
	Modified by	
	
	$Id: main.c,v 1.2 1997/11/25 07:41:39 yasu Exp $
 *---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	<PR/gzsort.h>
#include	"system.h"

extern	Vp		viewport;
extern	Gfx		clearCfb[];
extern	Gfx		gfxFinish[];
extern	Gfx     	rdpcmdShPoly[];
extern	zVtxSrc		quadVtxSrc[];
extern	zColor  	quadColor[];

/*---------------------------------------------------------------------------*
 * Define Task Data
 *---------------------------------------------------------------------------*/
OSTask	tlist = {
  M_GFXTASK,					/* task type                */
  OS_TASK_LOADABLE,				/* task flags               */
  (u64 *)&rspbootTextStart,			/* boot ucode ptr           */
  SP_BOOT_UCODE_SIZE,				/* boot ucode size          */
  (u64 *)&gspZSort_fifoTextStart,		/* ucode ptr                */
  SP_UCODE_SIZE,				/* ucode size               */
  (u64 *)&gspZSort_fifoDataStart,		/* ucode data ptr           */
  SP_UCODE_DATA_SIZE,				/* ucode data size          */
  NULL, 					/* dram stack      (not used) */
  0,						/* dram stack size (not used) */
  (u64 *)system_rdpfifo,			/* fifo buffer top          */
  (u64 *)system_rdpfifo+RDPFIFO_SIZE,		/* fifo buffer bottom       */
  NULL,						/* data ptr      (set later) */
  NULL, 					/* data size     (doesn't need to be set)*/
  (u64 *)system_rspyield,			/* yield data ptr           */
  OS_YIELD_DATA_SIZE,				/* yield data size          */
};

typedef	struct	{
  Mtx		projection;
  Mtx		viewing;
  Mtx		modeling;
} Dynamic;

#define	VTX_NUM		4
#define	UMEM_VSRC	0
#define	UMEM_VDEST	1024

Dynamic		dynamic;
Gfx		glist[GLIST_LEN];
zVtxDest	quadVtxDest[VTX_NUM];
zShQuad		quadZobj;

/*---------------------------------------------------------------------------*
 * M A I N 
 *---------------------------------------------------------------------------*/
void	Main(void *arg)
{
  u8	draw_frame = 0;
  u16	perspNorm;
  Gfx	*gp;
  u32	i;
  float	deg = 0.0;
  
  while(1){
    /*------ Retrace Stand-by ------*/
    osRecvMesg(&retraceMessageQ, NULL, OS_MESG_NOBLOCK);
    osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);

    /*=======================================================================*
     *    Z S o r t  P a s s  1
     *=======================================================================*/
    /*------ Create Gfx List ------*/
    gp = glist;
    
    /*------ Set Perspective Array ------*/
    guPerspective(&dynamic.projection, &perspNorm, 33,
		  (float)SCREEN_WD/(float)SCREEN_HT,
		  (float)CLIP_NEAR, (float)CLIP_FAR, 1.0);
    guLookAt(&dynamic.viewing, 0, 0, 200,  0, 0, 0,  0, 1, 0);

    /*------ Set Model Array ------*/
    guRotate(&dynamic.modeling, deg, 0.2, 1.0, 0.7);
    
    /*--- WriteBack Dcache ---*/
    osWritebackDCache(&dynamic, sizeof(Dynamic));
    
    /*--- Set ViewPort Projection Viewing ---*/
    gSPZViewport(gp ++, &viewport);
    gSPZPerspNormalize(gp ++, perspNorm);  
    gSPZSetMtx(gp ++, GZM_MMTX, &dynamic.projection);
    gSPZSetMtx(gp ++, GZM_PMTX, &dynamic.viewing);
    gSPZMtxCat(gp ++, GZM_PMTX, GZM_MMTX, GZM_PMTX);
    
    /*--- Set modelview matrix and make MP matrix ---*/
    gSPZSetMtx(gp ++, GZM_MMTX, &dynamic.modeling);
    gSPZMtxCat(gp ++, GZM_MMTX, GZM_PMTX, GZM_MPMTX);
    
    /*--- Vertex calculation ---*/
    gSPZSetUMem(  gp ++, UMEM_VSRC,  VTX_NUM*ZSIZE_VSRC,  quadVtxSrc);
    gSPZMultMPMtx(gp ++, GZM_MPMTX,  UMEM_VSRC, VTX_NUM,  UMEM_VDEST);
    gSPZGetUMem(  gp ++, UMEM_VDEST, VTX_NUM*ZSIZE_VDEST, quadVtxDest);
    osInvalDCache(quadVtxDest, VTX_NUM*ZSIZE_VDEST);
    
    /*--- End of DL ---*/
    gSPZEndDisplayList(gp ++);
    
    /*------ Do Gfx task ------*/
    tlist.t.data_ptr = (u64 *)glist;
    osWritebackDCache(glist, ((u32)gp)-((u32)glist));
    osSpTaskStart(&tlist);
    
    /*------ Wait till the task done ------*/
    osRecvMesg(&rspMessageQ, NULL, OS_MESG_BLOCK);
    
    /*=======================================================================*
     *    Z S o r t  P a s s  2
     *=======================================================================*/
    
    /*------ Initialize segment/cfb ------*/
    gp = glist;
    gSPZSegment(gp ++, CFB_SEGMENT, system_cfb[draw_frame]);
    gSPZRdpCmd(gp ++, clearCfb);
    
    /*------ Make Gfx list ------*/    
    for (i = 0; i < 4; i ++){
      quadZobj.t.v[i].x = quadVtxDest[i].sx;
      quadZobj.t.v[i].y = quadVtxDest[i].sy;
      quadZobj.t.v[i].r = quadColor[i].n.r;
      quadZobj.t.v[i].b = quadColor[i].n.b;
      quadZobj.t.v[i].g = quadColor[i].n.g;
      quadZobj.t.v[i].a = quadColor[i].n.a;
    }
    quadZobj.t.header  = (zHeader *)G_ZOBJ_NONE;
    quadZobj.t.rdpcmd1 = rdpcmdShPoly;
    osWritebackDCache(&quadZobj, sizeof(quadZobj));
    gSPZObject(gp ++, ZHDR(&quadZobj, ZH_SHQUAD), (zHeader *)G_ZOBJ_NONE);
    
    /*------ Make it finished ------*/
    gSPZRdpCmd(gp ++, gfxFinish);
    gSPZEndDisplayList(gp ++);
    
    /*------ Do Gfx task ------*/
    tlist.t.data_ptr = (u64 *)glist;
    osWritebackDCache(glist, ((u32)gp)-((u32)glist));
    osSpTaskStart(&tlist);

    /*------ Wait till the task done ------*/
    osRecvMesg(&rspMessageQ, NULL, OS_MESG_BLOCK);
    osRecvMesg(&rdpMessageQ, NULL, OS_MESG_BLOCK);
    
    /*------ Swap color frame buffer ------*/
    osViSwapBuffer(system_cfb[draw_frame]);
    draw_frame ^= 1;

    /*------ Rotate quadrangle ------*/
    deg += 1.0;
    if (deg >= 360.0) deg = 0.0;
  }
}

/*======== End of main.c ========*/
