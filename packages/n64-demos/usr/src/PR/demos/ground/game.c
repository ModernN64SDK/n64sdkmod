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
        $Revision: 1.12 $
        $Date: 1999/01/14 10:11:46 $
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
#include "texture.h"

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
	&rdp_output[0],		/* output fifo buffer start */
	&rdp_output[0]+RDP_OUTPUT_LEN,	/* output fifo buffer end */
	NULL,			/* display list pointer (fill in later) */
	0,			/* display list size (ignored) */
	NULL,			/* yield buffer (used if yield will occur) */
	0			/* yield buffer length */
};

/*
 * Viewport
 */
Vp vp = {
	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,  /* scale */
	SCREEN_WD*2, SCREEN_HT*2, G_MAXZ/2, 0,  /* translate */
};

/*
 * global variables
 */
Gfx		*glistp;	/* RSP display list pointer */
Dynamic		dynamic;	/* dynamic data */
int		draw_buffer=0;	/* frame buffer being updated (0 or 1) */
char		*position_str;	/* initialization string */
int		fontcol[4];	/* color for shadowed fonts */

/*
 * macros 
 */
#define PADTHRESH(num,thresh)	((num > thresh) ? num-thresh :		\
                		(num < -thresh) ? num+thresh : 0)
#define ABS(x)			((x>0) ? x : -x)
#define PI			(3.14159)
#if 0
#define FONT_COL		  0,  0,100, 255
#endif
#define FONT_COL		 55,155,255, 255
#define TITLE_COL		  0,140,  0, 255
#define HILITE_COL		150,  0,  0, 255
#define HILITE2_COL		125, 50,  0, 255
#define HILITE3_COL		100,100,  0, 255
#define PULSE_COL 		(1.0-pulse)*255, pulse*255, 0, 255
#define FONTCOL(r,g,b,a)	{fontcol[0]=r; 	\
				fontcol[1]=g;	\
				fontcol[2]=b;	\
				fontcol[3]=a;}
#define FONTCOLM(c)		FONTCOL(c)
#if 0
#define SHOWFONT(glp,str,x,y)	{						\
          	font_set_color( 0,0,0,255);					\
          	font_set_pos( (x)+(1), (y)+(1) );				\
          	font_show_string( glp, str );					\
         	font_set_color( fontcol[0],fontcol[1],fontcol[2],fontcol[3]);	\
          	font_set_pos( x, y );						\
          	font_show_string( glp, str );}
#endif
#define SHOWFONT(glp,str,x,y)	{						\
          	font_set_color( 0,0,0,255);					\
          	font_set_pos( (x)+(1), (y)+(0) );				\
          	font_show_string( glp, str );					\
          	font_set_pos( (x)+(0), (y)+(1) );				\
          	font_show_string( glp, str );					\
         	font_set_color( fontcol[0],fontcol[1],fontcol[2],fontcol[3]);	\
          	font_set_pos( x, y );						\
          	font_show_string( glp, str );}


#define NEAR_MAX 8192.0		/* near plane max value */


/*
 * Menus
 */
enum {
	MT_END, 
	MT_SEPERATOR, 
	MT_MENU, 
	MT_ADDVALI, 
	MT_ADDVALF, 
	MT_MULVALI,
	MT_MULVALF,
	MT_ACTION,
	MT_UPMENU
};
typedef struct MenuItem {
	char		*text;
	int		type;
	struct MenuItem	*nextmenu;
	float		val;
	float		inc,min,max;
	float		*incp, *minp, *maxp;
	u8		color[4];
} MenuItem;

MenuItem dumpmenu[] = {
{"DUMP MENU",MT_SEPERATOR,0,    0.0, 0.0, 0.0, 0.0,    0,0,0,  TITLE_COL},
{"main menu",   MT_UPMENU,   0,    0.0, 0.0, 0.0, 0.0,    0,0,0,       FONT_COL},
{"TRI mask:  ", MT_ADDVALI,  0,    3.0, 1.0, 0.0,  3.0,   0,0,0,         FONT_COL},
{"Dump Position",MT_ACTION,  0,      0,0,0,0,               0,0,0,       FONT_COL},
{"",           MT_END   ,    0,      0,0,0,0,               0,0,0,       FONT_COL},
};
enum {
	DM_TITLE,
	DM_UPMENU,
	DM_TRIMASK,
	DM_POSITION,
	DM_END
};

