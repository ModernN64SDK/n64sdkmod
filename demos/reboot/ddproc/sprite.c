/*
 * Copyright 1995, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 *
 * UNPUBLISHED -- Rights reserved under the copyright laws of the United
 * States.   Use of a copyright notice is precautionary only and does not
 * imply publication or disclosure.
 *
 * U.S. GOVERNMENT RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to restrictions
 * as set forth in FAR 52.227.19(c)(2) or subparagraph (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS 252.227-7013 and/or
 * in similar or successor clauses in the FAR, or the DOD or NASA FAR
 * Supplement.  Contractor/manufacturer is Silicon Graphics, Inc.,
 * 2011 N. Shoreline Blvd. Mountain View, CA 94039-7311.
 *
 * THE CONTENT OF THIS WORK CONTAINS CONFIDENTIAL AND PROPRIETARY
 * INFORMATION OF SILICON GRAPHICS, INC. ANY DUPLICATION, MODIFICATION,
 * DISTRIBUTION, OR DISCLOSURE IN ANY FORM, IN WHOLE, OR IN PART, IS STRICTLY
 * PROHIBITED WITHOUT THE PRIOR EXPRESS WRITTEN PERMISSION OF SILICON
 * GRAPHICS, INC.
 */
/*---------------------------------------------------------------------*
        Copyright (C) 1995 Nintendo. (Originated by SGI)
        
        $RCSfile: sprite.c,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:17:01 $
 *---------------------------------------------------------------------*/

/*
   To display the built-in font for DD
   Modified sprite.c 

   G_IM_TRANSPARENT : Ignore the alpha specification for the sprite

   Unable to display the correct built-in font for the
   DD (English) due to the limited bit map width for the source
   with the current sprite.c.
   This is being fixed eagerly. */

#include "hsp.h"

#define SP_CUTOUT_ALPHA_LEVEL 0x80
#define G_CC_PRIMI       0,0,0,PRIMITIVE,0,PRIMITIVE,0,TEXEL0

/*
 * spSetZ()
 *
 */
void 
spSetZ (Sprite *sp, s32 z)
{
    sp->zdepth = (s16)z;
}

/*
 * spMove()
 *
 *   Moves a sprite on the screen.
 */
void 
spMove (Sprite *sp, s32 x, s32 y)
{
    sp->x = (s16)x;
    sp->y = (s16)y;
}

static s32 scissor_xmax;
static s32 scissor_ymax;

static s32 scissor_xmin;
static s32 scissor_ymin;

/*
 * spScissor()
 *
 *   Sets the scissoring box to be used for subsequent Sprite Drawing
 */
void 
spScissor (s32 xmin, s32 xmax, s32 ymin, s32 ymax )
{
    scissor_xmin = xmin;
    scissor_ymin = ymin;

    scissor_xmax = xmax;
    scissor_ymax = ymax;
}

static int *prev_bmbuf = NULL;

/*
 * drawbitmap()
 *
 *   Draws a bitmap from a sprite at a location on the screen.  If the
 *   "usevert" flag is set, then the routine will use vertices and triangles
 *   to draw the bitmap.  Otherwise, the texture rectangle instruction
 *   will be used.
 */

