/*======================================================================*/
/*	Mirror House							*/
/*	om_select_chara.c						*/
/*									*/
/*	Copyright (C) 1998 MONEGI CORPORATION				*/
/*									*/
/*======================================================================*/
#include <pack.h>

#include "nd.h"
#include "nusys.h"

#include "om.h"
#include "om_def.h"
#include "om_audio.h"



/*------ Constant Definitions ------*/
#define CHARACTER_INDEX		OM_FRAKE_SELECT1	/* Starting number of the character table */
#define CHARACTER_NUM		4	/* Number of display characters */
#define ANIMATION_WAIT		59	/* Count value for animation wait */
#define ANIMATION_END_WAIT	70	/* Count value for animation end wait */
enum{
	ANIM_TRIG		= 0x01,
	ANIM_DIRECTION	= 0x02
};

enum{
	OM_SC_FIRST,
	OM_SC_SELECT,
	OM_SC_A_BUTTON,
	OM_SC_ANIMATION_WAIT,
	OM_SC_ANIMATION_END_WAIT,
	OM_SC_END
};



/*------ Global Variables ------*/
static int nProgress = OM_SC_FIRST;							/* Processing number */
static int nAnimationCount;									/* Animation counter */
static int nAnimFlag[CHARACTER_NUM] = { 0 , 0 , 0 , 0 };	/* Animation flag bit 0 - Trigger for animation start bit 1- Deform direction (+...0, -...1) */
static OM_CHARACTER_DATA *Cursor;							/* Cursor character */
static int CursorOffset[CHARACTER_NUM] =					/* x offset of cursor */
							{ 100 , 120 , 140 , 160 };



/*------ External Reference Declarations ------*/
extern int nCharacterNumber;
extern OM_PROGRAM_SEQ_ST om_program_seq_st;
extern EXCONTPAD *pad;
extern OM_CHARACTER_ST om_character_st;
extern Sprite sp_SelectBG_sprite;
extern void om_game_eval( OM_CHARACTER_ST *st , float um[4][4] );
extern Gfx *om_game_draw3d( Gfx *gp , OM_CHARACTER_ST *st , float um[4][4] );
extern void om_com_character_globalresource_change( OM_CHARACTER_DATA * );
extern void om_audio_se_start(u32 se_no);



/*------ Prototype Declarations ------*/
static void om_Init( OM_CHARACTER_DATA * );
static void om_SetCharacterDsp( OM_CHARACTER_DATA * );
static void om_EvalPad( OM_CHARACTER_DATA * );
static void om_AnimChara( OM_CHARACTER_DATA * );
static void om_AttachDeform( OM_CHARACTER_DATA * );
static void om_CheckAButton( OM_CHARACTER_DATA * );
static void om_TurnOffCharacter( OM_CHARACTER_DATA * );
static void om_SetKimeCharacter( OM_CHARACTER_DATA * );
static void om_AnimationWait( OM_CHARACTER_DATA * );
static void om_AnimationEndWait( OM_CHARACTER_DATA * );
static Gfx *om_DrawBG( Gfx * );



/*====== Character selection (callback function) ======*/
void om_select_character_main( int nPendingGfx )
{
	int i;
	Gfx *glistp;
	float um[4][4];
	OM_CHARACTER_ST *st;
	OM_CHARACTER_DATA *data;

	/* Initialize */
	st = &om_character_st;
	data = st->data;
	for( i = 0 ; i < CHARACTER_INDEX ; i++ ) data = data->next;
	Cursor = data;
	for( i = 0 ; i < 5 ; i++ ) Cursor = Cursor->next;
	guMtxIdentF( um );

	/* Branch according to the processing number */
	switch( nProgress )
	{
		/* Initialize first one frame */
		case OM_SC_FIRST:
			om_Init( data );
			break;

		/* Character selection */
		case OM_SC_SELECT:
			om_EvalPad( data );	/* Evaluate pad data */
			om_AnimChara( data );	/* Animation */
			om_CheckAButton( data ); /* When button A is pushed */
			break;

		/* When button A is pushed */
		case OM_SC_A_BUTTON:
			om_TurnOffCharacter( data );	/* No display of 4 characters */
			om_SetKimeCharacter( data );	/* Character display */
			om_audio_se_start(FX_SELECTIT);
			nAnimationCount = ANIMATION_WAIT;
			nProgress = OM_SC_ANIMATION_WAIT;
			break;

		/* Wait during animation */
		case OM_SC_ANIMATION_WAIT:
			om_AnimationWait( data );
			break;

		/* Animation end wait */
		case OM_SC_ANIMATION_END_WAIT:
			om_AnimationEndWait( data );
			break;

		/* End character selection */
		case OM_SC_END:
			nProgress = OM_SC_FIRST;
			om_program_seq_st.seq_no = OM_GAME_INIT;
			break;
	}

	/* Evaluate character */
	om_game_eval( st , um );

	/* Build display list */
	if( nPendingGfx < 1 )
	{
		if( glistp = gfxBegin() )
		{
			cur_scene.fill_col = GPACK_RGBA5551( 0 , 0 , 0 , 1 );	/* Black background */
			glistp = ndBuildSceneGfx( glistp , um );
			glistp = om_DrawBG( glistp );
			glistp = om_game_draw3d( glistp , st , um );
			gfxEnd( glistp );
			gfxFlush( NU_SC_SWAPBUFFER, NU_GFX_UCODE_F3DEX_NON );
		}
	}

	/* Updata pad data */
	pad = expadReadController();

	gfxWaitSync();
}



