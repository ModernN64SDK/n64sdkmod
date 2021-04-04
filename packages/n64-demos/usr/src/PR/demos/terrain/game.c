/**************************************************************************;
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

/*---------------------------------------------------------------------*
        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
        
        $RCSfile: game.c,v $
        $Revision: 1.11 $
        $Date: 1999/04/16 09:41:32 $
 *---------------------------------------------------------------------*/

/*
 * File:  game.c
 *
 *
 */

#include <ultra64.h>
#include <PR/ramrom.h>		/* needed for argument passing into the app */
#include <assert.h>

#include "game.h"
#include "static.h"
#include "controller.h"
#include "font_ext.h"
#include "shared.h"


/*
 * Task header.
 */
OSTask          taskHeader =
{
	M_GFXTASK,		/* type of task */
	OS_TASK_DP_WAIT,	/* flags - wait for DP to be available */
	NULL,			/* ucode boot (fill in later) */
	0,			/* ucode boot size (fill in later) */
	NULL,			/* ucode (fill in later) */
	SP_UCODE_SIZE,		/* ucode size */
	NULL,			/* ucode data (fill in later) (to init DMEM) */
	SP_UCODE_DATA_SIZE,	/* ucode data size */
	&dram_stack[0],		/* stack used by ucode */
	SP_DRAM_STACK_SIZE8,	/* size of stack */
	&rdp_output[0],		/* fifo output buffer start */
	&rdp_output[0]+RDP_OUTPUT_LEN,/* fifo output buffer end */
	NULL,			/* display list pointer (fill in later) */
	0,			/* display list size (ignored) */
	NULL,			/* yield buffer (used if yield will occur) */
	0			/* yield buffer length */
};

/*
 * global variables
 */
Gfx		*glistp;	/* RSP display list pointer */
Dynamic		dynamic;	/* dynamic data */
int		draw_buffer=0;	/* frame buffer being updated (0 or 1) */
int		fontcol[4];	/* color for shadowed fonts */
Lights0		nolight = 	/* use for light = black */
		gdSPDefLights0(0,0,0);
Vp vpl = {
        SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,  /* scale */
        SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,  /* translate */
};

/*
 * local functions 
 */
char **myalloc(u32 bytesize);
char **myrealloc(char **handle, u32 bytesize);
void makeseldl(int x, int z);
void makenorms(void);
void makeedgetouch(void);
void makegrounddl(void);
void saveterrain(void);
void makeground(void);
void saveterrain(void);
s16  deltafunc(int da, float r,int remove);
void applydelta(int da, float dr, int x, int z, int remove);
void loadterrain(void);
void render_texoff(Gfx **gp);
void render_texon(Gfx **gp,int thetex);
void newtex(Gfx **gp, int *lasttex, int *loadtex, int thetex);
void loadtexture(void);

/*
 * macros 
 */
#define DL_STARTSIZE 500
#define DL_INCSIZE 100

#ifdef DEBUG
#define PRINTF osSyncPrintf
#endif

#define MAXLIGHTS 7
#define PADTHRESH(num,thresh)	((num > thresh) ? num-thresh :		\
                		(num < -thresh) ? num+thresh : 0)
#define ABS(x)			((x>0) ? x : -x)
#define PI			3.14159
#define FONTCOL(r,g,b,a)        {fontcol[0]=r;  \
                                fontcol[1]=g;   \
                                fontcol[2]=b;   \
                                fontcol[3]=a;}
#define FONTCOLM(c)             FONTCOL(c)
#define SHOWFONT(glp,str,x,y)   {                                              \
                font_set_color( fontcol[0],fontcol[1],fontcol[2],fontcol[3]);  \
                font_set_pos( x, y );                                          \
                font_show_string( glp, str );}				       \
                font_set_color( 0,0,0,255);                                    \
                font_set_pos( (x)+(1), (y)+(0) );                              \
                font_show_string( glp, str );                                  \
                font_set_pos( (x)+(0), (y)+(1) );                              \
                font_show_string( glp, str );
/*
 * macros for the default terrain 
 */
#define IFREQ	(PI*4/(dimx))
#define JFREQ	(PI*8/(dimz))
#define AMPMAX	(10)


/*
 * Menus
 */
enum {
	MT_END, 
	MT_SEPERATOR, 
	MT_MENU, 
	MT_ADDVALI, 
	MT_FADVALI, 
	MT_ADDVALF, 
	MT_ADDVALS,
	MT_MULVALI,
	MT_MULVALF,
	MT_UPMENU
};
typedef struct MenuItem {
	char		*text;
	int		type;
	struct MenuItem	*nextmenu;
	float		val;
	float		inc,min,max;
	u8		color[4];
} MenuItem;

char *noStr[] = {
	"","",
};
char *onStr[] = {
	"off","on",
};
char *svStr[] = {
	"nocull","allcull","section cull",
};
char svChar[] = "sap";
char *rtStr[] = {
	"automatic","joystick",
};
char *mmStr[] = {
	"Material Color","Bilinear","Trilinear Mipmap",
};
char *cmStr[] = {
	"Make Mountains","Place Texture",
};
char *geoStr[] = {
	"Teapot",
	"Cube",
};
char *txStr[20] = {
	"None",
	"Brick", 
	"Checkers",
};
char *ltStr[] = {
	"off",
	"1 Light",
	"2 Lights",
	"3 Lights",
	"4 Lights",
	"5 Lights",
	"6 Lights",
	"7 Lights",
};
char *lmStr[] = {
	"Light 1 Menu",
	"Light 2 Menu",
	"Light 3 Menu",
	"Light 4 Menu",
	"Light 5 Menu",
	"Light 6 Menu",
	"Light 7 Menu",
};
enum {
	FN_SQUARE,
	FN_SQUAREM1,
	FN_LINEAR,
	FN_RAND,
	FN_TABLE,
	FN_END
};
char *funStr[] = {
	"square",
	"1-square",
	"linear",
	"random",
	"plateau",
};
char *grStr[] = {
	"off",
	"below",
	"above",
	"right",
	"left",
	"above and right",
	"above and left",
	"below and right",
	"below and left",
	"above or right",
	"above or left",
	"below or right",
	"below or left",
	"all"
};
char *opStr[] = {
	"move",
	"scale"
};

MenuItem texturemenu[] = {
	{"TEXTURE MENU",       MT_SEPERATOR, 0,0,0,0,0,
	0,   25,   0, 0xff},
	{"Main menu",	       MT_UPMENU,    0,0,0,0,0,
	5, 255, 155, 0xff},
	{"Texture         : ", MT_ADDVALS,(MenuItem *) txStr, 0,1,0,2,       
	55, 255, 155, 0xff},
	{"Texture Scale   : ", MT_MULVALF,   0, 0.5, 1.2,   1.0/65536.0, 1.0,
	55, 255, 155, 0xff},
	{"Group Texture   : ", MT_ADDVALS,(MenuItem *) grStr, 13,1,0,13,       
	55, 255, 155, 0xff},
	{"Operation       : ", MT_ADDVALS,(MenuItem *) opStr, 0,1,0,1,       
	55, 255, 155, 0xff},
	{"New Texture",        MT_ADDVALS,(MenuItem *) noStr, 0,1,-1,1,       
	55, 255, 155, 0xff},
	{"",                   MT_END,       0,0,0,0,0,
	55, 255, 155, 0xff},
};
enum {
	TM_TITLE,
	TM_MAINMENU,
	TM_TEXTURE,
	TM_TXSCALE,
	TM_GROUP,
	TM_OP,
	TM_NEWTEX,
	TM_END
};

MenuItem fogmenu[] = {
	{"FOG MENU",           MT_SEPERATOR, 0,0,0,0,0,
	0,   25,   0, 0xff},
	{"Main menu",	       MT_UPMENU,    0,0,0,0,0,
	5, 255, 155, 0xff},
	{"Enable Fog : ",      MT_ADDVALS,(MenuItem *) onStr, 0,1,0,1,       
	55, 255, 155, 0xff},
	{"min   : ",      MT_FADVALI,   0, 950,   1,     0,  1000,
	5, 255, 155, 0xff},
	{"max   : ",      MT_FADVALI,   0,1000,   1,     0,  1000,
	5, 255, 155, 0xff},
	{"Red   : ",      MT_FADVALI,   0, 64,   1,     0,  255,
	5, 255, 155, 0xff},
	{"Green : ",      MT_FADVALI,   0, 64,   1,     0,  255,
	5, 255, 155, 0xff},
	{"Blue  : ",      MT_FADVALI,   0, 255,   1,     0,  255,
	5, 255, 155, 0xff},
	{"",                   MT_END,       0,0,0,0,0,
	55, 255, 155, 0xff},
};
enum {
	FM_TITLE,
	FM_MAINMENU,
	FM_ENABLE,
	FM_MIN,
	FM_MAX,
	FM_RED,
	FM_GREEN,
	FM_BLUE,
	FM_END
};

enum {
	DM_TITLE,
	DM_MAINMENU,
	DM_SAVE,
	DM_LOAD,
	DM_SAVEMODE,
	DM_SURFDP,
	DM_LINEDP,
	DM_SURFSPP,
	DM_END
};

MenuItem modemenu[] = {
	{"MODE MENU",          MT_SEPERATOR, 0,0,0,0,0,
	0,   25,   0, 0xff},
	{"Main menu",	       MT_UPMENU,    0,0,0,0,0,
	5, 255, 155, 0xff},
	{"Enable Contours : ", MT_ADDVALS,(MenuItem *) onStr, 0,1,0,1,       
	55, 255, 155, 0xff},
	{"Terrain Size x  : ", MT_FADVALI,   0, SIDE, 1,     2,  SIDEMAX,
	5, 255, 155, 0xff},
	{"Terrain Size z  : ", MT_FADVALI,   0, SIDE, 1,     2,  SIDEMAX,
	5, 255, 155, 0xff},
	{"Decal Parm      : ", MT_FADVALI,   0, 33,   1,     0,  255,
	5, 255, 155, 0xff},
	{"Enable Nearclip : ", MT_ADDVALS,(MenuItem *) onStr, 1,1,0,1,       
	55, 255, 155, 0xff},
	{"Center Cursor   : ", MT_ADDVALS,(MenuItem *) onStr, 1,1,0,1,       
	55, 255, 155, 0xff},
	{"",                   MT_END,       0,0,0,0,0,
	55, 255, 155, 0xff},
};
enum {
	OM_TITLE,
	OM_MAINMENU,
	OM_CONTOUR,
	OM_DIMX,
	OM_DIMZ,
	OM_DECAL,
	OM_NEARCLIP,
	OM_LOOKATC,
	OM_END
};

MenuItem materialmenu[] = {
	{"MATERIAL MENU",      MT_SEPERATOR, 0,0,0,0,0,
	0,   25,   0, 0xff},
	{"Main menu",	       MT_UPMENU,    0,0,0,0,0,
	5, 255, 155, 0xff},
	{"Red      : ",        MT_FADVALI,   0, 255,   1,     0,  255,
	5, 255, 155, 0xff},
	{"Green    : ",        MT_FADVALI,   0, 255,   1,     0,  255,
	5, 255, 155, 0xff},
	{"Blue     : ",        MT_FADVALI,   0, 255,   1,     0,  255,
	5, 255, 155, 0xff},
	{"Diffuse Multiplier  :",MT_FADVALI,   0, 100,   1,     0,  255,
	5, 255, 155, 0xff},
	{"",                   MT_END,       0,0,0,0,0,
	55, 255, 155, 0xff},
};
enum {
	AM_TITLE,
	AM_MAINMENU,
	AM_RED,
	AM_GREEN,
	AM_BLUE,
	AM_DIFFUSE,
	AM_SPEC,
	AM_END
};

enum {			/* for light menu (see template below) */
	LM_TITLE,
	LM_MAINMENU,
	LM_AMBON,
	LM_DIFFON,
	LM_X,
	LM_Y,
	LM_Z,
	LM_RED,
	LM_GREEN,
	LM_BLUE,
	LM_AMB,
	LM_L1,
	LM_L2,
	LM_L3,
	LM_L4,
	LM_L5,
	LM_L6,
	LM_L7,
	LM_END
};
enum {			/* for ambient menu (see template below) */
	BM_TITLE,
	BM_MAINMENU,
	BM_AMBON,
	BM_DIFFON,
	BM_RED,
	BM_GREEN,
	BM_BLUE,
	BM_L1,
	BM_L2,
	BM_L3,
	BM_L4,
	BM_L5,
	BM_L6,
	BM_L7,
	BM_END
};
MenuItem lightmenu[MAXLIGHTS+1][LM_END+1];
MenuItem ambientmenu[1][BM_END+1];

