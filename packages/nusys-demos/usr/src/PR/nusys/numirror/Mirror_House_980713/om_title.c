/*======================================================================*/
/*	NIFFprev							*/
/*	om_title.c							*/
/*									*/
/*	Copyright (C) 1998, NINTENDO Co,Ltd.				*/
/*									*/
/*======================================================================*/
#include	<ultra64.h>
#include	"om_title.h"
#include	"nusys.h"
#include	"pack.h"
#include	"om.h"
#include	"om_audio.h"
#include	<PR/gs2dex.h>
#include	"om_segment.h"

/* ===========================================================================
	External References
 =========================================================================== */
extern	uObjTxSprite title_sp[];
extern	uObjTxSprite logo_sp[];
extern	u16	logo_white32[];
extern	u16	logo[];
extern	u16	opening[];
extern	uObjMtx	mt;
extern	OM_PROGRAM_SEQ_ST om_program_seq_st;
extern EXCONTPAD	*pad;

extern void om_pi_dma_data_read(u32 seg_no);

/* ===========================================================================
	Global Variable Declarations
 =========================================================================== */

static		u8	om_title_seq = DEF_TITLE_INIT;
static		s8	om_start_mode;
static		u16	om_title_time;
static		s16	om_title_opening_displaytime = 0;
static		u8	om_title_firsttime = 1;


	/*Change the values of Alpha */
static	u8	om_title_alpha[240] =
{
	255,254,253,251,249,247,244,240,236,232,
	227,221,214,207,200,192,184,175,165,154,
	143,132,120,107, 94, 80, 65, 48, 27,  0,

	  4,  8, 12, 15, 19, 22, 25, 28, 30, 32,
	 35, 38, 41, 43, 46, 48, 51, 53, 56, 58,
	 60, 62, 64, 66, 68, 70, 72, 74, 76, 78,

	 80, 82, 83, 85, 87, 88, 90, 92, 94, 95,
	 97, 99,100,102,104,105,107,109,111,112,
	114,116,118,120,122,123,125,126,127,129,

	130,132,133,135,136,137,139,140,142,143,
	145,147,148,149,150,151,153,154,155,156,
	158,159,160,162,163,164,165,166,167,169,

	170,171,172,173,174,175,176,177,178,179,
	180,181,182,183,183,184,185,186,187,188,
	189,190,191,192,193,194,194,195,196,197,

	198,199,200,201,201,202,203,204,204,205,
	206,207,207,208,209,210,210,211,212,212,
	213,214,214,215,216,217,217,218,219,219,

	220,221,222,222,223,224,224,225,225,226,
	227,227,228,229,229,230,231,231,232,233,
	233,234,235,235,236,237,237,238,239,239,

	240,240,241,241,242,242,243,243,244,244,
	245,245,246,246,247,247,248,248,249,249,
	250,250,251,251,252,252,253,253,254,254,
};

/* ===========================================================================
	Function Definitions
 =========================================================================== */
/*
 *	om_title_main()
 *
 *	Controls the title screen.  Calls appropriate functions according to
 *	the status of the title screen
 *
 */
void om_title_main( int pedingGfx )
{
	switch( om_title_seq )
	{
		case DEF_TITLE_INIT :
			om_title_init();
			break;
		case DEF_TITLE_LOGO_INIT :
			om_title_logo_init();
			break;
		case DEF_TITLE_LOGO :
			om_title_logo();
			break;
		case DEF_TITLE_MAIN :
			om_title_title();
			break;
		case DEF_TITLE_MAIN_INIT :
			om_title_title_init();
			break;
		case DEF_TITLE_END :
			om_title_end();
			break;
		default :
			om_title_seq = DEF_TITLE_INIT;
			break;
	}
	gfxWaitSync();
	pad = expadReadController();
}

/*
 *	om_title_init()
 *
 *	Initializes title screen.  Uses 1 frame to clear the title screen.
 *
 */
