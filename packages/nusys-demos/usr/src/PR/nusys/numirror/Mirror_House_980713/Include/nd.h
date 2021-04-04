/*======================================================================*/
/*		NIFFprev						*/
/*		nd.h							*/
/*									*/
/*		Copyright (C) 1997,1998, NINTENDO Co,Ltd.		*/
/*									*/
/*======================================================================*/
#ifndef __ND_H
#define __ND_H

/* ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	The simple 3D manager ND

	Header
 :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

/* ---------------------------------------------------------------------------
	constant values
	reference types
 --------------------------------------------------------------------------- */

/* NDC stands for ND Constant value */
/* NDR stands for ND Resource type */

#ifndef __ND_COMMONS__
#define __ND_COMMONS__

/* ------------------ macros */

#define SEGTOP(sno) ((u32)(sno)<<24)
#define SEGADRS(sno,adrs) (SEGTOP(sno)|((u32)(adrs)&0x00ffffff))

/* ------------------ deform flag */

enum{
	/* Flags used for deform */
	NDC_DEFORM_DIRECTION		= 0x01,
	NDC_DEFORM_STOP				= 0x02,
	NDC_DEFORM_DIRECTION_TRIG	= 0x10000,
	NDC_DEFORM_END				= 0x20000,
	NDC_DEFORM_ON				= 0x40000
};

/* ------------------ geometric animation */

enum{
  /* NDR_ANIM_CHANNEL.kind */
  NDC_ANIMKIND_TRS_X  = 0x00,
  NDC_ANIMKIND_TRS_Y  = 0x01,
  NDC_ANIMKIND_TRS_Z  = 0x02,
  NDC_ANIMKIND_ROT_X  = 0x03,
  NDC_ANIMKIND_ROT_Y  = 0x04,
  NDC_ANIMKIND_ROT_Z  = 0x05,
  NDC_ANIMKIND_SCL_X  = 0x06,
  NDC_ANIMKIND_SCL_Y  = 0x07,
  NDC_ANIMKIND_SCL_Z  = 0x08
};

typedef struct {
  u16 kind;
  s16 start;
  s16 end;
  s16 nkey;
  s16 *key;
  float *value;
} NDR_ANIM_CHANNEL;

typedef struct {
  NDR_ANIM_CHANNEL *chlist;
  u16 nch;
  u16 total;
} NDR_ANIM;

/* ------------------ shape */

typedef struct{
  s16 *key;
  s16 *tblno;
  u16 nkey;
  u16 nelem;
} NDR_DEFORM;

typedef struct{
  Gfx *gfx;
  Vtx *vtx;
  u16 nvtx;
  u16 vtxsegno;

  /* using texture segment */
  u8 *img;
  u16 *tlut;
  u16 imgsegno, tlutsegno;

  /* shape deform data link */
  s16 *shaperef;
  NDR_DEFORM *vtx_deform;
  NDR_DEFORM *vtxcol_deform;
  NDR_DEFORM *vtxnv_deform;

} NDR_SHAPE;

/* ------------------ object */

enum{
  /* object types */
  NDC_TYPE_NULL = 0x01,
  NDC_TYPE_3D = 0x02,
  NDC_TYPE_BILLBOARD = 0x03,

  /* ColorCombine Mode */
  NDC_CC_COMBINED = 0x00,
  NDC_CC_TEX0 = 0x01,
  NDC_CC_TEX1 = 0x02,
  NDC_CC_PRIMITIVE = 0x03,
  NDC_CC_SHADE = 0x04,
  NDC_CC_ENV = 0x05,
  NDC_CC_KEY_CENTER = 0x10,
  NDC_CC_KEY_SCALE = 0x11,
  NDC_CC_COMBINED_ALPHA = 0x06,
  NDC_CC_TEX0_ALPHA = 0x07,
  NDC_CC_TEX1_ALPHA = 0x08,
  NDC_CC_PRIMITIVE_ALPHA = 0x09,
  NDC_CC_SHADE_ALPHA = 0x0a,
  NDC_CC_ENV_ALPHA = 0x0b,
  NDC_CC_LOD = 0x0c,
  NDC_CC_PRIM_LOD = 0x0d,
  NDC_CC_NOISE = 0x12,
  NDC_CC_K4 = 0x14,
  NDC_CC_K5 = 0x15,
  NDC_CC_1 = 0x0e,
  NDC_CC_0 = 0x0f,

