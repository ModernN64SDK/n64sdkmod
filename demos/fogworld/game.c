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
        $Revision: 1.9 $
        $Date: 1999/04/16 07:11:47 $
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
	&rdp_output[0]+RDP_OUTPUT_LEN,	/* fifo output buffer end */
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

Lights2		light = gdSPDefLights2(16,16,16,		/* ambient light */
				107,26,107,	64,0,10,	/* light 1 */
				107,107,26,	-64,-64,10);	/* light 2 */
				
/*
 * macros 
 */
#define STATIC2VIRTUAL(x)	(((u32)(&x) & 0x00ffffff) + (u32)staticSegment)
#define MAXLIGHTS 7
#define PADTHRESH(num,thresh)	((num > thresh) ? num-thresh :		\
                		(num < -thresh) ? num+thresh : 0)
#define ABS(x)			((x>0) ? x : -x)
#define SGN(x)			((x>0) ? 1 : -1)
#define PI			3.14159
#define FONTCOL(r,g,b,a)        {fontcol[0]=r;  \
                                fontcol[1]=g;   \
                                fontcol[2]=b;   \
                                fontcol[3]=a;}
#define FONTCOLM(c)             FONTCOL(c)
#define SHOWFONT(glp,str,x,y)   {                                             \
                font_set_color( 0,0,0,255);                                   \
                font_set_pos( (x)+(1), (y)+(0) );                             \
                font_show_string( glp, str );                                 \
                font_set_pos( (x)+(0), (y)+(1) );                             \
                font_show_string( glp, str );                                 \
                font_set_color( fontcol[0],fontcol[1],fontcol[2],fontcol[3]); \
                font_set_pos( x, y );                                         \
                font_show_string( glp, str );}

/*
 * Help 
 */
typedef struct Help {
	int x,y,incy;
	char **words;
} Help;

char *help0[] = {
	"Demonstration of fog and culling.",
	"Use the joystick to steer around.",
	"Use these buttons:",
	"R       stop.",
	"E       accelerate.",
	"D       decelerate.",
	"START   Reset position and speed.",
	"L       Turn menu on/off.",
	"+ UP    choose previous menu item.",
	"+ DOWN  choose next menu item.",
	"+ LEFT  Decrement menu itme value.",
	"+ RIGHT Increment menu itme value.",
	" ",
	"Additional help is available in",
	"the Help Menu.",
	""
};
char *help1[] = {
	"This demo illistrates the use of",
	"Fog and Volume Culling.",
	"Fog can be used (as in this",
	"demo) to hide distant sections of",
	"a visual database as they come into",
	"view and dissappear from view.",
	"See FOG HELP for info on",
	"using fog.",
	"Volume Culling is used to quickly",
	"eliminate sections of the visual",
	"database which are out of view. See",
	"CULL HELP for info on volume",
	"Culling.",
	"Turn fog on and off to see its",
	"visual benefits",
	"",
};
char *help2[] = {
	"Turn fog on and off in the main menu.",
	"The color of the fog (which is also",
	"the color of the background) can be",
	"adjusted in the Fog Menu, as can the",
	"min and max values.  The max value",
	"indicates where fog will completely",
	"obscure everything.  It ranges from",
	"0 (=at the near plane) to 1000 (=at",
	"the far plane).  The min value has",
	"the same range and indicates where",
	"the fog will begin to appear.  The",
	"min and max values are nonlinear so",
	"some tweaking may be neccessary to",
	"get the desired results. See the",
	"gSPFogPosition man page for info.",
	"",
};
char *help3[] = {
	"Volume culling is a way to improve",
	"performance without using CPU power.",
	"The display list sent to the RSP",
	"includes a gSPVertex command",
	"describing the volume of the",
	"geometry which the display list is",
	"about to draw.  Then the",
	"gSPCullDisplayList command",
	"skips drawing that geometry if the",
	"bounding volume is outside the view.",
	"Volume culling uses the RSP to",
	"decide what to cull so the CPU",
	"doesn't have to spend so much time",
	"on that task.  See the",
	"gSPCullDisplayList man page for info",
	"",
};
char *help4[] = {
	"The Viewing Menu has a number of",
	"modes you can adjust plus a way of",
	"dumping the display list.  To dump",
	"the display list which gets sent",
	"to the RSP select DUMP RSP and press",
	"the + RIGHT button.  Use DUMP",
	"RDP to see what gets sent to the",
	"RDP.  Text is not included in the",
	"dump.  This program draws 9 copies",
	"of the terrain: the copy the eye is",
	"over, and the copies all around that",
	"one.  To simplify a dump you can",
	"turn MULTIPLE GEOMETRY off and only",
	"1 copy will be drawn. Bumper and",
	"Minalt are for collision detection",
	"",
};
char *help5[] = {
	"The indicators at the bottom of",
	"the screen show you:",
	"fr    frame rate (notice how much",
	"      faster it is with the menu",
	"      turned off.",
	"a,d   altitude and direction.",
	"p,s   position and speed.",
	" ",
	"Another feature is Wireframe.",
	"In the Viewing Menu choose",
	"Wireframe to see the geometry of",
	"the terrain with the line",
	"microcode.  This is implemented by",
	"simply using the regular (triangle)",
	"display list with the line microcode",
	"",
};