void om_title_init( void )
{
	Gfx *gp;
		/* Clear screen */
	if( gp = gfxBegin() )
	{
		gSPDisplayList(gp++ , om_title_setup_RCP_dl);
		gp = ndClearFB( gp , 0xffff , 0x0001 );
		gfxEnd(gp);
		gfxFlush(NU_SC_SWAPBUFFER, NU_GFX_UCODE_S2DEX);
	}

	om_title_time = 0;
	om_start_mode = 0;
	om_title_seq = DEF_TITLE_LOGO_INIT;
}

/*
 *	om_title_end()
 *
 *	Ends the title. Fades out the title screen.  If the start button is 
 *	pressed, allocates the next title processing function.  If the start 
 * 	button is not pressed, returns to the beginning of the title. Slight 
 *	restructuring of the program enables features such as movement for 
 *	demos.
 *
 */
void om_title_end( void )
{
	Gfx *gp;
	if( gp = gfxBegin() )
	{
		gSPDisplayList(gp++ , om_title_setup_RCP_dl);
		gp = ndClearFB( gp , 0xffff , 0x0001 );
		if( om_title_time<=60 )
		{
			gp = om_title_display_background( gp );
			gp = om_title_display_texture( gp , 0 , om_title_time*4 );
		}
		gfxEnd(gp);
		gfxFlush(NU_SC_SWAPBUFFER, NU_GFX_UCODE_S2DEX);
	}
	if( 90 <= ++om_title_time )
	{
		om_title_time = 0;
		om_title_seq = DEF_TITLE_INIT;
			/* Music stop */
		om_audio_seq_player_seq_stop(0);
		if( om_start_mode )
			om_program_seq_st.seq_no = OM_GAME_DATA_ENTRY;
	}
}

/*
 *	om_title_title_init()
 *
 *	Initialize the title screen. Initializes the variables, and fades in 
 *	the title screen
 *
 */
void om_title_title_init( void )
{
	Gfx *gp;
	if( om_title_firsttime )
	{
		om_pi_dma_data_read(OM_OPIMGS_SEGMENT);
		om_title_setup_background_parameters();
		om_title_firsttime = 0;
	}
	if( gp = gfxBegin() )
	{
		gSPDisplayList( gp++ , om_title_setup_RCP_dl );
		gp = ndClearFB( gp , 0xffff , 0xffff );
		if( 30 <= om_title_time )
		{
			gp = om_title_display_background( gp );
			gp = om_title_display_texture( gp , 255 , om_title_alpha[om_title_time-30]);
		}
		gfxEnd(gp);
		gfxFlush(NU_SC_SWAPBUFFER, NU_GFX_UCODE_S2DEX);
	}
	om_title_time++;
	if( 60 <= om_title_time )
	{
			/* Read sound data */
		om_audio_seq_bank_set(OM_TITLE_MUSIC);
		om_title_opening_displaytime = 0;
		om_title_time = 0;
		om_title_seq = DEF_TITLE_MAIN;
		om_title_firsttime = 1;
			/* Music start */
		om_audio_seq_player_play();
	}
}

/*
 *	om_title_title()
 *
 *	The main title screen function
 *
 */
void om_title_title( void )
{
	om_title_display();
	if( pad[0].trigger & CONT_START )
	{
		om_title_time = 0;
		om_title_seq = DEF_TITLE_END;
		om_start_mode = 1;
	}
	if( 3600 <= om_title_time )
	{
		om_title_time = 0;
		om_title_seq = DEF_TITLE_END;
	}
	om_title_time++;
}

/*
 *	om_title_display()
 *
 *	Renders the title screen
 *
 */
void om_title_display( void )
{
	Gfx	*gp;
	if( gp = gfxBegin() )
	{
		gp = om_title_display_background(gp);
		om_title_display_PushStart();
		gfxEnd(gp);
		gfxFlush(NU_SC_NOSWAPBUFFER, NU_GFX_UCODE_S2DEX);
	}
}

