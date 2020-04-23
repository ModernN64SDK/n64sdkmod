
/*---------------------------------------------------------------------*
        Copyright (C) 1997 Nintendo. (Originated by SGI)
        
        $RCSfile: font.c,v $
        $Revision: 1.5 $
        $Date: 1998/12/17 06:59:50 $
 *---------------------------------------------------------------------*/
/*
	Example of FONTS
*/

#include <ultra64.h>
#include <PR/mbi.h>
#include <PR/sp.h>
#include "font.h"

#include "hdd.h"

void jtext_sprite( Sprite *txt, u16 mes );
void transkan(int code, u8 *ptr);
s32  _first = 1;

OSPiHandle	*handler;

#include "../Texture/letters_img.h"

static char letters_string[] = "abcdefhiuvsnorm.klxz4321567890`!gjpqytw,EFGHKLNOPRSUVXZ@JM:WIYABCD'T\"+-=Q_/?()[]";

static Bitmap letters_bms[] = {
    { 8, 64, 0*8, 0*12, letters_img,   12,0 },
    { 8, 64, 1*8, 0*12, letters_img,   12,0 },
    { 8, 64, 2*8, 0*12, letters_img,   12,0 },
    { 8, 64, 3*8, 0*12, letters_img,   12,0 },
    { 8, 64, 4*8, 0*12, letters_img,   12,0 },
    { 8, 64, 5*8, 0*12, letters_img,   12,0 },
    { 8, 64, 6*8, 0*12, letters_img,   12,0 },
    { 7, 64, 7*8, 0*12, letters_img,   12,0 },

    { 8, 64, 0*8, 1*12, letters_img,   12,0 },
    { 8, 64, 1*8, 1*12, letters_img,   12,0 },
    { 8, 64, 2*8, 1*12, letters_img,   12,0 },
    { 8, 64, 3*8, 1*12, letters_img,   12,0 },
    { 8, 64, 4*8, 1*12, letters_img,   12,0 },
    { 8, 64, 5*8, 1*12, letters_img,   12,0 },
    { 9, 64, 6*8, 1*12, letters_img,   12,0 },
    { 6, 64, 7*8+1, 1*12, letters_img,   12,0 },

    { 8, 64, 0*8, 2*12, letters_img,   12,0 },
    { 8, 64, 1*8, 2*12, letters_img,   12,0 },
    { 8, 64, 2*8, 2*12, letters_img,   12,0 },
    { 8, 64, 3*8, 2*12, letters_img,   12,0 },
    { 8, 64, 4*8, 2*12, letters_img,   12,0 },
    { 8, 64, 5*8, 2*12, letters_img,   12,0 },
    { 8, 64, 6*8, 2*12, letters_img,   12,0 },
    { 8, 64, 7*8, 2*12, letters_img,   12,0 },

    { 8, 64, 0*8, 3*12, letters_img,   12,0 },
    { 8, 64, 1*8, 3*12, letters_img,   12,0 },
    { 8, 64, 2*8, 3*12, letters_img,   12,0 },
    { 8, 64, 3*8, 3*12, letters_img,   12,0 },
    { 8, 64, 4*8, 3*12, letters_img,   12,0 },
    { 8, 64, 5*8, 3*12, letters_img,   12,0 },
    { 7, 64, 6*8, 3*12, letters_img,   12,0 },
    { 7, 64, 7*8, 3*12, letters_img,   12,0 },

    { 8, 64, 0*8, 4*12, letters_img,   16,0 },
    { 8, 64, 1*8, 4*12, letters_img,   16,0 },
    { 8, 64, 2*8, 4*12, letters_img,   16,0 },
    { 8, 64, 3*8, 4*12, letters_img,   16,0 },
    { 8, 64, 4*8, 4*12, letters_img,   16,0 },
    { 8, 64, 5*8, 4*12, letters_img,   16,0 },
    { 9, 64, 6*8, 4*12, letters_img,   16,0 },
    { 6, 64, 7*8+1, 4*12, letters_img,   16,0 },

    { 8, 64, 0*8, 64+0*12, letters_img,   12,0 },
    { 8, 64, 1*8, 64+0*12, letters_img,   12,0 },
    { 8, 64, 2*8, 64+0*12, letters_img,   12,0 },
    { 8, 64, 3*8, 64+0*12, letters_img,   12,0 },
    { 8, 64, 4*8, 64+0*12, letters_img,   12,0 },
    { 8, 64, 5*8, 64+0*12, letters_img,   12,0 },
    { 8, 64, 6*8, 64+0*12, letters_img,   12,0 },
    { 8, 64, 7*8, 64+0*12, letters_img,   12,0 },

    { 8, 64, 0*8, 64+1*12, letters_img,   12,0 },
    { 8, 64, 1*8, 64+1*12, letters_img,   12,0 },
    { 8, 64, 2*8, 64+1*12, letters_img,   12,0 },
    { 8, 64, 3*8, 64+1*12, letters_img,   12,0 },
    { 8, 64, 4*8, 64+1*12, letters_img,   12,0 },
    { 8, 64, 5*8, 64+1*12, letters_img,   12,0 },
    { 8, 64, 6*8, 64+1*12, letters_img,   12,0 },
    { 8, 64, 7*8, 64+1*12, letters_img,   12,0 },

    { 7, 64, 0*8, 64+2*12, letters_img,   12,0 },
    { 9, 64, 1*8-1, 64+2*12, letters_img,   12,0 },
    { 6, 64, 2*8, 64+2*12, letters_img,   12,0 },
    { 9, 64, 3*8-1, 64+2*12, letters_img,   12,0 },
    { 7, 64, 4*8, 64+2*12, letters_img,   12,0 },
    { 9, 64, 5*8-1, 64+2*12, letters_img,   12,0 },
    { 8, 64, 6*8, 64+2*12, letters_img,   12,0 },
    { 8, 64, 7*8, 64+2*12, letters_img,   12,0 },

    { 8, 64, 0*8, 64+3*12, letters_img,   12,0 },
    { 8, 64, 1*8, 64+3*12, letters_img,   12,0 },
    { 7, 64, 2*8, 64+3*12, letters_img,   12,0 },
    { 9, 64, 3*8-1, 64+3*12, letters_img,   12,0 },
    { 7, 64, 4*8, 64+3*12, letters_img,   12,0 },
    { 9, 64, 5*8-1, 64+3*12, letters_img,   12,0 },
    { 8, 64, 6*8, 64+3*12, letters_img,   12,0 },
    { 8, 64, 7*8, 64+3*12, letters_img,   12,0 },

    { 8, 64, 0*8, 64+4*12+1, letters_img,   16,0 },
    { 8, 64, 1*8, 64+4*12+1, letters_img,   16,0 },
    { 8, 64, 2*8, 64+4*12+1, letters_img,   16,0 },
    { 8, 64, 3*8, 64+4*12+1, letters_img,   16,0 },
    { 8, 64, 4*8, 64+4*12+1, letters_img,   16,0 },
    { 8, 64, 5*8, 64+4*12+1, letters_img,   16,0 },
    { 8, 64, 6*8, 64+4*12+1, letters_img,   16,0 },
    { 8, 64, 7*8, 64+4*12+1, letters_img,   16,0 },

};



