/*============================================================================
  NuSYSTEM sample program [SNAKE TAIL HACK]
  
  font.c

  Copyright (C) 1997, NINTENDO Co,Ltd.
  ============================================================================*/

#include <PR/mbi.h>
#include <PR/os.h>
#include "graphic.h"
#include "font.h"
#include "fontimg.h"

/*The buffer for the font output */
char 	outstring[100];

/*The texture pallet */
u16  default_tlut[16] = {
    GPACK_RGBA5551(   0,   0,   0,    0),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255)
};

u16  white_tlut[16] = {
    GPACK_RGBA5551(   0,   0,   0,    0),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255),
    GPACK_RGBA5551( 255, 255, 255,  255)
};

u16  black_tlut[16] = {
    GPACK_RGBA5551(   0,   0,   0,    0),
    GPACK_RGBA5551(   0,   0,   0,  255),
    GPACK_RGBA5551(   0,   0,   0,  255),
    GPACK_RGBA5551(   0,   0,   0,  255),
    GPACK_RGBA5551(   0,   0,   0,  255),
    GPACK_RGBA5551(   0,   0,   0,  255),
    GPACK_RGBA5551(   0,   0,   0,  255),
    GPACK_RGBA5551(   0,   0,   0,  255),
    GPACK_RGBA5551(   0,   0,   0,  255),
    GPACK_RGBA5551(   0,   0,   0,  255),
    GPACK_RGBA5551(   0,   0,   0,  255),
    GPACK_RGBA5551(   0,   0,   0,  255),
    GPACK_RGBA5551(   0,   0,   0,  255),
    GPACK_RGBA5551(   0,   0,   0,  255),
    GPACK_RGBA5551(   0,   0,   0,  255),
    GPACK_RGBA5551(   0,   0,   0,  255)
};

u16  red_tlut[16] = {
    GPACK_RGBA5551(   0,   0,   0,    0),
    GPACK_RGBA5551( 255,   0,   0,  255),
    GPACK_RGBA5551( 255,   0,   0,  255),
    GPACK_RGBA5551( 255,   0,   0,  255),
    GPACK_RGBA5551( 255,   0,   0,  255),
    GPACK_RGBA5551( 255,   0,   0,  255),
    GPACK_RGBA5551( 255,   0,   0,  255),
    GPACK_RGBA5551( 255,   0,   0,  255),
    GPACK_RGBA5551( 255,   0,   0,  255),
    GPACK_RGBA5551( 255,   0,   0,  255),
    GPACK_RGBA5551( 255,   0,   0,  255),
    GPACK_RGBA5551( 255,   0,   0,  255),
    GPACK_RGBA5551( 255,   0,   0,  255),
    GPACK_RGBA5551( 255,   0,   0,  255),
    GPACK_RGBA5551( 255,   0,   0,  255),
    GPACK_RGBA5551( 255,   0,   0,  255),
};

u16  green_tlut[16] = {
    GPACK_RGBA5551(   0,   0,   0,    0),
    GPACK_RGBA5551(   0, 255,   0,  255),
    GPACK_RGBA5551(   0, 255,   0,  255),
    GPACK_RGBA5551(   0, 255,   0,  255),
    GPACK_RGBA5551(   0, 255,   0,  255),
    GPACK_RGBA5551(   0, 255,   0,  255),
    GPACK_RGBA5551(   0, 255,   0,  255),
    GPACK_RGBA5551(   0, 255,   0,  255),
    GPACK_RGBA5551(   0, 255,   0,  255),
    GPACK_RGBA5551(   0, 255,   0,  255),
    GPACK_RGBA5551(   0, 255,   0,  255),
    GPACK_RGBA5551(   0, 255,   0,  255),
    GPACK_RGBA5551(   0, 255,   0,  255),
    GPACK_RGBA5551(   0, 255,   0,  255),
    GPACK_RGBA5551(   0, 255,   0,  255),
    GPACK_RGBA5551(   0, 255,   0,  255),

};

u16  blue_tlut[16] = {
    GPACK_RGBA5551(   0,   0,   0,    0),
    GPACK_RGBA5551(   0,   0, 255,  255),
    GPACK_RGBA5551(   0,   0, 255,  255),
    GPACK_RGBA5551(   0,   0, 255,  255),
    GPACK_RGBA5551(   0,   0, 255,  255),
    GPACK_RGBA5551(   0,   0, 255,  255),
    GPACK_RGBA5551(   0,   0, 255,  255),
    GPACK_RGBA5551(   0,   0, 255,  255),
    GPACK_RGBA5551(   0,   0, 255,  255),
    GPACK_RGBA5551(   0,   0, 255,  255),
    GPACK_RGBA5551(   0,   0, 255,  255),
    GPACK_RGBA5551(   0,   0, 255,  255),
    GPACK_RGBA5551(   0,   0, 255,  255),
    GPACK_RGBA5551(   0,   0, 255,  255),
    GPACK_RGBA5551(   0,   0, 255,  255),
    GPACK_RGBA5551(   0,   0, 255,  255)
};

