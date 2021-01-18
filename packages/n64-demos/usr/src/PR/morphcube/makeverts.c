
/*---------------------------------------------------------------------*
        Copyright (C) 1997 Nintendo. (Originated by SGI)
        
        $RCSfile: makeverts.c,v $
        $Revision: 1.1.1.1 $
        $Date: 2002/05/02 03:27:18 $
 *---------------------------------------------------------------------*/
/*
 * makeverts.c -- Make verticies (and vertex lists)
 *                for warping cube into sphere
 *
 * After makestuff() is called, there will be VCNT verticies in
 * vcube[] and in vsphere[].
 * The display list mtglist[] contains the commands to draw the appropriate
 * triangles when the verticies are transferred (usually after a morph)
 * to the rsp_dynamic.mvlist array.  The order of the verticies must be
 * preserved when moving them to this array.
 *
 * Scale:  The cube is 2000 x 2000 currently, giving the sphere a radius
 *         of 1000.  The center of both is at (0,0,0).
 *         The radius of the sphere is made larger if BIGSPHERE is #define'd
 *         in morphdemo.h
 */

#include "morphdemo.h"

/*
 * Array containing entire texture w/all mipmap levels
 *
 * texture[i], where i is even, is a 33x33 top-level tile.
 * texture[i+1] contains the lower level mipmaps for texture[i].
 * texture[i] and the part of texture[i+1] which is used are loaded in
 * by a single load block command.  They form a Tmem image for a given
 * tile+mipmaps, and thus must be pre-swizzled.
 * The 2nd dimension of the array is actually 33*36 because of padding at
 * the ends of the rows required by the swizzling.
 * Not all of texture[i+1] is used -- the rest is just wasted space.
 * A more efficient program would avoid this wasted space.
 */
extern unsigned short texture[98][1188]; /* Mario, or batman */

/* NVE is the number of verticies along an edge of the cube
 * An odd number works best
 */
/* Moved to morphdemo.h: #define NVE 5  */

/* We do not share verticies from different faces, to provide the opportunity
 * to color or texture the faces differently
 *
 * VCNT = # of different verticies
 */
/* Moved to morphdemo.h: #define VCNT (NVE*NVE*7) */

/*
 * TCNTE = # of different triangles on an edge
 */
#define TCNTE ((NVE-1)*(NVE-1)*2)

Vtx vcube[VCNT];
Vtx vsphere[VCNT];

/*
 * We assume that the sphere "touches" the cube at the center points of
 * each face -- ie, the sphere radius equals 1/2 the cube edge length.
 * If this value is too small, problems will result from the quantization
 * due to the storage of the values as integers
 */

#define R 1000


/*
 * Texturing info
 * If we do texturing (DOTEX is defined), we assume that the texture is
 * 225 x 225 and that NVE is set to 8.
 * DOTEX is set in morphdemo.h
 */
#define TEXF  0      /* Face which has texture */ 

/* Verticies of a single face */
static int vfacex[NVE*NVE];
static int vfacey[NVE*NVE];
static int vfaces[NVE*NVE];
static int vfacet[NVE*NVE];

/* Declare size to be big to be safe */
#define MTGLISTSIZE (TCNTE*6*30)
Gfx mtglist[MTGLISTSIZE];  /* Morph triangle graphics display list */

/* Prototypes for functions in this file */
static void makecube(void);
static void makesphere(void);

/* Externals */
extern dynamic_stuff rsp_dynamic;  /* RSP-Addressable version of dynamic DL */


void makestuff() {

  makecube();   /* Cube verticies, plus the (common) list of verticies */
  makesphere(); /* Sphere verticies, computed from cube verticies      */

} /* makestuff */


/*
 * make the cube, and the triangle list
 */