Font letters_font = { letters_string, letters_bms, letters_img };

char *my_strchr( char *, char );

void
text_sprite( Sprite *txt, char *str, Font *fnt, int xlen, int ylen )
{
    int i, ci;
    int x;
    int y;
    char *indx;
    Bitmap *bm;

    txt->width = xlen * 8;
    txt->height = ylen * 16;

    bm = txt->bitmap;

    i = 0;
    ci = 0;
    for(y=0; y<ylen; y++) {
	for(x=0; x<xlen; x++, i++, ci++) {
	    if( str[ci] == '\0' ) {
		bm[i]       = fnt->bitmaps[0];
		bm[i].width = -1;
		txt->nbitmaps = i;
		return;
	    };
	    if( str[ci] == '\n' ) {
		bm[i]      = fnt->bitmaps[0];
		bm[i].buf = NULL;
		ci--;
		continue;
	    };

	    if( (indx = my_strchr(fnt->index_string, str[ci])) != NULL ) {
		bm[i]      = fnt->bitmaps[indx-fnt->index_string];
	    } else {
		bm[i]      = fnt->bitmaps[0];
		bm[i].buf = NULL;
	    };
	};

	if( str[ci] == '\n' )
	    ci++;
    };

    txt->nbitmaps = i;

    return;
}

