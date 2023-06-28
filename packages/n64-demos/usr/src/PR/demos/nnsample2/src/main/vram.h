/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 2

		Copyright (C) 1997, NINTENDO Co,Ltd.

============================================================================*/
#ifndef VRAM_H
#define VRAM_H

extern u16 cfb[2][SCREEN_WD*SCREEN_HT];              /* color frame buffer */
extern u64 rdp_output[GFX_RDP_OUTPUT_SIZE];  /* RSP writes back RDP data */
extern u64 dram_stack[];         /* used for matrix stack */
extern unsigned short zbuffer[SCREEN_WD*SCREEN_HT]; /* Z-buffer */
extern u64 gfxYieldBuf[];
#endif /* VRAM_H */