static void makecube() {
  int ix, iy, i, j, f, tri, fb, v, nextload, mml, taddr, twidth, twidth64;
  float Rfloat, rdv;
  Gfx *l;
  Gfx *l2;

  /* Compute offset value */
  Rfloat = R;
  rdv = 2.0 * Rfloat / (NVE-1);

  /* First make verticies for a single face, setting only X and Y and
   * texture coordinates */
  for (iy=0; iy<NVE; iy++) {
    for (ix=0; ix<NVE; ix++) {
      vfacex[ix+NVE*iy] = ((float)ix) * rdv - Rfloat;
      vfacey[ix+NVE*iy] = ((float)iy) * rdv - Rfloat;
      vfaces[ix+NVE*iy] = (32*2*ix) << 5; /* S10.5 format,coords*2 */
      /* t is texel 32 for iy odd, texel 0 for iy even */
      vfacet[ix+NVE*iy] = (32*2*iy) << 5; /* S10.5 format,coords*2 */
    }
  }

  /*
   * Replicate the single face, adjusting appropriately
   */
  for (v = 0; v < NVE*NVE; v++) {
    vcube[v].v.ob[0] = vfacex[v];
    vcube[v].v.ob[1] = vfacey[v];
    vcube[v].v.ob[2] = R;
#ifdef DO_LIGHTING
    vcube[v].v.cn[0] = 0; vcube[v].v.cn[1] = 0;
    vcube[v].v.cn[2] = 127;
#else
    vcube[v].v.cn[0] = 0xff; vcube[v].v.cn[1] = 0x0; vcube[v].v.cn[2] = 0x0;
#endif
    vcube[v].v.cn[3] = 0xff; /* Alpha */
    vcube[v].v.tc[0] = vfaces[v];
    vcube[v].v.tc[1] = vfacet[v];
  }

  fb = NVE*NVE;  /* fb = "face base" */
  for (v = 0; v < NVE*NVE; v++) {
    vcube[fb+v].v.ob[0] = vfacex[v];
    vcube[fb+v].v.ob[1] = vfacey[v];
    vcube[fb+v].v.ob[2] = -R;
#ifdef DO_LIGHTING
    vcube[fb+v].v.cn[0] = 0; vcube[fb+v].v.cn[1] = 0;
    vcube[fb+v].v.cn[2] = -128;
#else
    vcube[fb+v].v.cn[0]=0xff;vcube[fb+v].v.cn[1]=0x0;vcube[fb+v].v.cn[2] = 0x0;
#endif
    vcube[fb+v].v.cn[3] = 0xff; /* Alpha */
    vcube[fb+v].v.tc[0] = vfaces[v];
    vcube[fb+v].v.tc[1] = vfacet[v];
  }

  fb = NVE*NVE*2;
  for (v = 0; v < NVE*NVE; v++) {
    vcube[fb+v].v.ob[0] = vfacex[v];
    vcube[fb+v].v.ob[1] = R;
    vcube[fb+v].v.ob[2] = vfacey[v];
#ifdef DO_LIGHTING
    vcube[fb+v].v.cn[0] = 0; vcube[fb+v].v.cn[1] = 127;
    vcube[fb+v].v.cn[2] = 0;
#else
    vcube[fb+v].v.cn[0]=0x0;vcube[fb+v].v.cn[1]=0xff;vcube[fb+v].v.cn[2] = 0x0;
#endif
    vcube[fb+v].v.cn[3] = 0xff; /* Alpha */
    vcube[fb+v].v.tc[0] = vfaces[v];
    vcube[fb+v].v.tc[1] = vfacet[v];
  }

  fb = NVE*NVE*3;
  for (v = 0; v < NVE*NVE; v++) {
    vcube[fb+v].v.ob[0] = vfacex[v];
    vcube[fb+v].v.ob[1] = -R;
    vcube[fb+v].v.ob[2] = vfacey[v];
#ifdef DO_LIGHTING
    vcube[fb+v].v.cn[0] = 0; vcube[fb+v].v.cn[1] = -128;
    vcube[fb+v].v.cn[2] = 0;
#else
    vcube[fb+v].v.cn[0]=0x0;vcube[fb+v].v.cn[1]=0xff;vcube[fb+v].v.cn[2] = 0x0;
#endif
    vcube[fb+v].v.cn[3] = 0xff; /* Alpha */
    vcube[fb+v].v.tc[0] = vfaces[v];
    vcube[fb+v].v.tc[1] = vfacet[v];
  }

  fb = NVE*NVE*4;
  for (v = 0; v < NVE*NVE; v++) {
    vcube[fb+v].v.ob[0] = R;
    vcube[fb+v].v.ob[1] = vfacex[v];
    vcube[fb+v].v.ob[2] = vfacey[v];
#ifdef DO_LIGHTING
    vcube[fb+v].v.cn[0] = 127; vcube[fb+v].v.cn[1] = 0;
    vcube[fb+v].v.cn[2] = 0;
#else
    vcube[fb+v].v.cn[0]=0x0;vcube[fb+v].v.cn[1]=0x0;vcube[fb+v].v.cn[2] = 0xff;
#endif
    vcube[fb+v].v.cn[3] = 0xff; /* Alpha */
    vcube[fb+v].v.tc[0] = vfaces[v];
    vcube[fb+v].v.tc[1] = vfacet[v];
  }

  fb = NVE*NVE*5;
  for (v = 0; v < NVE*NVE; v++) {
    vcube[fb+v].v.ob[0] = -R;
    vcube[fb+v].v.ob[1] = vfacex[v];
    vcube[fb+v].v.ob[2] = vfacey[v];
#ifdef DO_LIGHTING
    vcube[fb+v].v.cn[0] = -128; vcube[fb+v].v.cn[1] = 0;
    vcube[fb+v].v.cn[2] = 0;
#else
    vcube[fb+v].v.cn[0]=0x0;vcube[fb+v].v.cn[1]=0x0;vcube[fb+v].v.cn[2] = 0xff;
#endif
    vcube[fb+v].v.cn[3] = 0xff; /* Alpha */
    vcube[fb+v].v.tc[0] = vfaces[v];
    vcube[fb+v].v.tc[1] = vfacet[v];
  }

  /*
   * This code loads two rows of verticies at a time.  One of them
   * starts at RSP vertex 0, and the 2nd one at RSP vertex 8.
   * Because of this, it won't work for more than 8 verticies on a
   * cube edge.
   */

#ifdef DEBUG
  if (NVE > 8) {
    osSyncPrintf("Uh-oh.  This part of code won't work for that\n");
  }
#endif

  l = &(mtglist[0]);
#ifdef DOMM
  gSPTexture(l++, 0x8000, 0x8000, 5, 0, G_ON);
#else
  gSPTexture(l++, 0x8000, 0x8000, 0, 0, G_ON);
#endif
  for (f=0; f<6; f++) {  /* Each face */
    fb = NVE*NVE*f;    /* 1st vertex for this face */
    gSPVertex(l++, &(rsp_dynamic.mvlist[fb]), NVE, 0);  /* top row of verts */
    nextload = 8;
    for (i=0; i<NVE-1; i++) {  /* loop over rows */
      gSPVertex(l++, &(rsp_dynamic.mvlist[fb+NVE*(i+1)]), NVE, nextload);
      for (j=0; j<NVE-1; j++) {  /* loop throught columns */
#ifdef DOTEX
	/*
         * The following code is essentially doing a custom version of
	 * a gDPLoadTextureBlock, where the texture is pre-swizzled.
	 * If we're doing mipmapping, then all levels of mipmap are
         * loaded with one gDPLoadBlock command.
         *
         * Performance note:  We render the faces one at a time, which means
         * that since the texture on all faces is the same, we
         * load all of the texture tiles 6 times.  It would
         * probably be better to load each tile once, and rendering it
         * for all six faces.  We would loose some efficiency in the vertex
         * loads as a result, but the tradeoff would probably be favorable.
	 */
	/* Tell RDP where to load from RDRAM, plus texture size & width */
	gDPSetTextureImage(l++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1,
			   K0_TO_PHYS(&(texture[(j+i*(NVE-1))*2][0])));
	/* Tell RDP where to put texture in Tmem, ignore other stuff */
	gDPSetTile(l++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0 /* line size */,
		   0,             /* tmem addr */
		   0,             /* tile # */
		   0, 0, 0, 0, 0, 0, 0); /* misc -- set later */
	gDPLoadSync(l++); /* Make sure previous tex lookups are done */
	/* Do the actual load.  Count takes account of row pads to 64 bits.
         * Set DxT to zero to prevent 'automatic' swizzling, since we've
         * done it ourselves already */
#ifdef DOMM
	gDPLoadBlock(l++, 0 /* tile # */, 0, 0,
		     33*36 + 17*20 + 9*12 + 5*8 + 3*4 + 2*4 -1,/* TxCnt-1 */
		     0);
#else
	gDPLoadBlock(l++, 0 /* tile # */, 0, 0,
		     33*36 - 1, /* TxCnt-1 */
		     0);
#endif
	/* Set tile parameters correctly for texturing */
        /* This is the top-level tile */
	gDPSetTile(l++, G_IM_FMT_RGBA, G_IM_SIZ_16b,
		   (32 >> 2)+1, /* Line size, in 64-bit words */
		   0, 0, 0,     /* Tmem addr, tile #, palette--N/A) */
		   0, G_TX_NOMASK, G_TX_NOLOD, /* s: clamp/mirr, mask, shift */
		   0, G_TX_NOMASK, G_TX_NOLOD  /* t: clamp/mirr, mask, shift */
		   );
	/* Indicate what upper left and lower right (s,t) for tile are */
	gDPSetTileSize(l++, 0, (j*32)<<2, (i*32)<<2,
		       ((j+1)*32)<<2, ((i+1)*32)<<2);  /* 10.2 format */
#ifdef DOMM
	taddr = 33*36 >> 2;
	/* Loop through mip-map levels */
	for (mml = 1; mml<6; mml++) {
	  /* Probably could pull this command out of vertex loop */
	  gDPSetTile(l++, G_IM_FMT_RGBA, G_IM_SIZ_16b,
		     (((32 >> mml)+1) + 3)>>2, /* Line size,in 64-bit words*/
		     taddr,	/* Tmem addr */
		     mml, 0,	/* Tile #, palette--N/A) */
		     0, G_TX_NOMASK, mml, /* s: cl/mirr, mask, shift */
		     0, G_TX_NOMASK, mml /* t: cl/mirr, mask, shift */
		     );
	  /* Calculate address for next one */
	  twidth = (32 >> mml)+1;        /* width and height in texels */
	  twidth64 = (twidth+3) >> 2;    /* width in 64-bit chunks */
	  taddr += twidth64*twidth; /* width64 * height */
	  /* Indicate what upper left and lower right (s,t) for tile are */
	  gDPSetTileSize(l++, mml, ((j*32)<<2)>>mml, ((i*32)<<2)>>mml,
			 (((j+1)*32)<<2)>>mml,
			 (((i+1)*32)<<2)>>mml);  /* 10.2 format */
	}
#endif
#endif
	/* Need to get vertex order right for front vs. rear facing polys */
	if ((f==1) || (f==2) || (f==5)) {
	  /* correct order for faces 1, 2, 5 */
	  if (nextload == 8) {
	    gSP1Triangle(l++, j, j+8,   j+8+1, 0);
	    gSP1Triangle(l++, j, j+8+1, j+1,   0);
	  } else {
	    gSP1Triangle(l++, j+8, j,   j+1, 0);
	    gSP1Triangle(l++, j+8, j+1, j+9, 0);
	  }
	} else {
	  /* correct order for faces 0, 3, 4 */
	  if (nextload == 8) {
	    gSP1Triangle(l++, j, j+8+1, j+8,   0);
	    gSP1Triangle(l++, j, j+1,   j+8+1, 0);
	  } else {
	    gSP1Triangle(l++, j+8, j+1, j,   0);
	    gSP1Triangle(l++, j+8, j+9, j+1, 0);
	  }
	}
      }
      nextload ^= 8;
    }
  }
  gSPEndDisplayList(l++);

#ifdef DEBUG
  if (l > mtglist+MTGLISTSIZE) {  /* does this work?? */
    osSyncPrintf("Uh-Oh -- overflowed mtglist\n");
  }
#endif

} /* makecube */  