/*	"THis is the longest string to useEND",          */

Help help[] = {
	{23,30,13,(char **)help0},
	{23,30,13,(char **)help1},
	{23,30,13,(char **)help2},
	{23,30,13,(char **)help3},
	{23,30,13,(char **)help4},
	{23,30,13,(char **)help5},
};

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
	MT_FADVALF, 
	MT_ADDVALS,
	MT_FMLVALI,
	MT_MULVALI,
	MT_MULVALF,
	MT_FMLVALF,
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
char *fmStr[] = {
	"off","normal","transparent"
};
char *bStr[] = {
	"","",
};
char *rtStr[] = {
	"automatic","joystick",
};
char *lmStr[] = {
	"","","","","","","","","","","","","","","","",""
};

MenuItem helpmenu[] = {
	{"HELP MENU",          MT_SEPERATOR,      0, 0,0,0,0,
	0,   25,   0, 0xff},
        {"Main menu",          MT_UPMENU,    0,0,0,0,0,
        5, 255, 155, 0xff},
        {"Overview",           MT_ADDVALS,(MenuItem *) bStr, 1,1,-1,1,
        55, 255, 155, 0xff},
        {"Fog Help",           MT_ADDVALS,(MenuItem *) bStr, 1,1,-1,1,
        55, 255, 155, 0xff},
        {"Cull Help",           MT_ADDVALS,(MenuItem *) bStr, 1,1,-1,1,
        55, 255, 155, 0xff},
        {"Other Help",           MT_ADDVALS,(MenuItem *) bStr, 1,1,-1,1,
        55, 255, 155, 0xff},
        {"Misc Help",           MT_ADDVALS,(MenuItem *) bStr, 1,1,-1,1,
        55, 255, 155, 0xff},
        {"",                   MT_END,       0,0,0,0,0,
        55, 255, 155, 0xff},
};
enum {
	HM_TITLE,
	HM_MAINMENU,
	HM_OVERVIEW,
	HM_FOG,
	HM_CULL,
	HM_OTHER,
	HM_MISC,
	HM_END
};

MenuItem viewmenu[] = {
	{"View MENU",          MT_SEPERATOR, 0,0,0,0,0,
	0,   25,   0, 0xff},
	{"Main menu",	       MT_UPMENU,    0,0,0,0,0,
	5, 255, 155, 0xff},
	{"Near Clipping : ",MT_ADDVALS,(MenuItem *) onStr, 1,1,0,1,
	55, 255, 155, 0xff},
	{"Field of View : ",   MT_ADDVALF,   0, 37.0,1.0,1.0,120.0,       
	55, 255, 155, 0xff},
	{"Near Plane    : ",   MT_FMLVALI,   0,  20,1.05,1,1000,
	55, 255, 155, 0xff},
	{"Far Plane     : ",   MT_FMLVALI,   0, 1024,1.05,1,10000,
	55, 255, 155, 0xff},
	{"Multiple geometry:", MT_ADDVALS,(MenuItem *) onStr, 1,1,0,1,
	55, 255, 155, 0xff},
	{"bumper        : ",   MT_FADVALI,   0, 20,1,1,500,       
	55, 255, 155, 0xff},
	{"bumper2       : ",   MT_FADVALI,   0, 100,1,1,500,       
	55, 255, 155, 0xff},
	{"minalt        : ",   MT_FADVALI,   0, 10,1,1,500,       
	55, 255, 155, 0xff},
	{"Wireframe     : ", MT_ADDVALS,(MenuItem *) onStr, 0,1,0,1,
	55, 255, 155, 0xff},
	{"",                   MT_END,       0,0,0,0,0,
	55, 255, 155, 0xff},
};
enum {
	VM_TITLE,
	VM_MAINMENU,
	VM_NEARCLIP,
	VM_FOV,
	VM_NEAR,
	VM_FAR,
	VM_MULTI,
	VM_BUMPER,
	VM_BUMPER2,
	VM_MINALT,
	VM_WIRE,
	VM_END
};