MenuItem mainmenu[] = {
	{"MAIN MENU",          MT_SEPERATOR,      0, 0,0,0,0,
	0,   25,   0, 0xff},
	{"Cursor Mode     : ", MT_ADDVALS,(MenuItem *) cmStr, 0,1,0,1,       
	55, 255, 155, 0xff},
	{"Enable Ambient  : ", MT_ADDVALS,(MenuItem *) ltStr, 1,1,0,1,       
	55, 255, 155, 0xff},
	{"Enable Diffuse  : ", MT_ADDVALS,(MenuItem *) ltStr, 2,1,0,7,       
	55, 255, 155, 0xff},
	{"Texture Menu",       MT_MENU, texturemenu, 0,0,0,0,
	55, 255, 155, 0xff},
	{"Material Menu",      MT_MENU,materialmenu, 0,0,0,0,
	55, 255, 155, 0xff},
	{"Mode Menu",          MT_MENU,    modemenu, 0,0,0,0,
	55, 255, 155, 0xff},
	{"Light Menus",        MT_MENU, ambientmenu[0], 0,0,0,0,
	55, 255, 155, 0xff},
	{"Fog Menu",           MT_MENU,     fogmenu, 0,0,0,0,
	55, 255, 155, 0xff},
	{"Bump Function   : ",MT_ADDVALS,(MenuItem *) funStr, 0,1,0,FN_END-1,
	55, 255, 155, 0xff},
	{"",                   MT_END,            0, 0,0,0,0,
	55, 255, 155, 0xff},
};
enum {
	MM_TITLE,
	MM_CURSORMODE,
	MM_AMBON,
	MM_DIFFON,
	MM_TXMENU,
	MM_MATMENU,
	MM_MODEMENU,
	MM_LIGHTMENU,
	MM_FOGMENU,
	MM_BFUNC,
	MM_END
};

MenuItem lighttemplate[] = {
	{"L",      	       MT_SEPERATOR, 0,0,0,0,0,
	0,   25,   0, 0xff},
	{"Main Menu",          MT_MENU,mainmenu, 0,0,0,0,
	55, 255, 155, 0xff},
	{"Enable Ambient   : ",MT_ADDVALS,(MenuItem *) onStr, 0,1,0,1,       
	55, 255, 155, 0xff},
	{"Number Of Lights : ",MT_ADDVALS,(MenuItem *) ltStr, 0,1,0,7,       
	55, 255, 155, 0xff},
	{"X     :",            MT_FADVALI,   0,  64,   1,  -127,  127,
	5, 255, 155, 0xff},
	{"Y     :",            MT_FADVALI,   0,   0,   1,  -127,  127,
	5, 255, 155, 0xff},
	{"Z     :",            MT_FADVALI,   0,  64,   1,  -127,  127,
	5, 255, 155, 0xff},
	{"Red   :",            MT_FADVALI,   0, 255,   1,     0,  255,
	5, 255, 155, 0xff},
	{"Green :",            MT_FADVALI,   0, 255,   1,     0,  255,
	5, 255, 155, 0xff},
	{"Blue  :",            MT_FADVALI,   0, 255,   1,     0,  255,
	5, 255, 155, 0xff},
	{"Ambient Menu",       MT_MENU,ambientmenu[0], 0,0,0,0,
	55, 255, 155, 0xff},
	{"Light 1 Menu",       MT_MENU,lightmenu[0], 0,0,0,0,
	55, 255, 155, 0xff},
	{"Light 2 Menu",       MT_MENU,lightmenu[1], 0,0,0,0,
	55, 255, 155, 0xff},
	{"Light 3 Menu",       MT_MENU,lightmenu[2], 0,0,0,0,
	55, 255, 155, 0xff},
	{"Light 4 Menu",       MT_MENU,lightmenu[3], 0,0,0,0,
	55, 255, 155, 0xff},
	{"Light 5 Menu",       MT_MENU,lightmenu[4], 0,0,0,0,
	55, 255, 155, 0xff},
	{"Light 6 Menu",       MT_MENU,lightmenu[5], 0,0,0,0,
	55, 255, 155, 0xff},
	{"Light 7 Menu",       MT_MENU,lightmenu[6], 0,0,0,0,
	55, 255, 155, 0xff},
	{"",                   MT_END,       0,0,0,0,0,
	55, 255, 155, 0xff},
};

MenuItem ambienttemplate[] = {
	{"AMBIENT LIGHT MENU", MT_SEPERATOR, 0,0,0,0,0,
	0,   25,   0, 0xff},
	{"Main menu",	       MT_MENU,mainmenu,0,0,0,0,
	5, 255, 155, 0xff},
	{"Enable Ambient : ", MT_ADDVALS,(MenuItem *) onStr, 0,1,0,1,       
	55, 255, 155, 0xff},
	{"Diffuse Lights : ", MT_ADDVALS,(MenuItem *) ltStr, 0,1,0,7,       
	55, 255, 155, 0xff},
	{"Red   :",            MT_FADVALI,   0, 255,   1,     0,  255,
	5, 255, 155, 0xff},
	{"Green :",            MT_FADVALI,   0, 255,   1,     0,  255,
	5, 255, 155, 0xff},
	{"Blue  :",            MT_FADVALI,   0, 255,   1,     0,  255,
	5, 255, 155, 0xff},
	{"Light 1 Menu",       MT_MENU,lightmenu[0], 0,0,0,0,
	55, 255, 155, 0xff},
	{"Light 2 Menu",       MT_MENU,lightmenu[1], 0,0,0,0,
	55, 255, 155, 0xff},
	{"Light 3 Menu",       MT_MENU,lightmenu[2], 0,0,0,0,
	55, 255, 155, 0xff},
	{"Light 4 Menu",       MT_MENU,lightmenu[3], 0,0,0,0,
	55, 255, 155, 0xff},
	{"Light 5 Menu",       MT_MENU,lightmenu[4], 0,0,0,0,
	55, 255, 155, 0xff},
	{"Light 6 Menu",       MT_MENU,lightmenu[5], 0,0,0,0,
	55, 255, 155, 0xff},
	{"Light 7 Menu",       MT_MENU,lightmenu[6], 0,0,0,0,
	55, 255, 155, 0xff},
	{"",                   MT_END,       0,0,0,0,0,
	55, 255, 155, 0xff},
};

/*
 * create the ground database
 */

typedef struct TexEntry {
    char name[21];
    int	 dl;
    int  data;
    int  end;
    int  image;
    int  i1,i2,i3;
    int  levels;
} TexEntry;

typedef struct TexField {
    s16	s,t;
} TexField;

TexEntry **texlist;
char **texmem;
VertS **verts;
Gfx **grounddl;
Gfx **groundlinedl;
Vtx **groundvtx;
Gfx **groundseldl;
TexField **texfields;


int dimx=SIDE; 
int dimz=SIDE;
int numtex=0;


/* _zbufferSegmentEnd symbol generated by makerom */
extern char	_zbufferSegmentBssEnd[];
extern char	_cfbSegmentBssStart[];

void makeseldl(int x, int z)
{
    Gfx *d;
    d=*groundseldl;

    gSPVertex(d++, *groundvtx + x*dimz + z - 1, 3, 0);
    gSPVertex(d++, *groundvtx + (x-1)*dimz + z, 1, 3);
    gSPVertex(d++, *groundvtx + (x+1)*dimz + z, 2, 4);

    if (z>0)
      gSPLine3D(d++, 0, 1, 0);
    if (z<dimz-1)
      gSPLine3D(d++, 1, 2, 0);
    if (x>0)
      gSPLine3D(d++, 3, 1, 0);
    if (x<dimz-1)
      gSPLine3D(d++, 4, 1, 0);
    
    gSPEndDisplayList(d++);
}

void makenorms(void)
{
    int 	i,j;
    int		ip,im,jp,jm;
    float 	x,y,z,x1,y1,z1,x2,y2,z2;


    for (j=0; j<dimz; j++) {
    for (i=0; i<dimx; i++) {
	x1=2.0*SIDE2X;
	z1=0;
	x2=0;
	z2=2.0*SIDE2Z;

	ip = i+1;
	if (ip>=dimx-1) ip=0;
	jp = j+1;
	if (jp>=dimz-1) jp=0;
	im = i-1;
	if (im<0) im=dimx-2;
	jm = j-1;
	if (jm<0) jm=dimz-2;
	y1 = (*verts + (dimx*(j))+(ip))->altitude -
	     (*verts + (dimx*(j))+(im))->altitude;
	y2 = (*verts + (dimx*(jp))+(i))->altitude - 
	     (*verts + (dimx*(jm))+(i))->altitude;

	x = y1*z2 - z1*y2;
	y = z1*x2 - x1*z2;
	z = x1*y2 - y1*x2;

	x1 = 127.0/sqrtf(x*x + y*y + z*z);
	(*verts + (dimx*j) + (i))->normal[0] = x*x1;
	(*verts + (dimx*j) + (i))->normal[1] = y*x1;
	(*verts + (dimx*j) + (i))->normal[2] = z*x1;

    }
    }
}

void makeedgetouch(void)
{
    int		i;

    for (i=0; i<dimx; i++) {
	(*verts + dimx*(dimz-1) + (i))->altitude = 
			(*verts + dimx*(0) + (i))->altitude;
	(*verts + dimx*(dimz-1) + (i))->offset[0] = 
			(*verts + dimx*(0) + (i))->offset[0];
	(*verts + dimx*(dimz-1) + (i))->offset[1] = 
			(*verts + dimx*(0) + (i))->offset[1];
	(*verts + dimx*(dimz-1) + (i))->s = 
			(int)((*verts + dimx*(dimz-2) + (i))->s/32)*32 +
			(*verts + dimx*(0) + (i))->s -
			(int)((*verts + dimx*(0) + (i))->s/32)*32;
	(*verts + dimx*(dimz-1) + (i))->t = 
			(int)((*verts + dimx*(dimz-2) + (i))->t/32)*32 +
			(*verts + dimx*(0) + (i))->t -
			(int)((*verts + dimx*(0) + (i))->t/32)*32;
/*
	(*verts + dimx*(dimz-1) + (i))->t = 
			(*verts + dimx*(0) + (i))->t;
*/
    }
    for (i=0; i<dimz; i++) {
	(*verts + dimx*(i) + (dimx-1))->altitude =
		(*verts + dimx*(i) + (0))->altitude;
	(*verts + dimx*(i) + (dimx-1))->offset[0] =
		(*verts + dimx*(i) + (0))->offset[0];
	(*verts + dimx*(i) + (dimx-1))->offset[1] =
		(*verts + dimx*(i) + (0))->offset[1];
	(*verts + dimx*(i) + (dimx-1))->s =
		(*verts + dimx*(i) + (0))->s;
	(*verts + dimx*(i) + (dimx-1))->t =
		(*verts + dimx*(i) + (0))->t;
	(*verts + dimx*(i) + (dimx-1))->s = 
			(int)((*verts + dimx*(i) + (dimx-2))->s/32)*32 +
			(*verts + dimx*(i) + (0))->s -
			(int)((*verts + dimx*(i) + (0))->s/32)*32;
	(*verts + dimx*(i) + (dimx-1))->t = 
			(int)((*verts + dimx*(i) + (dimx-2))->t/32)*32 +
			(*verts + dimx*(i) + (0))->t -
			(int)((*verts + dimx*(i) + (0))->t/32)*32;
    }
}

void newtex(Gfx **gp, int *lasttex, int *loadtex, int thetex)
{
    if (thetex > numtex) return;

    if (thetex) {
	if (!*lasttex)
            render_texon(gp,thetex);
    } else {
	if (*lasttex)
            render_texoff(gp);
    }
    if (thetex && (thetex != *loadtex)) {
	gSPDisplayList((*gp)++, &((*texmem)[(*texlist)[thetex-1].dl]));
	*loadtex = thetex;
    }
    *lasttex = thetex;
}

