
/*************************************************************

  test_draw.c : Nintendo 64 Music Tools Library Sample
  (c) Copyright 1998, Software Creations (Holdings) Ltd.

  Version 3.14

  TESTMENU demo text drawing file. Uses a simple menu to 
  illustrate the use of various music library functions.

**************************************************************/

/* include system header files */
#ifndef F3DEX_GBI
#define F3DEX_GBI
#endif

#include <ultra64.h>

/* include current header file */
#include "test_rom.h"

/* pixel values */
#define z 0x0000,
#define W 0xFFFF,

unsigned short letter0[64]=
{
  z z W W W W z z 
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z z W W W W z z 
  z z z z z z z z
};

unsigned short letter1[64]=
{
  z z z W W z z z 
  z z W W W z z z 
  z z z W W z z z 
  z z z W W z z z 
  z z z W W z z z 
  z z z W W z z z 
  z z W W W W z z 
  z z z z z z z z
};

unsigned short letter2[64]=
{
  z z W W W W z z 
  z W W z z W W z 
  z z z z z W W z 
  z z W W W W z z 
  z W W z z z z z 
  z W W z z z z z 
  z W W W W W W z 
  z z z z z z z z
};

unsigned short letter3[64]=
{
  z z W W W W z z 
  z W W z z W W z 
  z z z z z W W z 
  z z z W W W z z 
  z z z z z W W z 
  z W W z z W W z 
  z z W W W W z z 
  z z z z z z z z
};

unsigned short letter4[64]=
{
  z z z z W W z z 
  z z z W W W z z 
  z z W W W W z z 
  z W W z W W z z 
  z W W W W W W z 
  z z z z W W z z 
  z z z z W W z z 
  z z z z z z z z
};

unsigned short letter5[64]=
{
  z W W W W W W z 
  z W W z z z z z 
  z W W z z z z z 
  z W W W W W z z 
  z z z z z W W z 
  z z z z z W W z 
  z W W W W W z z 
  z z z z z z z z
};

unsigned short letter6[64]=
{
  z z W W W W z z 
  z W W z z z z z 
  z W W z z z z z 
  z W W W W W z z 
  z W W z z W W z 
  z W W z z W W z 
  z z W W W W z z 
  z z z z z z z z
};

unsigned short letter7[64]=
{
  z W W W W W W z 
  z z z z z W W z 
  z z z z W W z z 
  z z z W W z z z 
  z z z W W z z z 
  z z z W W z z z 
  z z z W W z z z 
  z z z z z z z z
};

unsigned short letter8[64]=
{
  z z W W W W z z 
  z W W z z W W z 
  z W W z z W W z 
  z z W W W W z z 
  z W W z z W W z 
  z W W z z W W z 
  z z W W W W z z 
  z z z z z z z z
};

unsigned short letter9[64]=
{
  z z W W W W z z 
  z W W z z W W z 
  z W W z z W W z 
  z z W W W W W z 
  z z z z z W W z 
  z z z z z W W z 
  z z W W W W z z 
  z z z z z z z z
};

unsigned short letterA[64]=
{
  z z W W W W z z 
  z W W z z W W z 
  z W W z z W W z 
  z W W W W W W z 
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z z z z z z z z
};

unsigned short letterB[64]=
{
  z W W W W W z z 
  z W W z z W W z 
  z W W z z W W z 
  z W W W W W z z 
  z W W z z W W z 
  z W W z z W W z 
  z W W W W W z z 
  z z z z z z z z
};

unsigned short letterC[64]=
{
  z z W W W W z z 
  z W W z z W W z 
  z W W z z z z z 
  z W W z z z z z 
  z W W z z z z z 
  z W W z z W W z 
  z z W W W W z z 
  z z z z z z z z
};

unsigned short letterD[64]=
{
  z W W W W W z z 
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z W W W W W z z 
  z z z z z z z z
};

unsigned short letterE[64]=
{
  z W W W W W W z 
  z W W z z z z z 
  z W W z z z z z 
  z W W W W W z z 
  z W W z z z z z 
  z W W z z z z z 
  z W W W W W W z 
  z z z z z z z z
};

unsigned short letterF[64]=
{
  z W W W W W W z 
  z W W z z z z z 
  z W W z z z z z 
  z W W W W W z z 
  z W W z z z z z 
  z W W z z z z z 
  z W W z z z z z 
  z z z z z z z z
};

unsigned short letterG[64]=
{
  z z W W W W z z 
  z W W z z W W z 
  z W W z z z z z 
  z W W z W W W z 
  z W W z z W W z 
  z W W z z W W z 
  z z W W W W z z 
  z z z z z z z z
};