MenuItem texmenu[] = {
{"TEXTURE MENU", MT_SEPERATOR, 0,    0.0, 0.0, 0.0, 0.0,    0,0,0, TITLE_COL},
{"main menu",    MT_UPMENU,    0,    0.0, 0.0, 0.0, 0.0,    0,0,0, FONT_COL},
{"Texture:  ", MT_ADDVALI,   0,    1.0, 1.0, 1.0,  2.0,     0,0,0, FONT_COL},
{"Tx Scale: ", MT_MULVALI,   0,65535.0, 1.2, 0.0, 65535.0,  0,0,0, FONT_COL},
{"",           MT_END   ,    0,     0,0,0,0,                0,0,0, FONT_COL},
};
enum {
	TM_TITLE,
	TM_UPMENU,
	TM_TEXTURE,
	TM_SCALE,
	TM_END
};

MenuItem mainmenu[] = {
{"MAIN MENU",    MT_SEPERATOR, 0,    0.0, 0.0, 0.0, 0.0,    0,0,0,  TITLE_COL},
{"Texture Menu", MT_MENU,texmenu,    0.0, 0.0, 0.0, 0.0,    0,0,0,  FONT_COL},
{"Dump Menu",    MT_MENU,dumpmenu,   0.0, 0.0, 0.0, 0.0,    0,0,0,  FONT_COL},
{"ClipRatio:  ", MT_ADDVALI,   0,    1.0, 1.0, 1.0, 6.0,    0,0,0,  FONT_COL},
{"near plane: ", MT_MULVALF,   0,   16.0, 2.0, 1.0, 65536.0,0,0,0,  FONT_COL},
{"far plane:  ", MT_MULVALF,   0, 1024.0, 2.0, 1.0, 65536.0,0,0,0,  FONT_COL},
{"view scale: ", MT_MULVALF,   0,    1.0, 2.0, 0.00001, 1.0,0,0,0,  FONT_COL},
{"Eye Mode:   ", MT_ADDVALI,   0,    1.0, 1.0, 1.0, 4.0,    0,0,0,  FONT_COL},
{"Near Clip:  ", MT_ADDVALI,   0,    1.0, 1.0, 0.0, 1.0,    0,0,0,  FONT_COL},
{"",             MT_END   ,    0,     0,0,0,0,              0,0,0,  FONT_COL},
};
enum {
	MM_TITLE,
	MM_TEXMENU,
	MM_DUMPMENU,
	MM_CLIPRATIO,
	MM_NEAR,
	MM_FAR,
	MM_VIEWSCALE,
	MM_EYEMODE,
	MM_NEARCLIP,
	MM_END
};

/*
 * This structure is contains all position and menu info
 */
typedef struct Position {
    	float		pos[3];
    	float		eye[3];
    	float		scalex, scalez;
} Position;
	
