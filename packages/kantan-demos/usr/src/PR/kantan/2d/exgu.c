
#define __EXGU_C

/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	Extended gu function, extended g function
 :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#include "pack.h"

/* ===========================================================================
	static variables
 =========================================================================== */

/* ===========================================================================
	public functions
 =========================================================================== */

/* ----------------------------------------------------------------------------
name:
  exguTranslateF, exguScaleF, exguRotateRPYF, exguRotateXF, exguRotateYF,
  exguRotateZF, exguMtxCatF33
synopsis:
  void exguTranslateF(float mf[4][4], float x, float y, float z);
  void exguScaleF(float mf[4][4], float x, float y, float z);
  void exguRotateRPYF(float mf[4][4], float r, float p, float h);
  void exguRotateXF(float mf[4][4], float rot);
  void exguRotateYF(float mf[4][4], float rot);
  void exguRotateZF(float mf[4][4], float rot);
  void exguMtxCatF33(float ans[4][4], float ml[3][3]);
description:
  Plain matrix operating functions
  exguTranslateF, exguScaleF, exguRotateRPYF, exguRotateXF, exguRotateYF,
  exguRotateZF are extended version of the functions provided within libultra.
  exguMtxCatF33 is a module that performs matrix multiplication of 3 x 3 matrix for 4 x 4 matrix.
parameter:
  float mf[4][4];
  float ans[4][4];
	matrix to be performed
  float x,y,z;
	The amount of move  (exguTranslateF), or the angle  (exguRotate*), or the scale value
              (exguScaleF) for each axis. 
  float rot;
	Rotating angle. The number of DEGREE.
  float ml[4][4];
	matrix to perform
notes:
---------------------------------------------------------------------------- */
void exguTranslateF(float mf[4][4], float x, float y, float z)
{
    int i;

    for(i=0; i<4; ++i)
      mf[3][i] += mf[0][i]*x + mf[1][i]*y + mf[2][i]*z;
}

void exguScaleF(float mf[4][4], float x, float y, float z)
{
    int i;

    for (i=0; i<3; i++) {
	mf[0][i] *= x;
	mf[1][i] *= y;
	mf[2][i] *= z;
    }	
}

void exguMtxCatF33(float ans[4][4], float ml[3][3])
{
    int	i, j, k;
    float temp[3][3];

    for (i=0; i<3; i++) {
	for (j=0; j<3; j++) {
	    temp[i][j] = 0.0;
	    for (k=0; k<3; k++) {
		temp[i][j] += ml[i][k] * ans[k][j];
	    }
	}
    }

    /* make sure we handle case where result is an input */
    for (i=0; i<3; i++) {
	for (j=0; j<3; j++) {
	    ans[i][j] = temp[i][j];
	}
    }
}

void exguRotateRPYF(float mf[4][4], float r, float p, float h)
{
    float ans[3][3];
    static float dtor = 3.1415926 / 180.0;
    float sinr, sinp, sinh;
    float cosr, cosp, cosh;

    r *= dtor;
    p *= dtor;
    h *= dtor;
    sinr = sinf(r);
    cosr = cosf(r);
    sinp = sinf(p);
    cosp = cosf(p);
    sinh = sinf(h);
    cosh = cosf(h);

    ans[0][0] = cosp*cosh;
    ans[0][1] = cosp*sinh;
    ans[0][2] = -sinp;

    ans[1][0] = sinr*sinp*cosh - cosr*sinh;
    ans[1][1] = sinr*sinp*sinh + cosr*cosh;
    ans[1][2] = sinr*cosp;

    ans[2][0] = cosr*sinp*cosh + sinr*sinh;
    ans[2][1] = cosr*sinp*sinh - sinr*cosh;
    ans[2][2] = cosr*cosp;

    exguMtxCatF33(mf, ans);
}

void exguRotateXF(float mf[4][4], float rot)
{
    int i;
    static float dtor = 3.1415926 / 180.0;
    float sinx, cosx;
    float ftmp[4];

    ftmp[0] = rot*dtor;
    sinx = sinf(ftmp[0]);
    cosx = cosf(ftmp[0]);

    for(i=0; i<3; ++i)
      ftmp[i] = mf[1][i];

    for(i=0; i<3; ++i){
	mf[1][i] =  ftmp[i]*cosx + mf[2][i]*sinx;
	mf[2][i] = -ftmp[i]*sinx + mf[2][i]*cosx;
    }
}

void exguRotateYF(float mf[4][4], float rot)
{
    int i;
    static float dtor = 3.1415926 / 180.0;
    float siny, cosy;
    float ftmp[4];

    ftmp[0] = rot*dtor;
    siny = sinf(ftmp[0]);
    cosy = cosf(ftmp[0]);

    for(i=0; i<3; ++i)
      ftmp[i] = mf[0][i];

    for(i=0; i<3; ++i){
	mf[0][i] = ftmp[i]*cosy - mf[2][i]*siny;
	mf[2][i] = ftmp[i]*siny + mf[2][i]*cosy;
    }
}

