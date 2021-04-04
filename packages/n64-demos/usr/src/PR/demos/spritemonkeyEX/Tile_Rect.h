/*
  tile_rect.h
    Copyright (C) 1999, NINTENDO Co,Ltd., MONEGI CORPORATION.
    Jan. 28, 1999.
*/

#ifndef _Tile_Rect_h_
#define _Tile_Rect_h_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Rectangle {
    u16 width;      	/* Texture image width  */
    u16 height;      	/* Texture image height */
    u8	fmt;        	/* Texture image graphics format  */
    u8	size;       	/* Texture image depth size */
    u16 *timg;      	/* Texture image image data pointer */
    uObjTxtr *tlut; 	/* Texture image TLUT data pointer  */
    u16 flipx;      	/* Texture image X coordinate flip flag*/
    u16 flipy;      	/* Texture image Y coordinate flip flag */
    u16 scrollx;    	/* X coordinate scroll value of texture image */
    u16 scrolly;    	/* Y coordinate scroll value of texture image */
    s16 sclx;       	/* Screen coordinate upper left corner X origin */
    s16 scly;       	/* Screen coordinate upper left corner Y origin */
    float sc_width;   	/* Screen width   */
    float sc_height;   	/* Screen height */
    float scalex;   	/* X coordinate scale value*/
    float scaley;   	/* Y coordinate scale value */
} Rectangle;

extern Gfx *tile_rectangle( Gfx *, Rectangle * ); 
extern Gfx *setup_rsprdp( Gfx * );

#define	SCALE_VALUE(x)	(x / 4.0f)

extern int	giDrawMode;

#ifdef __cplusplus
}
#endif

#endif
























