
/*---------------------------------------------------------------------*
        Copyright (C) 1997 Nintendo. (Originated by SGI)
        
        $RCSfile: medit.c,v $
        $Revision: 1.1.1.1 $
        $Date: 2002/05/02 03:27:18 $
 *---------------------------------------------------------------------*/
// if anyone wants to port this to linux be my guest
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "n_aux.h"

/* For timing tests */
#include <sys/time.h>

/* Macros */
#define myabs(a) ((a)<0 ? -(a) : (a))

#define OUR_W 500
#define OUR_H 500

/* Width in pixels of separating border */
#define SEPBW 2

/*
 * structure for global vars during cmd loop, accessible by X callback
 * routines
 */
struct {
  /* 'permanent' info */
  char *vertfn;/* filename for vertex file */
  char *trifn; /* filename for triangle file */
  char *i1;    /* 1st image */
  char *i2;    /* 2nd image */
  int    w;    /* Image width, in image pixels */
  int    h;    /* Image height, in image pixels */
  int winw;    /* Window width, in screen pixels */
  int winh;    /* Window height, in screen pixels */
  int  mag;    /* Magnification factor */
  int vertcnt; /* Vertex cnt */
  int vmax;    /* Max allowed */
  int *verts;  /* Actual verticies */
  int tricnt;  /* Triangle cnt */
  int tmax;    /* Max allowed */
  int *tris;   /* Actual triangles */
  /* state */
  char cmd;    /* Current cmd */
  int show;    /* Show triangles? */
  int showone; /* # of single triangle to show, -1 if none */
  /* state for individual cmds */
  int x1, y1;  /* Vert #1 for new vert pairs, -1 = none */
  int v1, vi;  /* Vert index for vertex move, -1 = none ; image # of vert */
  int v2;      /* Vert index for triangle add/del */
  int cnt;     /* Points clicked so far for N/D */
} gv;

/*
 * 'temporary' points to display (within a cmd)
 */
struct {
  /* x,y in image #img, draw in red */
  int img;
  int rx;  /* if == -1 don't draw */
  int ry;
  /* 2nd such pt */
  int img2;
  int rx2; /* if == -1 don't draw */
  int ry2;
} pv;

#define ZNEAR 1.0
#define ZFAR  100.0

/* Prototypes */
void idlefunc(void);
void plot1(int img, int x, int y);
void display(void);

/*
 * NOTE:  (0,0) for gl is in the lower left hand corner, but
 *        (0,0) for the aux* functions is in the upper left hand corner!
 *              Agh!
 */

/*
 * Beep!
 */
void beep(void) {
  printf("\007"); fflush(stdout);
  printf("******* BEEP ******\n");
}


/*
 * Function to convert x,y as returned by an event into x,y pixel coords
 * in one of the two images.
 * 1  = image #1
 * 2  = image #2
 * -1 = error
 */
void cvtxy(int x, int y, int *img, int *ix, int *iy) {
  if (x < gv.w*gv.mag) {
    /* Left image */
    *img = 1;
    *ix = x / gv.mag;
    *iy = y / gv.mag;
  } else if (x < gv.w*gv.mag+SEPBW) {
    /* Boundary */
    *img = -1;
  } else {
    /* Right image */
    *img = 2;
    *ix = (x-gv.w*gv.mag-SEPBW) / gv.mag;
    *iy = y / gv.mag;
  }
} /* cvtxy */

/*
 * addvertex -- Add a pair of verticies to the list
 */
void addvertex(int x1, int y1, int x2, int y2) {
  int i;

  if (gv.vertcnt >= gv.vmax) {
    beep(); beep();
    printf("Exceeded VMAX.  Vertex pair not stored\n");
  } else {
    i = gv.vertcnt * 4;
    gv.verts[i+0] = x1;
    gv.verts[i+1] = y1;
    gv.verts[i+2] = x2;
    gv.verts[i+3] = y2;
    gv.vertcnt++;
  }
} /* addvertex */

/*
 * deletetri -- Delete a triangle from the tris list
 *
 * t = index into tris array
 */
void deletetri(int t) {
  int i;

  for (i=t+1; i<gv.tricnt; i++) {
    gv.tris[(i-1)*3+0] = gv.tris[i*3+0];
    gv.tris[(i-1)*3+1] = gv.tris[i*3+1];
    gv.tris[(i-1)*3+2] = gv.tris[i*3+2];
  }
  gv.tricnt--;
} /* deletetri */


