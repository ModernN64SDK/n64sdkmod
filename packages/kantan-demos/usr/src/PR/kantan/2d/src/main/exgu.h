
#ifndef __EXGU_H
#define __EXGU_H

#ifndef _ULTRA64_H_
#include <ultra64.h>
#endif

/* ===========================================================================
	defines
 =========================================================================== */

/* ===========================================================================
	type definitions
 =========================================================================== */

typedef struct{
    u8 r,g,b,a;
} RGBAQUAD;

/* ===========================================================================
	public functions
 =========================================================================== */

/* ---------------------------------------------------------------------------
   geometry operations sub-routine performed by CPU

Performs cumulative geometric operations in the designated matrix array.
	Be sure not to perform more than the minimum amount of computation 
	necessary.
*/

void exguTranslateF(float mf[4][4], float x, float y, float z);
void exguScaleF(float mf[4][4], float x, float y, float z);
void exguRotateRPYF(float mf[4][4], float r, float p, float h);
void exguRotateXF(float mf[4][4], float rot);
void exguRotateYF(float mf[4][4], float rot);
void exguRotateZF(float mf[4][4], float rot);

/* ---------------------------------------------------------------------------
   Routine for setting the RDP color combiner mode

Simplified routine that we think will not be used much during runtime.
Anyway, since it's for testing, it's been put together as we thought best.
	It supplies various character strings, but there's no need to terminate 
	at "¥0." 

void exgDPSetCombineMode2Cycle(Gfx *glp, char *mode1, char *mode2);
void exgDPSetCombineMode1Cycle(Gfx *glp, char *mode1);

      details of the mode [12] character string
	 ++++----------	Compute RGB combiner
	 ||||		 input source (a-b)*c+d.
	 ||||		 For details, refer to Fig. 12-11 on p. 37 in the 
	 ||||		 N64 Manual, 4/1/96 version.
	 ||||
	 ||||++++------	Compute Alpha combiner 
	 ||||||||	 input source (e-f)*g+h.
	 ||||||||	 For details, refer to Fig. 12-11 on p. 37 in the 
	 ||||||||	 N64 Manual, 4/1/96 version.
	 ||||||||
	"abcdefgh"

void exgDPSetRenderMode2Cycle(Gfx *glp, char *common, char *c0, char *c1);
void exgDPSetRenderMode1Cycle(Gfx *glp, char *common, char *c0);

      details of common character string
	 +------------- AA_EN
	 |+------------	Z_CMP
	 ||+----------- Z_UPD
	 |||+---------- IM_RD
	 ||||+--------- CLR_ON_CVG
	 ||||||+------- CVG_DST_*****
	 ||||||		  '0' = CVG_DST_CLAMP
	 ||||||		  '1' =	CVG_DST_WRAP
	 ||||||		  '2' =	CVG_DST_FULL
	 ||||||		  '3' = CVG_DST_SAVE
	 ||||||
	 ||||||+------- ZMODE_***
	 |||||||	  '0' = ZMODE_OPA
	 |||||||	  '1' = ZMODE_INTER
	 |||||||	  '2' = ZMODE_XLU
	 |||||||	  '3' = ZMODE_DEC
	 |||||||
	 |||||||+------ CVG_X_ALPHA
	 ||||||||+----- ALPHA_CVG_SEL
	 |||||||||+---- FORCE_BL
	 ||||||||||+---	TEX_EDGE
	 |||||||||||	
	"abcdefghijk"

      details of c[01] character string
since there's no detailed account in the current [3/27/96] Japanese 
manual, here's a simple explanation.  The final output color values 
are represented by the following formula.
	color = (p*a+m*b)/(a+b)
  set the values for p, a, m, and b in the above formula in the c0, c1
  character strings.

         +------------- p color
	 |		  '0' = first cycle: CC output color
	 |		        second cycle: output color of first cycle
	 |		  '1' = memory color
	 |		  '2' = blend color
	 |		  '3' = fog color
	 |
	 |+------------ a color
	 ||		  '0' = CC output color
	 ||		  '1' = alpha value of  fog color
	 ||		  '2' = alpha value of shadow color
	 ||		  '3' = 0.0
	 ||
	 ||+----------- m color
	 |||		  same as "p color" above
	 |||
	 |||+---------- b color
	 ||||		  '0' = "a color" taken from 1.0
	 ||||		  '1' = alpha value of memory color
	 ||||		  '2' = 1.0
	 ||||		  '3' = 0.0
        "pamb"
*/

void exgDPSetCombineModeDetail(Gfx *glp, int a[16]);
void exgDPSetCombineMode2Cycle(Gfx *glp, char *mode1, char *mode2);
void exgDPSetCombineMode1Cycle(Gfx *glp, char *mode1);

void exgDPSetRenderModeDetail(Gfx *glp, unsigned int a);
void exgDPSetRenderMode2Cycle(Gfx *glp, char *common, char *c0, char *c1);
void exgDPSetRenderMode1Cycle(Gfx *glp, char *common, char *c0);

u16 exguRGBA32ToRGBA16( u32 col );
RGBAQUAD *exguCalcFadeColor(RGBAQUAD *tar, int flag, int ofs, int wlev);
RGBAQUAD *exguCalcGradationColor(RGBAQUAD *tar, int flag, RGBAQUAD *fact, int lev);

#endif /* #ifndef __EXGU_H */
