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
        $Revision: 1.10 $
        $Date: 1998/12/24 15:09:57 $
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
	&rdp_output[0],		/* output fifo buffer start ptr */
	&rdp_output[0]+RDP_OUTPUT_LEN, /* output fifo buffer end ptr */
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

/*
 * macros 
 */
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
                font_set_color( 0,0,0,255);                                    \
                font_set_pos( (x)+(1), (y)+(0) );                              \
                font_show_string( glp, str );                                  \
                font_set_pos( (x)+(0), (y)+(1) );                              \
                font_show_string( glp, str );                                  \
                font_set_color( fontcol[0],fontcol[1],fontcol[2],fontcol[3]);  \
                font_set_pos( x, y );                                          \
                font_show_string( glp, str );}



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

MenuItem mainmenu[] = {
	{"MAIN MENU",          MT_SEPERATOR, 0, 0.0, 0.0,   0.0,  0.0,       
	0,   25,   0, 0xff},
	{"Texture         : ", MT_ADDVALI,   0, 0.0, 1.0,   0.0,  4.0,       
	55, 255, 155, 0xff},
	{"Control Rotation: ", MT_ADDVALI,   0, 0.0, 1.0,   0.0,  1.0,       
	55, 255, 155, 0xff},
	{"Rotation rate a : ", MT_ADDVALF,   0, 0.4, 0.1, -10.0, 10.0,     
	55, 255, 155, 0xff},
	{"Rotation rate b : ", MT_ADDVALF,   0, 0.1, 0.1, -10.0, 10.0,     
	55, 255, 155, 0xff},
	{"Texture Scale   : ", MT_MULVALF,   0, 0.4, 1.2,   1.0/65536.0, 1.0,
	55, 255, 155, 0xff},
	{"Smallest mip pic: ", MT_MULVALI,   0, 1.0, 2.0,   1.0, 32.0,
	55, 255, 155, 0xff},
	{"clamp lvl small(0) or big(1):",
			       MT_ADDVALI,   0, 0.0, 1.0,   0.0,  1.0,
	55, 255, 155, 0xff},
	{"",                   MT_END,       0,   0,   0,     0,    0,              
	55, 255, 155, 0xff},
};
enum {
	MM_TITLE,
	MM_TEXTURE,
	MM_ROTON,
	MM_ROTA,
	MM_ROTB,
	MM_TXSCALE,
	MM_MIPSMALLPIC,
	MM_SPARSE,
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
    float	rota, rotb;
    float	pos[3];
    float	scale;
    int		cmode=0;
    char 	*cstring;
    char	str[100];
    float	movex, movey;
    int		firstloop=1;
    int		menuon =0;
    MenuItem	*menu,*mp,*item;
    Gfx		*usetex;
    Gfx		*useMM;
    Gfx		*useHMM;
    int		miplevels;
    int		bottmem;
    u16		txscale;
    

    /*
     * Main game loop
     */
#ifdef DEBUG
    osSyncPrintf("Use the L button and crosshair for menu options.\n");
#endif