void makegrounddl(void)
{
    int 	i,j,k;
    Vtx 	*v;
    Gfx		*d,*l;
    int		lasttex=0,loadtex=0;
    Gfx  	*gdat;
    char 	*texdat;

    makeedgetouch();
    makenorms();

    /*
     * Create Vertices
     */
    v = *groundvtx;
    for (i=0; i<dimx; i++) {
      for (j=0; j<dimz; j++) {
	v->n.ob[0] = i*SIDE2X-WORLD_X/2 + (*verts + dimx*j + i)->offset[0];
	v->n.ob[2] = j*SIDE2Z-WORLD_Z/2 + (*verts + dimx*j + i)->offset[1];
	if (i==dimx-1)
	    v->n.ob[0] = 0*SIDE2X-WORLD_X/2 + WORLD_X;
	if (j==dimz-1)
	    v->n.ob[2] = 0*SIDE2Z-WORLD_Z/2 + WORLD_Z;
	v->n.ob[1] = (*verts + dimx*j + i)->altitude;
	v->n.flag = 0;

	v->n.tc[0] = (*verts + dimx*j + i)->s;
	v->n.tc[1] = (*verts + dimx*j + i)->t;

	/* vertex normals */
	v->n.n[0] = (*verts + dimx*j + i)->normal[0];
	v->n.n[1] = (*verts + dimx*j + i)->normal[1];
	v->n.n[2] = (*verts + dimx*j + i)->normal[2];

	v++;
      }
    }


    /*
     * Create Display List
     */
    grounddl = (Gfx **) myrealloc((char **)grounddl, 
				3+dimz+dimx*dimz*30*sizeof(Gfx));
    groundlinedl = (Gfx **) myrealloc((char **)groundlinedl, 
				2+dimz+dimx*dimz*30*sizeof(Gfx));

    d=*grounddl;
    l=*groundlinedl;

    render_texoff(&d);
    for (j=0; j<dimz-1; j+=7) {
      k = MIN(dimz-j,8);
      gSPVertex(d++, *groundvtx + j, k, 0);
      gSPVertex(l++, *groundvtx + j, k, 0);
      for (i=0; i<dimx-1; i+=2) {
	k = MIN(dimz-j,8);
	gSPVertex(d++, *groundvtx + ((i+1)*dimz) + j, k, 8);
	gSPVertex(l++, *groundvtx + ((i+1)*dimz) + j, k, 8);

	switch (k) {
	  case 8:
            if ((*verts + dimx*(j+6) + (i))->texture[0] != lasttex) {
		newtex(&d, &lasttex, &loadtex, 
			(*verts + dimx*(j+6) + (i))->texture[0]);
            }
	    if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+6) + (i))->flags) {
	        gSP1Triangle(d++, 14, 6, 7, 0);
	    } else {
	        gSP1Triangle(d++, 14, 6,15, 2);
	    }
            if ((*verts + dimx*(j+6) + (i))->texture[1] != lasttex) {
		newtex(&d, &lasttex, &loadtex, 
			(*verts + dimx*(j+6) + (i))->texture[1]);
            }
	    if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+6) + (i))->flags) {
	        gSP1Triangle(d++, 15,14, 7, 2);
	    } else {
	        gSP1Triangle(d++, 15, 6, 7, 0);
	    }
	  case 7:
            if ((*verts + dimx*(j+5) + (i))->texture[0] != lasttex) {
		newtex(&d, &lasttex, &loadtex, 
			(*verts + dimx*(j+5) + (i))->texture[0]);
            }
	    if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+5) + (i))->flags) {
	        gSP1Triangle(d++, 13, 5, 6, 0);
	    } else {
	        gSP1Triangle(d++, 13, 5,14, 2);
	    }
            if ((*verts + dimx*(j+5) + (i))->texture[1] != lasttex) {
		newtex(&d, &lasttex, &loadtex, 
			(*verts + dimx*(j+5) + (i))->texture[1]);
            }
	    if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+5) + (i))->flags) {
	        gSP1Triangle(d++, 14,13, 6, 2);
	    } else {
	        gSP1Triangle(d++, 14, 5, 6, 0);
	    }
	  case 6:
            if ((*verts + dimx*(j+4) + (i))->texture[0] != lasttex) {
		newtex(&d, &lasttex, &loadtex, 
			(*verts + dimx*(j+4) + (i))->texture[0]);
            }
	    if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+4) + (i))->flags) {
	        gSP1Triangle(d++, 12, 4, 5, 0);
	    } else {
	        gSP1Triangle(d++, 12, 4,13, 2);
	    }
            if ((*verts + dimx*(j+4) + (i))->texture[1] != lasttex) {
		newtex(&d, &lasttex, &loadtex, 
			(*verts + dimx*(j+4) + (i))->texture[1]);
            }
	    if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+4) + (i))->flags) {
	        gSP1Triangle(d++, 13,12, 5, 2);
	    } else {
	        gSP1Triangle(d++, 13, 4, 5, 0);
	    }
	  case 5:
            if ((*verts + dimx*(j+3) + (i))->texture[0] != lasttex) {
		newtex(&d, &lasttex, &loadtex, 
			(*verts + dimx*(j+3) + (i))->texture[0]);
            }
	    if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+3) + (i))->flags) {
	        gSP1Triangle(d++, 11, 3, 4, 0);
	    } else {
	        gSP1Triangle(d++, 11, 3,12, 2);
	    }
            if ((*verts + dimx*(j+3) + (i))->texture[1] != lasttex) {
		newtex(&d, &lasttex, &loadtex, 
			(*verts + dimx*(j+3) + (i))->texture[1]);
            }
	    if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+3) + (i))->flags) {
	        gSP1Triangle(d++, 12,11, 4, 2);
	    } else {
	        gSP1Triangle(d++, 12, 3, 4, 0);
	    }
	  case 4:
            if ((*verts + dimx*(j+2) + (i))->texture[0] != lasttex) {
		newtex(&d, &lasttex, &loadtex, 
			(*verts + dimx*(j+2) + (i))->texture[0]);
            }
	    if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+2) + (i))->flags) {
	        gSP1Triangle(d++, 10, 2, 3, 0);
	    } else {
	        gSP1Triangle(d++, 10, 2,11, 2);
	    }
            if ((*verts + dimx*(j+2) + (i))->texture[1] != lasttex) {
		newtex(&d, &lasttex, &loadtex, 
			(*verts + dimx*(j+2) + (i))->texture[1]);
            }
	    if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+2) + (i))->flags) {
	        gSP1Triangle(d++, 11,10, 3, 2);
	    } else {
	        gSP1Triangle(d++, 11, 2, 3, 0);
	    }
	  case 3:
            if ((*verts + dimx*(j+1) + (i))->texture[0] != lasttex) {
		newtex(&d, &lasttex, &loadtex, 
			(*verts + dimx*(j+1) + (i))->texture[0]);
            }
	    if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+1) + (i))->flags) {
	        gSP1Triangle(d++,  9, 1, 2, 0);
	    } else {
	        gSP1Triangle(d++,  9, 1,10, 2);
	    }
            if ((*verts + dimx*(j+1) + (i))->texture[1] != lasttex) {
		newtex(&d, &lasttex, &loadtex, 
			(*verts + dimx*(j+1) + (i))->texture[1]);
            }
	    if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+1) + (i))->flags) {
	        gSP1Triangle(d++, 10, 9, 2, 2);
	    } else {
	        gSP1Triangle(d++, 10, 1, 2, 0);
	    }
	  case 2:
            if ((*verts + dimx*(j) + (i))->texture[0] != lasttex) {
		newtex(&d, &lasttex, &loadtex, 
			(*verts + dimx*(j) + (i))->texture[0]);
            }
	    if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j) + (i))->flags) {
	        gSP1Triangle(d++,  8, 0, 1, 0);
	    } else {
	        gSP1Triangle(d++,  8, 0, 9, 2);
	    }
            if ((*verts + dimx*(j) + (i))->texture[1] != lasttex) {
		newtex(&d, &lasttex, &loadtex, 
			(*verts + dimx*(j) + (i))->texture[1]);
            }
	    if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j) + (i))->flags) {
	        gSP1Triangle(d++,  9, 8, 1, 2);
	    } else {
	        gSP1Triangle(d++,  9, 0, 1, 0);
	    }
	}

	switch (k) {
	  case 8:
	    gSPLine3D(l++, 6,14, 0);
	  case 7:
	    gSPLine3D(l++, 5,13, 0);
	  case 6:
	    gSPLine3D(l++, 4,12, 0);
	  case 5:
	    gSPLine3D(l++, 3,11, 0);
	  case 4:
	    gSPLine3D(l++, 2,10, 0);
	  case 3:
	    gSPLine3D(l++, 1, 9, 0);
	  case 2:
	    gSPLine3D(l++, 0, 8, 0);
	}

	switch (k) {
	  case 8:
	    gSPLine3D(l++, 6, 7, 0);
	  case 7:
	    gSPLine3D(l++, 5, 6, 0);
	  case 6:
	    gSPLine3D(l++, 4, 5, 0);
	  case 5:
	    gSPLine3D(l++, 3, 4, 0);
	  case 4:
	    gSPLine3D(l++, 2, 3, 0);
	  case 3:
	    gSPLine3D(l++, 1, 2, 0);
	  case 2:
	    gSPLine3D(l++, 0, 1, 0);
	}


	if (i+2<dimx) {
	  gSPVertex(d++, *groundvtx + ((i+2)*dimz) + j, k, 0);
	  gSPVertex(l++, *groundvtx + ((i+2)*dimz) + j, k, 0);
	  switch (k) {
	    case 8:
              if ((*verts + dimx*(j+6) + (i+1))->texture[0] != lasttex) {
		  newtex(&d, &lasttex, &loadtex, 
			  (*verts + dimx*(j+6) + (i+1))->texture[0]);
              }
	      if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+6) + (i+1))->flags) {
	          gSP1Triangle(d++,  6,14,15, 0);
	      } else {
	          gSP1Triangle(d++,  6,14, 7, 2);
	      }
              if ((*verts + dimx*(j+6) + (i+1))->texture[1] != lasttex) {
		  newtex(&d, &lasttex, &loadtex, 
			  (*verts + dimx*(j+6) + (i+1))->texture[1]);
              }
	      if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+6) + (i+1))->flags) {
	          gSP1Triangle(d++,  7, 6,15, 2);
	      } else {
	          gSP1Triangle(d++,  7,14,15, 0);
	      }
	    case 7:
              if ((*verts + dimx*(j+5) + (i+1))->texture[0] != lasttex) {
		  newtex(&d, &lasttex, &loadtex, 
			  (*verts + dimx*(j+5) + (i+1))->texture[0]);
              }
	      if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+5) + (i+1))->flags) {
	          gSP1Triangle(d++,  5,13,14, 0);
	      } else {
	          gSP1Triangle(d++,  5,13, 6, 2);
	      }
              if ((*verts + dimx*(j+5) + (i+1))->texture[1] != lasttex) {
		  newtex(&d, &lasttex, &loadtex, 
			  (*verts + dimx*(j+5) + (i+1))->texture[1]);
              }
	      if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+5) + (i+1))->flags) {
	          gSP1Triangle(d++,  6, 5,14, 2);
	      } else {
	          gSP1Triangle(d++,  6,13,14, 0);
	      }
	    case 6:
              if ((*verts + dimx*(j+4) + (i+1))->texture[0] != lasttex) {
		  newtex(&d, &lasttex, &loadtex, 
			  (*verts + dimx*(j+4) + (i+1))->texture[0]);
              }
	      if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+4) + (i+1))->flags) {
	          gSP1Triangle(d++,  4,12,13, 0);
	      } else {
	          gSP1Triangle(d++,  4,12, 5, 2);
	      }
              if ((*verts + dimx*(j+4) + (i+1))->texture[1] != lasttex) {
		  newtex(&d, &lasttex, &loadtex, 
			  (*verts + dimx*(j+4) + (i+1))->texture[1]);
              }
	      if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+4) + (i+1))->flags) {
	          gSP1Triangle(d++,  5, 4,13, 2);
	      } else {
	          gSP1Triangle(d++,  5,12,13, 0);
	      }
	    case 5:
              if ((*verts + dimx*(j+3) + (i+1))->texture[0] != lasttex) {
		  newtex(&d, &lasttex, &loadtex, 
			  (*verts + dimx*(j+3) + (i+1))->texture[0]);
              }
	      if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+3) + (i+1))->flags) {
	          gSP1Triangle(d++,  3,11,12, 0);
	      } else {
	          gSP1Triangle(d++,  3,11, 4, 2);
	      }
              if ((*verts + dimx*(j+3) + (i+1))->texture[1] != lasttex) {
		  newtex(&d, &lasttex, &loadtex, 
			  (*verts + dimx*(j+3) + (i+1))->texture[1]);
              }
	      if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+3) + (i+1))->flags) {
	          gSP1Triangle(d++,  4, 3,12, 2);
	      } else {
	          gSP1Triangle(d++,  4,11,12, 0);
	      }
	    case 4:
              if ((*verts + dimx*(j+2) + (i+1))->texture[0] != lasttex) {
		  newtex(&d, &lasttex, &loadtex, 
			  (*verts + dimx*(j+2) + (i+1))->texture[0]);
              }
	      if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+2) + (i+1))->flags) {
	          gSP1Triangle(d++,  2,10,11, 0);
	      } else {
	          gSP1Triangle(d++,  2,10, 3, 2);
	      }
              if ((*verts + dimx*(j+2) + (i+1))->texture[1] != lasttex) {
		  newtex(&d, &lasttex, &loadtex, 
			  (*verts + dimx*(j+2) + (i+1))->texture[1]);
              }
	      if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+2) + (i+1))->flags) {
	          gSP1Triangle(d++,  3, 2,11, 2);
	      } else {
	          gSP1Triangle(d++,  3,10,11, 0);
	      }
	    case 3:
              if ((*verts + dimx*(j+1) + (i+1))->texture[0] != lasttex) {
		  newtex(&d, &lasttex, &loadtex, 
			  (*verts + dimx*(j+1) + (i+1))->texture[0]);
              }
	      if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+1) + (i+1))->flags) {
	          gSP1Triangle(d++,  1, 9,10, 0);
	      } else {
	          gSP1Triangle(d++,  1, 9, 2, 2);
	      }
              if ((*verts + dimx*(j+1) + (i+1))->texture[1] != lasttex) {
		  newtex(&d, &lasttex, &loadtex, 
			  (*verts + dimx*(j+1) + (i+1))->texture[1]);
              }
	      if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j+1) + (i+1))->flags) {
	          gSP1Triangle(d++,  2, 1,10, 2);
	      } else {
	          gSP1Triangle(d++,  2, 9,10, 0);
	      }
	    case 2:
              if ((*verts + dimx*(j) + (i+1))->texture[0] != lasttex) {
		  newtex(&d, &lasttex, &loadtex, 
			  (*verts + dimx*(j) + (i+1))->texture[0]);
              }
	      if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j) + (i+1))->flags) {
	          gSP1Triangle(d++,  0, 8, 9, 0);
	      } else {
	          gSP1Triangle(d++,  0, 8, 1, 2);
	      }
              if ((*verts + dimx*(j) + (i+1))->texture[1] != lasttex) {
		  newtex(&d, &lasttex, &loadtex, 
			  (*verts + dimx*(j) + (i+1))->texture[1]);
              }
	      if (VERT_FLAG_SWAPDIAG & (*verts + dimx*(j) + (i+1))->flags) {
	          gSP1Triangle(d++,  1, 0, 9, 2);
	      } else {
	          gSP1Triangle(d++,  1, 8, 9, 0);
	      }
	  }

	  switch (k) {
	    case 8:
	      gSPLine3D(l++, 6,14, 0);
	    case 7:
	      gSPLine3D(l++, 5,13, 0);
	    case 6:
	      gSPLine3D(l++, 4,12, 0);
	    case 5:
	      gSPLine3D(l++, 3,11, 0);
	    case 4:
	      gSPLine3D(l++, 2,10, 0);
	    case 3:
	      gSPLine3D(l++, 1, 9, 0);
	    case 2:
	      gSPLine3D(l++, 0, 8, 0);
	  }

	  switch (k) {
	    case 8:
	      gSPLine3D(l++,14,15, 0);
	    case 7:
	      gSPLine3D(l++,13,14, 0);
	    case 6:
	      gSPLine3D(l++,12,13, 0);
	    case 5:
	      gSPLine3D(l++,11,12, 0);
	    case 4:
	      gSPLine3D(l++,10,11, 0);
	    case 3:
	      gSPLine3D(l++, 9,10, 0);
	    case 2:
	      gSPLine3D(l++, 8, 9, 0);
	  }
	}

      }
    }
    gSPEndDisplayList(d++);
    gSPEndDisplayList(l++);

    /*
     * make texture image commands point to texture data
     */
    texdat = *texmem;
    for (i=0; i<numtex; i++) {
        gdat = (Gfx *) &(texdat[(*texlist)[i].image]);
	gDPSetTextureImage(gdat++,
				(*texlist)[i].i1,
				(*texlist)[i].i2,
				(*texlist)[i].i3,
				(int)&(texdat[(*texlist)[i].data]));
    }
}


