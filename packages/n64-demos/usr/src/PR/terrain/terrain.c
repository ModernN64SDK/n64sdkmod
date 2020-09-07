/**************************************************************************
 *                                                                        *
 *               Copyright (C) 1995, Silicon Graphics, Inc.               *
 *                                                                        *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright  law.  They  may not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *                                                                        *
 *************************************************************************/

/*
 * Terrain.c    Created Thu Feb  8 16:38:59 PST 1996 by acorn
 */

#include <sys/u64gio.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <ultrahost.h>
#include <unistd.h>
#include <ultra64.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include "shared.h"

char	cmd[4];
char terrain_dataname[100];
char terrain_headername[100];
char terrain_savename[100];
int  terrain_num=0;

typedef struct HostTexEntry {
	char dl_name[100];
	char file_name[100];
	int  levels;
} HostTexEntry;

#define MAXTEX 50
HostTexEntry hosttexlist[MAXTEX];
int numtex = 0;
char path[100] = "../Texture/";

#define ALLCULL_FLAG 1
#define PARTCULL_FLAG 2

/*
 * local functions
 */
void loadtexture(int fd, char *filename);

void render_texoff(FILE *fp)
{
	fprintf(fp,"\tgsDPPipeSync(),\n");
	fprintf(fp,"\tgsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),\n");
	fprintf(fp,"\tgsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),\n");
}

void render_texon(FILE *fp,int thetex)
{
	fprintf(fp,"\tgsDPPipeSync(),\n");
	fprintf(fp,"\tgsDPSetCombineMode(G_CC_TRILERP, G_CC_MODULATERGB2),\n");
	fprintf(fp,"\tgsSPTexture(0x8000, 0x8000, %d, G_TX_RENDERTILE, G_ON),\n",
						hosttexlist[thetex-1].levels);
}


void newtex(FILE *fp, int *lasttex, int *loadtex, int thetex)
{
    if (thetex > numtex) return;

    if (thetex) {
        if (!*lasttex)
            render_texon(fp,thetex);
    } else {
        if (*lasttex)
            render_texoff(fp);
    }
    if (thetex && (thetex != *loadtex)) {
	fprintf(fp,"\tgsDPPipeSync(),\n");
	fprintf(fp,"\tgsSPDisplayList( %s ),\n",hosttexlist[thetex-1].dl_name);
        *loadtex = thetex;
    }
    *lasttex = thetex;
}

