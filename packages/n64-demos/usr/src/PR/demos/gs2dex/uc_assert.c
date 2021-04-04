/*---------------------------------------------------------------------
	Copyright (C) 1997, Nintendo.
	
	File		uc_assert.c
	Coded    by	Yoshitaka Yasumoto.	Feb 19, 1997.
	Modified by	
	Comments	
	
	$Id: uc_assert.c,v 1.7 1998/10/01 09:56:39 otsu Exp $
  --------------------------------------------------------------------*/
#include	<ultra64.h>

/*=====================================================================*
 *      ucode assert check
 *====================================================================*/
u32	ucCheckAssert(void)
{
  /*
   * When S2DEX ucode ends abnormally, SP_STATUS_SIG7 rises
   */
  return (IO_READ(SP_STATUS_REG) & SP_STATUS_SIG7);
}


/*=====================================================================*
 *  Table for ucode decoding
 *====================================================================*/
static char	*decode_table[] = {
  "Undefined", "G_SWITCH_UCODE",
  0, "G_OBJ_RENDERMODE", "G_OBJ_RECTANGLE_R", "G_TEXRECT_DONE",
  "G_RDPHALF_1", 0, 0, 0,
  "G_ENDDL",	"G_SETOTHERMODE_L",	"G_SETOTHERMODE_H", 	0,
  "G_MOVEWORD",	"G_SPRITE2D_DRAW", 	"G_SPRITE2D_SCALEFLIP", 0,
  "G_NOOP", "G_OBJ_LOADTMEM", "G_OBJ_LDTM_SPRITE", "G_OBJ_LDTM_RECT",
  "G_OBJ_LDTM_RECT_R",0,0,0,
  0,0,0,0, 0,0,0,0,
  0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
  0,0,0,0,
  "G_TEXRECT", "G_TEXRECTFLIP", "G_RDPLOADSYNC", "G_RDPPIPESYNC",
  "G_RDPTILESYNC", "G_RDPFULLSYNC", "G_SETKEYGB", "G_SETKEYR",
  "G_SETCONVERT", "G_SETSCISSOR", "G_SETPRIMDEPTH", "G_RDPSETOTHERMODE",
  "G_LOADTLUT", 0, "G_SETTILESIZE", "G_LOADBLOCK",
  "G_LOADTILE", "G_SETTILE", "G_FILLRECT", "G_SETFILLCOLOR",
  "G_SETFOGCOLOR", "G_SETBLENDCOLOR", "G_SETPRIMCOLOR", "G_SETENVCOLOR",
  "G_SETCOMBINE", "G_SETTIMG", "G_SETZIMG", "G_SETCIMG",
  "G_NOP", "G_BG_RECT1CYC", "G_BG_RECTCOPY","G_OBJ_RECTANGLE",
  "G_OBJ_SPRITE", "G_OBJ_MOVEMEM", "G_DL"
};

/*=====================================================================*
 *  DL log output
 *     Outputs log of commands processed by the RSP
 *====================================================================*/
void	ucDebugGfxLogPrint(OSTask *tp)
{
  int	n, gfx, cmd;
  u32	logtop = (u32)(tp->t.data_size);
  u32	logend;
  
  if (NULL == logtop) return;

#ifdef DEBUG  
  osSyncPrintf("----------------------------------------RSP DL Log\n");
#endif
  
  /*--- The current location of the log output is stored in this location ---*/
  logend = IO_READ(SP_DMEM_START+0xff4);
  
  /*------ Decode log in order ------*/
  for (n = logtop; n < logend; n += 8){
    gfx = IO_READ(n+4);
    cmd = (u8)(((u8)(gfx>>24)) - (u8)0xaf) + 1;
    if (cmd < 1 || 0x106-0xaf+1 < cmd) cmd = 0;
    if (decode_table[cmd] == 0) cmd = 0;

#ifdef DEBUG
    osSyncPrintf("%08x: %08x %s\n", IO_READ(n), gfx, decode_table[cmd]);
#endif

  }
  return;
}

/*=====================================================================*
 *  Output RDP commands
 *====================================================================*/