/*
 * deletevert -- Delete a vertex pair from the verts list, and any associated
 *               triangles
 *
 * v = index into verts array
 */
void deletevert(int v) {
  int i;
  int t, tdead;

  /*
   * Move higher entries down, adjust vertcnt
   */
  for (i=v+1; i<gv.vertcnt; i++) {
    gv.verts[(i-1)*4+0] = gv.verts[i*4+0];
    gv.verts[(i-1)*4+1] = gv.verts[i*4+1];
    gv.verts[(i-1)*4+2] = gv.verts[i*4+2];
    gv.verts[(i-1)*4+3] = gv.verts[i*4+3];
  }
  gv.vertcnt--;
  /*
   * Zap any associated triangles
   */
  tdead = 0;
  for (t=0; t<gv.tricnt; t++) {
    if ((gv.tris[t*3] == v) ||
	(gv.tris[t*3+1] == v) ||
	(gv.tris[t*3+2] == v)) {
      tdead++;
      deletetri(t);
    }
  }
  /*
   * Readjust triangle vert indicies
   */
  for (t=0; t<gv.tricnt; t++) {
    if (gv.tris[t*3+0] > v) gv.tris[t*3+0]--;
    if (gv.tris[t*3+1] > v) gv.tris[t*3+1]--;
    if (gv.tris[t*3+2] > v) gv.tris[t*3+2]--;
  }
  /*
   * Notify user
   */
  printf("Deleted 1 vertex pair and %i triangles\n", tdead);
} /* deletevert */


/*
 * whichvert -- find closet vert to clicked location.
 *              returns vert index, or -1 if none sufficiently close.
 *              params: img = 1 or 2 (image #), x&y = image coordinates
 */
int whichvert(int img, int x, int y) {
  int v, dist, offset;
  int best, bestdist;

  best = -1;
  bestdist = 32767;
  offset = ((img == 1) ? 0 : 2);
  for (v=0; v<gv.vertcnt; v++) {
    dist = myabs(gv.verts[v*4+offset+0] - x) +  /* x */
           myabs(gv.verts[v*4+offset+1] - y);   /* y */
    if ((dist < 15) && (dist < bestdist)) {
      best = v;
      bestdist = dist;
    }
  }
  return best;
}

/*
 * Drawtri -- draws triangle (on both images) given triangle #.
 */
void drawtri(int t) {
  int v0, v1, v2;
  
  v0 = gv.tris[t*3+0];
  v1 = gv.tris[t*3+1];
  v2 = gv.tris[t*3+2];
  glBegin(GL_TRIANGLES);
  /* Image #1 */
  glVertex2i(gv.verts[v0*4+0]*gv.mag, (gv.h-1-gv.verts[v0*4+1])*gv.mag);
  glVertex2i(gv.verts[v1*4+0]*gv.mag, (gv.h-1-gv.verts[v1*4+1])*gv.mag);
  glVertex2i(gv.verts[v2*4+0]*gv.mag, (gv.h-1-gv.verts[v2*4+1])*gv.mag);
  /* Image #2 */
  glVertex2i(SEPBW+(gv.w+gv.verts[v0*4+2])*gv.mag,
	     (gv.h-1-gv.verts[v0*4+3])*gv.mag);
  glVertex2i(SEPBW+(gv.w+gv.verts[v1*4+2])*gv.mag,
	     (gv.h-1-gv.verts[v1*4+3])*gv.mag);
  glVertex2i(SEPBW+(gv.w+gv.verts[v2*4+2])*gv.mag,
	     (gv.h-1-gv.verts[v2*4+3])*gv.mag);
  glEnd();
  glColor3f(1.0, 1.0, 0.0);
  glBegin(GL_LINE_LOOP);
  /* Image #1 */
  glVertex2i(gv.verts[v0*4+0]*gv.mag, (gv.h-1-gv.verts[v0*4+1])*gv.mag);
  glVertex2i(gv.verts[v1*4+0]*gv.mag, (gv.h-1-gv.verts[v1*4+1])*gv.mag);
  glVertex2i(gv.verts[v2*4+0]*gv.mag, (gv.h-1-gv.verts[v2*4+1])*gv.mag);
  glEnd();
  glBegin(GL_LINE_LOOP);
  /* Image #2 */
  glVertex2i(SEPBW+(gv.w+gv.verts[v0*4+2])*gv.mag,
	     (gv.h-1-gv.verts[v0*4+3])*gv.mag);
  glVertex2i(SEPBW+(gv.w+gv.verts[v1*4+2])*gv.mag,
	     (gv.h-1-gv.verts[v1*4+3])*gv.mag);
  glVertex2i(SEPBW+(gv.w+gv.verts[v2*4+2])*gv.mag,
	     (gv.h-1-gv.verts[v2*4+3])*gv.mag);
  glEnd();
} /* drawtri */

