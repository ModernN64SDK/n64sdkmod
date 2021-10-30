/*
   stage00.c 

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.			
*/

#include <assert.h>
#include <nusys.h>
#include "main.h"
#include "graphic.h"

static f32 lodPos_x; /* Display position X */
static f32 lodPos_y; /* Display position Y */
static f32 lodPos_z; /* Display position Z */
static f32 lodRot_x; /* Rotating angle for X axis */
static f32 lodRot_y; /* Rotating angle for Y axis */
static f32 lodRot_z; /* Rotating angle for Z axis */
static f32 lodSize;  /* Display size */

static u16 perspNorm;
static LookAt lookat;

static u32 pause;	/* Flag that controls automatic shift */
static u32 near_plane;	/* Near Plane */
static u32 far_plane;	/* Far Plane */
#ifdef F3DEX_GBI_2
static u8 ucode_name[][16] = {
    "F3DEX2",
    "F3DEX2.NoN",
    "F3DEX2_Rej",
    "F3DLX2_Rej",
    {NULL},
};
#else
static u8 ucode_name[][16] = {
    "F3DEX",
    "F3DEX.NoN",
    "F3DLX",
    "F3DLX.NoN",
    "F3DLX.Rej",
    {NULL},
};
#endif
static s32 ucode_number;

void shade_lodmodel(Dynamic* dynamicp);

/* Initialize stage 0 */
void initStage00(void)
{
    /* Initialize parameters */
    lodPos_x = 0.0;
    lodPos_y = 0.0;
    lodPos_z = 400.0;
    lodSize  = 2.0;
    lodRot_x = 0.0;
    lodRot_y = 0.0;
    lodRot_z = 0.0;

    pause = 0;
    near_plane = 10;
    far_plane = 2000;
    ucode_number = 0;

    /* Read display list of LOD model */
    nuPiReadRom((u32)_lodmodelSegmentRomStart, gfx_dl_buf,
		(u32)_lodmodelSegmentRomEnd - (u32)_lodmodelSegmentRomStart);
}

/* Create display list and launch task */
void makeDL00(void)
{
  Dynamic* dynamicp;
  u8 conbuf[20]; 
  Mtx rotate_x;
  Mtx rotate_y; 
  Mtx rotate_z; 

  /* Specify display list buffer */
  dynamicp = &gfx_dynamic[gfx_gtask_no];
  glistp = &gfx_glist[gfx_gtask_no][0];

  /*  Initialize RCP */
  gfxRCPInit();

  /* Clear frame buffer and Z buffer */
  gfxClearCfb();

  /* projection, viewing, modeling matrix set */
  guPerspective(&dynamicp->projection, &perspNorm,
		30,
		(f32)SCREEN_WD/(f32)SCREEN_HT,
		near_plane, far_plane, 1.0);
  guLookAtReflect(&dynamicp->viewing, &lookat,
		  0, 0, 600,
		  0, 0, 0,
		  0, 1, 0);

  guTranslate(&dynamicp->translate, lodPos_x, lodPos_y, lodPos_z);

  guScale(&dynamicp->scaling, lodSize, lodSize, lodSize);

  guRotate(&rotate_x, lodRot_x, 1.0F, 0.0F, 0.0F);
  guRotate(&rotate_y, lodRot_y, 0.0F, 1.0F, 0.0F);
  guRotate(&rotate_z, lodRot_z, 0.0F, 0.0F, 1.0F);
  guMtxCatL(&rotate_x, &rotate_y, &dynamicp->modeling);
  guMtxCatL(&rotate_z, &dynamicp->modeling, &dynamicp->modeling);

  /* Draw LOD model */
  shade_lodmodel(dynamicp);

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  assert((glistp - gfx_glist[gfx_gtask_no]) < GFX_GLIST_LEN);

  /* Launch task but do not change the display buffer */
  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0],
		 (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx),
		 ucode_number , NU_SC_NOSWAPBUFFER);

  /* Debug console */
  nuDebConClear(NU_DEB_CON_WINDOW0);
  if(pause)
  {
      nuDebConTextPos(NU_DEB_CON_WINDOW0, 17, 19);
      nuDebConTextAttr(NU_DEB_CON_WINDOW0, NU_DEB_CON_ATTR_BLINK);
      nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_RED);
      nuDebConCPuts(NU_DEB_CON_WINDOW0, "Pause");
      nuDebConTextAttr(NU_DEB_CON_WINDOW0, NU_DEB_CON_ATTR_NORMAL);
      nuDebConTextColor(NU_DEB_CON_WINDOW0, NU_DEB_CON_TEXT_WHITE);
  }  

  nuDebConTextPos(NU_DEB_CON_WINDOW0, 8, 20);
  sprintf(conbuf, "UCODE Name = %s", ucode_name[ucode_number]);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, conbuf);

  nuDebConTextPos(NU_DEB_CON_WINDOW0, 8, 21);
  sprintf(conbuf, "Near Plane = %5ld", (s32)near_plane);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, conbuf);
  nuDebConTextPos(NU_DEB_CON_WINDOW0, 9, 22);
  sprintf(conbuf, "Far Plane = %5ld", (s32)far_plane);
  nuDebConCPuts(NU_DEB_CON_WINDOW0, conbuf);

  nuDebConTextPos(NU_DEB_CON_WINDOW0, 13, 23);
  sprintf(conbuf, "Depth = %5ld", (s32)(600.0-lodPos_z));
  nuDebConCPuts(NU_DEB_CON_WINDOW0, conbuf);
  nuDebConDisp(NU_SC_NOSWAPBUFFER);

  /* Performance meter */
  nuDebTaskPerfBar0(1, 200, NU_SC_SWAPBUFFER);

  /* Change display list buffer */
  gfx_gtask_no ^= 1;
}