    while (1) {



	pad = ReadController(CONT_G | CONT_A | CONT_START | CONT_L | CONT_UP | 
					CONT_DOWN | CONT_LEFT | CONT_RIGHT);

#define NUMMODES 4
	if (pad[0]->button & CONT_G) if (++cmode>=NUMMODES) cmode=0;;

	movex = PADTHRESH(pad[0]->stick_x, 10);
	movey = PADTHRESH(pad[0]->stick_y, 10);
	movex *= ABS(movex);
	movey *= ABS(movey);

	if ((pad[0]->button & CONT_A) || (pad[0]->button & CONT_START) || firstloop) {
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
					MAX(item->max, 
					MIN(item->min, item->val*item->inc));
			} else if (item->type == MT_ADDVALI || item->type == MT_ADDVALF) {
				item->val = 
					MAX(item->max, 
					MIN(item->min, item->val+item->inc));
			}
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
					MAX(item->max, 
					MIN(item->min, item->val/item->inc));
			} else if (item->type == MT_ADDVALI || item->type == MT_ADDVALF) {
				item->val = 
					MAX(item->max, 
					MIN(item->min, item->val-item->inc));
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
				}
				else item--;
			}
		}
	}

	if ((int)mainmenu[MM_ROTON].val==0) {
		if (cmode==0) cmode=1;
		rota += mainmenu[MM_ROTA].val;
		rotb += mainmenu[MM_ROTB].val;
	}
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
		30.0, 320.0/240.0, 
		10, 1000, 1.0);

	guLookAt(&(dynamicp->viewing), 
		0.0, 0.0, 500.0,
		0.0, 0.0,   0.0,
		0.0, 1.0,   0.0);
		
		
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


	for (miplevels = 0; (1<<miplevels) < mainmenu[MM_MIPSMALLPIC].val;
		miplevels++);
	/* 
	 * choose texture
	 */
	switch ((int) mainmenu[MM_TEXTURE].val) {
	  case 0:
		usetex = load_tex_chainlink_dl;
		useMM = RGBA16chainlinkMM_dl;
		useHMM = RGBA16chainlinkHMM_dl;
		miplevels = MAX(0,4 - miplevels);
		bottmem = 86;
		break;
	  case 1:
		usetex = load_tex_brick_dl;
		useMM = RGBA16brickMM_dl;
		useHMM = RGBA16brickHMM_dl;
		miplevels = MAX(0,5 - miplevels);
		bottmem = 342;
		break;
	  case 2:
		usetex = load_tex_checker_dl;
		useMM = RGBA16checkerMM_dl;
		useHMM = RGBA16checkerHMM_dl;
		miplevels = MAX(0,5 - miplevels);
		bottmem = 342;
		break;
	  case 3:
		usetex = load_tex_molecule_dl;
		useMM = RGBA16moleculeMM_dl;
		useHMM = RGBA16moleculeHMM_dl;
		miplevels = MAX(0,5 - miplevels);
		bottmem = 342;
		break;
	  case 4:
		usetex = load_tex_mymip_dl;
		useMM = RGBA16mymipMM_dl;
		useHMM = RGBA16mymipHMM_dl;
		miplevels = MAX(0,4 - miplevels);
		bottmem = 86;
		break;
	}

	txscale = (int)(mainmenu[MM_TXSCALE].val * 65536.0);
	if (mainmenu[MM_TXSCALE].val >= 1.0)
		txscale = 65535;

	/*
	 * View 1 - Point Sampled
	 */
	gSPDisplayList(glistp++, view_1_dl);
	gSPDisplayList(glistp++, cube_texture_setup_dl);
	gDPSetTextureFilter(glistp++, G_TF_POINT);
	gSPDisplayList(glistp++, usetex);
	gSPTexture(glistp++, txscale, txscale,
		0,G_TX_RENDERTILE, G_ON);
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->modeling)),
		G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
	gSPDisplayList(glistp++, cube_dl);
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->modeling2)),
		G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
	gSPDisplayList(glistp++, runway_dl);

	/*
	 * View 2 - Bilinear (no mip map)
	 */
	gSPDisplayList(glistp++, view_2_dl);
	gSPDisplayList(glistp++, cube_texture_setup_dl);
	gDPSetTextureFilter(glistp++, G_TF_BILERP);
	gSPDisplayList(glistp++, usetex);
	gSPTexture(glistp++, txscale, txscale,
		0,G_TX_RENDERTILE, G_ON);
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->modeling)),
		G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
	gSPDisplayList(glistp++, cube_dl);
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->modeling2)),
		G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
	gSPDisplayList(glistp++, runway_dl);

	/*
	 * View 3 - trilinear mip mapping
	 */
	gSPDisplayList(glistp++, view_3_dl);
	gSPDisplayList(glistp++, cube_textureMM3_setup_dl);
	gSPDisplayList(glistp++, useMM);
	gSPTexture(glistp++, txscale, txscale,
		miplevels,G_TX_RENDERTILE, G_ON);
	if (mainmenu[MM_SPARSE].val == 0.0) {
	    gDPSetTile(glistp++, 0, 2, 1, bottmem, miplevels,
					0, 0, 0, 5, 0, 0, 5);
            gDPSetTileSize(glistp++, miplevels,2, 2,0,0);
        }
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->modeling)),
		G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
	gSPDisplayList(glistp++, cube_dl);
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->modeling2)),
		G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
	gSPDisplayList(glistp++, runway_dl);

	/*
	 * View 4 - trilinear hand generated mip mapping
	 */
	gSPDisplayList(glistp++, view_4_dl);
	gSPDisplayList(glistp++, cube_textureMM3_setup_dl);
	gSPDisplayList(glistp++, useHMM);
	gSPTexture(glistp++, txscale, txscale,
		miplevels,G_TX_RENDERTILE, G_ON);
	if (mainmenu[MM_SPARSE].val == 0.0) {
	    gDPSetTile(glistp++, 0, 2, 1, bottmem, miplevels,
					0, 0, 0, 5, 0, 0, 5);
            gDPSetTileSize(glistp++, miplevels,2, 2,0,0);
        }
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->modeling)),
		G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
	gSPDisplayList(glistp++, cube_dl);
	gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->modeling2)),
		G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
	gSPDisplayList(glistp++, runway_dl);


	/* 
	 * done with views
	 */
	gSPDisplayList(glistp++, view_default_dl);

	/* edges of 4 tests */
	gSPDisplayList(glistp++, border_dl);



	/* 
	 * Draw some text 
	 */
	font_init( &glistp );
	font_set_transparent( 1 );

        font_set_scale( 1.0, 1.0 );
        font_set_win( 200, 1 );

	FONTCOL(55, 255, 155, 255);
	SHOWFONT(&glistp,cstring,18,210);

	if (!menuon) {				/* view labels */
	    FONTCOL(100, 100, 0, 255 );
	    SHOWFONT(&glistp,"Point Sampled",18,20);
	    SHOWFONT(&glistp,"Bilinear",163,20);
	    SHOWFONT(&glistp,"RGB2C Mip Map",18,125);
	    SHOWFONT(&glistp,"Hand Gen Mip Map",163,125);
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
		if (mp->type == MT_ADDVALI || mp->type == MT_MULVALI)
			sprintf(str, "%s%d",mp->text, (int) mp->val);
		else if (mp->type == MT_ADDVALF || mp->type == MT_MULVALF)
			sprintf(str, "%s%.2f",mp->text, mp->val);
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
