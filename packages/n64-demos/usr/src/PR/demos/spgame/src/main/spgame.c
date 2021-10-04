/**************************************************************************
 *                                                                        *
 *               Copyright (C) 1994, Silicon Graphics, Inc.               *
 *                                                                        *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright  law.  They  may not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *                                                                        *
 *************************************************************************/

/*
 * File:	sptext.c
 * Creator:	rww@sgi.com
 * Create Date:	Mon Apr 17 11:45:57 PDT 1995
 *
 * VERY simple app, draws a bunch of text.
 *
 */

#include <ultra64.h>
#include <PR/os.h>
#include "main.h"

void do_bkg( Gfx ** );
void do_game( Gfx ** );
void do_time( Gfx **, int, int, int, int );

void
sp_do_work( Gfx **pglistp )
{
    static int count = 0;
    extern Sprite lit_sprite;
    Gfx *gp;
    static OSTime prev;
    OSTime tmp;
    static int ticks = 0;
    extern int rsp_ticks;
    extern int rdp_ticks;
    int frm_ticks;

    if( ticks > 0 )
	frm_ticks = (tmp = osGetTime()) - prev;
    else {
	frm_ticks = 0;
	tmp = osGetTime();
    }

    prev = tmp;

    gp = *pglistp;

    spInit( &gp );			/* Initialize Sprites */

    do_bkg( &gp );

    do_game( &gp );   /* */

    do_time( &gp, ticks, rsp_ticks, rdp_ticks, frm_ticks );   /* */

    spFinish( &gp );		

    gp--;

    *pglistp = gp;

    count ++;

    ticks = osGetTime() - prev;
}

void
do_bkg( Gfx **pglistp )
{
    Gfx *gp, *dl;
    extern Sprite lit_sprite;

    gp = *pglistp;

    lit_sprite.rsp_dl_next   = lit_sprite.rsp_dl;

    spMove( &lit_sprite, 0,0 );
    spScale( &lit_sprite, 1.0, 1.0 );
    spSetAttribute( &lit_sprite, SP_FASTCOPY );

    dl = spDraw( &lit_sprite );

    gSPDisplayList(gp++, dl );

    *pglistp = gp;
}


#include "sp_ball.h"

extern Sprite ball_sprite;

typedef struct state_struct {
    Sprite *sp;		/* Sprite   */

    double size;	/* Radius   */

    double  s,vs;	/* Scaling */

    double  x, y;	/* Position */
    double vx,vy;	/* Velocity */

    double  e,ve;	/* Explosion */

    double  a,va;	/* Alpha */
} Sobj;

Sobj ball = {

	&ball_sprite,

	16,

	1.6, 0.00,

	10, 0,
	 -1,  -2,
	
	0, 0,		/* Don't explode yet! */

	255, 0,		/* Don't fade yet! */
    };

#define NUM_BALLS	(7)

Sobj balls[NUM_BALLS];

#define NUM_balls_GFX (NUM_DL(NUM_ball_BMS))

static Gfx balls_gfx[NUM_BALLS][NUM_balls_GFX];

void
init_game( void )
{
    int i;

    for( i=0; i<NUM_BALLS; i++ ) {
	balls[i] = ball;
	balls[i].x  += (200.0* i)/NUM_BALLS;
	balls[i].vx += (2.0  * i)/NUM_BALLS;
	balls[i].y  += (100.0*i)/NUM_BALLS;
	balls[i].vy += (4.0* i)/NUM_BALLS;

	balls[i].s  -= (0.5*i)/NUM_BALLS;
	balls[i].vs += (0.05* i+0.02)/NUM_BALLS;
/*	
*/
    };

}

void draw_sobj( Gfx **, Sobj * );
void move_sobj( Sobj *, int );

void
do_game( Gfx **pglistp )
{
    Gfx *gp;
    static int first = 1;
    int i;

    if( first ) {
	first = 0;
	init_game();
    };

    gp = *pglistp;

    for( i=0; i<NUM_BALLS; i++ ) {
	balls[i].sp->rsp_dl = &(balls_gfx[i][0]);
        spSetAttribute( balls[i].sp, SP_TRANSPARENT | SP_CUTOUT );
	draw_sobj( &gp, &(balls[i]) );
	move_sobj( &(balls[i]), i );
    };


    *pglistp = gp;
}