unsigned short letterH[64]=
{
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z W W W W W W z 
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z z z z z z z z
};

unsigned short letterI[64]=
{
  z z W W W W z z 
  z z z W W z z z 
  z z z W W z z z 
  z z z W W z z z 
  z z z W W z z z 
  z z z W W z z z 
  z z W W W W z z 
  z z z z z z z z
};

unsigned short letterJ[64]=
{
  z z z W W W W z 
  z z z z z W W z 
  z z z z z W W z 
  z z z z z W W z 
  z z z z z W W z 
  z W W z z W W z 
  z z W W W W z z 
  z z z z z z z z
};

unsigned short letterK[64]=
{
  z W W z z W W z 
  z W W z z W W z 
  z W W z W W z z 
  z W W W W z z z 
  z W W z W W z z 
  z W W z z W W z 
  z W W z z W W z 
  z z z z z z z z
};

unsigned short letterL[64]=
{
  z W W z z z z z 
  z W W z z z z z 
  z W W z z z z z 
  z W W z z z z z 
  z W W z z z z z 
  z W W z z z z z 
  z W W W W W W z 
  z z z z z z z z
};

unsigned short letterM[64]=
{
  z W W z z z W W 
  z W W W z W W W 
  z W W W W W W W 
  z W W z W z W W 
  z W W z z z W W 
  z W W z z z W W 
  z W W z z z W W 
  z z z z z z z z
};

unsigned short letterN[64]=
{
  z W W z z W W z 
  z W W W z W W z 
  z W W W W W W z 
  z W W z W W W z 
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z z z z z z z z
};

unsigned short letterO[64]=
{
  z z W W W W z z 
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z z W W W W z z 
  z z z z z z z z
};

unsigned short letterP[64]=
{
  z W W W W W z z 
  z W W z z W W z 
  z W W z z W W z 
  z W W W W W z z 
  z W W z z z z z 
  z W W z z z z z 
  z W W z z z z z 
  z z z z z z z z
};

unsigned short letterQ[64]=
{
  z z W W W W z z 
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z W W z W z W z 
  z W W z W W z z 
  z z W W z W W z 
  z z z z z z z z
};

unsigned short letterR[64]=
{
  z W W W W W z z 
  z W W z z W W z 
  z W W z z W W z 
  z W W W W W z z 
  z W W W W z z z 
  z W W z W W z z 
  z W W z z W W z 
  z z z z z z z z
};

unsigned short letterS[64]=
{
  z z W W W W z z 
  z W W z z W W z 
  z W W z z z z z 
  z z W W W W z z 
  z z z z z W W z 
  z W W z z W W z 
  z z W W W W z z 
  z z z z z z z z
};

unsigned short letterT[64]=
{
  z W W W W W W z 
  z z z W W z z z 
  z z z W W z z z 
  z z z W W z z z 
  z z z W W z z z 
  z z z W W z z z 
  z z z W W z z z 
  z z z z z z z z
};

unsigned short letterU[64]=
{
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z z W W W W z z 
  z z z z z z z z
};

unsigned short letterV[64]=
{
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z z W W W W z z 
  z z z W W z z z 
  z z z z z z z z
};

unsigned short letterW[64]=
{
  z W W z z z W W 
  z W W z z z W W 
  z W W z z z W W 
  z W W z W z W W 
  z W W W W W W W 
  z W W W z W W W 
  z z W z z z W z 
  z z z z z z z z
};

unsigned short letterX[64]=
{
  z W W z z W W z 
  z W W z z W W z 
  z z W W W W z z 
  z z z W W z z z 
  z z W W W W z z 
  z W W z z W W z 
  z W W z z W W z 
  z z z z z z z z
};

unsigned short letterY[64]=
{
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z z W W W W z z 
  z z z W W z z z 
  z z z W W z z z 
  z z z W W z z z 
  z z z z z z z z
};

unsigned short letterZ[64]=
{
  z W W W W W W z 
  z z z z z W W z 
  z z z z W W z z 
  z z z W W z z z 
  z z W W z z z z 
  z W W z z z z z 
  z W W W W W W z 
  z z z z z z z z
};

unsigned short letterPointer[64]=
{
  W W z z z z z z 
  W W W W z z z z 
  W W W W W W z z 
  W W W W W W W W 
  W W W W W W z z 
  W W W W z z z z 
  W W z z z z z z 
  z z z z z z z z
};


unsigned short letterASC33[64]=
{
  z z z W W z z z 
  z z z W W z z z 
  z z z W W z z z 
  z z z W W z z z 
  z z z W W z z z 
  z z z z z z z z 
  z z z W W z z z 
  z z z z z z z z
};