static void 
drawbitmap (Gfx **glp, Sprite *s, Bitmap *b,
	    s32 x, s32 y, s32 xx, s32 yy, s32 fs, s32 ft, s32 sx, s32 sy)
{
    s32 rs, rt;
    s32 rxh, ryh;
    s32 rxl, ryl;
    Gfx *gl;
    s32 tex_width, tex_height;
    Gtexrect gt, *g;
    s32 s_clamp, t_clamp,
	s_mask,  t_mask, 
	s_lod,   t_lod;

    g = &gt;

    tex_width = b->width_img;
    tex_height= s->bmHreal;
    
    gl = *glp;

    /* Scissoring */
    if( (x >= scissor_xmax) || (y >= scissor_ymax) ) {
	return;
    }

    if( (xx < scissor_xmin) || (yy < scissor_ymin) ) {
	return;
    }


	if (x < scissor_xmin) {
	    rxh = scissor_xmin*4;
	    rs = (b->s<<5) + fs + (((scissor_xmin - x)*sx)>>5);
	} else {
	    rxh = x*4;
	    rs = (b->s<<5) + fs;
	};

	if (y < scissor_ymin) {
	    ryh = scissor_ymin*4;
	    rt = (b->t<<5) + ft + (((scissor_ymin-y)*sy)>>5);
	} else {
	    ryh = y*4;
	    rt = (b->t<<5) + ft;
	}

	if( xx >= scissor_xmax ) {
	    rxl = scissor_xmax*4;
	} else {
	    rxl = xx*4;
	};

	if( yy >= scissor_ymax ) {
	    ryl = scissor_ymax*4;
	} else {
	    ryl = yy*4;
	};

	s_clamp = G_TX_CLAMP;	t_clamp = G_TX_CLAMP;
	s_mask  = G_TX_NOMASK;	t_mask  = G_TX_NOMASK;
	s_lod   = G_TX_NOLOD;	t_lod   = G_TX_NOLOD;

#define LOAD_TEX

#ifdef LOAD_TEX
	switch(s->bmsiz) {
	  case G_IM_SIZ_DD:
		gDPLoadTextureTile_4b(gl++, b->buf, s->bmfmt, tex_width, tex_height,
							  b->s, b->t,
							  b->s + b->width,b->t + b->actualHeight,
							  0,
							  s_clamp, t_clamp,
							  s_mask,  t_mask, 
							  s_lod,   t_lod);
		break;
	  case G_IM_SIZ_4b:
	    if (s->attr & SP_TEXSHUF) {
		gDPLoadTextureBlock_4bS(gl++, b->buf, s->bmfmt,
				       tex_width, tex_height, 0, 
				       s_clamp, t_clamp,
				       s_mask,  t_mask, 
				       s_lod,   t_lod);
	    } else {
		gDPLoadTextureBlock_4b(gl++, b->buf, s->bmfmt,
				       tex_width, tex_height, 0, 
				       s_clamp, t_clamp,
				       s_mask,  t_mask, 
				       s_lod,   t_lod);
	    };
	    break;
	  case G_IM_SIZ_8b:
	    if (s->attr & SP_TEXSHUF) {
		gDPLoadTextureBlockS(gl++, b->buf, s->bmfmt, G_IM_SIZ_8b,
				    tex_width, tex_height, 0,
				       s_clamp, t_clamp,
				       s_mask,  t_mask, 
				       s_lod,   t_lod);
	    } else {
		gDPLoadTextureBlock(gl++, b->buf, s->bmfmt, G_IM_SIZ_8b,
				    tex_width, tex_height, 0,
				       s_clamp, t_clamp,
				       s_mask,  t_mask, 
				       s_lod,   t_lod);
	    };
	    break;
	  case G_IM_SIZ_16b:
	    if (s->bmfmt == G_IM_FMT_YUV )
		  {
			if (s->attr & SP_TEXSHUF)
			  {
				gDPLoadTextureBlockYuvS(gl++, b->buf, s->bmfmt, G_IM_SIZ_16b,
									  tex_width, tex_height, 0,
									  s_clamp, t_clamp,
									  s_mask,  t_mask, 
									  s_lod,   t_lod);
			  }
			else
			  {
				if( b->LUToffset != 0 ) 
				  { /* Split Y and UV areas */
					unsigned char *uv, *addr;
					int j;
					s32 tmem, siz;
					
					gDPSetTextureImage( gl++, G_IM_FMT_I, G_IM_SIZ_8b, 1, b->buf);
					gDPSetTile( gl++, G_IM_FMT_I, G_IM_SIZ_8b, 0, 256, G_TX_LOADTILE, 0,
							   t_clamp, t_mask, t_lod,
							   s_clamp, s_mask, s_lod );
					gDPLoadSync( gl++ );
					gDPLoadBlock( gl++, G_TX_LOADTILE, 0,0, tex_width*tex_height -1,
								 CALC_DXT(tex_width,1) );
					gDPLoadSync( gl++ );

					uv = ((unsigned char *)b->buf) +
					  ((tex_width*tex_height)/2)*b->LUToffset;
					
					for( j=0; j<=(tex_height/2); j++ ) {
					  addr = uv + ((tex_height/2) -2 -j)*tex_width;
					  siz = 3*tex_width;
					  tmem = (tex_width/8)*(tex_height - 2 - 2*j);
					  if( tmem < 0 ) 
						{
						  addr += (-tmem)*8;
						  siz -= (-tmem)*8;
						  tmem = 0;
						};
					  if( j == 0 )
						siz = 2*tex_width;
					  
					  gDPSetTextureImage( gl++, G_IM_FMT_I, G_IM_SIZ_8b, 1, addr);
					  gDPSetTile( gl++, G_IM_FMT_I, G_IM_SIZ_8b, 0, tmem,
								 G_TX_LOADTILE, 0,
								 t_clamp, t_mask, t_lod,
								 s_clamp, s_mask, s_lod );
					  gDPLoadSync( gl++ );
					  gDPLoadBlock( gl++, G_TX_LOADTILE, 0,0, siz -1,
								   CALC_DXT(tex_width,1) );
					  gDPLoadSync( gl++ );
					};

					gDPSetTile( gl++, s->bmfmt, G_IM_SIZ_16b, (((tex_width) * 1)+7)>>3, 0,
							   G_TX_RENDERTILE, 0,
							   t_clamp, t_mask, t_lod,
							   s_clamp, s_mask, s_lod );
					gDPSetTileSize( gl++, G_TX_RENDERTILE, 0, 0,
								   ((tex_width)-1) << G_TEXTURE_IMAGE_FRAC,
								   ((tex_height)-1) << G_TEXTURE_IMAGE_FRAC );

				  }
				else /*  b->LUToffset != 0 */
				  {
					gDPLoadTextureBlockYuv(gl++, b->buf, s->bmfmt, G_IM_SIZ_16b,
										   tex_width, tex_height, 0,
										   s_clamp, t_clamp,
										   s_mask,  t_mask, 
										   s_lod,   t_lod);
				  };
			  };
		  } 
		else /*  not G_IM_FMT_YUV */
		  {
			if (s->attr & SP_TEXSHUF)
			  {
				gDPLoadTextureBlockS(gl++, b->buf, s->bmfmt, G_IM_SIZ_16b,
									 tex_width, tex_height, 0,
									 s_clamp, t_clamp,
									 s_mask,  t_mask, 
									 s_lod,   t_lod);
			  }
			else
			  {
				gDPLoadTextureBlock(gl++, b->buf, s->bmfmt, G_IM_SIZ_16b,
									tex_width, tex_height, 0,
									s_clamp, t_clamp,
									s_mask,  t_mask, 
									s_lod,   t_lod);
			  };
		  };
	    break;
	  case G_IM_SIZ_32b:
	    if (s->attr & SP_TEXSHUF) {
		gDPLoadTextureBlockS(gl++, b->buf, s->bmfmt, G_IM_SIZ_32b,
				    tex_width, tex_height, 0,
				       s_clamp, t_clamp,
				       s_mask,  t_mask, 
				       s_lod,   t_lod);
	    } else {
		gDPLoadTextureBlock(gl++, b->buf, s->bmfmt, G_IM_SIZ_32b,
				    tex_width, tex_height, 0,
				       s_clamp, t_clamp,
				       s_mask,  t_mask, 
				       s_lod,   t_lod);
	    };
	    break;
       }
       prev_bmbuf = b->buf;

#endif /* LOAD_TEX */


	g->cmd = G_TEXRECT;
	g->tile = 0;

	g->xh = rxh;
	g->xl = rxl;
	g->yh = ryh;
	g->yl = ryl;

	g->s = rs;
	g->t = rt;

	if ((s->attr & SP_FASTCOPY) && (s->bmfmt != G_IM_FMT_YUV) )
	    g->dsdx = sx * 4;
	else
	    g->dsdx = sx;

	g->dtdy = sy;

#ifdef rmDEBUG
    rmonPrintf("\trect (xh,l= %d,%d, yh,l= %d,%d, s,t= %d,%d, dsdx=%d, dtdy=%d )\n",
	    rxh,rxl,ryh,ryl,rs,rt,g->dsdx,sy );
#endif

    gSPTextureRectangle(gl++,g->xh,g->yh,g->xl,g->yl,g->tile,g->s,g->t,g->dsdx,g->dtdy);
    gDPPipeSync(gl++);

    *glp = gl;
}

