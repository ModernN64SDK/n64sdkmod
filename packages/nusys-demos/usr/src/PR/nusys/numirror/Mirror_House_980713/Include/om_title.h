/*======================================================================*/
/*	NIFFprev							*/
/*	om_title.h							*/
/*									*/
/*	Copyright (C) 1998, NINTENDO Co,Ltd.				*/
/*									*/
/*======================================================================*/
#ifndef	_TITLE
#define	_TITLE

#include	<ultra64.h>
#include	<PR/gs2dex.h>

#define		DEF_TITLE_INIT		0	/*Title initialization*/
#define		DEF_TITLE_LOGO_INIT	1	/*Title logo initialization*/
#define		DEF_TITLE_LOGO		2	/*Main title logo*/
#define		DEF_TITLE_MAIN_INIT	3	/*Main title initialization*/
#define		DEF_TITLE_MAIN		4	/*Main title*/
#define		DEF_TITLE_END		5	/*Exit title*/

#define		DEF_PUSHSTART_XPOS	12	/*x-coordinates for displaying PushStartButton*/
#define		DEF_PUSHSTART_YPOS	24	/*y-coordinates for displaying PushStartButton*/
#define		DEF_SCREEN_WIDTH	320	/*Screen width*/
#define		DEF_SCREEN_HEIGHT	240	/*Screen heigh*/

/* ===========================================================================
	Function Prototype Declarations
 =========================================================================== */
void om_title_main( int pedingGfx );

void om_title_init( void );
void om_title_end( void );

void om_title_title( void );
void om_title_title_init( void );
void om_title_display( void );
void om_title_display_PushStart( void );
void om_title_stream_PushStart( u8 );
void om_title_stream_PushStart2( u8 );
void om_title_stream_PushStart3( u8 );
void om_title_blink_PushStart( u8 );
Gfx *om_title_display_background( Gfx *gp );
void om_title_setup_logo_parameters( void );
void om_title_setup_background_parameters( void );

void om_title_logo( void );
void om_title_logo_init( void );
void om_title_logo_display( void );
Gfx *om_title_display_texture( Gfx *gp , u8 , u8 );

/* ===========================================================================
	External Reference
 =========================================================================== */
extern	uObjTxSprite title_sp[];
extern	uObjTxSprite logo_sp[];
extern	uObjMtx	mt;
uObjTxtr objTlut;

/* ===========================================================================
	General-Purpose Display Lists
 =========================================================================== */
	/* RCP initialization */
static Gfx om_title_setup_RCP_dl[] =
{
	gsDPPipeSync(),
	gsDPPipelineMode(G_PM_1PRIMITIVE),

	gsDPSetTextureLOD(G_TL_TILE),
	gsDPSetTextureLUT(G_TT_NONE),
	gsDPSetTextureDetail(G_TD_CLAMP),
	gsDPSetTexturePersp(G_TP_NONE),
	gsDPSetTextureFilter(G_TF_BILERP),
	gsDPSetTextureConvert(G_TC_FILT),

	gsDPSetCombineMode(G_CC_DECALRGBA,G_CC_DECALRGBA),
	gsDPSetCombineKey(G_CK_NONE),

	gsDPSetAlphaCompare(G_AC_NONE),
	gsDPSetColorDither(G_CD_DISABLE),
	gsDPSetBlendColor(0,0,0,0x01),
	gsDPSetRenderMode(G_RM_OPA_SURF,G_RM_OPA_SURF2),
	gsSPEndDisplayList(),
};

	/* Paste transparent texture */
static Gfx	om_title_texture_dl[] =
{
	gsDPSetCombineMode(G_CC_PRIMITIVE,G_CC_PRIMITIVE),
	gsDPSetRenderMode(G_RM_ZB_XLU_SURF,G_RM_ZB_XLU_SURF2),
	gsSPTextureRectangle(0,0,319<<2,239<<2,
						 0,0,0,0,0),
	gsSPEndDisplayList(),
};

#endif	/* _TITLE */