/*
 * whichtri -- find triangle index based on verticies (any order), -1 if none
 */
int whichtri(int v1, int v2, int v3) {
  int t, tv1, tv2, tv3;

  for (t=0; t<gv.tricnt; t++) {
    tv1 = gv.tris[t*3+0];
    tv2 = gv.tris[t*3+1];
    tv3 = gv.tris[t*3+2];
    if ( ((tv1 == v1) && (tv2 == v2) && (tv3 == v3)) ||
	 ((tv1 == v1) && (tv2 == v3) && (tv3 == v2)) ||
	 ((tv1 == v2) && (tv2 == v1) && (tv3 == v3)) ||
	 ((tv1 == v2) && (tv2 == v3) && (tv3 == v1)) ||
	 ((tv1 == v3) && (tv2 == v1) && (tv3 == v2)) ||
	 ((tv1 == v3) && (tv2 == v2) && (tv3 == v1))
       ) {
      return(t);
    }
  }
  return(-1);
} /* whichtri */


/*
 * New triangle, given the 3 pts; called within leftbutton callback function
 */
void newtri(int v1, int v2, int v3) {
  int t, td;

  if (gv.tricnt >= gv.tmax) {
    beep(); printf("Can't add triangle -- exceeded TMAX\n");
    return;
  }
  /*
   * Check for duplicate
   */
  td = whichtri(v1, v2, v3);
  if (td != -1) {
    beep(); printf("That triangle already exists\n");
    return;
  }
  /*
   * Add it
   */
  t = gv.tricnt;
  gv.tricnt++;
  gv.tris[t*3+0] = v1;
  gv.tris[t*3+1] = v2;
  gv.tris[t*3+2] = v3;
  /*
   * Draw new triangle (it will be overwritten quickly by redraw)
   */
  glColor3f(1.0, 0.0, 0.0);
  drawtri(t);
  glFlush();
  sleep(2);
} /* newtri */


/*
 * Delete triangle, given the 3 pts; called within leftbutton callback function
 */
void deltri(int v1, int v2, int v3) {
  int t;
  t = whichtri(v1, v2, v3);
  if (t == -1) {
    beep(); printf("No such triangle\n");
    return;
  }
  deletetri(t);
  printf("Deleted triangle #%i\n", t);
} /* deltri */

/*
 * Callback function for left mouse button being depressed
 */