/*
 *	om_title_display_PushStart()
 *
 *		Alternates between blinking and streaming characters, 
 *		depending on the time used for the PushStartButton character 
 *		display subroutines.
 *	
 */
void om_title_display_PushStart( void )
{
		/* Adjust time */
	u8 timing = (u8)(((om_title_opening_displaytime & 0xfff8)>>3)&3);
	nuDebConClear( NU_DEB_CON_WINDOW0 );
	nuDebConClear( NU_DEB_CON_WINDOW1 );
	nuDebConClear( NU_DEB_CON_WINDOW2 );
	nuDebConClear( NU_DEB_CON_WINDOW3 );

	if( om_title_opening_displaytime <= 259 )
		om_title_stream_PushStart( timing );

	if(  260 <= om_title_opening_displaytime && 479 >= om_title_opening_displaytime )
		om_title_blink_PushStart( timing );

	if(  480 <= om_title_opening_displaytime && 684 >= om_title_opening_displaytime )
		om_title_stream_PushStart2( timing );

	if(  685 <= om_title_opening_displaytime && 983 >= om_title_opening_displaytime )
		om_title_stream_PushStart3( timing );

		/* Loop */
	if( ++om_title_opening_displaytime>=983)
		om_title_opening_displaytime = 50;
}

/*
 *	om_title_stream_PushStart()
 *
 *	Displays so that the PushStartButton characters stream horizontally.
 *
 */
void om_title_stream_PushStart( u8 Timing )
{
	char	PushStart0[18];
	char	PushStart1[18];
	char	PushStart2[18];
	char	PushStart3[18];
	nuDebConTextColor( Timing     , 0);
	nuDebConTextColor((Timing+1)&3, 4);
	nuDebConTextColor((Timing+2)&3,12);
	nuDebConTextColor((Timing+3)&3,15);
	sprintf( PushStart0 , "P       r   u   n" );
	sprintf( PushStart1 , " u   S   t   t   " );
	sprintf( PushStart2 , "  s   t       t  " );
	sprintf( PushStart3 , "   h   a   B   o " );
	nuDebConTextPos( 0 , DEF_PUSHSTART_XPOS , DEF_PUSHSTART_YPOS );
	nuDebConCPuts( 0 , PushStart0 );
	nuDebConTextPos( 1 , DEF_PUSHSTART_XPOS , DEF_PUSHSTART_YPOS );
	nuDebConCPuts( 1 , PushStart1 );
	nuDebConTextPos( 2 , DEF_PUSHSTART_XPOS , DEF_PUSHSTART_YPOS );
	nuDebConCPuts( 2 , PushStart2 );
	nuDebConTextPos( 3 , DEF_PUSHSTART_XPOS , DEF_PUSHSTART_YPOS );
	nuDebConCPuts( 3 , PushStart3 );
}

/*
 *	om_title_stream_PushStart2()
 *
 *	Displays so that the PushStartButton characters stream horizontally.
 *
 */
void om_title_stream_PushStart2( u8 Timing )
{
	char	PushStart0[18];
	char	PushStart1[18];
	char	PushStart2[18];
	char	PushStart3[18];
	nuDebConTextColor( Timing     , 0 );
	nuDebConTextColor((Timing+1)&3, 4 );
	nuDebConTextColor((Timing+2)&3,12 );
	nuDebConTextColor((Timing+3)&3,15 );
	sprintf( PushStart0 , "   h   a t   t   " );
	sprintf( PushStart1 , "  s   t       t  " );
	sprintf( PushStart2 , " u   S     B   o " );
	sprintf( PushStart3 , "P       r   u   n" );
	nuDebConTextPos( 0 , DEF_PUSHSTART_XPOS , DEF_PUSHSTART_YPOS );
	nuDebConCPuts( 0 , PushStart0 );
	nuDebConTextPos( 1 , DEF_PUSHSTART_XPOS , DEF_PUSHSTART_YPOS );
	nuDebConCPuts( 1 , PushStart1 );
	nuDebConTextPos( 2 , DEF_PUSHSTART_XPOS , DEF_PUSHSTART_YPOS );
	nuDebConCPuts( 2 , PushStart2 );
	nuDebConTextPos( 3 , DEF_PUSHSTART_XPOS , DEF_PUSHSTART_YPOS );
	nuDebConCPuts( 3 , PushStart3 );
}