void exguRotateZF(float mf[4][4], float rot)
{
    int i;
    static float dtor = 3.1415926 / 180.0;
    float sinz, cosz;
    float ftmp[4];

    ftmp[0] = rot*dtor;
    sinz = sinf(ftmp[0]);
    cosz = cosf(ftmp[0]);

    for(i=0; i<3; ++i)
      ftmp[i] = mf[0][i];

    for(i=0; i<3; ++i){
	mf[0][i] =  ftmp[i]*cosz + mf[1][i]*sinz;
	mf[1][i] = -ftmp[i]*sinz + mf[1][i]*cosz;
    }
}

/* ----------------------------------------------------------------------------
name:
  exgDPSetCombineModeDetail, exgDPSetCombineMode1Cycle,
  exgDPSetCombineMode2Cycle, exgDPSetRenderModeDetail,
  exgDPSetRenderMode2Cycle, exgDPSetRenderMode1Cycle
synopsis:
  void exgDPSetCombineModeDetail(Gfx *glp, int a[16]);
  void exgDPSetCombineMode1Cycle(Gfx *glp, char *mode1);
  void exgDPSetCombineMode2Cycle(Gfx *glp, char *mode1, char *mode2);
  void exgDPSetRenderModeDetail(Gfx *glp, unsigned int a);
  void exgDPSetRenderMode2Cycle(Gfx *glp, char *common, char *c0, char *c1);
  void exgDPSetRenderMode1Cycle(Gfx *glp, char *common, char *c0);
description:
  A function to provide how to specify a mode setting of RDP dynamically.
parameter:
  Gfx *glp;
	The destination to create a display list
  int a[16];
	CC mode
  char *mode1, *mode2;
	A character string to create CC mode.
	"abcdefgh" ... A character string mode1, mode2
	 ||||||||
	 ||||++++------	Input source of Alpha combiner
	 ||||    	 Calculates (e-f)*g+h  
	 ||||    	For details, the diagram 11-11on p60 of 'Graphics' section of the N64 manual.
	 ||||    	  
	 ||||    
	 ++++----------	Input source of RGB combiner
	     		 Calculates (a-b)*c+d  
	     		 For details, the diagram 11-10 of p59 of "Graphics' section of the N64 manual.
	     
  unsigned int a;
	BL mode value
  char *common;
	A character string to create BL mode.   common specifies the common portion using 1 and 2
              cycle modes.
	"abcdefghijk" ... character string common
	 |||||||||||	
	 ||||||||||+---	TEX_EDGE
	 |||||||||+---- FORCE_BL
	 ||||||||+----- ALPHA_CVG_SEL
	 |||||||+------ CVG_X_ALPHA
	 ||||||+------- ZMODE_***
	 ||||||		  '0' = ZMODE_OPA
	 ||||||		  '1' = ZMODE_INTER
	 ||||||		  '2' = ZMODE_XLU
	 ||||||		  '3' = ZMODE_DEC
	 ||||||
	 |||||+------- CVG_DST_*****
	 |||||		  '0' = CVG_DST_CLAMP
	 |||||		  '1' =	CVG_DST_WRAP
	 |||||		  '2' =	CVG_DST_FULL
	 |||||		  '3' = CVG_DST_SAVE
	 |||||
	 ||||+--------- CLR_ON_CVG
	 |||+---------- IM_RD
	 ||+----------- Z_UPD
	 |+------------	Z_CMP
	 +------------- AA_EN

  char *c0, *c1;
	A character string to create BL mode.  c0 and c1 are BL setting values for each pipeline for the
             time of cycle mode.  The final output color value is expressed as shown below
           color = (p*a+m*b)/(a+b)
        "pamb"
	 ||||
	 |||+---------- b color
	 |||		  '0' = "a color" is subtracted from 1.0  
	 |||		  '1' = alpha color of memory color
	 |||		  '2' = 1.0
	 |||		  '3' = 0.0
	 |||
	 ||+----------- m color
	 ||		  '0' = first cycle: output color of CC
	 ||		         second cycle: output color of first cycle 
	 ||		  '1' = memory color
	 ||		  '2' = blend color
	 ||		  '3' = fog color
	 ||
	 |+------------ a color
	 |		  '0' = output color of CC
	 |		  '1' =alpha value of fog color
	 |		  '2' = alpha value of shade color
	 |		  '3' = 0.0
	 |
         +------------- p color
			same as "m color' above

notes:
  As this is a plain version for testing, 
 Be careful with the value to 
---------------------------------------------------------------------------- */
void exgDPSetCombineModeDetail(Gfx *glp, int a[16])
{
    unsigned int i;

    i =  G_SETCOMBINE << 24;
    i |= ((a[0]&15)<<20) | ((a[2]&31)<<15) | (a[4]<<12) | (a[6]<<9);
    i |= ((a[8]&15)<<5) | (a[10]&31);
    glp->words.w0 = i;
    i =  ((a[1]&15)<<28) | ((a[3]&7)<<15) | (a[5]<<12) | (a[7]<<9);
    i |= ((a[9]&15)<<24) | (a[12]<<21) | (a[14]<<18) | ((a[11]&7)<<6)
      | (a[13]<<3) | a[15];
    glp->words.w1 = i;
}