/* Process the game status for stage0 */
void updateGame00(void)
{  

  /* Read data of the controller 1 */
  nuContDataGetEx(contdata,0);

  /* << Pause process >> */
  if(contdata[0].trigger & START_BUTTON)
      pause ^= 1;

  /* << Change microcode >> */
  /* A or B button to change */
  if(contdata[0].trigger & A_BUTTON)
      ucode_number--;
  if(contdata[0].trigger & B_BUTTON)
      ucode_number++;

  /* Adjust the range */
  if(ucode_number < 0)
      ucode_number = 0;
  if(ucode_name[ucode_number][0] == NULL)
      ucode_number--;

  /* << Near Plane >> */

  /* Increase/decrease with down and left buttons of C button unit */
  if(contdata[0].button & D_CBUTTONS)
      near_plane -= 1.0;
  if(contdata[0].button & L_CBUTTONS)
      near_plane += 1.0;
  /* Adjust the valid range  */
  if(near_plane < 1.0)
      near_plane = 1.0;
  if(near_plane > far_plane)
      near_plane = far_plane;


  /* << Far Plane >> */

  /* Increase/decrease with right and up buttons of C button unit */
  if(contdata[0].button & R_CBUTTONS)
      far_plane -= 1.0;
  if(contdata[0].button & U_CBUTTONS)
      far_plane += 1.0;
  /* Adjust the valid range */
  if(far_plane < near_plane)
      far_plane = near_plane;
  if(far_plane > 30000.0)
      far_plane = 30000.0;


  /* << Z axis shift process >> */

  /* An object moves towards far by default */
  if(!pause)
      lodPos_z -= 5.0;

  /* Change the moving speed with up/down buttons of controller */
  if(contdata[0].button & U_JPAD)
      lodPos_z += 10.0;

  if(contdata[0].button & D_JPAD)
      lodPos_z -= 10.0;

  /* It comes back near if it goes too far */
  if(lodPos_z < (600.0-far_plane))
      lodPos_z = 600.0-near_plane;

  /* It goes back far if it comes too near */
  else if(lodPos_z > (600.0-near_plane))
      lodPos_z = 600.0-far_plane;


  /* << XY axis shift process >> */

  /* Move left/right with left/right buttons of controller */
  if(contdata[0].button & L_JPAD)
      lodPos_x -= 1.0;

  if(contdata[0].button & R_JPAD)
      lodPos_x += 1.0;


  /* << rotating process >> */

  /* Give slight rotating by default */
  if(!pause)
  {
      lodRot_x += 1.0;
      lodRot_y += 1.0;
      lodRot_z += 0.1;
  }

  /* Adjust play of stick */
  if( contdata[0].stick_x > -5 && contdata[0].stick_x < 5 )
      contdata[0].stick_x = 0;
  if( contdata[0].stick_y > -5 && contdata[0].stick_y < 5 )
      contdata[0].stick_y = 0;

  /* 3D stick changes the rotating speed */
  lodRot_x += (f32)contdata[0].stick_y/10.0;
  lodRot_y += (f32)contdata[0].stick_x/10.0;

  /* Adjust angle range */
  if(lodRot_x<0.0)
      lodRot_x+=360.0;
  else if(lodRot_x>360.0)
      lodRot_x-=360.0;
  if(lodRot_y<0.0)
      lodRot_y+=360.0;
  else if(lodRot_y>360.0)
      lodRot_y-=360.0;
  if(lodRot_z<0.0)
      lodRot_z+=360.0;
  else if(lodRot_z>360.0)
      lodRot_z-=360.0;
}

/* Determining LOD vertex */
static Vtx testvtx = {
  { 0,    0,    0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
};

/* Display list of LOD model itself */
/* If the parameters for gSPBranchLessZ are fixed,   */
/* then gsSPBranchLessZ can be used. In that case,   */
/* it's not necessary to prepare another display list*/
static void lodmodel_dl(void)
{
    /* Specify display list buffer */
    lod_gtask_no ^= 1;
    lodlistp = &lod_glist[lod_gtask_no][0];

    gSPVertex(lodlistp++, &testvtx, 1, 0);
    gSPBranchLessZ(lodlistp++, near__dl,   0,  500, near_plane, far_plane, G_BZ_PERSP);
    gSPBranchLessZ(lodlistp++, middle__dl, 0, 1000, near_plane, far_plane, G_BZ_PERSP);
    gSPBranchLessZ(lodlistp++, far__dl,    0, far_plane, near_plane, far_plane, G_BZ_PERSP);
    gSPEndDisplayList(lodlistp++);

    /* Write back in cache */
    osWritebackDCache(&lod_glist[lod_gtask_no][0],
		      (s32)(lodlistp-lod_glist[lod_gtask_no]) * sizeof (Gfx));
}

/* Draw LOD model */
void shade_lodmodel(Dynamic* dynamicp)
{
  gSPPerspNormalize(glistp++, perspNorm);
  gSPLookAt(glistp++, &lookat);

  gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->projection)),
	    G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);

  gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->viewing)),
	    G_MTX_PROJECTION|G_MTX_MUL|G_MTX_NOPUSH);

  gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->translate)),
	    G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);

  gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->modeling)),
	    G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);

  gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->scaling)),
	    G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);

  /* Create display list of LOD model */
  lodmodel_dl();

  gSPSegment(glistp++, STATIC_SEGMENT, OS_K0_TO_PHYSICAL(&gfx_dl_buf));

  gSPDisplayList(glistp++, &lod_glist[lod_gtask_no][0]);
}
