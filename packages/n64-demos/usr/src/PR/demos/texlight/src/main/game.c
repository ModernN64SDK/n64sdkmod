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

/*---------------------------------------------------------------------*
        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
        
        $RCSfile: game.c,v $
        $Revision: 1.11 $
        $Date: 1999/04/16 09:44:46 $
 *---------------------------------------------------------------------*/

/*
 * File:  onetri.c
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
#include "texture.h"

/*
 * Task header.
 */
OSTask          taskHeader =
{
    M_GFXTASK,			/* type of task */
    OS_TASK_DP_WAIT,		/* flags - wait for DP to be available */
    NULL,			/* ucode boot (fill in later) */
    0,				/* ucode boot size (fill in later) */
    NULL,			/* ucode (fill in later) */
    SP_UCODE_SIZE,		/* ucode size */
    NULL,			/* ucode data (fill in later) (to init DMEM) */
    SP_UCODE_DATA_SIZE,		/* ucode data size */
    &dram_stack[0],		/* stack used by ucode */
    SP_DRAM_STACK_SIZE8,	/* size of stack */
    &rdp_output[0],		/* fifo output buffer start ptr */
    &rdp_output[0]+RDP_OUTPUT_LEN, /* fifo output buffer end ptr */
    NULL,			/* display list pointer (fill in later) */
    0,				/* display list size (ignored) */
    NULL,			/* yield buffer (used if yield will occur) */
    0				/* yield buffer length */
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

/*
 * macros 
 */
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
#define SHOWFONT(glp,str,x,y) \
{ \
    font_set_color( 0,0,0,255);  \
    font_set_pos( (x)+(1), (y)+(0) ); \
    font_show_string( glp, str ); \
    font_set_pos( (x)+(0), (y)+(1) ); \
    font_show_string( glp, str ); \
    font_set_color( fontcol[0],fontcol[1],fontcol[2],fontcol[3]); \
    font_set_pos( x, y ); \
    font_show_string( glp, str ); \
}


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
	float		inc,max,min;
	u8		color[4];
} MenuItem;