  /* Render Mode(complexed) */
  NDC_RM_AA_EN                     = 0x80000000,
  NDC_RM_Z_CMP                     = 0x40000000,
  NDC_RM_Z_UPD                     = 0x20000000,
  NDC_RM_IM_RD                     = 0x10000000,
  NDC_RM_CVG_DST                   = 0x0c000000,
    NDC_RM_CVG_DST_CLAMP           = 0x00000000,
    NDC_RM_CVG_DST_WRAP            = 0x04000000,
    NDC_RM_CVG_DST_FULL            = 0x08000000,
    NDC_RM_CVG_DST_SAVE            = 0x0c000000,
  NDC_RM_CLR_ON_CVG                = 0x02000000,
  NDC_RM_CVG_X_ALPHA               = 0x01000000,
  NDC_RM_ALPHA_CVG_SEL             = 0x00800000,
  NDC_RM_FORCE_BL                  = 0x00400000,
  NDC_RM_ZMODE                     = 0x00300000,
    NDC_RM_ZMODE_OPA               = 0x00000000,
    NDC_RM_ZMODE_INTER             = 0x00100000,
    NDC_RM_ZMODE_XLU               = 0x00200000,
    NDC_RM_ZMODE_DEC               = 0x00300000,
  NDC_RM_ALPHA_COMPARE             = 0x000c0000,
    NDC_RM_ALPHA_COMPARE_NONE      = 0x00000000,
    NDC_RM_ALPHA_COMPARE_THRESHOLD = 0x00040000,
    NDC_RM_ALPHA_COMPARE_DITHER    = 0x00080000,
  NDC_RM_DITHER_ALPHA              = 0x00030000,
    NDC_RM_DITHER_ALPHA_PATTERN    = 0x00000000,
    NDC_RM_DITHER_ALPHA_NOPATTERN  = 0x00010000,
    NDC_RM_DITHER_ALPHA_NOISE      = 0x00020000,
    NDC_RM_DITHER_ALPHA_DISABLE    = 0x00030000,
  NDC_RM_BLENDER_MUX0              = 0x0000ff00,
  NDC_RM_BLENDER_MUX1              = 0x000000ff,

  /* NDR_OBJ.regflag(bit assign) */
  NDC_REGF_2CYC        = 0x4000
};

typedef struct{
  float tx, ty, tz;
  float rx, ry, rz;
  float sx, sy, sz;
} TRX;

typedef struct ndr_objTag{

  /* object propaties */
  u16 type, gid;
  u32 prio;
  TRX trx;

  /* resourse index */
  s16 shape;

  /* RSP, RDP status register setting */
  u16 regflag;
  u32 geommode;
  u32 rdmode;
  u32 primcol;
  u8 cc1[8], cc2[8];

  /* geometric animation setting */
  s16 geom_anim;
  u16 loop;
  u16 framerate;
  u32 order;

  /* children links */
  s16 bb_obj[2];
  u16 nchild;
  u16 *child;

} NDR_OBJ;

/* ------------------ scene */

typedef struct{
  TRX trx;
  u16 objno;
} NDR_SCN_INST_OBJ;

typedef struct{
  int type;
  u32 color;
  float x, y, z;
  float a1,a2;
} NDR_SCN_LIGHT;

typedef struct{
  struct{
    u16 near, far;
    u8 r,g,b,a;
  }fog;
  struct{
    u32 mode;
    float fovy, aspect, scale;
    u16 top, bottom, left, right, near, far;
    u16 eye_inst, upper_inst, lookat_inst;
  }cam;
  struct{
    u32 amb_color;
    int nlight;
    NDR_SCN_LIGHT *light;
  }light;
  u16 fill_col, fill_depth;
  NDR_SCN_INST_OBJ *entryobj;
  u16 nentobj;
} NDR_SCENE;

typedef struct{
  NDR_SCENE *scene;
  float convert_scale;

  NDR_OBJ **obj;
  NDR_SHAPE **shape;
  NDR_ANIM **anim;
  s16 **rawvtx;
  u32 **rawvtxcol;
  s8 **rawvtxnv;

  u16 nobj;
  u16 nshape;
  u16 nanim;
  u16 nrawvtx;
  u16 nrawvtxcol;
  u16 nrawvtxnv;
} NDR_RESINFO;

#endif    /* #ifndef __ND_COMMONS__ */

/* ---------------------------------------------------------------------------
	dynamic types
 --------------------------------------------------------------------------- */

