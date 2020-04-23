/*
 * Convert medit format files to C code for use by morphfaces program
 *
 * (c) 1995 Silicon Graphics
 *
 * Author: Bill Mark, August 1995
 *
 *
 * Arrays and variables output into C program:
 *   'code' Segment
 *     tricnt              = # of triangles (for each image)
 *     trilist[3*tricnt]   = indicies to verts for triangle ([t]...[t+2] = tri)
 *     vface1[tricnt*3*4]  = (x,y,s,t) of verts for image #1
 *                           ([v]=x,[v+1]=y,[v+2]=s,[v+3]=t).
 *                           In the generated code it's actually an array
 *                           Vtx vface1[tricnt*3].
 *     vface2[tricnt*3*4]  = same for image #2
 *     tcoords1[tricnt][4] = sl,sh,tl,th for texture block loaded with tri.
 *                           [0] = lows, [1] = lowt, [2] = highs, [3] = hight
 *     tcoords2[tricnt][4] = same for image #2
 *     tindex1[tricnt*3]   = [t+0] = start index in texture[] for block
 *                           [t+1] = width of the block
 *                           [t+2] = height of the block
 *     tindex2[tricnt*3]   = same for image #2
 *
 *   'static' Segment
 *     texture[] = array containing all texture block data
 *
 */

#include <stdio.h>

/* Can increase these if too small -- sets triangle and vertex array sizes */
#define VMAX 600
#define TMAX 400

/* Maximum texture width and height */
/* These can't necessarily be changed without causing problems */
#define MAXTW 32
#define MAXTH 32

/*
 * stmaxmin -- utility routine for buildit
 */
void stmaxmin(int v0, int v1, int v2, int *verts, int offset,
	      int *stmin, int *stmax) {

    *stmin = verts[v0*4+offset];
    *stmax = verts[v0*4+offset];
    if (verts[v1*4+offset] > *stmax) {
      *stmax = verts[v1*4+offset];
    }
    if (verts[v1*4+offset] < *stmin) {
      *stmin = verts[v1*4+offset];
    }
    if (verts[v2*4+offset] > *stmax) {
      *stmax = verts[v2*4+offset];
    }
    if (verts[v2*4+offset] < *stmin) {
      *stmin = verts[v2*4+offset];
    }
} /* stmaxmin */

/*
 * buildit -- build arrays that will be later written to C output files
 *            mf = factor by which to multiply x and y coordinates
 *            Coordinates are centered around zero, then multiplied by mf
 *            The original coordinates were texel coordinates.
 */