char *
my_strchr( char *str, char target )
{
    while( *str && (*str != target) )
	str++;
    
    if( *str )
	return( str );
    
    return( NULL );
}

#define DYN_SPRITE_HACK

#define NUM_template_BMS        (24*40)

static Bitmap template_bm[NUM_template_BMS];

static Gfx      template_dl[NUM_DL(NUM_template_BMS)];

Sprite template_sprite = {

        0,0,                    /* Position: x,y */

        0,0,                    /* Sprite size in texels (x,y) */

        1.0,1.0,                /* Sprite Scale: x,y */

        0,0,                    /* Explosion (x,y) */

        SP_TRANSPARENT,                   /* Sprite Attributes */
        0x1234,                 /* Sprite Depth: Z */

        255,255,255,255,        /* Sprite Coloration: RGBA */

        0,0,NULL,               /* Color LookUp Table: start_index, length, address */

        0,1,                    /* Sprite Bitmap index: start index, step increment */

        NUM_template_BMS,                       /* Number of bitmaps */
        NUM_DL(NUM_template_BMS),               /* Number of display list locations allocated */

        15, 128,                 /* Sprite Bitmap Height: Used_height, physical height */
        G_IM_FMT_I,             /* Sprite Bitmap Format */
        G_IM_SIZ_4b,            /* Sprite Bitmap Texel Size */

        template_bm,            /* Pointer to bitmaps */

        template_dl,            /* Display list memory */

        NULL,                  /* HACK: dynamic_dl pointer */

};


static unsigned char font_red = 255;
static unsigned char font_grn = 255;
static unsigned char font_blu = 255;
static unsigned char font_alf = 255;

static int font_xpos = 0;
static int font_ypos = 0;

static int font_win_width = 40;
static int font_win_height = 1;

static double font_xscale = 1.0;
static double font_yscale = 1.0;

/* Initialize Font local variables and prepare for drawing sprites */

void
font_init( Gfx **glistp )
{
    Gfx *gxp;

    gxp = *glistp;

    spInit( &gxp );

    template_sprite.rsp_dl_next = template_sprite.rsp_dl;

    *glistp = gxp;

    font_red = 255;
    font_grn = 255;
    font_blu = 255;
    font_alf = 255;

    font_xpos = 0;
    font_ypos = 0;

    font_win_width = 40;
    font_win_height = 1;

    font_xscale = 1.0;
    font_yscale = 1.0;

}


/* Call spFinish() to clean up when done using Sprites */

void
font_finish( Gfx **glistp )
{
    Gfx *gxp;

    gxp = *glistp;

    spFinish( &gxp );

    *glistp = (gxp-1);		/* Don't use final EndDisplayList() */

}



/* Set text window area (units are characters) */

void
font_set_win( int width, int height )
{
    font_win_width = width;
    font_win_height = height;
}

/* Set text window position (upper left corner) */

void
font_set_pos( int xpos, int ypos )
{
    font_xpos = xpos;
    font_ypos = ypos;
}


/* Set text size */

void
font_set_scale( double xscale, double yscale )
{
    font_xscale = xscale;
    font_yscale = yscale;
}


/* Set text color */

void
font_set_color( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha )
{
    font_red = red;
    font_grn = green;
    font_blu = blue;
    font_alf = alpha;
}

void
font_set_transparent( int flag )
{
    if( flag )
	spSetAttribute( &template_sprite, SP_TRANSPARENT | SP_CUTOUT );
    else
	spClearAttribute( &template_sprite, SP_TRANSPARENT | SP_CUTOUT );
}

/* Convert the string to a sprite with the propler bitmaps
   assembled from the basic font texture. */

void
font_show_string( Gfx **glistp, char *val_str )
{
  Sprite *sp;
  Gfx *gxp, *dl;

  gxp = *glistp;
  
  sp = &template_sprite;

  sp->width =  font_win_width*8  + 8;
  sp->height = font_win_height*16 + 8;
  sp->bmheight = 15;		/* add by mitu  */
  sp->bmHreal = 128;

  text_sprite( sp, val_str, &letters_font, font_win_width, font_win_height );

  spMove( sp, font_xpos, font_ypos );
  spColor( sp, font_red, font_grn, font_blu, font_alf );
  spScale( sp, font_xscale, font_yscale );

  dl = spDraw( sp );

  gSPDisplayList( gxp++, dl );

  *glistp = gxp;
}