enum{
    NDC_ITYPE_NULL = 0,
    NDC_ITYPE_ROOT = 1,
    NDC_ITYPE_OBJECT = 2,
    NDC_ITYPE_ANIMTK = 3,
    NDC_ITYPE_ANIM = 4,
    NDC_ITYPE_DYNVTX = 5,
    NDC_ITYPE_DEFORM_VTX = 6,
    NDC_ITYPE_DEFORM_VTXCOL = 7,
    NDC_ITYPE_DEFORM_VTXNV = 8,

    NDC_IPRIO_SYSTEM_BOTTOM = 0x0000,
    NDC_IPRIO_OBJEND = 0x2000,
    NDC_IPRIO_OBJ = 0x3000,
    NDC_IPRIO_OBJROOT = 0x3fff,
    NDC_IPRIO_ANIM = 0x4000,
    NDC_IPRIO_ANIMTK = 0x5000,		/* time keeper */
    NDC_IPRIO_ANIMROOT = 0x5fff,
    NDC_IPRIO_DYNVTX = 0x6000,
    NDC_IPRIO_DEFORM = 0x6100,
    NDC_IPRIO_ROOT = 0x7fff,
    NDC_IPRIO_SYSTEM_TOP = 0xffff,

    NDC_IFLAG_MTX_READY = 0x4000,
    NDC_IFLAG_ZONBIE = 0x8000,

    NDC_ROTORDER_XYZ = 0x00010203,
    NDC_ROTORDER_ZYX = 0x00030201,
    NDC_ROTORDER_YXZ = 0x00020103,
    NDC_ROTORDER_ZXY = 0x00030102,
    NDC_ROTORDERMASK = 0x00ffffff,

    /* for NDD_INST_OBJ.flag */
    NDC_OFLAG_ENABLE        = 0x80000000,
    NDC_OFLAG_VISIBLE       = 0x40000000,
    NDC_OFLAG_RENDERMODE    = 0x20000000,
    NDC_OFLAG_ALPHACOMPARE  = 0x10000000,
    NDC_OFLAG_ALPHADITHER   = 0x08000000,
    NDC_OFLAG_CYCLETYPE     = 0x04000000,
    NDC_OFLAG_CCMODE        = 0x02000000,
    NDC_OFLAG_GEOMMODE      = 0x01000000,
    NDC_OFLAG_PRIMCOLOR     = 0x00800000,
    NDC_OFLAG_ENVCOLOR      = 0x00400000,
    NDC_OFLAG_FOGCOLOR      = 0x00200000,
    NDC_OFLAG_BLEANDCOLOR   = 0x00100000,
    NDC_OFLAG_MATRIX        = 0x00080000,
    NDC_OFLAG_ALLREG        = 0x3ff80000,	/* combined flags */

    NDC_OFLAG_PAUSE_ANIM    = 0x00040000,
    NDC_OFLAG_PAUSE_DEFORM  = 0x00020000,

    NDC_OFLAG_BILLBOARD     = 0x00008000,
    NDC_OFLAG_2CYC          = NDC_REGF_2CYC
};

typedef struct{
    u8 r,g,b,a;
} RGBAQUAD;

struct ndd_instanceTag;

typedef struct{
    TRX trx;
} NDD_INST_NULL;

typedef struct{
    /* geometry propaty */
    float modelview[4][4];		/* Used by the View portion */
    TRX trx;
    u32 flag;
    u32 order;
    struct ndd_instanceTag *bb_obj[2];

    /* display propaty */
    NDR_SHAPE *shape;
    u32 geommode;
    u32 rdmode;
    u8 cc1[8], cc2[8];
    u32 primcol, envcol, fogcol, blcol;
    u8 prim_m, prim_l;
} NDD_INST_OBJ;

typedef struct{
    struct ndd_instanceTag *tarinst;
    float time;
    float timescale;
    s16 start;
    s16 end;
    struct ndd_instanceTag *animinst[10];
    u16 naniminst;
    s16 loop;
} NDD_INST_ANIMTK;

typedef struct{
    struct ndd_instanceTag *tarinst;
    float time;
    u16 dummy_padding;
    u16 kind;
    s16 start;
    s16 end;
    s16 nowkeypos;
    s16 nkey;
    s16 *key;
    float *value;
} NDD_INST_ANIM;

typedef struct{
    struct ndd_instanceTag *tarinst;	/* link at NDD_INST_OBJ type */
    Vtx *tarvtx[2];		/* destination data table(double buffer) */
    NDR_SHAPE *dyn_shape;	/* dynamic shape struct */
    NDR_SHAPE *src_shape;	/* old shape struct(backup) */
    struct ndd_instanceTag *dfminst[4];
} NDD_INST_DYNAMICVTX;