Position pp;

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
    int		cmode=0;
    char	str[100];
    float	movex, movey;
    int		firstloop=1;
    int		menuon =1;
    MenuItem	*menu,*mp,*item;
    float	modmat[4][4];
    float	m1[4][4];
    float	m2[4][4];
    float	allmat[4][4];
    unsigned short *loadtex;
    float	balla = -0.02, ballv=0.0, ballp=0.0, ballvi = 0.3;

    

    /*
     * Main game loop
     */
    while (1) {


	pad = ReadController(CONT_G | CONT_A | CONT_START | CONT_L | CONT_UP | 
					CONT_DOWN | CONT_LEFT | CONT_RIGHT);

#define NUMMODES 4
	if ((pad[0]->button & CONT_G) || (pad[0]->button & CONT_A))
		if (++cmode>=NUMMODES) cmode=0;;

	movex = PADTHRESH(pad[0]->stick_x, 10);
	movey = PADTHRESH(pad[0]->stick_y, 10);
	movex *= ABS(movex);
	movey *= ABS(movey);

	if ((pad[0]->button & CONT_START) || firstloop) {
		pp.pos[0] = 0.0;
		pp.pos[1] = 0.0;
		pp.pos[2] = 0.0;
		pp.eye[0] = 20.0;
		pp.eye[1] = 20.0;
		pp.eye[2] = 50.0;
		pp.scalex  = 1.0;
		pp.scalez  = 1.0;
		menu=mainmenu;
		item = &menu[1];
		cmode=0;

		mainmenu[MM_NEAR].maxp = & mainmenu[MM_FAR].val;
		mainmenu[MM_FAR].minp  = & mainmenu[MM_NEAR].val;

		if (firstloop && position_str) {
		    u32 *p;
		    MenuItem *m, *i;
		    int done=0;
		    u32 num;

		    p = (u32 *) &pp;
		    p--;
		    while(*position_str != 'm') {
			if (*position_str == 'x') {
			    p++;
			    *p = 0;
			}
			if (*position_str>= '0' && *position_str<='9') {
			    *p *= 16;
			    *p += *position_str - '0';
			}
			if (*position_str>= 'a' && *position_str<='f') {
			    *p *= 16;
			    *p += *position_str - 'a' + 10;
			}
			position_str++;
		    }


		    m = mainmenu;
		    i = m;
		    while (!done) {
		        while (i->type != MT_END) {
			    if (i->type == MT_MENU) {
			        if (i->nextmenu[1].type == MT_UPMENU) {
				    i->nextmenu[1].nextmenu = m;
				    i->nextmenu[1].val = *((float *) &i);
				    m = i->nextmenu;
				    i = &m[2];
			        }
			    } else {
				num = 0;
				position_str++;
				while (*position_str != 'm') {
				    if (*position_str>= '0' && *position_str<='9') {
			    	        num *= 16;
			    	        num += *position_str - '0';
				    }
				    if (*position_str>= 'a' && *position_str<='f') {
			    	        num *= 16;
			    	        num += *position_str - 'a' + 10;
				    }
				    position_str++;
				}
				*((int *) &i->val) = num;
			        i++;
			    }
		        }
		        if (m[1].type == MT_UPMENU) {
			    i = (MenuItem *) *((int *) &m[1].val);
			    i++;
			    m = m[1].nextmenu;
		        } else done=1;
		    }

		}
		firstloop=0;
	}

	if (pad[0]->button & CONT_L) {
		menuon ^= 1;
	}
	if (menuon) {
		if (item->incp) item->inc = *item->incp;
		if (item->type == MT_MULVALI || item->type == MT_MULVALF) {
			if (item->minp) item->min = *item->minp * item->inc;
			if (item->maxp) item->max = *item->maxp / item->inc;
			if (item==(mainmenu+MM_NEAR))
			  if (item->max>NEAR_MAX) item->max=NEAR_MAX;
		} else if (item->type == MT_ADDVALI || item->type == MT_ADDVALF) {
			if (item->minp) item->min = *item->minp + item->inc;
			if (item->maxp) item->max = *item->maxp - item->inc;
		}
		if (pad[0]->button & CONT_RIGHT) {
			if (item->type == MT_MENU || item->type == MT_UPMENU) {
				if (item->type == MT_MENU) {
					if (item->nextmenu[1].type == MT_UPMENU)
						item->nextmenu[1].nextmenu = menu;
				}
				menu = item->nextmenu;
				item = menu;
				pad[0]->button |= CONT_DOWN;
			} else if (item->type == MT_MULVALI || item->type == MT_MULVALF) {
				item->val = 
					MAX(item->min, 
					MIN(item->max, item->val*item->inc));
			} else if (item->type == MT_ADDVALI || item->type == MT_ADDVALF) {
				item->val = 
					MAX(item->min, 
					MIN(item->max, item->val+item->inc));
			} else if (item->type == MT_ACTION)
				item->val = 1;
		}
		if (pad[0]->button & CONT_LEFT) {
			if (item->type == MT_MENU || item->type == MT_UPMENU) {
				if (item->type == MT_MENU) {
					if (item->nextmenu[1].type == MT_UPMENU)
						item->nextmenu[1].nextmenu = menu;
				}
				menu = item->nextmenu;
				item = menu;
				pad[0]->button |= CONT_DOWN;
			} else if (item->type == MT_MULVALI || item->type == MT_MULVALF) {
				item->val = 
					MAX(item->min, 
					MIN(item->max, item->val/item->inc));
			} else if (item->type == MT_ADDVALI || item->type == MT_ADDVALF) {
				item->val = 
					MAX(item->min, 
					MIN(item->max, item->val-item->inc));
			} else if (item->type == MT_ACTION)
				item->val = 1;
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
				}
				else item--;
			}
		}
	}

	switch (cmode) {
	    case 0:
		pp.scalex += pp.scalex*movex/(80*80*4);
		pp.scalex = MAX(pp.scalex,0.001);
		pp.scalez += pp.scalez*movey/(80*80*4);
		pp.scalez = MAX(pp.scalez,0.001);
		cstring="scale ground";
		break;
	    case 1:
		pp.pos[0] += movex/500.0;
		pp.pos[2] -= movey/500.0;
		if (mainmenu[MM_EYEMODE].val > 2.0) {
			pp.eye[0] -= movex/1000.0;
			pp.eye[2] += movey/1000.0;
		}
		cstring="move ball (X,Z)";
		break;
	    case 2:	{
		float r,x,z;
		r = sqrtf(pp.eye[0]*pp.eye[0] + pp.eye[2]*pp.eye[2]);
		x = pp.eye[0] + pp.eye[2]*movex/10000.0;
		z = pp.eye[2] - pp.eye[0]*movex/10000.0;
		r -= movey/1000.0;
		r = r / sqrtf(x*x + z*z);
		pp.eye[0] = x*r;
		pp.eye[2] = z*r;
		cstring="move eye (A,R)";
		break;	}
	    case 3:	{
		float r,x,z;
		r = sqrtf(pp.eye[0]*pp.eye[0] + pp.eye[2]*pp.eye[2]);
		x = pp.eye[0] + pp.eye[2]*movex/10000.0;
		z = pp.eye[2] - pp.eye[0]*movex/10000.0;
		r = r / sqrtf(x*x + z*z);
		pp.eye[0] = x*r;
		pp.eye[1] += movey/1000.0;
		pp.eye[2] = z*r;
		cstring="move eye (A,Y)";
		break;	}
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
	 * Modify & specify Viewport
	 */
	vp.vp.vscale[0] = SCREEN_WD*2 * mainmenu[MM_VIEWSCALE].val;
	vp.vp.vscale[1] = SCREEN_HT*2 * mainmenu[MM_VIEWSCALE].val;
	gSPViewport(glistp++, &vp);
	if (mainmenu[MM_VIEWSCALE].val < 1.0 ) {
		gDPPipeSync(glistp++);
		gDPSetFillColor(glistp++, GPACK_RGBA5551(0,0,0,1) << 16 |
                     GPACK_RGBA5551(0,0,0,1));
		gDPFillRectangle(glistp++, 
			0, 
			0, 
			SCREEN_WD-1, 
			(SCREEN_HT/2)-(SCREEN_HT/2)*mainmenu[MM_VIEWSCALE].val);
		gDPFillRectangle(glistp++, 
			0, 
			SCREEN_HT-(SCREEN_HT/2)+(SCREEN_HT/2)*mainmenu[MM_VIEWSCALE].val, 
			SCREEN_WD-1, 
			SCREEN_HT-1);
		gDPFillRectangle(glistp++, 
			0, 
			0, 
			(SCREEN_WD/2)-(SCREEN_WD/2)*mainmenu[MM_VIEWSCALE].val,
			SCREEN_HT-1);
		gDPFillRectangle(glistp++, 
			SCREEN_WD-(SCREEN_WD/2)+(SCREEN_WD/2)*mainmenu[MM_VIEWSCALE].val, 
			0, 
			SCREEN_WD-1, 
			SCREEN_HT-1);
	}

	/*
	 * Initialize RCP state.
	 */
	gSPDisplayList(glistp++, init_dl);

	/*
	 * set up matrices
	 */
	guPerspectiveF(allmat, &perspnorm,
		30.0, 320.0/240.0, 
		mainmenu[MM_NEAR].val, mainmenu[MM_FAR].val, 1.0);
	guPerspective(&(dynamicp->projection), &perspnorm,
		30.0, 320.0/240.0, 
		mainmenu[MM_NEAR].val, mainmenu[MM_FAR].val, 1.0);


	if (mainmenu[MM_EYEMODE].val == 1.0 || mainmenu[MM_EYEMODE].val == 3.0) {
	    guLookAtF(m2,
		pp.pos[0]+pp.eye[0], pp.pos[1]+pp.eye[1], pp.pos[2]+pp.eye[2],
		pp.pos[0], pp.pos[1], pp.pos[2],
		0.0, 1.0,   0.0);
	    guLookAt(&(dynamicp->viewing),
		pp.pos[0]+pp.eye[0], pp.pos[1]+pp.eye[1], pp.pos[2]+pp.eye[2],
		pp.pos[0], pp.pos[1], pp.pos[2],
		0.0, 1.0,   0.0);
	} else {
	    guLookAtF(m2,
		pp.pos[0]+pp.eye[0], pp.pos[1]+pp.eye[1], pp.pos[2]+pp.eye[2],
		pp.pos[0], pp.pos[1]+ballp, pp.pos[2],
		0.0, 1.0,   0.0);
	    guLookAt(&(dynamicp->viewing),
		pp.pos[0]+pp.eye[0], pp.pos[1]+pp.eye[1], pp.pos[2]+pp.eye[2],
		pp.pos[0], pp.pos[1]+ballp, pp.pos[2],
		0.0, 1.0,   0.0);
	}

	guMtxCatF(m2, allmat, m1);
	
		
		
	guScale(&(dynamicp->modeling), 
		pp.scalex, 1.0, pp.scalez);
	guScaleF(modmat, 
		pp.scalex, 1.0, pp.scalez);

	guMtxCatF(modmat, m1, allmat);


	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->projection)),
		G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->viewing)),
		G_MTX_PROJECTION|G_MTX_MUL|G_MTX_NOPUSH);
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->modeling)),
		G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);


	gSPPerspNormalize(glistp++, perspnorm);

	switch((int)mainmenu[MM_CLIPRATIO].val) {
		case 1 :
			gSPClipRatio(glistp++, FRUSTRATIO_1);
			break;
		case 2 :
			gSPClipRatio(glistp++, FRUSTRATIO_2);
			break;
		case 3 :
			gSPClipRatio(glistp++, FRUSTRATIO_3);
			break;
		case 4 :
			gSPClipRatio(glistp++, FRUSTRATIO_4);
			break;
		case 5 :
			gSPClipRatio(glistp++, FRUSTRATIO_5);
			break;
		case 6 :
			gSPClipRatio(glistp++, FRUSTRATIO_6);
			break;
	}


	/*
	 *  render triangles
	 */
	gSPDisplayList(glistp++, ground_texture_setup_dl);

	gSPTexture(glistp++, texmenu[TM_SCALE].val, texmenu[TM_SCALE].val, 
				0, G_TX_RENDERTILE, G_ON);
	switch((int)texmenu[TM_TEXTURE].val) {
		case 1:
			loadtex = RGBA16brick;
			break;
		case 2:
			loadtex = RGBA16molecule;
			break;
	}
	gDPLoadTextureBlock(glistp++, loadtex, G_IM_FMT_RGBA, G_IM_SIZ_16b,
                        32, 32, 0,
                         G_TX_WRAP, G_TX_WRAP,
                         5, 5, G_TX_NOLOD, G_TX_NOLOD);

	gSPDisplayList(glistp++, ground_dl);
	if ((int)dumpmenu[DM_TRIMASK].val & 1)
		gSP1Triangle(glistp++, 0, 2, 1, 0);
	if ((int)dumpmenu[DM_TRIMASK].val & 2)
		gSP1Triangle(glistp++, 2, 0, 3, 0);


	/* 
  	 * Draw the Ball and Shadow
	 */
	ballv += balla;
	ballp += ballv;
	if (ballp<1.0) {
		ballp = 1.0;
		ballv = ballvi;
	}


	gDPPipeSync(glistp++);
	guPosition(&(dynamicp->ballmodel),0,0,0,1.0/20.0,pp.pos[0],pp.pos[1]+ballp,pp.pos[2]);
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->ballmodel)),
		  G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
	gSPDisplayList(glistp++, ball_setup_dl);
	gSPDisplayList(glistp++, ball_dl);

	
	guPosition(&(dynamicp->ballshadowmodel),
		   0,0,0,1.0/20.0,pp.pos[0]-ballp/0.8,pp.pos[1],pp.pos[2]);
	guScale(&(dynamicp->ballshadowmodel2),1.0, 0.0, 1.0);
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->ballshadowmodel2)),
		  G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->ballshadowmodel)),
		  G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);
	gSPDisplayList(glistp++, ball_shadow_setup_dl);
	gSPDisplayList(glistp++, ball_dl);


	/* 
	 * Draw some text 
	 */
	{
	  float maxx, maxy, maxz, minx, miny, minz, maxw, minw;
	  font_init( &glistp );
	  font_set_transparent( 1 );

	  font_set_scale( 1.0, 1.0 );
	  font_set_win( 200, 1 );

	  FONTCOLM(FONT_COL);
	  SHOWFONT(&glistp,cstring,20,210);

	  maxx = MAX(MAX(modmat[0][0]*(-50) + modmat[2][0]*(-50) + modmat[3][0],
			 modmat[0][0]*( 50) + modmat[2][0]*( 50) + modmat[3][0]),
		     MAX(modmat[0][0]*(-50) + modmat[2][0]*( 50) + modmat[3][0],
			 modmat[0][0]*( 50) + modmat[2][0]*(-50) + modmat[3][0]));
	  maxy = MAX(MAX(modmat[0][1]*(-50) + modmat[2][1]*(-50) + modmat[3][1],
			 modmat[0][1]*( 50) + modmat[2][1]*( 50) + modmat[3][1]),
		     MAX(modmat[0][1]*(-50) + modmat[2][1]*( 50) + modmat[3][1],
			 modmat[0][1]*( 50) + modmat[2][1]*(-50) + modmat[3][1]));
	  maxz = MAX(MAX(modmat[0][2]*(-50) + modmat[2][2]*(-50) + modmat[3][2],
			 modmat[0][2]*( 50) + modmat[2][2]*( 50) + modmat[3][2]),
		     MAX(modmat[0][2]*(-50) + modmat[2][2]*( 50) + modmat[3][2],
			 modmat[0][2]*( 50) + modmat[2][2]*(-50) + modmat[3][2]));
	  minx = MIN(MIN(modmat[0][0]*(-50) + modmat[2][0]*(-50) + modmat[3][0],
			 modmat[0][0]*( 50) + modmat[2][0]*( 50) + modmat[3][0]),
		     MIN(modmat[0][0]*(-50) + modmat[2][0]*( 50) + modmat[3][0],
			 modmat[0][0]*( 50) + modmat[2][0]*(-50) + modmat[3][0]));
	  miny = MIN(MIN(modmat[0][1]*(-50) + modmat[2][1]*(-50) + modmat[3][1],
			 modmat[0][1]*( 50) + modmat[2][1]*( 50) + modmat[3][1]),
		     MIN(modmat[0][1]*(-50) + modmat[2][1]*( 50) + modmat[3][1],
			 modmat[0][1]*( 50) + modmat[2][1]*(-50) + modmat[3][1]));
	  minz = MIN(MIN(modmat[0][2]*(-50) + modmat[2][2]*(-50) + modmat[3][2],
			 modmat[0][2]*( 50) + modmat[2][2]*( 50) + modmat[3][2]),
		     MIN(modmat[0][2]*(-50) + modmat[2][2]*( 50) + modmat[3][2],
			 modmat[0][2]*( 50) + modmat[2][2]*(-50) + modmat[3][2]));
	  if (maxx >= (65536/2) ||
	      maxy >= (65536/2) ||
	      maxz >= (65536/2) ||
	      minx <= (-65536/2) ||
	      miny <= (-65536/2) ||
	      minz <= (-65536/2) )
	    FONTCOLM(HILITE_COL)
	  else
	    FONTCOLM(FONT_COL)
	      sprintf(str, "WC:(%d,%d,%d)-(%d,%d,%d)",
		      (int)(modmat[0][0]*(-50) + modmat[2][0]*(-50) + modmat[3][0]),
		      (int)(modmat[0][1]*(-50) + modmat[2][1]*(-50) + modmat[3][1]),
		      (int)(modmat[0][2]*(-50) + modmat[2][2]*(-50) + modmat[3][2]),
		      (int)(modmat[0][0]*( 50) + modmat[2][0]*( 50) + modmat[3][0]),
		      (int)(modmat[0][1]*( 50) + modmat[2][1]*( 50) + modmat[3][1]),
		      (int)(modmat[0][2]*( 50) + modmat[2][2]*( 50) + modmat[3][2])
		      );
	  SHOWFONT(&glistp,str,20,139);

	  maxx = MAX(MAX(allmat[0][0]*(-50) + allmat[2][0]*(-50) + allmat[3][0],
			 allmat[0][0]*( 50) + allmat[2][0]*( 50) + allmat[3][0]),
		     MAX(allmat[0][0]*(-50) + allmat[2][0]*( 50) + allmat[3][0],
			 allmat[0][0]*( 50) + allmat[2][0]*(-50) + allmat[3][0]));
	  maxy = MAX(MAX(allmat[0][1]*(-50) + allmat[2][1]*(-50) + allmat[3][1],
			 allmat[0][1]*( 50) + allmat[2][1]*( 50) + allmat[3][1]),
		     MAX(allmat[0][1]*(-50) + allmat[2][1]*( 50) + allmat[3][1],
			 allmat[0][1]*( 50) + allmat[2][1]*(-50) + allmat[3][1]));
	  maxz = MAX(MAX(allmat[0][2]*(-50) + allmat[2][2]*(-50) + allmat[3][2],
			 allmat[0][2]*( 50) + allmat[2][2]*( 50) + allmat[3][2]),
		     MAX(allmat[0][2]*(-50) + allmat[2][2]*( 50) + allmat[3][2],
			 allmat[0][2]*( 50) + allmat[2][2]*(-50) + allmat[3][2]));
	  maxw = MAX(MAX(allmat[0][3]*(-50) + allmat[2][3]*(-50) + allmat[3][3],
			 allmat[0][3]*( 50) + allmat[2][3]*( 50) + allmat[3][3]),
		     MAX(allmat[0][3]*(-50) + allmat[2][3]*( 50) + allmat[3][3],
			 allmat[0][3]*( 50) + allmat[2][3]*(-50) + allmat[3][3]));
	  minx = MIN(MIN(allmat[0][0]*(-50) + allmat[2][0]*(-50) + allmat[3][0],
			 allmat[0][0]*( 50) + allmat[2][0]*( 50) + allmat[3][0]),
		     MIN(allmat[0][0]*(-50) + allmat[2][0]*( 50) + allmat[3][0],
			 allmat[0][0]*( 50) + allmat[2][0]*(-50) + allmat[3][0]));
	  miny = MIN(MIN(allmat[0][1]*(-50) + allmat[2][1]*(-50) + allmat[3][1],
			 allmat[0][1]*( 50) + allmat[2][1]*( 50) + allmat[3][1]),
		     MIN(allmat[0][1]*(-50) + allmat[2][1]*( 50) + allmat[3][1],
			 allmat[0][1]*( 50) + allmat[2][1]*(-50) + allmat[3][1]));
	  minz = MIN(MIN(allmat[0][2]*(-50) + allmat[2][2]*(-50) + allmat[3][2],
			 allmat[0][2]*( 50) + allmat[2][2]*( 50) + allmat[3][2]),
		     MIN(allmat[0][2]*(-50) + allmat[2][2]*( 50) + allmat[3][2],
			 allmat[0][2]*( 50) + allmat[2][2]*(-50) + allmat[3][2]));
	  minw = MIN(MIN(allmat[0][3]*(-50) + allmat[2][3]*(-50) + allmat[3][3],
			 allmat[0][3]*( 50) + allmat[2][3]*( 50) + allmat[3][3]),
		     MIN(allmat[0][3]*(-50) + allmat[2][3]*( 50) + allmat[3][3],
			 allmat[0][3]*( 50) + allmat[2][3]*(-50) + allmat[3][3]));

	  font_set_scale( 1.5, 1.5 );
	  if (	  maxx >= (65536/2) ||
	      maxy >= (65536/2) ||
	      maxz >= (65536/2) ||
	      maxw >= (65536/2) ||
	      minx <= (-65536/2) ||
	      miny <= (-65536/2) ||
	      minz <= (-65536/2) ||
	      minw <= (-65536/2) ) {
	    sprintf(str, "CC=32768 (%s%s%s%s%s%s%s%s)",
		    (maxx >= (65536/2))?"+X":"",
		    (maxy >= (65536/2))?"+Y":"",
		    (maxz >= (65536/2))?"+Z":"",
		    (maxw >= (65536/2))?"+W":"",
		    (minx <= (-65536/2))?"-X":"",
		    (miny <= (-65536/2))?"-Y":"",
		    (minz <= (-65536/2))?"-Z":"",
		    (minw <= (-65536/2))?"-W":"");
	    FONTCOLM(HILITE_COL);
	    SHOWFONT(&glistp,str,25,116);
	  } else if (	ABS(maxx-minx) >= (65536/2) ||
		     ABS(maxy-miny) >= (65536/2) ||
		     ABS(maxz-minz) >= (65536/2) ||
		     ABS(maxw-minw) >= (65536/2) ) {
	    sprintf(str, "CC diff = 32768 (%s%s%s%s)",
		    (ABS(maxx-minx) >= (65536/2))?"X":"",
		    (ABS(maxy-miny) >= (65536/2))?"Y":"",
		    (ABS(maxz-minz) >= (65536/2))?"Z":"",
		    (ABS(maxw-minw) >= (65536/2))?"W":"");
	    FONTCOLM(HILITE2_COL);
	    SHOWFONT(&glistp,str,25,116);
	  } else if (	ABS(maxx-minx) + ABS(maxw-minw) >= (65536/2) ||
		     ABS(maxy-miny) + ABS(maxw-minw) >= (65536/2) ||
		     ABS(maxz-minz) + ABS(maxw-minw) >= (65536/2) ) {
	    sprintf(str, "CC diff+w = 32768 (%s%s%s)",
		    (ABS(maxx-minx) + ABS(maxw-minw) >= (65536/2))?"X":"",
		    (ABS(maxy-miny) + ABS(maxw-minw) >= (65536/2))?"Y":"",
		    (ABS(maxz-minz) + ABS(maxw-minw) >= (65536/2))?"Z":"");
	    FONTCOLM(HILITE3_COL);
	    SHOWFONT(&glistp,str,25,116);
	  } else {
	    FONTCOLM(FONT_COL);
	  }
	  font_set_scale( 1.0, 1.0 );
	  sprintf(str, "CC:(%d,%d,%d,%d)-",
		  (int)(allmat[0][0]*(-50) + allmat[2][0]*(-50) + allmat[3][0]),
		  (int)(allmat[0][1]*(-50) + allmat[2][1]*(-50) + allmat[3][1]),
		  (int)(allmat[0][2]*(-50) + allmat[2][2]*(-50) + allmat[3][2]),
		  (int)(allmat[0][3]*(-50) + allmat[2][3]*(-50) + allmat[3][3])
		  );
	  SHOWFONT(&glistp,str,20,154);
	  
	  sprintf(str, "   (%d,%d,%d,%d)-",
		  (int)(allmat[0][0]*( 50) + allmat[2][0]*(-50) + allmat[3][0]),
		  (int)(allmat[0][1]*( 50) + allmat[2][1]*(-50) + allmat[3][1]),
		  (int)(allmat[0][2]*( 50) + allmat[2][2]*(-50) + allmat[3][2]),
		  (int)(allmat[0][3]*( 50) + allmat[2][3]*(-50) + allmat[3][3])
		  );
	  SHOWFONT(&glistp,str,20,169);
	  
	  sprintf(str, "   (%d,%d,%d,%d)-",
		  (int)(allmat[0][0]*( 50) + allmat[2][0]*( 50) + allmat[3][0]),
		  (int)(allmat[0][1]*( 50) + allmat[2][1]*( 50) + allmat[3][1]),
		  (int)(allmat[0][2]*( 50) + allmat[2][2]*( 50) + allmat[3][2]),
		  (int)(allmat[0][3]*( 50) + allmat[2][3]*( 50) + allmat[3][3])
		  );
	  SHOWFONT(&glistp,str,20,184);
	  
	  sprintf(str, "   (%d,%d,%d,%d)",
		  (int)(allmat[0][0]*(-50) + allmat[2][0]*( 50) + allmat[3][0]),
		  (int)(allmat[0][1]*(-50) + allmat[2][1]*( 50) + allmat[3][1]),
		  (int)(allmat[0][2]*(-50) + allmat[2][2]*( 50) + allmat[3][2]),
		  (int)(allmat[0][3]*(-50) + allmat[2][3]*( 50) + allmat[3][3])
		  );
	  SHOWFONT(&glistp,str,20,199);
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
	    if (mp == item)
	      FONTCOLM(PULSE_COL)
	    else
	      FONTCOL(mp->color[0], mp->color[1], 
		      mp->color[2], mp->color[3])
		if (mp->type == MT_ADDVALI || mp->type == MT_MULVALI)
		  sprintf(str, "%s%d",mp->text, (int) mp->val);
		else if (mp->type == MT_ADDVALF || mp->type == MT_MULVALF)
		  sprintf(str, "%s%.2f",mp->text, mp->val);
		else	
		  sprintf(str, "%s",mp->text );
	    SHOWFONT(&glistp,str,20,movey);
	    movey += 11.0;
	  }
	}

	font_finish( &glistp );


	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);