int hex2dec(char c)
{
    if(c>='a') c-='a'-'A';
    if(c>='A') return c-'A'+10;
    return c-'0';
}

void exgDPSetCombineMode1Cycle(Gfx *glp, char *mode1)
{
    int i,a[16];

    for(i=0; i<8; ++i)
      a[i] = a[i+8] = hex2dec(mode1[i]);

    exgDPSetCombineModeDetail(glp, a);
}

void exgDPSetCombineMode2Cycle(Gfx *glp, char *mode1, char *mode2)
{
    int i,a[16];

    for(i=0; i<8; ++i){
	a[i]   = hex2dec(mode1[i]);
	a[i+8] = hex2dec(mode2[i]);
    }

    exgDPSetCombineModeDetail(glp, a);
}

void exgDPSetRenderModeDetail(Gfx *glp, unsigned int a)
{
    glp->words.w0 = (G_SETOTHERMODE_L<<24)|(G_MDSFT_RENDERMODE<<8)|29;
    glp->words.w1 = a;
}

unsigned int exgsubRM_MASK(char *p)
{
    unsigned int f;

    f=0;
    if(*p>='0' && *p<='3') f|=(1UL*(*p-'0'))<<30;
    ++p;
    if(*p>='0' && *p<='3') f|=(1UL*(*p-'0'))<<26;
    ++p;
    if(*p>='0' && *p<='3') f|=(1UL*(*p-'0'))<<22;
    ++p;
    if(*p>='0' && *p<='3') f|=(1UL*(*p-'0'))<<18;

    return f;
}

void exgDPSetRenderMode2Cycle(Gfx *glp, char *common, char *c0, char *c1)
{
    unsigned int i,j;
    char *p;

    i=0;
    p = common;
    if(*p++=='1') i|=AA_EN;
    if(*p++=='1') i|=Z_CMP;
    if(*p++=='1') i|=Z_UPD;
    if(*p++=='1') i|=IM_RD;
    if(*p++=='1') i|=CLR_ON_CVG;
    if(*p>='0' && *p<='3') i|=(1UL*(*p-'0'))<<8;
    ++p;
    if(*p>='0' && *p<='3') i|=(1UL*(*p-'0'))<<10;
    ++p;
    if(*p++=='1') i|=CVG_X_ALPHA;
    if(*p++=='1') i|=ALPHA_CVG_SEL;
    if(*p++=='1') i|=FORCE_BL;
    if(*p++=='1') i|=TEX_EDGE;

    j =  exgsubRM_MASK(c0);
    j |= exgsubRM_MASK(c1)>>2;

    exgDPSetRenderModeDetail(glp, i|j);
}

void exgDPSetRenderMode1Cycle(Gfx *glp, char *common, char *c0)
{
    exgDPSetRenderMode2Cycle(glp, common, c0, c0);
}

/* ----------------------------------------------------------------------------
name:
    exguRGBA32ToRGBA16
synopsis:
    u16 bmConvRGBA32ToRGBA16( u32 col );
description:
parameter:
return:
notes:
---------------------------------------------------------------------------- */
u16 exguRGBA32ToRGBA16( u32 col )
{
    return
      ((col & 0xf8000000) >> 16) | 
      ((col & 0x00f80000) >> 13) |
      ((col & 0x0000f800) >> 10) |
      ((col & 0x00000080) >> 7);
}

/* ----------------------------------------------------------------------------
name:
    exguCalcFadeColor
synopsis:
    RGBAQUAD *exguCalcFadeColor(RGBAQUAD *tar, int ofs, int wlev)
description:
    return one color shown by tar, modulated to the brightness and 
    density specified by ofs and wlev.
parameter:
    RGBAQUAD
return:
notes:
---------------------------------------------------------------------------- */
RGBAQUAD *exguCalcFadeColor(RGBAQUAD *tar, int flag, int ofs, int wlev)
{
    int i, m;
    u8 *up;

    up = (u8*)tar;
    m = 0x08;
    for(i=0; i<4; ++i, m>>=1)
      if(flag & m)
	up[i] = (((u32)up[i] * ofs) >> 8) + wlev;

    return tar;
}

/* ----------------------------------------------------------------------------
name:
    exguCalcGradationColor
synopsis:
    RGBAQUAD *exguCalcGradationColor(RGBAQUAD *tar, RGBAQUAD *fact, int lev)
description:
parameter:
return:
notes:
---------------------------------------------------------------------------- */
RGBAQUAD *exguCalcGradationColor(RGBAQUAD *tar, int flag, RGBAQUAD *fact, int lev)
{
    int i, m, rlev;
    u8 *up1, *up2;

    rlev =255-lev;
    up1 = (u8*)tar;
    up2 = (u8*)fact;
    m = 0x08;
    for(i=0; i<4; ++i, m>>=1)
      if(flag & m)
	up1[i] = (up1[i] * lev) + (up2[i] * rlev);

    return tar;
}

/* ==========================================================================
	end of file.						%eof
 ========================================================================== */