/*====== Initialization ======*/
/*
	--- Initialize the selection screen ---

	void om_Init( OM_CHARACTER_DATA *object );
	Parameter: OM_CHARACTER_DATA *object --- Object data
	Return value:  None 
*/
void om_Init( OM_CHARACTER_DATA *object )
{
	int i;
	OM_CHARACTER_ST *st;
	OM_CHARACTER_DATA *init;

	st = &om_character_st;
	nCharacterNumber = 0xff;
	nProgress = OM_SC_SELECT;
	/* Initialize animation flag */
	for( i = 0 ; i < CHARACTER_NUM ; i++ ) nAnimFlag[i] = 0;
	/* Initialize deform flag */
	init = object;
	for( i = 0 ; i < CHARACTER_NUM ; i++ )
	{
		init->deform_flg = 0;
		init = init->next;
	}
	/* Character display, no-display */
	om_SetCharacterDsp( st->data );
	/* Start cursor animation */
	om_com_character_globalresource_change( Cursor );
	ndAttachAnimation( Cursor->cur_instp->anim_root_instp , Cursor->cur_instp->instp , 1 , 0 , 0x1000 + Cursor->entry_number , 2 );
}



/*====== Set characters not used to no-display ======*/
/*
	---  Change dsp_status member of OM_CHARACTER_DATA  ---

	void om_SetCharacterDsp( OM_CHARACTER_DATA *object );
	Parameter: OM_CHARACTER_DATA *object --- Object data
	Return value : none
*/
void om_SetCharacterDsp( OM_CHARACTER_DATA *object )
{
	int i;

	/* No-display of character data up to CHARACTER_INDEX  */
	for( i = 0 ; i < CHARACTER_INDEX ; i++ )
	{
		object->dsp_status = 0;
		object = object->next;
	}

	/* Display CHARACTER_NUM number of characters */
	for( i = 0 ; i < CHARACTER_NUM ; i++ )
	{
		object->dsp_status = 1;
		object = object->next;
	}

	/* No display until end */
	while( object != NULL )
	{
		object->dsp_status = 0;
		object = object->next;
	}

	/* Display cursor */
	Cursor->dsp_status = 1;
}



/*====== Evaluate pad data ======*/
/*
	--- Change flags such as the animation flag based on pad data ---

	void om_EvalPad();
	Parameters :  none
	Return value:  none  
*/
void om_EvalPad( OM_CHARACTER_DATA *object )
{
	int i;
	int nPreChara = nCharacterNumber;	/* Previous character number */
	int nCurrentChara = nCharacterNumber;	/* Current character number */

	/* Clear trigger */
	for( i = 0 ; i < CHARACTER_NUM ; i++ ) nAnimFlag[i] &= ~ANIM_TRIG;

	/* Process left and right pad */
	if( pad->trigger & R_JPAD ) nCurrentChara++;
	if( pad->trigger & L_JPAD ) nCurrentChara--;
	if( nCurrentChara == 0xff ) nCurrentChara = 0;	/* To set ANIM_TRIG */
	nCurrentChara &= 0x03;								/* use range of 0 to 3 */
	nCharacterNumber = nCurrentChara;

	/* Animation trigger processing */
	if( nPreChara != nCurrentChara )
	{
		if( nPreChara != 0xff )
			nAnimFlag[nPreChara] |= ANIM_DIRECTION;		/* Set deform for one direction */
		nAnimFlag[nCurrentChara] |= ANIM_TRIG;			/* Startanimation */
		nAnimFlag[nCurrentChara] &= ~ANIM_DIRECTION;	/* Set direction flag to + */
	}

	/* Determine x-coordinates of cursor */
	for( i = 0 ; i < nCharacterNumber ; i++ ) object = object->next;
	Cursor->x = object->x + CursorOffset[nCharacterNumber];
}