char *onStr[] = {
	"off","on",
};
char *bStr[] = {
	"","",
};
char *rtStr[] = {
	"automatic","joystick",
};
char *mmStr[] = {
	"Material Color","Bilinear","Trilinear Mipmap",
};
char *geoStr[] = {
	"Teapot",
	"Cube",
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

MenuItem rotmenu[]= {
	{"ROTATION MENU",      MT_SEPERATOR, 0,0,0,0,0,
	0,   25,   0, 0xff},
	{"Main menu",	       MT_UPMENU,    0,0,0,0,0,
	5, 255, 155, 0xff},
	{"Control Rotation: ", MT_ADDVALS,(MenuItem *) rtStr, 0,1,0,1,
	55, 255, 155, 0xff},
	{"Rotation rate a : ", MT_ADDVALF,   0, 0.4, 0.1, -10.0, 10.0,
	55, 255, 155, 0xff},
	{"Rotation rate b : ", MT_ADDVALF,   0, 0.1, 0.1, -10.0, 10.0,
	55, 255, 155, 0xff},
	{"",                   MT_END,       0,0,0,0,0,
	55, 255, 155, 0xff},
};
enum {
	RM_TITLE,
	RM_MAINMENU,
	RM_ROTON,
	RM_ROTA,
	RM_ROTB,
	RM_END
};

MenuItem texturemenu[] = {
	{"TEXTURE MENU",       MT_SEPERATOR, 0,0,0,0,0,
	0,   25,   0, 0xff},
	{"Main menu",	       MT_UPMENU,    0,0,0,0,0,
	5, 255, 155, 0xff},
	{"Texture         : ", MT_ADDVALI,   0,   0,   1,             0,   3,
	55, 255, 155, 0xff},
	{"Texture Scale   : ", MT_MULVALF,   0, 0.4, 1.2,   1.0/65536.0, 1.0,
	55, 255, 155, 0xff},
	{"",                   MT_END,       0,0,0,0,0,
	55, 255, 155, 0xff},
};
enum {
	TM_TITLE,
	TM_MAINMENU,
	TM_TEXTURE,
	TM_TXSCALE,
	TM_END
};

MenuItem viewmenu[] = {
	{"MATERIAL MENU",      MT_SEPERATOR, 0,0,0,0,0,
	0,   25,   0, 0xff},
	{"Main menu",	       MT_UPMENU,    0,0,0,0,0,
	5, 255, 155, 0xff},
	{"Field of View : ",   MT_ADDVALF,   0, 30.0,1.0,1.0,120.0,       
	55, 255, 155, 0xff},
	{"",                   MT_END,       0,0,0,0,0,
	55, 255, 155, 0xff},
};
enum {
	VM_TITLE,
	VM_MAINMENU,
	VM_FOV,
	VM_END
};

MenuItem materialmenu[] = {
	{"MATERIAL MENU",      MT_SEPERATOR, 0,0,0,0,0,
	0,   25,   0, 0xff},
	{"Main menu",	       MT_UPMENU,    0,0,0,0,0,
	5, 255, 155, 0xff},
	{"Geometry : ",        MT_ADDVALS,(MenuItem *) geoStr, 0,1,0,1,       
	55, 255, 155, 0xff},
	{"Red      : ",        MT_FADVALI,   0, 255,   1,     0,  255,
	5, 255, 155, 0xff},
	{"Green    : ",        MT_FADVALI,   0, 255,   1,     0,  255,
	5, 255, 155, 0xff},
	{"Blue     : ",        MT_FADVALI,   0, 255,   1,     0,  255,
	5, 255, 155, 0xff},
	{"Diffuse Multiplier  :",MT_FADVALI,   0, 100,   1,     0,  255,
	5, 255, 155, 0xff},
	{"Specular Multiplier :",MT_FADVALI,   0, 255,   1,     0,  255,
	5, 255, 155, 0xff},
	{"",                   MT_END,       0,0,0,0,0,
	55, 255, 155, 0xff},
};
enum {
	AM_TITLE,
	AM_MAINMENU,
	AM_GEOM,
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
	LM_DIRECTION,
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
	{"Enable Texture  : ", MT_ADDVALS,(MenuItem *) mmStr, 2,1,0,2,       
	55, 255, 155, 0xff},
	{"Enable Ambient  : ", MT_ADDVALS,(MenuItem *) ltStr, 1,1,0,1,       
	55, 255, 155, 0xff},
	{"Enable Diffuse  : ", MT_ADDVALS,(MenuItem *) ltStr, 2,1,0,7,       
	55, 255, 155, 0xff},
	{"Enable Specular : ", MT_ADDVALS,(MenuItem *) ltStr, 2,1,0,7,       
	55, 255, 155, 0xff},
	{"Texture Menu",       MT_MENU, texturemenu, 0,0,0,0,
	55, 255, 155, 0xff},
	{"Material Menu",      MT_MENU,materialmenu, 0,0,0,0,
	55, 255, 155, 0xff},
	{"Light Menus",        MT_MENU, ambientmenu[0], 0,0,0,0,
	55, 255, 155, 0xff},
	{"Auto Rotation Menu", MT_MENU,     rotmenu, 0,0,0,0,
	55, 255, 155, 0xff},
	{"Viewing Menu",       MT_MENU,     viewmenu, 0,0,0,0,
	55, 255, 155, 0xff},
	{"",                   MT_END,            0, 0,0,0,0,
	55, 255, 155, 0xff},
};
enum {
	MM_TITLE,
	MM_TXON,
	MM_AMBON,
	MM_DIFFON,
	MM_SPECON,
	MM_TXMENU,
	MM_MATMENU,
	MM_LIGHTMENU,
	MM_ROTMENU,
	MM_VIEWMENU,
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
	{"Light Direction",    MT_ADDVALS,(MenuItem *) bStr, 0,1,0,1,
	5, 255, 155, 0xff},
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

#define SP_NONE		0
#define SP_LIGHTDIR	1

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
    char 	*cstring;
    char	str[100];
    float	movex, movey;
    int		firstloop=1;
    int		menuon =0;
    MenuItem	*menu=0,*mp,*item;
    u16		txscale;
    int		rendering;
    int		oldbut;
    int 	i,j;
    Gfx		*geometry;
    int		numspec;
    int		nextspec;
    int		specialmode=SP_NONE;
    char	specialstring[100];
    

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
    /* last light is not used, but neccessary for specular */
    lightmenu[MAXLIGHTS][LM_X].val = 1;
    lightmenu[MAXLIGHTS][LM_Y].val = 1;
    lightmenu[MAXLIGHTS][LM_Z].val = 1;
    lightmenu[MAXLIGHTS][LM_RED].val = 0;
    lightmenu[MAXLIGHTS][LM_GREEN].val = 0;
    lightmenu[MAXLIGHTS][LM_BLUE].val = 0;
    

    /*
     * Main game loop
     */

#ifdef DEBUG
    osSyncPrintf("Use the L button and crosshair for menu options.\n");
#endif

    oldbut = 0;
    while (1) {



	pad = ReadController(CONT_G | CONT_START | CONT_L | CONT_UP | 
					CONT_DOWN | CONT_R);

#define NUMMODES 4
	if (pad[0]->button & CONT_G) if (++cmode>=NUMMODES) cmode=0;;

	movex = PADTHRESH(pad[0]->stick_x, 10);
	movey = PADTHRESH(pad[0]->stick_y, 10);
	movex *= ABS(movex);
	movey *= ABS(movey);

	if (specialmode == SP_LIGHTDIR) {
	    float x,y,l;
	    menuon=0;
	    if (pad[0]->button & (CONT_G | CONT_START | CONT_L | CONT_UP |
                                        CONT_DOWN | CONT_R)) {
		specialmode=SP_NONE;
		menuon=1;
	    }
	    movex *= 0.000001;
	    movey *= 0.001;
	    menu[LM_Z].val -= (int) movey;
	    if (menu[LM_Z].val > 120) menu[LM_Z].val = 120;
	    if (menu[LM_Z].val <-120) menu[LM_Z].val =-120;

	    l = (120.0 * 120.0) - (menu[LM_Z].val * menu[LM_Z].val);
	    if (l<0) l=0;
	    x =   menu[LM_Y].val * menu[LM_Z].val;
	    y = - menu[LM_X].val * menu[LM_Z].val;
	    x *= movex;
	    y *= movex;
	    x += menu[LM_X].val;
	    y += menu[LM_Y].val;
	    if (ABS(x) <0.01 && ABS(y) <0.01) y=1.0;
	    l = sqrtf(l/(x*x + y*y));
	    
	    menu[LM_X].val = x*l;
	    menu[LM_Y].val = y*l;
	 
	    
	    sprintf(specialstring,"Light X=%d Y=%d Z=%d",
			(int) menu[LM_X].val,
			(int) menu[LM_Y].val,
			(int) menu[LM_Z].val);
	    cstring=specialstring;
	    movex = 0;
	    movey = 0;
	    pad[0]->button = 0;
	}


	if ((pad[0]->button & CONT_START) || 
	    firstloop) {
		pos[0] = 0.0;
		pos[1] = 0.0;
		pos[2] = 0.0;
		scale  = 1.0;
		rota   = 0.0;
		rotb   = 0.0;
		firstloop=0;
		menu=mainmenu;
		item = &menu[1];
		cmode=0;
	}

	if (pad[0]->button & CONT_L) {
		menuon ^= 1;
	}
	if (menuon) {
	    if (pad[0]->button & CONT_RIGHT) {
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
			MAX(item->max, 
			MIN(item->min, item->val*item->inc));
		} else if (item->type == MT_ADDVALI || 
			   item->type == MT_ADDVALF || 
			   item->type == MT_ADDVALS) {
		    item->val = 
			MAX(item->max, 
			MIN(item->min, item->val+item->inc));
		}
	      }	
	      if (item->type == MT_FADVALI) {
		    item->val =
			MAX(item->max,
			MIN(item->min, item->val+item->inc));
	      }
	    }
	    if (pad[0]->button & CONT_LEFT) {
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
			MAX(item->max, 
			MIN(item->min, item->val/item->inc));
		} else if (item->type == MT_ADDVALI || 
			   item->type == MT_ADDVALF ||
			   item->type == MT_ADDVALS) {
		    item->val = 
			MAX(item->max, 
			MIN(item->min, item->val-item->inc));
		}
	      }
	      if (item->type == MT_FADVALI) {
		    item->val =
			MAX(item->max,
			MIN(item->min, item->val-item->inc));
	      }
	    }
	    if (pad[0]->button & CONT_R) {
	        if (item->type == MT_MULVALI ||
		    item->type == MT_MULVALF ||
		    item->type == MT_ADDVALI ||
		    item->type == MT_ADDVALF ||
                    item->type == MT_ADDVALS ||
		    item->type == MT_FADVALI) {
		    if (item->val == item->min)
			item->val = item->max;
		    else if (item->val == item->max)
			item->val = (item->max+item->min)/2;
		    else
			item->val = item->min;
		}
	    }
	    if (pad[0]->button & CONT_DOWN) {
		item++;
		while (item->type <= MT_SEPERATOR) {
		    if (item->type == MT_SEPERATOR) item++;
		    else item = menu;
		}
	    }
	    if (pad[0]->button & CONT_UP) {
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

	if ((item->text == lighttemplate[LM_DIRECTION].text) &&
	    (item->val ==1.0)) {
	    item->val = 0;
	    specialmode=SP_LIGHTDIR;
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

	if ((int)rotmenu[RM_ROTON].val==0) {
		if (cmode==0) cmode=1;
		rota += rotmenu[RM_ROTA].val;
		rotb += rotmenu[RM_ROTB].val;
	}
	if (!specialmode)
	switch (cmode) {
	    case 0:
		rota += movex/1000.0;
		rotb -= movey/1000.0;
		if (rota>360.0) rota -= 360.0;
		if (rota<0.0)   rota += 360.0;
		if (rotb>360.0) rotb -= 360.0;
		if (rotb<0.0)   rotb += 360.0;
		cstring="rotate object with joystick";
		break;
	    case 1:
		scale += scale*movex/(80*80*4);
		scale = MAX(scale,0.001);
		cstring="scale object with joystick";
		break;
	    case 2:
		pos[0] += movex/1000.0;
		pos[2] -= movey/1000.0;
		cstring="move object (X,Z) with joystick";
		break;
	    case 3:
		pos[0] += movex/1000.0;
		pos[1] += movey/1000.0;
		cstring="move object (X,Y) with joystick";
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
	gSPSegment(glistp++, TEXTURE_SEGMENT, 
			OS_K0_TO_PHYSICAL(textureSegment));

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
		viewmenu[VM_FOV].val,
		320.0/240.0, 
		100, 1000, 1.0);

	guLookAtHilite(&dynamicp->viewing,
                &(dynamicp->lookat), &(dynamicp->hilite),
                0.0, 0.0, 500.0,
                0.0, 0.0,   0.0,
                0.0, 1.0,   0.0,
                lightmenu[0][LM_X].val,
                lightmenu[0][LM_Y].val,
                lightmenu[0][LM_Z].val,
                lightmenu[1][LM_X].val,
                lightmenu[1][LM_Y].val,
                lightmenu[1][LM_Z].val,
                32, 32);
		
		
	guPosition(&(dynamicp->modeling), 
		rotb, rota, 0.0, scale, pos[0], pos[1], pos[2]);

	guPosition(&(dynamicp->modeling2), 
		0.0, 0.0, 0.0, 1.0, 
		20.0*sinf(rota*PI/180.0), 
		20.0*sinf(rotb*PI/180.0) ,
		20.0*cosf(rota*PI/180.0) );

	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->projection)),
		G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->viewing)),
		G_MTX_PROJECTION|G_MTX_MUL|G_MTX_NOPUSH);

	gSPPerspNormalize(glistp++, perspnorm);

	gSPLookAt(glistp++, &(dynamicp->lookat));

	txscale = (int)(texturemenu[TM_TXSCALE].val * 65536.0);
	if (texturemenu[TM_TXSCALE].val >= 1.0)
		txscale = 65535;