MenuItem fogmenu[] = {
        {"FOG MENU",           MT_SEPERATOR, 0,0,0,0,0,
        0,   25,   0, 0xff},
        {"Main menu",          MT_UPMENU,    0,0,0,0,0,
        5, 255, 155, 0xff},
        {"min   : ",      MT_FADVALI,   0, 996,   1,     0,  1000,
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
        FM_MIN,
        FM_MAX,
        FM_RED,
        FM_GREEN,
        FM_BLUE,
        FM_END
};

MenuItem mainmenu[] = {
	{"MAIN MENU",          MT_SEPERATOR,      0, 0,0,0,0,
	0,   25,   0, 0xff},
        {"Help Menu",          MT_MENU,     helpmenu, 0,0,0,0,
        55, 255, 155, 0xff},
        {"Fog Mode : ",        MT_ADDVALS,(MenuItem *) onStr, 1,1,0,1,
        55, 255, 155, 0xff},
        {"Fog Menu",           MT_MENU,     fogmenu, 0,0,0,0,
        55, 255, 155, 0xff},
	{"Viewing Menu",       MT_MENU,     viewmenu, 0,0,0,0,
	55, 255, 155, 0xff},
	{"",                   MT_END,            0, 0,0,0,0,
	55, 255, 155, 0xff},
};
enum {
	MM_TITLE,
	MM_HELP,
        MM_FOGMODE,
	MM_FOGMENU,
	MM_VIEWMENU,
	MM_END
};

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
    char 	*cstring;
    char	str[100];
    float	movex, movey;
    int		firstloop=2;
    int		menuon =0;
    MenuItem	*menu=0,*mp,*item;
    int		oldbut;
    float	eyex,eyey,eyez;
    float	atx,aty,atz;
    float	upx,upy,upz;
    float	roll, pitch, yaw, speed;
    float	alta;
    int		dimx, dimz;
    int 	onground=0;
    int		rate=1;
    int		helpnote=1;
    float	oldmode;
    
    /*
     * setup for terrain
     */
     dimx = *((int *) STATIC2VIRTUAL(terrain0_dimx));
     dimz = *((int *) STATIC2VIRTUAL(terrain0_dimz));

    /*
     * fog mode
     */
    oldmode = mainmenu[MM_FOGMODE].val;

    /*
     * Main game loop
     */
    /*osSyncPrintf("Use the L button and crosshair for menu options.\n");*/
    oldbut = 0;
    while (1) {

	pad = ReadController(0);

	movex = PADTHRESH(pad[0]->stick_x, 10);
	movey = PADTHRESH(pad[0]->stick_y, 10);
	movex *= ABS(movex);
	movey *= ABS(movey);
	
	if (movex>0 || movey>0 || pad[0]->button) helpnote = 0;

	if ((pad[0]->button & CONT_START  & ~oldbut) || 
	    firstloop) {
		if (firstloop) firstloop--;
		menu=mainmenu;
		item = &menu[1];
		roll = 0.0;
		pitch = 0.0;
		yaw = 90*PI/180;
		speed = 7.0;

		eyex = -160;
		eyey = 37;
		eyez = 0;
		atx = eyex;
		aty = eyey;
		atz = eyez-50.0;
		upx = 0.0;
		upy = 1.0;
		upz = 0.0;
	}

	if (pad[0]->button & CONT_L & ~oldbut) {
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
		    oldbut &= ~CONT_DOWN;
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
	      if (item->type == MT_FADVALI ||
		  item->type == MT_FADVALF) {
		    item->val =
			MAX(item->max,
			MIN(item->min, item->val+item->inc));
	      } else if (item->type == MT_FMLVALI || 
			 item->type == MT_FMLVALF) {
		    item->val = 
			MAX(item->max, 
			MIN(item->min, item->val*item->inc));
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
		    oldbut &= ~CONT_DOWN;
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
	      if (item->type == MT_FADVALI ||
                  item->type == MT_FADVALF) {
		    item->val =
			MAX(item->max,
			MIN(item->min, item->val-item->inc));
	      } else if (item->type == MT_FMLVALI || 
                         item->type == MT_FMLVALF) {
		    item->val =
                        MAX(item->max,
                        MIN(item->min, item->val/item->inc));
              }
	    }
	    if (pad[0]->button & CONT_B & ~oldbut) {
	        if (item->type == MT_MULVALI ||
		    item->type == MT_MULVALF ||
		    item->type == MT_FMLVALF ||
		    item->type == MT_FMLVALI ||
		    item->type == MT_ADDVALI ||
		    item->type == MT_ADDVALF ||
                    item->type == MT_ADDVALS ||
		    item->type == MT_FADVALF ||
		    item->type == MT_FADVALI) {
		    if (item->val == item->min)
			item->val = item->max;
		    else if (item->val == item->max)
			item->val = (item->max+item->min)/2;
		    else
			item->val = item->min;
		}
	    }
	    if (pad[0]->button & CONT_DOWN & ~oldbut) {
		item++;
		while (item->type <= MT_SEPERATOR) {
		    if (item->type == MT_SEPERATOR) item++;
		    else item = menu;
		}
	    }
	    if (pad[0]->button & CONT_UP & ~oldbut) {
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
	 * Post menu processing
	 */
	if (oldmode != mainmenu[MM_FOGMODE].val && mainmenu[MM_FOGMODE].val != 0) {
		float tmp;
		oldmode = mainmenu[MM_FOGMODE].val;
		tmp = fogmenu[FM_MIN].val;
		fogmenu[FM_MIN].val = fogmenu[FM_MAX].val;
		fogmenu[FM_MAX].val = tmp;
	}

	/*
	 * let the joystick control the view
	 */
	if (pad[0]->button & CONT_E) speed +=1.0;
	else if (pad[0]->button & CONT_D) speed -= 1.0;
	if (pad[0]->button & CONT_R) speed=0;

	pitch = -movey*PI/(6*6400);
	roll = movex*PI/(2*6400);
	if (pitch>0) onground=0;


	yaw += movex*PI/(6400*40);
	if (yaw>2*PI) yaw-=2*PI;
	if (yaw<  0) yaw+=2*PI;

	aty = sinf(pitch);
	atx = cosf(yaw)*cosf(pitch);
	atz = sinf(yaw)*cosf(pitch);

	eyex += atx*speed;
	eyey += aty*speed;
	eyez += atz*speed;
	if (speed == 0) eyey += aty;

	if (eyex>512) {
	    eyex-=1024;
	}
	if (eyez>512) {
	    eyez-=1024;
	}
	if (eyex<-512) {
	    eyex+=1024;
	}
	if (eyez<-512) {
	    eyez+=1024;
	}



	/* 
	 * Follow the terrain's altitude
	 */
	{
	    int   bumpi,bumpj,i,j,ii,jj,indx,indz;
	    float tx,ty,tz,dx,dz,d2,d;
    
	    alta = -1000;
	    bumpi = viewmenu[VM_BUMPER2].val * dimx/1024.0 +1;
	    bumpj = viewmenu[VM_BUMPER2].val * dimz/1024.0 +1;
	    indx = (int) ((eyex+512) * dimx/1024.0);
	    indz = (int) ((eyez+512) * dimz/1024.0);
    
    
    
	    for (i=-bumpi; i<=bumpi; i++) {
	        ii = (i+indx)%dimx;
	    for (j=-bumpj; j<=bumpj; j++) {
	        jj = (j+indz)%dimz;
	        tx = ((Vtx *) STATIC2VIRTUAL(terrain0_vtx))
                                    [ii * dimz + jj].n.ob[0];
	        ty = ((Vtx *) STATIC2VIRTUAL(terrain0_vtx))
                                    [ii * dimz + jj].n.ob[1];
	        tz = ((Vtx *) STATIC2VIRTUAL(terrain0_vtx))
                                    [ii * dimz + jj].n.ob[2];
	        dx = tx-eyex;
	        dz = tz-eyez;
	        d2 = dx*dx + dz*dz;
	        if (d2<viewmenu[VM_BUMPER].val*viewmenu[VM_BUMPER].val) {
		    alta = MAX(alta,ty);
	        } else if (d2<viewmenu[VM_BUMPER2].val*viewmenu[VM_BUMPER2].val) {
		    d=sqrtf(d2);
		    d = 1.0 - (d-viewmenu[VM_BUMPER].val)/
			    (viewmenu[VM_BUMPER2].val-viewmenu[VM_BUMPER].val);
		    alta = MAX(alta, ty * d);
	        }
    
	    }
	    }
	    alta += viewmenu[VM_MINALT].val;
	}


	if (onground || eyey<alta) {
	    eyey=alta;
	    onground=1;
	}
	

	aty = eyey + 100*sinf(pitch);
	atx = eyex + 100*cosf(yaw)*cosf(pitch);
	atz = eyez + 100*sinf(yaw)*cosf(pitch);

	upx = -10 * sinf(roll) * sinf(yaw) * cosf(pitch);
	upy =  10 * cosf(roll);
	upz =  10 * sinf(roll) * cosf(yaw) * cosf(pitch);

	sprintf(str,"fr=%d a=%d d=%d p(%d,%d) s%d",
				60/rate,
				(int)eyey, (int)(yaw*180/PI), 
				(int)eyex,(int)eyez, (int)speed);
	cstring=str;


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

	gDPSetFillColor(glistp++, GPACK_RGBA5551(
			(int)fogmenu[FM_RED].val,
			(int)fogmenu[FM_GREEN].val,
			(int)fogmenu[FM_BLUE].val,
			1) << 16 |
			GPACK_RGBA5551(
			(int)fogmenu[FM_RED].val,
			(int)fogmenu[FM_GREEN].val,
			(int)fogmenu[FM_BLUE].val,
			1));
	gSPDisplayList(glistp++, clearcfb_dl);

	/*
	 * Initialize RCP state.
	 */
	gSPDisplayList(glistp++, init_dl);

	/*
	 * set up matrices
	 */
	if (viewmenu[VM_NEAR].val >= viewmenu[VM_FAR].val) 
		viewmenu[VM_FAR].val = viewmenu[VM_NEAR].val+1.0;
	guPerspective(&(dynamicp->projection), &perspnorm,
		viewmenu[VM_FOV].val,
		320.0/240.0, 
		viewmenu[VM_NEAR].val,
		viewmenu[VM_FAR].val,
		1.0);

	guLookAt(&dynamicp->viewing,
		eyex,eyey,eyez,
		atx,aty,atz,
		upx,upy,upz);
		
	guMtxIdent(&(dynamicp->modeling));

	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->projection)),
		G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->viewing)),
		G_MTX_PROJECTION|G_MTX_MUL|G_MTX_NOPUSH);
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->modeling)),
		G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);


	gSPPerspNormalize(glistp++, perspnorm);


	gSPSetLights2(glistp++, light);


	if (mainmenu[MM_FOGMODE].val>0.0) {
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

	if (viewmenu[VM_WIRE].val == 1.0) {
	    switch((int)mainmenu[MM_FOGMODE].val) {
	      case 0:
	        gSPDisplayList(glistp++, setrend_line);	
	        break;
	      case 1:
	        gSPDisplayList(glistp++, setrend_line);	
	        break;
	    }
	} else {
	    switch((int)mainmenu[MM_FOGMODE].val) {
	      case 0:
	        gSPDisplayList(glistp++, setrend_light);	
	        break;
	      case 1:
	        gSPDisplayList(glistp++, setrend_light_fog);	
	        break;
	    }
	}

	if (viewmenu[VM_MULTI].val == 1.0) {
	    gSPDisplayList(glistp++, terrain_dl);
	} else {
	    gSPDisplayList(glistp++, terrain0_dl);
	}
	

	gDPPipeSync(glistp++);
	gDPSetCycleType(glistp++, G_CYC_1CYCLE);

	/*
	 * Draw Text
	 */
	{
	  int dohelp=-1,helpi=1;
	  MenuItem *helpitem;

	  /* 
	   * neccessary after lines
	   */
	  gDPSetScissor(glistp++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT);

	  font_init( &glistp );
	  font_set_transparent( 1 );
    
	  font_set_scale( 1.0, 1.0 );
	  font_set_win( 200, 1 );

	  for (helpitem=&helpmenu[2];helpitem->type!=MT_END; helpitem++) {
	    if (helpitem->val != 0.0) {
	      if (!(pad[0]->button & (CONT_RIGHT | CONT_LEFT)))
		helpitem->val =0.0;
	      dohelp=helpi;
	    }
	    helpi++;
	  }
	  if (pad[0]->button & CONT_A) dohelp=0;

	  if (dohelp!=-1) {
	    /* 
	     * Draw help screen
	     */
	    char	**helpstring;
	    int	x,y;

	    x=help[dohelp].x;
	    y=help[dohelp].y;
	    FONTCOL(55, 255, 155, 255);
	    for (helpstring=help[dohelp].words; helpstring[0][0]!='\0'; helpstring++) {
	      SHOWFONT(&glistp,helpstring[0],x,y);
	      y+=help[dohelp].incy;
	    }
	  } else {

	    FONTCOL(55, 255, 155, 255);
	    SHOWFONT(&glistp,cstring,18,210);
    
	    if (helpnote) {
	      static int time=0;
	      if (++time>25) {
		FONTCOL(255, 110, 110, 255);
		SHOWFONT(&glistp,"Press the A button for help",18,120);
	      }
	      if (time>50) time=0;
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
	      for (mp = menu; mp->type != MT_END; mp++) {
		if (mp == item) {
		  FONTCOL(255, pulse*255, 0, 255);
		} else {
		  FONTCOL(mp->color[0], mp->color[1], 
			  mp->color[2], mp->color[3]);
		}
		if (mp->type == MT_ADDVALI || 
		    mp->type == MT_MULVALI ||
		    mp->type == MT_FMLVALI ||
		    mp->type == MT_FADVALI)
		  sprintf(str, "%s%d",mp->text, (int) mp->val);
		else if (mp->type == MT_ADDVALF || 
			 mp->type == MT_FADVALF ||
			 mp->type == MT_FMLVALF ||
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
	     * RSP output over FIFO to RDP: 
	     */
	    if (viewmenu[VM_WIRE].val == 1.0) {
	        theadp->t.ucode = (u64 *) gspL3DEX2_fifoTextStart;
	        theadp->t.ucode_data = (u64 *) gspL3DEX2_fifoDataStart;
	    } else if (viewmenu[VM_NEARCLIP].val == 1.0) {
	        theadp->t.ucode = (u64 *) gspF3DEX2_fifoTextStart;
	        theadp->t.ucode_data = (u64 *) gspF3DEX2_fifoDataStart;
	    } else {
	        theadp->t.ucode = (u64 *) gspF3DEX2_NoN_fifoTextStart;
	        theadp->t.ucode_data = (u64 *) gspF3DEX2_NoN_fifoDataStart;
	    }
	} else {
	    /*
	     * RSP output over XBUS to RDP: 
	     */
	    if (viewmenu[VM_WIRE].val == 1.0) {
	        theadp->t.ucode = (u64 *) gspL3DEX2_xbusTextStart;
	        theadp->t.ucode_data = (u64 *) gspL3DEX2_xbusDataStart;
	    } else if (viewmenu[VM_NEARCLIP].val == 1.0) {
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
	(void)osRecvMesg(&rdpMessageQ, NULL, OS_MESG_BLOCK);

	/*
	 * setup to swap buffers 
	 */
	osViSwapBuffer(cfb[draw_buffer]);

	/*
	 * Make sure there isn't an old retrace in queue 
	 * (assumes queue has a depth of 1) 
	 */
	rate=1;
	while (!MQ_IS_EMPTY(&retraceMessageQ)) {
	    rate++;
	    (void) osRecvMesg(&retraceMessageQ, NULL, OS_MESG_NOBLOCK);
	}

	/*
	 * Wait for Vertical retrace to finish swap buffers 
	 */
	(void) osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);

	draw_buffer ^= 1;
	oldbut=pad[0]->button;

    }
}
