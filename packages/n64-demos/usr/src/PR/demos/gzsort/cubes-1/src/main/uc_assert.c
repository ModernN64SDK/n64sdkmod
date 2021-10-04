/*---------------------------------------------------------------------*
	Copyright (C) 1997, Nintendo.
	
	File		uc_assert.c
	Coded    by	Yoshitaka Yasumoto.	Sep 12, 1997.
	Modified by	
	
	$Id: uc_assert.c,v 1.1 1997/09/12 07:03:27 yasu Exp $
 *---------------------------------------------------------------------*/
#include	<ultra64.h>

/*==========================================================================*
 *  Check ucode Assertion
 *==========================================================================*/
u32	ucCheckAssert(void)
{
  /*
   * S2DEX ucode ‚ªˆÙíI—¹‚µ‚½‚Æ‚«‚Í SP_STATUS_SIG7 ‚ª—§‚Â
   */
  return (IO_READ(SP_STATUS_REG) & SP_STATUS_SIG7);
}


/*==========================================================================*
 *  Table for ucode Decoding
 *==========================================================================*/
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

/*==========================================================================*
 *  DL Log Output
 *     Outputs log of commands RSP processed
 *==========================================================================*/
void	ucDebugGfxLogPrint(OSTask *tp)
{
  int	n, gfx, cmd;
  u32	logtop = (u32)(tp->t.data_size);
  u32	logend;
  
  if (NULL == logtop) return;
  
  osSyncPrintf("----------------------------------------RSP DL Log\n");
  
  /*------ Output Position of the current log is stored in this postion ------*/
  logend = IO_READ(SP_DMEM_START+0xff4);
  
  /*------ Decode log in order ------*/
  for (n = logtop; n < logend; n += 8){
    gfx = IO_READ(n+4);
    cmd = (u8)(((u8)(gfx>>24)) - (u8)0xaf) + 1;
    if (cmd < 1 || 0x106-0xaf+1 < cmd) cmd = 0;
    if (decode_table[cmd] == 0) cmd = 0;
    osSyncPrintf("%08x: %08x %s\n", IO_READ(n), gfx, decode_table[cmd]);
  }
  return;
}

/*==========================================================================*
 *  RDP Command Output
 *==========================================================================*/
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
  u32	n, cmd0, cmd1;
  u32	fifotop = (u32)(tp->t.output_buff);
  u32	fifoend = (u32)(tp->t.output_buff_size);
  int	count = 0;
  
  osSyncPrintf("---------------------------------------- RDP FIFO\n");
  for (n = fifotop; n < fifoend; n += 8){
    cmd0 = IO_READ(n+0);
    cmd1 = IO_READ(n+4);
    
    if (count > 0){
      count -= 8;
    } else {      
      head = (cmd0 >> 24) & 0x3f;
      if (decode_rdp[head] == 0) head = 1;	/* Undefined         */
      if (8 <= head && head <= 15){			/* Triangle Command  */
	count = count_rdp[head-8] - 8;
      } else if (head == 0x24 || head == 0x25){	/* TextureRectangle Command */
	count = 8;
      }
    }
    osSyncPrintf("%08x: %08x %08x %s\n", n, cmd0, cmd1, decode_rdp[head]);
    if (head == 0x29) break;
  }
}

/*==========================================================================*
 *  Output Area for Debug
 *==========================================================================*/
void	ucDebugInfoPrint(void)
{
  osSyncPrintf("Debug = (%08x %08x %08x %08x)\n",
	       IO_READ(SP_DMEM_START+0xfb0), IO_READ(SP_DMEM_START+0xfb4),
	       IO_READ(SP_DMEM_START+0xfb8), IO_READ(SP_DMEM_START+0xfbc));
}

/*==========================================================================*
 *  Output Assert info data
 *==========================================================================*/
void	ucDebugAssertPrint(void)
{
  osSyncPrintf("-------- Assert!! --------\nPC=%08x inp=%08x gfx0=0x%08x\n",
	       IO_READ(SP_DMEM_START+0xfa8), IO_READ(SP_DMEM_START+0xfa0),
	       IO_READ(SP_DMEM_START+0xfa4));
}

/*==========================================================================*
 *  IMEM Output
 *==========================================================================*/
void	ucDebugIMEMPrint(void)
{
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
}

/*==========================================================================*
 *  DMEM Output
 *==========================================================================*/
void	ucDebugDMEMPrint(void)
{
  u32	i;
  u32	p0, p1, p2, p3;
  
  osSyncPrintf("----------------------------------------DMEM\n");
  for (i = 0; i < 1024; i += 4){
    p0 = IO_READ(SP_DMEM_START+(i+0)*4);
    p1 = IO_READ(SP_DMEM_START+(i+1)*4);
    p2 = IO_READ(SP_DMEM_START+(i+2)*4);
    p3 = IO_READ(SP_DMEM_START+(i+3)*4);
    osSyncPrintf("%04x : %08x %08x %08x %08x\n", i*4, p0, p1, p2, p3);
  }
}

/*======== End of uc_assert.c ========*/
