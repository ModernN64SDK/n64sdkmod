/*============================================================================
  NuSYSTEM sample program [SNAKE TAIL HACK]

  hitchk.c
  
  Copyright (C) 1997, NINTENDO Co,Ltd.
  ============================================================================*/

#include <assert.h>
#include <nusys.h>

#include "graphic.h"
/*--------------------------
  extern
  --------------------------*/
extern SnakePara Snake;
extern float ObjPosTbl[OBJ_MAX][3];
extern ShotTail mtx_shot[];

/*--------------------------------------*/
/*  Collision detection between the head and wall 		*/
/*  IN:	PadNum The player number 	*/
/*  RET:None 				*/
/*--------------------------------------*/
void
Snake2Wall(int PadNum)
{
    float SnakeX,SnakeY;

    /* Get the position of the self-machine  */
    SnakeX = Snake.pos_x[PadNum][0];
    SnakeY = Snake.pos_y[PadNum][0];

    if(-BASE_MAX_SIZE+50 <= SnakeX && BASE_MAX_SIZE-50 >= SnakeX &&
       -BASE_MAX_SIZE+50 <= SnakeY && BASE_MAX_SIZE-50 >= SnakeY){
	/* If it is inside of the map  */
	Snake.wallhit[PadNum] = 0;
    }else{
	/* If it is outside of the map  */
	Snake.wallhit[PadNum] = 1;
    }
}


/*----------------------------------------------*/
/*  Collision detection between the head and map object 	*/
/*  IN:	PadNum The player number 		*/
/*  RET:None 					*/
/*----------------------------------------------*/
void
Snake2Obj(int PadNum)
{
    int idx;
    float SnakeX,SnakeY;
    float ObjX,ObjY;

    /* Get the position of the self-machine  */
    SnakeX = Snake.pos_x[PadNum][0];
    SnakeY = Snake.pos_y[PadNum][0];

    for(idx = 0; idx < OBJ_MAX-1; idx++){
	/* Get the position of the map object  */
	ObjX = ObjPosTbl[idx][0];
	ObjY = ObjPosTbl[idx][1];

	if( ObjX - OBJ_SIZE*OBJ_SCALE <= SnakeX && ObjX + OBJ_SIZE*OBJ_SCALE >= SnakeX &&
	   ObjY - OBJ_SIZE*OBJ_SCALE <= SnakeY && ObjY + OBJ_SIZE*OBJ_SCALE >= SnakeY){
	    /* If it hits the object, retern the object number  */
	    Snake.objhit[PadNum] = idx;
	    break;
	}else{
	    /* If it doesn't hit the object  */
	    Snake.objhit[PadNum] = -1;
	}
    }
}

/*-----------------------------------------------------------------------------
  Collision detection between the head and enemy 
  -----------------------------------------------------------------------------*/
void
Snake2Enemy(int PadNum)
{
    int idx;
    float SnakeX,SnakeY;
    float EnemyX,EnemyY;

    /* Get the position of the self-machine  */
    SnakeX = Snake.pos_x[PadNum][0];
    SnakeY = Snake.pos_y[PadNum][0];

    for(idx = 0; idx < 4; idx++){
	if(idx == PadNum){
	    continue;
	}

	/* Get the position of enemy  */
	EnemyX = Snake.pos_x[idx][0];
	EnemyY = Snake.pos_y[idx][0];
	
	if( EnemyX - OBJ_SIZE*OBJ_SCALE <= SnakeX && EnemyX + OBJ_SIZE*OBJ_SCALE >= SnakeX &&
	   EnemyY - OBJ_SIZE*OBJ_SCALE <= SnakeY && EnemyY + OBJ_SIZE*OBJ_SCALE >= SnakeY){
	    /* If it hits enemy, return the enemy's number  */
	    Snake.enemyhit[PadNum] = idx;
	    break;
	}else{
	    /* If it hasn't hit the enemy  */
	    Snake.enemyhit[PadNum] = -1;
	}
    }
}


/*-----------------------------------------------------------------------------
  Collision detection between the shot tail and the wall 
  -----------------------------------------------------------------------------*/
