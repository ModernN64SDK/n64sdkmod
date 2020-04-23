/*---------------------------------------------------------------------
	Copyright (C) 1997, Nintendo.
	
	File		rgb2u64.c
	Coded    by	Yoshitaka Yasumoto.	Mar 19, 1997.
	Modified by	
	Comments	
	
	$Id: rgb2u64.c,v 1.2 1997/08/07 03:24:19 yasu Exp $
  ---------------------------------------------------------------------*/
/*
 *	To compile:  cc	rgb2u64.c -o rgb2u64 -limage
 */
#include <stdio.h>
#include <stdlib.h>
#include <gl/image.h>

#define	GRGBA5551(r, g, b, a)	((r)<<11|(g)<<6|(b)<<1|(a))

int main(int argc,char *argv[])
{
  IMAGE *image;
  int	x, y, z, x0, y0;
  int	r, g, b;
  short *rbuf, *gbuf,	*bbuf;
  unsigned short	pixel;
  
  int	imgsize_x, imgsize_y;
  
  /* chack parameters */
  if (4 == argc){
    imgsize_x = atoi(argv[2]);
    imgsize_y = atoi(argv[3]);
  } else if (2 != argc){
    /* print usage message */
    fprintf(stderr,"usage: rgb2u64 infile [x_size y_size]\n");
    return 1;
  }
    
  /* open the image file */
  if ((image=iopen(argv[1],"r")) == NULL ){
    fprintf(stderr,"readimage: can't open input file %s\n",argv[1]);
    return 1;
  }
  
  /* print a little info about the image */
  fprintf(stderr,"Image x and y size     : %d  %d\n",image->xsize,image->ysize);
  fprintf(stderr,"Image zsize in channels: %d    \n",image->zsize);
  fprintf(stderr,"Image pixel min and max: %d  %d\n",image->min,image->max);

  if (2 == argc){
    imgsize_x = image->xsize;
    imgsize_y = image->ysize;
  }
  
  /* allocate buffers for image data */
  rbuf = (short *)malloc(image->xsize*sizeof(short));
  gbuf = (short *)malloc(image->xsize*sizeof(short));
  bbuf = (short *)malloc(image->xsize*sizeof(short));

  x0 = (image->xsize - imgsize_x) / 2;
  y0 = (image->ysize - imgsize_y) / 2;
  
  /* check to see if the image is B/W or RGB */
  if ( image->zsize == 1 ) {
    fprintf(stderr,"This is a black and write image. exiting...\n");
    return 1;
  } else if ( image->zsize >= 3 ){  /* if the image has	alpha zsize is 4 */
    fprintf(stderr,"This is a rgb image.\n");
    for (y = y0 + imgsize_y - 1; y >= y0; y--) {
      if (y < 0 || y >= image->ysize){
	pixel = GRGBA5551(0,0,0,1);
	for (x = 0; x < imgsize_x; x ++){
	  printf("%c%c", pixel>>8, pixel&0xff);
	}
      } else {
	getrow(image,rbuf,y,0);
	getrow(image,gbuf,y,1);
	getrow(image,bbuf,y,2);
	for (x = x0; x < x0 + imgsize_x; x ++){
	  if (x < 0 || x >= image->xsize){
	    pixel = GRGBA5551(0,0,0,1);
	  } else {
#if 0
	    r = (int)(((float)rbuf[x]) / 8.0f + 0.25f);
	    g = (int)(((float)gbuf[x]) / 8.0f + 0.25f);
	    b = (int)(((float)bbuf[x]) / 8.0f + 0.25f);
#else
	    r = rbuf[x] >> 3;
	    g = gbuf[x] >> 3;
	    b = bbuf[x] >> 3;	    
#endif
	    if (r > 31) r = 31;
	    if (g > 31) g = 31;
	    if (b > 31) b = 31;
	    pixel = GRGBA5551(r, g, b, 1);
	  }
	  printf("%c%c", pixel>>8, pixel&0xff);
	}
      }
    }
  }
  return 0;
}
/*======== End of rgb2u64.c ========*/