void leftButtonDown(AUX_EVENTREC *event) {
  GLint x, y;
  int ix, iy, img, v, xtrue, ytrue;

  x = event->data[AUX_MOUSEX];
  y = event->data[AUX_MOUSEY];

  /* printf("(x,y) = (%i, %i)\n", x, y); */
  cvtxy(x, y, &img, &ix, &iy);
  printf("Image = %i, (ix,iy) = (%i, %i)\n", img, ix, iy);

  /*
   * Abort if boundary was selected
   */
  if (img == -1) {
    beep();
    return;
  }

  /*
   * Do cmd
   */
  switch (gv.cmd) {
    /* Delete pair */
    case 'd':
      v = whichvert(img, ix, iy);  /* Determine which vert was selected */
      if (v == -1) {
	beep(); printf("No vert selected\n");
	return;
      }
      deletevert(v);
      break;
    /* Find pairs */
    case 'p':
      v = whichvert(img, ix, iy);
      if (v == -1) {
	pv.rx = -1;
	beep(); printf("No vert selected\n");
	return;
      }
      if (img == 1) {
	pv.img = 2;
	pv.rx = gv.verts[v*4+2];
	pv.ry = gv.verts[v*4+3];
      } else {
	pv.img = 1;
	pv.rx = gv.verts[v*4+0];
	pv.ry = gv.verts[v*4+1];
      }
      printf("Selected vertex pair #%i\n", v);
      break;
    /* Move vert */
    case 'm':
      if (gv.v1 == -1) {
	/* Select vert to move */
	v = whichvert(img, ix, iy);
	if (v == -1) {
	  beep(); printf("No vert selected\n");
	  return;
	} else {
	  gv.v1 = v;
	  gv.vi = img;
	  /* Mark red */
	  pv.img = img;
	  pv.rx = (img == 1 ? gv.verts[v*4+0] : gv.verts[v*4+2]);
	  pv.ry = (img == 1 ? gv.verts[v*4+1] : gv.verts[v*4+3]);
	}
      } else {
	/* This is dest location */
	if (img != gv.vi) {
	  beep(); printf("Vert must remain in same image! -- Click again\n");
	  return;
	}
	/* New loc */
	pv.rx = -1;
	pv.ry = -1;
	if (gv.vi == 1) {
	  gv.verts[gv.v1*4+0] = ix;
	  gv.verts[gv.v1*4+1] = iy;
	} else {
	  gv.verts[gv.v1*4+2] = ix;
	  gv.verts[gv.v1*4+3] = iy;
	}
	gv.v1 = -1;
	gv.vi = 0;
      }
      break;
    case 'n':
      if (img == 1) {
	gv.x1 = ix;
	gv.y1 = iy;
	if (gv.x1 != -1) {
	  /* They're changing their mind about 1st point */
	  display();  /* Zaps previous point, the slow way... */
	}
	/* Draw new point in red -- gets done anyway */
	/* glColor3f(1.0, 0.0, 0.0); */
	/* plot1(1, ix, iy); */
	/* Mark for display to draw red pt */
	pv.img = 1;
	pv.rx = ix;
	pv.ry = iy;
      } else {
	/* Clicked on 2nd image */
	if (gv.x1 == -1) {
          beep(); return;
	} else {
	  addvertex(gv.x1, gv.y1, ix, iy);
	  glColor3f(1.0, 0.0, 0.0);
	  plot1(2, ix, iy);
	  /* set flags for starting next pair */
	  gv.x1 = -1;
	  gv.y1 = -1;
	  pv.rx = -1;
	}
      }
      break;
    /*
     * Triangle cmds
     */
    /* New and delete */
    case 'N':
    case 'D':
      /* Find vert */
      v = whichvert(img, ix, iy);
      if (v == -1) {
	beep(); printf("No vert selected\n");
	return;
      }
      if (img == 1) {
	xtrue = gv.verts[v*4+0]; ytrue = gv.verts[v*4+1];
      } else {
	xtrue = gv.verts[v*4+2]; ytrue = gv.verts[v*4+3];
      }
      if (gv.cnt == 0) {
	/* This is 1st pt selected */
	pv.img = img;
	pv.rx = xtrue;
	pv.ry = ytrue;
	gv.v1 = v;  /* Save vert */
	gv.cnt++;
	break;
      } else if (gv.cnt == 1) {
	/* This is 2nd pt selected */
	if (img != pv.img) {
	  beep(); printf("Must stay in same image -- resel pt #2\n");
	  break;
	}
	pv.img2 = img;
	pv.rx2  = xtrue;
	pv.ry2  = ytrue;
	gv.v2   = v;   /* Save vert */
	gv.cnt++;
	break;
      } else {
	/* This is 3rd (and final) pt selected */
	if (img != pv.img) {
	  beep(); printf("Must stay in same image -- resel pt #3\n");
	  break;
	}
	if (gv.cmd == 'N') {
	  newtri(gv.v1, gv.v2, v);
	} else {
	  deltri(gv.v1, gv.v2, v);
	}
	/* Ready for next one */
	pv.rx = -1;
	pv.rx2 = -1;
	gv.cnt = 0;
	break;
      }
    default:
      beep();
  }

} /* leftButtonDown */

/*
 * Callback function for space key being depressed
 * This indicates that we should terminate current command and return to
 * command line
 */
void spacepressed(void) {
  pv.rx = -1; /* Clear red point flag */
  pv.rx2 = -1;
  auxIdleFunc(idlefunc); /* Reactivate cmd line */
} /* spacepressed */