u16  yellow_tlut[16] = {
    GPACK_RGBA5551(   0,   0,   0,    0),
    GPACK_RGBA5551( 255, 255,   0,  255),
    GPACK_RGBA5551( 255, 255,   0,  255),
    GPACK_RGBA5551( 255, 255,   0,  255),
    GPACK_RGBA5551( 255, 255,   0,  255),
    GPACK_RGBA5551( 255, 255,   0,  255),
    GPACK_RGBA5551( 255, 255,   0,  255),
    GPACK_RGBA5551( 255, 255,   0,  255),
    GPACK_RGBA5551( 255, 255,   0,  255),
    GPACK_RGBA5551( 255, 255,   0,  255),
    GPACK_RGBA5551( 255, 255,   0,  255),
    GPACK_RGBA5551( 255, 255,   0,  255),
    GPACK_RGBA5551( 255, 255,   0,  255),
    GPACK_RGBA5551( 255, 255,   0,  255),
    GPACK_RGBA5551( 255, 255,   0,  255),
    GPACK_RGBA5551( 255, 255,   0,  255),
};

u16  purple_tlut[16] = {
    GPACK_RGBA5551(   0,   0,   0,    0),
    GPACK_RGBA5551( 255,   0, 255,  255),
    GPACK_RGBA5551( 255,   0, 255,  255),
    GPACK_RGBA5551( 255,   0, 255,  255),
    GPACK_RGBA5551( 255,   0, 255,  255),
    GPACK_RGBA5551( 255,   0, 255,  255),
    GPACK_RGBA5551( 255,   0, 255,  255),
    GPACK_RGBA5551( 255,   0, 255,  255),
    GPACK_RGBA5551( 255,   0, 255,  255),
    GPACK_RGBA5551( 255,   0, 255,  255),
    GPACK_RGBA5551( 255,   0, 255,  255),
    GPACK_RGBA5551( 255,   0, 255,  255),
    GPACK_RGBA5551( 255,   0, 255,  255),
    GPACK_RGBA5551( 255,   0, 255,  255),
    GPACK_RGBA5551( 255,   0, 255,  255),
    GPACK_RGBA5551( 255,   0, 255,  255),
};

u16  aqua_tlut[16] = {
    GPACK_RGBA5551(   0,   0,   0,    0),
    GPACK_RGBA5551(   0, 255, 255,  255),
    GPACK_RGBA5551(   0, 255, 255,  255),
    GPACK_RGBA5551(   0, 255, 255,  255),
    GPACK_RGBA5551(   0, 255, 255,  255),
    GPACK_RGBA5551(   0, 255, 255,  255),
    GPACK_RGBA5551(   0, 255, 255,  255),
    GPACK_RGBA5551(   0, 255, 255,  255),
    GPACK_RGBA5551(   0, 255, 255,  255),
    GPACK_RGBA5551(   0, 255, 255,  255),
    GPACK_RGBA5551(   0, 255, 255,  255),
    GPACK_RGBA5551(   0, 255, 255,  255),
    GPACK_RGBA5551(   0, 255, 255,  255),
    GPACK_RGBA5551(   0, 255, 255,  255),
    GPACK_RGBA5551(   0, 255, 255,  255),
    GPACK_RGBA5551(   0, 255, 255,  255),
};

/*The offset table of the font texture */
static int FontIdx[] = {
    0+256*0,  4+256*0,  8+256*0,  12+256*0,  16+256*0, 20+256*0, 24+256*0, 28+256*0,
    0+256*1,  4+256*1,  8+256*1,  12+256*1,  16+256*1, 20+256*1, 24+256*1, 28+256*1,
    0+256*2,  4+256*2,  8+256*2,  12+256*2,  16+256*2, 20+256*2, 24+256*2, 28+256*2,
    0+256*3,  4+256*3,  8+256*3,  12+256*3,  16+256*3, 20+256*3, 24+256*3, 28+256*3,
    0+256*4,  4+256*4,  8+256*4,  12+256*4,  16+256*4, 20+256*4, 24+256*4, 28+256*4,
    0+256*5,  4+256*5,  8+256*5,  12+256*5,  16+256*5, 20+256*5, 24+256*5, 28+256*5,
    0+256*6,  4+256*6,  8+256*6,  12+256*6,  16+256*6, 20+256*6, 24+256*6, 28+256*6,
    0+256*7,  4+256*7,  8+256*7,  12+256*7,  16+256*7, 20+256*7, 24+256*7, 28+256*7,
    0+256*8,  4+256*8,  8+256*8,  12+256*8,  16+256*8, 20+256*8, 24+256*8, 28+256*8,
    0+256*9,  4+256*9,  8+256*9,  12+256*9,  16+256*9, 20+256*9, 24+256*9, 28+256*9,
    0+256*10, 4+256*10, 8+256*10, 12+256*10, 16+256*10,20+256*10,24+256*10,28+256*10,
};
    
