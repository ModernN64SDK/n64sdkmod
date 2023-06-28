/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 1

		Copyright (C) 1997, NINTENDO Co., Ltd.

============================================================================*/

#ifndef _MAIN01_H_
#define _MAIN01_H_

/* Fiter status flag */
#define FITER_FLAG_START        1       /* Move into screen from outside at start */
#define FITER_FLAG_PLAYER       2       /* Player has control */

#define FITER_BULLET_NUM        8       /* Number of bullets fired at same time */

/* Bullet status */
#define FITER_BULLET_FLAG_NONE  0       /* empty */
#define FITER_BULLET_FLAG_FIRE  1       /* fire */

/* Bullet parameter structure */
typedef struct{
  CPos  pos;            /* coordinates*/
  Mtx   model_trans;    /* model matrix */
  Mtx   model_rotate;   /* model matrix */
  Mtx   model_size;     /* model matrix */
  u8    flag;           /* status flag */
} CBullet;


/* machine parameter structure */
typedef struct{
  CPos    pos;           /* coordinates */
  Mtx     model_rotate;  /* model rotation */
  Mtx     model_trans;   /* model transfer */
  Mtx     model_size;    /* size (scale) */
  u32     flag;          /* status flag */
  CBullet bullet[FITER_BULLET_NUM];  /* bullet structure */
  u8      bullet_cnt;    /* counter for bullet firing position */
  Gfx*    dlist_ptr;     /* display list pointer */
  Gfx*    dlist_buf;     /* display list buffer */
} CFiter;

#endif /* _MAIN01_H_ */