/*====== Character animation ======*/
/*
	--- Animate the characters ---

	void om_AnimChara( OM_CHARACTER_DATA *object );
	Parameter : OM_CHARACTER_DATA --- Object data
	Return value:  none 
*/
void om_AnimChara( OM_CHARACTER_DATA *object )
{
	int i;

	for( i = 0 ; i < CHARACTER_NUM ; i++ )
	{
		/* Initial deform flag setting */
		object->deform_flg &= ~NDC_DEFORM_DIRECTION_TRIG;
		object->deform_flg |= NDC_DEFORM_STOP;
		/* If deform finished */
		if( object->deform_flg & NDC_DEFORM_END )
		{
			/* Wnen deform is in one direction, clear animation */
			if( object->deform_flg & NDC_DEFORM_DIRECTION )
			{
				if( object->deform_flg & NDC_DEFORM_ON )
				{
					om_com_character_globalresource_change( object );
					ndDeleteInstanceGID( object->cur_instp->dynvtx_root_instp, -1, 0x2000 + object->entry_number , 0x200f);
					ndDeleteInstanceGID( object->cur_instp->deform_root_instp, -1, 0x2000 + object->entry_number , 0x200f);
				}
				object->deform_flg &= ~NDC_DEFORM_END;
				object->deform_flg &= ~NDC_DEFORM_ON;
			}
		}

	/* Trigger when the direction differs from the previous direction */
		if( ((nAnimFlag[i] & ANIM_DIRECTION) && !(object->deform_flg & NDC_DEFORM_DIRECTION)) ||
			(!(nAnimFlag[i] & ANIM_DIRECTION) && (object->deform_flg & NDC_DEFORM_DIRECTION)) )
		{
			object->deform_flg ^= NDC_DEFORM_DIRECTION;
			object->deform_flg |= NDC_DEFORM_DIRECTION_TRIG;
			object->deform_flg &= ~NDC_DEFORM_END;
		}

		/* At start of deform */
		if( nAnimFlag[i] & ANIM_TRIG ) om_AttachDeform( object );

		object = object->next;
	}
}



/*====== Attach deform animation ======*/
/*
	--- Start deform animation ---

	void om_AttachDeform( OM_CHARACTER_DATA *object );
	Parameter : OM_CHARACTER_DATA *object --- Object data 
	Return value: none
*/
void om_AttachDeform( OM_CHARACTER_DATA *object )
{
	/* Break if during current deform */
	if( object->deform_flg & NDC_DEFORM_ON ) return;

	/* Set instance pointer */
	om_com_character_globalresource_change( object );
	dynvtx_root_instp = object->cur_instp->dynvtx_root_instp;
	deform_root_instp = object->cur_instp->deform_root_instp;
	/* Start deform */
	deform_flg = object->deform_flg;
	ndAttachDeform( object->cur_instp->instp , 0 , 0 , 0 , 0x2000 + object->entry_number , 0x06 );

	/* Set deform flag */
	object->deform_flg |= NDC_DEFORM_ON;
}



/*====== Check button A ======*/
/*	--- Set so that character selected if button A pressed ---

	void om_CheckAButton( OM_CHARACTER_DATA *object );
	Parameter: OM_CHARACTER_DATA *object --- Object data
	Return value:  none 
*/
void om_CheckAButton( OM_CHARACTER_DATA *object )
{
	int i;
	OM_CHARACTER_DATA *Dist;

	/* Check whether button A was pressed */
	if( pad->trigger & A_BUTTON )
	{
		/* Selected character */
		Dist = object;
		for( i = 0 ; i < nCharacterNumber ; i++ ) Dist = Dist->next;

		/* Ignore until deform of the selected character ends */
		if( Dist->deform_flg & NDC_DEFORM_END ) nProgress = OM_SC_A_BUTTON;
	}
}



/*====== No character display except for xxvictory posexx character. ======*/
/*
	--- Set dsp_status member of OM_CHARACTER_DATA to 0---

	void om_TurnOffCharacter( OM_CHARACTER_DATA *object );
	Parameter:  OM_CHARACTER_DATA *object --- Object data
	Return value:  none
*/
void om_TurnOffCharacter( OM_CHARACTER_DATA *object )
{
	int i;

	/* No display of 4 characters */
	for( i = 0 ; i < CHARACTER_NUM ; i++ )
	{
		om_com_character_globalresource_change( object );
		ndDeleteInstanceGID( object->cur_instp->dynvtx_root_instp, -1, 0x2000 + object->entry_number , 0x200f);
		ndDeleteInstanceGID( object->cur_instp->deform_root_instp, -1, 0x2000 + object->entry_number , 0x200f);
		object->dsp_status = 0;
		object = object->next;
	}
	/* No display of cursor */
	object->next->dsp_status = 0;
	om_com_character_globalresource_change( Cursor );
	ndDeleteInstanceGID( Cursor->cur_instp->anim_root_instp , -1 , 0x1000 + Cursor->entry_number , 0x100f );
}