/*
 *	om_title_stream_PushStart2()
 *
 *	Displays so that the PushStartButton characters stream horizontally.
 *
 */
void om_title_stream_PushStart3( u8 Timing )
{
	char	PushStart0[18];
	char	PushStart1[18];
	char	PushStart2[18];
	char	PushStart3[18];
	nuDebConTextColor( Timing     , 0 );
	nuDebConTextColor((Timing+1)&3, 4 );
	nuDebConTextColor((Timing+2)&3,12 );
	nuDebConTextColor((Timing+3)&3,15 );
	sprintf( PushStart0 , "Push             " );
	sprintf( PushStart1 , "     Start       " );
	sprintf( PushStart2 , "           Button" );
	sprintf( PushStart3 , "                 " );
	nuDebConTextPos( 0 , DEF_PUSHSTART_XPOS , DEF_PUSHSTART_YPOS );
	nuDebConCPuts( 0 , PushStart0 );
	nuDebConTextPos( 1 , DEF_PUSHSTART_XPOS , DEF_PUSHSTART_YPOS );
	nuDebConCPuts( 1 , PushStart1 );
	nuDebConTextPos( 2 , DEF_PUSHSTART_XPOS , DEF_PUSHSTART_YPOS );
	nuDebConCPuts( 2 , PushStart2 );
	nuDebConTextPos( 3 , DEF_PUSHSTART_XPOS , DEF_PUSHSTART_YPOS );
	nuDebConCPuts( 3 , PushStart3 );
}

/*
 *	om_title_blink_PushStart()
 *
 *	Causes the PushStartButton characters to blink.
 *
 */
void om_title_blink_PushStart( u8 Timing )
{
	char	TextColor[4] = { 15 , 12 , 4 , 0 };
	char	PushStart0[18];
	nuDebConTextColor( 3 , TextColor[Timing] );
	sprintf( PushStart0 , "Push Start Button" );
	nuDebConTextPos( 3 , DEF_PUSHSTART_XPOS , DEF_PUSHSTART_YPOS );
	nuDebConCPuts( 3 , PushStart0 );
}

/*
 *	om_title_display_background()
 *
 *	Displays the title screen background.
 *
 */
Gfx *om_title_display_background( Gfx *gp )
{
	int i=0;
	gSPDisplayList( gp++ , om_title_setup_RCP_dl );
	gp = ndClearFB( gp , 0xffff , 0x0001 );
	gDPSetRenderMode( gp++ , G_RM_OPA_SURF, G_RM_OPA_SURF2 );

	gDPPipeSync( gp++ );
	gDPSetPrimColor(gp ++, 0,0,255,255,255,128);

	gDPSetTextureFilter( gp++, G_TF_POINT );
	gDPSetCycleType( gp++, G_CYC_1CYCLE );
	gSPObjMatrix( gp++ , &mt );
	for( i=0 ; i<40 ; i++ )
	{
		gSPObjLoadTxRectR( gp++ , &(title_sp[i]) );
	}
	gSPObjMatrix( gp++ , &mt );
	for( i=0 ; i<40 ; i++ )
	{
		gSPObjLoadTxSprite( gp++ , &(title_sp[i]) );
	}
	return	gp;
}

/*
 *	om_title_setup_background_parameters()
 *
 *	Allocates the starting address of the sprite character data based on 
 *	the title screen bitmap.
 *
 */