void saveterrain(int fd, int flags)
{
    u64		buffer[(sizeof(VertS)+7)/8];
    VertS	*v,*verts;
    u32		dimx,dimz;
    s16		minx=0x7fff, maxx=-0x7fff;
    s16		miny=0x7fff, maxy=-0x7fff;
    s16		minz=0x7fff, maxz=-0x7fff;
    int		i,j,k;
    int		section;
    s16		x,y,z;
    FILE	*fp;
    int		lasttex=0,loadtex=0;

    if (uhReadGame(fd, buffer, sizeof(buffer)) == -1) {
        fprintf(stderr, "uhReadGame %s\n", sys_errlist[errno]);
        return;
    }
    dimx=((u32 *)buffer)[0];
    dimz=((u32 *)buffer)[1];
    verts = (VertS *) malloc(sizeof(VertS) * dimx * dimz);
    v = verts;

    fprintf(stderr,"Getting vertices from U64\n");
    if (!(fp=fopen(terrain_savename,"w"))) {
        fprintf(stderr, "error opening file: %s\n",terrain_savename);
	return;
    }
    fprintf(fp,"This file created by the terrain program\n");
    fprintf(fp,"This file is data for reloading back into \n");
    fprintf(fp,"the terrain program\n");
    fprintf(fp,"\n");
    fprintf(fp,"(%d %d)\n",dimx,dimz);
    for (i=0; i<dimx; i++) {
	fprintf(stderr,"  %d%%\n",i*100/dimx);
        for (j=0; j<dimz; j++) {
    	    if (uhReadGame(fd, buffer, sizeof(buffer)) == -1) {
        	fprintf(stderr, "uhReadGame %s\n", sys_errlist[errno]);
        	return;
    	    }
	    v  = &verts[dimx*j + i];
	    *v = *((VertS *) &buffer);
    	    fprintf(fp,"{%d %d %d %d %d %d %d %d %d %d %d}\n",
		v->altitude,
		v->normal[0],
		v->normal[1],
		v->normal[2],
		v->texture[0],
		v->texture[1],
		v->flags,
		v->offset[0],
		v->offset[1],
		v->s,
		v->t);
        }
    }
    fprintf(fp,"[%d\n",numtex);
    for (i=0; i<numtex; i++) {
	fprintf(fp,"[%s\n",hosttexlist[i].file_name);
    }
    fprintf(fp,"\n");
    fclose(fp);

    if (!(fp=fopen(terrain_headername,"w"))) {
        fprintf(stderr, "error opening file: %s\n",terrain_headername);
	return;
    }
    fprintf(fp,"/*\n");
    fprintf(fp," * This file created by the terrain program\n");
    fprintf(fp," * This is the header file with the data structure names\n");
    fprintf(fp," */\n");
    fprintf(fp,"\n");
    fprintf(fp,"#ifndef TERRAIN%d_HEADER\n",terrain_num);
    fprintf(fp,"#define TERRAIN%d_HEADER\n",terrain_num);
    fprintf(fp,"\n");
    fprintf(fp,"extern int terrain%d_dimx;\n",terrain_num);
    fprintf(fp,"extern int terrain%d_dimz;\n",terrain_num);
    fprintf(fp,"extern Vtx terrain%d_vtx[];\n",terrain_num);
    fprintf(fp,"extern Gfx terrain%d_dl[];\n",terrain_num);
    fprintf(fp,"\n");
    fprintf(fp,"/*\n");
    fprintf(fp," * The following textures are required:\n");
    fprintf(fp," */\n");
    for (i=0; i<numtex; i++) {
	fprintf(fp,"extern Gfx %s[];\t/* file: %s */\n",
				hosttexlist[i].dl_name,
				hosttexlist[i].file_name);
    }
    fprintf(fp,"\n");
    fprintf(fp,"#endif /* TERRAIN%d_HEADER */\n",terrain_num);
    fclose(fp);

    v=verts;
    fprintf(stderr,"dimensions: (%d,%d)\n",dimx, dimz);
    fprintf(stderr,"Writing vertices\n");
    if (!(fp=fopen(terrain_dataname,"w"))) {
        fprintf(stderr, "error opening file: %s\n",terrain_dataname);
	return;
    }
    fprintf(fp,"/*\n");
    fprintf(fp," * This file created by the terrain program\n");
    fprintf(fp," * This is the vertices for the terrain\n");
    fprintf(fp," */\n");
    fprintf(fp,"\n");
    fprintf(fp,"/* dimensions: (%d,%d) */\n",dimx, dimz);
    fprintf(fp,"\n");
    fprintf(fp,"int terrain%d_dimx=%d;\n",terrain_num,dimx);
    fprintf(fp,"int terrain%d_dimz=%d;\n",terrain_num,dimz);
    fprintf(fp,"\n");
    fprintf(fp,"Vtx terrain%d_vtx[%d] = {\n",terrain_num,dimx*dimz);

    for (i=0; i<dimx; i++) {
    for (j=0; j<dimz; j++) {
	x = i*SIDE2X-WORLD_X/2 + (verts + dimx*j + i)->offset[0];
	y =                      (verts + dimx*j + i)->altitude;
	z = j*SIDE2Z-WORLD_Z/2 + (verts + dimx*j + i)->offset[1];
	maxx = MAX(maxx, x);
	minx = MIN(minx, x);
	maxy = MAX(maxy, y);
	miny = MIN(miny, y);
	maxz = MAX(maxz, z);
	minz = MIN(minz, z);
	fprintf(fp,"\t{ %5d,%5d,%5d,  %d, 0x%04x, 0x%04x,  %4d, %4d, %4d, %3d, },\n",
		x,
		y,
		z,
		0,
		((verts + dimx*j + i)->s)&0xffff,
		((verts + dimx*j + i)->t)&0xffff,
		(verts + dimx*j + i)->normal[0],
		(verts + dimx*j + i)->normal[1],
		(verts + dimx*j + i)->normal[2],
		255
		);
	v++;
    }
    }
    fprintf(fp,"};\n");
    fprintf(fp,"\n");

    if (flags & ALLCULL_FLAG) {
        fprintf(fp,"Vtx terrain%d_allcullvtx[8] = {\n",terrain_num);
	fprintf(fp,"\t{ %5d,%5d,%5d,  0,0,0,0,0,0,0, },\n",minx,miny,minz);
	fprintf(fp,"\t{ %5d,%5d,%5d,  0,0,0,0,0,0,0, },\n",maxx,miny,minz);
	fprintf(fp,"\t{ %5d,%5d,%5d,  0,0,0,0,0,0,0, },\n",minx,miny,maxz);
	fprintf(fp,"\t{ %5d,%5d,%5d,  0,0,0,0,0,0,0, },\n",maxx,miny,maxz);
	fprintf(fp,"\t{ %5d,%5d,%5d,  0,0,0,0,0,0,0, },\n",minx,maxy,minz);
	fprintf(fp,"\t{ %5d,%5d,%5d,  0,0,0,0,0,0,0, },\n",maxx,maxy,minz);
	fprintf(fp,"\t{ %5d,%5d,%5d,  0,0,0,0,0,0,0, },\n",minx,maxy,maxz);
	fprintf(fp,"\t{ %5d,%5d,%5d,  0,0,0,0,0,0,0, },\n",maxx,maxy,maxz);
        fprintf(fp,"};\n");
        fprintf(fp,"\n");
    }

    if (flags & PARTCULL_FLAG) {
        fprintf(fp,"Vtx terrain%d_partcullvtx[%d] = {\n",terrain_num,
				(((dimx-2)/7)+2)*(((dimz-2)/7)+2)*2);
        for (i=0; i<dimx+6; i+=7) {
	  if (i>=dimx) i=dimx-1;
          for (j=0; j<dimz+6; j+=7) {
	    int ii, jj;
	    if (j>=dimx) j=dimx-1;
	    miny = 0x7fff;
	    maxy = -0x7fff;
	    for (jj = j-7; jj<=j+7; jj++)  {
	      for (ii = i-7; ii<=i+7; ii++) {
		if (ii>=0 && jj>=0 && ii<dimx && jj<dimz) {
	          y = (verts + dimx*jj + ii)->altitude;
		  maxy = MAX(maxy, y);
		  miny = MIN(miny, y);
		}
	      }
	    }

	    fprintf(fp,"\t{ %5d,%5d,%5d,  0,0,0,0,0,0,0, },\n",
		(int)(i*SIDE2X-WORLD_X/2),
		miny,
		(int)(j*SIDE2Z-WORLD_Z/2));
	    fprintf(fp,"\t{ %5d,%5d,%5d,  0,0,0,0,0,0,0, },\n",
		(int)(i*SIDE2X-WORLD_X/2),
		maxy,
		(int)(j*SIDE2Z-WORLD_Z/2));
	  }
	}

        fprintf(fp,"};\n");
    }

    fprintf(fp,"\n");

    fprintf(stderr,"Writing display lists\n");

    fprintf(fp,"/*\n");
    fprintf(fp," * This file created by the terrain program\n");
    fprintf(fp," * This is the triangles of the terrain\n");
    fprintf(fp," */\n");
    fprintf(fp,"\n");
    fprintf(fp,"/* dimensions: (%d,%d) */\n",dimx, dimz);
    fprintf(fp,"\n");
    if (!(flags & PARTCULL_FLAG)) {
    	fprintf(fp,"Gfx terrain%d_dl[] = {\n",terrain_num);
	render_texoff(fp);
        if (flags & ALLCULL_FLAG) {
	    fprintf(fp,"\tgsSPVertex(&(terrain%d_allcullvtx[0]),8,0),\n",terrain_num);
	    fprintf(fp,"\tgsSPCullDisplayList(0,7),\n");
        }
    }

    section = 0;
    for (j=0; j<dimz-1; j+= 7) {
	k = MIN(dimz-j,8);
	if (!(flags & PARTCULL_FLAG)) {
	    fprintf(fp,"\tgsSPVertex(&(terrain%d_vtx[%d]),%d,0),\n",terrain_num,j,k);
	}
    	for (i=0; i<dimx-1; i+= 2) {
	    k = MIN(dimz-j,8);
	    if ((flags & PARTCULL_FLAG) && (!(i%7))) {
		if (section) {
    		    fprintf(fp,"\tgsSPEndDisplayList(),\n");
    		    fprintf(fp,"};\n");
    		    fprintf(fp,"\n");
		}
		fprintf(fp,"Gfx terrain%d_sec%d_dl[] = {\n",terrain_num,section++);
		loadtex = 0;
		lasttex = 0;
	        fprintf(fp,"\tgsSPVertex(&(terrain%d_partcullvtx[%d]),4,0),\n",
						terrain_num,2*((i/7)*(((dimz-2)/7)+2)+(j/7)));
	        fprintf(fp,"\tgsSPVertex(&(terrain%d_partcullvtx[%d]),4,4),\n",
						terrain_num,2*(((i/7)+1)*(((dimz-2)/7)+2)+(j/7)));
	        fprintf(fp,"\tgsSPCullDisplayList(0,7),\n");
	        fprintf(fp,"\tgsSPVertex(&(terrain%d_vtx[%d]),%d,0),\n",
							terrain_num,((i)*dimz)+j,k);
	    }
	    fprintf(fp,"\tgsSPVertex(&(terrain%d_vtx[%d]),%d,8),\n",
							terrain_num,((i+1)*dimz)+j,k);
	    switch(k) {
              case 8:
                if ((verts + dimx*(j+6) + (i))->texture[0] != lasttex) {
                    newtex(fp, &lasttex, &loadtex,
                            (verts + dimx*(j+6) + (i))->texture[0]);
                }
                if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+6) + (i))->flags) {
                    fprintf(fp,"\tgsSP1Triangle( 14, 6, 7, 0),\n");
                } else {
                    fprintf(fp,"\tgsSP1Triangle( 14, 6,15, 2),\n");
                }
                if ((verts + dimx*(j+6) + (i))->texture[1] != lasttex) {
                    newtex(fp, &lasttex, &loadtex,
                            (verts + dimx*(j+6) + (i))->texture[1]);
                }
                if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+6) + (i))->flags) {
                    fprintf(fp,"\tgsSP1Triangle( 15,14, 7, 2),\n");
                } else {
                    fprintf(fp,"\tgsSP1Triangle( 15, 6, 7, 0),\n");
                }
              case 7:
                if ((verts + dimx*(j+5) + (i))->texture[0] != lasttex) {
                    newtex(fp, &lasttex, &loadtex,
                            (verts + dimx*(j+5) + (i))->texture[0]);
                }
                if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+5) + (i))->flags) {
                    fprintf(fp,"\tgsSP1Triangle( 13, 5, 6, 0),\n");
                } else {
                    fprintf(fp,"\tgsSP1Triangle( 13, 5,14, 2),\n");
                }
                if ((verts + dimx*(j+5) + (i))->texture[1] != lasttex) {
                    newtex(fp, &lasttex, &loadtex,
                            (verts + dimx*(j+5) + (i))->texture[1]);
                }
                if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+5) + (i))->flags) {
                    fprintf(fp,"\tgsSP1Triangle( 14,13, 6, 2),\n");
                } else {
                    fprintf(fp,"\tgsSP1Triangle( 14, 5, 6, 0),\n");
                }
              case 6:
                if ((verts + dimx*(j+4) + (i))->texture[0] != lasttex) {
                    newtex(fp, &lasttex, &loadtex,
                            (verts + dimx*(j+4) + (i))->texture[0]);
                }
                if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+4) + (i))->flags) {
                    fprintf(fp,"\tgsSP1Triangle( 12, 4, 5, 0),\n");
                } else {
                    fprintf(fp,"\tgsSP1Triangle( 12, 4,13, 2),\n");
                }
                if ((verts + dimx*(j+4) + (i))->texture[1] != lasttex) {
                    newtex(fp, &lasttex, &loadtex,
                            (verts + dimx*(j+4) + (i))->texture[1]);
                }
                if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+4) + (i))->flags) {
                    fprintf(fp,"\tgsSP1Triangle( 13,12, 5, 2),\n");
                } else {
                    fprintf(fp,"\tgsSP1Triangle( 13, 4, 5, 0),\n");
                }
              case 5:
                if ((verts + dimx*(j+3) + (i))->texture[0] != lasttex) {
                    newtex(fp, &lasttex, &loadtex,
                            (verts + dimx*(j+3) + (i))->texture[0]);
                }
                if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+3) + (i))->flags) {
                    fprintf(fp,"\tgsSP1Triangle( 11, 3, 4, 0),\n");
                } else {
                    fprintf(fp,"\tgsSP1Triangle( 11, 3,12, 2),\n");
                }
                if ((verts + dimx*(j+3) + (i))->texture[1] != lasttex) {
                    newtex(fp, &lasttex, &loadtex,
                            (verts + dimx*(j+3) + (i))->texture[1]);
                }
                if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+3) + (i))->flags) {
                    fprintf(fp,"\tgsSP1Triangle( 12,11, 4, 2),\n");
                } else {
                    fprintf(fp,"\tgsSP1Triangle( 12, 3, 4, 0),\n");
                }
              case 4:
                if ((verts + dimx*(j+2) + (i))->texture[0] != lasttex) {
                    newtex(fp, &lasttex, &loadtex,
                            (verts + dimx*(j+2) + (i))->texture[0]);
                }
                if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+2) + (i))->flags) {
                    fprintf(fp,"\tgsSP1Triangle( 10, 2, 3, 0),\n");
                } else {
                    fprintf(fp,"\tgsSP1Triangle( 10, 2,11, 2),\n");
                }
                if ((verts + dimx*(j+2) + (i))->texture[1] != lasttex) {
                    newtex(fp, &lasttex, &loadtex,
                            (verts + dimx*(j+2) + (i))->texture[1]);
                }
                if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+2) + (i))->flags) {
                    fprintf(fp,"\tgsSP1Triangle( 11,10, 3, 2),\n");
                } else {
                    fprintf(fp,"\tgsSP1Triangle( 11, 2, 3, 0),\n");
                }
              case 3:
                if ((verts + dimx*(j+1) + (i))->texture[0] != lasttex) {
                    newtex(fp, &lasttex, &loadtex,
                            (verts + dimx*(j+1) + (i))->texture[0]);
                }
                if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+1) + (i))->flags) {
                    fprintf(fp,"\tgsSP1Triangle(  9, 1, 2, 0),\n");
                } else {
                    fprintf(fp,"\tgsSP1Triangle(  9, 1,10, 2),\n");
                }
                if ((verts + dimx*(j+1) + (i))->texture[1] != lasttex) {
                    newtex(fp, &lasttex, &loadtex,
                            (verts + dimx*(j+1) + (i))->texture[1]);
                }
                if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+1) + (i))->flags) {
                    fprintf(fp,"\tgsSP1Triangle( 10, 9, 2, 2),\n");
                } else {
                    fprintf(fp,"\tgsSP1Triangle( 10, 1, 2, 0),\n");
                }
              case 2:
                if ((verts + dimx*(j) + (i))->texture[0] != lasttex) {
                    newtex(fp, &lasttex, &loadtex,
                            (verts + dimx*(j) + (i))->texture[0]);
                }
                if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j) + (i))->flags) {
                    fprintf(fp,"\tgsSP1Triangle(  8, 0, 1, 0),\n");
                } else {
                    fprintf(fp,"\tgsSP1Triangle(  8, 0, 9, 2),\n");
                }
                if ((verts + dimx*(j) + (i))->texture[1] != lasttex) {
                    newtex(fp, &lasttex, &loadtex,
                            (verts + dimx*(j) + (i))->texture[1]);
                }
                if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j) + (i))->flags) {
                    fprintf(fp,"\tgsSP1Triangle(  9, 8, 1, 2),\n");
                } else {
                    fprintf(fp,"\tgsSP1Triangle(  9, 0, 1, 0),\n");
                }
	    }

		
	    
	    if ((flags & PARTCULL_FLAG) && (!((i+1)%7))) {
		i-=1;
	    } else if (i+2<dimx) {
	      fprintf(fp,"\tgsSPVertex(&(terrain%d_vtx[%d]),%d,0),\n",
							terrain_num,((i+2)*dimz)+j,k);

	      switch(k) {
                case 8:
                  if ((verts + dimx*(j+6) + (i+1))->texture[0] != lasttex) {
                      newtex(fp, &lasttex, &loadtex,
                              (verts + dimx*(j+6) + (i+1))->texture[0]);
                  }
                  if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+6) + (i+1))->flags) {
                      fprintf(fp,"\tgsSP1Triangle(  6,14,15, 0),\n");
                  } else {
                      fprintf(fp,"\tgsSP1Triangle(  6,14, 7, 2),\n");
                  }
                  if ((verts + dimx*(j+6) + (i+1))->texture[1] != lasttex) {
                      newtex(fp, &lasttex, &loadtex,
                              (verts + dimx*(j+6) + (i+1))->texture[1]);
                  }
                  if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+6) + (i+1))->flags) {
                      fprintf(fp,"\tgsSP1Triangle(  7, 6,15, 2),\n");
                  } else {
                      fprintf(fp,"\tgsSP1Triangle(  7,14,15, 0),\n");
                  }
                case 7:
                  if ((verts + dimx*(j+5) + (i+1))->texture[0] != lasttex) {
                      newtex(fp, &lasttex, &loadtex,
                              (verts + dimx*(j+5) + (i+1))->texture[0]);
                  }
                  if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+5) + (i+1))->flags) {
                      fprintf(fp,"\tgsSP1Triangle(  5,13,14, 0),\n");
                  } else {
                      fprintf(fp,"\tgsSP1Triangle(  5,13, 6, 2),\n");
                  }
                  if ((verts + dimx*(j+5) + (i+1))->texture[1] != lasttex) {
                      newtex(fp, &lasttex, &loadtex,
                              (verts + dimx*(j+5) + (i+1))->texture[1]);
                  }
                  if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+5) + (i+1))->flags) {
                      fprintf(fp,"\tgsSP1Triangle(  6, 5,14, 2),\n");
                  } else {
                      fprintf(fp,"\tgsSP1Triangle(  6,13,14, 0),\n");
                  }
                case 6:
                  if ((verts + dimx*(j+4) + (i+1))->texture[0] != lasttex) {
                      newtex(fp, &lasttex, &loadtex,
                              (verts + dimx*(j+4) + (i+1))->texture[0]);
                  }
                  if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+4) + (i+1))->flags) {
                      fprintf(fp,"\tgsSP1Triangle(  4,12,13, 0),\n");
                  } else {
                      fprintf(fp,"\tgsSP1Triangle(  4,12, 5, 2),\n");
                  }
                  if ((verts + dimx*(j+4) + (i+1))->texture[1] != lasttex) {
                      newtex(fp, &lasttex, &loadtex,
                              (verts + dimx*(j+4) + (i+1))->texture[1]);
                  }
                  if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+4) + (i+1))->flags) {
                      fprintf(fp,"\tgsSP1Triangle(  5, 4,13, 2),\n");
                  } else {
                      fprintf(fp,"\tgsSP1Triangle(  5,12,13, 0),\n");
                  }
                case 5:
                  if ((verts + dimx*(j+3) + (i+1))->texture[0] != lasttex) {
                      newtex(fp, &lasttex, &loadtex,
                              (verts + dimx*(j+3) + (i+1))->texture[0]);
                  }
                  if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+3) + (i+1))->flags) {
                      fprintf(fp,"\tgsSP1Triangle(  3,11,12, 0),\n");
                  } else {
                      fprintf(fp,"\tgsSP1Triangle(  3,11, 4, 2),\n");
                  }
                  if ((verts + dimx*(j+3) + (i+1))->texture[1] != lasttex) {
                      newtex(fp, &lasttex, &loadtex,
                              (verts + dimx*(j+3) + (i+1))->texture[1]);
                  }
                  if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+3) + (i+1))->flags) {
                      fprintf(fp,"\tgsSP1Triangle(  4, 3,12, 2),\n");
                  } else {
                      fprintf(fp,"\tgsSP1Triangle(  4,11,12, 0),\n");
                  }
                case 4:
                  if ((verts + dimx*(j+2) + (i+1))->texture[0] != lasttex) {
                      newtex(fp, &lasttex, &loadtex,
                              (verts + dimx*(j+2) + (i+1))->texture[0]);
                  }
                  if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+2) + (i+1))->flags) {
                      fprintf(fp,"\tgsSP1Triangle(  2,10,11, 0),\n");
                  } else {
                      fprintf(fp,"\tgsSP1Triangle(  2,10, 3, 2),\n");
                  }
                  if ((verts + dimx*(j+2) + (i+1))->texture[1] != lasttex) {
                      newtex(fp, &lasttex, &loadtex,
                              (verts + dimx*(j+2) + (i+1))->texture[1]);
                  }
                  if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+2) + (i+1))->flags) {
                      fprintf(fp,"\tgsSP1Triangle(  3, 2,11, 2),\n");
                  } else {
                      fprintf(fp,"\tgsSP1Triangle(  3,10,11, 0),\n");
                  }
                case 3:
                  if ((verts + dimx*(j+1) + (i+1))->texture[0] != lasttex) {
                      newtex(fp, &lasttex, &loadtex,
                              (verts + dimx*(j+1) + (i+1))->texture[0]);
                  }
                  if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+1) + (i+1))->flags) {
                      fprintf(fp,"\tgsSP1Triangle(  1, 9,10, 0),\n");
                  } else {
                      fprintf(fp,"\tgsSP1Triangle(  1, 9, 2, 2),\n");
                  }
                  if ((verts + dimx*(j+1) + (i+1))->texture[1] != lasttex) {
                      newtex(fp, &lasttex, &loadtex,
                              (verts + dimx*(j+1) + (i+1))->texture[1]);
                  }
                  if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j+1) + (i+1))->flags) {
                      fprintf(fp,"\tgsSP1Triangle(  2, 1,10, 2),\n");
                  } else {
                      fprintf(fp,"\tgsSP1Triangle(  2, 9,10, 0),\n");
                  }
                case 2:
                  if ((verts + dimx*(j) + (i+1))->texture[0] != lasttex) {
                      newtex(fp, &lasttex, &loadtex,
                              (verts + dimx*(j) + (i+1))->texture[0]);
                  }
                  if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j) + (i+1))->flags) {
                      fprintf(fp,"\tgsSP1Triangle(  0, 8, 9, 0),\n");
                  } else {
                      fprintf(fp,"\tgsSP1Triangle(  0, 8, 1, 2),\n");
                  }
                  if ((verts + dimx*(j) + (i+1))->texture[1] != lasttex) {
                      newtex(fp, &lasttex, &loadtex,
                              (verts + dimx*(j) + (i+1))->texture[1]);
                  }
                  if (VERT_FLAG_SWAPDIAG & (verts + dimx*(j) + (i+1))->flags) {
                      fprintf(fp,"\tgsSP1Triangle(  1, 0, 9, 2),\n");
                  } else {
                      fprintf(fp,"\tgsSP1Triangle(  1, 8, 9, 0),\n");
                  }
	      }
	    }

	}
    }
    fprintf(fp,"\tgsSPEndDisplayList(),\n");
    fprintf(fp,"};\n");
    fprintf(fp,"\n");


    if (flags & PARTCULL_FLAG) {
        fprintf(fp,"Gfx terrain%d_dl[] = {\n",terrain_num);
	render_texoff(fp);
        if (flags & ALLCULL_FLAG) {
	    fprintf(fp,"\tgsSPVertex(&(terrain%d_allcullvtx[0]),8,0),\n",terrain_num);
	    fprintf(fp,"\tgsSPCullDisplayList(0,7),\n");
        }
	for (i=0; i<section; i++) {
	    fprintf(fp,"\tgsSPDisplayList( terrain%d_sec%d_dl ),\n",terrain_num,i);
	    
	}
        fprintf(fp,"\tgsSPEndDisplayList(),\n");
        fprintf(fp,"};\n");
        fprintf(fp,"\n");
    }




    fclose(fp);
    free(verts);

    fprintf(stderr,"Save done!\n");

}