#define MYALLOC_ALIGN 0x10
#define MAXMEM 50
int myalloc_nummem=-1;
char *myalloc_maxmem;
char *myalloc_memtable[MAXMEM+1];
char **myalloc(u32 bytesize)
{
    bytesize = (bytesize + MYALLOC_ALIGN-1) & ~(MYALLOC_ALIGN-1);
    if (myalloc_nummem<0) {
	myalloc_memtable[0] = (char *) freeSegment;
	myalloc_maxmem      = (char *) _cfbSegmentBssStart;
	myalloc_nummem = 0;
    }
    if (myalloc_nummem>MAXMEM) {
#ifdef DEBUG
	PRINTF("ERROR: myalloc: not enough myalloc_memtable entries\n");
#endif
	return (char **) 0;
    }
    myalloc_nummem++;
    myalloc_memtable[myalloc_nummem] = 
		myalloc_memtable[myalloc_nummem-1] + bytesize;
    if (myalloc_memtable[myalloc_nummem] >= myalloc_maxmem) {
	myalloc_nummem--;
#ifdef DEBUG
	PRINTF("ERROR: myalloc: not enough memory\n");
#endif
	return (char **) 0;
    }
    return &myalloc_memtable[myalloc_nummem-1];
}
char **myrealloc(char **handle, u32 bytesize)
{
    int theentry,entry;
    int bytedelta;
    char *addr;
    u32 *s,*d;
    bytesize = (bytesize + MYALLOC_ALIGN-1) & ~(MYALLOC_ALIGN-1);
    theentry = (int) (handle - myalloc_memtable);
    if ((theentry < 0) || (theentry > myalloc_nummem-1)) {
#ifdef DEBUG
	PRINTF("ERROR: myrealloc: invalid handle\n");
#endif
	return (char **) 0;
    }
    if (theentry == myalloc_nummem-1) {
	myalloc_nummem--;
	return myalloc(bytesize);
    }
    bytedelta = bytesize - 
	(int) (myalloc_memtable[theentry+1] - myalloc_memtable[theentry]);
    if (bytedelta>0) {
	if (myalloc_memtable[myalloc_nummem] + bytedelta >= myalloc_maxmem) {
#ifdef DEBUG
	    PRINTF("ERROR: myrealloc: not enough memory\n");
#endif
	    return (char **) 0;
	}
	for (addr = myalloc_memtable[myalloc_nummem]; 
		addr >= myalloc_memtable[theentry+1]; 
		addr -= sizeof(u32)) {
	    s=(u32 *) addr;
	    d=(u32 *) (addr+bytedelta);
	    *d=*s;
	}
	for (entry = theentry+1; entry<=myalloc_nummem; entry++)
	    myalloc_memtable[entry] += bytedelta;
    }
    if (bytedelta<0) {
	for (addr = myalloc_memtable[theentry+1]; 
		addr < myalloc_memtable[myalloc_nummem]; 
		addr+=sizeof(u32)) {
	    s=(u32 *) addr;
	    d=(u32 *) (addr+bytedelta);
	    *d=*s;
	}
	for (entry = theentry+1; entry<=myalloc_nummem; entry++)
	    myalloc_memtable[entry] += bytedelta;
    }
    return &myalloc_memtable[theentry];
    
}


void makeground(void)
{
    int 	i,j;

    texlist = (TexEntry **) myalloc(0);
    texmem = (char **) myalloc(0);
    texfields = (TexField **) myalloc(0);
    verts = (VertS **) myalloc(dimx*dimz*sizeof(VertS));
    groundvtx = (Vtx **) myalloc(dimx*dimz*sizeof(Vtx));
    groundseldl = (Gfx **) myalloc(9*sizeof(Gfx));
    grounddl = (Gfx **) myalloc(sizeof(Gfx));
    groundlinedl = (Gfx **) myalloc(sizeof(Gfx));

    osSyncPrintf("startdata=  %08x\n",myalloc_memtable[0]);
    osSyncPrintf("--------------------------------\n");
    osSyncPrintf("VertS    =  %08x\n",*verts);
    osSyncPrintf("vert     =  %08x\n",*groundvtx);
    osSyncPrintf("grounddl =  %08x\n",*grounddl);
    osSyncPrintf("grlinedl =  %08x\n",*groundlinedl);
    osSyncPrintf("--------------------------------\n");
    osSyncPrintf("dataend  =  %08x\n",myalloc_maxmem);

    for (j=0; j<dimz; j++) {
    for (i=0; i<dimx; i++) {
	(*verts + (dimx*j)+i)->altitude = 
		sinf((float)i*IFREQ) * cosf((float)j*JFREQ) * AMPMAX;
	(*verts + (dimx*j)+i)->texture[0] = 0;
	(*verts + (dimx*j)+i)->texture[1] = 0;
	(*verts + (dimx*j)+i)->flags = 0;
	(*verts + (dimx*j)+i)->offset[0] = 0;
	(*verts + (dimx*j)+i)->offset[1] = 0;
	(*verts + (dimx*j)+i)->s = (i-(dimx/2.0))*32*32/5;
	(*verts + (dimx*j)+i)->t = (j-(dimz/2.0))*32*32/5;
    }
    }

    makegrounddl();

}

/*
 * Save terrain to a file on the host
 */
void saveterrain(void)
{
    u64 	buffer[(sizeof(VertS)+7)/8];
    VertS 	*v;
    int		i,j;
    u32		*n;

    makenorms();

    /*
     * announce dimensions
     */
    n=(u32 *)buffer;	
    n[0]=dimx;
    n[1]=dimz;

#ifdef DEBUG
    osWriteHost(buffer,sizeof(buffer));
#endif  

    /*
     * dump Vertices
     */
    v = (VertS *) buffer;
    for (i=0; i<dimx; i++) {
      for (j=0; j<dimz; j++) {

	*v = *(*verts + dimx*j + i);

#ifdef DEBUG
	osWriteHost(buffer,sizeof(buffer));
#endif

      }
    }
}

/*
 * Load terrain from a file on the host
 */
void loadterrain(void)
{
    u64 	buffer[(sizeof(VertS)+7)/8];
    VertS 	*v;
    int		i,j,addtex;
    u32		*n;
    /*int		dimx, dimz;*/


    /*
     * get dimensions
     */

#ifdef DEBUG
    osReadHost(buffer, sizeof(buffer));
#endif

    n=(u32 *)buffer;
    dimx = n[0];
    dimz = n[1];
    modemenu[OM_DIMX].val = dimx;
    modemenu[OM_DIMZ].val = dimz;

    v = (VertS *) buffer;

    for (i=0; i<dimx; i++) {
      for (j=0; j<dimz; j++) {

#ifdef DEBUG
        osReadHost(buffer, sizeof(buffer));
#endif

	*(*verts + dimx*j + i) = *v;
      }
    }

#ifdef DEBUG
    osReadHost(buffer, 8);
#endif

    addtex = buffer[0];
    numtex = 0;
    for (i=0; i<addtex;i++) {
	loadtexture();
    }
    texturemenu[TM_TEXTURE].val = numtex;
    texturemenu[TM_TEXTURE].max = numtex;
    makegrounddl();
}