void buildit(int *tris, int tricnt, int *verts,
	     char *i1, char *i2, int w, int h,
	     unsigned int texture[],
	     int *tasize,                   /* # of elements in texture[] */
	     int tindex1[], int tindex2[],
	     int tcoords1[][4], int tcoords2[][4],
	     int trilist[], int vface1[], int vface2[],
	     int mf) {

  int t;
  int v0, v1, v2;
  int smin, smax, tmin, tmax;
  int i,j;
  int r,g,b;
  int twidth, theight;

  *tasize = 0;  /* Reset to zero */

  for (t=0; t<tricnt; t++) {
    /* Vert indicies */
    v0 = tris[t*3];
    v1 = tris[t*3+1];
    v2 = tris[t*3+2];
    /* We don't share verts in the output, so each triangle gets three */
    trilist[t*3+0] = t*3+0; /* v0 */
    trilist[t*3+1] = t*3+1; /* v1 */
    trilist[t*3+2] = t*3+2; /* v2 */
    /*
     * Put in vertex info (vface1, vface2)
     */
    vface1[t*12+0+0] = (verts[v0*4+0]-w/2)*mf;     /* v0, x, f1 */
    vface1[t*12+0+1] = (h-1-verts[v0*4+1]-h/2)*mf; /* v0, y, f1 */
    vface2[t*12+0+0] = (verts[v0*4+2]-w/2)*mf;     /* v0, x, f2 */
    vface2[t*12+0+1] = (h-1-verts[v0*4+3]-h/2)*mf; /* v0, y, f2 */
    vface1[t*12+0+2] = verts[v0*4+0]; /* v0, s, f1 */
    vface1[t*12+0+3] = verts[v0*4+1]; /* v0, t, f1 */
    vface2[t*12+0+2] = verts[v0*4+2]; /* v0, s, f2 */
    vface2[t*12+0+3] = verts[v0*4+3]; /* v0, t, f2 */

    vface1[t*12+4+0] = (verts[v1*4+0]-w/2)*mf;     /* v1, x, f1 */
    vface1[t*12+4+1] = (h-1-verts[v1*4+1]-h/2)*mf; /* v1, y, f1 */
    vface2[t*12+4+0] = (verts[v1*4+2]-w/2)*mf;     /* v1, x, f2 */
    vface2[t*12+4+1] = (h-1-verts[v1*4+3]-h/2)*mf; /* v1, y, f2 */
    vface1[t*12+4+2] = verts[v1*4+0]; /* v1, s, f1 */
    vface1[t*12+4+3] = verts[v1*4+1]; /* v1, t, f1 */
    vface2[t*12+4+2] = verts[v1*4+2]; /* v1, s, f2 */
    vface2[t*12+4+3] = verts[v1*4+3]; /* v1, t, f2 */

    vface1[t*12+8+0] = (verts[v2*4+0]-w/2)*mf;     /* v2, x, f1 */
    vface1[t*12+8+1] = (h-1-verts[v2*4+1]-h/2)*mf; /* v2, y, f1 */
    vface2[t*12+8+0] = (verts[v2*4+2]-w/2)*mf;     /* v2, x, f2 */
    vface2[t*12+8+1] = (h-1-verts[v2*4+3]-h/2)*mf; /* v2, y, f2 */
    vface1[t*12+8+2] = verts[v2*4+0]; /* v2, s, f1 */
    vface1[t*12+8+3] = verts[v2*4+1]; /* v2, t, f1 */
    vface2[t*12+8+2] = verts[v2*4+2]; /* v2, s, f2 */
    vface2[t*12+8+3] = verts[v2*4+3]; /* v2, t, f2 */
    /*
     * Image #1: Compute texture block location, and check size
     */
    stmaxmin(v0, v1, v2, verts, 0, &smin, &smax);
    stmaxmin(v0, v1, v2, verts, 1, &tmin, &tmax);
    if ((smax - smin >= MAXTW) || (tmax - tmin) >= MAXTH) {
      fprintf(stderr, "Triangle #%i, face #1 exceeds max texture size\n", t);
      fprintf(stderr, "-> smax = %i, smin = %i, tmax = %i, tmin = %i\n",
	      smax,smin,tmax,tmin);
      fprintf(stderr, "-> v0 = %i, v1 = %i, v2 = %i\n", v0, v1, v2);
      exit(1);
    }
    /*
     * Image #1: Texture coordinates for texture block
     */
    twidth = smax-smin+1;
    twidth = ((twidth+3)>>2) << 2; /* round up to nearest /4 */
    smax = smin + twidth - 1;
    theight = tmax-tmin+1;
    tcoords1[t][0] = smin;
    tcoords1[t][1] = tmin;
    tcoords1[t][2] = smax;
    tcoords1[t][3] = tmax;

    /* Fill -- Do not swizzle here */
    for (i=0; i<=(smax-smin); i++) {
      for (j=0; j<=(tmax-tmin); j++) {
	r = i1[((tmin+j)*w+smin+i)*3+0] & 0xff;
	g = i1[((tmin+j)*w+smin+i)*3+1] & 0xff;
	b = i1[((tmin+j)*w+smin+i)*3+2] & 0xff;
	texture[(*tasize)+j*twidth+i] =
	  (r << 24) | (g << 16) | (b << 8) | 0xff;
      }
    }
    tindex1[t*3+0] = *tasize;
    tindex1[t*3+1] = twidth;
    tindex1[t*3+2] = theight;
    *tasize += theight*twidth;
    /*
     * Image #2: Compute texture block location, and check size
     */
    stmaxmin(v0, v1, v2, verts, 2, &smin, &smax);
    stmaxmin(v0, v1, v2, verts, 3, &tmin, &tmax);
    if ((smax - smin >= MAXTW) || (tmax - tmin) >= MAXTH) {
      fprintf(stderr, "Triangle #%i, face #2 exceeds max texture size\n", t);
      fprintf(stderr, "-> smin = %i, smax = %i, tmin = %i, tmax = %i\n",
	      smin, smax, tmin, tmax);
      fprintf(stderr, "-> v0 = %i, v1 = %i, v2 = %i\n",
	      v0, v1, v2);
      exit(1);
    }
    /*
     * Image #2: Texture coordinates for texture block
     */
    twidth = smax-smin+1;
    twidth = ((twidth+3)>>2) << 2; /* round up to nearest / 4*/
    smax = smin + twidth - 1;
    theight = tmax-tmin+1;
    tcoords2[t][0] = smin;
    tcoords2[t][1] = tmin;
    tcoords2[t][2] = smax;
    tcoords2[t][3] = tmax;

    /* Fill -- Do not swizzle here */
    for (i=0; i<=(smax-smin); i++) {
      for (j=0; j<=(tmax-tmin); j++) {
	r = i2[((tmin+j)*w+smin+i)*3+0] & 0xff;
	g = i2[((tmin+j)*w+smin+i)*3+1] & 0xff;
	b = i2[((tmin+j)*w+smin+i)*3+2] & 0xff;
	texture[(*tasize)+j*twidth+i] =
	  (r << 24) | (g << 16) | (b << 8) | 0xff;
      }
    }
    tindex2[t*3+0] = *tasize;
    tindex2[t*3+1] = twidth;
    tindex2[t*3+2] = theight;
    *tasize += theight*twidth;
  } /* for t */
} /* buildit */