/*
 * PixelCoords -- set up for gl integer pixel coordinates.
 *                w = width, h = height of viewport
 */
void PixelCoords(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, w, 0, h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.375, 0.375, 0.0);
}

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
#if 0
  /* This makes it upside-down */
  fread(*image, 3, *width**height, f);
#else
  for (i=*height-1; i>=0; i--) {
    fread(&((*image)[i**width*3]), 3, *width, f);
  }
#endif
  fclose(f);
} /* readppm */


/*
 * drawimages -- draw both images and the border between them
 *
 */
void drawimages(char *i1, char *i2, int w, int h, int mag) {
  int i;

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
  glPixelZoom((GLfloat) mag, (GLfloat) mag);
  /* 1st image */
  glRasterPos2i(0, 0);
  glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, i1);
  /* 2nd image */
  glRasterPos2i(w*mag+SEPBW, 0);
  glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, i2);
  /* white border between images */
  glColor3f(1.0, 1.0, 1.0); /* White */
  glBegin(GL_LINES);
  for (i=0; i<SEPBW; i++) {
    glVertex2i(w*mag+i, 0);
    glVertex2i(w*mag+i, h*mag);
  }
  glEnd();
  glFlush();
} /* drawimages */


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
 * wrvertfile -- Write out vertex file, from verts[] data
 */
void wrvertfile(char *vertfn, int vertcnt, int *verts) {
  FILE *f;
  int v;
  char fn2[100];

  /* First rename the old file to a backup name */
  strcpy(fn2, vertfn);
  strcat(fn2, "~");
  rename(vertfn, fn2);

  f = fopen(vertfn, "w");
  if (f == NULL) {
    fprintf(stderr, "Error opening vertfile '%s' for write\n", vertfn);
    return;
  }

  for (v=0; v<vertcnt; v++) {
    fprintf(f, "%i %i %i %i\n",
	    verts[v*4+0], verts[v*4+1], verts[v*4+2], verts[v*4+3]);
  }
  fclose(f);
  printf("Wrote %i verticies to '%s'\n", vertcnt, vertfn);
} /* wrvertfile */

/*
 * wrtrifile -- Write out triangle file, from tris[] data
 *              check for out of range verts as we go
 */
void wrtrifile(char *trifn, int tricnt, int *tris, int vertcnt) {
  FILE *f;
  int t;
  char fn2[100];

  /* First rename the old file to a backup name */
  strcpy(fn2, trifn);
  strcat(fn2, "~");
  rename(trifn, fn2);

  f = fopen(trifn, "w");
  if (f == NULL) {
    fprintf(stderr, "Error opening trifile '%s' for write\n", trifn);
    return;
  }

  for (t=0; t<tricnt; t++) {
    /* Final check that no verts are out of range */
    if ((tris[t*3] >= vertcnt) || (tris[t*3+1] >= vertcnt) ||
	(tris[t*3+2] >= vertcnt)) {
      printf("Out of range vert found in triangle #%i\n", t);
    }
    fprintf(f, "%i %i %i\n",
	    tris[t*3+0], tris[t*3+1], tris[t*3+2]);
  }
  fclose(f);
  printf("Wrote %i triangles to '%s'\n", tricnt, trifn);
} /* wrtrifile */

/*
 * plot1 -- plot a vertex
 *          img = 1 or 2
 *          x, y = image coordinate points
 *          w, h = image width and height
 *          mag  = mag factor
 * Caller should set color
 * Relies upon the gv structure already being set up.
 */
void plot1(int img, int x, int y) {
  int offset, px, py;

  if (img == 1) {
    offset = 0;
  } else {
    offset = gv.w*gv.mag+SEPBW;
  }

  glBegin(GL_POINTS);
  for (px = 0; px < gv.mag; px++) {
    for (py = 0; py < gv.mag; py++) {
      glVertex2i(x*gv.mag+px+offset,
		 (gv.h-1-y)*gv.mag+py);
    }
  }
  glEnd();
  glFlush();
} /* plot1 */


/*
 * plotverts -- plot all verticies
 *              For speed, avoids calling plot1
 *              Will plot red vert from pv if needed
 */