void loadtexture(void)
{
    u64 buffer[12];
    int i,texptr,texdatasize;
    char *texdat;
    Gfx  *gdat;

#ifdef DEBUG
    osReadHost(buffer, 8*1);
#endif

    if (buffer[0] == 0xfffffffe) {
	return;
    }

#ifdef DEBUG
    osReadHost(buffer, 8*12);
#endif

    ((char *) buffer)[20] = '\0';
    
	
    numtex++;
    texlist = (TexEntry **) myrealloc((char **)texlist, numtex*sizeof(TexEntry));
    if (numtex<2)
	(*texlist)[numtex-1].data = 0;
    else
	(*texlist)[numtex-1].data = (*texlist)[numtex-2].end;

    for (i=0; i<21; i++) 
	(*texlist)[numtex-1].name[i] = ((char *)buffer)[i];
    texptr = (*texlist)[numtex-1].data;

    texmem = (char **) myrealloc((char **)texmem, texptr+256);
    texdatasize = texptr+64;
    texdat = *texmem;

#ifdef DEBUG
    osReadHost(buffer, 8);
#endif

    while (buffer[0] != 0xffffffff && buffer[0] != 0xfffffffe) {
	texdat[texptr++] = ((((u32)buffer[0])&0xff00) >>8);
	texdat[texptr++] = (((u32)buffer[0])&0x00ff );
	if (texptr >= texdatasize-2) {
    	    texmem = (char **) myrealloc((char **)texmem, texptr+256);
    	    texdatasize = texptr+64;
    	    texdat = *texmem;
	}

#ifdef DEBUG
        osReadHost(buffer, 8);
#endif

    }

    if (buffer[0] == 0xfffffffe) {
    	numtex--;
	return;
    }

    texmem = (char **) myrealloc((char **)texmem, texptr+256);
    texdatasize = texptr+64;
    texdat = *texmem;
    (*texlist)[numtex-1].dl = texptr;

#ifdef DEBUG
    osReadHost(buffer, 8);
#endif

    while (buffer[0] != 0xffffffff && buffer[0] != 0xfffffffe) {
        gdat = (Gfx *) &(texdat[texptr]);
	switch ((int) buffer[0]) {
	  case 1: /* gsDPSetTextureImage */

#ifdef DEBUG
	    osReadHost(buffer, 8*3);
#endif

	    (*texlist)[numtex-1].image = texptr;
	    (*texlist)[numtex-1].i1 = (int)buffer[0];
	    (*texlist)[numtex-1].i2 = (int)buffer[1];
	    (*texlist)[numtex-1].i3 = (int)buffer[2];
            texptr += 8;
	    break;
	  case 2: /* gsDPSetTile */

#ifdef DEBUG
	    osReadHost(buffer, 8*12);
#endif

	    gDPSetTile(gdat++,(int)buffer[0],(int)buffer[1],
		(int)buffer[2],(int)buffer[3],(int)buffer[4],
		(int)buffer[5],(int)buffer[6],(int)buffer[7],
		(int)buffer[8],(int)buffer[9],(int)buffer[10],
		(int)buffer[11]);
            texptr += 8;
	    break;
	  case 3: /* gsDPLoadBlock */

#ifdef DEBUG
	    osReadHost(buffer, 8*5);
#endif

	    gDPLoadBlock(gdat++,(int)buffer[0],(int)buffer[1],
                (int)buffer[2],(int)buffer[3],(int)buffer[4]);
            texptr += 8;
	    break;
	  case 4: /* gsDPSetTileSize */

#ifdef DEBUG
	    osReadHost(buffer, 8*5);
#endif

	    gDPSetTileSize(gdat++,(int)buffer[0],(int)buffer[1],
                (int)buffer[2],(int)buffer[3],(int)buffer[4]);
            texptr += 8;
	    break;

	}

#ifdef DEBUG
        osReadHost(buffer, 8);
#endif

    }
    gdat = (Gfx *) &(texdat[texptr]);
    gSPEndDisplayList(gdat++);
    texptr += 8;
    (*texlist)[numtex-1].end = texptr;

    if (buffer[0] == 0xfffffffe) {
    	numtex--;
	return;
    }
    
#ifdef DEBUG
    osReadHost(buffer, 8);
#endif

    (*texlist)[numtex-1].levels = buffer[0];
	
}

#define NUMRAND 200
int randptr=0;
s16 randlist[NUMRAND];
s16 deltafunc(int da, float r,int remove)
{
    if (r>=1.0) return 0;

    randptr = ++randptr;
    switch ((int)mainmenu[MM_BFUNC].val) {
      case FN_SQUARE:
    	return (1.0-r*r)*da;
      case FN_SQUAREM1:
        return (1.0-r)*(1.0-r)*da;
      case FN_LINEAR:
	return (1.0-r)*da;
      case FN_RAND:
	if (da == 0) return 0;
	if (remove || randptr>=NUMRAND) {
	    return (randlist[randptr % NUMRAND]);
	} else {
	    return (randlist[randptr % NUMRAND]=RAND(da));
	}
    }
    return da;
}

void applydelta(int da, float dr, int x, int z, int remove)
{
    int xx,zz,xxx,zzz;
    float r;
    remove = remove?(-1):1;

    randptr=0;
    if (dr>0.0)
      for (xx=x-(int)dr-1; xx<=x+(int)dr+1; xx++) {
        for (zz=z-(int)dr-1; zz<=z+(int)dr+1; zz++) {
	  r = sqrtf((xx-x)*(xx-x) + (zz-z)*(zz-z))/(float)dr;
	  xxx=xx;
	  zzz=zz;
	  while (xxx>=dimx-1) xxx-=dimx-1;
	  while (xxx<   0) xxx+=dimx;
	  while (zzz>=dimz-1) zzz-=dimz-1;
	  while (zzz<   0) zzz+=dimz;
	  (*verts + (dimx*zzz) + xxx)->altitude += 
				remove*deltafunc(da,r,remove==-1.0);
        }
      }
    else
      (*verts + (dimx*z) + x)->altitude += da*remove;
}

void render_texoff(Gfx **gp)
{
    u16 txscale;
    int decal;

    if (mainmenu[MM_DIFFON].val == 0.0) {
        if (mainmenu[MM_AMBON].val > 0.0) {
            gDPSetPrimColor((*gp)++, 0,0, 
                (int) (materialmenu   [AM_RED].val * 
                ambientmenu[0][BM_RED].val / 255.0),
                (int) (materialmenu   [AM_GREEN].val *
                ambientmenu[0][BM_GREEN].val / 255.0),
                (int) (materialmenu   [AM_BLUE].val *
                ambientmenu[0][BM_BLUE].val / 255.0),
                255);
        } else {
            gDPSetPrimColor((*gp)++, 0,0, 
                (int) materialmenu[AM_RED].val,
                (int) materialmenu[AM_GREEN].val,
                (int) materialmenu[AM_BLUE].val,
                255);
        }
        if (fogmenu[FM_ENABLE].val==1.0) {
            gSPDisplayList((*gp)++, setrend_fog);
        } else {
            gSPDisplayList((*gp)++, setrend);
        }
    } else {
        if (fogmenu[FM_ENABLE].val==1.0) {
            gSPDisplayList((*gp)++, setrend_light_fog);	
        } else {
            gSPDisplayList((*gp)++, setrend_light);	
        }
    }
    txscale = (int)(texturemenu[TM_TXSCALE].val * 65536.0);
    if (texturemenu[TM_TXSCALE].val >= 1.0)
	txscale = 65535;
    decal = (modemenu[OM_CONTOUR].val==1.0)?(int)modemenu[OM_DECAL].val:0;
    gSPTextureL((*gp)++, txscale,txscale,0,
			(int)decal,G_TX_RENDERTILE,G_OFF);
}

void render_texon(Gfx **gp,int thetex)
{
    u16 txscale;
    int decal;

    if (mainmenu[MM_DIFFON].val == 0.0) {
        if (mainmenu[MM_AMBON].val > 0.0) {
            if (fogmenu[FM_ENABLE].val==1.0) {
                gSPDisplayList((*gp)++, setrend_trilerp_prim_fog);
            } else {
                gSPDisplayList((*gp)++, setrend_trilerp_prim);
            }
        } else {
            if (fogmenu[FM_ENABLE].val==1.0) {
                gSPDisplayList((*gp)++, setrend_trilerp_fog);
            } else {
                gSPDisplayList((*gp)++, setrend_trilerp);
            }
        }
    } else {
        if (fogmenu[FM_ENABLE].val==1.0) {
            gSPDisplayList((*gp)++, setrend_light_trilerp_fog);
        } else {
            gSPDisplayList((*gp)++, setrend_light_trilerp);
        }
    }
    txscale = (int)(texturemenu[TM_TXSCALE].val * 65536.0);
    if (texturemenu[TM_TXSCALE].val >= 1.0)
	txscale = 65535;
    decal = (modemenu[OM_CONTOUR].val==1.0)?(int)modemenu[OM_DECAL].val:0;
    gSPTextureL((*gp)++, txscale,txscale,(*texlist)[thetex-1].levels,
        (int)decal,G_TX_RENDERTILE,G_ON);
}

/*
 * This is the main routine of the app.
 */