/*
 * writeit -- write the two C output files
 *
 *            sfile   = filename of 'static' segment C code
 *            cfile   = filename of 'code' segment C code
 *            tricnt  = # of entries in triangle array
 *            texture, tindex1, tindex2,
 *            tcoords1, tcoords2, trilist, vface1, vface2
 *                    = arrays for C code
 *            tasize  = size of texture[] array 
 *            tris    = original triangle verts; printed in comments
 */
void writeit(char *sfile, char *cfile,
	     int tricnt,
	     unsigned int texture[], int tasize, int tindex1[], int tindex2[],
	     int tcoords1[][4], int tcoords2[][4],
	     int trilist[], int vface1[], int vface2[],
	     int tris[]) {
  int t, i;
  FILE *fs;
  FILE *fc;

  /* open static output file */
  fs = fopen(sfile, "w");
  if (fs == NULL) {
    fprintf(stderr, "Error opening sfile '%s' for output\n", sfile);
    exit(1);
  }

  /*
   * Write texture
   */
  fprintf(fs, "/* Generated by medit2c ('static' segment) */\n"
	      "static u64 padder; /* Enforce alignment */\n"
	      "unsigned int texture[%i] = {\n  ", tasize);
  for (i=0; i<tasize; i++) {
    fprintf(fs, "0x%x,", texture[i]);
    if (i % 16 == 0) fprintf(fs, "\n  ");
  }
  fprintf(fs, "\n};\n");
  /*
   * Close 'static' output file
   */
  fclose(fs);

  /*
   * open 'code' output file
   */
  fc = fopen(cfile, "w");
  if (fc == NULL) {
    fprintf(stderr, "Error opening cfile '%s' for output\n", cfile);
    exit(1);
  }
  /*
   * Write tricnt
   */
  fprintf(fc, "/* generated by medit2c ('code' segment) */\n");
  fprintf(fc, "int tricnt = %i;\n", tricnt);
  /*
   * Write trilist
   */
  fprintf(fc, "int trilist[%i] = {\n", tricnt*3);
  for (t = 0; t<tricnt; t++) {
    fprintf(fc, "  %i, %i, %i,\n",
	    trilist[t*3+0], trilist[t*3+1], trilist[t*3+2]);
  } /* for t */
  fprintf(fc, "\n};\n\n");
  /*
   * Write vface1
   */
  fprintf(fc, "Vtx vface1[%i] = {\n", tricnt*3);
  for (t = 0; t<tricnt; t++) {
    fprintf(fc, "  /* Verts for face #1, triangle #%i */\n", t);
    fprintf(fc, "  {%i,%i,0,0,%i,%i,0,0,0,0}, /* Orig vert #%i */\n",
	    vface1[t*12+0], vface1[t*12+1], vface1[t*12+2], vface1[t*12+3],
	    tris[t*3+0]);
    fprintf(fc, "  {%i,%i,0,0,%i,%i,0,0,0,0}, /* Orig vert #%i */\n",
	    vface1[t*12+4], vface1[t*12+5], vface1[t*12+6], vface1[t*12+7],
	    tris[t*3+1]);
    fprintf(fc, "  {%i,%i,0,0,%i,%i,0,0,0,0}, /* Orig vert #%i */\n",
	    vface1[t*12+8], vface1[t*12+9], vface1[t*12+10], vface1[t*12+11],
	    tris[t*3+2]);
  } /* for t */
  fprintf(fc, "\n};\n\n");
  /*
   * Write vface2
   */
  fprintf(fc, "Vtx vface2[%i] = {\n", tricnt*3);
  for (t=0; t<tricnt; t++) {
    fprintf(fc, "  /* Verts for face #2, triangle #%i */\n", t);
    fprintf(fc, "  {%i,%i,0,0,%i,%i,0,0,0,0}, /* Orig vert #%i */\n",
	    vface2[t*12+0], vface2[t*12+1], vface2[t*12+2], vface2[t*12+3],
	    tris[t*3+0]);
    fprintf(fc, "  {%i,%i,0,0,%i,%i,0,0,0,0}, /* Orig vert #%i */\n",
	    vface2[t*12+4], vface2[t*12+5], vface2[t*12+6], vface2[t*12+7],
	    tris[t*3+1]);
    fprintf(fc, "  {%i,%i,0,0,%i,%i,0,0,0,0}, /* Orig vert #%i */\n",
	    vface2[t*12+8], vface2[t*12+9], vface2[t*12+10], vface2[t*12+11],
	    tris[t*3+2]);
  } /* for t */
  fprintf(fc, "\n};\n\n");
  /*
   * Write tcoords1
   */
  fprintf(fc, "int tcoords1[%i][4] = {\n", tricnt);
  for (t=0; t<tricnt; t++) {
    fprintf(fc, "  {%i,%i,%i,%i},  /* Triangle #%i, Face #1 */\n",
	    tcoords1[t][0], tcoords1[t][1], tcoords1[t][2], tcoords1[t][3], t);
  } /* for t */
  fprintf(fc, "};\n\n");
  /*
   * Write tcoords2
   */
  fprintf(fc, "int tcoords2[%i][4] = {\n", tricnt);
  for (t=0; t<tricnt; t++) {
    fprintf(fc, "  {%i,%i,%i,%i},  /* Triangle #%i, Face #2 */\n",
	    tcoords2[t][0], tcoords2[t][1], tcoords2[t][2], tcoords2[t][3], t);
  } /* for t */
  fprintf(fc, "};\n\n");
  /*
   * Write tindex1
   */
  fprintf(fc, "int tindex1[%i] = {\n", tricnt*3);
  for (t = 0; t<tricnt; t++) {
    fprintf(fc, "  %i, %i, %i,\n",
	    tindex1[t*3+0], tindex1[t*3+1], tindex1[t*3+2]);
  } /* for t */
  fprintf(fc, "\n};\n\n");
  /*
   * Write tindex2
   */
  fprintf(fc, "int tindex2[%i] = {\n", tricnt*3);
  for (t = 0; t<tricnt; t++) {
    fprintf(fc, "  %i, %i, %i,\n",
	    tindex2[t*3+0], tindex2[t*3+1], tindex2[t*3+2]);
  } /* for t */
  fprintf(fc, "\n};\n\n");
  /*
   * Close 'code' file
   */
  fclose(fc);
} /* writeit */