void
jtext_sprite( Sprite *txt, u16 mes )
{
  int		i;
  int		addr = kaddr[mes];

  for( i = 0 ; message[mes][i*2] ; i ++, addr += 128 ) {
    template_bm[i].width = 16;
    template_bm[i].width_img = 16;
    template_bm[i].s = 0;
    template_bm[i].t = 0;
    template_bm[i].buf = &kbuffer[addr];
    template_bm[i].actualHeight = 16;
    template_bm[i].LUToffset = 0;
  }
  template_bm[i].width = -1;
  txt->nbitmaps = i;

  txt->width =  i*16;
  txt->height = 16;
}

void
jfont_show_string( Gfx **glistp, u16 mes )
{
  Sprite	*sp;
  Gfx		*gxp, *dl;

  if( _first )
	{
	  if( !dd_driveExist() ) return; 
	  setupkanji();
	  _first = 0;
	}

  gxp = *glistp;

  sp = &template_sprite;

  sp->width =  font_win_width*8  + 8;
  sp->height = font_win_height*16 + 8;

  sp->bmheight = 16;
  sp->bmHreal = 16;

  jtext_sprite( sp, mes );

#if 0				/* with Shade */
  spMove( sp, font_xpos+1, font_ypos+1 );
  spColor( sp, 0, 0, 0, font_alf );
  spScale( sp, font_xscale, font_yscale );

  dl = spDraw( sp );
  gSPDisplayList( gxp++, dl );
#endif

  spMove( sp, font_xpos, font_ypos );
  spColor( sp, font_red, font_grn, font_blu, font_alf );
  spScale( sp, font_xscale, font_yscale );

  dl = spDraw( sp );

  gSPDisplayList( gxp++, dl );

  *glistp = gxp;
}

#ifdef  EUC_CODE
int e2s(int code);
#endif

void
transkan(int code, u8 *ptr)
{
  int	i;

#ifdef EUC_CODE
  code = e2s(code);
#endif

  i = LeoGetKAdr(code);
  try_assert("get font error",(i>=0));

  dmaIOMessageBuf.hdr.pri 	= OS_MESG_PRI_NORMAL;
  dmaIOMessageBuf.hdr.retQueue 	= &dmaMessageQ;
  dmaIOMessageBuf.dramAddr 	= ptr;
  dmaIOMessageBuf.devAddr 	= DDROM_FONT_START + i;
  dmaIOMessageBuf.size		= 128;
	
  handler->transferInfo.cmdType	= OS_OTHERS;
  osEPiStartDma(handler, &dmaIOMessageBuf, OS_READ);
  osRecvMesg(&dmaMessageQ, NULL, OS_MESG_BLOCK);
}

#ifdef EUC_CODE
int 
e2s(int code)
{
  int c1,c2;
  
  c1 = (code >> 8) & 0xff;
  c2 = (code >> 0) & 0xff;

  if (c1 % 2)
	{
	  c1 = ( c1>>1 ) + ( c1 < 0xdf ? 0x31 : 0x71);
	  c2 -= 0x60 + (c2 < 0xe0);
    }
  else
	{
	  c1 = (c1>>1) + (c1 < 0xdf ? 0x30 : 0x70);
	  c2 -= 2;
    }

  return (int)( ((c1<<8) & 0xff00) + ( c2 & 0xff ) );
}
#endif


/* translate kanji-font in a message to buffer */

s32
initkanchr(u8 *str, s32 addr) {
  s16	i = 0;

  while ( str[i] != NULL ) {
    transkan( (int)( str[i] * 256 + str[i+1] ), (u8 *)&kbuffer[addr]);
    addr += 128;
    i += 2;
  }
  return addr;
}

s32
setupkanji(void)
{
  s32 i,kptr=0;
  
  handler = osDriveRomInit();

  for ( i = 0 ; i < ERROR_MES_SIZE ; i ++ )
	{
	  kaddr[i] = kptr;
	  kptr = initkanchr(message[i], kptr);
	} 

  return -1;
}