void plotverts(int vertcnt, int *verts, int w, int h, int mag) {
  int v;
  int px, py;
  int offset;

  glColor3f(0.0, 1.0, 0.0);  /* Green */
  glBegin(GL_POINTS);
  for (v=0; v<vertcnt; v++) {
    /* image #1 */
    for (px = 0; px < mag; px++) {
      for (py = 0; py < mag; py++) {
	glVertex2i(verts[v*4]*mag+px, (h-1-verts[v*4+1])*mag+py);
      }
    }
    /* image #2 */
    for (px = 0; px < mag; px++) {
      for (py = 0; py < mag; py++) {
	glVertex2i(w*mag+SEPBW+verts[v*4+2]*mag+px, (h-1-verts[v*4+3])*mag+py);
      }
    }
  }
  if (pv.rx != -1) {
    offset = (pv.img == 1 ? 0 : w*mag+SEPBW);
    glColor3f(1.0, 0.0, 0.0);
    for (px = 0; px < mag; px++) {
      for (py = 0; py < mag; py++) {
	glVertex2i(offset+pv.rx*mag+px, (h-1-pv.ry)*mag+py);
      }
    }
  }
  if (pv.rx2 != -1) {
    offset = (pv.img2 == 1 ? 0 : w*mag+SEPBW);
    glColor3f(1.0, 0.0, 0.0);
    for (px = 0; px < mag; px++) {
      for (py = 0; py < mag; py++) {
	glVertex2i(offset+pv.rx2*mag+px, (h-1-pv.ry2)*mag+py);
      }
    }
  }
  glEnd();
  glFlush();
} /* plotverts */

/*
 * plottriangles -- plot all triangles (in blue)
 */
void plottriangles(void) {
  int t;

  for (t=0; t<gv.tricnt; t++) {
    glColor3f(0.0, 0.0, 1.0);
    drawtri(t);
  }
  glFlush();
} /* plottriangles */


/*
 * parseoptions
 *
 * ppm1   = first .ppm file
 * ppm2   = 2nd .ppm file
 * vertfn = vertex file
 * trifn  = triangle file
 * mag = magnification factor for display
 */
void parseoptions(int argc, char *argv[],
		  char **ppm1fn, char **ppm2fn, char **vertfn, char **trifn,
		  int *mag) {
  extern char *optarg;
  extern int   optind;
  int c;
  

  *ppm1fn = NULL;
  *ppm2fn = NULL;
  *vertfn = NULL;
  *trifn  = NULL;
  *mag = 1;

  while ((c = getopt(argc, argv, "hp:m:v:t:")) != EOF) {
    switch (c) {
    /*
     * magnification factor
     */
    case 'm':
      *mag = atoi(optarg);
      break;
    /*
     * .ppm files
     */
    case 'p':
      /* .ppm file, need 2 */
      if (*ppm1fn == NULL) {
	*ppm1fn = optarg;
      } else if (*ppm2fn == NULL) {
	*ppm2fn = optarg;
      } else {
	fprintf(stderr, "Only 2 .ppm files allowed\n");
	exit(1);
      }
      break;
    /*
     * vertex file
     */
    case 'v':
      if (*vertfn == NULL) {
	*vertfn = optarg;
      } else {
	fprintf(stderr, "Only 1 vertex file allowed\n");
	exit(1);
      }
      break;
    /*
     * triangle file
     */
    case 't':
      if (*trifn == NULL) {
	*trifn = optarg;
      } else {
	fprintf(stderr, "Only 1 triangle file allowed\n");
	exit(1);
      }
      break;
    /*
     * help
     */
    case '?':
    case 'h':
      fprintf(stderr,
	      "usage: %s [-m num] -p f1.ppm -p f2.ppm -v vf -t tf [-h]\n"
	      "   -m num  = Set magnification factor to num (default = 1)\n"
	      "   -p file = Specify .ppm image file, two needed\n"
	      "   -v file = Specify vertex file, one needed\n"
	      "   -t file = Specify triangle file, one needed\n",
	      argv[0]);
      exit(2);
    } /* switch */
  } /* while */

  /* Final checks */
  if ((*ppm1fn == NULL) || (*ppm2fn == NULL)) {
    fprintf(stderr, "Need 2 .ppm files\n");
    exit(1);
  }
  if (*vertfn == NULL) {
    fprintf(stderr, "Need a vertex file (empty file OK)\n");
    exit(1);
  }
  if (*trifn == NULL) {
    fprintf(stderr, "Need a triangle file (empty file OK)\n");
    exit(1);
  }
} /* parseoptions */