typedef struct{
    struct ndd_instanceTag *tarinst;	/* link at NDD_INST_DYNAMICVTX type */
    s32 *modify_vtx;	/* high-16bit:Vertex low-16bit:fragment */
    s32 *delta;		/* high-16bit:Vertex low-16bit:fragment */
    s16 downcounter;		/* if zero then next key */
    s16 nowkey;

    /* copy of source NDR_DEFORM */ 
    s16 *key;
    s16 **rawvtx;	/* opened table list in local allocate memory */
    u16 nkey;
    u16 nelem;
} NDD_INST_DEFORM_VTX;

typedef struct{
    struct ndd_instanceTag *tarinst;	/* link at NDD_INST_DYNAMICVTX type */
    s16 *modify_vtxcol;	/* high-8bit:color low-8bit:fragment */
    s16 *delta;		/* high-8bit:color low-8bit:fragment */
    s16 downcounter;		/* if zero then next key */
    s16 nowkey;

    /* copy of source NDR_DEFORM */ 
    s16 *key;
    u32 **rawvtxcol;	/* opened table list local allocate memory */
    u16 nkey;
    u16 nelem;
} NDD_INST_DEFORM_VTXCOL;

typedef struct{
    struct ndd_instanceTag *tarinst;	/* link at NDD_INST_DYNAMIC type */
    s16 *modify_vtxnv;	/* high-8bit:color low-8bit:fragment */
    s16 *delta;		/* high-8bit:color low-8bit:fragment */
    s16 downcounter;		/* if zero then next key */
    s16 nowkey;

    /* copy of source NDR_DEFORM */ 
    s16 *key;
    s8 **rawvtxnv;	/* opened table list local allocate memory */
    u16 nkey;
    u16 nelem;
} NDD_INST_DEFORM_VTXNV;

enum{
    NDC_ICB_EVAL = 0,
    NDC_ICB_DIE = 1
};

typedef void (*NDD_INST_CALLBACK)(struct ndd_instanceTag *isp, int type);

typedef struct ndd_instanceTag{
    struct ndd_instanceTag *next;
    struct ndd_instanceTag *back;
    struct ndd_instanceTag *parent;
    struct ndd_instanceTag *bros;
    struct ndd_instanceTag *child;
    NDD_INST_CALLBACK instCB;	/* callback */
    s16 type;			/* NDC_ITYPE_* */
    u16 prio;			/* NDC_IPRIO_* */
    u16 flag;			/* NDC_IFLAG_* */
    u16 nchild;

    u32 gid;
    union{
	NDD_INST_NULL null;
	NDD_INST_OBJ obj;
	NDD_INST_ANIM anim;
	NDD_INST_ANIMTK animtk;
	NDD_INST_DYNAMICVTX dynvtx;
	NDD_INST_DEFORM_VTX dfm_vtx;
	NDD_INST_DEFORM_VTXCOL dfm_vtxcol;
	NDD_INST_DEFORM_VTXNV dfm_vtxnv;
    }u;
} NDD_INSTANCE;

enum{
    /* for NDD_LIGHTSET.flag */
    NDC_LFLAG_ENABLE = 0x8000,		/* Evaluated only when ndInitLight() used */
    NDC_LFLAG_ALREADY_SETS = 0x4000,	/* Register set (for the manager) */
    NDC_LFLAG_HAVEPOS = 0x0001,		/* Has a position */
    NDC_LFLAG_ATTENUATION = 0x0002,	/* Evaluates attenuation from start, end */
    NDC_LFLAG_CUTOFF = 0x0004		/* Valid or invalid cutoff */
};

typedef struct {
    u16 flag;
    u16 lnum;			/* Light number transmitted to the registers */
    u32 stored_color;		/* Color value transmitted to the registers */
    s8 stored_nv[3];		/* Normal vector transmitted to the registers */
    RGBAQUAD color;		/* Only RGB valid */
    float fnv[3];		/* Vector from the light source to the object (Normalized) */
    float fpos[3];		/* World coordinate position */
    float start, end;
    float cutoff;		/* Light projection angle (radians) */
} NDD_LIGHT;

#define NUM_LIGHTSET_LIGHTS 7

typedef struct {
    NDD_LIGHT light[NUM_LIGHTSET_LIGHTS];
    RGBAQUAD ambient;
    u16 numlights;		/* Used by the manager */
} NDD_LIGHTSET;