void
game(void)
{
    OSTask      *theadp;
    Dynamic     *dynamicp;
    OSContPad	**pad;
    u16		perspnorm;
    float	rota, rotb;
    float	pos[3];
    float	scale;
    int		cmode=0;
    char 	*cstring=" ";
    char	str[100];
    float	movex, movey;
    int		firstloop=1;
    int		menuon =0;
    MenuItem	*menu,*mp,*item;
    int		oldbut;
    int 	i,j;
    float	eyex,eyey,eyez,atx,aty,atz,upx,upy,upz;
    int		selx=20,selz=20;
    int		deltalt=0, olddeltalt=0;
    float	deltrad=0.0;
    int		redodl;
    float	oldbfunc=-1.0;
    u64		cmdbuf=0;
    char	*cmd=(char *) &cmdbuf;
    int		texorig[2]={0,0};
    int		texdraw=0;
    int		triorient=0;
    

    /*
     * setup light menus
     */
    for (i=0; i<MAXLIGHTS; i++) {
        for (j=0; j<=LM_END; j++)
    	    lightmenu[i][j] = lighttemplate[j];
        lightmenu[i][0].text = lmStr[i];
    }
    for (j=0; j<=BM_END; j++)
        ambientmenu[0][j] = ambienttemplate[j];

    /*
     * setup default lights
     */
    ambientmenu[0][BM_RED].val = 16;
    ambientmenu[0][BM_GREEN].val = 16;
    ambientmenu[0][BM_BLUE].val = 16;
    lightmenu[0][LM_X].val = 64;
    lightmenu[0][LM_Y].val = 0;
    lightmenu[0][LM_Z].val = 10;
    lightmenu[0][LM_RED].val = 200;
    lightmenu[0][LM_GREEN].val = 50;
    lightmenu[0][LM_BLUE].val = 200;
    lightmenu[1][LM_X].val = -64;
    lightmenu[1][LM_Y].val = -64;
    lightmenu[1][LM_Z].val = 10;
    lightmenu[1][LM_RED].val = 200;
    lightmenu[1][LM_GREEN].val = 200;
    lightmenu[1][LM_BLUE].val = 50;
    
    /*
     * setup ground
     */
    makeground();
    selx=dimx/2;
    selz=dimz/2;
#ifdef DEBUG    
    PRINTF("returned from makeground\n");
#endif

    /*
     * Main game loop
     */
    osSyncPrintf("Use the L button and crosshair for menu options.\n");
    oldbut = 0;
    while (1) {



	pad = ReadController(0);
	redodl=0;

	if (pad[0]->button & CONT_G) 
	    cmode=1;
	else
	    cmode=0;

	movex = PADTHRESH(pad[0]->stick_x, 10);
	movey = PADTHRESH(pad[0]->stick_y, 10);
	movex *= ABS(movex);
	movey *= ABS(movey);

	if ((pad[0]->button & CONT_START && !(oldbut & CONT_START)) || 
	    firstloop) {
		pos[0] = 0.0;
		pos[1] = 0.0;
		pos[2] = 0.0;
		scale  = 0.31;
		rota   = 333.0;
		rotb   = 19.0;
		firstloop=0;
		menu=mainmenu;
		item = &menu[1];
		cmode=0;
		eyex = 0;
		eyey = 0;
		eyez = 500;
		atx = 0;
		aty = 0;
		atz = 0;
		upx = 0;
		upy = 1;
		upz = 0;
	}

	if (pad[0]->button & CONT_L && !(oldbut & CONT_L)) {
		menuon ^= 1;
	}
	if (menuon) {
	    if (pad[0]->button & CONT_RIGHT) {
	      if (menu == modemenu) {
		redodl = 1;
	      }
	      if (!(oldbut & CONT_RIGHT)) {
		if (item->type == MT_MENU || item->type == MT_UPMENU) {
		    if (item->type == MT_MENU) {
			if (item->nextmenu[1].type == MT_UPMENU)
			    item->nextmenu[1].nextmenu = menu;
		    }
		    menu = item->nextmenu;
		    item = menu;
		    pad[0]->button |= CONT_DOWN;
		} else if (item->type == MT_MULVALI || 
			   item->type == MT_MULVALF) {
		    item->val = 
			MAX(item->min, 
			MIN(item->max, item->val*item->inc));
		} else if (item->type == MT_ADDVALI || 
			   item->type == MT_ADDVALF || 
			   item->type == MT_ADDVALS) {
		    item->val = 
			MAX(item->min, 
			MIN(item->max, item->val+item->inc));
		}
	      }	
	      if (item->type == MT_FADVALI) {
		    item->val =
			MAX(item->min,
			MIN(item->max, item->val+item->inc));
	      }
	    }
	    if (pad[0]->button & CONT_LEFT) {
	      if (menu == modemenu) {
		redodl = 1;
	      }
	      if (!(oldbut & CONT_LEFT)) {
		if (item->type == MT_MENU || item->type == MT_UPMENU) {
		    if (item->type == MT_MENU) {
			if (item->nextmenu[1].type == MT_UPMENU)
			    item->nextmenu[1].nextmenu = menu;
		    }
		    menu = item->nextmenu;
		    item = menu;
		    pad[0]->button |= CONT_DOWN;
		} else if (item->type == MT_MULVALI || 
			   item->type == MT_MULVALF) {
		    item->val = 
			MAX(item->min, 
			MIN(item->max, item->val/item->inc));
		} else if (item->type == MT_ADDVALI || 
			   item->type == MT_ADDVALF ||
			   item->type == MT_ADDVALS) {
		    item->val = 
			MAX(item->min, 
			MIN(item->max, item->val-item->inc));
		}
	      }
	      if (item->type == MT_FADVALI) {
		    item->val =
			MAX(item->min,
			MIN(item->max, item->val-item->inc));
	      }
	    }
	    if (pad[0]->button & CONT_R && !(oldbut & CONT_R)) {
	        if (item->type == MT_MULVALI ||
		    item->type == MT_MULVALF ||
		    item->type == MT_ADDVALI ||
		    item->type == MT_ADDVALF ||
                    item->type == MT_ADDVALS ||
		    item->type == MT_FADVALI) {
		    if (item->val == item->max)
			item->val = item->min;
		    else if (item->val == item->min)
			item->val = (item->min+item->max)/2;
		    else
			item->val = item->max;
		}
	    }
	    if (pad[0]->button & CONT_DOWN && !(oldbut & CONT_DOWN)) {
		item++;
		while (item->type <= MT_SEPERATOR) {
		    if (item->type == MT_SEPERATOR) item++;
		    else item = menu;
		}
	    }
	    if (pad[0]->button & CONT_UP && !(oldbut & CONT_UP)) {
		item--;
		while (item < menu || item->type <= MT_SEPERATOR) {
		    if (item < menu) {
			item = menu;
			while(item->type != MT_END) item++;
		    } else item--;
		}
	    }
	}

	/*
	 * Copy mirrored menu items
	 */
	if (item->text == ambienttemplate[BM_AMBON].text)
	    mainmenu[MM_AMBON].val = item->val;
	if (item->text == ambienttemplate[BM_DIFFON].text)
	    mainmenu[MM_DIFFON].val = item->val;
	if (item->text == lighttemplate[LM_AMBON].text)
	    mainmenu[MM_AMBON].val = item->val;
	if (item->text == lighttemplate[LM_DIFFON].text)
	    mainmenu[MM_DIFFON].val = item->val;

	if ((item->text == lighttemplate[LM_AMBON].text) ||
	    (item->text == ambienttemplate[BM_AMBON].text) ||
	    (item->text == mainmenu[MM_AMBON].text)) {
	    for (i=0; i<MAXLIGHTS; i++)
	        lightmenu[i][LM_AMBON].val = item->val;
	    ambientmenu[0][LM_AMBON].val = item->val;
	}

	if ((item->text == lighttemplate[LM_DIFFON].text) ||
	    (item->text == ambienttemplate[BM_DIFFON].text) ||
	    (item->text == mainmenu[MM_DIFFON].text)) {
	    for (i=0; i<MAXLIGHTS; i++)
	        lightmenu[i][LM_DIFFON].val = item->val;
	    ambientmenu[0][LM_DIFFON].val = item->val;
	}

	/*
	 * check change of dimension
	 */
	if (dimx != modemenu[OM_DIMX].val ||
		dimz != modemenu[OM_DIMZ].val) {
	    int olddimx, olddimz, i, j;

	    olddimx=dimx;
	    olddimz=dimz;
	    dimx = modemenu[OM_DIMX].val;
	    dimz = modemenu[OM_DIMZ].val;
	    if (dimx*dimz < olddimx*olddimz) {
		for (j=0; j<dimz; j++)
		for (i=0; i<dimx; i++) {
		    (*verts + dimx*j + i)->altitude = 
			(*verts + olddimx*j + i)->altitude;
		    (*verts + dimx*j + i)->texture[0] = 
			(*verts + olddimx*j + i)->texture[0];
		    (*verts + dimx*j + i)->texture[1] = 
			(*verts + olddimx*j + i)->texture[1];
		    (*verts + dimx*j + i)->flags = 
			(*verts + olddimx*j + i)->flags;
		    (*verts + dimx*j + i)->offset[0] = 
			(*verts + olddimx*j + i)->offset[0];
		    (*verts + dimx*j + i)->offset[1] = 
			(*verts + olddimx*j + i)->offset[1];
		    (*verts + dimx*j + i)->s = 
			(*verts + olddimx*j + i)->s;
		    (*verts + dimx*j + i)->t = 
			(*verts + olddimx*j + i)->t;
		}
	    }
    	    verts = (VertS **) 
		myrealloc((char **)verts, dimx*dimz*sizeof(VertS));
    	    groundvtx = (Vtx **) 
		myrealloc((char **)groundvtx, dimx*dimz*sizeof(Vtx));
	    if (dimx*dimz > olddimx*olddimz) {
		for (j=olddimz-1; j>=0; j--)
		for (i=olddimx-1; i>=0; i--) {
		    (*verts + dimx*j + i)->altitude = 
			(*verts + olddimx*j + i)->altitude;
		    (*verts + dimx*j + i)->texture[0] = 
			(*verts + olddimx*j + i)->texture[0];
		    (*verts + dimx*j + i)->texture[1] = 
			(*verts + olddimx*j + i)->texture[1];
		    (*verts + dimx*j + i)->flags = 
			(*verts + olddimx*j + i)->flags;
		    (*verts + dimx*j + i)->offset[0] = 
			(*verts + olddimx*j + i)->offset[0];
		    (*verts + dimx*j + i)->offset[1] = 
			(*verts + olddimx*j + i)->offset[1];
		    (*verts + dimx*j + i)->s = 
			(*verts + olddimx*j + i)->s;
		    (*verts + dimx*j + i)->t = 
			(*verts + olddimx*j + i)->t;
		}
		for (j=0; j<olddimz; j++)
		for (i=olddimx; i<dimx; i++) {
		    (*verts + dimx*j + i)->altitude = 0;
		    (*verts + dimx*j + i)->texture[0] = 0;
		    (*verts + dimx*j + i)->texture[1] = 0;
		    (*verts + dimx*j + i)->flags = 0;
		    (*verts + dimx*j + i)->offset[0] = 0;
		    (*verts + dimx*j + i)->offset[1] = 0;
		    (*verts + dimx*j + i)->s = (i-(dimx/2.0))*32*32/5;
		    (*verts + dimx*j + i)->t = (j-(dimz/2.0))*32*32/5;
		}
		for (j=olddimz; j<dimz; j++)
		for (i=0; i<dimx; i++) {
		    (*verts + dimx*j + i)->altitude = 0;
		    (*verts + dimx*j + i)->texture[0] = 0;
		    (*verts + dimx*j + i)->texture[1] = 0;
		    (*verts + dimx*j + i)->flags = 0;
		    (*verts + dimx*j + i)->offset[0] = 0;
		    (*verts + dimx*j + i)->offset[1] = 0;
		    (*verts + dimx*j + i)->s = (i-(dimx/2.0))*32*32/5;
		    (*verts + dimx*j + i)->t = (j-(dimz/2.0))*32*32/5;
		}
	    }
    	    selx=dimx/2;
    	    selz=dimz/2;
	    redodl=1;
	}

	/*
	 * menu commands 
	 */
	if (texturemenu[TM_NEWTEX].val != 0.0) {
	    cmd[0]='t';

#ifdef DEBUG
	    osWriteHost(cmd,4);
#endif
	    loadtexture();
	    texturemenu[TM_NEWTEX].val = 0;
	    texturemenu[TM_TEXTURE].val = numtex;
	    texturemenu[TM_TEXTURE].max = numtex;
	    redodl = 1;
	}

	/*
	 * ensure lights have nonzero directions 
	 * (or else LookatHilight will hang)
	 */

    	for (i=0; i<MAXLIGHTS; i++) {
	    if (lightmenu[i][LM_X].val <  1.0 &&
		lightmenu[i][LM_Y].val <  1.0 &&
		lightmenu[i][LM_Z].val <  1.0 &&
		lightmenu[i][LM_X].val > -1.0 &&
		lightmenu[i][LM_Y].val > -1.0 &&
		lightmenu[i][LM_Z].val > -1.0) {
		lightmenu[i][LM_X].val = 1.0;
		lightmenu[i][LM_Y].val = 1.0;
		lightmenu[i][LM_Z].val = 1.0;
	    }
    	}

	/*
	 * let the joystick control the geometry
	 */
	if (!menuon && cmode==1) {
	    if (pad[0]->button & CONT_UP)
	    	pos[2] -= 5;
	    if (pad[0]->button & CONT_DOWN)
	    	pos[2] += 5;
	    if (pad[0]->button & CONT_LEFT)
	    	pos[0] += 5;
	    if (pad[0]->button & CONT_RIGHT)
	    	pos[0] -= 5;
	}
	if (!menuon && cmode==0) {
	    if (pad[0]->button & CONT_UP  && !(oldbut & CONT_UP)) {
	    	if (--selz < 0) selz = dimz-1;
	        redodl=1;
	        olddeltalt = 0;
		texorig[0]=(*verts + dimx*selz + selx)->texture[0];
		texorig[1]=(*verts + dimx*selz + selx)->texture[1];
		triorient = 0;
	    }
	    if (pad[0]->button & CONT_DOWN  && !(oldbut & CONT_DOWN)) {
	    	if (++selz >= dimz) selz=0;
	        redodl=1;
	        olddeltalt = 0;
		texorig[0]=(*verts + dimx*selz + selx)->texture[0];
		texorig[1]=(*verts + dimx*selz + selx)->texture[1];
		triorient = 0;
	    }
	    if (pad[0]->button & CONT_LEFT  && !(oldbut & CONT_LEFT)) {
	    	if (--selx < 0) selx = dimx-1;
	        redodl=1;
	        olddeltalt = 0;
		texorig[0]=(*verts + dimx*selz + selx)->texture[0];
		texorig[1]=(*verts + dimx*selz + selx)->texture[1];
		triorient = 0;
	    }
	    if (pad[0]->button & CONT_RIGHT  && !(oldbut & CONT_RIGHT)) {
	    	if (++selx >= dimx) selx=0;
	        redodl=1;
	        olddeltalt = 0;
		texorig[0]=(*verts + dimx*selz + selx)->texture[0];
		texorig[1]=(*verts + dimx*selz + selx)->texture[1];
		triorient = 0;
	    }
	}

	if (mainmenu[MM_CURSORMODE].val == 0.0) {
	  if (pad[0]->button & CONT_C) {
	    if ((deltrad -= 0.5) <0) deltrad=0;
	    redodl=1;
	  }
	  if (pad[0]->button & CONT_F) {
	    deltrad += 0.5;
	    redodl=1;
	  }
	  if (pad[0]->button & CONT_E) {
	    olddeltalt = 0;
	    deltalt += 10;
	    redodl=1;
	  }
	  if (pad[0]->button & CONT_D) {
	    olddeltalt = 0;
	    deltalt -= 10;
	    redodl=1;
	  }
	  if (pad[0]->button & CONT_A && !(oldbut & CONT_A)) {
	    int tmp;
	    tmp = olddeltalt;
	    olddeltalt = deltalt;
	    deltalt = tmp;
	    redodl=1;
	  }
	  if (pad[0]->button & CONT_B && !(oldbut & CONT_B)) {
	    int tmp;
	    applydelta(deltalt, deltrad, selx, selz, 0);
	    tmp = olddeltalt;
	    olddeltalt = deltalt;
	    deltalt = tmp;
	    applydelta(deltalt, deltrad, selx, selz, 1);
	    redodl=1;
	  }
	  if (deltalt) cstring="A=revert   B=keep changes";
	  else cstring="Move Cursor";
	} else if (mainmenu[MM_CURSORMODE].val == 1.0) {
	  if (pad[0]->button & CONT_A && !(oldbut & CONT_A)) {
	      texdraw ^= 1;
	      redodl=1;
	      texorig[0]=(*verts + dimx*selz + selx)->texture[0];
	      texorig[1]=(*verts + dimx*selz + selx)->texture[1];
	      triorient = 0;
	  }
	  if (pad[0]->button & CONT_B && !(oldbut & CONT_B)) {
	      texturemenu[TM_TEXTURE].val += texturemenu[TM_TEXTURE].inc;
	      if (texturemenu[TM_TEXTURE].val > texturemenu[TM_TEXTURE].max)
		  texturemenu[TM_TEXTURE].val = texturemenu[TM_TEXTURE].min;
	      texorig[0]=(*verts + dimx*selz + selx)->texture[0];
	      texorig[1]=(*verts + dimx*selz + selx)->texture[1];
	      triorient = 0;
	      redodl=1;
	  }
	  if (cmode) {
	      int gm,is,ie,js,je;
	      gm = texturemenu[TM_GROUP].val;
	      is = ie = selx;
	      js = je = selz;
	      switch(gm) {
		case 0:
	          is = ie = selx;
	          js = je = selz;
		  break;
		case 1:
		  is = 0;
		  ie = dimx-1;
		  js = selz;
		  je = dimz-1;
		  break;
		case 2:
		  is = 0;
		  ie = dimx-1;
		  js = 0;
		  je = selz;
		  break;
		case 3:
		  is = selx;
		  ie = dimx-1;
		  js = 0;
		  je = dimz-1;
		  break;
		case 4:
		  is = 0;
		  ie = selx;
		  js = 0;
		  je = dimz-1;
		  break;
		case 5:
		  is = selx;
		  ie = dimx-1;
		  js = 0;
		  je = selz;
		  break;
		case 6:
		  is = 0;
		  ie = selx;
		  js = 0;
		  je = selz;
		  break;
		case 7:
		  is = selx;
		  ie = dimx-1;
		  js = selz;
		  je = dimz-1;
		  break;
		case 8:
		  is = 0;
		  ie = selx;
		  js = selz;
		  je = dimz-1;
		  break;
		case 9:
		  is = selx;
		  ie = -1;
		  js = -1;
		  je = selz;
		  break;
		case 10:
		  is = -1;
		  ie = selx;
		  js = -1;
		  je = selz;
		  break;
		case 11:
		  is = selx;
		  ie = -1;
		  js = selz;
		  je = -1;
		  break;
		case 12:
		  is = -1;
		  ie = selx;
		  js = selz;
		  je = -1;
		  break;
		case 13:
		  is = 0;
		  ie = dimx-1;
		  js = 0;
		  je = dimz-1;
		  break;
	      }
	      if (pad[0]->button & CONT_C) {
		  if (is<0) is = 0;
		  if (ie<0) ie = dimx-1;
		  if (js<0 || je<0) {
		      js = 0;
		      je = dimz-1;
		  }
		  for (i=is; i<=ie; i++)
		  for (j=js; j<=je; j++)
		    if (texturemenu[TM_OP].val == 0)
		      (*verts + dimx*j + i)->s += 16;
		    else
		      (*verts + dimx*j + i)->s += (i-selx)*4;
	          redodl=1;
	      }
	      if (pad[0]->button & CONT_F) {
		  if (is<0) is = 0;
		  if (ie<0) ie = dimx-1;
		  if (js<0 || je<0) {
		      js = 0;
		      je = dimz-1;
		  }
		  for (i=is; i<=ie; i++)
		  for (j=js; j<=je; j++)
		    if (texturemenu[TM_OP].val == 0)
		      (*verts + dimx*j + i)->s -= 16;
		    else
		      (*verts + dimx*j + i)->s -= (i-selx)*4;
	          redodl=1;
	      }
	      if (pad[0]->button & CONT_D) {
		  if (is<0 || ie<0) {
		      is = 0;
		      ie = dimx-1;
		  }
		  if (js<0) js = 0;
		  if (je<0) je = dimz-1;
		  for (i=is; i<=ie; i++)
		  for (j=js; j<=je; j++)
		    if (texturemenu[TM_OP].val == 0)
		      (*verts + dimx*j + i)->t -= 16;
		    else
		      (*verts + dimx*j + i)->t += (j-selz)*4;
	          redodl=1;
	      }
	      if (pad[0]->button & CONT_E) {
		  if (is<0 || ie<0) {
		      is = 0;
		      ie = dimx-1;
		  }
		  if (js<0) js = 0;
		  if (je<0) je = dimz-1;
		  for (i=is; i<=ie; i++)
		  for (j=js; j<=je; j++)
		    if (texturemenu[TM_OP].val == 0)
		      (*verts + dimx*j + i)->t += 16;
		    else
		      (*verts + dimx*j + i)->t -= (j-selz)*4;
	          redodl=1;
	      }
	  } else {
	      if (pad[0]->button & CONT_F && !(oldbut & CONT_F)) {
	          (*verts + dimx*selz + selx)->offset[0] += 1;
	          redodl=1;
	      }
	      if (pad[0]->button & CONT_C && !(oldbut & CONT_C)) {
	          (*verts + dimx*selz + selx)->offset[0] -= 1;
	          redodl=1;
	      }
	      if (pad[0]->button & CONT_E && !(oldbut & CONT_E)) {
	          (*verts + dimx*selz + selx)->offset[1] -= 1;
	          redodl=1;
	      }
	      if (pad[0]->button & CONT_D && !(oldbut & CONT_D)) {
	          (*verts + dimx*selz + selx)->offset[1] += 1;
	          redodl=1;
	      }
	      if (!menuon && pad[0]->button & CONT_R && !(oldbut & CONT_R)) {
	        if (++triorient > 6) {
		    triorient = 1;
	        }
	        redodl=1;
	      }
	  }
	  if (!triorient && texdraw) {
	      if (((*verts + dimx*selz + selx)->texture[0] != 
			(u8) texturemenu[TM_TEXTURE].val) ||
		    ((*verts + dimx*selz + selx)->texture[1] !=
			(u8) texturemenu[TM_TEXTURE].val)) {
		(*verts + dimx*selz + selx)->texture[0] = 
			(u8) texturemenu[TM_TEXTURE].val;
		(*verts + dimx*selz + selx)->texture[1] = 
			(u8) texturemenu[TM_TEXTURE].val;
	        redodl=1;
	      }
	  }

	  switch (triorient) {
	    case 0:
	        cstring = texdraw?"Paint Texture":"Move Cursor";
		break;
	    case 1:
	        (*verts + dimx*selz + selx)->flags &=  ~VERT_FLAG_SWAPDIAG;
		(*verts + dimx*selz + selx)->texture[0] = 
			(u8) texturemenu[TM_TEXTURE].val;
		(*verts + dimx*selz + selx)->texture[1] = texorig[1];
	  	cstring=texdraw?"Paint Tri 1":"Move Cursor (T1)";
		break;
	    case 2:
		(*verts + dimx*selz + selx)->texture[0] = texorig[0];
		(*verts + dimx*selz + selx)->texture[1] = 
			(u8) texturemenu[TM_TEXTURE].val;
	  	cstring=texdraw?"Paint Tri 2":"Move Cursor (T2)";
		break;
	    case 3:
	        (*verts + dimx*selz + selx)->flags |=  VERT_FLAG_SWAPDIAG;
		(*verts + dimx*selz + selx)->texture[0] = 
			(u8) texturemenu[TM_TEXTURE].val;
		(*verts + dimx*selz + selx)->texture[1] = texorig[1];
	  	cstring=texdraw?"Paint Tri 1 (alt)":"Move Cursor (T1alt)";
		break;
	    case 4:
		(*verts + dimx*selz + selx)->texture[0] = texorig[0];
		(*verts + dimx*selz + selx)->texture[1] = 
			(u8) texturemenu[TM_TEXTURE].val;
	  	cstring=texdraw?"Paint Tri 2 (alt)":"Move Cursor (T2alt)";
		break;
	    case 5:
	        (*verts + dimx*selz + selx)->flags &=  ~VERT_FLAG_SWAPDIAG;
		(*verts + dimx*selz + selx)->texture[0] = texorig[0];
		(*verts + dimx*selz + selx)->texture[1] = texorig[1];
	  	cstring=texdraw?"Paint Tex (undone)":"Move Cursor";
		break;
	    case 6:
		(*verts + dimx*selz + selx)->texture[0] = 
			(u8) texturemenu[TM_TEXTURE].val;
		(*verts + dimx*selz + selx)->texture[1] = 
			(u8) texturemenu[TM_TEXTURE].val;
	  	cstring=texdraw?"Paint Tex (redone)":"Move Cursor (replaced)";
		break;
	  }
	}
	if (redodl || mainmenu[MM_BFUNC].val != oldbfunc) {
	    applydelta(deltalt, deltrad, selx, selz, 0);
    	    makegrounddl();
	    applydelta(deltalt, deltrad, selx, selz, 1);
	    oldbfunc = mainmenu[MM_BFUNC].val;
	}

	makeseldl(selx, selz);

	switch (cmode) {
	    case 0:
		rota += movex/1000.0;
		rotb -= movey/1000.0;
		while (rota>360.0) rota -= 360.0;
		while (rota<0.0)   rota += 360.0;
		while (rotb>360.0) rotb -= 360.0;
		while (rotb<0.0)   rotb += 360.0;
		break;
	    case 1:
		scale += scale*movex/(80*80*4);
		scale = MAX(scale,0.001);
		pos[1] += movey/400.0;
		break;
	    case 2:
		pos[0] += movex/1000.0;
		pos[2] -= movey/1000.0;
		break;
	    case 3:
		pos[0] += movex/1000.0;
		pos[1] += movey/1000.0;
		break;
	}

	/*
	 * pointers to build the display list.
	 */
	theadp = &taskHeader;
	dynamicp = &dynamic;
	glistp = dynamicp->glist;

	/*
	 * Tell RCP where each segment is
	 */
	gSPSegment(glistp++, 0, 0x0);	/* physical segment */
	gSPSegment(glistp++, STATIC_SEGMENT, 
			OS_K0_TO_PHYSICAL(staticSegment));
	gSPSegment(glistp++, CFB_SEGMENT, 
			OS_K0_TO_PHYSICAL(cfb[draw_buffer]));

	/*
	 * Clear z and color framebuffer.
	 */
	gSPDisplayList(glistp++, clearzbuffer_dl);
	gSPDisplayList(glistp++, clearcfb_dl);

	/*
	 * Initialize RCP state.
	 */
	gSPDisplayList(glistp++, init_dl);

	/*
	 * set up matrices
	 */
	guPerspective(&(dynamicp->projection), &perspnorm,
		30.0, 320.0/240.0, 
		100, 1000, 1.0);


	guPosition(&(dynamicp->modeling), 
		rotb, rota, 0.0, scale, pos[0], pos[1], pos[2]);


	guLookAtHilite(&dynamicp->viewing,
                &(dynamicp->lookat), &(dynamicp->hilite),
		eyex, eyey, eyez,
		atx,  aty,  atz,
		upx,  upy,  upz,

                lightmenu[0][LM_X].val,
                lightmenu[0][LM_Y].val,
                lightmenu[0][LM_Z].val,
                lightmenu[1][LM_X].val,
                lightmenu[1][LM_Y].val,
                lightmenu[1][LM_Z].val,
                32, 32);
		
		
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->projection)),
		G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->viewing)),
		G_MTX_PROJECTION|G_MTX_MUL|G_MTX_NOPUSH);

	gSPPerspNormalize(glistp++, perspnorm);

	gSPLookAt(glistp++, &(dynamicp->lookat));

	if (mainmenu[MM_AMBON].val > 0.0) {
	  dynamicp->light.a.l.col [0] = (int) ambientmenu[0][BM_RED].val;
	  dynamicp->light.a.l.colc[0] = (int) ambientmenu[0][BM_RED].val;
	  dynamicp->light.a.l.col [1] = (int) ambientmenu[0][BM_GREEN].val;
	  dynamicp->light.a.l.colc[1] = (int) ambientmenu[0][BM_GREEN].val;
	  dynamicp->light.a.l.col [2] = (int) ambientmenu[0][BM_BLUE].val;
	  dynamicp->light.a.l.colc[2] = (int) ambientmenu[0][BM_BLUE].val;
	} else {
	  dynamicp->light.a.l.col [0] = (int) 0;
	  dynamicp->light.a.l.colc[0] = (int) 0;
	  dynamicp->light.a.l.col [1] = (int) 0;
	  dynamicp->light.a.l.colc[1] = (int) 0;
	  dynamicp->light.a.l.col [2] = (int) 0;
	  dynamicp->light.a.l.colc[2] = (int) 0;
	}

	if (mainmenu[MM_DIFFON].val > 0.0) {
	    for (i=0; i<MAXLIGHTS; i++) {
		dynamicp->light.l[i].l.col [0] = 
					(int) (lightmenu[i][LM_RED].val *
					materialmenu[AM_DIFFUSE].val / 256.0);
		dynamicp->light.l[i].l.colc[0] = 
					(int) (lightmenu[i][LM_RED].val *
					materialmenu[AM_DIFFUSE].val / 256.0);
		dynamicp->light.l[i].l.col [1] = 
					(int) (lightmenu[i][LM_GREEN].val *
					materialmenu[AM_DIFFUSE].val / 256.0);
		dynamicp->light.l[i].l.colc[1] = 
					(int) (lightmenu[i][LM_GREEN].val *
					materialmenu[AM_DIFFUSE].val / 256.0);
		dynamicp->light.l[i].l.col [2] = 
					(int) (lightmenu[i][LM_BLUE].val *
					materialmenu[AM_DIFFUSE].val / 256.0);
		dynamicp->light.l[i].l.colc[2] = 
					(int) (lightmenu[i][LM_BLUE].val *
					materialmenu[AM_DIFFUSE].val / 256.0);
		dynamicp->light.l[i].l.dir [0] = (int) lightmenu[i][LM_X].val;
		dynamicp->light.l[i].l.dir [1] = (int) lightmenu[i][LM_Y].val;
		dynamicp->light.l[i].l.dir [2] = (int) lightmenu[i][LM_Z].val;
	    }
	    for (i=0; i< (int) mainmenu[MM_DIFFON].val; i++) {
                gSPLight(glistp++, &(dynamicp->light.l[i]), i+1);
	    }
	    gSPLight(glistp++, &(dynamicp->light.a), 
					(int) mainmenu[MM_DIFFON].val+1);
	    gSPNumLights(glistp++, (int) mainmenu[MM_DIFFON].val);
	} else {
	  gSPLight(glistp++, &nolight, 1);
	  gSPLight(glistp++, &(dynamicp->light.a), 2);
	  gSPNumLights(glistp++, NUMLIGHTS_0);
	}

	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->modeling)),
		G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);

	if (fogmenu[FM_ENABLE].val==1.0) {
	    gDPSetFogColor(glistp++, 
			(int)fogmenu[FM_RED].val,
			(int)fogmenu[FM_GREEN].val,
			(int)fogmenu[FM_BLUE].val,
			255);
	    if ((int)fogmenu[FM_MIN].val == (int)fogmenu[FM_MAX].val) {
		gSPFogPosition(glistp++, 
			(int)fogmenu[FM_MIN].val-1,
			(int)fogmenu[FM_MAX].val);
	    } else {
	        gSPFogPosition(glistp++,
			(int)fogmenu[FM_MIN].val,
			(int)fogmenu[FM_MAX].val);
	    }
	}

	/* draw geometry */
	gSPDisplayList(glistp++, *grounddl);

	gDPPipeSync(glistp++);
	gDPSetCycleType(glistp++, G_CYC_1CYCLE);


    gDPPipeSync(glistp++);
    gDPSetCycleType(glistp++, G_CYC_1CYCLE);


	/* 
	 * Draw some text 
	 */
	font_init( &glistp );
	font_set_transparent( 1 );

        font_set_scale( 1.0, 1.0 );
        font_set_win( 200, 1 );


	gDPSetRenderMode(glistp++, 
			G_RM_AA_ZB_TEX_EDGE & ~Z_CMP, 
			G_RM_AA_ZB_TEX_EDGE2 & ~Z_CMP);
	gDPSetDepthSource(glistp++, G_ZS_PRIM);
	gDPSetPrimDepth(glistp++, 0,0);
	FONTCOL(55, 255, 155, 255);
	SHOWFONT(&glistp,cstring,18,210);
	if (mainmenu[MM_CURSORMODE].val==1.0) {
		SHOWFONT(&glistp,txStr[(int)texturemenu[TM_TEXTURE].val],18,190);
	}

	/*
	 * MENU
	 */
	if (menuon) {
	    static float pulse=0, dpulse=0.1;
	    pulse += dpulse;
	    if (pulse>= 1.0) {pulse=1.0; dpulse *= -1; }
	    if (pulse<= 0.0) {pulse=0.0; dpulse *= -1; }
	    movey = 20.0;
	    for (i=0; i<numtex; i++) {
	        txStr[i+1] = ((*texlist)[i].name);
	    }
	    for (mp = menu; mp->type != MT_END; mp++) {
		if (mp == item) {
		    FONTCOL(255, pulse*255, 0, 255);
		} else {
		    FONTCOL(mp->color[0], mp->color[1], 
			    mp->color[2], mp->color[3]);
		}
		if (mp->type == MT_ADDVALI || 
		    mp->type == MT_MULVALI ||
		    mp->type == MT_FADVALI)
		  sprintf(str, "%s%d",mp->text, (int) mp->val);
		else if (mp->type == MT_ADDVALF || 
			 mp->type == MT_MULVALF)
		  sprintf(str, "%s%.2f",mp->text, mp->val);
		else if (mp->type == MT_ADDVALS)
		  sprintf(str, "%s%s",mp->text, 
			  ((char **) mp->nextmenu)[(int)mp->val]);
		else	
		  sprintf(str, "%s",mp->text );
		SHOWFONT( &glistp, str,20,movey );
	    	movey += 11.0;
	    }
	}
	
        font_finish( &glistp );


	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);

