/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 1

		Copyright (C) 1997, NINTENDO Co., Ltd.

============================================================================*/
#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#define GFX_MESGS_MAX     8
#define GFX_GLIST_LEN     2048

#define GFX_00            00
#define GFX_01            01
#define GFX_10            10
#define GFX_20            20
#define GFX_30            30
#define GFX_NULL          0xffffffff

/* graphic microcode  */
#define GFX_GSPCODE_F3DEX2    0
#define GFX_GSPCODE_L3DEX2    1  /* line ucode */

/* graphic task number (1 frame) */
#define GFX_GTASK_NUM        8


/*-------------------------- define structure ------------------------------ */
/*
The structure defined here is defined for use with the sample.  
Please make additions and changes appropriate to your use.
*/
/* projection matrix  structure*/
typedef struct {
  Mtx     projection;
  Mtx     modeling;
  Mtx     viewing;
  Mtx     line_mtx;
} Dynamic;

/*
 view structure
*/
typedef struct {
  float   eye_x;
  float   eye_y;
  float   eye_z;
  float   at_x;
  float   at_y;
  float   at_z;
  float   up_x;
  float   up_y;
  float   up_z;
  float   pitch;
  float   yaw;
  float   roll;
  float   dist;
} CLookAt;

/* object coordinates structure */
typedef struct {
  float x;     /* X position  */
  float y;     /* Y position  */
  float z;     /* Z position */
  float roll;  /* Z axis of rotation */
  float pitch; /* X axis of rotation */
  float yaw;   /* Y axis of rotation */
  float size;  /* enlargement ratio */
} CPos;

/* model structure */
typedef struct {
  float pos_x;
  float pos_y;
  float pos_z;
  float rot_x;
  float rot_y;
  float rot_z;
  float size;
  Mtx     trans;
  Mtx     scale;
  Mtx     rotate;
} ModelMtx;

/* graphic task message structure */
typedef union {    
    struct {
        short   type;
    } gen;
    struct {
        short   type;
    } done;
    NNScMsg   app;
} GFXMsg;

/*-------------------------------- parameter---------------------------------*/
extern u32 graphic_no;
extern Dynamic gfx_dynamic[];
extern OSMesgQueue *sched_gfxMQ;
extern OSMesgQueue gfx_msgQ;
extern Gfx* glist_ptr;
extern u32 gfx_cfbdrawbuffer;
extern u8* gfxdlist_buffer_ptr;
extern NNScTask   gfx_task[];
extern Gfx gfx_dlist_buf[GFX_DLIST_BUF_NUM][GFX_DLIST_BUF_SIZE];
extern Gfx gfx_glist[GFX_GTASK_NUM][GFX_GLIST_LEN];
extern u32 gfx_gtask_no;
extern CLookAt lookat;
extern u32     pendingGFX ;
/*-------------------------------- function ---------------------------------*/
extern void gfxInit(u8*);
extern void gfxCreateGraphicThread(NNSched*);
extern short gfxWaitMessage(void);
extern void gfxTaskStart(NNScTask*, Gfx*, s32, u32, u32);
extern void gfxPositionF(float mf[4][4], CPos* );
extern void gfxPosition(Mtx* , CPos*);
extern void gfxTranslateF(float mf[4][4], CPos*);
extern void gfxTranslate(Mtx* , CPos*);
extern void gfxRCPInit(void);
extern void gfxClearCfb(void);

/*------------------------------- other extern define -----------------------*/
extern Gfx rdpstateinit_dl[];
extern Gfx setup_rdpstate[];
extern Gfx setup_rspstate[];

#endif /* _GRAPHIC_H_ */