typedef struct {
    u16 top, bottom, left, right;
    NDD_INSTANCE *cam[3];
    int enacam;
    u16 fill_col, fill_depth;
    struct{
	u16 near, far;
	RGBAQUAD color;
    } fog;
    struct{
	u32 mode;
	float fovy, aspect;
	u16 top, bottom, left, right, near, far;
	float scale;
    } camera;
    struct{
	float right[3];
	float upper[3];
    }reflect;
    NDD_LIGHTSET lightset;
} NDD_SCENE;

typedef struct {
    u16 ofs;
    u16 count;
} NDD_RESOFFSET;

enum {
    NDC_RES_USED_OBJ       = 0x0001,
    NDC_RES_USED_SHAPE     = 0x0002,
    NDC_RES_USED_ANIM      = 0x0004,
    NDC_RES_USED_RAWVTX    = 0x0008,
    NDC_RES_USED_RAWVTXCOL = 0x0010,
    NDC_RES_USED_RAWVTXNV  = 0x0020,
    NDC_RES_USED_ALL       = 0x003f
};

typedef struct {
    u16 usemask;
    NDR_RESINFO *resinfo;
    NDD_RESOFFSET obj;
    NDD_RESOFFSET shape;
    NDD_RESOFFSET anim;
    NDD_RESOFFSET rawvtx;
    NDD_RESOFFSET rawvtxcol;
    NDD_RESOFFSET rawvtxnv;
} NDD_RESINFO;

/* ---------------------------------------------------------------------------
	ndres.c
 --------------------------------------------------------------------------- */

/* variable */
extern NDR_OBJ **objlist;
extern NDR_SHAPE **shapelist;
extern NDR_ANIM **animlist;

extern s16 **rawvtxlist;
extern u32 **rawvtxcollist;
extern s8 **rawvtxnvlist;

/* function */
void ndResInit(int olplim, int slplim, int alplim, int rvlplim, int rvclplim, int rvnlplim);
NDD_RESINFO *ndResEntry(NDR_RESINFO *rrp, int usemask);
void ndResRelease(NDD_RESINFO *drp);

/* ---------------------------------------------------------------------------
	ndman.c
 --------------------------------------------------------------------------- */

/* variable */
extern u32 deform_flg;			/* Deform flag */

extern int ndinst_cnt;

extern NDD_SCENE cur_scene;		/* current scene */
extern NDD_INSTANCE top_inst;		/* top instance */
extern NDD_INSTANCE bottom_inst;	/* terminator instance */
extern NDD_INSTANCE *root_instp;	/* root instance pointer */
extern NDD_INSTANCE *obj_root_instp;	/* Object entry root */
extern NDD_INSTANCE *anim_root_instp;	/* Animation entry root */
extern NDD_INSTANCE *deform_root_instp; /* Deform entry root */
extern NDD_INSTANCE *dynvtx_root_instp; /* Dynamic vertex entry root */

/* function */
NDD_INSTANCE *ndGenInstance(NDD_INSTANCE *parent, u16 type, u16 prio, int gid);
void ndDeleteInstance(NDD_INSTANCE *isp);
u16 ndChangePriority(NDD_INSTANCE *isp, u16 newprio);
NDD_INSTANCE *ndChangeParent(NDD_INSTANCE *isp, NDD_INSTANCE *newparent);
void ndDeleteInstanceGID(NDD_INSTANCE *isp, int type, int gid, int mask);

void ndInit(void);
NDD_INSTANCE *ndEntryObjectRaw(NDD_INSTANCE *parent, NDR_OBJ *rop, int gid);
NDD_INSTANCE *ndEntryAnimationRaw(NDD_INSTANCE *parent, NDR_ANIM *anp, int gid, NDD_INSTANCE *tarinst);

NDD_INSTANCE *ndEntryObject(NDD_INSTANCE *parent, int objno, int shapeno, int animno, int gid, int flag);
void ndAttachAnimation(NDD_INSTANCE *parent, NDD_INSTANCE *isp, int objno, int animno, int gid, int flag);
NDD_INSTANCE *ndEntryDeformVtx(NDD_INSTANCE *isp, NDR_DEFORM *dmfp, int ofs, int gid);
NDD_INSTANCE *ndEntryDeformVtxcol(NDD_INSTANCE *isp, NDR_DEFORM *dmfp, int ofs, int gid);
NDD_INSTANCE *ndEntryDeformVtxnv(NDD_INSTANCE *isp, NDR_DEFORM *dmfp, int ofs, int gid);
NDD_INSTANCE *ndEntryDeformShape(NDD_INSTANCE *isp, int ofs_rawvtx, int ofs_rawvtxcol, int ofs_rawvtxnv, int gid, int flag);
void ndAttachDeform(NDD_INSTANCE *isp, int rawvtxno, int rawvtxcolno, int rawvtxnvno, int gid, int flag);
void ndSetupScene(NDD_RESINFO *rip, int gid, int flag);
void ndSetScreen(int top, int bottom, int left, int right);