/*
 * Read in a ppm file
 *
 * IN:  filename = file to read it from
 * OUT: width, height = width and height of image
 *      image = pointer to the image in memory
 */
void readppm(char *filename, int *width, int *height, char **image) {
  FILE *f;
  char buf[512];
  int i;

  /*
   * Open the file
   */
  f = fopen(filename, "r");
  if (f == NULL) {
    fprintf(stderr, "Error opening .ppm file '%s'\n", filename);
    exit(1);
  }

  /*
   * read in the ppm file header, currently only supports binary format,
   * and assumes one and only one comment line.
   */
  fgets(buf, 512, f);   /* P6 */
  while (1) {
    fgets(buf, 512, f);   /* # ..., or W H */
    if (buf[0] != '#') break;
  }
  sscanf(buf," %i %i ",width,height);
  fprintf(stderr, "Reading '%s', w=%i, h=%i\n", filename, *width, *height);
  fgets(buf, 512, f);   /* 256 */
  
  /* now actually read in the texture data */
  *image = (unsigned char *)calloc(*width**height,3);
  fread(*image, 3, *width**height, f);
  fclose(f);
} /* readppm */


/*
 * rdvertfile -- Read in vertex file, set up verts[].
 */
void rdvertfile(char *vertfn, int *vertcnt, int *verts, int vmax,
		int w, int h) {
  FILE *f;
  char buf[256];
  int x1, y1, x2, y2;

  f = fopen(vertfn, "r");
  if (f == NULL) {
    fprintf(stderr, "Error opening vertfile, '%s'\n", vertfn);
    exit(1);
  }

  *vertcnt = 0;
  while (fgets(buf, 256, f) != NULL) {
    if (*vertcnt >= vmax) {
      fprintf(stderr, "Exceeded vmax (%i)\n", *vertcnt); exit(1);}
    sscanf(buf, " %i %i %i %i ", &x1, &y1, &x2, &y2);
    if ((x1 < 0) || (x1 >= w) ||
	(y1 < 0) || (y1 >= h) ||
	(x2 < 0) || (x2 >= w) ||
	(y2 < 0) || (y2 >= h)) {
      fprintf(stderr, "Vertex #%i out of range\n", (*vertcnt)+1);
      exit(1);
    }
    verts[*vertcnt*4+0] = x1;
    verts[*vertcnt*4+1] = y1;
    verts[*vertcnt*4+2] = x2;
    verts[*vertcnt*4+3] = y2;
    (*vertcnt)++;
  }
  fclose(f);
} /* rdvertfile */

