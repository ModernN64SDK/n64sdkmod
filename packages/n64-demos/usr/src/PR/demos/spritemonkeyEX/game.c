/*
  game.c
    Copyright (C) 1999, NINTENDO Co,Ltd., MONEGI CORPORATION.
    Jan. 28, 1999.
*/

#include <ultra64.h>
#include <PR/gs2dex.h>

#include "Tile_Rect.h"
#include "Cont.h"

extern Rectangle dk_rect;
extern Rectangle dk7_rect;
extern Rectangle road_rect;
extern Rectangle l2tv_rect;

int	giDrawMode = 0;

static void add_scrollx_rect( Rectangle *rect );
static void add_scrolly_rect( Rectangle *rect );
static void sub_scrollx_rect( Rectangle *rect );
static void sub_scrolly_rect( Rectangle *rect );
static void add_sclx_rect( Rectangle *rect );
static void sub_sclx_rect( Rectangle *rect );
static void add_scly_rect( Rectangle *rect );
static void sub_scly_rect( Rectangle *rect );
static void add_sc_width_rect( Rectangle *rect );
static void sub_sc_width_rect( Rectangle *rect );
static void add_sc_hight_rect( Rectangle *rect );
static void sub_sc_hight_rect( Rectangle *rect );
static void add_scalex_rect( Rectangle *rect );
static void sub_scalex_rect( Rectangle *rect );
static void add_scaley_rect( Rectangle *rect );
static void sub_scaley_rect( Rectangle *rect );

#define _Memcpy(dest, src, size)	bcopy(src, dest, size)

static u16 gNewTexture = 0;

static u16 gZButtonPush = 0;
static Rectangle gRect;

void game_init( void );
Gfx *game_main( Gfx *glistp, Control *cont );

static void	SetMyTexture(void)
{
	static char	*pszFormat[] = {
		"RGBA", "YUV", "CI", "IA", "I"
	};
	static char	*pszSize[] = {
		"4b", "8b", "16b", "32b", "DD"
	};
	Rectangle	*pRect;

	switch(gNewTexture) {
    case 0:
		pRect = &dk_rect;
        break;
    case 1:
		pRect = &dk7_rect;
        break;
    case 2:
		pRect = &l2tv_rect;
		break;
	default:
		pRect = &road_rect;
        break;
    }
    _Memcpy(&gRect, pRect, sizeof(Rectangle));
#ifdef	_DEBUG
	/* print image information */
	osSyncPrintf("width = %d, height = %d, format = %s, size = %s\n",
		gRect.width,
		gRect.height,
		pszFormat[gRect.fmt],
		pszSize[gRect.size]);
#endif	/* _DEBUG */
}

void game_init( void )
{
	SetMyTexture();
}

Gfx *game_main( Gfx *glistp, Control *cont )
{
    /* Press down the Z button */
    if ( cont->trigger & Z_TRIG ) gZButtonPush ^= 1;

    /* Change drawing texture image */
    if ( cont->trigger & CONT_A )
    {
        gNewTexture += 1;
        if ( gNewTexture > 3 ) gNewTexture = 0;
		SetMyTexture();
    }

	/* Change BG drawing method */
    if ( cont->trigger & CONT_B ) {
		static char	*pszDrawMode[] = {
			"draw : gSPBgRect1Cyc\n",
			"draw : guS2DEmuBgRect1Cyc\n"
		};

    	giDrawMode++;
    	if(giDrawMode > 1) {
    		giDrawMode = 0;
    	}
#ifdef	_DEBUG
		osSyncPrintf(pszDrawMode[giDrawMode]);
#endif	/* _DEBUG */
    }
	/* reset parameter */
    if ( cont->trigger & CONT_START )
    {
#ifdef	_DEBUG
		osSyncPrintf("reset parameter\n");
#endif	/* _DEBUG */
		SetMyTexture();
    }
	
     gRect.flipy ^= 1;

    /* Flip T (inversion) */
    if ( cont->trigger & CONT_R ) gRect.flipy ^= 1;

    /* Flip S (inversion) */
    if ( cont->trigger & CONT_L ) gRect.flipx ^= 1;

    if ( gZButtonPush )
    {
        /* Z trigger is pressed down */
        /* Scroll control of Texture Image */
        if ( cont->lastbutton & L_JPAD ) add_scrollx_rect( &gRect );

        if ( cont->lastbutton & R_JPAD ) sub_scrollx_rect( &gRect );

        if ( cont->lastbutton & U_JPAD ) add_scrolly_rect( &gRect );

        if ( cont->lastbutton & D_JPAD ) sub_scrolly_rect( &gRect );
    } else {
        /* Texture Image scaling*/
        if ( cont->lastbutton & L_JPAD ) sub_scalex_rect( &gRect );

        if ( cont->lastbutton & R_JPAD ) add_scalex_rect( &gRect );

        if ( cont->lastbutton & U_JPAD ) sub_scaley_rect( &gRect );

        if ( cont->lastbutton & D_JPAD ) add_scaley_rect( &gRect );
    }

    /* Control C UNIT button */
    /* Change Texture Image size */
    if ( cont->lastbutton & L_CBUTTONS ) sub_sc_width_rect( &gRect );

    if ( cont->lastbutton & R_CBUTTONS ) add_sc_width_rect( &gRect );

    if ( cont->lastbutton & U_CBUTTONS ) sub_sc_hight_rect( &gRect );

    if ( cont->lastbutton & D_CBUTTONS ) add_sc_hight_rect( &gRect );

    /* Analogue stick control */
    /* Control Texture Rectangle display coordinates */
    if ( cont->stick_x < -3.0f ) sub_sclx_rect( &gRect );

    if ( cont->stick_x > 3.0f ) add_sclx_rect( &gRect );

    if ( cont->stick_y > 3.0f ) sub_scly_rect( &gRect );

    if ( cont->stick_y < -3.0f ) add_scly_rect( &gRect );

    /* Initialize RSP RDP */
    glistp = setup_rsprdp( glistp );
    /* Drawing Texture Tile*/
    glistp = tile_rectangle( glistp, &gRect );

    return glistp;
}