#ifdef DEBUG
#ifndef __MWERKS__
	assert((glistp - dynamicp->glist) < GLIST_LEN);
#endif
#endif

	/*
	 * Build graphics task:
	 */
	theadp->t.ucode_boot = (u64 *) rspbootTextStart;
	theadp->t.ucode_boot_size = 
	  (u32) rspbootTextEnd - (u32) rspbootTextStart;

	/*
	 * choose which ucode to run:
	 */
	if (rdp_flag) {
	    /*
	     * RSP output over FIFO to RDP: 
	     */
	    if (modemenu[OM_NEARCLIP].val == 1.0) {
	        theadp->t.ucode = (u64 *) gspF3DEX2_fifoTextStart;
	        theadp->t.ucode_data = (u64 *) gspF3DEX2_fifoDataStart;
	    } else {
	        theadp->t.ucode = (u64 *) gspF3DEX2_NoN_fifoTextStart;
	        theadp->t.ucode_data = (u64 *) gspF3DEX2_NoN_fifoDataStart;
	    }
	    theadp->t.output_buff_size = (u64 *)((int)rdp_output +
			(int)(RDP_OUTPUT_LEN*sizeof(u64)));
	} else {
	    /*
	     * RSP output over XBUS to RDP: 
	     */
	    if (modemenu[OM_NEARCLIP].val == 1.0) {
	    	theadp->t.ucode = (u64 *) gspF3DEX2_xbusTextStart;
	    	theadp->t.ucode_data = (u64 *) gspF3DEX2_xbusDataStart;
	    } else {
	        theadp->t.ucode = (u64 *) gspF3DEX2_NoN_xbusTextStart;
	        theadp->t.ucode_data = (u64 *) gspF3DEX2_NoN_xbusDataStart;
	    }
	}

	/*
	 * initial display list: 
	 */
	theadp->t.data_ptr = (u64 *) dynamicp->glist;
	theadp->t.data_size = 
		(u32) ((glistp - dynamicp->glist) * sizeof(Gfx));

	/*
	 * Write back dirty cache lines that need to be read by the RCP.
	 */
	osWritebackDCache(&dynamic, sizeof(dynamic));

	/*
	 * start up the RSP task
	 */
	osSpTaskStart(theadp);

	/*
	 * wait for RDP completion 
	 */
	(void) osRecvMesg(&rdpMessageQ, NULL, OS_MESG_BLOCK);

	/* 
	 * Draw lines
	 */
	glistp = dynamicp->glist;

	gSPSegment(glistp++, 0, 0x0);	/* physical segment */
	gSPSegment(glistp++, STATIC_SEGMENT, 
			OS_K0_TO_PHYSICAL(staticSegment));
	gSPSegment(glistp++, CFB_SEGMENT, 
			OS_K0_TO_PHYSICAL(cfb[draw_buffer]));

 
	gSPDisplayList(glistp++, setframebufs_dl);
	gSPDisplayList(glistp++, init_dl);

	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->projection)),
		G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->viewing)),
		G_MTX_PROJECTION|G_MTX_MUL|G_MTX_NOPUSH);

	gSPPerspNormalize(glistp++, perspnorm);

	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->modeling)),
		G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);

	if (fogmenu[FM_ENABLE].val==1.0) {
	    gDPSetFogColor(glistp++, 
			(int)fogmenu[FM_RED].val,
			(int)fogmenu[FM_GREEN].val,
			(int)fogmenu[FM_BLUE].val,
			255);
	    if ((int)fogmenu[FM_MIN].val == (int)fogmenu[FM_MAX].val) {
		gSPFogPosition(glistp++, 
			(int)fogmenu[FM_MIN].val-1,
			(int)fogmenu[FM_MAX].val);
	    } else {
	        gSPFogPosition(glistp++,
			(int)fogmenu[FM_MIN].val,
			(int)fogmenu[FM_MAX].val);
	    }
	    gSPDisplayList(glistp++, linesetup_fog_dl);
	} else {
	    gSPDisplayList(glistp++, linesetup_dl);
	}
	if (modemenu[OM_CONTOUR].val ==1.0) {
	    gSPDisplayList(glistp++, *groundlinedl);
	}
	gDPSetPrimColor(glistp++,0,0,255,0,0,255);
	gSPDisplayList(glistp++, *groundseldl);
	

	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);