static	char	*decode_rdp[] = {
  "Noop",          "Undefined",       0,                0,
  0,               0,                 0,                0,
  "Tri",           "ZB-Tri",          "TX-Tri",         "TX-ZB-Tri",
  "Shade-Tri",     "Shade-ZB-Tri",    "Shade-TX-Tri",   "Shade-TX-ZB-Tri",
  0,               0,                 0,                0,
  0,               0,                 0,                0,
  0,               0,                 0,                0,
  0,               0,                 0,                0,

  0,               0,                 0,                0,
  "G_TEXRECT",     "G_TEXRECTFLIP",   "G_RDPLOADSYNC",  "G_RDPPIPESYNC",
  "G_RDPTILESYNC", "G_RDPFULLSYNC",   "G_SETKEYGB",     "G_SETKEYR",
  "G_SETCONVERT",  "G_SETSCISSOR",    "G_SETPRIMDEPTH", "G_RDPSETOTHERMODE",

  "G_LOADTLUT",    0,                 "G_SETTILESIZE",  "G_LOADBLOCK",
  "G_LOADTILE",    "G_SETTILE",       "G_FILLRECT",     "G_SETFILLCOLOR",
  "G_SETFOGCOLOR", "G_SETBLENDCOLOR", "G_SETPRIMCOLOR", "G_SETENVCOLOR",
  "G_SETCOMBINE",  "G_SETTIMG",       "G_SETZIMG",      "G_SETCIMG",
};

static	int	count_rdp[] = {
  32, 32+16, 32+64, 32+64+16, 32+64, 32+64+16, 32+64+64, 32+64+64+16
};

void	ucDebugRdpFifoPrint(OSTask *tp)
{
  u8	head;
  u32	n, cmd0;
  u32	fifotop = (u32)(tp->t.output_buff);
  u32	fifoend = (u32)(tp->t.output_buff_size);
  int	count = 0;

#ifdef DEBUG
  u32	cmd1;
#endif

#ifdef DEBUG  
  osSyncPrintf("---------------------------------------- RDP FIFO\n");
#endif

  for (n = fifotop; n < fifoend; n += 8){
    cmd0 = IO_READ(n+0);

#ifdef DEBUG
    cmd1 = IO_READ(n+4);
#endif
    
    if (count > 0){
      count -= 8;
    } else {      
      head = (cmd0 >> 24) & 0x3f;
      if (decode_rdp[head] == 0) head = 1;	/* Undefined         */
      if (8 <= head && head <= 15){		/* Triangle command */
	count = count_rdp[head-8] - 8;
      } else if (head == 0x24 || head == 0x25){	/* TextureRectangle command */
	count = 8;
      }
    }

#ifdef DEBUG
    osSyncPrintf("%08x: %08x %08x %s\n", n, cmd0, cmd1, decode_rdp[head]);
#endif

    if (head == 0x29) break;
  }
}

/*=====================================================================*
 *  Output Debug area
 *====================================================================*/
void	ucDebugInfoPrint(void)
{
#ifdef DEBUG
  osSyncPrintf("Debug = (%08x %08x %08x %08x %08x)\n",
	       IO_READ(SP_DMEM_START+0xfac), IO_READ(SP_DMEM_START+0xfb0),
	       IO_READ(SP_DMEM_START+0xfb4), IO_READ(SP_DMEM_START+0xfb8),
	       IO_READ(SP_DMEM_START+0xfbc));
#endif
}

/*=====================================================================*
 * Output Assert information data 
 *====================================================================*/
void	ucDebugAssertPrint(void)
{
#ifdef DEBUG
  osSyncPrintf("-------- Assert!! --------\nPC=%08x inp=%08x gfx0=0x%08x\n",
	       IO_READ(SP_DMEM_START+0xfa8), IO_READ(SP_DMEM_START+0xfa0),
	       IO_READ(SP_DMEM_START+0xfa4));
#endif
}

/*=====================================================================*
 *  Output IMEM
 *====================================================================*/
void	ucDebugIMEMPrint(void)
{
#ifdef DEBUG
  u32	i;
  u32	p0, p1, p2, p3;

  osSyncPrintf("----------------------------------------IMEM\n");

  for (i = 0; i < 1024; i += 4){
    p0 = IO_READ(SP_IMEM_START+(i+0)*4);
    p1 = IO_READ(SP_IMEM_START+(i+1)*4);
    p2 = IO_READ(SP_IMEM_START+(i+2)*4);
    p3 = IO_READ(SP_IMEM_START+(i+3)*4);

    osSyncPrintf("%04x : %08x %08x %08x %08x\n", i*4, p0, p1, p2, p3);
  }
#endif

}

/*=====================================================================*
 *  Output DMEM
 *====================================================================*/
void	ucDebugDMEMPrint(void)
{
#ifdef DEBUG
  u32	i;
  u32   p0, p1, p2, p3;

  osSyncPrintf("----------------------------------------DMEM\n");

  for (i = 0; i < 1024; i += 4){
    p0 = IO_READ(SP_DMEM_START+(i+0)*4);
    p1 = IO_READ(SP_DMEM_START+(i+1)*4);
    p2 = IO_READ(SP_DMEM_START+(i+2)*4);
    p3 = IO_READ(SP_DMEM_START+(i+3)*4);

    osSyncPrintf("%04x : %08x %08x %08x %08x\n", i*4, p0, p1, p2, p3);
  }
#endif

}

/*======== End of uc_assert.c ========*/

