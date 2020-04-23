/*
 * ppm2cTex.c -- Convert PPM texture file to C array for use as
 *               tiled and possible mipmapped texture.
 *
 * (c) 1995, Silicon Graphics
 *
 * Tiles which are produced overlap by one pixel on edges
 * Tiles are 33x33, 16-bit texture.
 * They can be mipmapped.
 *
 * Program input, via stdin is a .ppm texture file.
 * Program output, via stdout is a .c file.
 *
 * Use toppm to convert .rgb to .ppm
 *
 * Bugs:  Makes assumption that there is one and only one comment line in
 *        the .ppm file, which is not always true.
 *        Also, only works for binary format .ppm files.
 *
 */

#include <stdio.h>

/*
 * writeval -- Utility routine to write out a single texel
 *
 * fp = file pointer to write to
 * texture = array containing binary texture data
 * texel   = index into texture for R element to write.
 * x       = offset in tile
 * xw      = width of tile
 *
 */
void writeval(FILE *fp, unsigned char *texture, int texel, int x, int xw) {
  unsigned char  red,green,blue,alpha;
  unsigned short val;

  if (x >= xw) {fprintf(fp, "0,"); return;}
  
  red   = 0x1f & (texture[texel+0] >> 3);
  green = 0x1f & (texture[texel+1] >> 3);
  blue  = 0x1f & (texture[texel+2] >> 3);
  alpha = 1;
  val   = (red << 11) | (green << 6) | (blue << 1) | alpha;
  fprintf(fp,"0x%04x,",val);
} /* writeval */

/*
 * writeval_mm -- Utility routine to compute and write out a mipmapped
 *                texel
 *
 * fp = file pointer to write to
 * texture = array containing binary texture data
 * ybase = ybase in texture array for this tile
 * xbase = xbase in texture array for this tile
 * x = x offset in tile (in low-res coordinates!)
 * y = y offset in tile (in low-res coordinates!)
 * height, width = height and width of whole texture
 * mmlvl = mipmap level, 0 = full 33x33, 1 = 17x17, etc.
 * xw = width of texture at this mipmap level
 *      (causes a write of 0x0 if x >= xw, which can happen due to
 *       the way the swizzling loop which calls this routine works)
 */
writeval_mm(FILE *fp, unsigned char *texture, int ybase, int xbase,
	    int x, int y, int height, int width, int mmlvl, int xw) {
  int mx, my;
  int xs, ys, cnt;
  float rsum, bsum, gsum;
  int indx;
  unsigned short val;
  int red, green, blue, alpha;
  int xabs, yabs;
  float xweight, yweight;

  if (x >= xw) {fprintf(fp, "0,"); return;}

  xs = x << mmlvl;  /* x offset into tile in original texture coords */
  ys = y << mmlvl;  /* y offset into tile in original texture coords */
  cnt = 1 << (mmlvl-1); /* half-width of box filter */
  
  /* box filter */
  rsum = 0.0; gsum = 0.0; bsum = 0.0;
  for (mx = xs-cnt; mx <= xs+cnt; mx++) {
    xabs = xbase + mx;
    if (xabs < 0) xabs = 0;
    if (xabs > width-1) xabs = width-1;
    if ((mx != xs-cnt) && (mx != xs+cnt)) {
      xweight = 1.0;
    } else {
      xweight = 0.5;
    }
    for (my = ys-cnt; my <= ys+cnt; my++) {
      yabs = ybase + my;
      if (yabs < 0) yabs = 0;
      if (yabs > height-1) yabs = height-1;
      if ((my != ys-cnt) && (my != ys+cnt)) {
	yweight = 1.0;
      } else {
	yweight = 0.5;
      }

      indx = ((height-1-yabs)*width + xabs)*3;
      rsum += (0x1f & (texture[indx+0] >> 3))*xweight*yweight;
      gsum += (0x1f & (texture[indx+1] >> 3))*xweight*yweight;
      bsum += (0x1f & (texture[indx+2] >> 3))*xweight*yweight;
    }
  }
  red   = rsum / (float) (cnt*cnt*4);
  green = gsum / (float) (cnt*cnt*4);
  blue  = bsum / (float) (cnt*cnt*4);
  alpha = 1;

  val   = (red << 11) | (green << 6) | (blue << 1) | alpha;
  fprintf(fp,"0x%04x,",val);
} /* writeval_mm */