unsigned short letterASC34[64]=
{
  z W W z z W W z 
  z W W z z W W z 
  z W W z z W W z 
  z z z z z z z z 
  z z z z z z z z 
  z z z z z z z z 
  z z z z z z z z 
  z z z z z z z z
};

unsigned short letterASC35[64]=
{
  z z W W z W W z 
  z z W W z W W z 
  z W W W W W W W 
  z z W W z W W z 
  z W W W W W W W 
  z z W W z W W z 
  z z W W z W W z 
  z z z z z z z z
};

unsigned short letterASC36[64]=
{
  z z z W W z z z 
  z z W W W W W z 
  z W z W W z z z 
  z z W W W W z z 
  z z z W W z W z 
  z W W W W W z z 
  z z z W W z z z 
  z z z z z z z z
};

unsigned short letterASC37[64]=
{
  z W W z z z W z 
  z W W z z W W z 
  z z z z W W z z 
  z z z W W z z z 
  z z W W z z z z 
  z W W z z W W z 
  z W z z z W W z 
  z z z z z z z z
};

unsigned short letterASC38[64]=
{
  z z W W W z z z 
  z W z z z W z z 
  z W z z z W z z 
  z z W z W z z z 
  z W z W z W z z 
  z W z z W z z z 
  z z W W z W W z 
  z z z z z z z z
};

unsigned short letterASC39[64]=
{
  z z z W W z z z 
  z z z W W z z z 
  z z z W z z z z 
  z z z z z z z z 
  z z z z z z z z 
  z z z z z z z z 
  z z z z z z z z 
  z z z z z z z z
};

unsigned short letterASC40[64]=
{
  z z z z z W W z 
  z z z z W W z z 
  z z z z W W z z 
  z z z z W W z z 
  z z z z W W z z 
  z z z z W W z z 
  z z z z z W W z 
  z z z z z z z z
};

unsigned short letterASC41[64]=
{
  z W W z z z z z 
  z z W W z z z z 
  z z W W z z z z 
  z z W W z z z z 
  z z W W z z z z 
  z z W W z z z z 
  z W W z z z z z 
  z z z z z z z z
};

unsigned short letterASC42[64]=
{
  z z z z z z z z 
  z W z W W z W z 
  z z W W W W z z 
  z W W W W W W z 
  z z W W W W z z 
  z W z W W z W z 
  z z z z z z z z 
  z z z z z z z z
};

unsigned short letterASC43[64]=
{
  z z z z z z z z 
  z z z W W z z z 
  z z z W W z z z 
  z W W W W W W z 
  z z z W W z z z 
  z z z W W z z z 
  z z z z z z z z 
  z z z z z z z z
};

unsigned short letterASC44[64]=
{
  z z z z z z z z 
  z z z z z z z z 
  z z z z z z z z 
  z z z z z z z z 
  z z z z z z z z 
  z W W z z z z z 
  z W W z z z z z 
  z W z z z z z z
};

unsigned short letterASC45[64]=
{
  z z z z z z z z 
  z z z z z z z z 
  z z z z z z z z 
  z W W W W W W z 
  z z z z z z z z 
  z z z z z z z z 
  z z z z z z z z 
  z z z z z z z z
};

unsigned short letterASC46[64]=
{
  z z z z z z z z 
  z z z z z z z z 
  z z z z z z z z 
  z z z z z z z z 
  z z z z z z z z 
  z W W z z z z z 
  z W W z z z z z 
  z z z z z z z z
};

unsigned short letterASC47[64]=
{
  z z z z z z W z 
  z z z z z W W z 
  z z z z W W z z 
  z z z W W z z z 
  z z W W z z z z 
  z W W z z z z z 
  z W z z z z z z 
  z z z z z z z z
};

unsigned short letterASC58[64]=
{
  z z z z z z z z 
  z z z z z z z z 
  z z z W W z z z 
  z z z W W z z z 
  z z z z z z z z 
  z z z W W z z z 
  z z z W W z z z 
  z z z z z z z z
};

unsigned short letterASC59[64]=
{
  z z z z z z z z 
  z z z z z z z z 
  z z z W W z z z 
  z z z W W z z z 
  z z z z z z z z 
  z z z W W z z z 
  z z z W W z z z 
  z z W W z z z z
};

unsigned short letterASC60[64]=
{
  z z z z W W z z 
  z z z W W z z z 
  z z W W z z z z 
  z W W z z z z z 
  z z W W z z z z 
  z z z W W z z z 
  z z z z W W z z 
  z z z z z z z z
};