static u16 sp_attr = 0;

void
spInit( Gfx **glistp )
{
    Gfx		*gl;

    gl = *glistp;

    sp_attr = 0;

    scissor_xmin = 0;
    scissor_ymin = 0;

    scissor_xmax = 320;
    scissor_ymax = 240;

    /* Turn on texturing */
	/*  RDP attribute  absolute must do before making any changes  */
    gDPPipeSync         ( gl++ );				
	/*  Considerably high quality pixel can be created by one cycle
    mode  */
    gDPSetCycleType     ( gl++, G_CYC_1CYCLE);  

	/*  gSPTexture(Gfx *gdl, int sc, int tc, int level, int tile, int on) */
	/*  sc,tc : The scaling parameter for the texture coordinate is  0.5 when format is 0.16 */
	/*  level : mip map level (0 because only one image and the mipmap is    not being used) */
	/*  tile  : texture tile number   The largest mipmap number used */
    gSPTexture          ( gl++, 0x8000, 0x8000, 0,  G_TX_RENDERTILE, G_ON);

    gDPSetAlphaCompare  ( gl++, G_AC_NONE   );

	/* Sprite will not be converted to transparent or semitransparent when it is drawn. Argument value is True/False. */
    gDPSetTexturePersp  ( gl++, G_TP_NONE   );

	/*   Bi-linear filtering is done from the four texel which surround the     object pixel */
	/*   G_TF_POINT, G_TF_AVERAGE, G_TF_BILERP */
    gDPSetTextureFilter ( gl++, G_TF_BILERP );

	/*   Filter process (G_TC_FILT) to be done to output */
	/*   (G_TC_CONV, G_TC_FILTCONV, G_TC_FILT) */
    gDPSetTextureConvert( gl++, G_TC_FILT   );

	/*  Use the highest level of detail texture inside of the multiple
    texture tiles */
	/*  ( G_TD_CLAMP, G_TD_SHARPEN, G_TD_DETAIL) */
    gDPSetTextureDetail ( gl++, G_TD_CLAMP  );

	/*   LOD( : Level of Detail   Automatic selection for mipmap ) Funtion
    turned off */
	/* Use the tile which was selected by gSPTexture (mipmap) and mipmap+1 */
	/*   ( G_TL_TILE, G_TL_LOD ) */
    gDPSetTextureLOD    ( gl++, G_TL_TILE );

	/*  TLUT (Texture look up table : color palette) will not be used. */
	/*  Corresponding color palette needs to be loaded in order to use TLUT  */
	/* (G_TT_NONE, G_TT_RGBA16, G_TT_IA16) */
    gDPSetTextureLUT    ( gl++, G_TT_NONE   );

#define SPRITE_SURF	G_RM_TEX_EDGE
#define SPRITE_SURF2	G_RM_TEX_EDGE2

	/*  Blender : Fuction to mix the pixel and the frame buffer. */
	/*  Use the drawing of semitransparent polygon, anti-alias processing,
    fog processing and dither processing. */
	/*  gDPSetRenderMode(Gfx *gdl,u32 mode1,u32 mode2 ) */
	/*  There is no meaning of specification of mode2 in 1cycle mode. */
    gDPSetRenderMode( gl++, G_RM_AA_OPA_SURF,G_RM_AA_OPA_SURF2 );

    *glistp = gl;
}