void loadterrain(int fd)
{
    u64		buffer[(sizeof(VertS)+7)/8];
    int		buf2[11];
    VertS	*v;
    u32		dimx,dimz;
    int		i,j,c,addtex;
    FILE	*fp;
    char	savepath;


    if (!(fp=fopen(terrain_savename,"r"))) {
        fprintf(stderr, "error opening file: %s\n",terrain_savename);
	return;
    }

    c=getc(fp);
    while(c!='(' && c!=EOF) {
	c=getc(fp);
    }

    fscanf(fp,"%i %i",&dimx,&dimz);
    fprintf(stderr,"dimensions: (%d,%d)\n",dimx, dimz);

    ((u32 *)buffer)[0] = dimx;
    ((u32 *)buffer)[1] = dimz;

    if (uhWriteGame(fd, buffer, sizeof(buffer)) == -1) {
        fprintf(stderr, "uhWriteGame %s\n", sys_errlist[errno]);
        return;
    }

    fprintf(stderr,"Reading vertices & sending to U64\n");
    v=(VertS *) buffer;
    for (i=0; i<dimx; i++) {
	fprintf(stderr,"  %d%%\n",i*100/dimx);
        for (j=0; j<dimz; j++) {
		c=getc(fp);
		while (c!='{' && c!=EOF) { 	/*}*/
		    c=getc(fp);
		}
		if (c==EOF) {
		    fprintf(stderr,"bad input file - too short\n");
		    exit(0);
		}
		fscanf(fp,"%i %i %i %i %i %i %i %i %i %i %i",
		    &buf2[0],
		    &buf2[1],
		    &buf2[2],
		    &buf2[3],
		    &buf2[4],
		    &buf2[5],
		    &buf2[6],
		    &buf2[7],
		    &buf2[8],
		    &buf2[9],
		    &buf2[10]
		    );

		v->altitude 	= buf2[0];
		v->normal[0] 	= buf2[1];
		v->normal[1] 	= buf2[2];
		v->normal[2]	= buf2[3];
		v->texture[0] 	= buf2[4];
		v->texture[1] 	= buf2[5];
		v->flags 	= buf2[6];
		v->offset[0] 	= buf2[7];
		v->offset[1] 	= buf2[8];
		v->s 		= buf2[9];
		v->t 		= buf2[10];

    		if (uhWriteGame(fd, buffer, sizeof(buffer)) == -1) {
        	    fprintf(stderr, "uhWriteGame %s\n", sys_errlist[errno]);
        	    return;
    		}
	}
    }

    c=getc(fp);
    while(c!='[' && c!=EOF) {
	c=getc(fp);
    }
    fscanf(fp,"%i",&addtex);
    buffer[0] = addtex;
    if (uhWriteGame(fd, buffer, 8) == -1) {
        fprintf(stderr, "uhWriteGame %s\n", sys_errlist[errno]);
        return;
    }
    numtex = 0;
    savepath=path[0];
    path[0]='\0';
    for (i=0; i<addtex; i++) {
        c=getc(fp);
        while(c!='[' && c!=EOF) {
	    c=getc(fp);
        }
	fscanf(fp,"%s",&hosttexlist[numtex].file_name);
	loadtexture(fd,hosttexlist[numtex].file_name);
    }
    path[0]=savepath;

    fclose(fp);
    fprintf(stderr,"Terrain Load done!\n");
}

