/*============================================================================
  vram.h
  ============================================================================*/
#ifndef VRAM_H
#define VRAM_H

extern u16 cfb[2][SCREEN_WD*SCREEN_HT];              /* color frame buffer */
extern u64 rdp_output[4096*16];  /* RSP writes back RDP data */
extern u64 rdp_output_len;       /* RSP writes back size of RDP data */
extern u16 rsp_cfb[];            /* RSP view of frame buffer. */
extern u64 dram_stack[];         /* used for matrix stack */
extern unsigned short zbuffer[SCREEN_WD*SCREEN_HT]; /* Z-buffer */
extern u64 gfxYieldBuf[];
#endif /* VRAM_H */