/*
 * makesphere -- generate sphere verticies from cube verticies
 */
static void makesphere(void) {
  int i;
  float x, y, z, d;
  signed char xc, yc, zc;

  for (i=0; i<VCNT; i++) {
    x = vcube[i].v.ob[0];
    y = vcube[i].v.ob[1];
    z = vcube[i].v.ob[2];
    d = sqrtf(x*x + y*y + z*z);
#ifdef BIGSPHERE
    vsphere[i].v.ob[0] = x/d * R * 1.7320;
    vsphere[i].v.ob[1] = y/d * R * 1.7320;
    vsphere[i].v.ob[2] = z/d * R * 1.7320;
#else
    vsphere[i].v.ob[0] = x/d * R;
    vsphere[i].v.ob[1] = y/d * R;
    vsphere[i].v.ob[2] = z/d * R;
#endif
#ifdef DO_LIGHTING
    /* Normals */
    vsphere[i].n.n[0] = x/d * 127;
    vsphere[i].n.n[1] = y/d * 127;
    vsphere[i].n.n[2] = z/d * 127;
    vsphere[i].n.a    = 0xff; /* Alpha */
#else
    /* Color = white */
    vsphere[i].v.cn[0] = 0x80;
    vsphere[i].v.cn[1] = 0x80;
    vsphere[i].v.cn[2] = 0x00;
    vsphere[i].v.cn[3] = 0xff; /* Alpha */
#endif
  }
}