/*
 * rdtrifile -- Read in triangle file, set up tris[]
 */
void rdtrifile(char *trifn, int *tricnt, int *tris, int tmax,
	       int vertcnt) {
  FILE *f;
  char buf[256];
  int v1, v2, v3;

  f = fopen(trifn, "r");
  if (f == NULL) {
    fprintf(stderr, "Error opening trifile, '%s'\n", trifn);
    exit(1);
  }

  *tricnt = 0;
  while (fgets(buf, 256, f) != NULL) {
    if (*tricnt >= tmax) {fprintf(stderr, "Exceeded tmax\n"); exit(1);}
    sscanf(buf, " %i %i %i ", &v1, &v2, &v3);
    if ((v1 < 0) || (v1 >= vertcnt) ||
	(v2 < 0) || (v2 >= vertcnt) ||
	(v3 < 0) || (v3 >= vertcnt)) {
      fprintf(stderr, "Triangle %i uses out of range vertex\n", (*tricnt)+1);
      exit(1);
    }
    tris[*tricnt*3+0] = v1;
    tris[*tricnt*3+1] = v2;
    tris[*tricnt*3+2] = v3;
    (*tricnt)++;
  }
  fclose(f);
} /* rdtrifile */

/*
 * parseoptions -- Parse command line options
 *   vfile = vertex input file (from medit)
 *   tfile = triangle input file (from medit)
 *   ifile = image #1 input file
 *   jfile = image #2 input file
 *   sfile = 'static' segment C code output file
 *   cfile = 'code' segment C code output file
 *   
 */