#define MAXLINE 200
int getline(FILE *fp,char *buf)
{
        int c,n;
        char *bp;

        bp=buf;
        n=MAXLINE;
        c=getc(fp);
        while (c!=EOF && c!='\n' && n>1) {
                *(bp++)=c;
                n--;
                c=getc(fp);
        }
        *(bp++)='\0';
        if (c==EOF) return 1;
        return 0;
}

int checkword(char *line,char *word)
{
        int n;
        n= strlen(word);
        while(*line) {
                if (!strncmp(line,word,n))
                        return 1;
                line++;
        }
        return 0;
}

void badsend(int fd) {
    u64 buf;

    numtex--;
    buf = 0xfffffffe;
    if (uhWriteGame(fd, &buf, 8) == -1) {
        fprintf(stderr, "uhWriteGame %s\n", sys_errlist[errno]);
        return;
    }
}

/* 
 * the following characters are commands to getfilename:
 *  ?       - list directory
 *  ..      - up one directory
 *  dirname - go to directory
 *
 * otherwise the word is returned as typed
 */
void getfilename(char *name, char *prompt, char *lsstr)
{
    int 	done=0;
    pid_t       lspid;
    int         lsstat,i,done2;
    FILE	*fp;
    char 	line[300],*c,*s;

    while(!done) {
	printf("path = %s\n",path);
	printf("%s",prompt);
	scanf("%s",name);
	if (name[0] == '?' && name[1]=='\0') {
            if ((lspid = fork()) == -1) {
                fprintf(stderr, "host: fork: %s\n", sys_errlist[errno]);
                exit(0);
            } else if (lspid == 0) {
                (void)execl("./terrainls", "./terrainls", path, 0);
                fprintf(stderr, "host: execl(\"ls\") failed\n");
                exit(0);
            }
	    (void) wait(&lsstat);
printf("stat=%x  WIFEXITED=%d  WEXITSTATUS=%d\n",lsstat,WIFEXITED(lsstat),WEXITSTATUS(lsstat));
	    if (!(fp=fopen("terrainfile.file","r"))) {
		printf("could not open file terrainfile.file\n");
		strcpy(name,"/");
		return;
	    }
	    done2 = 0;
	    while (!done2) {
	        done2 = getline(fp,line);
	        for (c=line; (*c!=' ' && *c!='\t') && *c!='\0'; c++);
	        for (; (*c==' ' || *c=='\t') && *c!='\0'; c++);
	        for (; (*c!=' ' && *c!='\t') && *c!='\0'; c++);
	        for (; (*c==' ' || *c=='\t') && *c!='\0'; c++);
	        for (; (*c!=' ' && *c!='\t') && *c!='\0'; c++);
	        for (; (*c==' ' || *c=='\t') && *c!='\0'; c++);
	        for (; (*c!=' ' && *c!='\t') && *c!='\0'; c++);
	        for (; (*c==' ' || *c=='\t') && *c!='\0'; c++);
	        for (; (*c!=' ' && *c!='\t') && *c!='\0'; c++);
	        for (; (*c==' ' || *c=='\t') && *c!='\0'; c++);
	        for (; (*c!=' ' && *c!='\t') && *c!='\0'; c++);
	        for (; (*c==' ' || *c=='\t') && *c!='\0'; c++);
	        for (; (*c!=' ' && *c!='\t') && *c!='\0'; c++);
	        for (; (*c==' ' || *c=='\t') && *c!='\0'; c++);
	        for (; (*c!=' ' && *c!='\t') && *c!='\0'; c++);
	        for (; (*c==' ' || *c=='\t') && *c!='\0'; c++);
	        s=c; /* filename start */
	        if (c != '\0') {
	            for (; (*c!=' ' && *c!='\t') && *c!='\0'; c++);
		    if (*(c-1)=='/') {
			printf("%s\n",s);
		    }
	        }
	    }
	    fclose (fp);
            if ((lspid = fork()) == -1) {
                fprintf(stderr, "host: fork: %s\n", sys_errlist[errno]);
                exit(0);
            } else if (lspid == 0) {
                (void)execl(lsstr, lsstr, path, 0);
                fprintf(stderr, "host: execl(\"ls\") failed\n");
                exit(0);
            }
	    (void) wait(&lsstat);
        } else if (name[0] == '.' && name[1]=='.' && name[2]=='\0') {
	    if (path[strlen(path)-1] == '/') path[strlen(path)-1] = '\0';

	    for (i=strlen(path); i!=0 && path[i]!='/' && (path[i]!='.' || path[i+1]!='.'); i--);
	    if (strlen(path)==0) {
		strcpy(path,"..");
	    } else if (path[i] =='.' && path[i+1]=='.') {
		path[i+2]='/';
		path[i+3]='.';
		path[i+4]='.';
		path[i+5]='\0';
	    } else if (path[i] == '/') {
		path[i] = '\0';
	    } else {
		path[0] = '\0';
	    }
	} else if (name[0] == '/' && name[1]=='\0') {
	    return;
	} else {
	    i=strlen(path);
	    strcat(path,"/");
	    strcat(path,name);
            if ((lspid = fork()) == -1) {
                fprintf(stderr, "host: fork: %s\n", sys_errlist[errno]);
                exit(0);
            } else if (lspid == 0) {
                (void)execl("./terrainfile", "./terrainfile", path, 0);
                fprintf(stderr, "host: execl(\"file\") failed\n");
                exit(0);
            }
	    (void) wait(&lsstat);
printf("stat=%x  WIFEXITED=%d  WEXITSTATUS=%d\n",lsstat,WIFEXITED(lsstat),WEXITSTATUS(lsstat));
	    if (!(fp=fopen("terrainfile.file","r"))) {
		printf("could not open file terrainfile.file\n");
		strcpy(name,"/");
		return;
	    }
	    (void) getline(fp,line);
	    fclose(fp);
	    for (c=line; (*c!=' ' && *c!='\t') && *c!='\0'; c++);
	    for (      ; (*c==' ' || *c=='\t') && *c!='\0'; c++);
	    if (strncmp(c,"directory",9)) {
		path[i]='\0';
		done=1;
	    }
	}
    }
}