/*====== Display selected characters ======*/
/*
		--- Display the xxvictory posexx character at the position of 
		the selected character ---

	void om_SetKimeCharacter( OM_CHARACTER_DATA *object );
	Parameter:  OM_CHARACTER_DATA *object --- Object data
	Return value:  none
*/
void om_SetKimeCharacter( OM_CHARACTER_DATA *object )
{
	int i;
	OM_CHARACTER_DATA *Src;
	OM_CHARACTER_DATA *Dist;

	/* Selected character */
	Src = object;
	for( i = 0 ; i < nCharacterNumber ; i++ ) Src = Src->next;
	/* Character to display */
	Dist = object;
	for( i = 0 ; i < CHARACTER_NUM ; i++ ) Dist = Dist->next;

	/* Copy coordinates */
	Dist->x = Src->x;
	Dist->y = Src->y;
	Dist->z = Src->z;

	/* Display */
	Dist->dsp_status = 1;

	/* Animation */
	om_com_character_globalresource_change( Dist );
	ndAttachAnimation( Dist->cur_instp->anim_root_instp , Dist->cur_instp->instp , 1 , 0 , 0x1000 + Dist->entry_number , 2 );
}



/*====== Animation wait ======*/
/*	
	--- Wait for end of xxvictory posexx by the selected character ---

	void om_AnimationWait();
	Parameter:  OM_CHARACTER_DATA --- Object data
	Return value:  none
*/
void om_AnimationWait( OM_CHARACTER_DATA *object )
{
	int i;
	OM_CHARACTER_DATA *Dist;

	/* Character to display */
	Dist = object;
	for( i = 0 ; i < CHARACTER_NUM ; i++ ) Dist = Dist->next;

	/* Chekc animation counter */
	if( nAnimationCount > 0 ) nAnimationCount--;
	else
	{
		om_com_character_globalresource_change( Dist );
		ndDeleteInstanceGID( Dist->cur_instp->anim_root_instp , -1 , 0x1000 + Dist->entry_number , 0x100f );
		nProgress = OM_SC_ANIMATION_END_WAIT;
		nAnimationCount = ANIMATION_END_WAIT;
	}
}



/*====== Animation end wait ======*/
/*
		--- Wait following xxvictory posexx ---

	void om_AnimationEndWait();
	Parameter:  OM_CHARACTER_DATA --- Object data
	Return value:  none
*/
void om_AnimationEndWait( OM_CHARACTER_DATA *object )
{
	int i;
	OM_CHARACTER_DATA *Dist;

	/* Character to display */
	Dist = object;
	for( i = 0 ; i < CHARACTER_NUM ; i++ ) Dist = Dist->next;

	/* Check animation counter */
	if( nAnimationCount > 0 ) nAnimationCount--;
	else
	{
		Dist->dsp_status = 0;			/* No display of victory pose */
		nProgress = OM_SC_END;
	}
}



/*====== Display background ======*/
/*
	--- Display background xxwith spritesxx ---

	Gfx *om_DrawBG( Gfx *glistp );
	Parameter:  Gfx *glistp --- Pointer to the display list 
	Return value:  Pointer to the tail end of the display list 
*/
Gfx *om_DrawBG( Gfx *glistp )
{
	Gfx *tmp;

	/* Build the display list for sprite display */
	spInit( &glistp );
	sp_SelectBG_sprite.rsp_dl_next   = sp_SelectBG_sprite.rsp_dl;
	spScissor( 0 , 319 , 0 , 239 );
	spMove( &sp_SelectBG_sprite , 0 , 0 );
	spScale( &sp_SelectBG_sprite , 1.0 , 1.0 );
	spSetAttribute( &sp_SelectBG_sprite , SP_FASTCOPY );
	tmp = spDraw( &sp_SelectBG_sprite );
	spFinish( &tmp );
	gSPDisplayList( glistp++ , tmp );

	/* Restore sprite perspective to original */
	gDPSetTexturePersp( glistp++ , G_TP_PERSP );

	return glistp;
}



