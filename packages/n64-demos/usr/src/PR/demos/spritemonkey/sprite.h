
/**************************************************************************
 *                                                                        *
 *               Copyright (C) 1995, Silicon Graphics, Inc.               *
 *                                                                        *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright  law.  They  may not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *                                                                        *
 *************************************************************************/

/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo. (Originated by SGI)
        
        $RCSfile: sprite.h,v $
        $Revision: 1.4 $
        $Date: 1998/09/30 13:27:42 $
 *---------------------------------------------------------------------*/

/*
 * File:	lines.h
 * Create Date:	Thu Jun 22 09:28:01 PDT 1995
 *
 */

#define	STATIC_SEGMENT		1

#define PRINTF osSyncPrintf 

#define MAXTEXTURES 2

/* this stack size is in bytes, and is a lot larger
 * than this program needs.
 */
#define	STACKSIZE	0x2000

#define	GLIST_LEN	8192

/*
 * ifdef needed because this file is included by "spec"
 */
#ifdef _LANGUAGE_C

/*
 * Layout of dynamic data.
 *
 * This structure holds the things which change per frame. It is advantageous
 * to keep dynamic data together so that we may selectively write back dirty
 * data cache lines to DRAM prior to processing by the RCP.
 *
 */
typedef struct {
  Mtx	projection;

  Mtx	modeling1;
  Mtx   modeling2;
  Mtx   modeling3;
  Mtx   modeling4;

  Mtx	viewing;
  Mtx   identity;
  Gfx	glist[GLIST_LEN];
} Dynamic;

extern Dynamic	dynamic;

extern unsigned short cfb_a[];
extern unsigned short cfb_b[];

extern void     *cfb_ptrs[2];

/* RSP address for the color frame buffer */

extern int SCREEN_WD;
extern int SCREEN_HT;

extern int TranslateHorizontal;
extern int TranslateVertical;

extern int ControllerMode;
extern int ActiveController;

void ClearScreen(void);
void InitVariables(void);

extern int     ControllerInput;
extern int     PrintInstructions;
extern int     NewTextureNumber;
			    
extern int     TextureStartS;
extern int     TextureStartT;

extern int     RectangleWidth;
extern int     RectangleHeight;

extern int     TextureScaleX;
extern int     TextureScaleY;

extern int     ImageHeight;
extern int     ImageWidth;

extern int     SubImageWidth;

extern int     TextureNumber;

extern int     FlipTextureX;
extern int     FlipTextureY;

extern int     YFlipMirror;

typedef struct 
{
  char TestName[80];
  void (*TestDisplayFunc)(void);
  void (*TestMainLoop)(void);
  void (*TestInitFunc)(void);
  void (*TestEngineFunc)(void);
} TestStructureElement;

extern TestStructureElement TestStruct[];


#endif	/* _LANGUAGE_C */