#define REND_TX 	1
#define REND_DIFF 	2
#define REND_SPEC 	4

	rendering = 0;

	if (mainmenu[MM_TXON].val > 0.0) {
	    rendering |= REND_TX;
	    if (mainmenu[MM_TXON].val > 1.0) {
		/*
		 * Mip Mapped Texture
		 */
		switch((int) texturemenu[TM_TEXTURE].val) {
		  case 0:
	    	    gSPDisplayList(glistp++, RGBA16brickMM_dl);
		    gSPTexture(glistp++, txscale, txscale,
			5,G_TX_RENDERTILE, G_ON);
		    break;
		  case 1:
	    	    gSPDisplayList(glistp++, RGBA16checkerMM_dl);
		    gSPTexture(glistp++, txscale, txscale,
			5,G_TX_RENDERTILE, G_ON);
		    break;
		  case 2:
	    	    gSPDisplayList(glistp++, RGBA16moleculeMM_dl);
		    gSPTexture(glistp++, txscale, txscale,
			5,G_TX_RENDERTILE, G_ON);
		    break;
		  case 3:
	    	    gSPDisplayList(glistp++, RGBA16chainlinkMM_dl);
		    gSPTexture(glistp++, txscale, txscale,
			4,G_TX_RENDERTILE, G_ON);
		    break;
		}
	    } else { 
		/*
		 * Non Mip Mapped Texture
		 */
		gSPTexture(glistp++, txscale, txscale,
			0,G_TX_RENDERTILE, G_ON);
		switch((int) texturemenu[TM_TEXTURE].val) {
		  case 0:
	    	    gSPDisplayList(glistp++, load_tex_brick_dl);
		    break;
		  case 1:
	    	    gSPDisplayList(glistp++, load_tex_checker_dl);
		    break;
		  case 2:
	    	    gSPDisplayList(glistp++, load_tex_molecule_dl);
		    break;
		  case 3:
	    	    gSPDisplayList(glistp++, load_tex_chainlink_dl);
		    break;
		}
	    }
	}

	if (mainmenu[MM_DIFFON].val > 0.0) {
	    rendering |= REND_DIFF;
	    if (mainmenu[MM_AMBON].val > 0.0) {
		if (mainmenu[MM_TXON].val > 0.0) {
	            dynamicp->light.a.l.col [0] = 
					(int) ambientmenu[0][BM_RED].val;
	            dynamicp->light.a.l.colc[0] = 
					(int) ambientmenu[0][BM_RED].val;
	            dynamicp->light.a.l.col [1] = 
					(int) ambientmenu[0][BM_GREEN].val;
	            dynamicp->light.a.l.colc[1] = 
					(int) ambientmenu[0][BM_GREEN].val;
	            dynamicp->light.a.l.col [2] = 
					(int) ambientmenu[0][BM_BLUE].val;
	            dynamicp->light.a.l.colc[2] = 
					(int) ambientmenu[0][BM_BLUE].val;
		} else {
	            dynamicp->light.a.l.col [0] = 
					(int) (ambientmenu[0][BM_RED].val *
					materialmenu   [AM_RED].val / 256.0);
	            dynamicp->light.a.l.colc[0] = 
					(int) (ambientmenu[0][BM_RED].val *
					materialmenu   [AM_RED].val / 256.0);
	            dynamicp->light.a.l.col [1] = 
					(int) (ambientmenu[0][BM_GREEN].val *
					materialmenu   [AM_GREEN].val / 256.0);
	            dynamicp->light.a.l.colc[1] = 
					(int) (ambientmenu[0][BM_GREEN].val *
					materialmenu   [AM_GREEN].val / 256.0);
	            dynamicp->light.a.l.col [2] = 
					(int) (ambientmenu[0][BM_BLUE].val *
					materialmenu   [AM_BLUE].val / 256.0);
	            dynamicp->light.a.l.colc[2] = 
					(int) (ambientmenu[0][BM_BLUE].val *
					materialmenu   [AM_BLUE].val / 256.0);
		}
	    } else {
	        dynamicp->light.a.l.col [0] = (int) 0;
	        dynamicp->light.a.l.colc[0] = (int) 0;
	        dynamicp->light.a.l.col [1] = (int) 0;
	        dynamicp->light.a.l.colc[1] = (int) 0;
	        dynamicp->light.a.l.col [2] = (int) 0;
	        dynamicp->light.a.l.colc[2] = (int) 0;
	    }
	    for (i=0; i<MAXLIGHTS; i++) {
		if (mainmenu[MM_TXON].val > 0.0) {
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
		} else {
		    dynamicp->light.l[i].l.col [0] = 
					(int) (lightmenu[i][LM_RED].val *
					materialmenu   [AM_RED].val *
					materialmenu[AM_DIFFUSE].val / 65536.0);
		    dynamicp->light.l[i].l.colc[0] = 
					(int) (lightmenu[i][LM_RED].val *
					materialmenu   [AM_RED].val *
					materialmenu[AM_DIFFUSE].val / 65536.0);
		    dynamicp->light.l[i].l.col [1] = 
					(int) (lightmenu[i][LM_GREEN].val *
					materialmenu   [AM_GREEN].val *
					materialmenu[AM_DIFFUSE].val / 65536.0);
		    dynamicp->light.l[i].l.colc[1] = 
					(int) (lightmenu[i][LM_GREEN].val *
					materialmenu   [AM_GREEN].val *
					materialmenu[AM_DIFFUSE].val / 65536.0);
		    dynamicp->light.l[i].l.col [2] = 
					(int) (lightmenu[i][LM_BLUE].val *
					materialmenu   [AM_BLUE].val *
					materialmenu[AM_DIFFUSE].val / 65536.0);
		    dynamicp->light.l[i].l.colc[2] = 
					(int) (lightmenu[i][LM_BLUE].val *
					materialmenu   [AM_BLUE].val *
					materialmenu[AM_DIFFUSE].val / 65536.0);
		}
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
	}

	numspec = (int) mainmenu[MM_SPECON].val;
	nextspec = 0;
	if (numspec > 0) {
	    rendering |= REND_SPEC;
	    gDPSetPrimColor(glistp++, 0, 0,
				(int) (lightmenu[0][LM_RED].val *
				materialmenu[AM_SPEC].val / 256.0),
				(int) (lightmenu[0][LM_GREEN].val *
				materialmenu[AM_SPEC].val / 256.0),
				(int) (lightmenu[0][LM_BLUE].val *
				materialmenu[AM_SPEC].val / 256.0),
				255);
    	    gDPSetEnvColor(glistp++,
				(int) (lightmenu[1][LM_RED].val *
				materialmenu[AM_SPEC].val / 256.0),
				(int) (lightmenu[1][LM_GREEN].val *
				materialmenu[AM_SPEC].val / 256.0),
				(int) (lightmenu[1][LM_BLUE].val *
				materialmenu[AM_SPEC].val / 256.0),
				255);
	}

	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->modeling)),
		G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);

	switch ((int)materialmenu[AM_GEOM].val) {
	  case 0:
	    geometry = teapot_dl;
	    break;
	  case 1:
	    geometry = cube_dl;
	    break;
	}

	switch (rendering) {

	  /******************************************************************/
	  case 0: /* no spec; no diff; no texture ***************************/
	    if (mainmenu[MM_AMBON].val > 0.0) {
		gDPSetPrimColor(glistp++, 0,0, 
			(int) (materialmenu   [AM_RED].val * 
			        ambientmenu[0][BM_RED].val / 255.0),
			(int) (materialmenu   [AM_GREEN].val *
			        ambientmenu[0][BM_GREEN].val / 255.0),
			(int) (materialmenu   [AM_BLUE].val *
			        ambientmenu[0][BM_BLUE].val / 255.0),
			255);
	    } else {
		gDPSetPrimColor(glistp++, 0,0, 
			(int) materialmenu[AM_RED].val,
			(int) materialmenu[AM_GREEN].val,
			(int) materialmenu[AM_BLUE].val,
			255);
	    }
	    gSPDisplayList(glistp++, setrend);
	    gSPDisplayList(glistp++, geometry);
	    break;

	  /******************************************************************/
	  case 1: /* no spec; no diff;    texture ***************************/
	  case 5: /*    spec; no diff;    texture ***************************/
	    if (mainmenu[MM_AMBON].val > 0.0) {
		if (mainmenu[MM_TXON].val > 1.0) {
		    gSPDisplayList(glistp++, setrend_trilerp_prim);
		} else {
		    gSPDisplayList(glistp++, setrend_bilerp_prim);
		}
		gDPSetPrimColor(glistp++, 0,0, 
			(int) ambientmenu[0][BM_RED].val,
			(int) ambientmenu[0][BM_GREEN].val,
			(int) ambientmenu[0][BM_BLUE].val,
			255);
	    } else {
		if (mainmenu[MM_TXON].val > 1.0) {
		    gSPDisplayList(glistp++, setrend_trilerp);
		} else {
		    gSPDisplayList(glistp++, setrend_bilerp);
		}
	    }
	    gSPDisplayList(glistp++, geometry);
	    break;

	  /******************************************************************/
	  case 2: /* no spec;    diff; no texture ***************************/
	    gSPDisplayList(glistp++, setrend_light);	
	    gSPDisplayList(glistp++, geometry);
	    break;

	  /******************************************************************/
	  case 3: /* no spec;    diff;    texture ***************************/
	  case 7: /*    spec;    diff;    texture ***************************/
	    if (mainmenu[MM_TXON].val > 1.0) {
	        gSPDisplayList(glistp++, setrend_light_trilerp);	
	    } else {
	        gSPDisplayList(glistp++, setrend_light_bilerp);	
	    }
	    gSPDisplayList(glistp++, geometry);
	    break;

	  /******************************************************************/
	  case 4: /*    spec; no diff; no texture ***************************/
	    if (numspec > 1) {
	        gSPDisplayList(glistp++, setrend_2spec);
	        gDPSetHilite1Tile(glistp++,G_TX_RENDERTILE  ,
				&(dynamicp->hilite), 32,32);
	    	gDPSetHilite2Tile(glistp++,G_TX_RENDERTILE+1,
				&(dynamicp->hilite), 32,32);
	    } else {
	        gSPDisplayList(glistp++, setrend_1spec);
	        gDPSetHilite1Tile(glistp++,G_TX_RENDERTILE  ,
				&(dynamicp->hilite), 32,32);
	    }
	    /* use ambient light to show material and/or ambient color   */
	    /* This lighting is free because we are autogenerating S & T */
	    if (mainmenu[MM_AMBON].val > 0.0) {
	        dynamicp->light.a.l.col [0] = 
				(int) (materialmenu   [AM_RED].val *
				ambientmenu[0][BM_RED].val / 255.0);
	        dynamicp->light.a.l.col [1] = 
				(int) (materialmenu   [AM_GREEN].val *
				ambientmenu[0][BM_GREEN].val / 255.0);
	        dynamicp->light.a.l.col [2] = 
				(int) (materialmenu   [AM_BLUE].val *
				ambientmenu[0][BM_BLUE].val / 255.0);
	        dynamicp->light.a.l.colc[0] = 
				(int) (materialmenu   [AM_RED].val *
				ambientmenu[0][BM_RED].val / 255.0);
	        dynamicp->light.a.l.colc[1] = 
				(int) (materialmenu   [AM_GREEN].val *
				ambientmenu[0][BM_GREEN].val / 255.0);
	        dynamicp->light.a.l.colc[2] = 
				(int) (materialmenu   [AM_BLUE].val *
				ambientmenu[0][BM_BLUE].val / 255.0);
	    } else {
	        dynamicp->light.a.l.col [0] = (int) materialmenu[AM_RED].val;
	        dynamicp->light.a.l.col [1] = (int) materialmenu[AM_GREEN].val;
	        dynamicp->light.a.l.col [2] = (int) materialmenu[AM_BLUE].val;
	        dynamicp->light.a.l.colc[0] = (int) materialmenu[AM_RED].val;
	        dynamicp->light.a.l.colc[1] = (int) materialmenu[AM_GREEN].val;
	        dynamicp->light.a.l.colc[2] = (int) materialmenu[AM_BLUE].val;
	    }
	    dynamicp->light.l[0].l.dir [0] = 0;
	    dynamicp->light.l[0].l.dir [1] = 0;
	    dynamicp->light.l[0].l.dir [2] = 0;
	    gSPSetLights0(glistp++, (dynamicp->light));
	    gSPDisplayList(glistp++, geometry);
	    numspec -= 2;
	    nextspec += 2;
	    break;

	  /******************************************************************/
	  case 6: /*    spec;    diff; no texture ***************************/
	    if (numspec > 1) {
	        gSPDisplayList(glistp++, setrend_2spec);
	        gDPSetHilite1Tile(glistp++,G_TX_RENDERTILE  ,
				&(dynamicp->hilite), 32,32);
	    	gDPSetHilite2Tile(glistp++,G_TX_RENDERTILE+1,
				&(dynamicp->hilite), 32,32);
	    } else {
	        gSPDisplayList(glistp++, setrend_1spec);
	        gDPSetHilite1Tile(glistp++,G_TX_RENDERTILE  ,
				&(dynamicp->hilite), 32,32);
	    }
	    gSPDisplayList(glistp++, geometry);
	    numspec -= 2;
	    nextspec += 2;
	    break;
	}

	/*
	 * Do additional passes for additional specular highlights
	 */
	while (numspec > 0) {
	    gSPSetLights0(glistp++, nolight);
	    if (numspec > 1) {
	        gSPDisplayList(glistp++, setrend_2spec);
	        gDPSetHilite1Tile(glistp++,G_TX_RENDERTILE  ,
				&(dynamicp->hilite), 32,32);
	    	gDPSetHilite2Tile(glistp++,G_TX_RENDERTILE+1,
				&(dynamicp->hilite), 32,32);
		gDPSetRenderMode(glistp++,
			G_RM_PASS, 
			G_RM_ZB_XLU_DECAL2 & ~CVG_DST_FULL | CVG_DST_SAVE);
	    } else {
	        gSPDisplayList(glistp++, setrend_1spec);
	        gDPSetHilite1Tile(glistp++,G_TX_RENDERTILE  ,
				&(dynamicp->hilite), 32,32);
		gDPSetRenderMode(glistp++,
			G_RM_ZB_XLU_DECAL  & ~CVG_DST_FULL | CVG_DST_SAVE, 
			G_RM_ZB_XLU_DECAL2 & ~CVG_DST_FULL | CVG_DST_SAVE);
	    }
	    gSPDisplayList(glistp++, geometry);
	    numspec -= 2;
	    nextspec += 2;
	    if (numspec > 0) {
	        gDPSetPrimColor(glistp++, 0, 0,
				(int) (lightmenu[nextspec][LM_RED].val *
				materialmenu[AM_SPEC].val / 256.0),
				(int) (lightmenu[nextspec][LM_GREEN].val *
				materialmenu[AM_SPEC].val / 256.0),
				(int) (lightmenu[nextspec][LM_BLUE].val *
				materialmenu[AM_SPEC].val / 256.0),
				255);
    	        gDPSetEnvColor(glistp++,
				(int) (lightmenu[nextspec+1][LM_RED].val *
				materialmenu[AM_SPEC].val / 256.0),
				(int) (lightmenu[nextspec+1][LM_GREEN].val *
				materialmenu[AM_SPEC].val / 256.0),
				(int) (lightmenu[nextspec+1][LM_BLUE].val *
				materialmenu[AM_SPEC].val / 256.0),
				255);
		guLookAtHilite(&dynamicp->viewing,
                	&(dynamicp->lookat), &(dynamicp->hilite),
                	0.0, 0.0, 500.0,
                	0.0, 0.0,   0.0,
                	0.0, 1.0,   0.0,
                	lightmenu[nextspec][LM_X].val,
                	lightmenu[nextspec][LM_Y].val,
                	lightmenu[nextspec][LM_Z].val,
                	lightmenu[nextspec+1][LM_X].val,
                	lightmenu[nextspec+1][LM_Y].val,
                	lightmenu[nextspec+1][LM_Z].val,
                	32, 32);
	    }
	}
	

	gDPPipeSync(glistp++);
	gDPSetCycleType(glistp++, G_CYC_1CYCLE);

	    /* 
	     * Draw some text 
	     */
	    font_init( &glistp );
	    font_set_transparent( 1 );
    
            font_set_scale( 1.0, 1.0 );
            font_set_win( 200, 1 );

	    FONTCOL(55, 255, 155, 255);
	    SHOWFONT(&glistp,cstring,18,210);


	    /*
	     * MENU
	     */
	    if (menuon) {
	        static float pulse=0, dpulse=0.1;
	        pulse += dpulse;
	        if (pulse>= 1.0) {pulse=1.0; dpulse *= -1; }
	        if (pulse<= 0.0) {pulse=0.0; dpulse *= -1; }
	        movey = 20.0;
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
	 *
	 */
	theadp->t.ucode_boot = (u64 *) rspbootTextStart;
	theadp->t.ucode_boot_size = 
			(u32) rspbootTextEnd - (u32) rspbootTextStart;

	/*
	 * choose which ucode to run:
	 */
	if (rdp_flag) {
	    /*
	     * re-direct output to DRAM: 
	     */
	    theadp->t.ucode = (u64 *) gspF3DEX2_fifoTextStart;
	    theadp->t.ucode_data = (u64 *) gspF3DEX2_fifoDataStart;
            theadp->t.output_buff_size
	      = (u64 *)((int)rdp_output + (int)(RDP_OUTPUT_LEN*sizeof(u64)));
	} else {
	    /*
	     * RSP output over XBUS to RDP: 
	     */
	    theadp->t.ucode = (u64 *) gspF3DEX2_xbusTextStart;
	    theadp->t.ucode_data = (u64 *) gspF3DEX2_xbusDataStart;
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
	 * wait for DP completion 
	 */
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