/*
 * display scene
 */
void display(void) {
  drawimages(gv.i1, gv.i2, gv.w, gv.h, gv.mag);
  if (gv.show) plottriangles();
  if (gv.showone != -1) {
    glColor3f(0.0, 0.0, 1.0);
    drawtri(gv.showone);
    glFlush();
  }
  plotverts(gv.vertcnt, gv.verts, gv.w, gv.h, gv.mag);
} /* display */

/*
 * Print cmd help
 */
void cmdhelp(void) {
  printf("\n"
	 "Commands:\n"
         "  ** Verticies **\n"
	 "  n = create new vertex pairs\n"
	 "  d = delete vertex pairs\n"
	 "  m = move single vertex\n"
         "  p = find vertex pairs\n"
	 "\n"
	 "  ** Triangles **\n"
	 "  N   = new triangles\n"
	 "  D   = delete triangles\n"
	 "  S n = display triangle #n, zero based\n"
	 "  +   = display all triangles\n"
	 "  -   = hide all triangles\n"
         "\n"
         "  ** General **\n"
	 "  h = help\n"
	 "  w = redraw window\n"
	 "  q = quit and save\n"
	);
}


/*
 * Callback function for idle -- implements cmd line
 */
void idlefunc(void) {
  char buf[256];
  char cmd;

  /*
   * Cleanup
   */
  pv.rx = -1;
  pv.rx2 = -1;

  printf("> ");
  gets(buf);
  cmd = buf[0];

  switch (cmd) {
    /*
     * Verts
     */
    /* New verts */
    case 'n':
      auxIdleFunc(NULL);
      printf("Create new vert pairs with mouse, hit SPACE when done\n");
      gv.cmd = 'n';
      gv.x1 = -1; gv.y1 = -1;
      break;
    /* Delete verts */
    case 'd':
      auxIdleFunc(NULL);
      printf("Delete vert pairs with mouse, hit SPACE when done\n");
      gv.cmd = 'd';
      break;
    /* Move single vert */
    case 'm':
      auxIdleFunc(NULL);
      printf("Move vertex: click to select, again to place.  SPACE=done\n");
      gv.cmd = 'm';
      gv.v1 = -1;
      break;
    /* Find pair */
    case 'p':
      auxIdleFunc(NULL);
      printf("Select vert -- pair vert will be red.  SPACE=done\n");
      gv.cmd = 'p';
      break;
    /*
     * Triangles
     */
    case 'N':
      auxIdleFunc(NULL);
      printf("Select 3 verts for new triangle.  SPACE=done\n");
      gv.cmd = 'N';
      gv.cnt = 0;
      break;
    case 'D':
      auxIdleFunc(NULL);
      printf("Select 3 verts for triangle to delete.  SPACE=done\n");
      gv.cmd = 'D';
      gv.cnt = 0;
      break;
    case 'S':
      gv.showone = atoi(&(buf[2]));
      if ((gv.showone < 0) || (gv.showone >= gv.tricnt)) {
	printf("Bad tri\n");
	gv.showone = -1;
	break;
      }
      printf("Showing triangle #%i, use - to stop\n", gv.showone);
      printf("Left  Verts: (%i, %i) (%i, %i) (%i, %i)\n",
	     gv.verts[gv.tris[gv.showone*3+0]*4+0],
	     gv.verts[gv.tris[gv.showone*3+0]*4+1],
	     gv.verts[gv.tris[gv.showone*3+1]*4+0],
	     gv.verts[gv.tris[gv.showone*3+1]*4+1],
	     gv.verts[gv.tris[gv.showone*3+2]*4+0],
	     gv.verts[gv.tris[gv.showone*3+2]*4+1]);
      printf("Right Verts: (%i, %i) (%i, %i) (%i, %i)\n",
	     gv.verts[gv.tris[gv.showone*3+0]*4+2],
	     gv.verts[gv.tris[gv.showone*3+0]*4+3],
	     gv.verts[gv.tris[gv.showone*3+1]*4+2],
	     gv.verts[gv.tris[gv.showone*3+1]*4+3],
	     gv.verts[gv.tris[gv.showone*3+2]*4+2],
	     gv.verts[gv.tris[gv.showone*3+2]*4+3]);
      break;
    case '+':
      gv.show = 1;
      break;
    case '-':
      gv.show = 0;
      gv.showone = -1;
      break;
    /*
     * Misc
     */
    /* Help */
    case 'h': cmdhelp(); break;
    /* Quit, saving verts and tris */
    case 'q':
      /* Write out verts and tris */
      wrvertfile(gv.vertfn, gv.vertcnt, gv.verts);
      wrtrifile(gv.trifn, gv.tricnt, gv.tris, gv.vertcnt);
      printf("Bye!\n");
      exit(0);
    /* Redraw window */
    case 'w':
      display();
      break;
    default:
      printf("Bad cmd.\n");
      break;
  } /* switch */
} /* idlefunc */

