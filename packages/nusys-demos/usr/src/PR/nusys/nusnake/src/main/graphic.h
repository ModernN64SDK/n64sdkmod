/*============================================================================
  NuSYSTEM sample program [SNAKE TAIL HACK]
  
  graphic.h
  
  Copyright (C) 1997, NINTENDO Co,Ltd.
  ============================================================================*/

#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_
#include "para.h"

/* The screen size  */
#define SCREEN_HT        240
#define SCREEN_WD        320

/* The maximum length of the display list of one task  */
#define GFX_GLIST_LEN     2048*10

/* The maximum value of the graphic task in the 1 retrace period  */
#define GFX_GTASK_NUM        2


/*-------------------------- define structure ------------------------------ */
/* The projection-matrix structure  */
typedef struct {
  Mtx     projection;
  Mtx     modeling;
  Mtx     viewing;
  
  Mtx     base_trans;
  Mtx     base_rotate;
  Mtx     base_scale;

  Mtx     saku_trans;
  Mtx     saku_rotate;
  Mtx     saku_scale;

  Mtx     obj_trans[OBJ_MAX];
  Mtx     obj_rotate[OBJ_MAX];
  Mtx     obj_scale[OBJ_MAX];

  Mtx     snake_trans[SNAKE_MAX][SNAKE_MAX_LEN+1];
  Mtx     snake_rotate[SNAKE_MAX][SNAKE_MAX_LEN+1];
  Mtx     snake_scale[SNAKE_MAX][SNAKE_MAX_LEN+1];

  Mtx     shot_trans[20];
  Mtx     shot_rotate[20];
  Mtx     shot_scale[20];

  /*The buffer for movie drawing */
  unsigned short MovieBuf[8200];
} Dynamic;

/* Various parameters of the self-machine  */
typedef struct {
    int tailcnt[SNAKE_MAX];	/*The number of tails connected to the self-machine */
    int wallhit[SNAKE_MAX];	/*Whether or not the self-machine is within the range of the map */
    int objhit[SNAKE_MAX];	/*Whether or not the self-machine is within the range of the map */
    int shottailhit[SNAKE_MAX];	/*Whether or not the tail shot by the enemy has hit self */
    int enemyhit[SNAKE_MAX];	/*Whether or not the self-machine has hit the enemy */
    int bufidx[SNAKE_MAX];	/*The index of the buffer which is for that the tail chases the locus of the self-machine */
    int bufidx_wk[SNAKE_MAX];	/*Work for the calculation of ^ */
    int stickon[SNAKE_MAX];	/*Whether or not the control stick is pulled down */
    float head_dir[SNAKE_MAX];	/*The head direction of the self-machine */
    int sleepcnt[SNAKE_MAX];	/*The fainting time (the V-unit)*/

    int enemytailhit[SNAKE_MAX];	/*Whether or not it has hit the opponent's tail */
    int enemytailhitnum[SNAKE_MAX];	/*The opponent's tail number when it has hit the opponent's tail */

    int tail_attr[SNAKE_MAX][SNAKE_MAX_LEN+1];	/*The type of the displaying tail */

    float pos_x[SNAKE_MAX][SNAKE_MAX_LEN+1];	/*The displaying X-coordinate */
    float pos_y[SNAKE_MAX][SNAKE_MAX_LEN+1];	/*The displaying Y-coordinate */
    float pos_z[SNAKE_MAX][SNAKE_MAX_LEN+1];	/*The displaying Z-coordinate */

    float rot_x[SNAKE_MAX][SNAKE_MAX_LEN+1];	/*The rotational angle of the displaying X-axis direction */
    float rot_y[SNAKE_MAX][SNAKE_MAX_LEN+1];	/*The rotational angle of the displaying Y-axis direction */
    float rot_z[SNAKE_MAX][SNAKE_MAX_LEN+1];	/*The rotational angle of the displaying Z-axis direction */

    float size_x[SNAKE_MAX][SNAKE_MAX_LEN+1];	/*The scale of the displaying X-axis direction */
    float size_y[SNAKE_MAX][SNAKE_MAX_LEN+1];	/*The scale of the displaying Y-axis direction */
    float size_z[SNAKE_MAX][SNAKE_MAX_LEN+1];	/*The scale of the displaying Z-axis direction */
} SnakePara;

/* The parameter of the tail for shooting  */
typedef struct{
    float pos_x;	/*The X-coordinate */
    float pos_y;	/*The Y-coordinate */
    float pos_z;	/*The Z-coordinate */
    float rot_x;	/*The rotational angle of the X-axis direction */
    float rot_y;	/*The rotational angle of the Y-axis direction */
    float rot_z;	/*The rotational angle of the Z-axis direction */
    float size_x;
    float size_y;
    float size_z;
    int tail_pattern;
    int user;
    float tail_dir;
    int status;
}ShotTail;

/* The head locus of the self-machine  */
typedef struct{
    float SnakeX[TAIL_MOVE_BUF];
    float SnakeY[TAIL_MOVE_BUF];
    float SnakeRotZ[TAIL_MOVE_BUF];
}SnakeMove;


/*-------------------------------- parameter---------------------------------*/
extern Dynamic gfx_dynamic[];
extern Gfx* glistp;
extern Gfx gfx_glist[GFX_GTASK_NUM][GFX_GLIST_LEN];
extern u32 gfx_gtask_no;
/*-------------------------------- function ---------------------------------*/
extern void gfxRCPInit(void);
extern void gfxClearCfb(void);
/*------------------------------- other extern define -----------------------*/
extern Gfx setup_rdpstate[];
extern Gfx setup_rspstate[];

#endif /* _GRAPHIC_H_ */