NDD_INSTANCE *ndGenInstanceDeformVtx(NDD_INSTANCE *isp, NDR_DEFORM *dmfp, int ofs, int gid);
void ndAttachAnimation2(NDD_INSTANCE *parent, NDD_INSTANCE *isp, int objno, int animno, int gid, int flag, u8 basetime);
/* ---------------------------------------------------------------------------
	ndeval.c
 --------------------------------------------------------------------------- */

/* variable */

/* function */
void ndSetupIdentTRX(TRX *p);
TRX *ndGetInstanceTRXPointer(NDD_INSTANCE *isp);
void ndMakeCameraMatrix(float mf[4][4]);

int ndProgressAnimTime(NDD_INST_ANIMTK *ntkp);
int ndSetAnimChannel(NDD_INST_ANIM *niap);
void ndReloadDynamicVertex(NDD_INST_DYNAMICVTX *nidp);
void ndDeformVertex(NDD_INST_DEFORM_VTX *nidvp);

void ndMultiTrx(float mf[4][4], TRX *trxp, u32 order);
int ndEvalInstanceOne(NDD_INSTANCE *isp, float mf[4][4], float cmf[4][4], int forceupd);
void ndEvalInstance(NDD_INSTANCE *isp, float mf[4][4], int forceupd);

/* ---------------------------------------------------------------------------
	nddisp.c
 --------------------------------------------------------------------------- */

/* variable */

/* function */
Gfx *ndClearFB(Gfx *gp, u16 fill_depth, u16 fill_col);

int ndEvalLight(float *tarpos, NDD_LIGHT *lp, RGBAQUAD *color, s8 *nv);
Gfx *ndReloadLightReg(Gfx *gp, NDD_LIGHT *lp, u32 newcolor, s8 *newlvec);
Gfx *ndSetupLightset(Gfx *gp, float *tarpos, NDD_LIGHTSET *lightset);
int ndInitLights( NDD_LIGHTSET *lightset );

Gfx *ndSetupObjectRegister( Gfx *gp, NDD_INST_OBJ *niop );
Gfx *ndDrawObjectInstance( Gfx *gp, NDD_INST_OBJ *niop );

Gfx *ndBuildInstanceGfxAtPrio(Gfx *gp, NDD_INSTANCE *isp, u16 start, u16 end);
Gfx *ndBuildInstanceGfx(Gfx *gp, NDD_INSTANCE *isp);
Gfx *ndBuildSceneGfx(Gfx *gp, float mul[4][4]);

/* ---------------------------------------------------------------------------
	ndstuff.c
 --------------------------------------------------------------------------- */

/* variable */

/* function */
void ndguTranslateF(float mf[4][4], float x, float y, float z);
void ndguScaleF(float mf[4][4], float x, float y, float z);
void ndguRotateXYZF(float mf[4][4], float x, float y, float z);
void ndguRotateZYXF(float mf[4][4], float x, float y, float z);
void ndguRotateZXYF(float mf[4][4], float x, float y, float z);
void ndguRotateF(float mf[4][4], float a, float x, float y, float z);
void ndguRotateXF(float mf[4][4], float rot);
void ndguRotateYF(float mf[4][4], float rot);
void ndguRotateZF(float mf[4][4], float rot);

Gfx *ndgDPSetCombineMode(Gfx *glp, u8 a[8], u8 b[8]);
Gfx *ndgDPSetRenderMode(Gfx *glp, u32 src);

u16 ndguRGBA32ToRGBA16( u32 col );
RGBAQUAD *ndguCalcFadeColor(RGBAQUAD *tar, int flag, int ofs, int wlev);
RGBAQUAD *ndguCalcGradationColor(RGBAQUAD *tar, int flag, RGBAQUAD *fact, int lev);

float ndguCalcNormal(float nv[3], float *a, float *b);
float ndguNormalize(float nv[3]);


#endif   /* #ifndef __ND_H */