int
ShotTail2Wall( float SnakeX, float SnakeY )
{
    if(-BASE_MAX_SIZE+50 <= SnakeX && BASE_MAX_SIZE-50 >= SnakeX &&
       -BASE_MAX_SIZE+50 <= SnakeY && BASE_MAX_SIZE-50 >= SnakeY){
	/* If it is inside of the map  */
	return(0);
    }else{
	/* If it is outside of the map  */
	return(1);
    }
}


/*-----------------------------------------------------------------------------
  Collision detection between the shot tail and the obstacle 
  -----------------------------------------------------------------------------*/
int
ShotTail2Obj( float SnakeX, float SnakeY)
{
    int idx;
    float ObjX,ObjY;

    for(idx = 0; idx < OBJ_MAX-1; idx++){
	ObjX = ObjPosTbl[idx][0];
	ObjY = ObjPosTbl[idx][1];
	if( ObjX - OBJ_SIZE*OBJ_SCALE <= SnakeX && ObjX + OBJ_SIZE*OBJ_SCALE >= SnakeX &&
	   ObjY - OBJ_SIZE*OBJ_SCALE <= SnakeY && ObjY + OBJ_SIZE*OBJ_SCALE >= SnakeY){
	    /* If it hits the object, return the object's number  */
	    return(idx);
	}
    }
    /* If it doesn't hit the object  */
    return(-1);
}


/*-----------------------------------------------------------------------------
  Collision detection between the self-machine and the shot tail 
  -----------------------------------------------------------------------------*/
void
Snake2ShotTail(int PadNum)
{
    int idx;
    float SnakeX,SnakeY;
    float TailX,TailY;

    /* Get the position of the self-machine  */
    SnakeX = Snake.pos_x[PadNum][0];
    SnakeY = Snake.pos_y[PadNum][0];
    
    for(idx = 0; idx < 20; idx++){
	if(mtx_shot[idx].tail_pattern == -1){
	    continue;
	}
	TailX = mtx_shot[idx].pos_x;
	TailY = mtx_shot[idx].pos_y;
    
	if( TailX - TAIL_SIZE <= SnakeX && TailX + TAIL_SIZE >= SnakeX &&
	   TailY - TAIL_SIZE <= SnakeY && TailY + TAIL_SIZE >= SnakeY){
	    if(mtx_shot[idx].status == TAIL_WAITING){
		Snake.shottailhit[PadNum] = idx;
		return;
	    }
	    if(mtx_shot[idx].status == TAIL_MOVING && mtx_shot[idx].user != PadNum){
		Snake.sleepcnt[PadNum] = MAX_SLEEP;
	    }
	    
	}
    }
    Snake.shottailhit[PadNum] = -1;
}

/*-----------------------------------------------------------------------------
  Collision detection between the self-machine and the enemy's tail 
  -----------------------------------------------------------------------------*/
void
Snake2EnemyTail(int PadNum)
{
    int idx, idx2;
    float SnakeX,SnakeY;
    float TailX,TailY;

    /* Get the position of the self-machine  */
    SnakeX = Snake.pos_x[PadNum][0];
    SnakeY = Snake.pos_y[PadNum][0];

    for(idx = 0; idx < 4; idx++){
	if(idx == PadNum)
	  continue;
	/*Check for connecting tails */
	for(idx2 = 1; idx2 < Snake.tailcnt[idx]+1; idx2++){
	    if(Snake.tail_attr[idx][idx2] == -1){
		break;
	    }
	    TailX = Snake.pos_x[idx][idx2];
	    TailY = Snake.pos_y[idx][idx2];
    
	    if( TailX - TAIL_SIZE <= SnakeX && TailX + TAIL_SIZE >= SnakeX &&
	       TailY - TAIL_SIZE <= SnakeY && TailY + TAIL_SIZE >= SnakeY){
		Snake.enemytailhit[PadNum] = idx;
		Snake.enemytailhitnum[PadNum] = idx2;
		return;
	    }
	}
    }
    Snake.enemytailhit[PadNum] = -1;
    Snake.enemytailhitnum[PadNum] = -1;
}
