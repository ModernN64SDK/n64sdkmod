/*============================================================================
  graphic.h
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

/* Graphic micro code  */
#define GFX_GSPCODE_F3DEX    0
#define GFX_GSPCODE_F3DLX    1
#define GFX_GSPCODE_F3DLXREJ 2
#define GFX_GSPCODE_F3DLPREJ 3
#define GFX_GSPCODE_L3DEX    4  /* line ucode */
#define GFX_GSPCODE_LINE3D   5  /* line ucode */

/* The number of graphic task (1 frame) */
#define GFX_GTASK_NUM        8

/* Projection matrix structure  */
typedef struct {
  Mtx     projection;
  Mtx     modeling;
  Mtx     viewing;
  Mtx     line_mtx;
} Dynamic;

/*
   View structure 
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

/* Structure of object coordinate */
typedef struct {
  float x;     /* X∞Ã√÷ */
  float y;     /* Y∞Ã√÷ */
  float z;     /* Z∞Ã√÷ */
  float roll;  /* Z1/4¥≤Û≈3/4 */
  float pitch; /* X1/4¥≤Û≈3/4 */
  float yaw;   /* Y1/4¥≤Û≈3/4 */
  float size;  /* ≥»¬ÁŒ®  */
} CPos;

/* Matrix structure of map (base portion) */
typedef struct {
  Mtx     trans;
  Mtx     scale;
  Mtx     rotate;
}BaseMtx;

/* Graphic task message structure */
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
extern u8 gfx_dlist_buf[GFX_DLIST_BUF_NUM][GFX_DLIST_BUF_SIZE];
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
extern void DrawBg_16b( int, int, int, int, unsigned short * );
extern void DrawBg_16b2( int, int, int, int, u16 * );
extern void DrawWindow( int, int, int, int, int, int, int, int );

/*------------------------------- other extern define -----------------------*/
extern Gfx rdpstateinit_dl[];
extern Gfx setup_rdpstate[];
extern Gfx setup_rspstate[];

#endif /* _GRAPHIC_H_ */
