/*======================================================================*/
/*	Mirror House							*/
/*	om_sprite.c							*/
/*									*/
/*	Copyright (C) 1998 MONEGI CORPORATION				*/
/*									*/
/*======================================================================*/
#include "pack.h"

#include "nusys.h"

/* ===========================================================================
	External Reference Declarations 
 =========================================================================== */
extern Sprite opening_sprite;

/* ===========================================================================
	External Function Reference Declarations 
 =========================================================================== */
extern void om_opening_bg_data_read(void);

/* ===========================================================================
	Function Prototype Declarations 
 =========================================================================== */
Gfx *om_title_bg_draw(Gfx *glistp);

/* ===========================================================================
	Function Definitions 
 =========================================================================== */
Gfx *om_title_bg_draw(Gfx *glistp)
{
    /* Load opening BG data from ROM to RDRAM */
    om_opening_bg_data_read();

    spInit( &glistp );
    opening_sprite.rsp_dl_next   = opening_sprite.rsp_dl;
    spMove(&opening_sprite, 0, 0);
    spScale( &opening_sprite, 1.0, 1.0 );
    spSetAttribute( &opening_sprite, SP_FASTCOPY);
    spDraw(&opening_sprite);
    spFinish( &opening_sprite.rsp_dl_next );
    gSPDisplayList( glistp++ , opening_sprite.rsp_dl );
    return glistp;
}