#ifdef DEBUG
	assert((glistp - dynamicp->glist) < GLIST_LEN);
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
	if (mainmenu[MM_NEARCLIP].val == 1.0) {
	  if (rdp_flag) {
	    /*
	     * RSP output over FIFO to RDP: 
	     */
	    theadp->t.ucode = (u64 *) gspF3DEX2_fifoTextStart;
	    theadp->t.ucode_data = (u64 *) gspF3DEX2_fifoDataStart;
	  } else {
	    /*
	     * RSP output over XBUS to RDP: 
	     */
	    theadp->t.ucode = (u64 *) gspF3DEX2_xbusTextStart;
	    theadp->t.ucode_data = (u64 *) gspF3DEX2_xbusDataStart;
	  }
	} else {
	  if (rdp_flag) {
	    /* 
	     * RSP output over FIFO to RDP: 
	     */
	    theadp->t.ucode = (u64 *) gspF3DEX2_NoN_fifoTextStart;
	    theadp->t.ucode_data = (u64 *) gspF3DEX2_NoN_fifoDataStart;
	  } else {
	    /*
	     * RSP output over XBUS to RDP: 
	     */
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
	 * Dump RSP if requested
	 */

#ifdef DEBUG

	if (dumpmenu[DM_POSITION].val ==1.0) {
		u32 *p;
		MenuItem *m, *i;
		int done=0;
		dumpmenu[DM_POSITION].val =0.0;
		osSyncPrintf("gload -a \"-i ");
		for (p = (u32 *) &pp; p<(u32 *) (&pp +1); p++) {
			osSyncPrintf("x%x",*p);
		}
		m = mainmenu;
		i = m;
		while (!done) {
		    while (i->type != MT_END) {
			if (i->type == MT_MENU) {
			    if (i->nextmenu[1].type == MT_UPMENU) {
				i->nextmenu[1].nextmenu = m;
				i->nextmenu[1].val = *((float *) &i);
				m = i->nextmenu;
				i = &m[2];
			    }
			} else {
			    osSyncPrintf("m%x",*((int *)&i->val));
			    i++;
			}
		    }
		    if (m[1].type == MT_UPMENU) {
			i = (MenuItem *) *((int *) &m[1].val);
			i++;
			m = m[1].nextmenu;
		    } else done=1;
		}
		
		osSyncPrintf("m\"\n");
	}
#endif

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

    }
}
