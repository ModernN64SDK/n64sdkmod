/*======================================================================*/
/*	Mirror House							*/
/*	om_segment.h							*/
/*									*/
/*	Copyright (C) 1998 MONEGI CORPORATION				*/
/*									*/
/*======================================================================*/

/* ===========================================================================
	External reference declaration (segment address) section
 =========================================================================== */
extern char _select_bgSegmentStart[];
extern char _select_bgSegmentEnd[];
extern char _select_bgSegmentRomStart[];
extern char _select_bgSegmentRomEnd[];
extern char _select_bgSegmentDataStart[];
extern char _select_bgSegmentDataEnd[];
extern char _select_bgSegmentBssStart[];
extern char _select_bgSegmentBssEnd[];

extern char _openingSegmentStart[];
extern char _openingSegmentEnd[];
extern char _openingSegmentRomStart[];
extern char _openingSegmentRomEnd[];
extern char _openingSegmentDataStart[];
extern char _openingSegmentDataEnd[];
extern char _openingSegmentBssStart[];
extern char _openingSegmentBssEnd[];

extern char _logoimgsSegmentStart[];
extern char _logoimgsSegmentEnd[];
extern char _logoimgsSegmentRomStart[];
extern char _logoimgsSegmentRomEnd[];
extern char _logoimgsSegmentDataStart[];
extern char _logoimgsSegmentDataEnd[];

extern char _opimgsSegmentStart[];
extern char _opimgsSegmentEnd[];
extern char _opimgsSegmentRomStart[];
extern char _opimgsSegmentRomEnd[];
extern char _opimgsSegmentDataStart[];
extern char _opimgsSegmentDataEnd[];

#if 0
extern char _gameoverSegmentStart[];
extern char _gameoverSegmentEnd[];
extern char _gameoverSegmentRomStart[];
extern char _gameoverSegmentRomEnd[];
extern char _gameoverSegmentDataStart[];
extern char _gameoverSegmentDataEnd[];
#endif

/* ===========================================================================
	define section
 =========================================================================== */
enum {
    OM_SELECT_BG_SEGMENT  = 0,
    OM_OPENING_BG_SEGMENT = 2,
    OM_LOGOIMGS_SEGMENT   = 4,
    OM_OPIMGS_SEGMENT     = 6,
    OM_GAMEOVER_SEGMENT   = 8
};