unsigned short letterASC61[64]=
{
  z z z z z z z z 
  z z z z z z z z 
  z z W W W W z z 
  z z z z z z z z 
  z z W W W W z z 
  z z z z z z z z 
  z z z z z z z z 
  z z z z z z z z
};

unsigned short letterASC62[64]=
{
  z z W W z z z z 
  z z z W W z z z 
  z z z z W W z z 
  z z z z z W W z 
  z z z z W W z z 
  z z z W W z z z 
  z z W W z z z z 
  z z z z z z z z
};

unsigned short letterASC63[64]=
{
  z z W W W W z z 
  z W W z z W W z 
  z z z z z W W z 
  z z z z W W z z 
  z z z W W z z z 
  z z z z z z z z 
  z z z W W z z z 
  z z z z z z z z
};

unsigned short letterASC64[64]=
{
  z z W W W W z z 
  z W W z z W W z 
  z W W z z W W z 
  z W W z W W z z 
  z W W z z z z z 
  z W W z z W W z 
  z z W W W W z z 
  z z z z z z z z
};

/* ASCII character lookup table */
unsigned short *letter_list[]=
{
  letterASC33, letterASC34, letterASC35, letterASC36, letterASC37,
  letterASC38, letterASC39, letterASC40, letterASC41, letterASC42,
  letterASC43, letterASC44, letterASC45, letterASC46, letterASC47,
  letter0, letter1, letter2, letter3, letter4, letter5, letter6, letter7, letter8, letter9,
  letterASC58, letterASC59, letterASC60, letterASC61, letterASC62,
  letterASC63, letterASC64,                                        
  letterA, letterB, letterC, letterD, letterE, letterF, letterG, letterH, letterI, letterJ,
  letterK, letterL, letterM, letterN, letterO, letterP, letterQ, letterR, letterS, letterT,
  letterU, letterV, letterW, letterX, letterY, letterZ,
};

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [GLOBAL FUNCTION]
  DrawString(text, x, y)

  [Parameters]
  text		address of ASCIIZ string
  x		   screen X position
  y		   screen Y position

  [Explanation]
  Generate the drawing commands necessary to draw the ASCIIZ string on to the screen
  at the specified position.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

void DrawString(char *text, int x, int y)
{
  char letter;

  while (*text)
  {
    letter = *text++;
    if (letter>' ' && letter<='Z')
      letter-='!';
    else if (letter>='a' && letter<='z')
      letter=letter-('a'-'A')-'!';
    else
    {
      x+=8;
      continue;
    }
    DrawChar(letter_list[letter],x,y);
    x+=8;
  }
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [GLOBAL FUNCTION]
  DrawChar(letter, x, y)

  [Parameters]
  letter    address of 8x8 16-bit RGBA graphic
  x		   screen X position
  y		   screen Y position

  [Explanation]
  Generate the drawing commands necessary to draw the specified 8x8 16-bit RGBA 
  character on to the screen at the specified position. Note this function does
  not detect any overflow of the drawing buffers.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

void DrawChar(unsigned short *letter, int x, int y)
{
  gDPLoadTextureBlock(glistp++, letter, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8,8,0,
		      G_TX_WRAP|G_TX_NOMIRROR,G_TX_WRAP|G_TX_NOMIRROR,
		      3,3,G_TX_NOLOD,G_TX_NOLOD);
  vertp->n.ob[0]	= x;
  vertp->n.ob[1]	= y;
  vertp->n.ob[2]	= 0;
  vertp->n.tc[0] 	= 0; 
  vertp->n.tc[1] 	= 0;
  vertp++;
  vertp->n.ob[0]	= x+8;
  vertp->n.ob[1]	= y;
  vertp->n.ob[2]	= vertp->n.tc[1] = 0;
  vertp->n.tc[0]	= 8*32;
  vertp++;
  vertp->n.ob[0]	= x+8;
  vertp->n.ob[1]	= y+8;
  vertp->n.ob[2]	= 0;
  vertp->n.tc[0]	= 8*32;
  vertp->n.tc[1]	= 8*32;
  vertp++;
  vertp->n.ob[0]	= x;
  vertp->n.ob[1]	= y+8;
  vertp->n.ob[2]	= 0;
  vertp->n.tc[0] 	= 0;
  vertp->n.tc[1]	= 8*32;
  vertp++;
  
  gSPVertex(glistp++,  OS_K0_TO_PHYSICAL(vertp-4),4,0);
  gSP2Triangles(glistp++, 0, 1, 2, 0, 0, 2, 3, 0);
}

/* end of file */