void om_title_setup_background_parameters( void )
{
	int	i;
	for( i=0 ; i<35 ; i++ )
	{
		title_sp[i].txtr.block.image = (u64 *)(&(opening[64*32*i]));
	}
	for( i=35 ; i<40 ; i++ )
	{
		title_sp[i].txtr.block.image = (u64 *)(&(opening[64*32*35+(i-35)*64*16]));
	}
}


/*
 *	om_title_logo_init()
 *
 *	Initializes display of the Monegi logo on the title screen.
 *
 */
void om_title_logo_init( void )
{
	om_pi_dma_data_read(OM_LOGOIMGS_SEGMENT);
	om_title_seq = DEF_TITLE_LOGO;
	om_title_setup_logo_parameters();
}

/*
 *	om_title_logo()
 *
 *	The main logo display function.
 *
 */
void om_title_logo( void )
{
	om_title_logo_display();
	om_title_time++;
	if( pad[0].trigger & CONT_START || 240 <= om_title_time )
	{
		om_title_time = 0;
		om_title_seq = DEF_TITLE_MAIN_INIT;
	}
}

/*
 *	om_title_logo_display()
 *
 *	Displays the logo.  Implements fade-in and fade-out by rendering  
 *	dummy pure white sprites over the entire screen and pasting transparent  *	textures from the front. The structure used for the background is 
 *	borrowed and used as the structure for the pure white sprites. 
 *
 */
void om_title_logo_display( void )
{
	Gfx *gp;
	int i;
	u8 color;
		/* Render background */
	if( gp = gfxBegin() )
	{
		gSPDisplayList( gp++ , om_title_setup_RCP_dl );
		gp = ndClearFB( gp , 0xffff , 0x0001 );
		gDPPipeSync( gp++ );

		gDPSetTextureFilter( gp++, G_TF_POINT );
		gDPSetCycleType( gp++, G_CYC_1CYCLE );
		gSPObjMatrix( gp++ , &mt );
		for( i=0 ; i<40 ; i++ )
		{
			gSPObjLoadTxRectR( gp++ , &(title_sp[i]) );
		}
		gSPObjLoadTxRectR( gp++ , &(logo_sp[0]) );
		gSPObjLoadTxRectR( gp++ , &(logo_sp[1]) );
		gSPObjMatrix( gp++ , &mt );
		for( i=0 ; i<40 ; i++ )
		{
			gSPObjLoadTxSprite( gp++ , &(title_sp[i]) );
		}
		gSPObjLoadTxSprite( gp++ , &(logo_sp[0]) );
		gSPObjLoadTxSprite( gp++ , &(logo_sp[1]) );
		if( om_title_time <= 29 )
			color=0;
		else
			color=255;
		gp = om_title_display_texture( gp , color , om_title_alpha[om_title_time] );

		gfxEnd( gp );
		gfxFlush(NU_SC_SWAPBUFFER, NU_GFX_UCODE_S2DEX);
	}
}

/*
 *	om_title_setup_logo_parameters()
 *
 *	Initializes the starting address of the sprite data for the logo 
 *	display.
 *	The pure white sprites use the tiled background.
 *
 */
void om_title_setup_logo_parameters( void )
{
	int	i;
	logo_sp[0].txtr.block.image = (u64 *)(&(logo[0]));
	logo_sp[1].txtr.block.image = (u64 *)(&(logo[64*32]));
	for( i=0 ; i<40 ; i++ )
		title_sp[i].txtr.block.image = (u64 *)logo_white32;
}

/*
 *	om_title_display_texture()
 *
 *	Function that pastes the transparent textures.
 *
 */
Gfx *om_title_display_texture( Gfx *gp , u8 color , u8 alpha )
{
	gDPSetPrimColor(gp++,0,0,color,color,color,alpha);
	gSPDisplayList(gp++,om_title_texture_dl);
	return	gp;
}

