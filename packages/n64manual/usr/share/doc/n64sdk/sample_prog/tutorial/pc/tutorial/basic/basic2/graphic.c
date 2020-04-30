/*******************************************************************
 *
 *  graphic.c
 *
 *******************************************************************/

#include  <nusys.h>

#include  "define.h"

/* Viewport information */
static  Vp  viewport = {
  SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0,
  SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0,
};

/* RSP initialization display list */
Gfx rspinit_dl[] = {
  gsSPViewport(&viewport),
  gsSPClearGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BOTH |
    G_FOG | G_LIGHTING | G_TEXTURE_GEN |
    G_TEXTURE_GEN_LINEAR | G_LOD),
  gsSPTexture(0, 0, 0, 0, G_OFF),
  gsSPEndDisplayList(),
};

/* RDP initialization display list */
Gfx rdpinit_dl[] = {
  gsDPSetCycleType(G_CYC_1CYCLE),
  gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),
  gsDPSetCombineKey(G_CK_NONE),
  gsDPSetAlphaCompare(G_AC_NONE),
  gsDPSetRenderMode(G_RM_NOOP, G_RM_NOOP2),
  gsDPSetColorDither(G_CD_DISABLE),
  gsDPPipeSync(),
  gsSPEndDisplayList(),
};

/* Display list */
Gfx           glist[GLIST_LENGTH];
Gfx *         glistp;

/* Function responsible for drawing */
void  Draw(void)
{
  int                     i, j;
  static unsigned char    col = 0;
  
  /* Draw directly into frame buffer */
  for(j = 0; j < SCREEN_WD * SCREEN_HT; j++)
    nuGfxCfb_ptr[j] = GPACK_RGBA5551(col, 2 * col, 3 * col, 1);
  col++;
  
  glistp = glist;
  
  /* Initialize RCP */
  gSPSegment(glistp++, 0, 0);
  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(rspinit_dl));
  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(rdpinit_dl));
  
  /* End display list creation */
  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);
  
  /* Start task */
  nuGfxTaskStart(glist,
                 (s32)(glistp - glist) * sizeof(Gfx),
                 NU_GFX_UCODE_F3DEX, NU_SC_SWAPBUFFER);
}