#ifdef DEBUG
#ifndef __MWERKS__
	assert((glistp - dynamicp->glist) < GLIST_LEN);
#endif
#endif

	theadp->t.ucode_boot = (u64 *) rspbootTextStart;
	theadp->t.ucode_boot_size = 
			(u32) rspbootTextEnd - (u32) rspbootTextStart;

	if (rdp_flag) {
	    theadp->t.ucode = (u64 *) gspL3DEX2_fifoTextStart;
	    theadp->t.ucode_data = (u64 *) gspL3DEX2_fifoDataStart;
	    theadp->t.output_buff_size = (u64 *)((int)rdp_output +
			(int)(RDP_OUTPUT_LEN*sizeof(u64)));
	} else {
	    theadp->t.ucode = (u64 *) gspL3DEX2_xbusTextStart;
	    theadp->t.ucode_data = (u64 *) gspL3DEX2_xbusDataStart;
	}

	theadp->t.data_ptr = (u64 *) dynamicp->glist;
	theadp->t.data_size = 
		(u32) ((glistp - dynamicp->glist) * sizeof(Gfx));

	osWritebackDCache(&dynamic, sizeof(dynamic));

	osSpTaskStart(theadp);


	(void) osRecvMesg(&rdpMessageQ, NULL, OS_MESG_BLOCK);

	/*
	 * setup to swap buffers 
	 */
	osViSwapBuffer(cfb[draw_buffer]);

	/*
	 * Make sure there isn't an old retrace in queue 
	 * (assumes queue has a depth of 1) 
	 */
	if (MQ_IS_FULL(&retraceMessageQ))
	    (void) osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);

	/*
	 * Wait for Vertical retrace to finish swap buffers 
	 */
	(void) osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);
	draw_buffer ^= 1;
	oldbut=pad[0]->button;

    }
}