void choosetexture(char *name)
{
    int done;
    int         c;
   

    done=0;
    while (!done) {
	getfilename(name, 
		"(Press / to cancel, ? for list)\nchoose rgb file to convert-->",
		"./terrainlsrgb");
	c = strlen(name);
        if (c<5 || (strcmp(&name[c-4],".rgb") && strcmp(&name[c-5],".rgba"))) {
	    name[c] = '.';
	    name[c+1] = 'r';
	    name[c+2] = 'g';
	    name[c+3] = 'b';
	    name[c+4] = '\0';
	    done=1;
        } else done=1;
    }
}

void loadtexture(int fd, char *filename)
{
    char name[100],line[MAXLINE],pathname[100],*np;
    FILE 	*fp;
    int 	done,c;
    u32		num;
    u64 	buf[12];
    int 	p[12];
    int		i;

    if (++numtex >= MAXTEX) {
	printf("Too many textures (%d). load cancelled\n",numtex);
	badsend(fd);
	return;
    }

    if (filename) {
        strcpy(name,filename);
    } else {
        done = 0;
        while(!done) {
	    getfilename(name, 
		"(Press / to cancel, ? for list, + for new texture)\nName of texture .h file:",
		"./terrainlsMM");
	    if (strlen(path)>0 && path[strlen(path)-1]!='/') {
		path[strlen(path)+1]='\0';
		path[strlen(path)]='/';
	    }
	    c = strlen(name);
	    if (name[0] == '+' && name[1]=='\0') {
		choosetexture(name);
		if (name[0]=='\0') 
		    done=2;
		else
		    done=1;
            } else if (name[0] == '/' && name[1] == '\0') {
		done=2;
            } else if (name[c-1] != 'h' || name[c-2] != '.') {
	        name[c] = '.';
	        name[c+1] = 'h';
	        name[c+2] = '\0';
	        done=1;
            } else done=1;
        }
    
        if (done==2) {
	    printf("load cancelled\n");
	    badsend(fd);
	    return;
        }
    }

    strcpy(pathname,path);
    strcat(pathname,name);
    if (!(fp=fopen(pathname,"r"))) {
	printf("error opening texture file %s\n",pathname);
	badsend(fd);
	return;
    }

    fprintf(stderr,"Loading texture: %s\n",pathname);
    strcpy(hosttexlist[numtex-1].file_name,pathname);

    buf[0] = 0x0;
    if (uhWriteGame(fd, buf, 8) == -1) {
        fprintf(stderr, "uhWriteGame %s\n", sys_errlist[errno]);
        return;
    }

    for (np=&name[strlen(name)]; np!=name && *np!='/'; np--);
    strncpy((char *)buf, np, 8*12);
    if (uhWriteGame(fd, buf, 8*12) == -1) {
	fprintf(stderr, "uhWriteGame %s\n", sys_errlist[errno]);
	return;
    }

    done = 0;
    while (!done) {
        done = getline(fp,line);
	if (!strncmp(line,"unsigned short",strlen("unsigned short")) &&
		checkword(line,"_buf[")) {
	    done = 2;
	}
    }
    if (done==1) {
	printf("File \"%s\" is not an rgb2c-created mipmapped texture file\n",
		pathname);
	badsend(fd);
	fclose(fp);
	return;
    }
    if (done==2) {
        c = getc(fp);							/*{*/
        while (c != '}') {						/*{*/
	    while (c!= '}' && c!= '0' && c!=EOF) c=getc(fp);
	    if (c=='0') c=getc(fp);
	    if (c=='x') {
		fscanf(fp,"%x",&num);
		buf[0] = num;
    		if (uhWriteGame(fd, buf, 8) == -1) {
        	    fprintf(stderr, "uhWriteGame %s\n", sys_errlist[errno]);
        	    return;
    		}
	    }
	}
    }
    num= 0xffffffff;
    buf[0] = num;
    if (uhWriteGame(fd, buf, 8) == -1) {
	fprintf(stderr, "uhWriteGame %s\n", sys_errlist[errno]);
	return;
    }

    done = 0;
    while (!done) {
        done = getline(fp,line);
	if (!strncmp(line,"Gfx",strlen("Gfx")) &&
		checkword(line,"_dl[")) {
	    done = 2;
	}
    }
    if (done==1) {
	badsend(fd);
	fclose(fp);
	return;
    }
   
    for (i=0; line[i]!='[';i++);
    line[i] = '\0';
    strcpy(hosttexlist[numtex-1].dl_name,&line[4]);
    hosttexlist[numtex-1].levels = -1;

    if (done==2) {
	done=0;
	while (!done) {
	    done = getline(fp,line);
	    if (checkword(line,"gsDPSetTextureImage(")) {
		sscanf(line," gsDPSetTextureImage( %i , %i , %i",
			&p[0],&p[1],&p[2]);
		/*printf(" gsDPSetTextureImage : %d %d %d\n",p[0],p[1],p[2]);*/
                buf[0] = 1;
                if (uhWriteGame(fd, buf, 8) == -1) {
	            fprintf(stderr, "uhWriteGame %s\n", sys_errlist[errno]);
	            return;
                }
                buf[0] = p[0];
                buf[1] = p[1];
                buf[2] = p[2];
                if (uhWriteGame(fd, buf, 8*3) == -1) {
	            fprintf(stderr, "uhWriteGame %s\n", sys_errlist[errno]);
	            return;
                }
	    } else if (checkword(line,"gsDPSetTile(")) {
	      if (checkword(line,"G_TX_LOADTILE")) {
		sscanf(line," gsDPSetTile( %i , %i , %i , %i , "
			"G_TX_LOADTILE, %i , %i , %i , %i , %i , %i , %i",
			&p[0],&p[1],&p[2],&p[3],&p[5],
			&p[6],&p[7],&p[8],&p[9],&p[10],&p[11]);
		p[4] = 7; /* LOADTILE */
	      } else {
		sscanf(line," gsDPSetTile( %i , %i , %i , %i , "
			"%i , %i , %i , %i , %i , %i , %i , %i",
			&p[0],&p[1],&p[2],&p[3],&p[4],&p[5],
			&p[6],&p[7],&p[8],&p[9],&p[10],&p[11]);
	      }
	      /*printf(" gsDPSetTile : %d %d %d %d %d %d %d %d %d %d %d %d\n",
			p[0],p[1],p[2],p[3],p[4],p[5],p[6],
			p[7],p[8],p[9],p[10],p[11]);*/
              buf[0] = 2;
              if (uhWriteGame(fd, buf, 8) == -1) {
	            fprintf(stderr, "uhWriteGame %s\n", sys_errlist[errno]);
	            return;
              }
              buf[0] = p[0];
              buf[1] = p[1];
              buf[2] = p[2];
              buf[3] = p[3];
              buf[4] = p[4];
              buf[5] = p[5];
              buf[6] = p[6];
              buf[7] = p[7];
              buf[8] = p[8];
              buf[9] = p[9];
              buf[10] = p[10];
              buf[11] = p[11];
              if (uhWriteGame(fd, buf, 8*12) == -1) {
	            fprintf(stderr, "uhWriteGame %s\n", sys_errlist[errno]);
	            return;
              }
	    } else if (checkword(line,"gsDPLoadBlock(")) {
		sscanf(line," gsDPLoadBlock( G_TX_LOADTILE, %i , %i , %i , %i",
			&p[1],&p[2],&p[3],&p[4]);
		p[0] = 7; /* LOADTILE */
		/*printf(" gsDPLoadBlock : %d %d %d %d %d\n",
			p[0],p[1],p[2],p[3],p[4]);*/
                buf[0] = 3;
                if (uhWriteGame(fd, buf, 8) == -1) {
	            fprintf(stderr, "uhWriteGame %s\n", sys_errlist[errno]);
	            return;
                }
                buf[0] = p[0];
                buf[1] = p[1];
                buf[2] = p[2];
                buf[3] = p[3];
                buf[4] = p[4];
                if (uhWriteGame(fd, buf, 8*5) == -1) {
	            fprintf(stderr, "uhWriteGame %s\n", sys_errlist[errno]);
	            return;
                }
	    } else if (checkword(line,"gsDPSetTileSize(")) {
		sscanf(line," gsDPSetTileSize( %i , %i , %i , "
			"%i << G_TEXTURE_IMAGE_FRAC, %i",
			&p[0],&p[1],&p[2],&p[3],&p[4]);
		/*printf(" gsDPSetTileSize : %d %d %d %d %d\n",
			p[0],p[1],p[2],p[3],p[4]);*/
                buf[0] = 4;
                if (uhWriteGame(fd, buf, 8) == -1) {
	            fprintf(stderr, "uhWriteGame %s\n", sys_errlist[errno]);
	            return;
                }
                buf[0] = p[0];
                buf[1] = p[1];
                buf[2] = p[2];
                buf[3] = p[3]<< 2; /* <<2 = <<G_TEXTURE_IMAGE_FRAC */
                buf[4] = p[4]<< 2; /* <<2 = <<G_TEXTURE_IMAGE_FRAC */
                if (uhWriteGame(fd, buf, 8*5) == -1) {
	            fprintf(stderr, "uhWriteGame %s\n", sys_errlist[errno]);
	            return;
                }
    		hosttexlist[numtex-1].levels++;
	    } else if (checkword(line,"gsSPEndDisplayList")) {
		done=2;
	    }
	}
    }
    num= 0xffffffff;
    buf[0] = num;
    if (uhWriteGame(fd, buf, 8) == -1) {
	fprintf(stderr, "uhWriteGame %s\n", sys_errlist[errno]);
	return;
    }

    num= hosttexlist[numtex-1].levels;
    buf[0] = num;
    if (uhWriteGame(fd, buf, 8) == -1) {
	fprintf(stderr, "uhWriteGame %s\n", sys_errlist[errno]);
	return;
    }

    fclose(fp);
    fprintf(stderr,"Texture loaded successfully.\n");

}