/*** Texture scroll control S ***/
static void add_scrollx_rect( Rectangle *rect )
{
	int	sc_max = rect->width - (int)(rect->sc_width / SCALE_VALUE(rect->scalex));

	if(rect->scrollx < sc_max) {
		u16	d = (u16)(2.0f * SCALE_VALUE(rect->scalex));

		rect->scrollx += (u16)((d > 0) ? d : 1);
	}
}

static void sub_scrollx_rect( Rectangle *rect )
{
    int	sc_d = (int)(2.0f * SCALE_VALUE(rect->scalex));

	if(sc_d < 1) sc_d = 1;
    rect->scrollx = (u16)((rect->scrollx > sc_d) ? rect->scrollx - sc_d : 0);
}

static void add_scrolly_rect( Rectangle *rect )
{
	int	sc_max = rect->height - (int)(rect->sc_height / SCALE_VALUE(rect->scaley));
	
	if(rect->scrolly < sc_max) {
		u16	d = (u16)(2.0f * SCALE_VALUE(rect->scaley));

		rect->scrolly += (u16)((d > 0) ? d : 1);
	}
}

static void sub_scrolly_rect( Rectangle *rect )
{
    int	sc_d = (int)(2.0f * SCALE_VALUE(rect->scaley));

	if(sc_d < 1) sc_d = 1;
    rect->scrolly = (u16)((rect->scrolly > sc_d) ? rect->scrolly - sc_d : 0);
}
/*** Texture scroll control E ***/

/*** Texture display position control S ***/
static void add_sclx_rect( Rectangle *rect )
{
    rect->sclx += 2;
}

static void sub_sclx_rect( Rectangle *rect )
{
    rect->sclx -= 2;
}

static void add_scly_rect( Rectangle *rect )
{
    rect->scly += 2;
}

static void sub_scly_rect( Rectangle *rect )
{
    rect->scly -= 2;
}
/*** Texture display position control E ***/

/*** Texture display (rectangle)size control S */
static void add_sc_width_rect( Rectangle *rect )
{
    float	fScMax = (u16)((float)(rect->width - rect->scrollx) * SCALE_VALUE(rect->scalex));
    
    if(rect->sc_width < fScMax) {
    	rect->sc_width += 2.0f;
    }
}

static void sub_sc_width_rect( Rectangle *rect )
{
    if ( rect->sc_width > 0.0f ) rect->sc_width -= 2.0f;
}

static void add_sc_hight_rect( Rectangle *rect )
{
    float	fScMax = (u16)((float)(rect->height - rect->scrolly) * SCALE_VALUE(rect->scaley));
    
    if(rect->sc_height < fScMax) {
    	rect->sc_height += 2.0f;
    }
}

static void sub_sc_hight_rect( Rectangle *rect )
{
    if ( rect->sc_height > 0.0f ) rect->sc_height -= 2.0f;
}
/*** Texture display (rectangle) size control E */

/*** Texture scaling control S ***/
static void add_scalex_rect( Rectangle *rect )
{
    if(rect->scalex < 6.0f) {
		float	fOldScw = rect->sc_width / rect->scalex;
    	rect->scalex += 0.05f;
		rect->sc_width = fOldScw * rect->scalex;
    }
}

static void sub_scalex_rect( Rectangle *rect )
{
    if(rect->scalex > 0.25f) {
		float	fOldScw = rect->sc_width / rect->scalex;
    	rect->scalex -= 0.05f;
		rect->sc_width = fOldScw * rect->scalex;
    }
}

static void add_scaley_rect( Rectangle *rect )
{
    if(rect->scaley < 6.0f) {
		float	fOldSch = rect->sc_height / rect->scaley;
    	rect->scaley += 0.05f;
		rect->sc_height = fOldSch * rect->scaley;
    }
}

static void sub_scaley_rect( Rectangle *rect )
{
    if(rect->scaley > 0.25f) {
		float	fOldSch = rect->sc_height / rect->scaley;
    	rect->scaley -= 0.05f;
		rect->sc_height = fOldSch * rect->scaley;
    }
}
/*** Texture scaling control E ***/