main(int argc, char *argv[]) {
  char           buf[256];
  int            width, height, num_cols, num_rows, icol, irow, x, y, texel;
  FILE          *fp = stdout;
  unsigned char *texture;
  int           mmflag = 0;  /* Do mip-mapping? */
  int           swflag = 0;  /* Do swizzling (produce Tmem image)? */
  int           c;
  int           texelcnt;
  int           mmlvl;
  int           tw;

  /* Check flags */
  while ((c = getopt(argc, argv, "msh")) != EOF) {
    switch (c) {
    case 'm':
      mmflag = 1;
      break;
    case 's':
      swflag = 1;
      fprintf(stderr, "Swizzing turned on\n");
      break;
    case '?':
    case 'h':
      fprintf(stderr, "usage: %s [-m] [-h] < blah.ppm > blah.c\n"
	              "       -m : mipmapped\n"
	              "       -s : swizzle words (Tmem image)\n"
	              "       -h : this help\n", argv[0]);
      exit(2);
    }
  } /* while */
  
  /* read in the ppm file header, currently only supports binary format,
   * and assumes one and only one comment line.
   */
  gets(buf);   /* P6 */
  gets(buf);   /* # ... */
  gets(buf);   /* W H */
  sscanf(buf," %i %i ",&width,&height);
  gets(buf);   /* 256 */
  fprintf(stderr, "PPM width = %i, height = %i\n", width, height);
  if (((width - 1) % 32 != 0) || ((height - 1) % 32 != 0)) {
    fprintf(stderr, "Width and/or Height not appropriate\n");
    exit(2);
  }
  
  num_cols = (width-1)  / 32;
  num_rows = (height-1) / 32;
  fprintf(stderr, "Width (in tiles) = %i, Height (in tiles) = %i\n",
	  num_cols, num_rows);
  
  /* now actually read in the texture data */
  texture = (unsigned char *)calloc(width*height,3);
  fread(texture, 3, width*height, stdin);
  
  /* write it back out */
  fprintf(fp,
     "static u64 padder; /* Start of texture must be 64 bit aligned */\n");
  
  if (mmflag) {
    /* All lower-res versions of texture go in 2nd '33*36' block after the
       full res block.  Some space is left vacant -- not maximally efficient */
    fprintf(fp,"unsigned short texture[%i][%i]={\n",num_rows*num_cols*2,33*36);
  } else {
    fprintf(fp,"unsigned short texture[%i][%i]={\n",num_rows*num_cols,33*33);
  }
  

  for (irow = 0; irow < num_rows; irow++) {
    fprintf(fp, "/* Row #%i */\n", irow);
    for (icol = 0; icol < num_cols; icol++) {
      fprintf(fp, "/* Column #%i */\n", icol);
      fprintf(fp,"{\n");
      for (y=0; y<33; y++) {
	fprintf(fp, "  ");
	for (x=0; x<36; x++) {
	  if ((!swflag) || ((y & 1) == 0)) {
	    /* Not swizzling (swflag off, or an even row) */
	    texel = ((height-1-(irow*32+y))*width + (icol*32+x))*3;
	    writeval(fp, texture, texel, x, 33);
	  } else {
	    /* It's an odd row, and we're swizzling, so here goes...
             * Do four texels, and adjust loop variable x appropriately */
	    x += 2;
	    texel = ((height-1-(irow*32+y))*width + (icol*32+x))*3;
	    writeval(fp, texture, texel, x, 33);
	    x += 1;
	    texel = ((height-1-(irow*32+y))*width + (icol*32+x))*3;
	    writeval(fp, texture, texel, x, 33);
	    x -= 3;
	    texel = ((height-1-(irow*32+y))*width + (icol*32+x))*3;
	    writeval(fp, texture, texel, x, 33);
	    x += 1;
	    texel = ((height-1-(irow*32+y))*width + (icol*32+x))*3;
	    writeval(fp, texture, texel, x, 33);
	    x += 2;
	  }
	}
	fprintf(fp, "\n");
      }
      fprintf(fp, "},\n\n");
      if (mmflag) {
	/* Lower-res versions */
	fprintf(fp,"{\n");
	texelcnt = 0;
	for (mmlvl = 1; mmlvl<6; mmlvl++) {
	  tw = (32 >> mmlvl) + 1;  /* Texture width */
	  for (y=0; y<tw; y++) {
	    fprintf(fp, "  ");
	    for (x=0; x<tw; x++) {
	      if ((!swflag) || ((y & 1) == 0)) {
		/* No swizzle */
		writeval_mm(fp, texture, irow*32, icol*32, x, y, height, width,
			    mmlvl, tw);
		texelcnt++;
	      } else {
		/* Swizzle -- do four texels and adjust loop var */
		x += 2;
		writeval_mm(fp, texture, irow*32, icol*32, x, y, height, width,
			    mmlvl, tw);
		x += 1;
		writeval_mm(fp, texture, irow*32, icol*32, x, y, height, width,
			    mmlvl, tw);
		x -= 3;
		writeval_mm(fp, texture, irow*32, icol*32, x, y, height, width,
			    mmlvl, tw);
		x += 1;
		writeval_mm(fp, texture, irow*32, icol*32, x, y, height, width,
			    mmlvl, tw);
		texelcnt += 4;
		x += 2;
	      }
	    }
	    /* Pad each row to 64-bit boundry */
	    while(texelcnt % 4 != 0) {
	      fprintf(fp, "0,");
	      texelcnt++;
	    }
	    fprintf(fp, "\n");
	  }
          fprintf(fp, "\n");
	}
	/* Pad out remainder of 33*36 'low-res' block with zeros
         * This is a waste of memory, but makes array accesses easy */
	for (;texelcnt<33*36;texelcnt++) {
	  fprintf(fp,"0,");
	}
	fprintf(fp, "\n");
	fprintf(fp, "},\n\n");
      }
    }
  }
  fprintf(fp,"};\n\n");
}