void parseoptions(int argc, char *argv[],
		  char **vfile, char **tfile, char **ifile, char **jfile,
		  char **sfile, char **cfile,
		  int *mf) {
  extern char *optarg;
  extern int  optind;
  int c;

  *vfile = NULL;
  *tfile = NULL;
  *ifile = NULL;
  *jfile = NULL;
  *sfile = NULL;
  *cfile = NULL;
  *mf = 1;  /* default */

  while ((c = getopt(argc, argv, "hm:v:t:i:j:s:c:")) != EOF) {
    switch (c) {
      case 'h':  /* help */
      case '?':
      default:
        fprintf(stderr,
		"usage: %s [-h] [-m mf] -v fn -t fn -i fn -j fn -s fn -c fn\n"
		"   -h     = help\n"
		"   -m mf  = set scale factor for x,y coords (def = 1)\n"
		"   -v fn  = vertex input file from medit\n"
		"   -t fn  = triangle input file from medit\n"
		"   -i fn  = image input file #1 (.ppm format)\n"
		"   -j fn  = image input file #2 (.ppm format)\n"
		"   -s fn  = 'static' segment C code output file\n"
		"   -c fn  = 'code' segment C code output file\n",
		argv[0]);
	exit(2);
      case 'm':
	*mf = atoi(optarg);
	break;
      case 'v':
	*vfile = optarg;
	break;
      case 't':
	*tfile = optarg;
	break;
      case 'i':
	*ifile = optarg;
	break;
      case 'j':
	*jfile = optarg;
	break;
      case 's':
	*sfile = optarg;
	break;
      case 'c':
	*cfile = optarg;
	break;
    } /* switch */
  } /* while */
  /*
   * Check that all files were specified
   */
  if ((*vfile == NULL) ||
      (*tfile == NULL) ||
      (*ifile == NULL) ||
      (*jfile == NULL) ||
      (*sfile == NULL) ||
      (*cfile == NULL)
     ) {
    fprintf(stderr, "** Need all 6 files specified, use -h for help\n");
    exit(1);
  }
} /* parseoptions */


/*
 * main
 */
int main(int argc, char *argv[]) {
  char *vfile; /* vertex input file */
  char *tfile; /* triangle input file */
  char *ifile; /* image #1 input file */
  char *jfile; /* image #2 input file */
  char *sfile; /* 'static' segment C code output file */
  char *cfile; /* 'code' segment C code output file */
  int  mf;     /* scale factor for x and y */
  char *i1;    /* image #1 */
  char *i2;    /* image #2 */
  int w, h;    /* image width and height */
  int w2, h2;  /* temp for chking 2nd image */
  void *texture;
  void *tindex1;
  void *tindex2;
  void *tcoords1;
  void *tcoords2;
  void *trilist;
  void *vface1;
  void *vface2;
  int tricnt, vertcnt;
  int tris[TMAX*3];
  int verts[VMAX*4];
  int tasize;  /* # of elements in texture[] array */

  /*
   * parse options and parameters
   */
  parseoptions(argc, argv, &vfile, &tfile, &ifile, &jfile,
	       &sfile, &cfile, &mf);

  /*
   * read input files
   */
  readppm(ifile, &w,  &h,  &i1);
  readppm(jfile, &w2, &h2, &i2);
  if ((w != w2) || (h != h2)) {
    fprintf(stderr, "Sizes of ifile and jfile must match.\n");
    exit(1);
  }
  rdvertfile(vfile, &vertcnt, verts, VMAX, w, h);
  printf("Read %i verticies\n", vertcnt);
  rdtrifile(tfile, &tricnt, tris, TMAX, vertcnt);
  printf("Read %i triangles\n", tricnt);

  /*
   * create arrays that will be written to C files later
   */
  texture  = (void *) malloc(sizeof(int)*tricnt*(32*32)*2); /* max possible */
  tindex1  = (void *) malloc(sizeof(int)*tricnt*3);
  tindex2  = (void *) malloc(sizeof(int)*tricnt*3);
  tcoords1 = (void *) malloc(sizeof(int)*tricnt*4);
  tcoords2 = (void *) malloc(sizeof(int)*tricnt*4);
  trilist  = (void *) malloc(sizeof(int)*tricnt*3);
  vface1   = (void *) malloc(sizeof(int)*tricnt*3*4);
  vface2   = (void *) malloc(sizeof(int)*tricnt*3*4);

  /*
   * Fill the arrays
   */
  buildit(tris, tricnt, verts, i1, i2, w, h,
	  texture, &tasize, tindex1, tindex2,
	  tcoords1, tcoords2, trilist, vface1, vface2,
	  mf);

  /*
   * Write the output files
   */
  writeit(sfile, cfile, tricnt,
	  texture, tasize, tindex1, tindex2,
	  tcoords1, tcoords2, trilist, vface1, vface2,
	  tris);
} /* main */