/*
 * cmdloop -- Command loop
 */
void cmdloop() {

  /* Set up for mouse buttons */
  auxMouseFunc(AUX_LEFTBUTTON, AUX_MOUSEDOWN, leftButtonDown);   /* sel pts */
  auxKeyFunc(AUX_SPACE, spacepressed);                           /* cmd esc */
  auxIdleFunc(idlefunc);   /* idlefunc is cmd line, disabled for active cmd */
  gv.cmd = '0';  /* No active cmd */
  auxReshapeFunc((void *) display);  /* Resize bad, but expose OK  */
  auxMainLoop(display);
} /* cmdloop */


/*
 * main
 */
int main(int argc, char *argv[])
{
  int mag;
  char *ppm1fn, *ppm2fn, *vertfn, *trifn;
  int h, w;       /* ppm file height and width */
  int h2, w2;     /* temp vars to chk 2nd ppm file */
  int winh, winw; /* Window height and width */
  char *i1, *i2;
#define VMAX 300
#define TMAX 500
  int verts[VMAX*4];  /* verts[i*4+0] = image1x, verts[i*4+1] = image1y,
			 verts[i*4+2] = image2x, verts[i*4+3] = image2y */
  int tris[TMAX*3];   /* tris[i+0...i+2] = 'i' values of verts in verts[] */
  int vertcnt, tricnt;
  
  /*
   * Parse command line options to get filenames, etc.
   */
  parseoptions(argc, argv, &ppm1fn, &ppm2fn, &vertfn, &trifn, &mag);
  
  /*
   * Open .ppm files
   */
  readppm(ppm1fn, &w,  &h,  &i1);
  readppm(ppm2fn, &w2, &h2, &i2);
  if ((w != w2) || (h != h2)) {
    fprintf(stderr, "Sizes of .ppm files must match.\n");
    exit(1);
  }

  /*
   * Compute window height and width
   */
  winh = mag*h;
  winw = mag*w*2+SEPBW;

  /*
   * Open the window
   */
  auxInitDisplayMode (AUX_SINGLE | AUX_RGB | AUX_DEPTH);
  auxInitPosition (0, 0, winw, winh);
  auxInitWindow (argv[0]);
  
  glDisable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  PixelCoords(winw, winh);  /* Use pixel coordinates */

  /*
   * Draw images
   */
  pv.rx = -1; /* clear this flag */
  pv.rx2 = -1;
  gv.show = 0;
  gv.showone = -1;
  drawimages(i1, i2, w, h, mag);

  /*
   * Read in the vertex and triangle files
   */
  rdvertfile(vertfn, &vertcnt, verts, VMAX, w, h);
  printf("Read %i verticies\n", vertcnt);
  rdtrifile(trifn, &tricnt, tris, TMAX, vertcnt);
  printf("Read %i triangles\n", tricnt);

  /*
   * Plot verts
   */
  plotverts(vertcnt, verts, w, h, mag);

  /*
   * Set up global data structure to hold vars during command loop
   * (To deal with X callbacks...)
   */
  gv.vertfn  = vertfn;
  gv.trifn   = trifn;
  gv.i1      = i1;
  gv.i2      = i2;
  gv.w       = w;
  gv.h       = h;
  gv.winw    = winw;
  gv.winh    = winh;
  gv.mag     = mag;
  gv.vertcnt = vertcnt;
  gv.vmax    = VMAX;
  gv.verts   = verts;
  gv.tricnt  = tricnt;
  gv.tmax    = TMAX;
  gv.tris    = tris;

  /*
   * Command loop, never returns.
   */
  cmdloop();

} /* main */