/*
 * spDraw()
 *
 *   Draws all of the sprites that are associated with bitmaps and that
 *   are supposed to be drawn (spDraw()).  They are drawn from 0 to maxsp,
 *   so sprites with higher numbers will overwrite lower-numbered sprites.
 *
 */
Gfx *
spDraw ( Sprite   *s )
{
    int		i;
    s32		x, y;
    float	sx, sy;
    Bitmap 	*b;
    Gfx		*gl;
    Gfx		*dl_start;
    s32		isx, isy;
    s32		tx, ty;
    s32		tx2, ty2;
    s32		x2, y2;
    float	ftx, fty;
    s32		fs, ft;
    s32		ex, ey;
	u16     attr_change , attr_on;
	u32     op0,op1;

#ifdef rmDEBUG
    rmonPrintf("spDraw (Sprite 0x%08x )\n", s );
#endif

    if (s->attr & SP_HIDDEN)
	return(NULL);

    prev_bmbuf = NULL;

    gl = s->rsp_dl_next;
    if( gl == NULL )
	gl = s->rsp_dl;
    dl_start = gl;

    b  = s->bitmap;
    ex = 0;
    ey = 0;

    if (s->attr & SP_EXTERN)	/* current attr is extern? */
	  sp_attr = s->attr;	/* Assume all settings should remain the same */
    else if (sp_attr & SP_EXTERN)/* previous attr was extern? */
	  sp_attr = ~s->attr;	/* Assume previous modes are all different */

	attr_change = s->attr ^ sp_attr;
	attr_on     = s->attr & attr_change;

    if ( attr_change )
	  {
		if ( attr_change & SP_TRANSPARENT )
		  {
#if 1
			op0 = ( attr_on & SP_TRANSPARENT ) ? G_RM_XLU_SURF : G_RM_AA_OPA_SURF;
			/* no meaning */
			op1 = ( attr_on & SP_TRANSPARENT ) ? G_RM_XLU_SURF2 : G_RM_AA_OPA_SURF2;

#else
			op0 = ( attr_on & SP_TRANSPARENT ) ? G_RM_AA_XLU_SURF : G_RM_AA_OPA_SURF;
			op1 = ( attr_on & SP_TRANSPARENT ) ? G_RM_AA_XLU_SURF2 : G_RM_AA_OPA_SURF2;
#endif
			gDPSetRenderMode( gl++, op0, op1);
		  }
		
		if ( attr_change & SP_CUTOUT )
		  {
			op0 = ( attr_on & SP_CUTOUT ) ? G_AC_THRESHOLD : G_AC_NONE;
			if(op0 == G_AC_THRESHOLD)
			  {
				/*  Set the threshold for alpha */
				gDPSetBlendColor ( gl++, 0xff, 0xff, 0xff, SP_CUTOUT_ALPHA_LEVEL);
			  }
			/*  On and off the round down mode below the
    alpha SP_CUTOUT_ALPHA_LEVEL  */
			gDPSetAlphaCompare ( gl++, op0 ) ;
		  }
		
		if ( attr_change & SP_TEXSHIFT )
		  {
			op0 = ( attr_on & SP_TEXSHIFT ) ? G_TF_AVERAGE :  G_TF_BILERP;
			gDPSetTextureFilter( gl++, op0 );
		  }
	  };

    sp_attr = s->attr;

    if( sp_attr & SP_Z )
	  {
		gDPSetPrimDepth(gl++, s->zdepth, 0);
	  };

	/*  Set the primitive color */
	/*  gDPSetPrimColor(Gfx *gdl, u32 m, u32 l, r,g,b,a) */
	/*  The smallest clump value for m: LOD<1.0 */
	/*  l: LOD fraction */
	/*  Set the color for the color combiner */

    gDPSetPrimColor( gl++, 0, 0, s->red, s->green, s->blue, s->alpha);

#define MY_K0   (175 & 0x1ff)
#define MY_K1   (-43 & 0x1ff)
#define MY_K2   (-89 & 0x1ff)
#define MY_K3   (222 & 0x1ff)
#define MY_K4   (114 & 0x1ff)
#define MY_K5   (42 & 0x1ff)

	switch(s->bmfmt)
	  {
	  case G_IM_FMT_I:
		if(s->attr & SP_TRANSPARENT)
		  {	gDPSetCombineMode ( gl++, G_CC_PRIMI , G_CC_PRIMI ); }
		else
		  {	gDPSetCombineMode ( gl++, G_CC_MODULATEI_PRIM , G_CC_MODULATEI_PRIM ); }
		break;

	  case G_IM_FMT_IA:  /*  intencity & alpha */
		/*  MODULATEIA_PRIM */
		/*  Both color and alpha : multiplication of texture and prim  */
		gDPSetCombineMode ( gl++, G_CC_MODULATEIA_PRIM  , G_CC_MODULATEIA_PRIM );
		break;

	  case G_IM_FMT_CI:  /*  color index  */
		/* G_CC_MODULATERGBA_PRIM (modulate rgba prim) same as MODULATEIA_PRIM */
		gDPSetCombineMode ( gl++, G_CC_MODULATERGBA_PRIM, G_CC_MODULATERGBA_PRIM );

		/*  The mode to be used for the color palette */
		/*  Palette color is RGBA16 or IA16. (RGBA16 in this case) */
		gDPSetTextureLUT( gl++, G_TT_RGBA16);  

		/*  Loading only the color palette */
		/*  Support only 256 palette in sp */
		gDPLoadTLUT_pal256( gl++, s->LUT );
		gDPLoadSync( gl++ );
		break;

	  case G_IM_FMT_YUV: /*  Brightness (Y) and color difference (UV) */
		gDPSetCombineMode ( gl++, G_CC_MODULATERGBA_PRIM, G_CC_MODULATERGBA_PRIM );

		if(s->attr & SP_FASTCOPY)
		  {
			gDPSetTextureConvert( gl++, G_TC_CONV);
			gDPSetTextureFilter( gl++, G_TF_POINT);
			gDPSetCombineMode( gl++, G_CC_1CYUV2RGB, G_CC_1CYUV2RGB);
			gDPSetConvert( gl++, MY_K0, MY_K1, MY_K2, MY_K3, MY_K4, MY_K5);
		  }
		else
		  {
			gDPSetCycleType( gl++, G_CYC_2CYCLE );
			gDPSetTextureFilter( gl++, G_TF_BILERP);
			gDPSetTextureConvert( gl++, G_TC_FILTCONV);
			gDPSetConvert( gl++, MY_K0, MY_K1, MY_K2, MY_K3, MY_K4, MY_K5);
			gDPSetCombineMode( gl++, G_CC_YUV2RGB, G_CC_PASS2);
		  }
		break;

	  case G_IM_FMT_RGBA:
		gDPSetCombineMode ( gl++, G_CC_MODULATERGBA_PRIM, G_CC_MODULATERGBA_PRIM );
		break;

	  default:					
		break;
	  }

    if (s->attr & SP_SCALE)
	  {
		sx = s->scalex;
		sy = s->scaley;
	  }
	else
	  {
		sx = 1.0F;
		sy = 1.0F;
	  }

    isx = (int) ((1<<10) / sx + 0.5F);
    isy = (int) ((1<<10) / sy + 0.5F);

    if( b )
	  {
		
		if( (s->attr & SP_FASTCOPY) && (s->bmfmt != G_IM_FMT_YUV) )
		  gDPSetCycleType( gl++, G_CYC_COPY);

		x = 0.0F;
		y = 0.0F;

		fty = s->y+y*sy;
		ty = (int) ( fty  + 0.99999F);
		ft = (int) (isy * (ty - fty));
		ft = (ft + 16)>>5;
		if( s->attr & SP_TEXSHIFT )
		  ft += 16;			/* 1/2 Texel for AntiAliasing */
		
		if( s->attr & SP_FRACPOS )
		  ft += s->frac_t;		/* Micro-positioning */
		
		y2 = y + s->bmheight;
		ty2 = (int) (s->y+y2*sy + 0.99999F);

		if( (s->attr & SP_FASTCOPY) && (s->bmfmt != G_IM_FMT_YUV) )
		  ty2--;
		

#ifdef rmDEBUG
    rmonPrintf("\tiscale=(%d,%d), %d bitmaps, sprite_size=(%d,%d)\n",
			   isx, isy, s->nbitmaps, s->width, s->height );
#endif

		for( i=0; (i < s->nbitmaps) && (b->width > 0); i++, b++ )
		  {
			if( (x+b->width) > s->width )
			  { /* Next bitmap hits edge? */
				int bh;

				x = 0;
				fs = 0;
				ex = 0;
				ey += s->expy;
				y += s->bmheight;	     /* Wrap to next line */
				y2 = y + s->bmheight;	     /* Wrap to next line */
				
				fty = s->y+y*sy;
				ty = (int) ( fty  + 0.9999F);
				ft = (int) (isy * (ty - fty));
				ft = (ft + 16)>>5;
				if( s->attr & SP_TEXSHIFT )
				  ft += 16;			/* 1/2 Texel for AntiAliasing */
				
				if( s->attr & SP_FRACPOS )
				  ft += s->frac_t;		/* Micro-positioning */

				ty2 = (int) (s->y+y2*sy + 0.9999F);

				ty  += ey;
				ty2 += ey;
				
				if( (s->attr & SP_FASTCOPY) && (s->bmfmt != G_IM_FMT_YUV) )
				  ty2--;

				/* Fill out to rect. bdy? */
				
				if( (b->actualHeight != 0) )
				  bh = b->actualHeight;
				else
				  bh = s->bmheight;
				
				if( (y + bh) > s->height ) /* Can't wrap any more? */
				  break;
			  };

#ifdef rmDEBUG
			rmonPrintf("bm# %d, x,y=(%d,%d) width=%d \n", i, x, y, b->width );
#endif
			
			ftx = s->x+x*sx;
			tx = (s32) (ftx + 0.9999F);
			fs = (s32) (isx * (tx - ftx));
			fs = (fs + 16)>>5;
			if( s->attr & SP_TEXSHIFT )
			  fs += 16;			/* 1/2 Texel for AntiAliasing */
			
			if( s->attr & SP_FRACPOS )
			  fs += s->frac_s;		/* Micro-positioning */
			
			x2 = x + b->width;
			tx2 = (int) (s->x+x2*sx + 0.9999F);
			
			if( (b->actualHeight != 0) )
			  {
				y2 = y + b->actualHeight;
				ty2 = (s32) (s->y+y2*sy + 0.9999F);
				ty2 += ey;
				
				if( (s->attr & SP_FASTCOPY) && (s->bmfmt != G_IM_FMT_YUV) )
				  ty2--;
			  };

#ifdef rmDEBUG
			rmonPrintf("\tfull(%g,%g) = int(%d,%d) + frac(%d,%d)\n",
					   ftx, fty, tx,ty, fs,ft );
#endif

			tx  += ex;
			tx2 += ex;

			if( (s->attr & SP_FASTCOPY) && (s->bmfmt != G_IM_FMT_YUV) )
			  tx2--;

			if( b->buf != NULL ) 	/* Skip over null bitmaps (blanks) */
			  drawbitmap ( &gl, s, b, tx, ty, tx2, ty2, fs, ft, isx, isy);

			x += b->width;
			ex += s->expx;
		  }

		if (s->attr & SP_FASTCOPY)
		  gDPSetCycleType( gl++, G_CYC_1CYCLE);
		
		if(s->bmfmt == G_IM_FMT_YUV)
		  {
			gDPSetCycleType( gl++, G_CYC_1CYCLE);
			gDPSetTextureFilter ( gl++, G_TF_BILERP );
			gDPSetTextureConvert( gl++, G_TC_FILT   );
		  };
		
	  }
	else /*  bitmap is not specified */
	  {
		int rgba;

		x = (s32)s->x;
		y = (s32)s->y;
		x2 = s->x + (s->width*sx)-1;
		y2 = s->y + (s->height*sy)-1;
		
		if( ( x >= scissor_xmax) || (y >= scissor_ymax) )
		  {
		  }
		else if( ( x2 < scissor_xmin) || (y2 < scissor_ymin) ) 
		  {
		  } 
		else
		  {
			
			if( x < scissor_xmin )
			  x = scissor_xmin;
			
			if( x2 >= scissor_xmax )
			  x2 = scissor_xmax - 1;
			
			if( y < scissor_ymin )
			  y = scissor_ymin;
			
			if( y2 >= scissor_ymax )
			  y2 = scissor_ymax - 1;

			rgba = GPACK_RGBA5551((s->red), (s->green), (s->blue), (s->alpha>>7));
			
			gSPTexture ( gl++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF);
			gDPSetCycleType( gl++, G_CYC_FILL);
			gDPSetFillColor( gl++, (rgba << 16) | (rgba) );
			
			gDPFillRectangle(  gl++, x,y, x2,y2 );
			gDPSetCycleType( gl++, G_CYC_1CYCLE);
			gSPTexture ( gl++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);
		  }
	  }
	
    if( s->bmfmt == G_IM_FMT_CI ) {
	  gDPSetTextureLUT( gl++, G_TT_NONE);
    };

    gSPEndDisplayList ( gl++ );

    s->rsp_dl_next   = gl;
    return(  dl_start );
}