int main(int argc, char *argv[])
{
    pid_t       pid;
    int         fd,done=0;

    if ((fd = uhOpenGame("/dev/u64")) == -1) {
        fprintf(stderr, "host: openGame(/dev/u64): %s\n", sys_errlist[errno]);
        return(1);
    }
    if ((pid = fork()) == -1) {
        fprintf(stderr, "host: fork: %s\n", sys_errlist[errno]);
        return(-1);
    } else if (pid == 0) {
        (void)execl("/usr/sbin/gload", "/usr/sbin/gload", 0);
        fprintf(stderr, "host: execl(\"gload\") failed\n");
        return(-1);
    }


    sprintf(terrain_dataname,"ter%d_data.h",terrain_num);
    sprintf(terrain_headername,"ter%d_header.h",terrain_num);
    sprintf(terrain_savename,"ter%d_save.h",terrain_num);

    while (!done) {
	if (uhReadGame(fd, cmd, sizeof(cmd)) == -1) {
            fprintf(stderr, "uhReadGame %s\n", sys_errlist[errno]);
            return(1);
        }
	/*printf("HOST: cmd=%s\n",cmd);*/
	switch (cmd[0]) {
	    case 's':
		saveterrain(fd,0);
		break;
	    case 'a':
		saveterrain(fd,ALLCULL_FLAG);
		break;
	    case 'p':
		saveterrain(fd,PARTCULL_FLAG | ALLCULL_FLAG);
		break;
	    case 'l':
		loadterrain(fd);
		break;
	    case 't':
		loadtexture(fd,(char *)0);
		break;
	}
    }

    return 0;
}