void
draw_sobj( Gfx **pglistp, Sobj *s )
{
    Sprite *sp;
    Gfx *gp, *dl;

    gp = *pglistp;

    sp = s->sp;

    sp->rsp_dl_next   = sp->rsp_dl;

    spMove( sp, (int)(s->x),(int)(s->y) );
    if( s->a == 255 )
	spScale( sp, 1.0/s->s, 1.0/s->s );
    else
	spScale( sp, s->s, s->s );

    sp->expx = (int) s->e;
    sp->expy = (int) s->e;
    sp->alpha = (int) s->a;

    dl = spDraw( sp );

    gSPDisplayList(gp++, dl );

    *pglistp = gp;
}

#define ACCEL (9.81/60)

void
move_sobj( Sobj *s , int i)
{

    s->s += s->vs;

    if( s->a == 255 )
	if( ((s->s < 0.5)       && (s->vs < 0)) ||
	    ((s->s > 5.0) && (s->vs > 0)) )
	    s->vs = -s->vs;

    s->x += s->vx;

    if( ((s->x < 20)       && (s->vx < 0)) ||
        ((s->x > (300-64)) && (s->vx > 0)) )
	s->vx = -s->vx;

    s->y += s->vy;

    if( ((s->y < 20)       && (s->vy < 0)) ||
        ((s->y > (220-64)) && (s->vy > 0)) )
	s->vy = -s->vy;

    s->vy += ACCEL;

    if( (s->y > (220-64)) && ( s->vy > -2 ) ) {	/* Jump! */
	s->vy -= 5;
	s->s   = 1.0/s->s;

	s->va = -1;
	s->ve = 1/8.0;
	s->vs = -s->s/270;
    };

    s->a += s->va;
    s->e += s->ve;

/*    s->s += s->vs; */

    if( (s->a <= 0) || (s->s <= 0) ) {	/* Explosion is over */
	s->ve = 0;
	s->va = 0;
	s->vs = ball.vs + (0.05* i + 0.03)/NUM_BALLS;

	s->e  = 0;
	s->s  = 1.6;
	s->a  = 255;
    };
}

#ifdef WALLS

void
check_walls( void)
{
    int i;
    double dt = 1.0/60;
    double tcur = cur->t;

    for(i=0; i<num_walls; i++) {
	if( collision( walls[i], cur ) ) {
	    i = (-1);
	    update_force( cur );

	};
    };
}

#endif /* WALLS */

#include "font_ext.h"


#define	TIK_SCALE	(50.223)
#define	TIK2USEC(x)	((int)(x/TIK_SCALE))

void
do_time( Gfx **pglistp, int cpu_ticks, int rsp_ticks, int rdp_ticks, int frm_ticks )
{
    Gfx *gp;
    static int count = 0;
    char str[80];

    gp = *pglistp;

    font_init( &gp );

    font_set_transparent( 1 );

    font_set_pos( 180, 200 );
    font_set_color( 55, 255, 155, 255 );
    font_set_scale( 1.0, 1.0 );
    font_set_win( 20, 1 );

    sprintf( str, "time %02d:%02d:%02d", count/3600, (count/60)%60, count%60 );
    font_show_string( &gp, str );

    font_set_pos( 150, 40 );
    font_set_color( 0, 0, 0, 255 );
    sprintf( str, " dl time %d usec", TIK2USEC(cpu_ticks) );
    font_show_string( &gp, str );

    font_set_pos( 150, 60 );
    sprintf( str, "rsp time %d usec", TIK2USEC(rsp_ticks) );
    font_show_string( &gp, str );

    font_set_pos( 150, 80 );
    sprintf( str, "rdp time %d usec", TIK2USEC(rdp_ticks) );
    font_show_string( &gp, str );

    font_set_pos( 150, 100 );
    sprintf( str, "frm time %d usec", TIK2USEC(frm_ticks) );
    font_show_string( &gp, str );

    font_finish( &gp );

    *pglistp = gp;

    count++;
}