/*The table for string search */
static char letters_string[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890.,`'\"@:+-=!_/?()[]";


/*----------------------------------------------------------------------*/
/*	Draw8Font - Display characters of 8x8 or 4x8 on the screen 		*/
/*	IN:	posx	The displaying X-coordinate 					*/
/*		posy	The displaying Y-coordinate					*/
/*		texcol	The displaying X color					*/
/*		texsize	The displaying size					*/
/*	RET:	None 										*/
/*----------------------------------------------------------------------*/
void
Draw8Font( int posx, int posy, int texcol, int texsize )
{
    int idx, idx2;

    gDPSetCycleType(glistp++, G_CYC_1CYCLE);
    gDPSetTextureFilter(glistp++, G_TF_POINT);
    gDPSetRenderMode(glistp++, G_RM_TEX_EDGE, G_RM_TEX_EDGE);
    
    gSPTexture(glistp++, 0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON);
    gDPSetCombineMode(glistp++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    gDPSetTexturePersp(glistp++, G_TP_NONE);
    
    switch(texcol){
      case TEX_COL_WHITE:
	gDPLoadTLUT_pal16(glistp++,  0, white_tlut);
	break;
      case TEX_COL_BLACK:
	gDPLoadTLUT_pal16(glistp++,  0, black_tlut);
	break;
      case TEX_COL_RED:
	gDPLoadTLUT_pal16(glistp++,  0, red_tlut);
	break;
      case TEX_COL_GREEN:
	gDPLoadTLUT_pal16(glistp++,  0, green_tlut);
	break;
      case TEX_COL_BLUE:
	gDPLoadTLUT_pal16(glistp++,  0, blue_tlut);
	break;
      case TEX_COL_YELLOW:
	gDPLoadTLUT_pal16(glistp++,  0, yellow_tlut);
	break;
      case TEX_COL_PURPLE:
	gDPLoadTLUT_pal16(glistp++,  0, purple_tlut);
	break;
      case TEX_COL_AQUA:
	gDPLoadTLUT_pal16(glistp++,  0, aqua_tlut);
	break;
      default:
	break;
    }


    gDPLoadTLUT_pal16(glistp++, 15, default_tlut);
    gDPSetTextureLUT(glistp++, G_TT_RGBA16);
    
    for(idx = 0; outstring[idx] != 0x0; idx++){
	for(idx2 = 0; idx2 < 80; idx2++){
	    if(outstring[idx] == letters_string[idx2]){
		gDPLoadTextureTile_4b(glistp++,
				      font_img+(sizeof(char))*FontIdx[idx2],
				      G_IM_FMT_I,
				      64, 80,
				      0, 0, 7, 7,
				      0,
				      G_TX_WRAP, G_TX_WRAP,
				      G_TX_NOMASK, G_TX_NOMASK,
				      G_TX_NOLOD, G_TX_NOLOD);
		if(texsize == 0){/* 8x8 font */
		    gSPTextureRectangle(glistp++,
					(int)( posx + idx * 8 ) << 2,
					(int)( posy ) << 2,
					(int)( posx + idx * 8 + 7) << 2,
					(int)( posy + 7 ) << 2,
					G_TX_RENDERTILE,
					(0 << 5),
					(0 << 5),
					(int)(1 << 10),
					(int)(1 << 10)
					);
		}else{/* 4x8 font */
		    gSPTextureRectangle(glistp++,
					(int)( posx + idx * 5 ) << 2,
					(int)( posy ) << 2,
					(int)( posx + idx * 8 + 4) << 2,
					(int)( posy + 7 ) << 2,
					G_TX_RENDERTILE,
					(0 << 6),
					(0 << 5),
					(int)(1 << 11),
					(int)(1 << 10)
					);
		}
		gDPPipeSync(glistp++);
	    }
	    gDPPipeSync(glistp++);
	}
    }
}