void
spFinish( Gfx **glistp )
{
    Gfx *gl;

    gl = *glistp;

    /* Turn off texturing */
    gSPTexture ( gl++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF);
    gDPSetCombineMode ( gl++, G_CC_SHADE, G_CC_SHADE);

    if (sp_attr & SP_TRANSPARENT) {
	/* Turn off transparency */
	gDPSetRenderMode( gl++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    }

    if (sp_attr & SP_CUTOUT) {
	/* Turn off Alpha write enable */
	gDPSetAlphaCompare ( gl++, G_AC_NONE) ;
    }

    gSPEndDisplayList ( gl++ );

    *glistp = gl;
}

Gfx *
spFillRect(Gfx * dl ,s32 x,s32 y,s32 x2,s32 y2 ,u8 r,u8 g,u8 b,u8 a)
{
  gDPPipeSync         ( dl++ );	
  gDPSetCycleType( dl++, G_CYC_1CYCLE ); 
  gSPTexture ( dl++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF); 
  gDPSetAlphaCompare  ( dl++, G_AC_NONE   ); 
  gDPSetTexturePersp  ( dl++, G_TP_NONE   ); 
  gDPSetTextureFilter ( dl++, G_TF_BILERP ); 
  gDPSetTextureConvert( dl++, G_TC_FILT   ); 
  gDPSetTextureDetail ( dl++, G_TD_CLAMP  ); 
  gDPSetPrimColor( dl++, 0, 0, r, g, b, a); 
  gDPSetRenderMode( dl++, G_RM_XLU_SURF, G_RM_XLU_SURF2 ); 
  gDPSetCombineMode ( dl++, G_CC_PRIMITIVE , G_CC_PRIMITIVE ); 
  gSPTextureRectangle(dl++, x<<2, y<<2, x2<<2, y2<<2 ,G_TX_RENDERTILE, 0, 0, 0x400, 0x400);  
  gSPEndDisplayList ( dl++ ); 

  return dl;
}


#if 0

/*  Revision */

   16-color palette was judged to be more appropriate if handled with tools, 
 (convert to 256 palette when converting sprite data)
	and sprite structures nTLUT (16 or 256) and startTLUT (16 * paletteNo) will no longer be used.  Therefore, the bitmap structure LUToffset will not be used, either.

	cf. The state of previous usage
	
	gDPLoadTLUT( gl++, s->nTLUT, 256+s->startTLUT, s->LUT );
	
	/*  With 16-color palette, 16 palettes can be used by switching.    */
	/*  (In this case, the upper nibble of pixel will be a palette number.) */
	/*  nTLUT : 16 or 256, startTLUT : 16 * [Palette number] (0 only for 256 palette) */
	/*  LUT : Head palette address */
	
	Q. Why is 16-color palette not supported?
	
	A. Not frequently used. It does not save TMEM. It will reduce the trouble of sprite.c.

	TEXSHUF,CUTOUT,



#endif
