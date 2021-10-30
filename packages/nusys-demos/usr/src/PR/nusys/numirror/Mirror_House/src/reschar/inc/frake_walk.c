
/* niff to gfx converter v1.2 */

/* ============================================================
          Header Area
 ============================================================ */

#ifndef __ND_COMMONS__
#define __ND_COMMONS__

/* ------------------ macros */

#define SEGTOP(sno) ((u32)(sno)<<24)
#define SEGADRS(sno,adrs) (SEGTOP(sno)|((u32)(adrs)&0x00ffffff))

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
  u16 nanim;
  u16 nshape;
  u16 nrawvtx;
  u16 nrawvtxcol;
  u16 nrawvtxnv;
} NDR_RESINFO;

#endif    /* #ifndef __ND_COMMONS__ */

#ifdef PREVIEW_TYPE
#undef PREVIEW_TYPE
#endif  /* #ifdef PREVIEW_TYPE */

#ifdef RESINFO_NAME
#undef RESINFO_NAME
#endif  /* #ifdef RESINFO_NAME */

#define PREVIEW_TYPE 28
extern NDR_RESINFO resinfo_frake_walk;
#define RESINFO_NAME resinfo_frake_walk

static Gfx _frake_walk_aligner0[] = { gsSPEndDisplayList() };

/* ============================================================
          TLUT
 ============================================================ */
static u16 tlut_frake_walk[] = {
/* name:t_shirt6  offset:0x00000000  length:32 bytes. */
0x9ffe,0xfd08,0xf4c8,0x5c1e,
0x3944,0x5a4c,0x218c,0x6dac,
0x8b92,0xf620,0xfe62,0x1082,
0xa416,0x0000,0x0000,0x0000,
/* name:belt5  offset:0x00000020  length:32 bytes. */
0x4826,0x0000,0x0002,0x188a,
0x49d6,0x7324,0x8c2c,0x8baa,
0x18c6,0x4214,0x40a8,0x40a8,
0x38ea,0x30ea,0x296c,0x0000,
/* name:mouth3_32  offset:0x00000040  length:32 bytes. */
0xed5e,0xf5a0,0xe51e,0xcc9a,
0xabd6,0x8310,0x624c,0x620c,
0x7ad0,0x9352,0xbc58,0x2906,
0x2084,0x0000,0x4188,0x0000,
/* name:eye_32c  offset:0x00000060  length:32 bytes. */
0xf5a0,0x8b52,0x6a90,0x0106,
0xbc58,0x418a,0xa528,0xef7a,
0xce30,0x2256,0x4b1c,0xfffe,
0x031e,0x03a8,0x0000,0x6b5a,
/* name:refmap2  offset:0x00000080  length:32 bytes. */
0x0000,0x18c8,0x1084,0x0842,
0x2108,0x210a,0x298c,0x4212,
0x5296,0x6b5c,0x7c22,0xb5f0,
0xe73a,0x9ce8,0xfffe,0x8464,
};

/* ============================================================
          Bitmap
 ============================================================ */
static u8 bmp_frake_walk[] = {
/* name:t_shirt6  type:CIDX  pixel/bit: 4 bits
  offset:0x00000000  length:512 bytes. */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,
0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,
0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,
0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,
0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,
0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,
0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,
0x11,0x10,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,
0x21,0x10,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,
0x11,0x10,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,
0x11,0x10,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,
0x11,0x10,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,
0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,
0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,
0x11,0x11,0x10,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,
0x11,0x11,0x10,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,
0x11,0x11,0x10,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,
0x11,0x11,0x10,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x01,0x11,0x11,
0x11,0x11,0x10,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x34,0x55,
0x55,0x46,0x70,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x68,0x9a,0xa9,
0xaa,0x99,0x8b,0x70,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x07,0xca,0xaa,0xaa,
0xaa,0xaa,0xa9,0xb0,0x00,0x00,0x00,0x00,
/* name:belt5  type:CIDX  pixel/bit: 4 bits
  offset:0x00000200  length:512 bytes. */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x11,0x11,0x11,0x23,0x45,0x67,0x54,0x31,
0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,
0x11,0x11,0x11,0x18,0x45,0x66,0x59,0x81,
0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,
0x11,0x11,0x11,0x23,0x45,0x77,0x54,0x32,
0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,
0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,
0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,
0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,
0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,
0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,
0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,
0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,
0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,
0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,
0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,
0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,
0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,
0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,
/* name:mouth3_32  type:CIDX  pixel/bit: 4 bits
  offset:0x00000400  length:512 bytes. */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x02,
0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0x02,0x23,0x45,0x56,
0x77,0x68,0x59,0x43,0x20,0x00,0x00,0x00,
0x00,0x00,0x00,0x12,0xa5,0x67,0xbc,0xcc,
0xcc,0xdc,0xcb,0xe7,0x8a,0x00,0x00,0x00,
0x00,0x00,0x02,0xa5,0xec,0xcc,0xcb,0xe7,
0x66,0x7e,0xeb,0xcc,0xce,0x53,0x00,0x00,
0x00,0x02,0xa8,0xec,0xcb,0xe8,0x59,0xa3,
0x22,0x2a,0x49,0x87,0xcc,0xb7,0x42,0x00,
0x00,0x29,0xec,0xcb,0x79,0x30,0x02,0x00,
0x00,0x00,0x00,0x23,0x5e,0xcc,0xe4,0x00,
0x12,0x9e,0xcb,0x79,0x22,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x2a,0x8b,0xce,0xa0,
0x35,0xbc,0xb9,0x32,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x25,0xbc,0x7a,
0x9b,0xce,0x42,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x13,0x8c,0xc5,
0x6c,0xe4,0x10,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xa6,0xb5,
0xa9,0xa2,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x4a,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
/* name:eye_32c  type:CIDX  pixel/bit: 4 bits
  offset:0x00000600  length:512 bytes. */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x12,
0x12,0x33,0x22,0x40,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x04,0x55,0x22,
0x67,0x87,0x62,0x25,0x40,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x01,0x55,0x55,0x33,
0x9a,0xbb,0xbb,0xb8,0x34,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x15,0x53,0xcc,0xdc,
0xc3,0x38,0xbb,0xbb,0x83,0x10,0x00,0x00,
0x00,0x00,0x00,0x01,0x5e,0xcd,0xc3,0x33,
0xcd,0xc3,0x8b,0xbb,0xb7,0x52,0x00,0x00,
0x00,0x00,0x00,0x45,0x5c,0xdc,0x3e,0xee,
0x3d,0xdc,0x37,0xbb,0xbb,0x85,0x40,0x00,
0x00,0x00,0x00,0x25,0x3d,0xdc,0xee,0xee,
0x3d,0xdd,0x3a,0xbb,0xbb,0xb2,0x10,0x00,
0x00,0x00,0x00,0x25,0xcd,0xc5,0x3e,0xee,
0x3d,0xdd,0xc9,0xbb,0xbb,0xb8,0x50,0x00,
0x00,0x00,0x00,0x53,0xdd,0xf7,0xbf,0xee,
0x3d,0xdd,0xc3,0xbb,0xbb,0xb7,0x54,0x00,
0x00,0x00,0x00,0x53,0xdc,0xbb,0xbb,0x3e,
0xcd,0xdd,0xd3,0xbb,0xbb,0xbb,0x22,0x00,
0x00,0x00,0x00,0x55,0xc6,0xbb,0xbb,0xfc,
0xdd,0xdd,0xc3,0xbb,0xbb,0xbb,0x62,0x00,
0x00,0x00,0x00,0x15,0xca,0xbb,0xbb,0xad,
0xdd,0xdd,0xc9,0xbb,0xbb,0xbb,0x42,0x00,
0x00,0x00,0x00,0x03,0x3c,0x7b,0xb7,0xcd,
0xdd,0xdd,0x36,0xbb,0xbb,0xbb,0x22,0x00,
0x00,0x00,0x00,0x04,0x33,0xc6,0x6c,0xdd,
0xdd,0xc3,0xab,0xbb,0xbb,0xb7,0x54,0x00,
0x00,0x00,0x00,0x00,0x23,0x3c,0xcc,0xdc,
0xc3,0x3a,0xbb,0xbb,0xbb,0xb1,0x20,0x00,
0x00,0x00,0x00,0x00,0x00,0x23,0x33,0x39,
0x9f,0xbb,0xbb,0xbb,0xb7,0x25,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x5f,
0xff,0x68,0x77,0x6f,0xf3,0x20,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,
0x12,0xf2,0x55,0x21,0x40,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x25,0x54,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x5e,0xee,0x54,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x13,0xee,0xee,0x35,0x40,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x04,0x5e,0xee,0xee,0xe5,
0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x04,0x5e,0xee,0xee,
0xe3,0x24,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x04,0x53,0xee,
0xee,0xee,0x54,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x24,
0x12,0x53,0x22,0x10,0x00,0x00,0x00,0x00,
/* name:refmap2  type:CIDX  pixel/bit: 4 bits
  offset:0x00000800  length:512 bytes. */
0x00,0x00,0x00,0x00,0x00,0x00,0x12,0x30,
0x33,0x34,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x24,0x22,0x22,
0x22,0x22,0x23,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x31,0x44,0x44,0x24,
0x22,0x23,0x23,0x23,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x41,0x44,0x44,0x44,0x44,
0x44,0x44,0x42,0x23,0x20,0x00,0x00,0x00,
0x00,0x00,0x03,0x14,0x24,0x44,0x44,0x44,
0x44,0x44,0x44,0x13,0x22,0x30,0x00,0x00,
0x00,0x00,0x34,0x44,0x44,0x44,0x44,0x44,
0x14,0x44,0x44,0x41,0x23,0x32,0x00,0x00,
0x00,0x04,0x34,0x44,0x44,0x24,0x12,0x24,
0x22,0x44,0x44,0x44,0x42,0x33,0x40,0x00,
0x00,0x03,0x24,0x44,0x42,0x33,0x33,0x22,
0x22,0x44,0x44,0x44,0x44,0x33,0x30,0x00,
0x00,0x23,0x24,0x44,0x42,0x33,0x33,0x22,
0x24,0x24,0x44,0x44,0x44,0x42,0x31,0x00,
0x00,0x23,0x22,0x44,0x03,0x32,0x33,0x24,
0x22,0x24,0x55,0x35,0x54,0x44,0x32,0x00,
0x01,0x33,0x22,0x43,0x32,0x22,0x11,0x22,
0x22,0x25,0x55,0x54,0x42,0x24,0x13,0x00,
0x02,0x32,0x24,0x43,0x24,0x42,0x33,0x22,
0x11,0x14,0x54,0x22,0x22,0x24,0x23,0x30,
0x02,0x04,0x22,0x33,0x22,0x21,0x22,0x34,
0x22,0x22,0x33,0x22,0x33,0x42,0x23,0x30,
0x23,0x04,0x20,0x30,0x00,0x03,0x33,0x03,
0x33,0x33,0x33,0x33,0x33,0x42,0x23,0x22,
0x23,0x32,0x23,0x33,0x32,0x44,0x33,0x33,
0x33,0x23,0x22,0x33,0x33,0x41,0x13,0x24,
0x23,0x33,0x33,0x44,0x22,0x42,0x23,0x32,
0x22,0x22,0x32,0x32,0x33,0x12,0x23,0x22,
0x23,0x33,0x03,0x22,0x22,0x23,0x33,0x32,
0x22,0x11,0x11,0x23,0x33,0x32,0x22,0x22,
0x23,0x22,0x11,0x11,0x12,0x23,0x23,0x31,
0x12,0x11,0x11,0x22,0x32,0x11,0x11,0x23,
0x33,0x11,0x11,0x11,0x14,0x44,0x42,0x44,
0x44,0x42,0x22,0x32,0x11,0x11,0x14,0x11,
0x02,0x21,0x11,0x11,0x11,0x11,0x44,0x44,
0x44,0x44,0x44,0x11,0x11,0x11,0x11,0x10,
0x02,0x11,0x11,0x11,0x11,0x14,0x42,0x55,
0x55,0x65,0x55,0x45,0x41,0x11,0x11,0x10,
0x02,0x11,0x11,0x11,0x11,0x44,0x45,0x55,
0x66,0x66,0x66,0x55,0x54,0x11,0x11,0x10,
0x00,0x11,0x11,0x11,0x44,0x14,0x55,0x66,
0x67,0x77,0x77,0x65,0x54,0x11,0x11,0x00,
0x00,0x41,0x11,0x11,0x11,0x11,0x55,0x66,
0x78,0x99,0x98,0x66,0x54,0x11,0x11,0x00,
0x00,0x01,0x11,0x11,0x11,0x11,0x55,0x66,
0x7a,0xbc,0xb9,0x76,0x51,0x14,0x10,0x00,
0x00,0x04,0x11,0x11,0x11,0x11,0x55,0x66,
0x8d,0xee,0xca,0x76,0x51,0x14,0x40,0x00,
0x00,0x00,0x41,0x11,0x11,0x11,0x15,0x66,
0x8d,0xee,0xb9,0x76,0x51,0x14,0x00,0x00,
0x00,0x00,0x04,0x41,0x11,0x11,0x11,0x56,
0x79,0xaf,0x97,0x65,0x55,0x50,0x00,0x00,
0x00,0x00,0x00,0x45,0x11,0x11,0x11,0x15,
0x67,0x77,0x66,0x55,0x55,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x51,0x11,0x11,0x55,
0x56,0x66,0x65,0x55,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x05,0x51,0x15,0x55,
0x55,0x55,0x55,0x50,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,
0x55,0x55,0x00,0x00,0x00,0x00,0x00,0x00,
};

static Gfx _frake_walk_aligner1[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_300_[] = {
  {    44,     5,   -40,0, 0x0000,0x0000, 0x4a,0xcf,0xcf,0xff }, /*     0 */
  {   -44,     5,   -40,0, 0x0000,0x0000, 0x38,0x9c,0x9c,0xff }, /*     1 */
  {    40,    31,   -38,0, 0x0000,0x0000, 0x9d,0xff,0xff,0xff }, /*     2 */
  {   -11,   107,    -2,0, 0x0000,0x0000, 0xa1,0xff,0xff,0xff }, /*     3 */
  {    11,   107,   -25,0, 0x0000,0x0000, 0xba,0xff,0xff,0xff }, /*     4 */
  {   -11,   107,   -25,0, 0x0000,0x0000, 0xad,0xff,0xff,0xff }, /*     5 */
  {   -11,   107,    -2,0, 0x0000,0x0000, 0xa1,0xff,0xff,0xff }, /*     6 */
  {    11,   107,    -2,0, 0x0000,0x0000, 0xaa,0xff,0xff,0xff }, /*     7 */
  {    11,   107,   -25,0, 0x0000,0x0000, 0xba,0xff,0xff,0xff }, /*     8 */
  {    41,    31,    28,0, 0x0000,0x0000, 0xaa,0xff,0xff,0xff }, /*     9 */
  {    40,    31,   -38,0, 0x0000,0x0000, 0x9d,0xff,0xff,0xff }, /*    10 */
  {    47,    98,     6,0, 0x0000,0x0000, 0xba,0xff,0xff,0xff }, /*    11 */
  {   -40,    31,   -38,0, 0x0000,0x0000, 0x8b,0xe8,0xe8,0xff }, /*    12 */
  {   -47,    98,     6,0, 0x0000,0x0000, 0x9f,0xff,0xff,0xff }, /*    13 */
  {   -46,    98,   -33,0, 0x0000,0x0000, 0x76,0xc5,0xc5,0xff }, /*    14 */
  {   -40,    31,   -38,0, 0x0000,0x0000, 0x8b,0xe8,0xe8,0xff }, /*    15 */
  {   -46,    98,   -33,0, 0x0000,0x0000, 0x76,0xc5,0xc5,0xff }, /*    16 */
  {    40,    31,   -38,0, 0x0000,0x0000, 0x9d,0xff,0xff,0xff }, /*    17 */
  {   -47,    98,     6,0, 0x0000,0x0000, 0x9f,0xff,0xff,0xff }, /*    18 */
  {   -11,   107,   -25,0, 0x0000,0x0000, 0xad,0xff,0xff,0xff }, /*    19 */
  {   -46,    98,   -33,0, 0x0000,0x0000, 0x76,0xc5,0xc5,0xff }, /*    20 */
  {   -46,    98,   -33,0, 0x0000,0x0000, 0x76,0xc5,0xc5,0xff }, /*    21 */
  {    11,   107,   -25,0, 0x0000,0x0000, 0xba,0xff,0xff,0xff }, /*    22 */
  {    46,    98,   -33,0, 0x0000,0x0000, 0xb0,0xff,0xff,0xff }, /*    23 */
  {   -46,    98,   -33,0, 0x0000,0x0000, 0x76,0xc5,0xc5,0xff }, /*    24 */
  {   -11,   107,   -25,0, 0x0000,0x0000, 0xad,0xff,0xff,0xff }, /*    25 */
  {    11,   107,   -25,0, 0x0000,0x0000, 0xba,0xff,0xff,0xff }, /*    26 */
  {    46,    98,   -33,0, 0x0000,0x0000, 0xb0,0xff,0xff,0xff }, /*    27 */
  {    11,   107,   -25,0, 0x0000,0x0000, 0xba,0xff,0xff,0xff }, /*    28 */
  {    47,    98,     6,0, 0x0000,0x0000, 0xba,0xff,0xff,0xff }, /*    29 */
  {    41,    31,    28,0, 0x0000,0x0000, 0xaa,0xff,0xff,0xff }, /*    30 */
  {    44,     5,   -40,0, 0x0000,0x0000, 0x4a,0xcf,0xcf,0xff }, /*    31 */
  {    40,    31,   -38,0, 0x0000,0x0000, 0x9d,0xff,0xff,0xff }, /*    32 */
  {   -41,    31,    28,0, 0x0000,0x0000, 0x91,0xf3,0xf3,0xff }, /*    33 */
  {    44,     5,    31,0, 0x0000,0x0000, 0x34,0x90,0x90,0xff }, /*    34 */
  {    41,    31,    28,0, 0x0000,0x0000, 0xaa,0xff,0xff,0xff }, /*    35 */
  {   -41,    31,    28,0, 0x0000,0x0000, 0x91,0xf3,0xf3,0xff }, /*    36 */
  {   -44,     5,    31,0, 0x0000,0x0000, 0x32,0x8c,0x8c,0xff }, /*    37 */
  {    44,     5,    31,0, 0x0000,0x0000, 0x34,0x90,0x90,0xff }, /*    38 */
  {   -40,    31,   -38,0, 0x0000,0x0000, 0x8b,0xe8,0xe8,0xff }, /*    39 */
  {   -44,     5,   -40,0, 0x0000,0x0000, 0x38,0x9c,0x9c,0xff }, /*    40 */
  {   -41,    31,    28,0, 0x0000,0x0000, 0x91,0xf3,0xf3,0xff }, /*    41 */
  {   -46,    98,   -33,0, 0x0000,0x0000, 0x76,0xc5,0xc5,0xff }, /*    42 */
  {    46,    98,   -33,0, 0x0000,0x0000, 0xb0,0xff,0xff,0xff }, /*    43 */
  {    40,    31,   -38,0, 0x0000,0x0000, 0x9d,0xff,0xff,0xff }, /*    44 */
  {   -44,     5,   -40,0, 0x0000,0x0000, 0x38,0x9c,0x9c,0xff }, /*    45 */
  {   -44,     5,    31,0, 0x0000,0x0000, 0x32,0x8c,0x8c,0xff }, /*    46 */
  {   -41,    31,    28,0, 0x0000,0x0000, 0x91,0xf3,0xf3,0xff }, /*    47 */
  {    41,    31,    28,0, 0x0000,0x0000, 0xaa,0xff,0xff,0xff }, /*    48 */
  {    44,     5,    31,0, 0x0000,0x0000, 0x34,0x90,0x90,0xff }, /*    49 */
  {    44,     5,   -40,0, 0x0000,0x0000, 0x4a,0xcf,0xcf,0xff }, /*    50 */
  {    11,   107,   -25,0, 0x0000,0x0000, 0xba,0xff,0xff,0xff }, /*    51 */
  {    11,   107,    -2,0, 0x0000,0x0000, 0xaa,0xff,0xff,0xff }, /*    52 */
  {    47,    98,     6,0, 0x0000,0x0000, 0xba,0xff,0xff,0xff }, /*    53 */
  {   -47,    98,     6,0, 0x0000,0x0000, 0x9f,0xff,0xff,0xff }, /*    54 */
  {   -11,   107,    -2,0, 0x0000,0x0000, 0xa1,0xff,0xff,0xff }, /*    55 */
  {   -11,   107,   -25,0, 0x0000,0x0000, 0xad,0xff,0xff,0xff }, /*    56 */
  {   -40,    31,   -38,0, 0x0000,0x0000, 0x8b,0xe8,0xe8,0xff }, /*    57 */
  {   -41,    31,    28,0, 0x0000,0x0000, 0x91,0xf3,0xf3,0xff }, /*    58 */
  {   -47,    98,     6,0, 0x0000,0x0000, 0x9f,0xff,0xff,0xff }, /*    59 */
  {    40,    31,   -38,0, 0x0000,0x0000, 0x9d,0xff,0xff,0xff }, /*    60 */
  {    46,    98,   -33,0, 0x0000,0x0000, 0xb0,0xff,0xff,0xff }, /*    61 */
  {    47,    98,     6,0, 0x0000,0x0000, 0xba,0xff,0xff,0xff }, /*    62 */
  {   -44,     5,   -40,0, 0x0000,0x0000, 0x38,0x9c,0x9c,0xff }, /*    63 */
  {   -40,    31,   -38,0, 0x0000,0x0000, 0x8b,0xe8,0xe8,0xff }, /*    64 */
  {    40,    31,   -38,0, 0x0000,0x0000, 0x9d,0xff,0xff,0xff }, /*    65 */
  {    41,    31,    28,0, 0x0779,0x0000, 0xff,0xff,0xff,0xff }, /*    66 */
  {    47,    98,     6,0, 0x07ff,0x070b, 0xff,0xff,0xff,0xff }, /*    67 */
  {   -41,    31,    28,0, 0x0086,0x0000, 0xf3,0xf3,0xf3,0xff }, /*    68 */
  {    47,    98,     6,0, 0x07ff,0x070b, 0xff,0xff,0xff,0xff }, /*    69 */
  {   -11,   107,    -2,0, 0x0302,0x0800, 0xff,0xff,0xff,0xff }, /*    70 */
  {   -47,    98,     6,0, 0x0000,0x070b, 0xff,0xff,0xff,0xff }, /*    71 */
  {    47,    98,     6,0, 0x07ff,0x070b, 0xff,0xff,0xff,0xff }, /*    72 */
  {    11,   107,    -2,0, 0x04fd,0x0800, 0xff,0xff,0xff,0xff }, /*    73 */
  {   -11,   107,    -2,0, 0x0302,0x0800, 0xff,0xff,0xff,0xff }, /*    74 */
  {    47,    98,     6,0, 0x07ff,0x070b, 0xff,0xff,0xff,0xff }, /*    75 */
  {   -47,    98,     6,0, 0x0000,0x070b, 0xff,0xff,0xff,0xff }, /*    76 */
  {   -41,    31,    28,0, 0x0086,0x0000, 0xf3,0xf3,0xf3,0xff }, /*    77 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_300_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 32, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP2Triangles(  3,  4,  5,  0,  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsSP2Triangles( 12, 13, 14,  0, 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP2Triangles( 21, 22, 23,  0, 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 30, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP2Triangles(  1,  2,  3,  0,  4,  5,  6,  0),
  gsSP1Triangle(  7,  8,  9,  0),
  gsSP1Triangle( 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsSP1Triangle( 16, 17, 18,  0),
  gsSP1Triangle( 19, 20, 21,  0),
  gsSP1Triangle( 22, 23, 24,  0),
  gsSP1Triangle( 25, 26, 27,  0),
  gsSPVertex( SEGADRS(5, 992), 2, 30 ),
  gsSP1Triangle( 28, 29, 30,  0),
  gsSPVertex( SEGADRS(5, 1024), 14, 0 ),
  gsSP1Triangle( 31,  0,  1,  0),
  gsDPPipeSync(),
  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0, TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0),
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
  gsDPSetTextureLUT( G_TT_RGBA16 ),
  gsDPLoadTLUT_pal16( 0, SEGADRS(7, 0) ),
  gsDPLoadMultiBlock_4b( SEGADRS(6,0), 0, 0,
    G_IM_FMT_CI, 32, 32, 0,
    0, 0, 5, 5, 0, 0 ),
  gsSP1Triangle(  2,  3,  4,  0),
  gsSP2Triangles(  5,  6,  7,  0,  8,  9, 10,  0),
  gsSP1Triangle( 11, 12, 13,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_300_ = {
  gfx_Shape_of___POLYHEDRON_300_,
  vtx_Shape_of___POLYHEDRON_300_, 78, 5,
  bmp_frake_walk,
  tlut_frake_walk,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_walk_aligner2[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_314_[] = {
  {   -92,    45,     4,0, 0x0000,0x0000, 0xbf,0x8a,0x63,0xff }, /*     0 */
  {   -52,    54,     5,0, 0x0000,0x0000, 0xff,0xd9,0x9c,0xff }, /*     1 */
  {   -75,    71,     5,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*     2 */
  {   -92,    45,     4,0, 0x0000,0x0000, 0xbf,0x8a,0x63,0xff }, /*     3 */
  {   -71,    30,     4,0, 0x0000,0x0000, 0xb3,0x81,0x5d,0xff }, /*     4 */
  {   -52,    54,     5,0, 0x0000,0x0000, 0xff,0xd9,0x9c,0xff }, /*     5 */
  {   -71,    30,   -20,0, 0x0000,0x0000, 0xff,0xc4,0x8d,0xff }, /*     6 */
  {   -92,    45,   -20,0, 0x0000,0x0000, 0xe7,0xa7,0x78,0xff }, /*     7 */
  {   -52,    54,   -22,0, 0x0000,0x0000, 0xff,0xc8,0x90,0xff }, /*     8 */
  {   -71,    30,     4,0, 0x0000,0x0000, 0xb3,0x81,0x5d,0xff }, /*     9 */
  {   -71,    30,   -20,0, 0x0000,0x0000, 0xff,0xc4,0x8d,0xff }, /*    10 */
  {   -52,    54,     5,0, 0x0000,0x0000, 0xff,0xd9,0x9c,0xff }, /*    11 */
  {   -92,    45,   -20,0, 0x0000,0x0000, 0xe7,0xa7,0x78,0xff }, /*    12 */
  {   -75,    71,     5,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    13 */
  {   -75,    71,   -22,0, 0x0000,0x0000, 0xbf,0x8a,0x63,0xff }, /*    14 */
  {   -52,    54,     5,0, 0x0000,0x0000, 0xff,0xd9,0x9c,0xff }, /*    15 */
  {   -56,    73,    -8,0, 0x0000,0x0000, 0xff,0xe2,0xa2,0xff }, /*    16 */
  {   -75,    71,     5,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    17 */
  {   -75,    71,     5,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    18 */
  {   -56,    73,    -8,0, 0x0000,0x0000, 0xff,0xe2,0xa2,0xff }, /*    19 */
  {   -75,    71,   -22,0, 0x0000,0x0000, 0xbf,0x8a,0x63,0xff }, /*    20 */
  {   -75,    71,   -22,0, 0x0000,0x0000, 0xbf,0x8a,0x63,0xff }, /*    21 */
  {   -56,    73,    -8,0, 0x0000,0x0000, 0xff,0xe2,0xa2,0xff }, /*    22 */
  {   -52,    54,   -22,0, 0x0000,0x0000, 0xff,0xc8,0x90,0xff }, /*    23 */
  {   -52,    54,   -22,0, 0x0000,0x0000, 0xff,0xc8,0x90,0xff }, /*    24 */
  {   -56,    73,    -8,0, 0x0000,0x0000, 0xff,0xe2,0xa2,0xff }, /*    25 */
  {   -52,    54,     5,0, 0x0000,0x0000, 0xff,0xd9,0x9c,0xff }, /*    26 */
  {   -92,    45,   -20,0, 0x0000,0x0000, 0xe7,0xa7,0x78,0xff }, /*    27 */
  {   -92,    45,     4,0, 0x0000,0x0000, 0xbf,0x8a,0x63,0xff }, /*    28 */
  {   -75,    71,     5,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    29 */
  {   -71,    30,   -20,0, 0x0000,0x0000, 0xff,0xc4,0x8d,0xff }, /*    30 */
  {   -52,    54,   -22,0, 0x0000,0x0000, 0xff,0xc8,0x90,0xff }, /*    31 */
  {   -52,    54,     5,0, 0x0000,0x0000, 0xff,0xd9,0x9c,0xff }, /*    32 */
  {   -92,    45,   -20,0, 0x0000,0x0000, 0xe7,0xa7,0x78,0xff }, /*    33 */
  {   -75,    71,   -22,0, 0x0000,0x0000, 0xbf,0x8a,0x63,0xff }, /*    34 */
  {   -52,    54,   -22,0, 0x0000,0x0000, 0xff,0xc8,0x90,0xff }, /*    35 */
  {   -71,    30,   -20,0, 0x0000,0x0000, 0xff,0xc4,0x8d,0xff }, /*    36 */
  {   -88,    29,    -8,0, 0x0000,0x0000, 0xbc,0x51,0x3a,0xff }, /*    37 */
  {   -92,    45,   -20,0, 0x0000,0x0000, 0xe7,0xa7,0x78,0xff }, /*    38 */
  {   -92,    45,   -20,0, 0x0000,0x0000, 0xe7,0xa7,0x78,0xff }, /*    39 */
  {   -88,    29,    -8,0, 0x0000,0x0000, 0xbc,0x51,0x3a,0xff }, /*    40 */
  {   -92,    45,     4,0, 0x0000,0x0000, 0xbf,0x8a,0x63,0xff }, /*    41 */
  {   -71,    30,     4,0, 0x0000,0x0000, 0xb3,0x81,0x5d,0xff }, /*    42 */
  {   -88,    29,    -8,0, 0x0000,0x0000, 0xbc,0x51,0x3a,0xff }, /*    43 */
  {   -71,    30,   -20,0, 0x0000,0x0000, 0xff,0xc4,0x8d,0xff }, /*    44 */
  {   -92,    45,     4,0, 0x0000,0x0000, 0xbf,0x8a,0x63,0xff }, /*    45 */
  {   -88,    29,    -8,0, 0x0000,0x0000, 0xbc,0x51,0x3a,0xff }, /*    46 */
  {   -71,    30,     4,0, 0x0000,0x0000, 0xb3,0x81,0x5d,0xff }, /*    47 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_314_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 32, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP1Triangle(  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 16, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP1Triangle(  1,  2,  3,  0),
  gsSP1Triangle(  4,  5,  6,  0),
  gsSP2Triangles(  7,  8,  9,  0, 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_314_ = {
  gfx_Shape_of___POLYHEDRON_314_,
  vtx_Shape_of___POLYHEDRON_314_, 48, 5,
  bmp_frake_walk,
  tlut_frake_walk,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_walk_aligner3[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_316_[] = {
  {  -110,    15,    -1,0, 0x0000,0x0000, 0xe1,0xa3,0x75,0xff }, /*     0 */
  {   -74,    29,     3,0, 0x0000,0x0000, 0xff,0xf6,0xb1,0xff }, /*     1 */
  {   -92,    42,     3,0, 0x0000,0x0000, 0xff,0xca,0x91,0xff }, /*     2 */
  {  -110,    15,    -1,0, 0x0000,0x0000, 0xe1,0xa3,0x75,0xff }, /*     3 */
  {   -94,     3,    -1,0, 0x0000,0x0000, 0xd4,0x9a,0x6e,0xff }, /*     4 */
  {   -74,    29,     3,0, 0x0000,0x0000, 0xff,0xf6,0xb1,0xff }, /*     5 */
  {   -94,     3,   -15,0, 0x0000,0x0000, 0xdf,0xa1,0x74,0xff }, /*     6 */
  {  -110,    15,   -15,0, 0x0000,0x0000, 0xe0,0xa2,0x74,0xff }, /*     7 */
  {   -74,    29,   -19,0, 0x0000,0x0000, 0xff,0xd2,0x97,0xff }, /*     8 */
  {  -110,    15,   -15,0, 0x0000,0x0000, 0xe0,0xa2,0x74,0xff }, /*     9 */
  {   -92,    42,   -19,0, 0x0000,0x0000, 0xd4,0x9a,0x6e,0xff }, /*    10 */
  {   -74,    29,   -19,0, 0x0000,0x0000, 0xff,0xd2,0x97,0xff }, /*    11 */
  {   -94,     3,    -1,0, 0x0000,0x0000, 0xd4,0x9a,0x6e,0xff }, /*    12 */
  {   -94,     3,   -15,0, 0x0000,0x0000, 0xdf,0xa1,0x74,0xff }, /*    13 */
  {   -74,    29,     3,0, 0x0000,0x0000, 0xff,0xf6,0xb1,0xff }, /*    14 */
  {  -110,    15,   -15,0, 0x0000,0x0000, 0xe0,0xa2,0x74,0xff }, /*    15 */
  {   -92,    42,     3,0, 0x0000,0x0000, 0xff,0xca,0x91,0xff }, /*    16 */
  {   -92,    42,   -19,0, 0x0000,0x0000, 0xd4,0x9a,0x6e,0xff }, /*    17 */
  {   -74,    29,     3,0, 0x0000,0x0000, 0xff,0xf6,0xb1,0xff }, /*    18 */
  {   -77,    44,    -8,0, 0x0000,0x0000, 0xff,0xf0,0xad,0xff }, /*    19 */
  {   -92,    42,     3,0, 0x0000,0x0000, 0xff,0xca,0x91,0xff }, /*    20 */
  {   -92,    42,     3,0, 0x0000,0x0000, 0xff,0xca,0x91,0xff }, /*    21 */
  {   -77,    44,    -8,0, 0x0000,0x0000, 0xff,0xf0,0xad,0xff }, /*    22 */
  {   -92,    42,   -19,0, 0x0000,0x0000, 0xd4,0x9a,0x6e,0xff }, /*    23 */
  {   -92,    42,   -19,0, 0x0000,0x0000, 0xd4,0x9a,0x6e,0xff }, /*    24 */
  {   -77,    44,    -8,0, 0x0000,0x0000, 0xff,0xf0,0xad,0xff }, /*    25 */
  {   -74,    29,   -19,0, 0x0000,0x0000, 0xff,0xd2,0x97,0xff }, /*    26 */
  {   -74,    29,   -19,0, 0x0000,0x0000, 0xff,0xd2,0x97,0xff }, /*    27 */
  {   -77,    44,    -8,0, 0x0000,0x0000, 0xff,0xf0,0xad,0xff }, /*    28 */
  {   -74,    29,     3,0, 0x0000,0x0000, 0xff,0xf6,0xb1,0xff }, /*    29 */
  {  -110,    15,   -15,0, 0x0000,0x0000, 0xe0,0xa2,0x74,0xff }, /*    30 */
  {  -110,    15,    -1,0, 0x0000,0x0000, 0xe1,0xa3,0x75,0xff }, /*    31 */
  {   -92,    42,     3,0, 0x0000,0x0000, 0xff,0xca,0x91,0xff }, /*    32 */
  {   -94,     3,   -15,0, 0x0000,0x0000, 0xdf,0xa1,0x74,0xff }, /*    33 */
  {   -74,    29,   -19,0, 0x0000,0x0000, 0xff,0xd2,0x97,0xff }, /*    34 */
  {   -74,    29,     3,0, 0x0000,0x0000, 0xff,0xf6,0xb1,0xff }, /*    35 */
  {   -94,     3,   -15,0, 0x0000,0x0000, 0xdf,0xa1,0x74,0xff }, /*    36 */
  {  -107,     2,    -8,0, 0x0000,0x0000, 0xc9,0x92,0x69,0xff }, /*    37 */
  {  -110,    15,   -15,0, 0x0000,0x0000, 0xe0,0xa2,0x74,0xff }, /*    38 */
  {  -110,    15,   -15,0, 0x0000,0x0000, 0xe0,0xa2,0x74,0xff }, /*    39 */
  {  -107,     2,    -8,0, 0x0000,0x0000, 0xc9,0x92,0x69,0xff }, /*    40 */
  {  -110,    15,    -1,0, 0x0000,0x0000, 0xe1,0xa3,0x75,0xff }, /*    41 */
  {  -110,    15,    -1,0, 0x0000,0x0000, 0xe1,0xa3,0x75,0xff }, /*    42 */
  {  -107,     2,    -8,0, 0x0000,0x0000, 0xc9,0x92,0x69,0xff }, /*    43 */
  {   -94,     3,    -1,0, 0x0000,0x0000, 0xd4,0x9a,0x6e,0xff }, /*    44 */
  {   -94,     3,    -1,0, 0x0000,0x0000, 0xd4,0x9a,0x6e,0xff }, /*    45 */
  {  -107,     2,    -8,0, 0x0000,0x0000, 0xc9,0x92,0x69,0xff }, /*    46 */
  {   -94,     3,   -15,0, 0x0000,0x0000, 0xdf,0xa1,0x74,0xff }, /*    47 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_316_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 32, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP2Triangles(  6,  7,  8,  0,  9, 10, 11,  0),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 16, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP1Triangle(  1,  2,  3,  0),
  gsSP1Triangle(  4,  5,  6,  0),
  gsSP1Triangle(  7,  8,  9,  0),
  gsSP1Triangle( 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_316_ = {
  gfx_Shape_of___POLYHEDRON_316_,
  vtx_Shape_of___POLYHEDRON_316_, 48, 5,
  bmp_frake_walk,
  tlut_frake_walk,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_walk_aligner4[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_318_[] = {
  {  -127,    -9,     2,0, 0x0000,0x0000, 0xff,0xbe,0x88,0xff }, /*     0 */
  {  -103,    -5,     3,0, 0x0000,0x0000, 0xff,0xd6,0x9a,0xff }, /*     1 */
  {  -116,     3,     3,0, 0x0000,0x0000, 0xd7,0x9b,0x6f,0xff }, /*     2 */
  {  -127,    -9,     2,0, 0x0000,0x0000, 0xff,0xbe,0x88,0xff }, /*     3 */
  {  -113,   -19,     2,0, 0x0000,0x0000, 0xff,0xfb,0xb4,0xff }, /*     4 */
  {  -103,    -5,     3,0, 0x0000,0x0000, 0xff,0xd6,0x9a,0xff }, /*     5 */
  {  -113,   -19,   -24,0, 0x0000,0x0000, 0xab,0x7b,0x59,0xff }, /*     6 */
  {  -127,    -9,   -24,0, 0x0000,0x0000, 0xba,0x86,0x60,0xff }, /*     7 */
  {  -103,    -5,   -19,0, 0x0000,0x0000, 0xc6,0x8f,0x67,0xff }, /*     8 */
  {  -127,    -9,   -24,0, 0x0000,0x0000, 0xba,0x86,0x60,0xff }, /*     9 */
  {  -116,     3,   -19,0, 0x0000,0x0000, 0xd3,0x99,0x6e,0xff }, /*    10 */
  {  -103,    -5,   -19,0, 0x0000,0x0000, 0xc6,0x8f,0x67,0xff }, /*    11 */
  {  -113,   -19,     2,0, 0x0000,0x0000, 0xff,0xfb,0xb4,0xff }, /*    12 */
  {  -113,   -19,   -24,0, 0x0000,0x0000, 0xab,0x7b,0x59,0xff }, /*    13 */
  {  -103,    -5,     3,0, 0x0000,0x0000, 0xff,0xd6,0x9a,0xff }, /*    14 */
  {  -127,    -9,   -24,0, 0x0000,0x0000, 0xba,0x86,0x60,0xff }, /*    15 */
  {  -116,     3,     3,0, 0x0000,0x0000, 0xd7,0x9b,0x6f,0xff }, /*    16 */
  {  -116,     3,   -19,0, 0x0000,0x0000, 0xd3,0x99,0x6e,0xff }, /*    17 */
  {  -127,    -9,   -24,0, 0x0000,0x0000, 0xba,0x86,0x60,0xff }, /*    18 */
  {  -127,    -9,     2,0, 0x0000,0x0000, 0xff,0xbe,0x88,0xff }, /*    19 */
  {  -116,     3,     3,0, 0x0000,0x0000, 0xd7,0x9b,0x6f,0xff }, /*    20 */
  {  -103,    -5,     3,0, 0x0000,0x0000, 0xff,0xd6,0x9a,0xff }, /*    21 */
  {  -103,     7,    -8,0, 0x0000,0x0000, 0xb3,0x82,0x5d,0xff }, /*    22 */
  {  -116,     3,     3,0, 0x0000,0x0000, 0xd7,0x9b,0x6f,0xff }, /*    23 */
  {  -116,     3,     3,0, 0x0000,0x0000, 0xd7,0x9b,0x6f,0xff }, /*    24 */
  {  -103,     7,    -8,0, 0x0000,0x0000, 0xb3,0x82,0x5d,0xff }, /*    25 */
  {  -116,     3,   -19,0, 0x0000,0x0000, 0xd3,0x99,0x6e,0xff }, /*    26 */
  {  -116,     3,   -19,0, 0x0000,0x0000, 0xd3,0x99,0x6e,0xff }, /*    27 */
  {  -103,     7,    -8,0, 0x0000,0x0000, 0xb3,0x82,0x5d,0xff }, /*    28 */
  {  -103,    -5,   -19,0, 0x0000,0x0000, 0xc6,0x8f,0x67,0xff }, /*    29 */
  {  -103,    -5,   -19,0, 0x0000,0x0000, 0xc6,0x8f,0x67,0xff }, /*    30 */
  {  -103,     7,    -8,0, 0x0000,0x0000, 0xb3,0x82,0x5d,0xff }, /*    31 */
  {  -103,    -5,     3,0, 0x0000,0x0000, 0xff,0xd6,0x9a,0xff }, /*    32 */
  {  -113,   -19,   -24,0, 0x0000,0x0000, 0xab,0x7b,0x59,0xff }, /*    33 */
  {  -103,    -5,   -19,0, 0x0000,0x0000, 0xc6,0x8f,0x67,0xff }, /*    34 */
  {  -103,    -5,     3,0, 0x0000,0x0000, 0xff,0xd6,0x9a,0xff }, /*    35 */
  {  -113,   -19,   -24,0, 0x0000,0x0000, 0xab,0x7b,0x59,0xff }, /*    36 */
  {  -124,   -20,    -8,0, 0x0000,0x0000, 0xe5,0xa5,0x77,0xff }, /*    37 */
  {  -127,    -9,   -24,0, 0x0000,0x0000, 0xba,0x86,0x60,0xff }, /*    38 */
  {  -127,    -9,   -24,0, 0x0000,0x0000, 0xba,0x86,0x60,0xff }, /*    39 */
  {  -124,   -20,    -8,0, 0x0000,0x0000, 0xe5,0xa5,0x77,0xff }, /*    40 */
  {  -127,    -9,     2,0, 0x0000,0x0000, 0xff,0xbe,0x88,0xff }, /*    41 */
  {  -127,    -9,     2,0, 0x0000,0x0000, 0xff,0xbe,0x88,0xff }, /*    42 */
  {  -124,   -20,    -8,0, 0x0000,0x0000, 0xe5,0xa5,0x77,0xff }, /*    43 */
  {  -113,   -19,     2,0, 0x0000,0x0000, 0xff,0xfb,0xb4,0xff }, /*    44 */
  {  -113,   -19,     2,0, 0x0000,0x0000, 0xff,0xfb,0xb4,0xff }, /*    45 */
  {  -124,   -20,    -8,0, 0x0000,0x0000, 0xe5,0xa5,0x77,0xff }, /*    46 */
  {  -113,   -19,   -24,0, 0x0000,0x0000, 0xab,0x7b,0x59,0xff }, /*    47 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_318_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 32, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP2Triangles(  6,  7,  8,  0,  9, 10, 11,  0),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP2Triangles( 15, 16, 17,  0, 18, 19, 20,  0),
  gsSP2Triangles( 21, 22, 23,  0, 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 16, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP1Triangle(  1,  2,  3,  0),
  gsSP1Triangle(  4,  5,  6,  0),
  gsSP1Triangle(  7,  8,  9,  0),
  gsSP1Triangle( 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_318_ = {
  gfx_Shape_of___POLYHEDRON_318_,
  vtx_Shape_of___POLYHEDRON_318_, 48, 5,
  bmp_frake_walk,
  tlut_frake_walk,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_walk_aligner5[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_312_[] = {
  {   -81,    69,     9,0, 0x0000,0x0000, 0x77,0xc7,0xc7,0xff }, /*     0 */
  {   -34,    80,     5,0, 0x0000,0x0000, 0xbf,0xff,0xff,0xff }, /*     1 */
  {   -57,    97,     5,0, 0x0000,0x0000, 0xaa,0xff,0xff,0xff }, /*     2 */
  {   -81,    69,     9,0, 0x0000,0x0000, 0x77,0xc7,0xc7,0xff }, /*     3 */
  {   -51,    48,     9,0, 0x0000,0x0000, 0x70,0xba,0xba,0xff }, /*     4 */
  {   -34,    80,     5,0, 0x0000,0x0000, 0xbf,0xff,0xff,0xff }, /*     5 */
  {   -51,    48,   -26,0, 0x0000,0x0000, 0xa4,0xff,0xff,0xff }, /*     6 */
  {   -81,    69,   -26,0, 0x0000,0x0000, 0x90,0xf0,0xf0,0xff }, /*     7 */
  {   -34,    80,   -22,0, 0x0000,0x0000, 0xb0,0xff,0xff,0xff }, /*     8 */
  {   -51,    48,     9,0, 0x0000,0x0000, 0x70,0xba,0xba,0xff }, /*     9 */
  {   -51,    48,   -26,0, 0x0000,0x0000, 0xa4,0xff,0xff,0xff }, /*    10 */
  {   -34,    80,     5,0, 0x0000,0x0000, 0xbf,0xff,0xff,0xff }, /*    11 */
  {   -81,    69,   -26,0, 0x0000,0x0000, 0x90,0xf0,0xf0,0xff }, /*    12 */
  {   -57,    97,     5,0, 0x0000,0x0000, 0xaa,0xff,0xff,0xff }, /*    13 */
  {   -57,    97,   -22,0, 0x0000,0x0000, 0x7a,0xcb,0xcb,0xff }, /*    14 */
  {   -34,    80,     5,0, 0x0000,0x0000, 0xbf,0xff,0xff,0xff }, /*    15 */
  {   -38,    99,    -8,0, 0x0000,0x0000, 0xc1,0xff,0xff,0xff }, /*    16 */
  {   -57,    97,     5,0, 0x0000,0x0000, 0xaa,0xff,0xff,0xff }, /*    17 */
  {   -57,    97,     5,0, 0x0000,0x0000, 0xaa,0xff,0xff,0xff }, /*    18 */
  {   -38,    99,    -8,0, 0x0000,0x0000, 0xc1,0xff,0xff,0xff }, /*    19 */
  {   -57,    97,   -22,0, 0x0000,0x0000, 0x7a,0xcb,0xcb,0xff }, /*    20 */
  {   -57,    97,   -22,0, 0x0000,0x0000, 0x7a,0xcb,0xcb,0xff }, /*    21 */
  {   -38,    99,    -8,0, 0x0000,0x0000, 0xc1,0xff,0xff,0xff }, /*    22 */
  {   -34,    80,   -22,0, 0x0000,0x0000, 0xb0,0xff,0xff,0xff }, /*    23 */
  {   -34,    80,   -22,0, 0x0000,0x0000, 0xb0,0xff,0xff,0xff }, /*    24 */
  {   -38,    99,    -8,0, 0x0000,0x0000, 0xc1,0xff,0xff,0xff }, /*    25 */
  {   -34,    80,     5,0, 0x0000,0x0000, 0xbf,0xff,0xff,0xff }, /*    26 */
  {   -81,    69,   -26,0, 0x0000,0x0000, 0x90,0xf0,0xf0,0xff }, /*    27 */
  {   -81,    69,     9,0, 0x0000,0x0000, 0x77,0xc7,0xc7,0xff }, /*    28 */
  {   -57,    97,     5,0, 0x0000,0x0000, 0xaa,0xff,0xff,0xff }, /*    29 */
  {   -51,    48,   -26,0, 0x0000,0x0000, 0xa4,0xff,0xff,0xff }, /*    30 */
  {   -34,    80,   -22,0, 0x0000,0x0000, 0xb0,0xff,0xff,0xff }, /*    31 */
  {   -34,    80,     5,0, 0x0000,0x0000, 0xbf,0xff,0xff,0xff }, /*    32 */
  {   -81,    69,   -26,0, 0x0000,0x0000, 0x90,0xf0,0xf0,0xff }, /*    33 */
  {   -57,    97,   -22,0, 0x0000,0x0000, 0x7a,0xcb,0xcb,0xff }, /*    34 */
  {   -34,    80,   -22,0, 0x0000,0x0000, 0xb0,0xff,0xff,0xff }, /*    35 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_312_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 32, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP1Triangle(  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 4, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP1Triangle(  1,  2,  3,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_312_ = {
  gfx_Shape_of___POLYHEDRON_312_,
  vtx_Shape_of___POLYHEDRON_312_, 36, 5,
  bmp_frake_walk,
  tlut_frake_walk,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_walk_aligner6[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_306_[] = {
  {    75,    71,     5,0, 0x0000,0x0000, 0xff,0xbc,0x87,0xff }, /*     0 */
  {    52,    54,     5,0, 0x0000,0x0000, 0xb3,0x82,0x5d,0xff }, /*     1 */
  {    92,    45,     4,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*     2 */
  {    52,    54,     5,0, 0x0000,0x0000, 0xb3,0x82,0x5d,0xff }, /*     3 */
  {    71,    30,     4,0, 0x0000,0x0000, 0xde,0xa0,0x73,0xff }, /*     4 */
  {    92,    45,     4,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*     5 */
  {    52,    54,   -22,0, 0x0000,0x0000, 0xfb,0xb6,0x82,0xff }, /*     6 */
  {    92,    45,   -20,0, 0x0000,0x0000, 0xe6,0xa6,0x77,0xff }, /*     7 */
  {    71,    30,   -20,0, 0x0000,0x0000, 0xc4,0x8e,0x66,0xff }, /*     8 */
  {    52,    54,     5,0, 0x0000,0x0000, 0xb3,0x82,0x5d,0xff }, /*     9 */
  {    71,    30,   -20,0, 0x0000,0x0000, 0xc4,0x8e,0x66,0xff }, /*    10 */
  {    71,    30,     4,0, 0x0000,0x0000, 0xde,0xa0,0x73,0xff }, /*    11 */
  {    75,    71,   -22,0, 0x0000,0x0000, 0xff,0xe4,0xa3,0xff }, /*    12 */
  {    75,    71,     5,0, 0x0000,0x0000, 0xff,0xbc,0x87,0xff }, /*    13 */
  {    92,    45,   -20,0, 0x0000,0x0000, 0xe6,0xa6,0x77,0xff }, /*    14 */
  {    75,    71,     5,0, 0x0000,0x0000, 0xff,0xbc,0x87,0xff }, /*    15 */
  {    56,    73,    -8,0, 0x0000,0x0000, 0xff,0xc8,0x90,0xff }, /*    16 */
  {    52,    54,     5,0, 0x0000,0x0000, 0xb3,0x82,0x5d,0xff }, /*    17 */
  {    75,    71,   -22,0, 0x0000,0x0000, 0xff,0xe4,0xa3,0xff }, /*    18 */
  {    56,    73,    -8,0, 0x0000,0x0000, 0xff,0xc8,0x90,0xff }, /*    19 */
  {    75,    71,     5,0, 0x0000,0x0000, 0xff,0xbc,0x87,0xff }, /*    20 */
  {    52,    54,   -22,0, 0x0000,0x0000, 0xfb,0xb6,0x82,0xff }, /*    21 */
  {    56,    73,    -8,0, 0x0000,0x0000, 0xff,0xc8,0x90,0xff }, /*    22 */
  {    75,    71,   -22,0, 0x0000,0x0000, 0xff,0xe4,0xa3,0xff }, /*    23 */
  {    52,    54,     5,0, 0x0000,0x0000, 0xb3,0x82,0x5d,0xff }, /*    24 */
  {    56,    73,    -8,0, 0x0000,0x0000, 0xff,0xc8,0x90,0xff }, /*    25 */
  {    52,    54,   -22,0, 0x0000,0x0000, 0xfb,0xb6,0x82,0xff }, /*    26 */
  {    75,    71,     5,0, 0x0000,0x0000, 0xff,0xbc,0x87,0xff }, /*    27 */
  {    92,    45,     4,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*    28 */
  {    92,    45,   -20,0, 0x0000,0x0000, 0xe6,0xa6,0x77,0xff }, /*    29 */
  {    52,    54,     5,0, 0x0000,0x0000, 0xb3,0x82,0x5d,0xff }, /*    30 */
  {    52,    54,   -22,0, 0x0000,0x0000, 0xfb,0xb6,0x82,0xff }, /*    31 */
  {    71,    30,   -20,0, 0x0000,0x0000, 0xc4,0x8e,0x66,0xff }, /*    32 */
  {    52,    54,   -22,0, 0x0000,0x0000, 0xfb,0xb6,0x82,0xff }, /*    33 */
  {    75,    71,   -22,0, 0x0000,0x0000, 0xff,0xe4,0xa3,0xff }, /*    34 */
  {    92,    45,   -20,0, 0x0000,0x0000, 0xe6,0xa6,0x77,0xff }, /*    35 */
  {    92,    45,   -20,0, 0x0000,0x0000, 0xe6,0xa6,0x77,0xff }, /*    36 */
  {    88,    29,    -8,0, 0x0000,0x0000, 0xf0,0x68,0x4b,0xff }, /*    37 */
  {    71,    30,   -20,0, 0x0000,0x0000, 0xc4,0x8e,0x66,0xff }, /*    38 */
  {    92,    45,     4,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*    39 */
  {    88,    29,    -8,0, 0x0000,0x0000, 0xf0,0x68,0x4b,0xff }, /*    40 */
  {    92,    45,   -20,0, 0x0000,0x0000, 0xe6,0xa6,0x77,0xff }, /*    41 */
  {    71,    30,     4,0, 0x0000,0x0000, 0xde,0xa0,0x73,0xff }, /*    42 */
  {    88,    29,    -8,0, 0x0000,0x0000, 0xf0,0x68,0x4b,0xff }, /*    43 */
  {    92,    45,     4,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*    44 */
  {    71,    30,   -20,0, 0x0000,0x0000, 0xc4,0x8e,0x66,0xff }, /*    45 */
  {    88,    29,    -8,0, 0x0000,0x0000, 0xf0,0x68,0x4b,0xff }, /*    46 */
  {    71,    30,     4,0, 0x0000,0x0000, 0xde,0xa0,0x73,0xff }, /*    47 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_306_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 32, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP1Triangle(  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 16, 0 ),
  gsSP2Triangles( 30, 31,  0,  0,  1,  2,  3,  0),
  gsSP1Triangle(  4,  5,  6,  0),
  gsSP1Triangle(  7,  8,  9,  0),
  gsSP1Triangle( 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_306_ = {
  gfx_Shape_of___POLYHEDRON_306_,
  vtx_Shape_of___POLYHEDRON_306_, 48, 5,
  bmp_frake_walk,
  tlut_frake_walk,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_walk_aligner7[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_308_[] = {
  {    92,    42,     3,0, 0x0000,0x0000, 0xff,0xce,0x94,0xff }, /*     0 */
  {    74,    29,     3,0, 0x0000,0x0000, 0xd1,0x97,0x6c,0xff }, /*     1 */
  {   110,    15,    -1,0, 0x0000,0x0000, 0xff,0xc7,0x8f,0xff }, /*     2 */
  {    74,    29,     3,0, 0x0000,0x0000, 0xd1,0x97,0x6c,0xff }, /*     3 */
  {    94,     3,    -1,0, 0x0000,0x0000, 0xce,0x95,0x6b,0xff }, /*     4 */
  {   110,    15,    -1,0, 0x0000,0x0000, 0xff,0xc7,0x8f,0xff }, /*     5 */
  {    74,    29,   -19,0, 0x0000,0x0000, 0xe2,0xa3,0x75,0xff }, /*     6 */
  {   110,    15,   -15,0, 0x0000,0x0000, 0xef,0xad,0x7c,0xff }, /*     7 */
  {    94,     3,   -15,0, 0x0000,0x0000, 0xc4,0x8e,0x66,0xff }, /*     8 */
  {    74,    29,   -19,0, 0x0000,0x0000, 0xe2,0xa3,0x75,0xff }, /*     9 */
  {    92,    42,   -19,0, 0x0000,0x0000, 0xff,0xc7,0x8f,0xff }, /*    10 */
  {   110,    15,   -15,0, 0x0000,0x0000, 0xef,0xad,0x7c,0xff }, /*    11 */
  {    74,    29,     3,0, 0x0000,0x0000, 0xd1,0x97,0x6c,0xff }, /*    12 */
  {    94,     3,   -15,0, 0x0000,0x0000, 0xc4,0x8e,0x66,0xff }, /*    13 */
  {    94,     3,    -1,0, 0x0000,0x0000, 0xce,0x95,0x6b,0xff }, /*    14 */
  {    92,    42,   -19,0, 0x0000,0x0000, 0xff,0xc7,0x8f,0xff }, /*    15 */
  {    92,    42,     3,0, 0x0000,0x0000, 0xff,0xce,0x94,0xff }, /*    16 */
  {   110,    15,   -15,0, 0x0000,0x0000, 0xef,0xad,0x7c,0xff }, /*    17 */
  {    92,    42,     3,0, 0x0000,0x0000, 0xff,0xce,0x94,0xff }, /*    18 */
  {   110,    15,    -1,0, 0x0000,0x0000, 0xff,0xc7,0x8f,0xff }, /*    19 */
  {   110,    15,   -15,0, 0x0000,0x0000, 0xef,0xad,0x7c,0xff }, /*    20 */
  {    92,    42,     3,0, 0x0000,0x0000, 0xff,0xce,0x94,0xff }, /*    21 */
  {    77,    44,    -8,0, 0x0000,0x0000, 0xff,0xbe,0x88,0xff }, /*    22 */
  {    74,    29,     3,0, 0x0000,0x0000, 0xd1,0x97,0x6c,0xff }, /*    23 */
  {    92,    42,   -19,0, 0x0000,0x0000, 0xff,0xc7,0x8f,0xff }, /*    24 */
  {    77,    44,    -8,0, 0x0000,0x0000, 0xff,0xbe,0x88,0xff }, /*    25 */
  {    92,    42,     3,0, 0x0000,0x0000, 0xff,0xce,0x94,0xff }, /*    26 */
  {    74,    29,   -19,0, 0x0000,0x0000, 0xe2,0xa3,0x75,0xff }, /*    27 */
  {    77,    44,    -8,0, 0x0000,0x0000, 0xff,0xbe,0x88,0xff }, /*    28 */
  {    92,    42,   -19,0, 0x0000,0x0000, 0xff,0xc7,0x8f,0xff }, /*    29 */
  {    74,    29,     3,0, 0x0000,0x0000, 0xd1,0x97,0x6c,0xff }, /*    30 */
  {    77,    44,    -8,0, 0x0000,0x0000, 0xff,0xbe,0x88,0xff }, /*    31 */
  {    74,    29,   -19,0, 0x0000,0x0000, 0xe2,0xa3,0x75,0xff }, /*    32 */
  {    74,    29,     3,0, 0x0000,0x0000, 0xd1,0x97,0x6c,0xff }, /*    33 */
  {    74,    29,   -19,0, 0x0000,0x0000, 0xe2,0xa3,0x75,0xff }, /*    34 */
  {    94,     3,   -15,0, 0x0000,0x0000, 0xc4,0x8e,0x66,0xff }, /*    35 */
  {   110,    15,   -15,0, 0x0000,0x0000, 0xef,0xad,0x7c,0xff }, /*    36 */
  {   107,     2,    -8,0, 0x0000,0x0000, 0xd4,0x99,0x6e,0xff }, /*    37 */
  {    94,     3,   -15,0, 0x0000,0x0000, 0xc4,0x8e,0x66,0xff }, /*    38 */
  {   110,    15,    -1,0, 0x0000,0x0000, 0xff,0xc7,0x8f,0xff }, /*    39 */
  {   107,     2,    -8,0, 0x0000,0x0000, 0xd4,0x99,0x6e,0xff }, /*    40 */
  {   110,    15,   -15,0, 0x0000,0x0000, 0xef,0xad,0x7c,0xff }, /*    41 */
  {    94,     3,    -1,0, 0x0000,0x0000, 0xce,0x95,0x6b,0xff }, /*    42 */
  {   107,     2,    -8,0, 0x0000,0x0000, 0xd4,0x99,0x6e,0xff }, /*    43 */
  {   110,    15,    -1,0, 0x0000,0x0000, 0xff,0xc7,0x8f,0xff }, /*    44 */
  {    94,     3,   -15,0, 0x0000,0x0000, 0xc4,0x8e,0x66,0xff }, /*    45 */
  {   107,     2,    -8,0, 0x0000,0x0000, 0xd4,0x99,0x6e,0xff }, /*    46 */
  {    94,     3,    -1,0, 0x0000,0x0000, 0xce,0x95,0x6b,0xff }, /*    47 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_308_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 32, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP2Triangles(  6,  7,  8,  0,  9, 10, 11,  0),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP2Triangles( 15, 16, 17,  0, 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 16, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP1Triangle(  1,  2,  3,  0),
  gsSP1Triangle(  4,  5,  6,  0),
  gsSP1Triangle(  7,  8,  9,  0),
  gsSP1Triangle( 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_308_ = {
  gfx_Shape_of___POLYHEDRON_308_,
  vtx_Shape_of___POLYHEDRON_308_, 48, 5,
  bmp_frake_walk,
  tlut_frake_walk,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_walk_aligner8[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_310_[] = {
  {   116,     3,     3,0, 0x0000,0x0000, 0xff,0xcd,0x93,0xff }, /*     0 */
  {   103,    -5,     3,0, 0x0000,0x0000, 0xd5,0x9a,0x6f,0xff }, /*     1 */
  {   127,    -9,     2,0, 0x0000,0x0000, 0xff,0xc4,0x8d,0xff }, /*     2 */
  {   103,    -5,     3,0, 0x0000,0x0000, 0xd5,0x9a,0x6f,0xff }, /*     3 */
  {   113,   -19,     2,0, 0x0000,0x0000, 0xc8,0x91,0x68,0xff }, /*     4 */
  {   127,    -9,     2,0, 0x0000,0x0000, 0xff,0xc4,0x8d,0xff }, /*     5 */
  {   103,    -5,   -19,0, 0x0000,0x0000, 0xee,0xac,0x7c,0xff }, /*     6 */
  {   127,    -9,   -24,0, 0x0000,0x0000, 0xf1,0xae,0x7d,0xff }, /*     7 */
  {   113,   -19,   -24,0, 0x0000,0x0000, 0xb7,0x84,0x5f,0xff }, /*     8 */
  {   103,    -5,   -19,0, 0x0000,0x0000, 0xee,0xac,0x7c,0xff }, /*     9 */
  {   116,     3,   -19,0, 0x0000,0x0000, 0xff,0xd5,0x99,0xff }, /*    10 */
  {   127,    -9,   -24,0, 0x0000,0x0000, 0xf1,0xae,0x7d,0xff }, /*    11 */
  {   103,    -5,     3,0, 0x0000,0x0000, 0xd5,0x9a,0x6f,0xff }, /*    12 */
  {   113,   -19,   -24,0, 0x0000,0x0000, 0xb7,0x84,0x5f,0xff }, /*    13 */
  {   113,   -19,     2,0, 0x0000,0x0000, 0xc8,0x91,0x68,0xff }, /*    14 */
  {   116,     3,   -19,0, 0x0000,0x0000, 0xff,0xd5,0x99,0xff }, /*    15 */
  {   116,     3,     3,0, 0x0000,0x0000, 0xff,0xcd,0x93,0xff }, /*    16 */
  {   127,    -9,   -24,0, 0x0000,0x0000, 0xf1,0xae,0x7d,0xff }, /*    17 */
  {   116,     3,     3,0, 0x0000,0x0000, 0xff,0xcd,0x93,0xff }, /*    18 */
  {   127,    -9,     2,0, 0x0000,0x0000, 0xff,0xc4,0x8d,0xff }, /*    19 */
  {   127,    -9,   -24,0, 0x0000,0x0000, 0xf1,0xae,0x7d,0xff }, /*    20 */
  {   116,     3,     3,0, 0x0000,0x0000, 0xff,0xcd,0x93,0xff }, /*    21 */
  {   103,     7,    -8,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    22 */
  {   103,    -5,     3,0, 0x0000,0x0000, 0xd5,0x9a,0x6f,0xff }, /*    23 */
  {   116,     3,   -19,0, 0x0000,0x0000, 0xff,0xd5,0x99,0xff }, /*    24 */
  {   103,     7,    -8,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    25 */
  {   116,     3,     3,0, 0x0000,0x0000, 0xff,0xcd,0x93,0xff }, /*    26 */
  {   103,    -5,   -19,0, 0x0000,0x0000, 0xee,0xac,0x7c,0xff }, /*    27 */
  {   103,     7,    -8,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    28 */
  {   116,     3,   -19,0, 0x0000,0x0000, 0xff,0xd5,0x99,0xff }, /*    29 */
  {   103,    -5,     3,0, 0x0000,0x0000, 0xd5,0x9a,0x6f,0xff }, /*    30 */
  {   103,     7,    -8,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    31 */
  {   103,    -5,   -19,0, 0x0000,0x0000, 0xee,0xac,0x7c,0xff }, /*    32 */
  {   103,    -5,     3,0, 0x0000,0x0000, 0xd5,0x9a,0x6f,0xff }, /*    33 */
  {   103,    -5,   -19,0, 0x0000,0x0000, 0xee,0xac,0x7c,0xff }, /*    34 */
  {   113,   -19,   -24,0, 0x0000,0x0000, 0xb7,0x84,0x5f,0xff }, /*    35 */
  {   127,    -9,   -24,0, 0x0000,0x0000, 0xf1,0xae,0x7d,0xff }, /*    36 */
  {   124,   -20,    -8,0, 0x0000,0x0000, 0xcb,0x93,0x69,0xff }, /*    37 */
  {   113,   -19,   -24,0, 0x0000,0x0000, 0xb7,0x84,0x5f,0xff }, /*    38 */
  {   127,    -9,     2,0, 0x0000,0x0000, 0xff,0xc4,0x8d,0xff }, /*    39 */
  {   124,   -20,    -8,0, 0x0000,0x0000, 0xcb,0x93,0x69,0xff }, /*    40 */
  {   127,    -9,   -24,0, 0x0000,0x0000, 0xf1,0xae,0x7d,0xff }, /*    41 */
  {   113,   -19,     2,0, 0x0000,0x0000, 0xc8,0x91,0x68,0xff }, /*    42 */
  {   124,   -20,    -8,0, 0x0000,0x0000, 0xcb,0x93,0x69,0xff }, /*    43 */
  {   127,    -9,     2,0, 0x0000,0x0000, 0xff,0xc4,0x8d,0xff }, /*    44 */
  {   113,   -19,   -24,0, 0x0000,0x0000, 0xb7,0x84,0x5f,0xff }, /*    45 */
  {   124,   -20,    -8,0, 0x0000,0x0000, 0xcb,0x93,0x69,0xff }, /*    46 */
  {   113,   -19,     2,0, 0x0000,0x0000, 0xc8,0x91,0x68,0xff }, /*    47 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_310_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 32, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP2Triangles(  6,  7,  8,  0,  9, 10, 11,  0),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP2Triangles( 15, 16, 17,  0, 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 16, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP1Triangle(  1,  2,  3,  0),
  gsSP1Triangle(  4,  5,  6,  0),
  gsSP1Triangle(  7,  8,  9,  0),
  gsSP1Triangle( 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_310_ = {
  gfx_Shape_of___POLYHEDRON_310_,
  vtx_Shape_of___POLYHEDRON_310_, 48, 5,
  bmp_frake_walk,
  tlut_frake_walk,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_walk_aligner9[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_304_[] = {
  {    57,    97,     5,0, 0x0000,0x0000, 0x97,0xfc,0xfc,0xff }, /*     0 */
  {    34,    80,     5,0, 0x0000,0x0000, 0x6d,0xb6,0xb6,0xff }, /*     1 */
  {    81,    69,     9,0, 0x0000,0x0000, 0xa4,0xff,0xff,0xff }, /*     2 */
  {    34,    80,     5,0, 0x0000,0x0000, 0x6d,0xb6,0xb6,0xff }, /*     3 */
  {    51,    48,     9,0, 0x0000,0x0000, 0x88,0xe4,0xe4,0xff }, /*     4 */
  {    81,    69,     9,0, 0x0000,0x0000, 0xa4,0xff,0xff,0xff }, /*     5 */
  {    34,    80,   -22,0, 0x0000,0x0000, 0x96,0xfb,0xfb,0xff }, /*     6 */
  {    81,    69,   -26,0, 0x0000,0x0000, 0x8e,0xed,0xed,0xff }, /*     7 */
  {    51,    48,   -26,0, 0x0000,0x0000, 0x74,0xc2,0xc2,0xff }, /*     8 */
  {    34,    80,     5,0, 0x0000,0x0000, 0x6d,0xb6,0xb6,0xff }, /*     9 */
  {    51,    48,   -26,0, 0x0000,0x0000, 0x74,0xc2,0xc2,0xff }, /*    10 */
  {    51,    48,     9,0, 0x0000,0x0000, 0x88,0xe4,0xe4,0xff }, /*    11 */
  {    57,    97,   -22,0, 0x0000,0x0000, 0xbe,0xff,0xff,0xff }, /*    12 */
  {    57,    97,     5,0, 0x0000,0x0000, 0x97,0xfc,0xfc,0xff }, /*    13 */
  {    81,    69,   -26,0, 0x0000,0x0000, 0x8e,0xed,0xed,0xff }, /*    14 */
  {    57,    97,     5,0, 0x0000,0x0000, 0x97,0xfc,0xfc,0xff }, /*    15 */
  {    81,    69,     9,0, 0x0000,0x0000, 0xa4,0xff,0xff,0xff }, /*    16 */
  {    81,    69,   -26,0, 0x0000,0x0000, 0x8e,0xed,0xed,0xff }, /*    17 */
  {    57,    97,     5,0, 0x0000,0x0000, 0x97,0xfc,0xfc,0xff }, /*    18 */
  {    38,    99,    -8,0, 0x0000,0x0000, 0xa9,0xff,0xff,0xff }, /*    19 */
  {    34,    80,     5,0, 0x0000,0x0000, 0x6d,0xb6,0xb6,0xff }, /*    20 */
  {    57,    97,   -22,0, 0x0000,0x0000, 0xbe,0xff,0xff,0xff }, /*    21 */
  {    38,    99,    -8,0, 0x0000,0x0000, 0xa9,0xff,0xff,0xff }, /*    22 */
  {    57,    97,     5,0, 0x0000,0x0000, 0x97,0xfc,0xfc,0xff }, /*    23 */
  {    34,    80,   -22,0, 0x0000,0x0000, 0x96,0xfb,0xfb,0xff }, /*    24 */
  {    38,    99,    -8,0, 0x0000,0x0000, 0xa9,0xff,0xff,0xff }, /*    25 */
  {    57,    97,   -22,0, 0x0000,0x0000, 0xbe,0xff,0xff,0xff }, /*    26 */
  {    34,    80,     5,0, 0x0000,0x0000, 0x6d,0xb6,0xb6,0xff }, /*    27 */
  {    38,    99,    -8,0, 0x0000,0x0000, 0xa9,0xff,0xff,0xff }, /*    28 */
  {    34,    80,   -22,0, 0x0000,0x0000, 0x96,0xfb,0xfb,0xff }, /*    29 */
  {    34,    80,     5,0, 0x0000,0x0000, 0x6d,0xb6,0xb6,0xff }, /*    30 */
  {    34,    80,   -22,0, 0x0000,0x0000, 0x96,0xfb,0xfb,0xff }, /*    31 */
  {    51,    48,   -26,0, 0x0000,0x0000, 0x74,0xc2,0xc2,0xff }, /*    32 */
  {    34,    80,   -22,0, 0x0000,0x0000, 0x96,0xfb,0xfb,0xff }, /*    33 */
  {    57,    97,   -22,0, 0x0000,0x0000, 0xbe,0xff,0xff,0xff }, /*    34 */
  {    81,    69,   -26,0, 0x0000,0x0000, 0x8e,0xed,0xed,0xff }, /*    35 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_304_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 32, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP1Triangle(  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsSP2Triangles( 12, 13, 14,  0, 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 4, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP1Triangle(  1,  2,  3,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_304_ = {
  gfx_Shape_of___POLYHEDRON_304_,
  vtx_Shape_of___POLYHEDRON_304_, 36, 5,
  bmp_frake_walk,
  tlut_frake_walk,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_walk_aligner10[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_264_[] = {
  {    57,   188,     0,0, 0x0000,0x0000, 0x00,0x44,0x88,0xff }, /*     0 */
  {    36,   132,   -52,0, 0x0000,0x0000, 0x00,0x54,0xa8,0xff }, /*     1 */
  {    51,   178,   -65,0, 0x0000,0x0000, 0x00,0x4d,0x9b,0xff }, /*     2 */
  {   -65,   160,    12,0, 0x0000,0x0000, 0x00,0x61,0xc2,0xff }, /*     3 */
  {   -66,   138,    24,0, 0x0000,0x0000, 0x00,0x00,0x52,0xff }, /*     4 */
  {   -71,   160,    11,0, 0x0000,0x0000, 0x00,0x4b,0x97,0xff }, /*     5 */
  {   -57,   188,     0,0, 0x0000,0x0000, 0x00,0x4b,0x96,0xff }, /*     6 */
  {   -65,   160,    12,0, 0x0000,0x0000, 0x00,0x61,0xc2,0xff }, /*     7 */
  {   -71,   160,    11,0, 0x0000,0x0000, 0x00,0x4b,0x97,0xff }, /*     8 */
  {    51,   178,   -65,0, 0x0000,0x0000, 0x00,0x4d,0x9b,0xff }, /*     9 */
  {     0,   132,   -74,0, 0x0000,0x0000, 0x00,0x46,0x8d,0xff }, /*    10 */
  {     0,   168,   -91,0, 0x0000,0x0000, 0x00,0x4a,0x94,0xff }, /*    11 */
  {     0,   168,   -91,0, 0x0000,0x0000, 0x00,0x4a,0x94,0xff }, /*    12 */
  {     0,   132,   -74,0, 0x0000,0x0000, 0x00,0x46,0x8d,0xff }, /*    13 */
  {   -51,   178,   -65,0, 0x0000,0x0000, 0x00,0x46,0x8c,0xff }, /*    14 */
  {   -51,   178,   -65,0, 0x0000,0x0000, 0x00,0x46,0x8c,0xff }, /*    15 */
  {   -36,   132,   -52,0, 0x0000,0x0000, 0x00,0x46,0x8c,0xff }, /*    16 */
  {   -57,   188,     0,0, 0x0000,0x0000, 0x00,0x4b,0x96,0xff }, /*    17 */
  {   -57,   188,     0,0, 0x0000,0x0000, 0xf2,0xaf,0x7d,0xff }, /*    18 */
  {   -52,   132,     0,0, 0x0000,0x0000, 0xdb,0x9e,0x72,0xff }, /*    19 */
  {   -36,   132,    52,0, 0x0000,0x0000, 0xcb,0x93,0x6a,0xff }, /*    20 */
  {    57,   188,     0,0, 0x0000,0x0000, 0xdb,0x9e,0x72,0xff }, /*    21 */
  {    42,   170,    65,0, 0x0000,0x0000, 0xf0,0x71,0x4b,0xff }, /*    22 */
  {    36,   132,    52,0, 0x0000,0x0000, 0xb7,0x84,0x5f,0xff }, /*    23 */
  {    52,   132,     0,0, 0x0000,0x0000, 0xff,0xbf,0x89,0xff }, /*    24 */
  {    15,   114,     0,0, 0x0000,0x0000, 0xe9,0xa9,0x79,0xff }, /*    25 */
  {    36,   132,   -52,0, 0x0000,0x0000, 0xff,0xc4,0x8d,0xff }, /*    26 */
  {   -36,   132,   -52,0, 0x0000,0x0000, 0xe1,0xa3,0x75,0xff }, /*    27 */
  {   -11,   114,   -20,0, 0x0000,0x0000, 0xe4,0xa5,0x76,0xff }, /*    28 */
  {   -15,   114,     0,0, 0x0000,0x0000, 0xdd,0xa0,0x73,0xff }, /*    29 */
  {    36,   132,   -52,0, 0x0000,0x0000, 0xff,0xc4,0x8d,0xff }, /*    30 */
  {    11,   114,   -20,0, 0x0000,0x0000, 0xff,0xc9,0x90,0xff }, /*    31 */
  {     0,   132,   -74,0, 0x0000,0x0000, 0xe3,0xa4,0x76,0xff }, /*    32 */
  {     0,   132,   -74,0, 0x0000,0x0000, 0xe3,0xa4,0x76,0xff }, /*    33 */
  {   -11,   114,   -20,0, 0x0000,0x0000, 0xe4,0xa5,0x76,0xff }, /*    34 */
  {   -36,   132,   -52,0, 0x0000,0x0000, 0xe1,0xa3,0x75,0xff }, /*    35 */
  {   -36,   132,    52,0, 0x0000,0x0000, 0xcb,0x93,0x6a,0xff }, /*    36 */
  {   -18,   114,    20,0, 0x0000,0x0000, 0xe0,0xa2,0x74,0xff }, /*    37 */
  {     0,   114,    28,0, 0x0000,0x0000, 0xe0,0xa2,0x74,0xff }, /*    38 */
  {   -36,   132,   -52,0, 0x0000,0x0000, 0xe1,0xa3,0x75,0xff }, /*    39 */
  {   -15,   114,     0,0, 0x0000,0x0000, 0xdd,0xa0,0x73,0xff }, /*    40 */
  {   -52,   132,     0,0, 0x0000,0x0000, 0xdb,0x9e,0x72,0xff }, /*    41 */
  {   -52,   132,     0,0, 0x0000,0x0000, 0xdb,0x9e,0x72,0xff }, /*    42 */
  {   -15,   114,     0,0, 0x0000,0x0000, 0xdd,0xa0,0x73,0xff }, /*    43 */
  {   -36,   132,    52,0, 0x0000,0x0000, 0xcb,0x93,0x6a,0xff }, /*    44 */
  {     0,   114,    28,0, 0x0000,0x0000, 0xe0,0xa2,0x74,0xff }, /*    45 */
  {    18,   114,    20,0, 0x0000,0x0000, 0xdb,0x9e,0x72,0xff }, /*    46 */
  {    36,   132,    52,0, 0x0000,0x0000, 0xb7,0x84,0x5f,0xff }, /*    47 */
  {    36,   132,    52,0, 0x0000,0x0000, 0xb7,0x84,0x5f,0xff }, /*    48 */
  {    15,   114,     0,0, 0x0000,0x0000, 0xe9,0xa9,0x79,0xff }, /*    49 */
  {    52,   132,     0,0, 0x0000,0x0000, 0xff,0xbf,0x89,0xff }, /*    50 */
  {    53,   178,   -80,0, 0x0000,0x0000, 0x4f,0x00,0x9f,0xff }, /*    51 */
  {    57,   201,   -84,0, 0x0000,0x0000, 0x49,0x00,0x92,0xff }, /*    52 */
  {    76,   188,     0,0, 0x0000,0x0000, 0x57,0x00,0xae,0xff }, /*    53 */
  {    73,   214,     0,0, 0x0000,0x0000, 0x5d,0x00,0xbb,0xff }, /*    54 */
  {    51,   229,    48,0, 0x0000,0x0000, 0x4f,0x00,0x9f,0xff }, /*    55 */
  {    64,   204,    47,0, 0x0000,0x0000, 0x50,0x00,0xa0,0xff }, /*    56 */
  {    64,   204,    47,0, 0x0000,0x0000, 0x50,0x00,0xa0,0xff }, /*    57 */
  {    76,   188,     0,0, 0x0000,0x0000, 0x57,0x00,0xae,0xff }, /*    58 */
  {    73,   214,     0,0, 0x0000,0x0000, 0x5d,0x00,0xbb,0xff }, /*    59 */
  {    64,   204,    47,0, 0x0000,0x0000, 0x50,0x00,0xa0,0xff }, /*    60 */
  {    51,   229,    48,0, 0x0000,0x0000, 0x4f,0x00,0x9f,0xff }, /*    61 */
  {     0,   210,    68,0, 0x0000,0x0000, 0x38,0x00,0x70,0xff }, /*    62 */
  {     0,   210,    68,0, 0x0000,0x0000, 0x38,0x00,0x70,0xff }, /*    63 */
  {   -51,   229,    48,0, 0x0000,0x0000, 0x3d,0x00,0x7b,0xff }, /*    64 */
  {   -64,   204,    47,0, 0x0000,0x0000, 0x3f,0x00,0x7f,0xff }, /*    65 */
  {   -64,   204,    47,0, 0x0000,0x0000, 0x3f,0x00,0x7f,0xff }, /*    66 */
  {   -51,   229,    48,0, 0x0000,0x0000, 0x3d,0x00,0x7b,0xff }, /*    67 */
  {   -73,   214,     0,0, 0x0000,0x0000, 0x3d,0x00,0x7a,0xff }, /*    68 */
  {   -76,   188,     0,0, 0x0000,0x0000, 0x45,0x00,0x8a,0xff }, /*    69 */
  {   -57,   201,   -84,0, 0x0000,0x0000, 0x47,0x00,0x8f,0xff }, /*    70 */
  {   -53,   178,   -80,0, 0x0000,0x0000, 0x4a,0x00,0x94,0xff }, /*    71 */
  {   -53,   178,   -80,0, 0x0000,0x0000, 0x4a,0x00,0x94,0xff }, /*    72 */
  {     0,   193,  -119,0, 0x0000,0x0000, 0x45,0x00,0x8a,0xff }, /*    73 */
  {     0,   168,  -112,0, 0x0000,0x0000, 0x42,0x00,0x85,0xff }, /*    74 */
  {     0,   168,  -112,0, 0x0000,0x0000, 0x42,0x00,0x85,0xff }, /*    75 */
  {     0,   193,  -119,0, 0x0000,0x0000, 0x45,0x00,0x8a,0xff }, /*    76 */
  {    53,   178,   -80,0, 0x0000,0x0000, 0x4f,0x00,0x9f,0xff }, /*    77 */
  {    36,   132,    52,0, 0x0000,0x0000, 0xb7,0x84,0x5f,0xff }, /*    78 */
  {    52,   132,     0,0, 0x0000,0x0000, 0xff,0xbf,0x89,0xff }, /*    79 */
  {    57,   188,     0,0, 0x0000,0x0000, 0xdb,0x9e,0x72,0xff }, /*    80 */
  {   -36,   132,    52,0, 0x0000,0x0000, 0xcb,0x93,0x6a,0xff }, /*    81 */
  {   -42,   170,    65,0, 0x0000,0x0000, 0xc6,0x5e,0x3e,0xff }, /*    82 */
  {   -57,   188,     0,0, 0x0000,0x0000, 0xf2,0xaf,0x7d,0xff }, /*    83 */
  {     0,   114,    28,0, 0x0000,0x0000, 0xe0,0xa2,0x74,0xff }, /*    84 */
  {    36,   132,    52,0, 0x0000,0x0000, 0xb7,0x84,0x5f,0xff }, /*    85 */
  {   -36,   132,    52,0, 0x0000,0x0000, 0xcb,0x93,0x6a,0xff }, /*    86 */
  {   -57,   188,     0,0, 0x0000,0x0000, 0x00,0x4b,0x96,0xff }, /*    87 */
  {   -71,   160,    11,0, 0x0000,0x0000, 0x00,0x4b,0x97,0xff }, /*    88 */
  {   -76,   188,     0,0, 0x0000,0x0000, 0x00,0x45,0x8a,0xff }, /*    89 */
  {   -76,   188,     0,0, 0x0000,0x0000, 0x00,0x45,0x8a,0xff }, /*    90 */
  {   -64,   174,    43,0, 0x0000,0x0000, 0x00,0x37,0x6e,0xff }, /*    91 */
  {   -64,   204,    47,0, 0x0000,0x0000, 0x00,0x3f,0x7f,0xff }, /*    92 */
  {   -64,   204,    47,0, 0x0000,0x0000, 0x00,0x3f,0x7f,0xff }, /*    93 */
  {   -56,   174,    44,0, 0x0000,0x0000, 0x00,0x3a,0x75,0xff }, /*    94 */
  {   -42,   204,    49,0, 0x0000,0x0000, 0x00,0x3d,0x7a,0xff }, /*    95 */
  {   -71,   160,    11,0, 0x0000,0x0000, 0x00,0x4b,0x97,0xff }, /*    96 */
  {   -66,   138,    24,0, 0x0000,0x0000, 0x00,0x00,0x52,0xff }, /*    97 */
  {   -64,   174,    43,0, 0x0000,0x0000, 0x00,0x37,0x6e,0xff }, /*    98 */
  {   -64,   174,    43,0, 0x0000,0x0000, 0x00,0x37,0x6e,0xff }, /*    99 */
  {   -66,   138,    24,0, 0x0000,0x0000, 0x00,0x00,0x52,0xff }, /*   100 */
  {   -56,   174,    44,0, 0x0000,0x0000, 0x00,0x3a,0x75,0xff }, /*   101 */
  {    42,   204,    49,0, 0x0000,0x0000, 0x00,0x36,0x6c,0xff }, /*   102 */
  {    56,   174,    44,0, 0x0000,0x0000, 0x00,0x3c,0x78,0xff }, /*   103 */
  {    64,   204,    47,0, 0x0000,0x0000, 0x00,0x50,0xa0,0xff }, /*   104 */
  {    56,   174,    44,0, 0x0000,0x0000, 0x00,0x3c,0x78,0xff }, /*   105 */
  {    64,   174,    43,0, 0x0000,0x0000, 0x00,0x44,0x89,0xff }, /*   106 */
  {    64,   204,    47,0, 0x0000,0x0000, 0x00,0x50,0xa0,0xff }, /*   107 */
  {    64,   204,    47,0, 0x0000,0x0000, 0x00,0x50,0xa0,0xff }, /*   108 */
  {    64,   174,    43,0, 0x0000,0x0000, 0x00,0x44,0x89,0xff }, /*   109 */
  {    76,   188,     0,0, 0x0000,0x0000, 0x00,0x57,0xae,0xff }, /*   110 */
  {   -51,   178,   -65,0, 0x0000,0x0000, 0x00,0x46,0x8c,0xff }, /*   111 */
  {   -54,   149,   -72,0, 0x0000,0x0000, 0x00,0x4d,0x9a,0xff }, /*   112 */
  {   -53,   178,   -80,0, 0x0000,0x0000, 0x00,0x4a,0x94,0xff }, /*   113 */
  {    76,   188,     0,0, 0x0000,0x0000, 0x00,0x57,0xae,0xff }, /*   114 */
  {    71,   160,    11,0, 0x0000,0x0000, 0x00,0x58,0xb1,0xff }, /*   115 */
  {    57,   188,     0,0, 0x0000,0x0000, 0x00,0x44,0x88,0xff }, /*   116 */
  {    54,   149,   -72,0, 0x0000,0x0000, 0x00,0x44,0x88,0xff }, /*   117 */
  {    57,   115,   -36,0, 0x0000,0x0000, 0x00,0x00,0x52,0xff }, /*   118 */
  {    54,   149,   -60,0, 0x0000,0x0000, 0x00,0x4a,0x94,0xff }, /*   119 */
  {    53,   178,   -80,0, 0x0000,0x0000, 0x00,0x4f,0x9f,0xff }, /*   120 */
  {     4,   140,  -108,0, 0x0000,0x0000, 0x00,0x3b,0x77,0xff }, /*   121 */
  {     0,   168,  -112,0, 0x0000,0x0000, 0x00,0x42,0x85,0xff }, /*   122 */
  {    54,   149,   -72,0, 0x0000,0x0000, 0x00,0x44,0x88,0xff }, /*   123 */
  {    54,   149,   -60,0, 0x0000,0x0000, 0x00,0x4a,0x94,0xff }, /*   124 */
  {    51,   178,   -65,0, 0x0000,0x0000, 0x00,0x4d,0x9b,0xff }, /*   125 */
  {    56,   174,    44,0, 0x0000,0x0000, 0x00,0x3c,0x78,0xff }, /*   126 */
  {    66,   138,    24,0, 0x0000,0x0000, 0x00,0x00,0x52,0xff }, /*   127 */
  {    64,   174,    43,0, 0x0000,0x0000, 0x00,0x44,0x89,0xff }, /*   128 */
  {    64,   174,    43,0, 0x0000,0x0000, 0x00,0x44,0x89,0xff }, /*   129 */
  {    66,   138,    24,0, 0x0000,0x0000, 0x00,0x00,0x52,0xff }, /*   130 */
  {    71,   160,    11,0, 0x0000,0x0000, 0x00,0x58,0xb1,0xff }, /*   131 */
  {    51,   178,   -65,0, 0x0000,0x0000, 0x00,0x4d,0x9b,0xff }, /*   132 */
  {    47,   148,   -68,0, 0x0000,0x0000, 0x00,0x44,0x88,0xff }, /*   133 */
  {    53,   178,   -80,0, 0x0000,0x0000, 0x00,0x4f,0x9f,0xff }, /*   134 */
  {    64,   174,    43,0, 0x0000,0x0000, 0x00,0x44,0x89,0xff }, /*   135 */
  {    71,   160,    11,0, 0x0000,0x0000, 0x00,0x58,0xb1,0xff }, /*   136 */
  {    76,   188,     0,0, 0x0000,0x0000, 0x00,0x57,0xae,0xff }, /*   137 */
  {    71,   160,    11,0, 0x0000,0x0000, 0x00,0x58,0xb1,0xff }, /*   138 */
  {    66,   138,    24,0, 0x0000,0x0000, 0x00,0x00,0x52,0xff }, /*   139 */
  {    65,   160,    12,0, 0x0000,0x0000, 0x00,0x4b,0x96,0xff }, /*   140 */
  {    57,   188,     0,0, 0x0000,0x0000, 0x00,0x44,0x88,0xff }, /*   141 */
  {    65,   158,    -6,0, 0x0000,0x0000, 0x00,0x3a,0x75,0xff }, /*   142 */
  {    76,   188,     0,0, 0x0000,0x0000, 0x00,0x57,0xae,0xff }, /*   143 */
  {     0,   168,  -112,0, 0x0000,0x0000, 0x00,0x42,0x85,0xff }, /*   144 */
  {    -4,   140,  -108,0, 0x0000,0x0000, 0x00,0x53,0xa7,0xff }, /*   145 */
  {   -53,   178,   -80,0, 0x0000,0x0000, 0x00,0x4a,0x94,0xff }, /*   146 */
  {    76,   188,     0,0, 0x0000,0x0000, 0x00,0x57,0xae,0xff }, /*   147 */
  {    73,   158,    -6,0, 0x0000,0x0000, 0x00,0x49,0x92,0xff }, /*   148 */
  {    53,   178,   -80,0, 0x0000,0x0000, 0x00,0x4f,0x9f,0xff }, /*   149 */
  {    53,   178,   -80,0, 0x0000,0x0000, 0x00,0x4f,0x9f,0xff }, /*   150 */
  {    54,   149,   -72,0, 0x0000,0x0000, 0x00,0x44,0x88,0xff }, /*   151 */
  {    51,   178,   -65,0, 0x0000,0x0000, 0x00,0x4d,0x9b,0xff }, /*   152 */
  {    65,   158,    -6,0, 0x0000,0x0000, 0x00,0x3a,0x75,0xff }, /*   153 */
  {    57,   115,   -36,0, 0x0000,0x0000, 0x00,0x00,0x52,0xff }, /*   154 */
  {    73,   158,    -6,0, 0x0000,0x0000, 0x00,0x49,0x92,0xff }, /*   155 */
  {    73,   158,    -6,0, 0x0000,0x0000, 0x00,0x49,0x92,0xff }, /*   156 */
  {    57,   115,   -36,0, 0x0000,0x0000, 0x00,0x00,0x52,0xff }, /*   157 */
  {    54,   149,   -72,0, 0x0000,0x0000, 0x00,0x44,0x88,0xff }, /*   158 */
  {   -53,   178,   -80,0, 0x0000,0x0000, 0x00,0x4a,0x94,0xff }, /*   159 */
  {   -73,   158,    -6,0, 0x0000,0x0000, 0x00,0x3d,0x7a,0xff }, /*   160 */
  {   -76,   188,     0,0, 0x0000,0x0000, 0x00,0x45,0x8a,0xff }, /*   161 */
  {     0,   168,  -112,0, 0x0000,0x0000, 0x00,0x42,0x85,0xff }, /*   162 */
  {     4,   140,  -108,0, 0x0000,0x0000, 0x00,0x3b,0x77,0xff }, /*   163 */
  {     0,   168,   -91,0, 0x0000,0x0000, 0x00,0x4a,0x94,0xff }, /*   164 */
  {     4,   140,  -108,0, 0x0000,0x0000, 0x00,0x3b,0x77,0xff }, /*   165 */
  {     4,   140,   -91,0, 0x0000,0x0000, 0x00,0x3f,0x7e,0xff }, /*   166 */
  {     0,   168,   -91,0, 0x0000,0x0000, 0x00,0x4a,0x94,0xff }, /*   167 */
  {   -53,   178,   -80,0, 0x0000,0x0000, 0x00,0x4a,0x94,0xff }, /*   168 */
  {   -54,   149,   -72,0, 0x0000,0x0000, 0x00,0x4d,0x9a,0xff }, /*   169 */
  {   -73,   158,    -6,0, 0x0000,0x0000, 0x00,0x3d,0x7a,0xff }, /*   170 */
  {   -76,   188,     0,0, 0x0000,0x0000, 0x00,0x45,0x8a,0xff }, /*   171 */
  {   -65,   158,    -6,0, 0x0000,0x0000, 0x00,0x3d,0x7b,0xff }, /*   172 */
  {   -57,   188,     0,0, 0x0000,0x0000, 0x00,0x4b,0x96,0xff }, /*   173 */
  {   -53,   178,   -80,0, 0x0000,0x0000, 0x00,0x4a,0x94,0xff }, /*   174 */
  {   -56,   148,   -81,0, 0x0000,0x0000, 0x00,0x4a,0x95,0xff }, /*   175 */
  {   -47,   148,   -68,0, 0x0000,0x0000, 0x00,0x41,0x82,0xff }, /*   176 */
  {   -76,   188,     0,0, 0x0000,0x0000, 0x00,0x45,0x8a,0xff }, /*   177 */
  {   -73,   158,    -6,0, 0x0000,0x0000, 0x00,0x3d,0x7a,0xff }, /*   178 */
  {   -65,   158,    -6,0, 0x0000,0x0000, 0x00,0x3d,0x7b,0xff }, /*   179 */
  {   -54,   149,   -72,0, 0x0000,0x0000, 0x00,0x4d,0x9a,0xff }, /*   180 */
  {   -57,   115,   -36,0, 0x0000,0x0000, 0x00,0x00,0x4c,0xff }, /*   181 */
  {   -73,   158,    -6,0, 0x0000,0x0000, 0x00,0x3d,0x7a,0xff }, /*   182 */
  {   -73,   158,    -6,0, 0x0000,0x0000, 0x00,0x3d,0x7a,0xff }, /*   183 */
  {   -57,   115,   -36,0, 0x0000,0x0000, 0x00,0x00,0x4c,0xff }, /*   184 */
  {   -65,   158,    -6,0, 0x0000,0x0000, 0x00,0x3d,0x7b,0xff }, /*   185 */
  {     0,   132,   -74,0, 0x0000,0x0000, 0x00,0x46,0x8d,0xff }, /*   186 */
  {   -36,   132,   -52,0, 0x0000,0x0000, 0x00,0x46,0x8c,0xff }, /*   187 */
  {   -51,   178,   -65,0, 0x0000,0x0000, 0x00,0x46,0x8c,0xff }, /*   188 */
  {   -54,   149,   -60,0, 0x0000,0x0000, 0x00,0x62,0xc4,0xff }, /*   189 */
  {   -57,   115,   -36,0, 0x0000,0x0000, 0x00,0x00,0x4c,0xff }, /*   190 */
  {   -54,   149,   -72,0, 0x0000,0x0000, 0x00,0x4d,0x9a,0xff }, /*   191 */
  {   -51,   178,   -65,0, 0x0000,0x0000, 0x00,0x46,0x8c,0xff }, /*   192 */
  {   -54,   149,   -60,0, 0x0000,0x0000, 0x00,0x62,0xc4,0xff }, /*   193 */
  {   -54,   149,   -72,0, 0x0000,0x0000, 0x00,0x4d,0x9a,0xff }, /*   194 */
  {    47,   148,   -68,0, 0x0000,0x0000, 0x00,0x44,0x88,0xff }, /*   195 */
  {    28,   113,   -63,0, 0x0000,0x0000, 0x00,0x00,0x57,0xff }, /*   196 */
  {    56,   148,   -81,0, 0x0000,0x0000, 0x00,0x4f,0x9e,0xff }, /*   197 */
  {    56,   148,   -81,0, 0x0000,0x0000, 0x00,0x4f,0x9e,0xff }, /*   198 */
  {    28,   113,   -63,0, 0x0000,0x0000, 0x00,0x00,0x57,0xff }, /*   199 */
  {     4,   140,  -108,0, 0x0000,0x0000, 0x00,0x3b,0x77,0xff }, /*   200 */
  {     4,   140,  -108,0, 0x0000,0x0000, 0x00,0x3b,0x77,0xff }, /*   201 */
  {    28,   113,   -63,0, 0x0000,0x0000, 0x00,0x00,0x57,0xff }, /*   202 */
  {     4,   140,   -91,0, 0x0000,0x0000, 0x00,0x3f,0x7e,0xff }, /*   203 */
  {    53,   178,   -80,0, 0x0000,0x0000, 0x00,0x4f,0x9f,0xff }, /*   204 */
  {    56,   148,   -81,0, 0x0000,0x0000, 0x00,0x4f,0x9e,0xff }, /*   205 */
  {     4,   140,  -108,0, 0x0000,0x0000, 0x00,0x3b,0x77,0xff }, /*   206 */
  {     0,   168,   -91,0, 0x0000,0x0000, 0x00,0x4a,0x94,0xff }, /*   207 */
  {    -4,   140,  -108,0, 0x0000,0x0000, 0x00,0x53,0xa7,0xff }, /*   208 */
  {     0,   168,  -112,0, 0x0000,0x0000, 0x00,0x42,0x85,0xff }, /*   209 */
  {     0,   168,   -91,0, 0x0000,0x0000, 0x00,0x4a,0x94,0xff }, /*   210 */
  {    -4,   140,   -91,0, 0x0000,0x0000, 0x00,0x57,0xae,0xff }, /*   211 */
  {    -4,   140,  -108,0, 0x0000,0x0000, 0x00,0x53,0xa7,0xff }, /*   212 */
  {   -53,   178,   -80,0, 0x0000,0x0000, 0x00,0x4a,0x94,0xff }, /*   213 */
  {   -47,   148,   -68,0, 0x0000,0x0000, 0x00,0x41,0x82,0xff }, /*   214 */
  {   -51,   178,   -65,0, 0x0000,0x0000, 0x00,0x46,0x8c,0xff }, /*   215 */
  {    -4,   140,  -108,0, 0x0000,0x0000, 0x00,0x53,0xa7,0xff }, /*   216 */
  {   -28,   113,   -63,0, 0x0000,0x0000, 0x00,0x00,0x55,0xff }, /*   217 */
  {   -56,   148,   -81,0, 0x0000,0x0000, 0x00,0x4a,0x95,0xff }, /*   218 */
  {    -4,   140,   -91,0, 0x0000,0x0000, 0x00,0x57,0xae,0xff }, /*   219 */
  {   -28,   113,   -63,0, 0x0000,0x0000, 0x00,0x00,0x55,0xff }, /*   220 */
  {    -4,   140,  -108,0, 0x0000,0x0000, 0x00,0x53,0xa7,0xff }, /*   221 */
  {    73,   158,    -6,0, 0x0000,0x0000, 0x00,0x49,0x92,0xff }, /*   222 */
  {    54,   149,   -72,0, 0x0000,0x0000, 0x00,0x44,0x88,0xff }, /*   223 */
  {    53,   178,   -80,0, 0x0000,0x0000, 0x00,0x4f,0x9f,0xff }, /*   224 */
  {   -56,   148,   -81,0, 0x0000,0x0000, 0x00,0x4a,0x95,0xff }, /*   225 */
  {   -28,   113,   -63,0, 0x0000,0x0000, 0x00,0x00,0x55,0xff }, /*   226 */
  {   -47,   148,   -68,0, 0x0000,0x0000, 0x00,0x41,0x82,0xff }, /*   227 */
  {    42,   170,    65,0, 0x0000,0x0000, 0xf0,0x71,0x4b,0xff }, /*   228 */
  {    57,   188,     0,0, 0x0000,0x0000, 0xdb,0x9e,0x72,0xff }, /*   229 */
  {    42,   204,    49,0, 0x0000,0x0000, 0xae,0x7d,0x5a,0xff }, /*   230 */
  {   -42,   170,    65,0, 0x0000,0x0000, 0xc6,0x5e,0x3e,0xff }, /*   231 */
  {   -42,   204,    49,0, 0x0000,0x0000, 0xc5,0x8e,0x66,0xff }, /*   232 */
  {   -57,   188,     0,0, 0x0000,0x0000, 0xf2,0xaf,0x7d,0xff }, /*   233 */
  {   -73,   214,     0,0, 0x0000,0x0000, 0x3d,0x00,0x7a,0xff }, /*   234 */
  {   -76,   188,     0,0, 0x0000,0x0000, 0x45,0x00,0x8a,0xff }, /*   235 */
  {   -64,   204,    47,0, 0x0000,0x0000, 0x3f,0x00,0x7f,0xff }, /*   236 */
  {     0,   275,    27,0, 0x0000,0x0000, 0xff,0xc7,0x00,0xff }, /*   237 */
  {    -6,   247,    66,0, 0x0000,0x0000, 0xd7,0xa0,0x00,0xff }, /*   238 */
  {     0,   236,    62,0, 0x0000,0x0000, 0xbf,0x8e,0x00,0xff }, /*   239 */
  {     0,   275,    27,0, 0x0000,0x0000, 0xff,0xc7,0x00,0xff }, /*   240 */
  {    -6,   279,    37,0, 0x0000,0x0000, 0xff,0xd3,0x00,0xff }, /*   241 */
  {    -6,   247,    66,0, 0x0000,0x0000, 0xd7,0xa0,0x00,0xff }, /*   242 */
  {     0,   236,    62,0, 0x0000,0x0000, 0xbf,0x8e,0x00,0xff }, /*   243 */
  {   -50,   241,    55,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   244 */
  {   -51,   229,    48,0, 0x0000,0x0000, 0xcd,0x99,0x00,0xff }, /*   245 */
  {     0,   236,    62,0, 0x0000,0x0000, 0xbf,0x8e,0x00,0xff }, /*   246 */
  {    -6,   247,    66,0, 0x0000,0x0000, 0xd7,0xa0,0x00,0xff }, /*   247 */
  {   -50,   241,    55,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   248 */
  {   -51,   229,    48,0, 0x0000,0x0000, 0xcd,0x99,0x00,0xff }, /*   249 */
  {   -59,   273,    28,0, 0x0000,0x0000, 0xe0,0xa7,0x00,0xff }, /*   250 */
  {   -59,   267,    16,0, 0x0000,0x0000, 0xc4,0x92,0x00,0xff }, /*   251 */
  {   -59,   267,    16,0, 0x0000,0x0000, 0xc4,0x92,0x00,0xff }, /*   252 */
  {    -6,   279,    37,0, 0x0000,0x0000, 0xff,0xd3,0x00,0xff }, /*   253 */
  {     0,   275,    27,0, 0x0000,0x0000, 0xff,0xc7,0x00,0xff }, /*   254 */
  {    51,   229,    48,0, 0x0000,0x0000, 0xff,0xc6,0x00,0xff }, /*   255 */
  {    50,   241,    55,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   256 */
  {     0,   236,    62,0, 0x0000,0x0000, 0xbf,0x8e,0x00,0xff }, /*   257 */
  {     0,   236,    62,0, 0x0000,0x0000, 0xbf,0x8e,0x00,0xff }, /*   258 */
  {     6,   247,    66,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   259 */
  {     0,   275,    27,0, 0x0000,0x0000, 0xff,0xc7,0x00,0xff }, /*   260 */
  {     0,   275,    27,0, 0x0000,0x0000, 0xff,0xc7,0x00,0xff }, /*   261 */
  {     6,   279,    37,0, 0x0000,0x0000, 0xff,0xc1,0x00,0xff }, /*   262 */
  {    59,   267,    16,0, 0x0000,0x0000, 0xff,0xe7,0x00,0xff }, /*   263 */
  {    59,   267,    16,0, 0x0000,0x0000, 0xff,0xe7,0x00,0xff }, /*   264 */
  {    59,   273,    28,0, 0x0000,0x0000, 0xff,0xf0,0x00,0xff }, /*   265 */
  {    51,   229,    48,0, 0x0000,0x0000, 0xff,0xc6,0x00,0xff }, /*   266 */
  {    59,   273,    28,0, 0x0000,0x0000, 0xff,0xf0,0x00,0xff }, /*   267 */
  {    50,   241,    55,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   268 */
  {    51,   229,    48,0, 0x0000,0x0000, 0xff,0xc6,0x00,0xff }, /*   269 */
  {     6,   279,    37,0, 0x0000,0x0000, 0xff,0xc1,0x00,0xff }, /*   270 */
  {    59,   273,    28,0, 0x0000,0x0000, 0xff,0xf0,0x00,0xff }, /*   271 */
  {    59,   267,    16,0, 0x0000,0x0000, 0xff,0xe7,0x00,0xff }, /*   272 */
  {     6,   247,    66,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   273 */
  {     6,   279,    37,0, 0x0000,0x0000, 0xff,0xc1,0x00,0xff }, /*   274 */
  {     0,   275,    27,0, 0x0000,0x0000, 0xff,0xc7,0x00,0xff }, /*   275 */
  {    50,   241,    55,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   276 */
  {     6,   247,    66,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   277 */
  {     0,   236,    62,0, 0x0000,0x0000, 0xbf,0x8e,0x00,0xff }, /*   278 */
  {   -59,   267,    16,0, 0x0000,0x0000, 0xc4,0x92,0x00,0xff }, /*   279 */
  {   -59,   273,    28,0, 0x0000,0x0000, 0xe0,0xa7,0x00,0xff }, /*   280 */
  {    -6,   279,    37,0, 0x0000,0x0000, 0xff,0xd3,0x00,0xff }, /*   281 */
  {   -51,   229,    48,0, 0x0000,0x0000, 0xcd,0x99,0x00,0xff }, /*   282 */
  {   -50,   241,    55,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   283 */
  {   -59,   273,    28,0, 0x0000,0x0000, 0xe0,0xa7,0x00,0xff }, /*   284 */
  {    -4,   140,  -108,0, 0x0000,0x0000, 0x00,0x53,0xa7,0xff }, /*   285 */
  {   -56,   148,   -81,0, 0x0000,0x0000, 0x00,0x4a,0x95,0xff }, /*   286 */
  {   -53,   178,   -80,0, 0x0000,0x0000, 0x00,0x4a,0x94,0xff }, /*   287 */
  {    47,   148,   -68,0, 0x0000,0x0000, 0x00,0x44,0x88,0xff }, /*   288 */
  {    56,   148,   -81,0, 0x0000,0x0000, 0x00,0x4f,0x9e,0xff }, /*   289 */
  {    53,   178,   -80,0, 0x0000,0x0000, 0x00,0x4f,0x9f,0xff }, /*   290 */
  {    65,   158,    -6,0, 0x0000,0x0000, 0x00,0x3a,0x75,0xff }, /*   291 */
  {    73,   158,    -6,0, 0x0000,0x0000, 0x00,0x49,0x92,0xff }, /*   292 */
  {    76,   188,     0,0, 0x0000,0x0000, 0x00,0x57,0xae,0xff }, /*   293 */
  {    71,   160,    11,0, 0x0000,0x0000, 0x00,0x58,0xb1,0xff }, /*   294 */
  {    65,   160,    12,0, 0x0000,0x0000, 0x00,0x4b,0x96,0xff }, /*   295 */
  {    57,   188,     0,0, 0x0000,0x0000, 0x00,0x44,0x88,0xff }, /*   296 */
  {   -64,   204,    47,0, 0x0000,0x0000, 0x00,0x3f,0x7f,0xff }, /*   297 */
  {   -64,   174,    43,0, 0x0000,0x0000, 0x00,0x37,0x6e,0xff }, /*   298 */
  {   -56,   174,    44,0, 0x0000,0x0000, 0x00,0x3a,0x75,0xff }, /*   299 */
  {   -76,   188,     0,0, 0x0000,0x0000, 0x00,0x45,0x8a,0xff }, /*   300 */
  {   -71,   160,    11,0, 0x0000,0x0000, 0x00,0x4b,0x97,0xff }, /*   301 */
  {   -64,   174,    43,0, 0x0000,0x0000, 0x00,0x37,0x6e,0xff }, /*   302 */
  {     0,   193,  -119,0, 0x0000,0x0000, 0x45,0x00,0x8a,0xff }, /*   303 */
  {    57,   201,   -84,0, 0x0000,0x0000, 0x49,0x00,0x92,0xff }, /*   304 */
  {    53,   178,   -80,0, 0x0000,0x0000, 0x4f,0x00,0x9f,0xff }, /*   305 */
  {   -53,   178,   -80,0, 0x0000,0x0000, 0x4a,0x00,0x94,0xff }, /*   306 */
  {   -57,   201,   -84,0, 0x0000,0x0000, 0x47,0x00,0x8f,0xff }, /*   307 */
  {     0,   193,  -119,0, 0x0000,0x0000, 0x45,0x00,0x8a,0xff }, /*   308 */
  {   -76,   188,     0,0, 0x0000,0x0000, 0x45,0x00,0x8a,0xff }, /*   309 */
  {   -73,   214,     0,0, 0x0000,0x0000, 0x3d,0x00,0x7a,0xff }, /*   310 */
  {   -57,   201,   -84,0, 0x0000,0x0000, 0x47,0x00,0x8f,0xff }, /*   311 */
  {     0,   210,    68,0, 0x0000,0x0000, 0x38,0x00,0x70,0xff }, /*   312 */
  {     0,   236,    62,0, 0x0000,0x0000, 0x39,0x00,0x72,0xff }, /*   313 */
  {   -51,   229,    48,0, 0x0000,0x0000, 0x3d,0x00,0x7b,0xff }, /*   314 */
  {    51,   229,    48,0, 0x0000,0x0000, 0x4f,0x00,0x9f,0xff }, /*   315 */
  {     0,   236,    62,0, 0x0000,0x0000, 0x39,0x00,0x72,0xff }, /*   316 */
  {     0,   210,    68,0, 0x0000,0x0000, 0x38,0x00,0x70,0xff }, /*   317 */
  {    57,   201,   -84,0, 0x0000,0x0000, 0x49,0x00,0x92,0xff }, /*   318 */
  {    73,   214,     0,0, 0x0000,0x0000, 0x5d,0x00,0xbb,0xff }, /*   319 */
  {    76,   188,     0,0, 0x0000,0x0000, 0x57,0x00,0xae,0xff }, /*   320 */
  {    15,   114,     0,0, 0x0000,0x0000, 0xe9,0xa9,0x79,0xff }, /*   321 */
  {    18,   114,    20,0, 0x0000,0x0000, 0xdb,0x9e,0x72,0xff }, /*   322 */
  {   -18,   114,    20,0, 0x0000,0x0000, 0xe0,0xa2,0x74,0xff }, /*   323 */
  {    15,   114,     0,0, 0x0000,0x0000, 0xe9,0xa9,0x79,0xff }, /*   324 */
  {   -18,   114,    20,0, 0x0000,0x0000, 0xe0,0xa2,0x74,0xff }, /*   325 */
  {   -15,   114,     0,0, 0x0000,0x0000, 0xdd,0xa0,0x73,0xff }, /*   326 */
  {    18,   114,    20,0, 0x0000,0x0000, 0xdb,0x9e,0x72,0xff }, /*   327 */
  {     0,   114,    28,0, 0x0000,0x0000, 0xe0,0xa2,0x74,0xff }, /*   328 */
  {   -18,   114,    20,0, 0x0000,0x0000, 0xe0,0xa2,0x74,0xff }, /*   329 */
  {    36,   132,    52,0, 0x0000,0x0000, 0xb7,0x84,0x5f,0xff }, /*   330 */
  {    18,   114,    20,0, 0x0000,0x0000, 0xdb,0x9e,0x72,0xff }, /*   331 */
  {    15,   114,     0,0, 0x0000,0x0000, 0xe9,0xa9,0x79,0xff }, /*   332 */
  {   -15,   114,     0,0, 0x0000,0x0000, 0xdd,0xa0,0x73,0xff }, /*   333 */
  {   -18,   114,    20,0, 0x0000,0x0000, 0xe0,0xa2,0x74,0xff }, /*   334 */
  {   -36,   132,    52,0, 0x0000,0x0000, 0xcb,0x93,0x6a,0xff }, /*   335 */
  {     0,   132,   -74,0, 0x0000,0x0000, 0xe3,0xa4,0x76,0xff }, /*   336 */
  {     0,   114,   -28,0, 0x0000,0x0000, 0xe7,0xa7,0x78,0xff }, /*   337 */
  {   -11,   114,   -20,0, 0x0000,0x0000, 0xe4,0xa5,0x76,0xff }, /*   338 */
  {    11,   114,   -20,0, 0x0000,0x0000, 0xff,0xc9,0x90,0xff }, /*   339 */
  {     0,   114,   -28,0, 0x0000,0x0000, 0xe7,0xa7,0x78,0xff }, /*   340 */
  {     0,   132,   -74,0, 0x0000,0x0000, 0xe3,0xa4,0x76,0xff }, /*   341 */
  {    15,   114,     0,0, 0x0000,0x0000, 0xe9,0xa9,0x79,0xff }, /*   342 */
  {    11,   114,   -20,0, 0x0000,0x0000, 0xff,0xc9,0x90,0xff }, /*   343 */
  {    36,   132,   -52,0, 0x0000,0x0000, 0xff,0xc4,0x8d,0xff }, /*   344 */
  {   -36,   132,   -52,0, 0x0000,0x0000, 0x00,0x46,0x8c,0xff }, /*   345 */
  {   -52,   132,     0,0, 0x0000,0x0000, 0x00,0x44,0x88,0xff }, /*   346 */
  {   -57,   188,     0,0, 0x0000,0x0000, 0x00,0x4b,0x96,0xff }, /*   347 */
  {    51,   178,   -65,0, 0x0000,0x0000, 0x00,0x4d,0x9b,0xff }, /*   348 */
  {    36,   132,   -52,0, 0x0000,0x0000, 0x00,0x54,0xa8,0xff }, /*   349 */
  {     0,   132,   -74,0, 0x0000,0x0000, 0x00,0x46,0x8d,0xff }, /*   350 */
  {    57,   188,     0,0, 0x0000,0x0000, 0x00,0x44,0x88,0xff }, /*   351 */
  {    52,   132,     0,0, 0x0000,0x0000, 0x00,0x52,0xa4,0xff }, /*   352 */
  {    36,   132,   -52,0, 0x0000,0x0000, 0x00,0x54,0xa8,0xff }, /*   353 */
  {     6,   247,    66,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   354 */
  {     9,   247,    60,0, 0x0000,0x0000, 0xff,0xe7,0x00,0xff }, /*   355 */
  {     6,   279,    37,0, 0x0000,0x0000, 0xff,0xc1,0x00,0xff }, /*   356 */
  {     6,   279,    37,0, 0x0000,0x0000, 0xff,0xc1,0x00,0xff }, /*   357 */
  {    53,   268,    29,0, 0x0000,0x0000, 0xef,0xb2,0x00,0xff }, /*   358 */
  {    59,   273,    28,0, 0x0000,0x0000, 0xff,0xf0,0x00,0xff }, /*   359 */
  {    59,   273,    28,0, 0x0000,0x0000, 0xff,0xf0,0x00,0xff }, /*   360 */
  {    53,   268,    29,0, 0x0000,0x0000, 0xef,0xb2,0x00,0xff }, /*   361 */
  {    50,   241,    55,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   362 */
  {    50,   241,    55,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   363 */
  {     9,   247,    60,0, 0x0000,0x0000, 0xff,0xe7,0x00,0xff }, /*   364 */
  {     6,   247,    66,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   365 */
  {    -6,   279,    37,0, 0x0000,0x0000, 0xff,0xd3,0x00,0xff }, /*   366 */
  {    -9,   247,    60,0, 0x0000,0x0000, 0xf9,0xb9,0x00,0xff }, /*   367 */
  {    -6,   247,    66,0, 0x0000,0x0000, 0xd7,0xa0,0x00,0xff }, /*   368 */
  {    -6,   247,    66,0, 0x0000,0x0000, 0xd7,0xa0,0x00,0xff }, /*   369 */
  {    -9,   247,    60,0, 0x0000,0x0000, 0xf9,0xb9,0x00,0xff }, /*   370 */
  {   -50,   241,    55,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   371 */
  {   -50,   241,    55,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   372 */
  {   -53,   268,    29,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   373 */
  {   -59,   273,    28,0, 0x0000,0x0000, 0xe0,0xa7,0x00,0xff }, /*   374 */
  {   -59,   273,    28,0, 0x0000,0x0000, 0xe0,0xa7,0x00,0xff }, /*   375 */
  {   -53,   268,    29,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   376 */
  {    -6,   279,    37,0, 0x0000,0x0000, 0xff,0xd3,0x00,0xff }, /*   377 */
  {   -53,   268,    29,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   378 */
  {    -9,   273,    37,0, 0x0000,0x0000, 0xf3,0xb5,0x00,0xff }, /*   379 */
  {    -6,   279,    37,0, 0x0000,0x0000, 0xff,0xd3,0x00,0xff }, /*   380 */
  {   -50,   241,    55,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   381 */
  {   -46,   242,    50,0, 0x0000,0x0000, 0xff,0xcd,0x00,0xff }, /*   382 */
  {   -53,   268,    29,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   383 */
  {    -9,   247,    60,0, 0x0000,0x0000, 0xf9,0xb9,0x00,0xff }, /*   384 */
  {   -46,   242,    50,0, 0x0000,0x0000, 0xff,0xcd,0x00,0xff }, /*   385 */
  {   -50,   241,    55,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   386 */
  {    -6,   279,    37,0, 0x0000,0x0000, 0xff,0xd3,0x00,0xff }, /*   387 */
  {    -9,   273,    37,0, 0x0000,0x0000, 0xf3,0xb5,0x00,0xff }, /*   388 */
  {    -9,   247,    60,0, 0x0000,0x0000, 0xf9,0xb9,0x00,0xff }, /*   389 */
  {    50,   241,    55,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   390 */
  {    46,   242,    50,0, 0x0000,0x0000, 0xff,0xcb,0x00,0xff }, /*   391 */
  {     9,   247,    60,0, 0x0000,0x0000, 0xff,0xe7,0x00,0xff }, /*   392 */
  {    53,   268,    29,0, 0x0000,0x0000, 0xef,0xb2,0x00,0xff }, /*   393 */
  {    46,   242,    50,0, 0x0000,0x0000, 0xff,0xcb,0x00,0xff }, /*   394 */
  {    50,   241,    55,0, 0x0000,0x0000, 0xe7,0xac,0x00,0xff }, /*   395 */
  {     6,   279,    37,0, 0x0000,0x0000, 0xff,0xc1,0x00,0xff }, /*   396 */
  {     9,   273,    37,0, 0x0000,0x0000, 0xef,0xb2,0x00,0xff }, /*   397 */
  {    53,   268,    29,0, 0x0000,0x0000, 0xef,0xb2,0x00,0xff }, /*   398 */
  {     9,   247,    60,0, 0x0000,0x0000, 0xff,0xe7,0x00,0xff }, /*   399 */
  {     9,   273,    37,0, 0x0000,0x0000, 0xef,0xb2,0x00,0xff }, /*   400 */
  {     6,   279,    37,0, 0x0000,0x0000, 0xff,0xc1,0x00,0xff }, /*   401 */
  {   -15,   114,     0,0, 0x0000,0x0000, 0xdd,0xa0,0x73,0xff }, /*   402 */
  {    15,    90,     0,0, 0x0000,0x0000, 0xd0,0x97,0x6c,0xff }, /*   403 */
  {    15,   114,     0,0, 0x0000,0x0000, 0xe9,0xa9,0x79,0xff }, /*   404 */
  {    15,   114,     0,0, 0x0000,0x0000, 0xe9,0xa9,0x79,0xff }, /*   405 */
  {    11,    90,   -20,0, 0x0000,0x0000, 0xff,0xc4,0x8d,0xff }, /*   406 */
  {    11,   114,   -20,0, 0x0000,0x0000, 0xff,0xc9,0x90,0xff }, /*   407 */
  {    15,   114,     0,0, 0x0000,0x0000, 0xe9,0xa9,0x79,0xff }, /*   408 */
  {    15,    90,     0,0, 0x0000,0x0000, 0xd0,0x97,0x6c,0xff }, /*   409 */
  {    11,    90,   -20,0, 0x0000,0x0000, 0xff,0xc4,0x8d,0xff }, /*   410 */
  {    11,   114,   -20,0, 0x0000,0x0000, 0xff,0xc9,0x90,0xff }, /*   411 */
  {     0,    90,   -28,0, 0x0000,0x0000, 0xe6,0xa6,0x77,0xff }, /*   412 */
  {     0,   114,   -28,0, 0x0000,0x0000, 0xe7,0xa7,0x78,0xff }, /*   413 */
  {    11,   114,   -20,0, 0x0000,0x0000, 0xff,0xc9,0x90,0xff }, /*   414 */
  {    11,    90,   -20,0, 0x0000,0x0000, 0xff,0xc4,0x8d,0xff }, /*   415 */
  {     0,    90,   -28,0, 0x0000,0x0000, 0xe6,0xa6,0x77,0xff }, /*   416 */
  {     0,   114,   -28,0, 0x0000,0x0000, 0xe7,0xa7,0x78,0xff }, /*   417 */
  {   -11,    90,   -20,0, 0x0000,0x0000, 0xe0,0xa2,0x74,0xff }, /*   418 */
  {   -11,   114,   -20,0, 0x0000,0x0000, 0xe4,0xa5,0x76,0xff }, /*   419 */
  {     0,   114,   -28,0, 0x0000,0x0000, 0xe7,0xa7,0x78,0xff }, /*   420 */
  {     0,    90,   -28,0, 0x0000,0x0000, 0xe6,0xa6,0x77,0xff }, /*   421 */
  {   -11,    90,   -20,0, 0x0000,0x0000, 0xe0,0xa2,0x74,0xff }, /*   422 */
  {   -11,   114,   -20,0, 0x0000,0x0000, 0xe4,0xa5,0x76,0xff }, /*   423 */
  {   -15,    90,     0,0, 0x0000,0x0000, 0xca,0x92,0x69,0xff }, /*   424 */
  {   -15,   114,     0,0, 0x0000,0x0000, 0xdd,0xa0,0x73,0xff }, /*   425 */
  {   -11,   114,   -20,0, 0x0000,0x0000, 0xe4,0xa5,0x76,0xff }, /*   426 */
  {   -11,    90,   -20,0, 0x0000,0x0000, 0xe0,0xa2,0x74,0xff }, /*   427 */
  {   -15,    90,     0,0, 0x0000,0x0000, 0xca,0x92,0x69,0xff }, /*   428 */
  {   -15,   114,     0,0, 0x0000,0x0000, 0xdd,0xa0,0x73,0xff }, /*   429 */
  {   -15,    90,     0,0, 0x0000,0x0000, 0xca,0x92,0x69,0xff }, /*   430 */
  {    15,    90,     0,0, 0x0000,0x0000, 0xd0,0x97,0x6c,0xff }, /*   431 */
  {    57,   201,   -84,0, 0x06d4,0x01f1, 0xdb,0xdb,0xdb,0xff }, /*   432 */
  {    23,   300,   -84,0, 0x064d,0x06b3, 0x64,0xfc,0xfc,0xff }, /*   433 */
  {    73,   214,     0,0, 0x059a,0x007a, 0xff,0xff,0xff,0xff }, /*   434 */
  {    73,   214,     0,0, 0x059a,0x007a, 0xff,0xff,0xff,0xff }, /*   435 */
  {    23,   300,   -84,0, 0x064d,0x06b3, 0x64,0xfc,0xfc,0xff }, /*   436 */
  {    59,   267,    16,0, 0x0505,0x0294, 0xff,0xff,0xff,0xff }, /*   437 */
  {    59,   267,    16,0, 0x0505,0x0294, 0xff,0xff,0xff,0xff }, /*   438 */
  {     0,   313,   -51,0, 0x0400,0x0676, 0x60,0xf2,0xf2,0xff }, /*   439 */
  {     0,   275,    27,0, 0x0400,0x02af, 0xf1,0xf1,0xf1,0xff }, /*   440 */
  {    59,   267,    16,0, 0x0505,0x0294, 0xff,0xff,0xff,0xff }, /*   441 */
  {    51,   229,    48,0, 0x04d7,0xfff9, 0xef,0xef,0xef,0xff }, /*   442 */
  {    73,   214,     0,0, 0x059a,0x007a, 0xff,0xff,0xff,0xff }, /*   443 */
  {   -20,   319,   -81,0, 0x023e,0x0781, 0x45,0xae,0xae,0xff }, /*   444 */
  {     0,   313,   -51,0, 0x0400,0x0676, 0x60,0xf2,0xf2,0xff }, /*   445 */
  {     0,   302,   -81,0, 0x07ff,0x06b4, 0x59,0xdf,0xdf,0xff }, /*   446 */
  {    59,   267,    16,0, 0x0505,0x0294, 0xff,0xff,0xff,0xff }, /*   447 */
  {    20,   319,   -81,0, 0x05c1,0x0781, 0x61,0xf3,0xf3,0xff }, /*   448 */
  {     0,   313,   -51,0, 0x0400,0x0676, 0x60,0xf2,0xf2,0xff }, /*   449 */
  {    57,   201,   -84,0, 0x06d4,0x01f1, 0xdb,0xdb,0xdb,0xff }, /*   450 */
  {    23,   302,  -104,0, 0x06e1,0x0749, 0x63,0xf8,0xf8,0xff }, /*   451 */
  {    23,   300,   -84,0, 0x064d,0x06b3, 0x64,0xfc,0xfc,0xff }, /*   452 */
  {   -42,   204,    49,0, 0x0331,0xfec5, 0xb8,0xb8,0xb8,0xff }, /*   453 */
  {     0,   210,    49,0, 0x0400,0xff09, 0xb0,0xb0,0xb0,0xff }, /*   454 */
  {     0,   210,    68,0, 0x0400,0xfe90, 0xa8,0xa8,0xa8,0xff }, /*   455 */
  {     0,   302,   -81,0, 0x07ff,0x06b4, 0x59,0xdf,0xdf,0xff }, /*   456 */
  {   -23,   300,   -84,0, 0x01b2,0x06b3, 0x3f,0x9f,0x9f,0xff }, /*   457 */
  {   -20,   319,   -81,0, 0x023e,0x0781, 0x45,0xae,0xae,0xff }, /*   458 */
  {     0,   210,    68,0, 0x0400,0xfe90, 0xa8,0xa8,0xa8,0xff }, /*   459 */
  {    42,   204,    49,0, 0x04ce,0xfec5, 0xa2,0xa2,0xa2,0xff }, /*   460 */
  {    64,   204,    47,0, 0x0516,0xfecf, 0xf1,0xf1,0xf1,0xff }, /*   461 */
  {   -59,   267,    16,0, 0x02fa,0x0294, 0xb0,0xb0,0xb0,0xff }, /*   462 */
  {   -23,   300,   -84,0, 0x01b2,0x06b3, 0x3f,0x9f,0x9f,0xff }, /*   463 */
  {   -73,   214,     0,0, 0x0265,0x007a, 0xb8,0xb8,0xb8,0xff }, /*   464 */
  {    23,   302,  -104,0, 0x06e1,0x0749, 0x63,0xf8,0xf8,0xff }, /*   465 */
  {     0,   292,  -111,0, 0x07ff,0x06f1, 0x4b,0xbc,0xbc,0xff }, /*   466 */
  {     0,   302,   -81,0, 0x07ff,0x06b4, 0x59,0xdf,0xdf,0xff }, /*   467 */
  {   -73,   214,     0,0, 0x0265,0x007a, 0xb8,0xb8,0xb8,0xff }, /*   468 */
  {   -23,   300,   -84,0, 0x01b2,0x06b3, 0x3f,0x9f,0x9f,0xff }, /*   469 */
  {   -57,   201,   -84,0, 0x012b,0x01f1, 0xd7,0xd7,0xd7,0xff }, /*   470 */
  {   -57,   201,   -84,0, 0x012b,0x01f1, 0xd7,0xd7,0xd7,0xff }, /*   471 */
  {   -23,   302,  -104,0, 0x011e,0x0749, 0x3b,0x95,0x95,0xff }, /*   472 */
  {     0,   292,  -111,0, 0x0000,0x06f1, 0x4b,0xbc,0xbc,0xff }, /*   473 */
  {   -57,   201,   -84,0, 0x012b,0x01f1, 0xd7,0xd7,0xd7,0xff }, /*   474 */
  {     0,   292,  -111,0, 0x0000,0x06f1, 0x4b,0xbc,0xbc,0xff }, /*   475 */
  {     0,   193,  -119,0, 0x0000,0x0267, 0xcf,0xcf,0xcf,0xff }, /*   476 */
  {     0,   193,  -119,0, 0x0800,0x0267, 0xcf,0xcf,0xcf,0xff }, /*   477 */
  {     0,   292,  -111,0, 0x07ff,0x06f1, 0x4b,0xbc,0xbc,0xff }, /*   478 */
  {    57,   201,   -84,0, 0x06d4,0x01f1, 0xdb,0xdb,0xdb,0xff }, /*   479 */
  {     0,   275,    27,0, 0x0400,0x02af, 0xf1,0xf1,0xf1,0xff }, /*   480 */
  {     0,   313,   -51,0, 0x0400,0x0676, 0x60,0xf2,0xf2,0xff }, /*   481 */
  {   -59,   267,    16,0, 0x02fa,0x0294, 0xb0,0xb0,0xb0,0xff }, /*   482 */
  {   -73,   214,     0,0, 0x0265,0x007a, 0xb8,0xb8,0xb8,0xff }, /*   483 */
  {   -51,   229,    48,0, 0x0328,0xfff9, 0xb8,0xb8,0xb8,0xff }, /*   484 */
  {   -59,   267,    16,0, 0x02fa,0x0294, 0xb0,0xb0,0xb0,0xff }, /*   485 */
  {     0,   302,   -81,0, 0x07ff,0x06b4, 0x59,0xdf,0xdf,0xff }, /*   486 */
  {     0,   313,   -51,0, 0x0400,0x0676, 0x60,0xf2,0xf2,0xff }, /*   487 */
  {    20,   319,   -81,0, 0x05c1,0x0781, 0x61,0xf3,0xf3,0xff }, /*   488 */
  {     0,   302,   -81,0, 0x07ff,0x06b4, 0x59,0xdf,0xdf,0xff }, /*   489 */
  {    20,   319,   -81,0, 0x05c1,0x0781, 0x61,0xf3,0xf3,0xff }, /*   490 */
  {    23,   300,   -84,0, 0x064d,0x06b3, 0x64,0xfc,0xfc,0xff }, /*   491 */
  {     0,   302,   -81,0, 0x07ff,0x06b4, 0x59,0xdf,0xdf,0xff }, /*   492 */
  {    23,   300,   -84,0, 0x064d,0x06b3, 0x64,0xfc,0xfc,0xff }, /*   493 */
  {    23,   302,  -104,0, 0x06e1,0x0749, 0x63,0xf8,0xf8,0xff }, /*   494 */
  {     0,   302,   -81,0, 0x07ff,0x06b4, 0x59,0xdf,0xdf,0xff }, /*   495 */
  {   -23,   302,  -104,0, 0x011e,0x0749, 0x3b,0x95,0x95,0xff }, /*   496 */
  {   -23,   300,   -84,0, 0x01b2,0x06b3, 0x3f,0x9f,0x9f,0xff }, /*   497 */
  {     0,   302,   -81,0, 0x07ff,0x06b4, 0x59,0xdf,0xdf,0xff }, /*   498 */
  {     0,   292,  -111,0, 0x07ff,0x06f1, 0x4b,0xbc,0xbc,0xff }, /*   499 */
  {   -23,   302,  -104,0, 0x091e,0x0749, 0x3b,0x95,0x95,0xff }, /*   500 */
  {     0,   313,   -51,0, 0x0400,0x0676, 0x60,0xf2,0xf2,0xff }, /*   501 */
  {   -20,   319,   -81,0, 0x023e,0x0781, 0x45,0xae,0xae,0xff }, /*   502 */
  {   -59,   267,    16,0, 0x02fa,0x0294, 0xb0,0xb0,0xb0,0xff }, /*   503 */
  {     0,   292,  -111,0, 0x07ff,0x06f1, 0x4b,0xbc,0xbc,0xff }, /*   504 */
  {    23,   302,  -104,0, 0x06e1,0x0749, 0x63,0xf8,0xf8,0xff }, /*   505 */
  {    57,   201,   -84,0, 0x06d4,0x01f1, 0xdb,0xdb,0xdb,0xff }, /*   506 */
  {     0,   210,    68,0, 0x0400,0xfe90, 0xa8,0xa8,0xa8,0xff }, /*   507 */
  {   -64,   204,    47,0, 0x02e9,0xfecf, 0xbe,0xbe,0xbe,0xff }, /*   508 */
  {   -42,   204,    49,0, 0x0331,0xfec5, 0xb8,0xb8,0xb8,0xff }, /*   509 */
  {   -23,   300,   -84,0, 0x01b2,0x06b3, 0x3f,0x9f,0x9f,0xff }, /*   510 */
  {   -23,   302,  -104,0, 0x011e,0x0749, 0x3b,0x95,0x95,0xff }, /*   511 */
  {   -57,   201,   -84,0, 0x012b,0x01f1, 0xd7,0xd7,0xd7,0xff }, /*   512 */
  {   -59,   267,    16,0, 0x02fa,0x0294, 0xb0,0xb0,0xb0,0xff }, /*   513 */
  {   -20,   319,   -81,0, 0x023e,0x0781, 0x45,0xae,0xae,0xff }, /*   514 */
  {   -23,   300,   -84,0, 0x01b2,0x06b3, 0x3f,0x9f,0x9f,0xff }, /*   515 */
  {    23,   300,   -84,0, 0x064d,0x06b3, 0x64,0xfc,0xfc,0xff }, /*   516 */
  {    20,   319,   -81,0, 0x05c1,0x0781, 0x61,0xf3,0xf3,0xff }, /*   517 */
  {    59,   267,    16,0, 0x0505,0x0294, 0xff,0xff,0xff,0xff }, /*   518 */
  {     0,   210,    68,0, 0x0400,0xfe90, 0xa8,0xa8,0xa8,0xff }, /*   519 */
  {     0,   210,    49,0, 0x0400,0xff09, 0xb0,0xb0,0xb0,0xff }, /*   520 */
  {    42,   204,    49,0, 0x04ce,0xfec5, 0xa2,0xa2,0xa2,0xff }, /*   521 */
  {    36,   132,    52,0, 0x077e,0x0000, 0xbe,0xbe,0xbe,0xff }, /*   522 */
  {    42,   170,    65,0, 0x0800,0x0755, 0xf9,0xa3,0x95,0xff }, /*   523 */
  {     0,   170,    73,0, 0x0400,0x07ff, 0xbc,0x96,0x71,0xff }, /*   524 */
  {    36,   132,    52,0, 0x077e,0x0000, 0xbe,0xbe,0xbe,0xff }, /*   525 */
  {     0,   170,    73,0, 0x0400,0x07ff, 0xbc,0x96,0x71,0xff }, /*   526 */
  {   -36,   132,    52,0, 0x0081,0x0000, 0xd3,0xd3,0xd3,0xff }, /*   527 */
  {     0,   170,    73,0, 0x0400,0x07ff, 0xbc,0x96,0x71,0xff }, /*   528 */
  {   -42,   170,    65,0, 0x0000,0x0755, 0xcd,0x86,0x7b,0xff }, /*   529 */
  {   -36,   132,    52,0, 0x0081,0x0000, 0xd3,0xd3,0xd3,0xff }, /*   530 */
  {   -42,   170,    65,0, 0xf800,0x0000, 0xcd,0x86,0x7b,0xff }, /*   531 */
  {     0,   210,    49,0, 0x0000,0x0800, 0xc4,0xc4,0xc4,0xff }, /*   532 */
  {   -42,   204,    49,0, 0xf800,0x06e5, 0xcc,0xcc,0xcc,0xff }, /*   533 */
  {    42,   170,    65,0, 0x0800,0x0000, 0xf9,0xa3,0x95,0xff }, /*   534 */
  {    42,   204,    49,0, 0x0800,0x06e5, 0xb4,0xb4,0xb4,0xff }, /*   535 */
  {     0,   210,    49,0, 0x0000,0x0800, 0xc4,0xc4,0xc4,0xff }, /*   536 */
  {   -42,   170,    65,0, 0xf800,0x0000, 0xcd,0x86,0x7b,0xff }, /*   537 */
  {     0,   170,    73,0, 0x0000,0x0000, 0xbc,0x96,0x71,0xff }, /*   538 */
  {     0,   210,    49,0, 0x0000,0x0800, 0xc4,0xc4,0xc4,0xff }, /*   539 */
  {     0,   170,    73,0, 0x0000,0x0000, 0xbc,0x96,0x71,0xff }, /*   540 */
  {    42,   170,    65,0, 0x0800,0x0000, 0xf9,0xa3,0x95,0xff }, /*   541 */
  {     0,   210,    49,0, 0x0000,0x0800, 0xc4,0xc4,0xc4,0xff }, /*   542 */
  {    -9,   247,    60,0, 0x0000,0x0000, 0xc2,0x5e,0x39,0xff }, /*   543 */
  {    -9,   273,    37,0, 0x0000,0x0000, 0xc2,0x29,0x66,0xff }, /*   544 */
  {   -46,   242,    50,0, 0x0000,0x0000, 0x02,0x69,0x46,0xff }, /*   545 */
  {     9,   273,    37,0, 0x0000,0x0000, 0x3e,0x29,0x66,0xff }, /*   546 */
  {    46,   242,    50,0, 0x0000,0x0000, 0xfe,0x69,0x46,0xff }, /*   547 */
  {    53,   268,    29,0, 0x0000,0x0000, 0xfa,0x36,0x72,0xff }, /*   548 */
  {     9,   273,    37,0, 0x0000,0x0000, 0x3e,0x29,0x66,0xff }, /*   549 */
  {     9,   247,    60,0, 0x0000,0x0000, 0x3e,0x5e,0x39,0xff }, /*   550 */
  {    46,   242,    50,0, 0x0000,0x0000, 0xfe,0x69,0x46,0xff }, /*   551 */
  {    -9,   273,    37,0, 0x0000,0x0000, 0xc2,0x29,0x66,0xff }, /*   552 */
  {   -53,   268,    29,0, 0x0000,0x0000, 0x06,0x36,0x72,0xff }, /*   553 */
  {   -46,   242,    50,0, 0x0000,0x0000, 0x02,0x69,0x46,0xff }, /*   554 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_264_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 32, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP1Triangle(  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSP2Triangles( 24, 25, 26,  0, 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 30, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP2Triangles(  1,  2,  3,  0,  4,  5,  6,  0),
  gsSP1Triangle(  7,  8,  9,  0),
  gsSP1Triangle( 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsSP1Triangle( 16, 17, 18,  0),
  gsSP1Triangle( 19, 20, 21,  0),
  gsSP2Triangles( 22, 23, 24,  0, 25, 26, 27,  0),
  gsSPVertex( SEGADRS(5, 992), 2, 30 ),
  gsSP1Triangle( 28, 29, 30,  0),
  gsSPVertex( SEGADRS(5, 1024), 31, 0 ),
  gsSP1Triangle( 31,  0,  1,  0),
  gsSP1Triangle(  2,  3,  4,  0),
  gsSP1Triangle(  5,  6,  7,  0),
  gsSP1Triangle(  8,  9, 10,  0),
  gsSP1Triangle( 11, 12, 13,  0),
  gsSP1Triangle( 14, 15, 16,  0),
  gsSP1Triangle( 17, 18, 19,  0),
  gsSP1Triangle( 20, 21, 22,  0),
  gsSP1Triangle( 23, 24, 25,  0),
  gsSP1Triangle( 26, 27, 28,  0),
  gsSPVertex( SEGADRS(5, 1520), 1, 31 ),
  gsSP1Triangle( 29, 30, 31,  0),
  gsSPVertex( SEGADRS(5, 1536), 29, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP1Triangle(  3,  4,  5,  0),
  gsSP2Triangles(  6,  7,  8,  0,  9, 10, 11,  0),
  gsSP2Triangles( 12, 13, 14,  0, 15, 16, 17,  0),
  gsSP2Triangles( 18, 19, 20,  0, 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSPVertex( SEGADRS(5, 2000), 3, 29 ),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 2048), 30, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP2Triangles(  1,  2,  3,  0,  4,  5,  6,  0),
  gsSP1Triangle(  7,  8,  9,  0),
  gsSP1Triangle( 10, 11, 12,  0),
  gsSP2Triangles( 13, 14, 15,  0, 16, 17, 18,  0),
  gsSP1Triangle( 19, 20, 21,  0),
  gsSP1Triangle( 22, 23, 24,  0),
  gsSP1Triangle( 25, 26, 27,  0),
  gsSPVertex( SEGADRS(5, 2528), 2, 30 ),
  gsSP1Triangle( 28, 29, 30,  0),
  gsSPVertex( SEGADRS(5, 2560), 31, 0 ),
  gsSP2Triangles( 31,  0,  1,  0,  2,  3,  4,  0),
  gsSP2Triangles(  5,  6,  7,  0,  8,  9, 10,  0),
  gsSP2Triangles( 11, 12, 13,  0, 14, 15, 16,  0),
  gsSP1Triangle( 17, 18, 19,  0),
  gsSP1Triangle( 20, 21, 22,  0),
  gsSP2Triangles( 23, 24, 25,  0, 26, 27, 28,  0),
  gsSPVertex( SEGADRS(5, 3056), 1, 31 ),
  gsSPVertex( SEGADRS(5, 3072), 29, 0 ),
  gsSP2Triangles( 29, 30, 31,  0,  0,  1,  2,  0),
  gsSP1Triangle(  3,  4,  5,  0),
  gsSP1Triangle(  6,  7,  8,  0),
  gsSP2Triangles(  9, 10, 11,  0, 12, 13, 14,  0),
  gsSP2Triangles( 15, 16, 17,  0, 18, 19, 20,  0),
  gsSP2Triangles( 21, 22, 23,  0, 24, 25, 26,  0),
  gsSPVertex( SEGADRS(5, 3536), 3, 29 ),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 3584), 13, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP1Triangle(  1,  2,  3,  0),
  gsSP1Triangle(  4,  5,  6,  0),
  gsSP1Triangle(  7,  8,  9,  0),
  gsSP1Triangle( 10, 11, 12,  0),
  gsSPClearGeometryMode( G_SHADING_SMOOTH ),
  gsSPVertex( SEGADRS(5, 3792), 19, 13 ),
  gsSP2Triangles( 13, 14, 15,  0, 16, 17, 18,  0),
  gsSP2Triangles( 19, 20, 21,  0, 22, 23, 24,  0),
  gsSP1Triangle( 25, 26, 27,  0),
  gsSP1Triangle( 28, 29, 30,  0),
  gsSPVertex( SEGADRS(5, 4096), 29, 0 ),
  gsSP1Triangle( 31,  0,  1,  0),
  gsSP1Triangle(  2,  3,  4,  0),
  gsSP1Triangle(  5,  6,  7,  0),
  gsSP1Triangle(  8,  9, 10,  0),
  gsSP1Triangle( 11, 12, 13,  0),
  gsSP1Triangle( 14, 15, 16,  0),
  gsSP1Triangle( 17, 18, 19,  0),
  gsSP1Triangle( 20, 21, 22,  0),
  gsSP1Triangle( 23, 24, 25,  0),
  gsSP1Triangle( 26, 27, 28,  0),
  gsSPSetGeometryMode( G_SHADING_SMOOTH ),
  gsSPVertex( SEGADRS(5, 4560), 3, 29 ),
  gsSP1Triangle( 29, 30, 31,  0),
  gsSPVertex( SEGADRS(5, 4608), 29, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP1Triangle(  3,  4,  5,  0),
  gsSP1Triangle(  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSPVertex( SEGADRS(5, 5072), 3, 29 ),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 5120), 30, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP2Triangles(  1,  2,  3,  0,  4,  5,  6,  0),
  gsSP1Triangle(  7,  8,  9,  0),
  gsSP1Triangle( 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsSP1Triangle( 16, 17, 18,  0),
  gsSP1Triangle( 19, 20, 21,  0),
  gsSP1Triangle( 22, 23, 24,  0),
  gsSP1Triangle( 25, 26, 27,  0),
  gsSPVertex( SEGADRS(5, 5600), 2, 30 ),
  gsSP1Triangle( 28, 29, 30,  0),
  gsSPVertex( SEGADRS(5, 5632), 2, 0 ),
  gsSP1Triangle( 31,  0,  1,  0),
  gsSPClearGeometryMode( G_SHADING_SMOOTH ),
  gsSPVertex( SEGADRS(5, 5664), 30, 2 ),
  gsSP1Triangle(  2,  3,  4,  0),
  gsSP1Triangle(  5,  6,  7,  0),
  gsSP1Triangle(  8,  9, 10,  0),
  gsSP1Triangle( 11, 12, 13,  0),
  gsSP1Triangle( 14, 15, 16,  0),
  gsSP1Triangle( 17, 18, 19,  0),
  gsSP1Triangle( 20, 21, 22,  0),
  gsSP1Triangle( 23, 24, 25,  0),
  gsSP1Triangle( 26, 27, 28,  0),
  gsSP1Triangle( 29, 30, 31,  0),
  gsSPVertex( SEGADRS(5, 6144), 18, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP1Triangle(  3,  4,  5,  0),
  gsSP1Triangle(  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsSPSetGeometryMode( G_SHADING_SMOOTH ),
  gsSPVertex( SEGADRS(5, 6432), 14, 18 ),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP2Triangles( 21, 22, 23,  0, 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 6656), 30, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP2Triangles(  1,  2,  3,  0,  4,  5,  6,  0),
  gsSP2Triangles(  7,  8,  9,  0, 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsDPPipeSync(),
  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0, TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0),
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
  gsDPSetTextureLUT( G_TT_RGBA16 ),
  gsDPLoadTLUT_pal16( 0, SEGADRS(7, 32) ),
  gsDPLoadMultiBlock_4b( SEGADRS(6,512), 0, 0,
    G_IM_FMT_CI, 32, 32, 0,
    0, 0, 5, 5, 0, 0 ),
  gsSP2Triangles( 16, 17, 18,  0, 19, 20, 21,  0),
  gsSP2Triangles( 22, 23, 24,  0, 25, 26, 27,  0),
  gsSPVertex( SEGADRS(5, 7136), 2, 30 ),
  gsSP1Triangle( 28, 29, 30,  0),
  gsSPVertex( SEGADRS(5, 7168), 31, 0 ),
  gsSP2Triangles( 31,  0,  1,  0,  2,  3,  4,  0),
  gsSP2Triangles(  5,  6,  7,  0,  8,  9, 10,  0),
  gsSP1Triangle( 11, 12, 13,  0),
  gsSP2Triangles( 14, 15, 16,  0, 17, 18, 19,  0),
  gsSP2Triangles( 20, 21, 22,  0, 23, 24, 25,  0),
  gsSP1Triangle( 26, 27, 28,  0),
  gsSPVertex( SEGADRS(5, 7664), 1, 31 ),
  gsSP1Triangle( 29, 30, 31,  0),
  gsSPVertex( SEGADRS(5, 7680), 29, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP2Triangles(  3,  4,  5,  0,  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSPVertex( SEGADRS(5, 8144), 3, 29 ),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 8192), 30, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP1Triangle(  1,  2,  3,  0),
  gsSP1Triangle(  4,  5,  6,  0),
  gsSP1Triangle(  7,  8,  9,  0),
  gsDPPipeSync(),
  gsDPSetTextureLUT( G_TT_RGBA16 ),
  gsDPLoadTLUT_pal16( 0, SEGADRS(7, 64) ),
  gsDPLoadMultiBlock_4b( SEGADRS(6,1024), 0, 0,
    G_IM_FMT_CI, 32, 32, 0,
    1, 1, 5, 5, 0, 0 ),
  gsSP1Triangle( 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsSP1Triangle( 16, 17, 18,  0),
  gsDPPipeSync(),
  gsDPSetTextureLUT( G_TT_RGBA16 ),
  gsDPLoadTLUT_pal16( 0, SEGADRS(7, 96) ),
  gsDPLoadMultiBlock_4b( SEGADRS(6,1536), 0, 0,
    G_IM_FMT_CI, 32, 32, 0,
    1, 1, 5, 5, 0, 0 ),
  gsSP1Triangle( 19, 20, 21,  0),
  gsSP1Triangle( 22, 23, 24,  0),
  gsSP1Triangle( 25, 26, 27,  0),
  gsSPVertex( SEGADRS(5, 8672), 1, 30 ),
  gsSP1Triangle( 28, 29, 30,  0),
  gsSPSetGeometryMode( G_LIGHTING|G_TEXTURE_GEN ),
  gsDPPipeSync(),
  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0),
  gsSPTexture(0x0800, 0x0800, 0, G_TX_RENDERTILE, G_ON),
  gsDPSetTextureLUT( G_TT_RGBA16 ),
  gsDPLoadTLUT_pal16( 0, SEGADRS(7, 128) ),
  gsDPLoadMultiBlock_4b( SEGADRS(6,2048), 0, 0,
    G_IM_FMT_CI, 32, 32, 0,
    0, 0, 5, 5, 0, 0 ),
  gsSPVertex( SEGADRS(5, 8688), 12, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP2Triangles(  6,  7,  8,  0,  9, 10, 11,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_264_ = {
  gfx_Shape_of___POLYHEDRON_264_,
  vtx_Shape_of___POLYHEDRON_264_, 555, 5,
  bmp_frake_walk,
  tlut_frake_walk,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_walk_aligner11[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_328_[] = {
  {    38,   -53,     3,0, 0x0000,0x0000, 0xff,0xee,0xab,0xff }, /*     0 */
  {    38,   -75,     3,0, 0x0000,0x0000, 0xff,0xb9,0x85,0xff }, /*     1 */
  {    38,   -53,   -22,0, 0x0000,0x0000, 0xfe,0xb8,0x84,0xff }, /*     2 */
  {    38,   -75,     3,0, 0x0000,0x0000, 0xff,0xb9,0x85,0xff }, /*     3 */
  {    38,   -75,   -22,0, 0x0000,0x0000, 0xfd,0xb7,0x84,0xff }, /*     4 */
  {    38,   -53,   -22,0, 0x0000,0x0000, 0xfe,0xb8,0x84,0xff }, /*     5 */
  {    38,   -53,   -22,0, 0x0000,0x0000, 0xfe,0xb8,0x84,0xff }, /*     6 */
  {    38,   -75,   -22,0, 0x0000,0x0000, 0xfd,0xb7,0x84,0xff }, /*     7 */
  {    13,   -53,   -22,0, 0x0000,0x0000, 0xe3,0xa4,0x76,0xff }, /*     8 */
  {    38,   -75,   -22,0, 0x0000,0x0000, 0xfd,0xb7,0x84,0xff }, /*     9 */
  {    13,   -75,   -22,0, 0x0000,0x0000, 0xbb,0x87,0x61,0xff }, /*    10 */
  {    13,   -53,   -22,0, 0x0000,0x0000, 0xe3,0xa4,0x76,0xff }, /*    11 */
  {    13,   -53,   -22,0, 0x0000,0x0000, 0xe3,0xa4,0x76,0xff }, /*    12 */
  {    13,   -75,     3,0, 0x0000,0x0000, 0xbb,0x87,0x61,0xff }, /*    13 */
  {    13,   -53,     3,0, 0x0000,0x0000, 0xff,0xd0,0x96,0xff }, /*    14 */
  {    13,   -53,   -22,0, 0x0000,0x0000, 0xe3,0xa4,0x76,0xff }, /*    15 */
  {    13,   -75,   -22,0, 0x0000,0x0000, 0xbb,0x87,0x61,0xff }, /*    16 */
  {    13,   -75,     3,0, 0x0000,0x0000, 0xbb,0x87,0x61,0xff }, /*    17 */
  {    13,   -53,     3,0, 0x0000,0x0000, 0xff,0xd0,0x96,0xff }, /*    18 */
  {    13,   -75,     3,0, 0x0000,0x0000, 0xbb,0x87,0x61,0xff }, /*    19 */
  {    38,   -53,     3,0, 0x0000,0x0000, 0xff,0xee,0xab,0xff }, /*    20 */
  {    13,   -75,     3,0, 0x0000,0x0000, 0xbb,0x87,0x61,0xff }, /*    21 */
  {    38,   -75,     3,0, 0x0000,0x0000, 0xff,0xb9,0x85,0xff }, /*    22 */
  {    38,   -53,     3,0, 0x0000,0x0000, 0xff,0xee,0xab,0xff }, /*    23 */
  {    38,   -75,   -22,0, 0x0000,0x0000, 0xfd,0xb7,0x84,0xff }, /*    24 */
  {    26,   -86,    -9,0, 0x0000,0x0000, 0xb3,0x4e,0x38,0xff }, /*    25 */
  {    13,   -75,   -22,0, 0x0000,0x0000, 0xbb,0x87,0x61,0xff }, /*    26 */
  {    38,   -75,     3,0, 0x0000,0x0000, 0xff,0xb9,0x85,0xff }, /*    27 */
  {    26,   -86,    -9,0, 0x0000,0x0000, 0xb3,0x4e,0x38,0xff }, /*    28 */
  {    38,   -75,   -22,0, 0x0000,0x0000, 0xfd,0xb7,0x84,0xff }, /*    29 */
  {    13,   -75,     3,0, 0x0000,0x0000, 0xbb,0x87,0x61,0xff }, /*    30 */
  {    26,   -86,    -9,0, 0x0000,0x0000, 0xb3,0x4e,0x38,0xff }, /*    31 */
  {    38,   -75,     3,0, 0x0000,0x0000, 0xff,0xb9,0x85,0xff }, /*    32 */
  {    13,   -75,   -22,0, 0x0000,0x0000, 0xbb,0x87,0x61,0xff }, /*    33 */
  {    26,   -86,    -9,0, 0x0000,0x0000, 0xb3,0x4e,0x38,0xff }, /*    34 */
  {    13,   -75,     3,0, 0x0000,0x0000, 0xbb,0x87,0x61,0xff }, /*    35 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_328_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 32, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP2Triangles(  6,  7,  8,  0,  9, 10, 11,  0),
  gsSP2Triangles( 12, 13, 14,  0, 15, 16, 17,  0),
  gsSP2Triangles( 18, 19, 20,  0, 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 4, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP1Triangle(  1,  2,  3,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_328_ = {
  gfx_Shape_of___POLYHEDRON_328_,
  vtx_Shape_of___POLYHEDRON_328_, 36, 5,
  bmp_frake_walk,
  tlut_frake_walk,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_walk_aligner12[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_332_[] = {
  {    38,  -104,     3,0, 0x0000,0x0000, 0xfc,0xb6,0x83,0xff }, /*     0 */
  {    38,   -80,     3,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*     1 */
  {    13,  -104,     3,0, 0x0000,0x0000, 0xb2,0x80,0x5c,0xff }, /*     2 */
  {    38,   -80,     3,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*     3 */
  {    13,   -80,     3,0, 0x0000,0x0000, 0xde,0xa0,0x73,0xff }, /*     4 */
  {    13,  -104,     3,0, 0x0000,0x0000, 0xb2,0x80,0x5c,0xff }, /*     5 */
  {    13,  -104,   -22,0, 0x0000,0x0000, 0xf6,0xb2,0x80,0xff }, /*     6 */
  {    13,   -80,   -22,0, 0x0000,0x0000, 0xf8,0xb3,0x81,0xff }, /*     7 */
  {    38,  -104,   -22,0, 0x0000,0x0000, 0xff,0xce,0x94,0xff }, /*     8 */
  {    13,   -80,   -22,0, 0x0000,0x0000, 0xf8,0xb3,0x81,0xff }, /*     9 */
  {    38,   -80,   -22,0, 0x0000,0x0000, 0xff,0xed,0xaa,0xff }, /*    10 */
  {    38,  -104,   -22,0, 0x0000,0x0000, 0xff,0xce,0x94,0xff }, /*    11 */
  {    13,   -80,     3,0, 0x0000,0x0000, 0xde,0xa0,0x73,0xff }, /*    12 */
  {    13,   -80,   -22,0, 0x0000,0x0000, 0xf8,0xb3,0x81,0xff }, /*    13 */
  {    13,  -104,     3,0, 0x0000,0x0000, 0xb2,0x80,0x5c,0xff }, /*    14 */
  {    38,   -80,   -22,0, 0x0000,0x0000, 0xff,0xed,0xaa,0xff }, /*    15 */
  {    38,   -80,     3,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    16 */
  {    38,  -104,   -22,0, 0x0000,0x0000, 0xff,0xce,0x94,0xff }, /*    17 */
  {    38,   -80,     3,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    18 */
  {    26,   -67,    -9,0, 0x0000,0x0000, 0xff,0x76,0x55,0xff }, /*    19 */
  {    13,   -80,     3,0, 0x0000,0x0000, 0xde,0xa0,0x73,0xff }, /*    20 */
  {    38,   -80,   -22,0, 0x0000,0x0000, 0xff,0xed,0xaa,0xff }, /*    21 */
  {    26,   -67,    -9,0, 0x0000,0x0000, 0xff,0x76,0x55,0xff }, /*    22 */
  {    38,   -80,     3,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    23 */
  {    13,   -80,   -22,0, 0x0000,0x0000, 0xf8,0xb3,0x81,0xff }, /*    24 */
  {    26,   -67,    -9,0, 0x0000,0x0000, 0xff,0x76,0x55,0xff }, /*    25 */
  {    38,   -80,   -22,0, 0x0000,0x0000, 0xff,0xed,0xaa,0xff }, /*    26 */
  {    13,   -80,     3,0, 0x0000,0x0000, 0xde,0xa0,0x73,0xff }, /*    27 */
  {    26,   -67,    -9,0, 0x0000,0x0000, 0xff,0x76,0x55,0xff }, /*    28 */
  {    13,   -80,   -22,0, 0x0000,0x0000, 0xf8,0xb3,0x81,0xff }, /*    29 */
  {    38,  -104,     3,0, 0x0000,0x0000, 0x9d,0x4e,0x00,0xff }, /*    30 */
  {    38,  -138,     3,0, 0x0000,0x0000, 0x86,0x43,0x00,0xff }, /*    31 */
  {    38,  -104,   -22,0, 0x0000,0x0000, 0xb1,0x58,0x00,0xff }, /*    32 */
  {    38,  -138,     3,0, 0x0000,0x0000, 0x86,0x43,0x00,0xff }, /*    33 */
  {    38,  -138,   -22,0, 0x0000,0x0000, 0xa7,0x53,0x00,0xff }, /*    34 */
  {    38,  -104,   -22,0, 0x0000,0x0000, 0xb1,0x58,0x00,0xff }, /*    35 */
  {    38,  -104,   -22,0, 0x0000,0x0000, 0xb1,0x58,0x00,0xff }, /*    36 */
  {    38,  -138,   -22,0, 0x0000,0x0000, 0xa7,0x53,0x00,0xff }, /*    37 */
  {    13,  -104,   -22,0, 0x0000,0x0000, 0x99,0x4c,0x00,0xff }, /*    38 */
  {    13,  -104,   -22,0, 0x0000,0x0000, 0x99,0x4c,0x00,0xff }, /*    39 */
  {    13,  -138,   -22,0, 0x0000,0x0000, 0x78,0x3c,0x00,0xff }, /*    40 */
  {    13,  -138,     3,0, 0x0000,0x0000, 0x6d,0x36,0x00,0xff }, /*    41 */
  {    38,  -138,   -22,0, 0x0000,0x0000, 0xa7,0x53,0x00,0xff }, /*    42 */
  {    13,  -138,   -22,0, 0x0000,0x0000, 0x78,0x3c,0x00,0xff }, /*    43 */
  {    13,  -104,   -22,0, 0x0000,0x0000, 0x99,0x4c,0x00,0xff }, /*    44 */
  {    13,  -104,   -22,0, 0x0000,0x0000, 0x99,0x4c,0x00,0xff }, /*    45 */
  {    13,  -138,     3,0, 0x0000,0x0000, 0x6d,0x36,0x00,0xff }, /*    46 */
  {    13,  -104,     3,0, 0x0000,0x0000, 0x6e,0x37,0x00,0xff }, /*    47 */
  {    13,  -104,     3,0, 0x0000,0x0000, 0x6e,0x37,0x00,0xff }, /*    48 */
  {    13,  -138,     3,0, 0x0000,0x0000, 0x6d,0x36,0x00,0xff }, /*    49 */
  {    38,  -104,     3,0, 0x0000,0x0000, 0x9d,0x4e,0x00,0xff }, /*    50 */
  {    13,  -138,     3,0, 0x0000,0x0000, 0x6d,0x36,0x00,0xff }, /*    51 */
  {    38,  -138,     3,0, 0x0000,0x0000, 0x86,0x43,0x00,0xff }, /*    52 */
  {    38,  -104,     3,0, 0x0000,0x0000, 0x9d,0x4e,0x00,0xff }, /*    53 */
  {    38,   -80,     3,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    54 */
  {    38,  -104,     3,0, 0x0000,0x0000, 0xfc,0xb6,0x83,0xff }, /*    55 */
  {    38,  -104,   -22,0, 0x0000,0x0000, 0xff,0xce,0x94,0xff }, /*    56 */
  {    13,   -80,   -22,0, 0x0000,0x0000, 0xf8,0xb3,0x81,0xff }, /*    57 */
  {    13,  -104,   -22,0, 0x0000,0x0000, 0xf6,0xb2,0x80,0xff }, /*    58 */
  {    13,  -104,     3,0, 0x0000,0x0000, 0xb2,0x80,0x5c,0xff }, /*    59 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_332_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 32, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP2Triangles(  6,  7,  8,  0,  9, 10, 11,  0),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 28, 0 ),
  gsSP2Triangles( 30, 31,  0,  0,  1,  2,  3,  0),
  gsSP2Triangles(  4,  5,  6,  0,  7,  8,  9,  0),
  gsSP1Triangle( 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsSP2Triangles( 16, 17, 18,  0, 19, 20, 21,  0),
  gsSP1Triangle( 22, 23, 24,  0),
  gsSP1Triangle( 25, 26, 27,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_332_ = {
  gfx_Shape_of___POLYHEDRON_332_,
  vtx_Shape_of___POLYHEDRON_332_, 60, 5,
  bmp_frake_walk,
  tlut_frake_walk,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_walk_aligner13[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_336_[] = {
  {    10,  -129,   -24,0, 0x0000,0x0000, 0xb7,0x5b,0x00,0xff }, /*     0 */
  {    44,  -151,   -30,0, 0x0000,0x0000, 0x9e,0x4f,0x00,0xff }, /*     1 */
  {     5,  -151,   -23,0, 0x0000,0x0000, 0x86,0x43,0x00,0xff }, /*     2 */
  {    55,  -119,     8,0, 0x0000,0x0000, 0xce,0x67,0x00,0xff }, /*     3 */
  {    10,  -119,    16,0, 0x0000,0x0000, 0xb5,0x5a,0x00,0xff }, /*     4 */
  {    52,  -109,    39,0, 0x0000,0x0000, 0xb6,0x5b,0x00,0xff }, /*     5 */
  {    10,  -119,    16,0, 0x0000,0x0000, 0xb5,0x5a,0x00,0xff }, /*     6 */
  {    18,  -109,    45,0, 0x0000,0x0000, 0xad,0x56,0x00,0xff }, /*     7 */
  {    52,  -109,    39,0, 0x0000,0x0000, 0xb6,0x5b,0x00,0xff }, /*     8 */
  {    59,  -151,     7,0, 0x0000,0x0000, 0xa0,0x50,0x00,0xff }, /*     9 */
  {    13,  -151,    46,0, 0x0000,0x0000, 0x63,0x31,0x00,0xff }, /*    10 */
  {     5,  -151,    17,0, 0x0000,0x0000, 0x6c,0x36,0x00,0xff }, /*    11 */
  {     5,  -151,    17,0, 0x0000,0x0000, 0x6c,0x36,0x00,0xff }, /*    12 */
  {    44,  -151,   -30,0, 0x0000,0x0000, 0x9e,0x4f,0x00,0xff }, /*    13 */
  {    59,  -151,     7,0, 0x0000,0x0000, 0xa0,0x50,0x00,0xff }, /*    14 */
  {    18,  -109,    45,0, 0x0000,0x0000, 0xad,0x56,0x00,0xff }, /*    15 */
  {    10,  -119,    16,0, 0x0000,0x0000, 0xb5,0x5a,0x00,0xff }, /*    16 */
  {    13,  -151,    46,0, 0x0000,0x0000, 0x63,0x31,0x00,0xff }, /*    17 */
  {    55,  -119,     8,0, 0x0000,0x0000, 0xce,0x67,0x00,0xff }, /*    18 */
  {    65,  -151,    37,0, 0x0000,0x0000, 0xa0,0x50,0x00,0xff }, /*    19 */
  {    59,  -151,     7,0, 0x0000,0x0000, 0xa0,0x50,0x00,0xff }, /*    20 */
  {    13,  -151,    46,0, 0x0000,0x0000, 0x63,0x31,0x00,0xff }, /*    21 */
  {    39,  -138,    62,0, 0x0000,0x0000, 0x53,0x29,0x00,0xff }, /*    22 */
  {    18,  -109,    45,0, 0x0000,0x0000, 0xad,0x56,0x00,0xff }, /*    23 */
  {    65,  -151,    37,0, 0x0000,0x0000, 0xa0,0x50,0x00,0xff }, /*    24 */
  {    39,  -138,    62,0, 0x0000,0x0000, 0x53,0x29,0x00,0xff }, /*    25 */
  {    13,  -151,    46,0, 0x0000,0x0000, 0x63,0x31,0x00,0xff }, /*    26 */
  {    52,  -109,    39,0, 0x0000,0x0000, 0xb6,0x5b,0x00,0xff }, /*    27 */
  {    39,  -138,    62,0, 0x0000,0x0000, 0x53,0x29,0x00,0xff }, /*    28 */
  {    65,  -151,    37,0, 0x0000,0x0000, 0xa0,0x50,0x00,0xff }, /*    29 */
  {    18,  -109,    45,0, 0x0000,0x0000, 0xad,0x56,0x00,0xff }, /*    30 */
  {    39,  -138,    62,0, 0x0000,0x0000, 0x53,0x29,0x00,0xff }, /*    31 */
  {    52,  -109,    39,0, 0x0000,0x0000, 0xb6,0x5b,0x00,0xff }, /*    32 */
  {    55,  -119,     8,0, 0x0000,0x0000, 0xce,0x67,0x00,0xff }, /*    33 */
  {    10,  -129,   -24,0, 0x0000,0x0000, 0xb7,0x5b,0x00,0xff }, /*    34 */
  {    10,  -119,    16,0, 0x0000,0x0000, 0xb5,0x5a,0x00,0xff }, /*    35 */
  {    55,  -119,     8,0, 0x0000,0x0000, 0xce,0x67,0x00,0xff }, /*    36 */
  {    41,  -129,   -29,0, 0x0000,0x0000, 0xda,0x6d,0x00,0xff }, /*    37 */
  {    10,  -129,   -24,0, 0x0000,0x0000, 0xb7,0x5b,0x00,0xff }, /*    38 */
  {    10,  -119,    16,0, 0x0000,0x0000, 0xb5,0x5a,0x00,0xff }, /*    39 */
  {     5,  -151,   -23,0, 0x0000,0x0000, 0x86,0x43,0x00,0xff }, /*    40 */
  {     5,  -151,    17,0, 0x0000,0x0000, 0x6c,0x36,0x00,0xff }, /*    41 */
  {    59,  -151,     7,0, 0x0000,0x0000, 0xa0,0x50,0x00,0xff }, /*    42 */
  {    41,  -129,   -29,0, 0x0000,0x0000, 0xda,0x6d,0x00,0xff }, /*    43 */
  {    55,  -119,     8,0, 0x0000,0x0000, 0xce,0x67,0x00,0xff }, /*    44 */
  {    59,  -151,     7,0, 0x0000,0x0000, 0xa0,0x50,0x00,0xff }, /*    45 */
  {    44,  -151,   -30,0, 0x0000,0x0000, 0x9e,0x4f,0x00,0xff }, /*    46 */
  {    41,  -129,   -29,0, 0x0000,0x0000, 0xda,0x6d,0x00,0xff }, /*    47 */
  {     5,  -151,    17,0, 0x0000,0x0000, 0x6c,0x36,0x00,0xff }, /*    48 */
  {     5,  -151,   -23,0, 0x0000,0x0000, 0x86,0x43,0x00,0xff }, /*    49 */
  {    44,  -151,   -30,0, 0x0000,0x0000, 0x9e,0x4f,0x00,0xff }, /*    50 */
  {    10,  -119,    16,0, 0x0000,0x0000, 0xb5,0x5a,0x00,0xff }, /*    51 */
  {    10,  -129,   -24,0, 0x0000,0x0000, 0xb7,0x5b,0x00,0xff }, /*    52 */
  {     5,  -151,   -23,0, 0x0000,0x0000, 0x86,0x43,0x00,0xff }, /*    53 */
  {    55,  -119,     8,0, 0x0000,0x0000, 0xce,0x67,0x00,0xff }, /*    54 */
  {    52,  -109,    39,0, 0x0000,0x0000, 0xb6,0x5b,0x00,0xff }, /*    55 */
  {    65,  -151,    37,0, 0x0000,0x0000, 0xa0,0x50,0x00,0xff }, /*    56 */
  {    10,  -119,    16,0, 0x0000,0x0000, 0xb5,0x5a,0x00,0xff }, /*    57 */
  {     5,  -151,    17,0, 0x0000,0x0000, 0x6c,0x36,0x00,0xff }, /*    58 */
  {    13,  -151,    46,0, 0x0000,0x0000, 0x63,0x31,0x00,0xff }, /*    59 */
  {    59,  -151,     7,0, 0x0000,0x0000, 0xa0,0x50,0x00,0xff }, /*    60 */
  {    65,  -151,    37,0, 0x0000,0x0000, 0xa0,0x50,0x00,0xff }, /*    61 */
  {    13,  -151,    46,0, 0x0000,0x0000, 0x63,0x31,0x00,0xff }, /*    62 */
  {    10,  -129,   -24,0, 0x0000,0x0000, 0xb7,0x5b,0x00,0xff }, /*    63 */
  {    41,  -129,   -29,0, 0x0000,0x0000, 0xda,0x6d,0x00,0xff }, /*    64 */
  {    44,  -151,   -30,0, 0x0000,0x0000, 0x9e,0x4f,0x00,0xff }, /*    65 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_336_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 32, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP2Triangles(  3,  4,  5,  0,  6,  7,  8,  0),
  gsSP2Triangles(  9, 10, 11,  0, 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 30, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP2Triangles(  1,  2,  3,  0,  4,  5,  6,  0),
  gsSP1Triangle(  7,  8,  9,  0),
  gsSP1Triangle( 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsSP1Triangle( 16, 17, 18,  0),
  gsSP1Triangle( 19, 20, 21,  0),
  gsSP1Triangle( 22, 23, 24,  0),
  gsSP1Triangle( 25, 26, 27,  0),
  gsSPVertex( SEGADRS(5, 992), 2, 30 ),
  gsSP1Triangle( 28, 29, 30,  0),
  gsSPVertex( SEGADRS(5, 1024), 2, 0 ),
  gsSP1Triangle( 31,  0,  1,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_336_ = {
  gfx_Shape_of___POLYHEDRON_336_,
  vtx_Shape_of___POLYHEDRON_336_, 66, 5,
  bmp_frake_walk,
  tlut_frake_walk,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_walk_aligner14[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_324_[] = {
  {    46,   -16,    12,0, 0x0000,0x0000, 0x00,0x6c,0x00,0xff }, /*     0 */
  {     0,   -70,    15,0, 0x0000,0x0000, 0x00,0x37,0x00,0xff }, /*     1 */
  {    53,   -70,    15,0, 0x0000,0x0000, 0x00,0x4f,0x00,0xff }, /*     2 */
  {    46,   -16,    12,0, 0x0000,0x0000, 0x00,0x6c,0x00,0xff }, /*     3 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x5a,0x00,0xff }, /*     4 */
  {     0,   -70,    15,0, 0x0000,0x0000, 0x00,0x37,0x00,0xff }, /*     5 */
  {     0,    -9,   -35,0, 0x0000,0x0000, 0x00,0x4d,0x00,0xff }, /*     6 */
  {    45,   -16,   -35,0, 0x0000,0x0000, 0x00,0x57,0x00,0xff }, /*     7 */
  {     0,   -70,   -37,0, 0x0000,0x0000, 0x00,0x3b,0x00,0xff }, /*     8 */
  {    45,   -16,   -35,0, 0x0000,0x0000, 0x00,0x57,0x00,0xff }, /*     9 */
  {    53,   -70,   -37,0, 0x0000,0x0000, 0x00,0x54,0x00,0xff }, /*    10 */
  {     0,   -70,   -37,0, 0x0000,0x0000, 0x00,0x3b,0x00,0xff }, /*    11 */
  {    24,     7,   -24,0, 0x0000,0x0000, 0x00,0x5f,0x00,0xff }, /*    12 */
  {     8,     7,   -24,0, 0x0000,0x0000, 0x00,0x43,0x00,0xff }, /*    13 */
  {    24,     7,    -3,0, 0x0000,0x0000, 0x00,0x6e,0x00,0xff }, /*    14 */
  {     8,     7,   -24,0, 0x0000,0x0000, 0x00,0x43,0x00,0xff }, /*    15 */
  {     8,     7,    -3,0, 0x0000,0x0000, 0x00,0x5c,0x00,0xff }, /*    16 */
  {    24,     7,    -3,0, 0x0000,0x0000, 0x00,0x6e,0x00,0xff }, /*    17 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x5a,0x00,0xff }, /*    18 */
  {     0,   -70,   -37,0, 0x0000,0x0000, 0x00,0x3b,0x00,0xff }, /*    19 */
  {     0,   -70,    15,0, 0x0000,0x0000, 0x00,0x37,0x00,0xff }, /*    20 */
  {    45,   -16,   -35,0, 0x0000,0x0000, 0x00,0x57,0x00,0xff }, /*    21 */
  {    46,   -16,    12,0, 0x0000,0x0000, 0x00,0x6c,0x00,0xff }, /*    22 */
  {    53,   -70,   -37,0, 0x0000,0x0000, 0x00,0x54,0x00,0xff }, /*    23 */
  {    24,     7,   -24,0, 0x0000,0x0000, 0x00,0x5f,0x00,0xff }, /*    24 */
  {    24,     7,    -3,0, 0x0000,0x0000, 0x00,0x6e,0x00,0xff }, /*    25 */
  {    46,   -16,    12,0, 0x0000,0x0000, 0x00,0x6c,0x00,0xff }, /*    26 */
  {    46,   -16,    12,0, 0x0000,0x0000, 0x00,0x6c,0x00,0xff }, /*    27 */
  {    24,     7,    -3,0, 0x0000,0x0000, 0x00,0x6e,0x00,0xff }, /*    28 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x5a,0x00,0xff }, /*    29 */
  {    45,   -16,   -35,0, 0x0000,0x0000, 0x00,0x57,0x00,0xff }, /*    30 */
  {    24,     7,   -24,0, 0x0000,0x0000, 0x00,0x5f,0x00,0xff }, /*    31 */
  {    46,   -16,    12,0, 0x0000,0x0000, 0x00,0x6c,0x00,0xff }, /*    32 */
  {     0,    -9,   -35,0, 0x0000,0x0000, 0x00,0x4d,0x00,0xff }, /*    33 */
  {    24,     7,   -24,0, 0x0000,0x0000, 0x00,0x5f,0x00,0xff }, /*    34 */
  {    45,   -16,   -35,0, 0x0000,0x0000, 0x00,0x57,0x00,0xff }, /*    35 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x5a,0x00,0xff }, /*    36 */
  {     8,     7,   -24,0, 0x0000,0x0000, 0x00,0x43,0x00,0xff }, /*    37 */
  {     0,    -9,   -35,0, 0x0000,0x0000, 0x00,0x4d,0x00,0xff }, /*    38 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x5a,0x00,0xff }, /*    39 */
  {     8,     7,    -3,0, 0x0000,0x0000, 0x00,0x5c,0x00,0xff }, /*    40 */
  {     8,     7,   -24,0, 0x0000,0x0000, 0x00,0x43,0x00,0xff }, /*    41 */
  {     0,    -9,   -35,0, 0x0000,0x0000, 0x00,0x4d,0x00,0xff }, /*    42 */
  {     8,     7,   -24,0, 0x0000,0x0000, 0x00,0x43,0x00,0xff }, /*    43 */
  {    24,     7,   -24,0, 0x0000,0x0000, 0x00,0x5f,0x00,0xff }, /*    44 */
  {    24,     7,    -3,0, 0x0000,0x0000, 0x00,0x6e,0x00,0xff }, /*    45 */
  {     8,     7,    -3,0, 0x0000,0x0000, 0x00,0x5c,0x00,0xff }, /*    46 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x5a,0x00,0xff }, /*    47 */
  {    46,   -16,    12,0, 0x0000,0x0000, 0x00,0x6c,0x00,0xff }, /*    48 */
  {    53,   -70,    15,0, 0x0000,0x0000, 0x00,0x4f,0x00,0xff }, /*    49 */
  {    53,   -70,   -37,0, 0x0000,0x0000, 0x00,0x54,0x00,0xff }, /*    50 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x5a,0x00,0xff }, /*    51 */
  {     0,    -9,   -35,0, 0x0000,0x0000, 0x00,0x4d,0x00,0xff }, /*    52 */
  {     0,   -70,   -37,0, 0x0000,0x0000, 0x00,0x3b,0x00,0xff }, /*    53 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_324_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 32, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP2Triangles(  6,  7,  8,  0,  9, 10, 11,  0),
  gsSP2Triangles( 12, 13, 14,  0, 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP2Triangles( 21, 22, 23,  0, 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 22, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP1Triangle(  1,  2,  3,  0),
  gsSP2Triangles(  4,  5,  6,  0,  7,  8,  9,  0),
  gsSP1Triangle( 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsSP1Triangle( 16, 17, 18,  0),
  gsSP1Triangle( 19, 20, 21,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_324_ = {
  gfx_Shape_of___POLYHEDRON_324_,
  vtx_Shape_of___POLYHEDRON_324_, 54, 5,
  bmp_frake_walk,
  tlut_frake_walk,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_walk_aligner15[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_322_[] = {
  {     5,    -7,    -4,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*     0 */
  {   -30,    11,    22,0, 0x0000,0x0000, 0x00,0x4f,0x00,0xff }, /*     1 */
  {    -5,    -7,    -4,0, 0x0000,0x0000, 0x00,0x44,0x00,0xff }, /*     2 */
  {     5,    -7,    -4,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*     3 */
  {    30,    11,    22,0, 0x0000,0x0000, 0x00,0x60,0x00,0xff }, /*     4 */
  {   -30,    11,    22,0, 0x0000,0x0000, 0x00,0x4f,0x00,0xff }, /*     5 */
  {    -6,    -7,   -15,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*     6 */
  {    30,    11,   -37,0, 0x0000,0x0000, 0x00,0x57,0x00,0xff }, /*     7 */
  {     6,    -7,   -15,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*     8 */
  {    -6,    -7,   -15,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*     9 */
  {   -30,    11,   -37,0, 0x0000,0x0000, 0x00,0x4d,0x00,0xff }, /*    10 */
  {    30,    11,   -37,0, 0x0000,0x0000, 0x00,0x57,0x00,0xff }, /*    11 */
  {   -30,    11,    22,0, 0x0000,0x0000, 0x00,0x4f,0x00,0xff }, /*    12 */
  {    30,    11,   -37,0, 0x0000,0x0000, 0x00,0x57,0x00,0xff }, /*    13 */
  {   -30,    11,   -37,0, 0x0000,0x0000, 0x00,0x4d,0x00,0xff }, /*    14 */
  {   -30,    11,    22,0, 0x0000,0x0000, 0x00,0x4f,0x00,0xff }, /*    15 */
  {    30,    11,    22,0, 0x0000,0x0000, 0x00,0x60,0x00,0xff }, /*    16 */
  {    30,    11,   -37,0, 0x0000,0x0000, 0x00,0x57,0x00,0xff }, /*    17 */
  {     6,    -7,   -15,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*    18 */
  {    30,    11,    22,0, 0x0000,0x0000, 0x00,0x60,0x00,0xff }, /*    19 */
  {     5,    -7,    -4,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*    20 */
  {     6,    -7,   -15,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*    21 */
  {    30,    11,   -37,0, 0x0000,0x0000, 0x00,0x57,0x00,0xff }, /*    22 */
  {    30,    11,    22,0, 0x0000,0x0000, 0x00,0x60,0x00,0xff }, /*    23 */
  {    -5,    -7,    -4,0, 0x0000,0x0000, 0x00,0x44,0x00,0xff }, /*    24 */
  {   -30,    11,    22,0, 0x0000,0x0000, 0x00,0x4f,0x00,0xff }, /*    25 */
  {    -6,    -7,   -15,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*    26 */
  {     6,    -7,   -15,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*    27 */
  {    -5,    -7,    -4,0, 0x0000,0x0000, 0x00,0x44,0x00,0xff }, /*    28 */
  {    -6,    -7,   -15,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*    29 */
  {     6,    -7,   -15,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*    30 */
  {     5,    -7,    -4,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*    31 */
  {    -5,    -7,    -4,0, 0x0000,0x0000, 0x00,0x44,0x00,0xff }, /*    32 */
  {   -30,    11,    22,0, 0x0000,0x0000, 0x00,0x4f,0x00,0xff }, /*    33 */
  {   -30,    11,   -37,0, 0x0000,0x0000, 0x00,0x4d,0x00,0xff }, /*    34 */
  {    -6,    -7,   -15,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*    35 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_322_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 32, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP2Triangles(  6,  7,  8,  0,  9, 10, 11,  0),
  gsSP2Triangles( 12, 13, 14,  0, 15, 16, 17,  0),
  gsSP2Triangles( 18, 19, 20,  0, 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 4, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP1Triangle(  1,  2,  3,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_322_ = {
  gfx_Shape_of___POLYHEDRON_322_,
  vtx_Shape_of___POLYHEDRON_322_, 36, 5,
  bmp_frake_walk,
  tlut_frake_walk,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_walk_aligner16[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_320_[] = {
  {   -53,   -70,    15,0, 0x0000,0x0000, 0x00,0x39,0x00,0xff }, /*     0 */
  {     0,   -70,    15,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*     1 */
  {   -46,   -16,    12,0, 0x0000,0x0000, 0x00,0x5a,0x00,0xff }, /*     2 */
  {     0,   -70,   -37,0, 0x0000,0x0000, 0x00,0x56,0x00,0xff }, /*     3 */
  {   -45,   -16,   -35,0, 0x0000,0x0000, 0x00,0x49,0x00,0xff }, /*     4 */
  {     1,    -9,   -35,0, 0x0000,0x0000, 0x00,0x5e,0x00,0xff }, /*     5 */
  {     0,   -70,   -37,0, 0x0000,0x0000, 0x00,0x56,0x00,0xff }, /*     6 */
  {   -53,   -70,   -37,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*     7 */
  {   -45,   -16,   -35,0, 0x0000,0x0000, 0x00,0x49,0x00,0xff }, /*     8 */
  {   -24,     7,    -3,0, 0x0000,0x0000, 0x00,0x5e,0x00,0xff }, /*     9 */
  {    -8,     7,   -24,0, 0x0000,0x0000, 0x00,0x72,0x00,0xff }, /*    10 */
  {   -24,     7,   -24,0, 0x0000,0x0000, 0x00,0x56,0x00,0xff }, /*    11 */
  {   -24,     7,    -3,0, 0x0000,0x0000, 0x00,0x5e,0x00,0xff }, /*    12 */
  {    -8,     7,    -3,0, 0x0000,0x0000, 0x00,0x6d,0x00,0xff }, /*    13 */
  {    -8,     7,   -24,0, 0x0000,0x0000, 0x00,0x72,0x00,0xff }, /*    14 */
  {     0,   -70,    15,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*    15 */
  {     0,   -70,   -37,0, 0x0000,0x0000, 0x00,0x56,0x00,0xff }, /*    16 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x64,0x00,0xff }, /*    17 */
  {   -53,   -70,   -37,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*    18 */
  {   -46,   -16,    12,0, 0x0000,0x0000, 0x00,0x5a,0x00,0xff }, /*    19 */
  {   -45,   -16,   -35,0, 0x0000,0x0000, 0x00,0x49,0x00,0xff }, /*    20 */
  {   -45,   -16,   -35,0, 0x0000,0x0000, 0x00,0x49,0x00,0xff }, /*    21 */
  {   -24,     7,   -24,0, 0x0000,0x0000, 0x00,0x56,0x00,0xff }, /*    22 */
  {     1,    -9,   -35,0, 0x0000,0x0000, 0x00,0x5e,0x00,0xff }, /*    23 */
  {   -53,   -70,   -37,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*    24 */
  {   -53,   -70,    15,0, 0x0000,0x0000, 0x00,0x39,0x00,0xff }, /*    25 */
  {   -46,   -16,    12,0, 0x0000,0x0000, 0x00,0x5a,0x00,0xff }, /*    26 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x64,0x00,0xff }, /*    27 */
  {   -24,     7,    -3,0, 0x0000,0x0000, 0x00,0x5e,0x00,0xff }, /*    28 */
  {   -46,   -16,    12,0, 0x0000,0x0000, 0x00,0x5a,0x00,0xff }, /*    29 */
  {   -46,   -16,    12,0, 0x0000,0x0000, 0x00,0x5a,0x00,0xff }, /*    30 */
  {   -24,     7,   -24,0, 0x0000,0x0000, 0x00,0x56,0x00,0xff }, /*    31 */
  {   -45,   -16,   -35,0, 0x0000,0x0000, 0x00,0x49,0x00,0xff }, /*    32 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x64,0x00,0xff }, /*    33 */
  {    -8,     7,    -3,0, 0x0000,0x0000, 0x00,0x6d,0x00,0xff }, /*    34 */
  {   -24,     7,    -3,0, 0x0000,0x0000, 0x00,0x5e,0x00,0xff }, /*    35 */
  {     1,    -9,   -35,0, 0x0000,0x0000, 0x00,0x5e,0x00,0xff }, /*    36 */
  {    -8,     7,   -24,0, 0x0000,0x0000, 0x00,0x72,0x00,0xff }, /*    37 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x64,0x00,0xff }, /*    38 */
  {    -8,     7,   -24,0, 0x0000,0x0000, 0x00,0x72,0x00,0xff }, /*    39 */
  {    -8,     7,    -3,0, 0x0000,0x0000, 0x00,0x6d,0x00,0xff }, /*    40 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x64,0x00,0xff }, /*    41 */
  {   -24,     7,   -24,0, 0x0000,0x0000, 0x00,0x56,0x00,0xff }, /*    42 */
  {    -8,     7,   -24,0, 0x0000,0x0000, 0x00,0x72,0x00,0xff }, /*    43 */
  {     1,    -9,   -35,0, 0x0000,0x0000, 0x00,0x5e,0x00,0xff }, /*    44 */
  {   -46,   -16,    12,0, 0x0000,0x0000, 0x00,0x5a,0x00,0xff }, /*    45 */
  {   -24,     7,    -3,0, 0x0000,0x0000, 0x00,0x5e,0x00,0xff }, /*    46 */
  {   -24,     7,   -24,0, 0x0000,0x0000, 0x00,0x56,0x00,0xff }, /*    47 */
  {     0,   -70,   -37,0, 0x0000,0x0000, 0x00,0x56,0x00,0xff }, /*    48 */
  {     1,    -9,   -35,0, 0x0000,0x0000, 0x00,0x5e,0x00,0xff }, /*    49 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x64,0x00,0xff }, /*    50 */
  {     0,   -70,    15,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*    51 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x64,0x00,0xff }, /*    52 */
  {   -46,   -16,    12,0, 0x0000,0x0000, 0x00,0x5a,0x00,0xff }, /*    53 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_320_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 32, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP2Triangles(  3,  4,  5,  0,  6,  7,  8,  0),
  gsSP2Triangles(  9, 10, 11,  0, 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsSP2Triangles( 18, 19, 20,  0, 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 22, 0 ),
  gsSP2Triangles( 30, 31,  0,  0,  1,  2,  3,  0),
  gsSP2Triangles(  4,  5,  6,  0,  7,  8,  9,  0),
  gsSP1Triangle( 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsSP1Triangle( 16, 17, 18,  0),
  gsSP1Triangle( 19, 20, 21,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_320_ = {
  gfx_Shape_of___POLYHEDRON_320_,
  vtx_Shape_of___POLYHEDRON_320_, 54, 5,
  bmp_frake_walk,
  tlut_frake_walk,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_walk_aligner17[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_330_[] = {
  {   -13,  -104,     3,0, 0x0000,0x0000, 0xff,0xcf,0x95,0xff }, /*     0 */
  {   -38,   -80,     3,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*     1 */
  {   -38,  -104,     3,0, 0x0000,0x0000, 0xd0,0x96,0x6c,0xff }, /*     2 */
  {   -38,  -104,   -22,0, 0x0000,0x0000, 0xe5,0xa5,0x77,0xff }, /*     3 */
  {   -13,   -80,   -22,0, 0x0000,0x0000, 0xff,0xd8,0x9b,0xff }, /*     4 */
  {   -13,  -104,   -22,0, 0x0000,0x0000, 0xff,0xc8,0x8f,0xff }, /*     5 */
  {   -38,  -104,   -22,0, 0x0000,0x0000, 0xe5,0xa5,0x77,0xff }, /*     6 */
  {   -38,   -80,   -22,0, 0x0000,0x0000, 0xe1,0xa2,0x75,0xff }, /*     7 */
  {   -13,   -80,   -22,0, 0x0000,0x0000, 0xff,0xd8,0x9b,0xff }, /*     8 */
  {   -13,  -104,     3,0, 0x0000,0x0000, 0xff,0xcf,0x95,0xff }, /*     9 */
  {   -13,   -80,   -22,0, 0x0000,0x0000, 0xff,0xd8,0x9b,0xff }, /*    10 */
  {   -13,   -80,     3,0, 0x0000,0x0000, 0xff,0xe3,0xa3,0xff }, /*    11 */
  {   -38,  -104,   -22,0, 0x0000,0x0000, 0xe5,0xa5,0x77,0xff }, /*    12 */
  {   -38,   -80,     3,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*    13 */
  {   -38,   -80,   -22,0, 0x0000,0x0000, 0xe1,0xa2,0x75,0xff }, /*    14 */
  {   -13,   -80,     3,0, 0x0000,0x0000, 0xff,0xe3,0xa3,0xff }, /*    15 */
  {   -26,   -67,    -9,0, 0x0000,0x0000, 0xff,0x88,0x61,0xff }, /*    16 */
  {   -38,   -80,     3,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*    17 */
  {   -38,   -80,     3,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*    18 */
  {   -26,   -67,    -9,0, 0x0000,0x0000, 0xff,0x88,0x61,0xff }, /*    19 */
  {   -38,   -80,   -22,0, 0x0000,0x0000, 0xe1,0xa2,0x75,0xff }, /*    20 */
  {   -38,   -80,   -22,0, 0x0000,0x0000, 0xe1,0xa2,0x75,0xff }, /*    21 */
  {   -26,   -67,    -9,0, 0x0000,0x0000, 0xff,0x88,0x61,0xff }, /*    22 */
  {   -13,   -80,   -22,0, 0x0000,0x0000, 0xff,0xd8,0x9b,0xff }, /*    23 */
  {   -13,   -80,   -22,0, 0x0000,0x0000, 0xff,0xd8,0x9b,0xff }, /*    24 */
  {   -26,   -67,    -9,0, 0x0000,0x0000, 0xff,0x88,0x61,0xff }, /*    25 */
  {   -13,   -80,     3,0, 0x0000,0x0000, 0xff,0xe3,0xa3,0xff }, /*    26 */
  {   -38,  -104,   -22,0, 0x0000,0x0000, 0x8e,0x47,0x00,0xff }, /*    27 */
  {   -38,  -138,     3,0, 0x0000,0x0000, 0x64,0x32,0x00,0xff }, /*    28 */
  {   -38,  -104,     3,0, 0x0000,0x0000, 0x81,0x40,0x00,0xff }, /*    29 */
  {   -38,  -104,   -22,0, 0x0000,0x0000, 0x8e,0x47,0x00,0xff }, /*    30 */
  {   -38,  -138,   -22,0, 0x0000,0x0000, 0x65,0x32,0x00,0xff }, /*    31 */
  {   -38,  -138,     3,0, 0x0000,0x0000, 0x64,0x32,0x00,0xff }, /*    32 */
  {   -13,  -104,   -22,0, 0x0000,0x0000, 0xab,0x55,0x00,0xff }, /*    33 */
  {   -38,  -138,   -22,0, 0x0000,0x0000, 0x65,0x32,0x00,0xff }, /*    34 */
  {   -38,  -104,   -22,0, 0x0000,0x0000, 0x8e,0x47,0x00,0xff }, /*    35 */
  {   -13,  -104,   -22,0, 0x0000,0x0000, 0xab,0x55,0x00,0xff }, /*    36 */
  {   -13,  -138,   -22,0, 0x0000,0x0000, 0x9a,0x4d,0x00,0xff }, /*    37 */
  {   -38,  -138,   -22,0, 0x0000,0x0000, 0x65,0x32,0x00,0xff }, /*    38 */
  {   -13,  -104,     3,0, 0x0000,0x0000, 0xb2,0x59,0x00,0xff }, /*    39 */
  {   -13,  -138,     3,0, 0x0000,0x0000, 0x74,0x3a,0x00,0xff }, /*    40 */
  {   -13,  -104,   -22,0, 0x0000,0x0000, 0xab,0x55,0x00,0xff }, /*    41 */
  {   -38,  -104,     3,0, 0x0000,0x0000, 0x81,0x40,0x00,0xff }, /*    42 */
  {   -13,  -138,     3,0, 0x0000,0x0000, 0x74,0x3a,0x00,0xff }, /*    43 */
  {   -13,  -104,     3,0, 0x0000,0x0000, 0xb2,0x59,0x00,0xff }, /*    44 */
  {   -38,  -104,     3,0, 0x0000,0x0000, 0x81,0x40,0x00,0xff }, /*    45 */
  {   -38,  -138,     3,0, 0x0000,0x0000, 0x64,0x32,0x00,0xff }, /*    46 */
  {   -13,  -138,     3,0, 0x0000,0x0000, 0x74,0x3a,0x00,0xff }, /*    47 */
  {   -13,  -138,     3,0, 0x0000,0x0000, 0x74,0x3a,0x00,0xff }, /*    48 */
  {   -13,  -138,   -22,0, 0x0000,0x0000, 0x9a,0x4d,0x00,0xff }, /*    49 */
  {   -13,  -104,   -22,0, 0x0000,0x0000, 0xab,0x55,0x00,0xff }, /*    50 */
  {   -38,  -104,   -22,0, 0x0000,0x0000, 0xe5,0xa5,0x77,0xff }, /*    51 */
  {   -38,  -104,     3,0, 0x0000,0x0000, 0xd0,0x96,0x6c,0xff }, /*    52 */
  {   -38,   -80,     3,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*    53 */
  {   -13,  -104,     3,0, 0x0000,0x0000, 0xff,0xcf,0x95,0xff }, /*    54 */
  {   -13,  -104,   -22,0, 0x0000,0x0000, 0xff,0xc8,0x8f,0xff }, /*    55 */
  {   -13,   -80,   -22,0, 0x0000,0x0000, 0xff,0xd8,0x9b,0xff }, /*    56 */
  {   -13,  -104,     3,0, 0x0000,0x0000, 0xff,0xcf,0x95,0xff }, /*    57 */
  {   -13,   -80,     3,0, 0x0000,0x0000, 0xff,0xe3,0xa3,0xff }, /*    58 */
  {   -38,   -80,     3,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*    59 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_330_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 32, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP2Triangles(  3,  4,  5,  0,  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 28, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP2Triangles(  1,  2,  3,  0,  4,  5,  6,  0),
  gsSP1Triangle(  7,  8,  9,  0),
  gsSP2Triangles( 10, 11, 12,  0, 13, 14, 15,  0),
  gsSP1Triangle( 16, 17, 18,  0),
  gsSP1Triangle( 19, 20, 21,  0),
  gsSP1Triangle( 22, 23, 24,  0),
  gsSP1Triangle( 25, 26, 27,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_330_ = {
  gfx_Shape_of___POLYHEDRON_330_,
  vtx_Shape_of___POLYHEDRON_330_, 60, 5,
  bmp_frake_walk,
  tlut_frake_walk,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_walk_aligner18[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_334_[] = {
  {    -5,  -151,   -23,0, 0x0000,0x0000, 0x9c,0x4e,0x00,0xff }, /*     0 */
  {   -44,  -151,   -30,0, 0x0000,0x0000, 0x56,0x2b,0x00,0xff }, /*     1 */
  {   -10,  -129,   -24,0, 0x0000,0x0000, 0xc8,0x64,0x00,0xff }, /*     2 */
  {   -52,  -109,    39,0, 0x0000,0x0000, 0xbd,0x5e,0x00,0xff }, /*     3 */
  {   -10,  -119,    16,0, 0x0000,0x0000, 0xd2,0x69,0x00,0xff }, /*     4 */
  {   -55,  -119,     8,0, 0x0000,0x0000, 0xb1,0x58,0x00,0xff }, /*     5 */
  {   -10,  -119,    16,0, 0x0000,0x0000, 0xd2,0x69,0x00,0xff }, /*     6 */
  {   -10,  -129,   -24,0, 0x0000,0x0000, 0xc8,0x64,0x00,0xff }, /*     7 */
  {   -55,  -119,     8,0, 0x0000,0x0000, 0xb1,0x58,0x00,0xff }, /*     8 */
  {   -52,  -109,    39,0, 0x0000,0x0000, 0xbd,0x5e,0x00,0xff }, /*     9 */
  {   -18,  -109,    45,0, 0x0000,0x0000, 0xc8,0x64,0x00,0xff }, /*    10 */
  {   -10,  -119,    16,0, 0x0000,0x0000, 0xd2,0x69,0x00,0xff }, /*    11 */
  {    -5,  -151,    17,0, 0x0000,0x0000, 0x9c,0x4e,0x00,0xff }, /*    12 */
  {   -13,  -151,    46,0, 0x0000,0x0000, 0x78,0x3c,0x00,0xff }, /*    13 */
  {   -59,  -151,     7,0, 0x0000,0x0000, 0x64,0x32,0x00,0xff }, /*    14 */
  {   -59,  -151,     7,0, 0x0000,0x0000, 0x64,0x32,0x00,0xff }, /*    15 */
  {   -44,  -151,   -30,0, 0x0000,0x0000, 0x56,0x2b,0x00,0xff }, /*    16 */
  {    -5,  -151,    17,0, 0x0000,0x0000, 0x9c,0x4e,0x00,0xff }, /*    17 */
  {   -13,  -151,    46,0, 0x0000,0x0000, 0x78,0x3c,0x00,0xff }, /*    18 */
  {   -10,  -119,    16,0, 0x0000,0x0000, 0xd2,0x69,0x00,0xff }, /*    19 */
  {   -18,  -109,    45,0, 0x0000,0x0000, 0xc8,0x64,0x00,0xff }, /*    20 */
  {   -10,  -129,   -24,0, 0x0000,0x0000, 0xc8,0x64,0x00,0xff }, /*    21 */
  {   -41,  -129,   -29,0, 0x0000,0x0000, 0x88,0x44,0x00,0xff }, /*    22 */
  {   -55,  -119,     8,0, 0x0000,0x0000, 0xb1,0x58,0x00,0xff }, /*    23 */
  {   -59,  -151,     7,0, 0x0000,0x0000, 0x64,0x32,0x00,0xff }, /*    24 */
  {   -65,  -151,    37,0, 0x0000,0x0000, 0x6b,0x35,0x00,0xff }, /*    25 */
  {   -55,  -119,     8,0, 0x0000,0x0000, 0xb1,0x58,0x00,0xff }, /*    26 */
  {   -18,  -109,    45,0, 0x0000,0x0000, 0xc8,0x64,0x00,0xff }, /*    27 */
  {   -39,  -138,    62,0, 0x0000,0x0000, 0x65,0x32,0x00,0xff }, /*    28 */
  {   -13,  -151,    46,0, 0x0000,0x0000, 0x78,0x3c,0x00,0xff }, /*    29 */
  {   -13,  -151,    46,0, 0x0000,0x0000, 0x78,0x3c,0x00,0xff }, /*    30 */
  {   -39,  -138,    62,0, 0x0000,0x0000, 0x65,0x32,0x00,0xff }, /*    31 */
  {   -65,  -151,    37,0, 0x0000,0x0000, 0x6b,0x35,0x00,0xff }, /*    32 */
  {   -65,  -151,    37,0, 0x0000,0x0000, 0x6b,0x35,0x00,0xff }, /*    33 */
  {   -39,  -138,    62,0, 0x0000,0x0000, 0x65,0x32,0x00,0xff }, /*    34 */
  {   -52,  -109,    39,0, 0x0000,0x0000, 0xbd,0x5e,0x00,0xff }, /*    35 */
  {   -52,  -109,    39,0, 0x0000,0x0000, 0xbd,0x5e,0x00,0xff }, /*    36 */
  {   -39,  -138,    62,0, 0x0000,0x0000, 0x65,0x32,0x00,0xff }, /*    37 */
  {   -18,  -109,    45,0, 0x0000,0x0000, 0xc8,0x64,0x00,0xff }, /*    38 */
  {    -5,  -151,    17,0, 0x0000,0x0000, 0x9c,0x4e,0x00,0xff }, /*    39 */
  {    -5,  -151,   -23,0, 0x0000,0x0000, 0x9c,0x4e,0x00,0xff }, /*    40 */
  {   -10,  -119,    16,0, 0x0000,0x0000, 0xd2,0x69,0x00,0xff }, /*    41 */
  {   -55,  -119,     8,0, 0x0000,0x0000, 0xb1,0x58,0x00,0xff }, /*    42 */
  {   -41,  -129,   -29,0, 0x0000,0x0000, 0x88,0x44,0x00,0xff }, /*    43 */
  {   -59,  -151,     7,0, 0x0000,0x0000, 0x64,0x32,0x00,0xff }, /*    44 */
  {   -41,  -129,   -29,0, 0x0000,0x0000, 0x88,0x44,0x00,0xff }, /*    45 */
  {   -44,  -151,   -30,0, 0x0000,0x0000, 0x56,0x2b,0x00,0xff }, /*    46 */
  {   -59,  -151,     7,0, 0x0000,0x0000, 0x64,0x32,0x00,0xff }, /*    47 */
  {   -44,  -151,   -30,0, 0x0000,0x0000, 0x56,0x2b,0x00,0xff }, /*    48 */
  {    -5,  -151,   -23,0, 0x0000,0x0000, 0x9c,0x4e,0x00,0xff }, /*    49 */
  {    -5,  -151,    17,0, 0x0000,0x0000, 0x9c,0x4e,0x00,0xff }, /*    50 */
  {    -5,  -151,   -23,0, 0x0000,0x0000, 0x9c,0x4e,0x00,0xff }, /*    51 */
  {   -10,  -129,   -24,0, 0x0000,0x0000, 0xc8,0x64,0x00,0xff }, /*    52 */
  {   -10,  -119,    16,0, 0x0000,0x0000, 0xd2,0x69,0x00,0xff }, /*    53 */
  {   -65,  -151,    37,0, 0x0000,0x0000, 0x6b,0x35,0x00,0xff }, /*    54 */
  {   -52,  -109,    39,0, 0x0000,0x0000, 0xbd,0x5e,0x00,0xff }, /*    55 */
  {   -55,  -119,     8,0, 0x0000,0x0000, 0xb1,0x58,0x00,0xff }, /*    56 */
  {   -13,  -151,    46,0, 0x0000,0x0000, 0x78,0x3c,0x00,0xff }, /*    57 */
  {    -5,  -151,    17,0, 0x0000,0x0000, 0x9c,0x4e,0x00,0xff }, /*    58 */
  {   -10,  -119,    16,0, 0x0000,0x0000, 0xd2,0x69,0x00,0xff }, /*    59 */
  {   -13,  -151,    46,0, 0x0000,0x0000, 0x78,0x3c,0x00,0xff }, /*    60 */
  {   -65,  -151,    37,0, 0x0000,0x0000, 0x6b,0x35,0x00,0xff }, /*    61 */
  {   -59,  -151,     7,0, 0x0000,0x0000, 0x64,0x32,0x00,0xff }, /*    62 */
  {   -44,  -151,   -30,0, 0x0000,0x0000, 0x56,0x2b,0x00,0xff }, /*    63 */
  {   -41,  -129,   -29,0, 0x0000,0x0000, 0x88,0x44,0x00,0xff }, /*    64 */
  {   -10,  -129,   -24,0, 0x0000,0x0000, 0xc8,0x64,0x00,0xff }, /*    65 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_334_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 32, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP2Triangles(  3,  4,  5,  0,  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsSP2Triangles( 12, 13, 14,  0, 15, 16, 17,  0),
  gsSP2Triangles( 18, 19, 20,  0, 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 30, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP1Triangle(  1,  2,  3,  0),
  gsSP1Triangle(  4,  5,  6,  0),
  gsSP1Triangle(  7,  8,  9,  0),
  gsSP1Triangle( 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsSP1Triangle( 16, 17, 18,  0),
  gsSP1Triangle( 19, 20, 21,  0),
  gsSP1Triangle( 22, 23, 24,  0),
  gsSP1Triangle( 25, 26, 27,  0),
  gsSPVertex( SEGADRS(5, 992), 2, 30 ),
  gsSP1Triangle( 28, 29, 30,  0),
  gsSPVertex( SEGADRS(5, 1024), 2, 0 ),
  gsSP1Triangle( 31,  0,  1,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_334_ = {
  gfx_Shape_of___POLYHEDRON_334_,
  vtx_Shape_of___POLYHEDRON_334_, 66, 5,
  bmp_frake_walk,
  tlut_frake_walk,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_walk_aligner19[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_326_[] = {
  {   -38,   -53,   -22,0, 0x0000,0x0000, 0xe1,0xa3,0x75,0xff }, /*     0 */
  {   -38,   -75,     3,0, 0x0000,0x0000, 0xaf,0x7e,0x5b,0xff }, /*     1 */
  {   -38,   -53,     3,0, 0x0000,0x0000, 0xff,0xd0,0x95,0xff }, /*     2 */
  {   -38,   -53,   -22,0, 0x0000,0x0000, 0xe1,0xa3,0x75,0xff }, /*     3 */
  {   -38,   -75,   -22,0, 0x0000,0x0000, 0xc7,0x90,0x67,0xff }, /*     4 */
  {   -38,   -75,     3,0, 0x0000,0x0000, 0xaf,0x7e,0x5b,0xff }, /*     5 */
  {   -13,   -53,   -22,0, 0x0000,0x0000, 0xff,0xe7,0xa6,0xff }, /*     6 */
  {   -38,   -75,   -22,0, 0x0000,0x0000, 0xc7,0x90,0x67,0xff }, /*     7 */
  {   -38,   -53,   -22,0, 0x0000,0x0000, 0xe1,0xa3,0x75,0xff }, /*     8 */
  {   -13,   -53,   -22,0, 0x0000,0x0000, 0xff,0xe7,0xa6,0xff }, /*     9 */
  {   -13,   -75,   -22,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*    10 */
  {   -38,   -75,   -22,0, 0x0000,0x0000, 0xc7,0x90,0x67,0xff }, /*    11 */
  {   -13,   -53,     3,0, 0x0000,0x0000, 0xff,0xde,0x9f,0xff }, /*    12 */
  {   -13,   -75,     3,0, 0x0000,0x0000, 0xd1,0x97,0x6d,0xff }, /*    13 */
  {   -13,   -53,   -22,0, 0x0000,0x0000, 0xff,0xe7,0xa6,0xff }, /*    14 */
  {   -38,   -53,     3,0, 0x0000,0x0000, 0xff,0xd0,0x95,0xff }, /*    15 */
  {   -13,   -75,     3,0, 0x0000,0x0000, 0xd1,0x97,0x6d,0xff }, /*    16 */
  {   -13,   -53,     3,0, 0x0000,0x0000, 0xff,0xde,0x9f,0xff }, /*    17 */
  {   -38,   -53,     3,0, 0x0000,0x0000, 0xff,0xd0,0x95,0xff }, /*    18 */
  {   -38,   -75,     3,0, 0x0000,0x0000, 0xaf,0x7e,0x5b,0xff }, /*    19 */
  {   -13,   -75,     3,0, 0x0000,0x0000, 0xd1,0x97,0x6d,0xff }, /*    20 */
  {   -13,   -75,     3,0, 0x0000,0x0000, 0xd1,0x97,0x6d,0xff }, /*    21 */
  {   -13,   -75,   -22,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*    22 */
  {   -13,   -53,   -22,0, 0x0000,0x0000, 0xff,0xe7,0xa6,0xff }, /*    23 */
  {   -13,   -75,   -22,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*    24 */
  {   -26,   -86,    -9,0, 0x0000,0x0000, 0x9f,0x45,0x31,0xff }, /*    25 */
  {   -38,   -75,   -22,0, 0x0000,0x0000, 0xc7,0x90,0x67,0xff }, /*    26 */
  {   -38,   -75,   -22,0, 0x0000,0x0000, 0xc7,0x90,0x67,0xff }, /*    27 */
  {   -26,   -86,    -9,0, 0x0000,0x0000, 0x9f,0x45,0x31,0xff }, /*    28 */
  {   -38,   -75,     3,0, 0x0000,0x0000, 0xaf,0x7e,0x5b,0xff }, /*    29 */
  {   -38,   -75,     3,0, 0x0000,0x0000, 0xaf,0x7e,0x5b,0xff }, /*    30 */
  {   -26,   -86,    -9,0, 0x0000,0x0000, 0x9f,0x45,0x31,0xff }, /*    31 */
  {   -13,   -75,     3,0, 0x0000,0x0000, 0xd1,0x97,0x6d,0xff }, /*    32 */
  {   -13,   -75,     3,0, 0x0000,0x0000, 0xd1,0x97,0x6d,0xff }, /*    33 */
  {   -26,   -86,    -9,0, 0x0000,0x0000, 0x9f,0x45,0x31,0xff }, /*    34 */
  {   -13,   -75,   -22,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*    35 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_326_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 32, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP2Triangles(  6,  7,  8,  0,  9, 10, 11,  0),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP2Triangles( 15, 16, 17,  0, 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 4, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP1Triangle(  1,  2,  3,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_326_ = {
  gfx_Shape_of___POLYHEDRON_326_,
  vtx_Shape_of___POLYHEDRON_326_, 36, 5,
  bmp_frake_walk,
  tlut_frake_walk,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

/* ============================================================
          Geometric Animation Data Area
 ============================================================ */
static s16 Animation_for_body_motion_ch0_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_body_motion_ch0_val[]={
  0.0000, 0.2010, 0.4020, 0.6028, 0.8035, 1.0040, 1.2042, 1.4041, 1.4041,
  1.2042, 1.0040, 0.8035, 0.6028, 0.4020, 0.2010, 0.0000, -0.2010, -0.4020,
  -0.6028, -0.8035, -1.0040, -1.2042, -1.4041, -1.4041, -1.2042, -1.0040,
  -0.8035, -0.6028, -0.4020, -0.2010, 0.0000,
};

static s16 Animation_for_body_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_body_motion_ch1_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, 0.0000,
};

static s16 Animation_for_body_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_body_motion_ch2_val[]={
  0.0000, -0.0016, -0.0065, -0.0147, -0.0262, -0.0409, -0.0589, -0.0801,
  -0.0801, -0.0589, -0.0409, -0.0262, -0.0147, -0.0065, -0.0016, 0.0000,
  -0.0016, -0.0065, -0.0147, -0.0262, -0.0409, -0.0589, -0.0801, -0.0801,
  -0.0589, -0.0409, -0.0262, -0.0147, -0.0065, -0.0016, 0.0000,
};

static s16 Animation_for_body_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_body_motion_ch3_val[]={
  0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
};

static s16 Animation_for_body_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_body_motion_ch4_val[]={
  0.0000, 0.0163, 0.0326, 0.0489, 0.0652, 0.0814, 0.0977, 0.1140, 0.1140,
  0.0977, 0.0814, 0.0652, 0.0489, 0.0326, 0.0163, 0.0000, -0.0163, -0.0326,
  -0.0489, -0.0652, -0.0814, -0.0977, -0.1140, -0.1140, -0.0977, -0.0814,
  -0.0652, -0.0489, -0.0326, -0.0163, 0.0000,
};

static s16 Animation_for_body_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_body_motion_ch5_val[]={
  0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, 0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, 0.0000,
};

static s16 Animation_for_body_motion_ch6_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_body_motion_ch6_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_body_motion_ch7_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_body_motion_ch7_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static NDR_ANIM_CHANNEL Animation_for_body_motion_chlist[]={
  {NDC_ANIMKIND_TRS_X, 0, 30, 31,
   Animation_for_body_motion_ch0_key, Animation_for_body_motion_ch0_val},
  {NDC_ANIMKIND_TRS_Y, 0, 30, 31,
   Animation_for_body_motion_ch1_key, Animation_for_body_motion_ch1_val},
  {NDC_ANIMKIND_TRS_Z, 0, 30, 31,
   Animation_for_body_motion_ch2_key, Animation_for_body_motion_ch2_val},
  {NDC_ANIMKIND_ROT_X, 0, 30, 31,
   Animation_for_body_motion_ch3_key, Animation_for_body_motion_ch3_val},
  {NDC_ANIMKIND_ROT_Y, 0, 30, 31,
   Animation_for_body_motion_ch4_key, Animation_for_body_motion_ch4_val},
  {NDC_ANIMKIND_ROT_Z, 0, 30, 31,
   Animation_for_body_motion_ch5_key, Animation_for_body_motion_ch5_val},
  {NDC_ANIMKIND_SCL_X, 0, 30, 31,
   Animation_for_body_motion_ch6_key, Animation_for_body_motion_ch6_val},
  {NDC_ANIMKIND_SCL_Z, 0, 30, 31,
   Animation_for_body_motion_ch7_key, Animation_for_body_motion_ch7_val},
};

static NDR_ANIM Animation_for_body_motion={
  Animation_for_body_motion_chlist, 8, 30
};

static s16 Animation_for_R_uparm_motion_ch0_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_uparm_motion_ch0_val[]={
  25.1702, 24.3103, 23.4449, 22.5775, 21.7110, 20.8485, 19.9937, 19.1493,
  18.7222, 18.4925, 17.9738, 17.1581, 16.0399, 14.6165, 12.8887, 10.8594,
  12.0169, 13.1386, 14.2150, 15.2368, 16.1950, 17.0803, 17.8841, 19.4092,
  21.4237, 23.0347, 24.2405, 25.0445, 25.4565, 25.4915, 25.1702,
};

static s16 Animation_for_R_uparm_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_uparm_motion_ch1_val[]={
  13.8951, 13.3754, 12.9433, 12.5979, 12.3390, 12.1656, 12.0762, 12.0698,
  11.2759, 9.9212, 8.9567, 8.3865, 8.2120, 8.4327, 9.0452, 10.0445,
  12.0112, 14.1467, 16.4490, 18.9155, 21.5429, 24.3270, 27.2627, 27.0198,
  23.7735, 20.9371, 18.5424, 16.6183, 15.1894, 14.2765, 13.8951,
};

static s16 Animation_for_R_uparm_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_uparm_motion_ch2_val[]={
  -5.5650, -2.9027, -0.2556, 2.3734, 4.9815, 7.5659, 10.1239, 12.6528,
  10.3814, 3.2525, -3.9521, -11.1938, -18.4333, -25.6309, -32.7475,
  -39.7441, -43.2216, -46.6753, -50.0969, -53.4785, -56.8117, -60.0890,
  -63.3025, -61.3194, -53.9459, -46.2850, -38.3883, -30.3105, -22.1085,
  -13.8403, -5.5651,
};

static s16 Animation_for_R_uparm_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_uparm_motion_ch3_val[]={
  0.0555, 0.0235, -0.0084, -0.0404, -0.0724, -0.1044, -0.1364, -0.1686,
  -0.1474, -0.0732, 0.0007, 0.0742, 0.1473, 0.2198, 0.2917, 0.3629,
  0.4072, 0.4516, 0.4962, 0.5409, 0.5859, 0.6310, 0.6764, 0.6576, 0.5736,
  0.4886, 0.4029, 0.3166, 0.2299, 0.1428, 0.0555,
};

static s16 Animation_for_R_uparm_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_uparm_motion_ch4_val[]={
  0.0001, 0.0137, 0.0274, 0.0412, 0.0552, 0.0692, 0.0832, 0.0973, 0.1026,
  0.0992, 0.0963, 0.0940, 0.0926, 0.0923, 0.0933, 0.0959, 0.0759, 0.0566,
  0.0380, 0.0203, 0.0034, -0.0125, -0.0274, -0.0373, -0.0402, -0.0399,
  -0.0366, -0.0306, -0.0223, -0.0119, 0.0001,
};

static s16 Animation_for_R_uparm_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_uparm_motion_ch5_val[]={
  0.2684, 0.2580, 0.2470, 0.2353, 0.2230, 0.2101, 0.1965, 0.1822, 0.1775,
  0.1808, 0.1813, 0.1793, 0.1747, 0.1677, 0.1584, 0.1468, 0.1587, 0.1695,
  0.1794, 0.1882, 0.1961, 0.2030, 0.2091, 0.2205, 0.2364, 0.2496, 0.2601,
  0.2675, 0.2715, 0.2718, 0.2684,
};

static s16 Animation_for_R_uparm_motion_ch6_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_uparm_motion_ch6_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_R_uparm_motion_ch7_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_uparm_motion_ch7_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_R_uparm_motion_ch8_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_uparm_motion_ch8_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static NDR_ANIM_CHANNEL Animation_for_R_uparm_motion_chlist[]={
  {NDC_ANIMKIND_TRS_X, 0, 30, 31,
   Animation_for_R_uparm_motion_ch0_key, Animation_for_R_uparm_motion_ch0_val},
  {NDC_ANIMKIND_TRS_Y, 0, 30, 31,
   Animation_for_R_uparm_motion_ch1_key, Animation_for_R_uparm_motion_ch1_val},
  {NDC_ANIMKIND_TRS_Z, 0, 30, 31,
   Animation_for_R_uparm_motion_ch2_key, Animation_for_R_uparm_motion_ch2_val},
  {NDC_ANIMKIND_ROT_X, 0, 30, 31,
   Animation_for_R_uparm_motion_ch3_key, Animation_for_R_uparm_motion_ch3_val},
  {NDC_ANIMKIND_ROT_Y, 0, 30, 31,
   Animation_for_R_uparm_motion_ch4_key, Animation_for_R_uparm_motion_ch4_val},
  {NDC_ANIMKIND_ROT_Z, 0, 30, 31,
   Animation_for_R_uparm_motion_ch5_key, Animation_for_R_uparm_motion_ch5_val},
  {NDC_ANIMKIND_SCL_X, 0, 30, 31,
   Animation_for_R_uparm_motion_ch6_key, Animation_for_R_uparm_motion_ch6_val},
  {NDC_ANIMKIND_SCL_Y, 0, 30, 31,
   Animation_for_R_uparm_motion_ch7_key, Animation_for_R_uparm_motion_ch7_val},
  {NDC_ANIMKIND_SCL_Z, 0, 30, 31,
   Animation_for_R_uparm_motion_ch8_key, Animation_for_R_uparm_motion_ch8_val},
};

static NDR_ANIM Animation_for_R_uparm_motion={
  Animation_for_R_uparm_motion_chlist, 9, 30
};

static s16 Animation_for_R_lowarm_motion_ch0_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowarm_motion_ch0_val[]={
  1.2748, 1.2485, 1.1537, 0.9907, 0.7608, 0.4659, 0.1075, -0.3115, -0.4549,
  -0.2846, -0.1225, 0.0314, 0.1769, 0.3140, 0.4420, 0.5615, 0.9089,
  1.1428, 1.2604, 1.2607, 1.1429, 0.9091, 0.5620, 0.4624, 0.6710, 0.8497,
  0.9977, 1.1143, 1.1997, 1.2532, 1.2748,
};

static s16 Animation_for_R_lowarm_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowarm_motion_ch1_val[]={
  1.7513, 1.7152, 1.5847, 1.3610, 1.0453, 0.6399, 0.1477, -0.4279, -0.6250,
  -0.3910, -0.1682, 0.0430, 0.2430, 0.4311, 0.6074, 0.7715, 1.2487,
  1.5700, 1.7316, 1.7317, 1.5702, 1.2490, 0.7721, 0.6353, 0.9219, 1.1672,
  1.3705, 1.5309, 1.6481, 1.7215, 1.7513,
};

static s16 Animation_for_R_lowarm_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowarm_motion_ch2_val[]={
  -8.1146, -9.5202, -10.9173, -12.2960, -13.6471, -14.9610, -16.2288,
  -17.4414, -17.8087, -17.3701, -16.9226, -16.4668, -16.0033, -15.5320,
  -15.0540, -14.5693, -12.8270, -11.0307, -9.2021, -7.3625, -5.5337,
  -3.7375, -1.9952, -1.5882, -2.4794, -3.3893, -4.3149, -5.2533, -6.2014,
  -7.1562, -8.1146,
};

static s16 Animation_for_R_lowarm_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowarm_motion_ch3_val[]={
  -0.4177, -0.4918, -0.5682, -0.6471, -0.7287, -0.8133, -0.9009, -0.9916,
  -1.0206, -0.9860, -0.9519, -0.9181, -0.8848, -0.8519, -0.8195, -0.7875,
  -0.6786, -0.5745, -0.4748, -0.3788, -0.2858, -0.1951, -0.1061, -0.0849,
  -0.1310, -0.1775, -0.2243, -0.2717, -0.3196, -0.3682, -0.4177,
};

static s16 Animation_for_R_lowarm_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowarm_motion_ch4_val[]={
  0.2871, 0.3311, 0.3733, 0.4135, 0.4513, 0.4865, 0.5185, 0.5471, 0.5554,
  0.5455, 0.5352, 0.5243, 0.5130, 0.5011, 0.4889, 0.4762, 0.4286, 0.3767,
  0.3213, 0.2630, 0.2024, 0.1402, 0.0769, 0.0616, 0.0948, 0.1278, 0.1605,
  0.1929, 0.2248, 0.2563, 0.2871,
};

static s16 Animation_for_R_lowarm_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowarm_motion_ch5_val[]={
  -0.0612, -0.0838, -0.1102, -0.1404, -0.1747, -0.2129, -0.2551, -0.3012,
  -0.3165, -0.2984, -0.2808, -0.2637, -0.2472, -0.2312, -0.2158, -0.2010,
  -0.1533, -0.1125, -0.0784, -0.0507, -0.0292, -0.0137, -0.0041, -0.0026,
  -0.0062, -0.0114, -0.0181, -0.0264, -0.0364, -0.0480, -0.0612,
};

static s16 Animation_for_R_lowarm_motion_ch6_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowarm_motion_ch6_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_R_lowarm_motion_ch7_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowarm_motion_ch7_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_R_lowarm_motion_ch8_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowarm_motion_ch8_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static NDR_ANIM_CHANNEL Animation_for_R_lowarm_motion_chlist[]={
  {NDC_ANIMKIND_TRS_X, 0, 30, 31,
   Animation_for_R_lowarm_motion_ch0_key, Animation_for_R_lowarm_motion_ch0_val},
  {NDC_ANIMKIND_TRS_Y, 0, 30, 31,
   Animation_for_R_lowarm_motion_ch1_key, Animation_for_R_lowarm_motion_ch1_val},
  {NDC_ANIMKIND_TRS_Z, 0, 30, 31,
   Animation_for_R_lowarm_motion_ch2_key, Animation_for_R_lowarm_motion_ch2_val},
  {NDC_ANIMKIND_ROT_X, 0, 30, 31,
   Animation_for_R_lowarm_motion_ch3_key, Animation_for_R_lowarm_motion_ch3_val},
  {NDC_ANIMKIND_ROT_Y, 0, 30, 31,
   Animation_for_R_lowarm_motion_ch4_key, Animation_for_R_lowarm_motion_ch4_val},
  {NDC_ANIMKIND_ROT_Z, 0, 30, 31,
   Animation_for_R_lowarm_motion_ch5_key, Animation_for_R_lowarm_motion_ch5_val},
  {NDC_ANIMKIND_SCL_X, 0, 30, 31,
   Animation_for_R_lowarm_motion_ch6_key, Animation_for_R_lowarm_motion_ch6_val},
  {NDC_ANIMKIND_SCL_Y, 0, 30, 31,
   Animation_for_R_lowarm_motion_ch7_key, Animation_for_R_lowarm_motion_ch7_val},
  {NDC_ANIMKIND_SCL_Z, 0, 30, 31,
   Animation_for_R_lowarm_motion_ch8_key, Animation_for_R_lowarm_motion_ch8_val},
};

static NDR_ANIM Animation_for_R_lowarm_motion={
  Animation_for_R_lowarm_motion_chlist, 9, 30
};

static s16 Animation_for_R_hand_motion_ch0_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_hand_motion_ch0_val[]={
  -30.2555, -28.7970, -27.3496, -25.9159, -24.4984, -23.0993, -21.7211,
  -20.3665, -18.3246, -15.6744, -13.1712, -10.8338, -8.6800, -6.7260,
  -4.9864, -3.4748, -2.8217, -2.2273, -1.6924, -1.2183, -0.8054, -0.4548,
  -0.1670, -0.6263, -2.5367, -5.3988, -9.1296, -13.6199, -18.7395, -24.3390,
  -30.2556,
};

static s16 Animation_for_R_hand_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_hand_motion_ch1_val[]={
  -40.4941, -38.5422, -36.6051, -34.6864, -32.7892, -30.9169, -29.0726,
  -27.2595, -24.5227, -20.9677, -17.6093, -14.4730, -11.5823, -8.9590,
  -6.6231, -4.5920, -3.7204, -2.9271, -2.2138, -1.5815, -1.0315, -0.5646,
  -0.1818, -0.8001, -3.3614, -7.1971, -12.1955, -18.2112, -25.0689,
  -32.5692, -40.4940,
};

static s16 Animation_for_R_hand_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_hand_motion_ch2_val[]={
  -67.1870, -66.7890, -66.2901, -65.6915, -64.9940, -64.1987, -63.3072,
  -62.3209, -60.6174, -57.9725, -54.9532, -51.5815, -47.8831, -43.8857,
  -39.6194, -35.1162, -32.8351, -30.5098, -28.1447, -25.7439, -23.3119,
  -20.8530, -18.3715, -22.1108, -31.7459, -40.6976, -48.7052, -55.5360,
  -60.9915, -64.9131, -67.1870,
};

static s16 Animation_for_R_hand_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_hand_motion_ch3_val[]={
  -1.5708, -1.5189, -1.4672, -1.4156, -1.3643, -1.3134, -1.2630, -1.2131,
  -1.1377, -1.0387, -0.9430, -0.8508, -0.7621, -0.6768, -0.5947, -0.5155,
  -0.4769, -0.4390, -0.4016, -0.3647, -0.3282, -0.2921, -0.2565, -0.3096,
  -0.4565, -0.6124, -0.7798, -0.9604, -1.1543, -1.3593, -1.5708,
};

static s16 Animation_for_R_hand_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_hand_motion_ch4_val[]={
  0.6415, 0.6409, 0.6389, 0.6357, 0.6312, 0.6254, 0.6185, 0.6103, 0.5955,
  0.5714, 0.5428, 0.5103, 0.4742, 0.4350, 0.3930, 0.3487, 0.3263, 0.3034,
  0.2802, 0.2565, 0.2326, 0.2083, 0.1838, 0.2207, 0.3156, 0.4036, 0.4823,
  0.5484, 0.5988, 0.6306, 0.6415,
};

static s16 Animation_for_R_hand_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_hand_motion_ch5_val[]={
  -0.6407, -0.6097, -0.5788, -0.5482, -0.5179, -0.4880, -0.4587, -0.4299,
  -0.3870, -0.3319, -0.2809, -0.2341, -0.1917, -0.1539, -0.1205, -0.0915,
  -0.0788, -0.0671, -0.0565, -0.0468, -0.0380, -0.0302, -0.0233, -0.0341,
  -0.0732, -0.1284, -0.2005, -0.2900, -0.3957, -0.5145, -0.6407,
};

static s16 Animation_for_R_hand_motion_ch6_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_hand_motion_ch6_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_R_hand_motion_ch7_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_hand_motion_ch7_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_R_hand_motion_ch8_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_hand_motion_ch8_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static NDR_ANIM_CHANNEL Animation_for_R_hand_motion_chlist[]={
  {NDC_ANIMKIND_TRS_X, 0, 30, 31,
   Animation_for_R_hand_motion_ch0_key, Animation_for_R_hand_motion_ch0_val},
  {NDC_ANIMKIND_TRS_Y, 0, 30, 31,
   Animation_for_R_hand_motion_ch1_key, Animation_for_R_hand_motion_ch1_val},
  {NDC_ANIMKIND_TRS_Z, 0, 30, 31,
   Animation_for_R_hand_motion_ch2_key, Animation_for_R_hand_motion_ch2_val},
  {NDC_ANIMKIND_ROT_X, 0, 30, 31,
   Animation_for_R_hand_motion_ch3_key, Animation_for_R_hand_motion_ch3_val},
  {NDC_ANIMKIND_ROT_Y, 0, 30, 31,
   Animation_for_R_hand_motion_ch4_key, Animation_for_R_hand_motion_ch4_val},
  {NDC_ANIMKIND_ROT_Z, 0, 30, 31,
   Animation_for_R_hand_motion_ch5_key, Animation_for_R_hand_motion_ch5_val},
  {NDC_ANIMKIND_SCL_X, 0, 30, 31,
   Animation_for_R_hand_motion_ch6_key, Animation_for_R_hand_motion_ch6_val},
  {NDC_ANIMKIND_SCL_Y, 0, 30, 31,
   Animation_for_R_hand_motion_ch7_key, Animation_for_R_hand_motion_ch7_val},
  {NDC_ANIMKIND_SCL_Z, 0, 30, 31,
   Animation_for_R_hand_motion_ch8_key, Animation_for_R_hand_motion_ch8_val},
};

static NDR_ANIM Animation_for_R_hand_motion={
  Animation_for_R_hand_motion_chlist, 9, 30
};

static s16 Animation_for_R_sode_motion_ch0_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_sode_motion_ch0_val[]={
  25.1702, 24.3103, 23.4449, 22.5775, 21.7110, 20.8485, 19.9937, 19.1493,
  18.7222, 18.4925, 17.9738, 17.1581, 16.0399, 14.6165, 12.8887, 10.8594,
  12.0169, 13.1386, 14.2150, 15.2368, 16.1950, 17.0803, 17.8841, 19.4092,
  21.4237, 23.0347, 24.2405, 25.0445, 25.4565, 25.4915, 25.1702,
};

static s16 Animation_for_R_sode_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_sode_motion_ch1_val[]={
  13.8951, 13.3754, 12.9433, 12.5979, 12.3390, 12.1656, 12.0762, 12.0698,
  11.2759, 9.9212, 8.9567, 8.3865, 8.2120, 8.4327, 9.0452, 10.0445,
  12.0112, 14.1467, 16.4490, 18.9155, 21.5429, 24.3270, 27.2627, 27.0198,
  23.7735, 20.9371, 18.5424, 16.6183, 15.1894, 14.2765, 13.8951,
};

static s16 Animation_for_R_sode_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_sode_motion_ch2_val[]={
  -5.5650, -2.9027, -0.2556, 2.3734, 4.9815, 7.5659, 10.1239, 12.6528,
  10.3814, 3.2525, -3.9521, -11.1938, -18.4333, -25.6309, -32.7475,
  -39.7441, -43.2216, -46.6753, -50.0969, -53.4785, -56.8117, -60.0890,
  -63.3025, -61.3194, -53.9459, -46.2850, -38.3883, -30.3105, -22.1085,
  -13.8403, -5.5651,
};

static s16 Animation_for_R_sode_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_sode_motion_ch3_val[]={
  0.0555, 0.0235, -0.0084, -0.0404, -0.0724, -0.1044, -0.1364, -0.1686,
  -0.1474, -0.0732, 0.0007, 0.0742, 0.1473, 0.2198, 0.2917, 0.3629,
  0.4072, 0.4516, 0.4962, 0.5409, 0.5859, 0.6310, 0.6764, 0.6576, 0.5736,
  0.4886, 0.4029, 0.3166, 0.2299, 0.1428, 0.0555,
};

static s16 Animation_for_R_sode_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_sode_motion_ch4_val[]={
  0.0001, 0.0137, 0.0274, 0.0412, 0.0552, 0.0692, 0.0832, 0.0973, 0.1026,
  0.0992, 0.0963, 0.0940, 0.0926, 0.0923, 0.0933, 0.0959, 0.0759, 0.0566,
  0.0380, 0.0203, 0.0034, -0.0125, -0.0274, -0.0373, -0.0402, -0.0399,
  -0.0366, -0.0306, -0.0223, -0.0119, 0.0001,
};

static s16 Animation_for_R_sode_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_sode_motion_ch5_val[]={
  0.2684, 0.2580, 0.2470, 0.2353, 0.2230, 0.2101, 0.1965, 0.1822, 0.1775,
  0.1808, 0.1813, 0.1793, 0.1747, 0.1677, 0.1584, 0.1468, 0.1587, 0.1695,
  0.1794, 0.1882, 0.1961, 0.2030, 0.2091, 0.2205, 0.2364, 0.2496, 0.2601,
  0.2675, 0.2715, 0.2718, 0.2684,
};

static s16 Animation_for_R_sode_motion_ch6_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_sode_motion_ch6_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_R_sode_motion_ch7_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_sode_motion_ch7_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_R_sode_motion_ch8_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_sode_motion_ch8_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static NDR_ANIM_CHANNEL Animation_for_R_sode_motion_chlist[]={
  {NDC_ANIMKIND_TRS_X, 0, 30, 31,
   Animation_for_R_sode_motion_ch0_key, Animation_for_R_sode_motion_ch0_val},
  {NDC_ANIMKIND_TRS_Y, 0, 30, 31,
   Animation_for_R_sode_motion_ch1_key, Animation_for_R_sode_motion_ch1_val},
  {NDC_ANIMKIND_TRS_Z, 0, 30, 31,
   Animation_for_R_sode_motion_ch2_key, Animation_for_R_sode_motion_ch2_val},
  {NDC_ANIMKIND_ROT_X, 0, 30, 31,
   Animation_for_R_sode_motion_ch3_key, Animation_for_R_sode_motion_ch3_val},
  {NDC_ANIMKIND_ROT_Y, 0, 30, 31,
   Animation_for_R_sode_motion_ch4_key, Animation_for_R_sode_motion_ch4_val},
  {NDC_ANIMKIND_ROT_Z, 0, 30, 31,
   Animation_for_R_sode_motion_ch5_key, Animation_for_R_sode_motion_ch5_val},
  {NDC_ANIMKIND_SCL_X, 0, 30, 31,
   Animation_for_R_sode_motion_ch6_key, Animation_for_R_sode_motion_ch6_val},
  {NDC_ANIMKIND_SCL_Y, 0, 30, 31,
   Animation_for_R_sode_motion_ch7_key, Animation_for_R_sode_motion_ch7_val},
  {NDC_ANIMKIND_SCL_Z, 0, 30, 31,
   Animation_for_R_sode_motion_ch8_key, Animation_for_R_sode_motion_ch8_val},
};

static NDR_ANIM Animation_for_R_sode_motion={
  Animation_for_R_sode_motion_chlist, 9, 30
};

static s16 Animation_for_L_uparm_motion_ch0_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_uparm_motion_ch0_val[]={
  -6.6255, -10.8275, -14.7768, -18.4489, -21.8222, -24.8769, -27.5958,
  -29.9645, -30.0011, -27.9053, -25.8115, -23.8315, -22.0688, -20.6147,
  -19.5460, -18.9218, -20.1238, -21.3163, -22.4988, -23.6706, -24.8315,
  -25.9808, -27.1180, -25.5455, -21.4810, -17.7631, -14.4661, -11.6552,
  -9.3850, -7.6982, -6.6255,
};

static s16 Animation_for_L_uparm_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_uparm_motion_ch1_val[]={
  5.0496, 8.2759, 11.8240, 15.6696, 19.7870, 24.1488, 28.7261, 33.4887,
  32.3910, 26.0827, 20.8211, 16.6102, 13.4281, 11.2304, 9.9510, 9.5065,
  10.3301, 11.1788, 12.0523, 12.9502, 13.8729, 14.8194, 15.7901, 14.4570,
  11.2733, 8.7204, 6.7945, 5.4849, 4.7737, 4.6382, 5.0496,
};

static s16 Animation_for_L_uparm_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_uparm_motion_ch2_val[]={
  -30.1689, -34.2019, -38.1662, -42.0556, -45.8645, -49.5874, -53.2195,
  -56.7562, -55.1585, -48.2026, -40.9642, -33.5850, -26.2085, -18.9773,
  -12.0305, -5.5016, -4.6696, -3.8417, -3.0181, -2.1989, -1.3843, -0.5744,
  0.2307, -0.8802, -4.2493, -8.0229, -12.1265, -16.4795, -20.9970, -25.5902,
  -30.1688,
};

static s16 Animation_for_L_uparm_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_uparm_motion_ch3_val[]={
  0.4145, 0.4588, 0.5030, 0.5469, 0.5905, 0.6338, 0.6767, 0.7192, 0.6806,
  0.5647, 0.4532, 0.3455, 0.2409, 0.1383, 0.0369, -0.0646, -0.0762,
  -0.0880, -0.0997, -0.1115, -0.1233, -0.1352, -0.1471, -0.1173, -0.0460,
  0.0258, 0.0988, 0.1735, 0.2506, 0.3307, 0.4145,
};

static s16 Animation_for_L_uparm_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_uparm_motion_ch4_val[]={
  0.2476, 0.2480, 0.2479, 0.2472, 0.2461, 0.2444, 0.2420, 0.2391, 0.2106,
  0.1521, 0.0880, 0.0192, -0.0535, -0.1293, -0.2074, -0.2872, -0.2938,
  -0.3003, -0.3068, -0.3134, -0.3199, -0.3265, -0.3330, -0.2959, -0.2152,
  -0.1350, -0.0556, 0.0227, 0.0996, 0.1747, 0.2476,
};

static s16 Animation_for_L_uparm_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_uparm_motion_ch5_val[]={
  0.0018, -0.0324, -0.0662, -0.0995, -0.1325, -0.1652, -0.1975, -0.2296,
  -0.2521, -0.2604, -0.2622, -0.2575, -0.2462, -0.2284, -0.2038, -0.1723,
  -0.1822, -0.1919, -0.2015, -0.2111, -0.2205, -0.2298, -0.2390, -0.2325,
  -0.2081, -0.1809, -0.1508, -0.1177, -0.0815, -0.0418, 0.0018,
};

static s16 Animation_for_L_uparm_motion_ch6_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_uparm_motion_ch6_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_L_uparm_motion_ch7_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_uparm_motion_ch7_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_L_uparm_motion_ch8_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_uparm_motion_ch8_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static NDR_ANIM_CHANNEL Animation_for_L_uparm_motion_chlist[]={
  {NDC_ANIMKIND_TRS_X, 0, 30, 31,
   Animation_for_L_uparm_motion_ch0_key, Animation_for_L_uparm_motion_ch0_val},
  {NDC_ANIMKIND_TRS_Y, 0, 30, 31,
   Animation_for_L_uparm_motion_ch1_key, Animation_for_L_uparm_motion_ch1_val},
  {NDC_ANIMKIND_TRS_Z, 0, 30, 31,
   Animation_for_L_uparm_motion_ch2_key, Animation_for_L_uparm_motion_ch2_val},
  {NDC_ANIMKIND_ROT_X, 0, 30, 31,
   Animation_for_L_uparm_motion_ch3_key, Animation_for_L_uparm_motion_ch3_val},
  {NDC_ANIMKIND_ROT_Y, 0, 30, 31,
   Animation_for_L_uparm_motion_ch4_key, Animation_for_L_uparm_motion_ch4_val},
  {NDC_ANIMKIND_ROT_Z, 0, 30, 31,
   Animation_for_L_uparm_motion_ch5_key, Animation_for_L_uparm_motion_ch5_val},
  {NDC_ANIMKIND_SCL_X, 0, 30, 31,
   Animation_for_L_uparm_motion_ch6_key, Animation_for_L_uparm_motion_ch6_val},
  {NDC_ANIMKIND_SCL_Y, 0, 30, 31,
   Animation_for_L_uparm_motion_ch7_key, Animation_for_L_uparm_motion_ch7_val},
  {NDC_ANIMKIND_SCL_Z, 0, 30, 31,
   Animation_for_L_uparm_motion_ch8_key, Animation_for_L_uparm_motion_ch8_val},
};

static NDR_ANIM Animation_for_L_uparm_motion={
  Animation_for_L_uparm_motion_chlist, 9, 30
};

static s16 Animation_for_L_lowarm_motion_ch0_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowarm_motion_ch0_val[]={
  -1.0019, -1.1005, -1.1772, -1.2322, -1.2648, -1.2752, -1.2634, -1.2295,
  -1.1952, -1.1756, -1.1540, -1.1301, -1.1042, -1.0763, -1.0463, -1.0140,
  -1.2587, -1.2109, -0.8718, -0.2517, 0.6313, 1.7510, 3.0746, 3.3321,
  2.4438, 1.6312, 0.9031, 0.2688, -0.2643, -0.6895, -1.0019,
};

static s16 Animation_for_L_lowarm_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowarm_motion_ch1_val[]={
  1.3762, 1.5120, 1.6174, 1.6927, 1.7377, 1.7520, 1.7357, 1.6890, 1.6419,
  1.6151, 1.5855, 1.5527, 1.5172, 1.4786, 1.4373, 1.3931, 1.7293, 1.6635,
  1.1978, 0.3459, -0.8672, -2.4054, -4.2239, -4.5776, -3.3575, -2.2407,
  -1.2406, -0.3693, 0.3631, 0.9472, 1.3762,
};

static s16 Animation_for_L_lowarm_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowarm_motion_ch2_val[]={
  -12.2184, -11.4368, -10.6480, -9.8539, -9.0564, -8.2570, -7.4578,
  -6.6605, -6.1399, -5.8942, -5.6490, -5.4044, -5.1603, -4.9169, -4.6743,
  -4.4325, -7.3038, -10.2039, -13.0475, -15.7508, -18.2340, -20.4242,
  -22.2569, -22.5449, -21.4636, -20.2235, -18.8393, -17.3279, -15.7074,
  -13.9973, -12.2185,
};

static s16 Animation_for_L_lowarm_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowarm_motion_ch3_val[]={
  -0.6425, -0.5975, -0.5532, -0.5098, -0.4671, -0.4251, -0.3837, -0.3429,
  -0.3165, -0.3040, -0.2916, -0.2793, -0.2670, -0.2547, -0.2425, -0.2303,
  -0.3758, -0.5288, -0.6919, -0.8671, -1.0553, -1.2556, -1.4646, -1.5028,
  -1.3679, -1.2354, -1.1067, -0.9828, -0.8641, -0.7507, -0.6425,
};

static s16 Animation_for_L_lowarm_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowarm_motion_ch4_val[]={
  -0.4113, -0.3886, -0.3653, -0.3413, -0.3167, -0.2916, -0.2660, -0.2400,
  -0.2228, -0.2146, -0.2063, -0.1980, -0.1897, -0.1814, -0.1730, -0.1646,
  -0.2611, -0.3519, -0.4348, -0.5067, -0.5646, -0.6054, -0.6265, -0.6281,
  -0.6194, -0.6022, -0.5770, -0.5446, -0.5056, -0.4609, -0.4113,
};

static s16 Animation_for_L_lowarm_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowarm_motion_ch5_val[]={
  0.1386, 0.1211, 0.1048, 0.0897, 0.0759, 0.0634, 0.0520, 0.0418, 0.0357,
  0.0330, 0.0304, 0.0279, 0.0255, 0.0233, 0.0211, 0.0191, 0.0499, 0.0962,
  0.1589, 0.2385, 0.3349, 0.4457, 0.5668, 0.5892, 0.5103, 0.4343, 0.3627,
  0.2967, 0.2371, 0.1844, 0.1386,
};

static s16 Animation_for_L_lowarm_motion_ch6_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowarm_motion_ch6_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_L_lowarm_motion_ch7_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowarm_motion_ch7_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_L_lowarm_motion_ch8_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowarm_motion_ch8_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static NDR_ANIM_CHANNEL Animation_for_L_lowarm_motion_chlist[]={
  {NDC_ANIMKIND_TRS_X, 0, 30, 31,
   Animation_for_L_lowarm_motion_ch0_key, Animation_for_L_lowarm_motion_ch0_val},
  {NDC_ANIMKIND_TRS_Y, 0, 30, 31,
   Animation_for_L_lowarm_motion_ch1_key, Animation_for_L_lowarm_motion_ch1_val},
  {NDC_ANIMKIND_TRS_Z, 0, 30, 31,
   Animation_for_L_lowarm_motion_ch2_key, Animation_for_L_lowarm_motion_ch2_val},
  {NDC_ANIMKIND_ROT_X, 0, 30, 31,
   Animation_for_L_lowarm_motion_ch3_key, Animation_for_L_lowarm_motion_ch3_val},
  {NDC_ANIMKIND_ROT_Y, 0, 30, 31,
   Animation_for_L_lowarm_motion_ch4_key, Animation_for_L_lowarm_motion_ch4_val},
  {NDC_ANIMKIND_ROT_Z, 0, 30, 31,
   Animation_for_L_lowarm_motion_ch5_key, Animation_for_L_lowarm_motion_ch5_val},
  {NDC_ANIMKIND_SCL_X, 0, 30, 31,
   Animation_for_L_lowarm_motion_ch6_key, Animation_for_L_lowarm_motion_ch6_val},
  {NDC_ANIMKIND_SCL_Y, 0, 30, 31,
   Animation_for_L_lowarm_motion_ch7_key, Animation_for_L_lowarm_motion_ch7_val},
  {NDC_ANIMKIND_SCL_Z, 0, 30, 31,
   Animation_for_L_lowarm_motion_ch8_key, Animation_for_L_lowarm_motion_ch8_val},
};

static NDR_ANIM Animation_for_L_lowarm_motion={
  Animation_for_L_lowarm_motion_chlist, 9, 30
};

static s16 Animation_for_L_hand_motion_ch0_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_hand_motion_ch0_val[]={
  -0.1114, -0.3185, -0.3218, -0.1218, 0.2810, 0.8842, 1.6840, 2.6767,
  3.0907, 2.7947, 2.5120, 2.2417, 1.9847, 1.7407, 1.5096, 1.2921, 1.5840,
  1.8991, 2.2376, 2.5984, 2.9822, 3.3876, 3.8154, 3.3611, 2.1708, 1.2034,
  0.4662, -0.0367, -0.3013, -0.3263, -0.1114,
};

static s16 Animation_for_L_hand_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_hand_motion_ch1_val[]={
  0.1148, 0.4000, 0.4129, 0.1531, -0.3778, -1.1770, -2.2398, -3.5601,
  -4.1137, -3.7241, -3.3516, -2.9963, -2.6584, -2.3379, -2.0351, -1.7499,
  -2.1404, -2.5619, -3.0141, -3.4970, -4.0098, -4.5526, -5.1248, -4.5174,
  -2.9260, -1.6336, -0.6487, 0.0220, 0.3741, 0.4050, 0.1148,
};

static s16 Animation_for_L_hand_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_hand_motion_ch2_val[]={
  -1.4775, -5.9628, -10.4623, -14.9506, -19.4017, -23.7905, -28.0913,
  -32.2799, -33.7926, -32.7215, -31.6410, -30.5517, -29.4539, -28.3479,
  -27.2344, -26.1137, -27.5976, -29.0683, -30.5247, -31.9656, -33.3905,
  -34.7978, -36.1868, -34.7080, -30.2456, -25.6353, -20.9082, -16.0963,
  -11.2320, -6.3480, -1.4775,
};

static s16 Animation_for_L_hand_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_hand_motion_ch3_val[]={
  -0.0156, -0.0774, -0.1395, -0.2022, -0.2657, -0.3304, -0.3963, -0.4638,
  -0.4890, -0.4704, -0.4520, -0.4337, -0.4156, -0.3975, -0.3796, -0.3617,
  -0.3844, -0.4073, -0.4304, -0.4536, -0.4771, -0.5009, -0.5248, -0.4993,
  -0.4257, -0.3542, -0.2844, -0.2159, -0.1485, -0.0819, -0.0156,
};

static s16 Animation_for_L_hand_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_hand_motion_ch4_val[]={
  -0.0148, -0.0600, -0.1051, -0.1497, -0.1938, -0.2372, -0.2796, -0.3208,
  -0.3357, -0.3251, -0.3145, -0.3038, -0.2930, -0.2820, -0.2711, -0.2600,
  -0.2746, -0.2891, -0.3035, -0.3177, -0.3317, -0.3456, -0.3592, -0.3447,
  -0.3007, -0.2553, -0.2086, -0.1610, -0.1127, -0.0639, -0.0148,
};

static s16 Animation_for_L_hand_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_hand_motion_ch5_val[]={
  0.0001, 0.0024, 0.0075, 0.0153, 0.0260, 0.0396, 0.0562, 0.0759, 0.0840,
  0.0782, 0.0727, 0.0673, 0.0622, 0.0573, 0.0526, 0.0481, 0.0541, 0.0605,
  0.0672, 0.0743, 0.0817, 0.0896, 0.0979, 0.0891, 0.0659, 0.0463, 0.0303,
  0.0178, 0.0086, 0.0027, 0.0001,
};

static s16 Animation_for_L_hand_motion_ch6_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_hand_motion_ch6_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_L_hand_motion_ch7_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_hand_motion_ch7_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_L_hand_motion_ch8_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_hand_motion_ch8_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static NDR_ANIM_CHANNEL Animation_for_L_hand_motion_chlist[]={
  {NDC_ANIMKIND_TRS_X, 0, 30, 31,
   Animation_for_L_hand_motion_ch0_key, Animation_for_L_hand_motion_ch0_val},
  {NDC_ANIMKIND_TRS_Y, 0, 30, 31,
   Animation_for_L_hand_motion_ch1_key, Animation_for_L_hand_motion_ch1_val},
  {NDC_ANIMKIND_TRS_Z, 0, 30, 31,
   Animation_for_L_hand_motion_ch2_key, Animation_for_L_hand_motion_ch2_val},
  {NDC_ANIMKIND_ROT_X, 0, 30, 31,
   Animation_for_L_hand_motion_ch3_key, Animation_for_L_hand_motion_ch3_val},
  {NDC_ANIMKIND_ROT_Y, 0, 30, 31,
   Animation_for_L_hand_motion_ch4_key, Animation_for_L_hand_motion_ch4_val},
  {NDC_ANIMKIND_ROT_Z, 0, 30, 31,
   Animation_for_L_hand_motion_ch5_key, Animation_for_L_hand_motion_ch5_val},
  {NDC_ANIMKIND_SCL_X, 0, 30, 31,
   Animation_for_L_hand_motion_ch6_key, Animation_for_L_hand_motion_ch6_val},
  {NDC_ANIMKIND_SCL_Y, 0, 30, 31,
   Animation_for_L_hand_motion_ch7_key, Animation_for_L_hand_motion_ch7_val},
  {NDC_ANIMKIND_SCL_Z, 0, 30, 31,
   Animation_for_L_hand_motion_ch8_key, Animation_for_L_hand_motion_ch8_val},
};

static NDR_ANIM Animation_for_L_hand_motion={
  Animation_for_L_hand_motion_chlist, 9, 30
};

static s16 Animation_for_L_sode_motion_ch0_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_sode_motion_ch0_val[]={
  -6.6255, -10.8275, -14.7768, -18.4489, -21.8222, -24.8769, -27.5958,
  -29.9645, -30.0011, -27.9053, -25.8115, -23.8315, -22.0688, -20.6147,
  -19.5460, -18.9218, -20.1238, -21.3163, -22.4988, -23.6706, -24.8315,
  -25.9808, -27.1180, -25.5455, -21.4810, -17.7631, -14.4661, -11.6552,
  -9.3850, -7.6982, -6.6255,
};

static s16 Animation_for_L_sode_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_sode_motion_ch1_val[]={
  5.0496, 8.2759, 11.8240, 15.6696, 19.7870, 24.1488, 28.7261, 33.4887,
  32.3910, 26.0827, 20.8211, 16.6102, 13.4281, 11.2304, 9.9510, 9.5065,
  10.3301, 11.1788, 12.0523, 12.9502, 13.8729, 14.8194, 15.7901, 14.4570,
  11.2733, 8.7204, 6.7945, 5.4849, 4.7737, 4.6382, 5.0496,
};

static s16 Animation_for_L_sode_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_sode_motion_ch2_val[]={
  -30.1689, -34.2019, -38.1662, -42.0556, -45.8645, -49.5874, -53.2195,
  -56.7562, -55.1585, -48.2026, -40.9642, -33.5850, -26.2085, -18.9773,
  -12.0305, -5.5016, -4.6696, -3.8417, -3.0181, -2.1989, -1.3843, -0.5744,
  0.2307, -0.8802, -4.2493, -8.0229, -12.1265, -16.4795, -20.9970, -25.5902,
  -30.1688,
};

static s16 Animation_for_L_sode_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_sode_motion_ch3_val[]={
  0.4145, 0.4588, 0.5030, 0.5469, 0.5905, 0.6338, 0.6767, 0.7192, 0.6806,
  0.5647, 0.4532, 0.3455, 0.2409, 0.1383, 0.0369, -0.0646, -0.0762,
  -0.0880, -0.0997, -0.1115, -0.1233, -0.1352, -0.1471, -0.1173, -0.0460,
  0.0258, 0.0988, 0.1735, 0.2506, 0.3307, 0.4145,
};

static s16 Animation_for_L_sode_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_sode_motion_ch4_val[]={
  0.2476, 0.2480, 0.2479, 0.2472, 0.2461, 0.2444, 0.2420, 0.2391, 0.2106,
  0.1521, 0.0880, 0.0192, -0.0535, -0.1293, -0.2074, -0.2872, -0.2938,
  -0.3003, -0.3068, -0.3134, -0.3199, -0.3265, -0.3330, -0.2959, -0.2152,
  -0.1350, -0.0556, 0.0227, 0.0996, 0.1747, 0.2476,
};

static s16 Animation_for_L_sode_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_sode_motion_ch5_val[]={
  0.0018, -0.0324, -0.0662, -0.0995, -0.1325, -0.1652, -0.1975, -0.2296,
  -0.2521, -0.2604, -0.2622, -0.2575, -0.2462, -0.2284, -0.2038, -0.1723,
  -0.1822, -0.1919, -0.2015, -0.2111, -0.2205, -0.2298, -0.2390, -0.2325,
  -0.2081, -0.1809, -0.1508, -0.1177, -0.0815, -0.0418, 0.0018,
};

static s16 Animation_for_L_sode_motion_ch6_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_sode_motion_ch6_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_L_sode_motion_ch7_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_sode_motion_ch7_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_L_sode_motion_ch8_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_sode_motion_ch8_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static NDR_ANIM_CHANNEL Animation_for_L_sode_motion_chlist[]={
  {NDC_ANIMKIND_TRS_X, 0, 30, 31,
   Animation_for_L_sode_motion_ch0_key, Animation_for_L_sode_motion_ch0_val},
  {NDC_ANIMKIND_TRS_Y, 0, 30, 31,
   Animation_for_L_sode_motion_ch1_key, Animation_for_L_sode_motion_ch1_val},
  {NDC_ANIMKIND_TRS_Z, 0, 30, 31,
   Animation_for_L_sode_motion_ch2_key, Animation_for_L_sode_motion_ch2_val},
  {NDC_ANIMKIND_ROT_X, 0, 30, 31,
   Animation_for_L_sode_motion_ch3_key, Animation_for_L_sode_motion_ch3_val},
  {NDC_ANIMKIND_ROT_Y, 0, 30, 31,
   Animation_for_L_sode_motion_ch4_key, Animation_for_L_sode_motion_ch4_val},
  {NDC_ANIMKIND_ROT_Z, 0, 30, 31,
   Animation_for_L_sode_motion_ch5_key, Animation_for_L_sode_motion_ch5_val},
  {NDC_ANIMKIND_SCL_X, 0, 30, 31,
   Animation_for_L_sode_motion_ch6_key, Animation_for_L_sode_motion_ch6_val},
  {NDC_ANIMKIND_SCL_Y, 0, 30, 31,
   Animation_for_L_sode_motion_ch7_key, Animation_for_L_sode_motion_ch7_val},
  {NDC_ANIMKIND_SCL_Z, 0, 30, 31,
   Animation_for_L_sode_motion_ch8_key, Animation_for_L_sode_motion_ch8_val},
};

static NDR_ANIM Animation_for_L_sode_motion={
  Animation_for_L_sode_motion_chlist, 9, 30
};

static s16 Animation_for_head_motion_ch0_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_head_motion_ch0_val[]={
  0.0000, -0.1836, -0.3672, -0.5506, -0.7340, -0.9172, -1.1002, -1.2829,
  -1.2829, -1.1002, -0.9172, -0.7340, -0.5506, -0.3672, -0.1836, 0.0000,
  0.2118, 0.4236, 0.6353, 0.8467, 1.0580, 1.2689, 1.4794, 1.4794, 1.2689,
  1.0580, 0.8467, 0.6353, 0.4236, 0.2118, 0.0000,
};

static s16 Animation_for_head_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_head_motion_ch1_val[]={
  0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
};

static s16 Animation_for_head_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_head_motion_ch2_val[]={
  0.0000, -0.0014, -0.0055, -0.0123, -0.0218, -0.0341, -0.0491, -0.0669,
  -0.0669, -0.0491, -0.0341, -0.0218, -0.0123, -0.0055, -0.0014, 0.0000,
  -0.0018, -0.0073, -0.0164, -0.0291, -0.0454, -0.0654, -0.0890, -0.0890,
  -0.0654, -0.0454, -0.0291, -0.0164, -0.0073, -0.0018, 0.0000,
};

static s16 Animation_for_head_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_head_motion_ch3_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, 0.0000,
};

static s16 Animation_for_head_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_head_motion_ch4_val[]={
  0.0000, -0.0149, -0.0298, -0.0446, -0.0595, -0.0744, -0.0893, -0.1041,
  -0.1041, -0.0893, -0.0744, -0.0595, -0.0446, -0.0298, -0.0149, 0.0000,
  0.0172, 0.0343, 0.0515, 0.0687, 0.0858, 0.1030, 0.1202, 0.1202, 0.1030,
  0.0858, 0.0687, 0.0515, 0.0343, 0.0172, 0.0000,
};

static s16 Animation_for_head_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_head_motion_ch5_val[]={
  0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, 0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, 0.0000,
};

static s16 Animation_for_head_motion_ch6_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_head_motion_ch6_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_head_motion_ch7_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_head_motion_ch7_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static NDR_ANIM_CHANNEL Animation_for_head_motion_chlist[]={
  {NDC_ANIMKIND_TRS_X, 0, 30, 31,
   Animation_for_head_motion_ch0_key, Animation_for_head_motion_ch0_val},
  {NDC_ANIMKIND_TRS_Y, 0, 30, 31,
   Animation_for_head_motion_ch1_key, Animation_for_head_motion_ch1_val},
  {NDC_ANIMKIND_TRS_Z, 0, 30, 31,
   Animation_for_head_motion_ch2_key, Animation_for_head_motion_ch2_val},
  {NDC_ANIMKIND_ROT_X, 0, 30, 31,
   Animation_for_head_motion_ch3_key, Animation_for_head_motion_ch3_val},
  {NDC_ANIMKIND_ROT_Y, 0, 30, 31,
   Animation_for_head_motion_ch4_key, Animation_for_head_motion_ch4_val},
  {NDC_ANIMKIND_ROT_Z, 0, 30, 31,
   Animation_for_head_motion_ch5_key, Animation_for_head_motion_ch5_val},
  {NDC_ANIMKIND_SCL_X, 0, 30, 31,
   Animation_for_head_motion_ch6_key, Animation_for_head_motion_ch6_val},
  {NDC_ANIMKIND_SCL_Z, 0, 30, 31,
   Animation_for_head_motion_ch7_key, Animation_for_head_motion_ch7_val},
};

static NDR_ANIM Animation_for_head_motion={
  Animation_for_head_motion_chlist, 8, 30
};

static s16 Animation_for_L_upleg_motion_ch0_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_upleg_motion_ch0_val[]={
  -2.9680, -2.8560, -2.7424, -2.6273, -2.5107, -2.3925, -2.2729, -2.1519,
  -1.8184, -1.3274, -0.9105, -0.5698, -0.3074, -0.1245, -0.0219, 0.0000,
  -0.0019, -0.0138, -0.0355, -0.0662, -0.1050, -0.1508, -0.2021, -0.2287,
  -0.2937, -0.4590, -0.7354, -1.1281, -1.6365, -2.2540, -2.9680,
};

static s16 Animation_for_L_upleg_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_upleg_motion_ch1_val[]={
  12.5350, 12.6035, 12.6707, 12.7366, 12.8013, 12.8646, 12.9265, 12.9871,
  12.2027, 10.5374, 8.8303, 7.0908, 5.3285, 3.5529, 1.7736, 0.0000,
  -0.0310, -0.0598, -0.0815, -0.0915, -0.0852, -0.0579, -0.0053, 0.7773,
  2.3651, 4.0521, 5.7991, 7.5642, 9.3038, 10.9748, 12.5350,
};

static s16 Animation_for_L_upleg_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_upleg_motion_ch2_val[]={
  -1.5498, -1.5720, -1.5942, -1.6167, -1.6392, -1.6619, -1.6847, -1.7077,
  -1.4511, -0.9791, -0.5943, -0.2978, -0.0902, 0.0282, 0.0580, -0.0000,
  -0.0128, -0.0865, -0.2209, -0.4153, -0.6688, -0.9800, -1.3472, -1.1296,
  -0.4479, 0.0023, 0.2061, 0.1540, -0.1582, -0.7286, -1.5498,
};

static s16 Animation_for_L_upleg_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_upleg_motion_ch3_val[]={
  -0.5236, -0.5280, -0.5324, -0.5368, -0.5412, -0.5456, -0.5500, -0.5544,
  -0.5198, -0.4460, -0.3720, -0.2978, -0.2234, -0.1490, -0.0745, -0.0000,
  0.0722, 0.1444, 0.2166, 0.2886, 0.3606, 0.4324, 0.5041, 0.4694, 0.3279,
  0.1860, 0.0438, -0.0985, -0.2406, -0.3824, -0.5236,
};

static s16 Animation_for_L_upleg_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_upleg_motion_ch4_val[]={
  -0.0032, -0.0050, -0.0067, -0.0084, -0.0101, -0.0118, -0.0134, -0.0150,
  -0.0119, -0.0052, -0.0002, 0.0031, 0.0047, 0.0047, 0.0031, 0.0000,
  -0.0000, -0.0014, -0.0042, -0.0083, -0.0138, -0.0206, -0.0287, -0.0226,
  -0.0044, 0.0091, 0.0176, 0.0208, 0.0184, 0.0103, -0.0032,
};

static s16 Animation_for_L_upleg_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_upleg_motion_ch5_val[]={
  0.0826, 0.0715, 0.0603, 0.0492, 0.0381, 0.0269, 0.0158, 0.0047, -0.0009,
  -0.0008, -0.0004, 0.0001, 0.0005, 0.0007, 0.0006, 0.0000, -0.0028,
  -0.0056, -0.0086, -0.0118, -0.0153, -0.0192, -0.0237, -0.0156, 0.0031,
  0.0188, 0.0324, 0.0447, 0.0565, 0.0689, 0.0826,
};

static s16 Animation_for_L_upleg_motion_ch6_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_upleg_motion_ch6_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_L_upleg_motion_ch7_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_upleg_motion_ch7_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_L_upleg_motion_ch8_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_upleg_motion_ch8_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static NDR_ANIM_CHANNEL Animation_for_L_upleg_motion_chlist[]={
  {NDC_ANIMKIND_TRS_X, 0, 30, 31,
   Animation_for_L_upleg_motion_ch0_key, Animation_for_L_upleg_motion_ch0_val},
  {NDC_ANIMKIND_TRS_Y, 0, 30, 31,
   Animation_for_L_upleg_motion_ch1_key, Animation_for_L_upleg_motion_ch1_val},
  {NDC_ANIMKIND_TRS_Z, 0, 30, 31,
   Animation_for_L_upleg_motion_ch2_key, Animation_for_L_upleg_motion_ch2_val},
  {NDC_ANIMKIND_ROT_X, 0, 30, 31,
   Animation_for_L_upleg_motion_ch3_key, Animation_for_L_upleg_motion_ch3_val},
  {NDC_ANIMKIND_ROT_Y, 0, 30, 31,
   Animation_for_L_upleg_motion_ch4_key, Animation_for_L_upleg_motion_ch4_val},
  {NDC_ANIMKIND_ROT_Z, 0, 30, 31,
   Animation_for_L_upleg_motion_ch5_key, Animation_for_L_upleg_motion_ch5_val},
  {NDC_ANIMKIND_SCL_X, 0, 30, 31,
   Animation_for_L_upleg_motion_ch6_key, Animation_for_L_upleg_motion_ch6_val},
  {NDC_ANIMKIND_SCL_Y, 0, 30, 31,
   Animation_for_L_upleg_motion_ch7_key, Animation_for_L_upleg_motion_ch7_val},
  {NDC_ANIMKIND_SCL_Z, 0, 30, 31,
   Animation_for_L_upleg_motion_ch8_key, Animation_for_L_upleg_motion_ch8_val},
};

static NDR_ANIM Animation_for_L_upleg_motion={
  Animation_for_L_upleg_motion_chlist, 9, 30
};

static s16 Animation_for_L_lowleg_motion_ch0_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowleg_motion_ch0_val[]={
  0.0000, 0.0001, 0.0000, 0.0000, -0.0000, -0.0000, -0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, -0.0000, -0.0000, -0.0000, -0.0000, 0.0000,
  0.0000, -0.0000, 0.0000, -0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, 0.0000, 0.0000, -0.0000, -0.0000, 0.0000,
};

static s16 Animation_for_L_lowleg_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowleg_motion_ch1_val[]={
  -31.4106, -24.4725, -18.2452, -12.8160, -8.2608, -4.6433, -2.0141,
  -0.4101, -0.0000, 0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  0.0000, -0.3244, -0.8320, -1.5214, -2.3911, -3.4388, -4.6620, -6.0577,
  -7.9507, -10.4521, -13.2613, -16.3649, -19.7484, -23.3956, -27.2892,
  -31.4106,
};

static s16 Animation_for_L_lowleg_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowleg_motion_ch2_val[]={
  56.6612, 51.0504, 44.6598, 37.5785, 29.9059, 21.7491, 13.2222, 4.4446,
  0.0000, 0.0000, 0.0000, -0.0000, -0.0000, 0.0000, -0.0000, 0.0000,
  3.7145, 7.4084, 11.0727, 14.6984, 18.2768, 21.7990, 25.2565, 29.2987,
  33.8555, 38.2292, 42.3992, 46.3455, 50.0495, 53.4935, 56.6612,
};

static s16 Animation_for_L_lowleg_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowleg_motion_ch3_val[]={
  0.8875, 0.7692, 0.6509, 0.5325, 0.4142, 0.2958, 0.1775, 0.0592, 0.0000,
  0.0000, 0.0000, -0.0000, -0.0000, 0.0000, -0.0000, 0.0000, 0.0494,
  0.0989, 0.1483, 0.1977, 0.2471, 0.2966, 0.3460, 0.4052, 0.4741, 0.5430,
  0.6119, 0.6808, 0.7497, 0.8186, 0.8875,
};

static s16 Animation_for_L_lowleg_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowleg_motion_ch4_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000,
  -0.0000, 0.0000, 0.0000, -0.0000, -0.0000, -0.0000, -0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
};

static s16 Animation_for_L_lowleg_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowleg_motion_ch5_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000,
  -0.0000, 0.0000, -0.0000, 0.0000, -0.0000, 0.0000, -0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
};

static s16 Animation_for_L_lowleg_motion_ch6_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowleg_motion_ch6_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_L_lowleg_motion_ch7_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowleg_motion_ch7_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_L_lowleg_motion_ch8_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowleg_motion_ch8_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static NDR_ANIM_CHANNEL Animation_for_L_lowleg_motion_chlist[]={
  {NDC_ANIMKIND_TRS_X, 0, 30, 31,
   Animation_for_L_lowleg_motion_ch0_key, Animation_for_L_lowleg_motion_ch0_val},
  {NDC_ANIMKIND_TRS_Y, 0, 30, 31,
   Animation_for_L_lowleg_motion_ch1_key, Animation_for_L_lowleg_motion_ch1_val},
  {NDC_ANIMKIND_TRS_Z, 0, 30, 31,
   Animation_for_L_lowleg_motion_ch2_key, Animation_for_L_lowleg_motion_ch2_val},
  {NDC_ANIMKIND_ROT_X, 0, 30, 31,
   Animation_for_L_lowleg_motion_ch3_key, Animation_for_L_lowleg_motion_ch3_val},
  {NDC_ANIMKIND_ROT_Y, 0, 30, 31,
   Animation_for_L_lowleg_motion_ch4_key, Animation_for_L_lowleg_motion_ch4_val},
  {NDC_ANIMKIND_ROT_Z, 0, 30, 31,
   Animation_for_L_lowleg_motion_ch5_key, Animation_for_L_lowleg_motion_ch5_val},
  {NDC_ANIMKIND_SCL_X, 0, 30, 31,
   Animation_for_L_lowleg_motion_ch6_key, Animation_for_L_lowleg_motion_ch6_val},
  {NDC_ANIMKIND_SCL_Y, 0, 30, 31,
   Animation_for_L_lowleg_motion_ch7_key, Animation_for_L_lowleg_motion_ch7_val},
  {NDC_ANIMKIND_SCL_Z, 0, 30, 31,
   Animation_for_L_lowleg_motion_ch8_key, Animation_for_L_lowleg_motion_ch8_val},
};

static NDR_ANIM Animation_for_L_lowleg_motion={
  Animation_for_L_lowleg_motion_chlist, 9, 30
};

static s16 Animation_for_L_shoe_motion_ch0_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_shoe_motion_ch0_val[]={
  2.9298, 3.1947, 3.4591, 3.7233, 3.9868, 4.2497, 4.5122, 4.7740, 4.5823,
  3.9344, 3.2835, 2.6299, 1.9743, 1.3171, 0.6588, 0.0000, -1.3828, -2.7607,
  -4.1292, -5.4834, -6.8187, -8.1304, -9.4140, -9.2342, -7.5766, -5.8758,
  -4.1416, -2.3839, -0.6126, 1.1621, 2.9298,
};

static s16 Animation_for_L_shoe_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_shoe_motion_ch1_val[]={
  -1.0706, -1.2729, -1.4920, -1.7282, -1.9822, -2.2533, -2.5415, -2.8469,
  -2.6213, -1.9305, -1.3444, -0.8634, -0.4884, -0.2193, -0.0563, -0.0000,
  -0.2279, -0.9235, -2.0855, -3.7091, -5.7886, -8.3176, -11.2863, -10.8397,
  -7.1891, -4.2699, -2.0983, -0.6865, -0.0431, -0.1715, -1.0706,
};

static s16 Animation_for_L_shoe_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_shoe_motion_ch2_val[]={
  16.6166, 18.1188, 19.6186, 21.1162, 22.6110, 24.1026, 25.5913, 27.0765,
  25.9882, 22.3140, 18.6221, 14.9154, 11.1970, 7.4696, 3.7363, 0.0000,
  -7.8419, -15.6570, -23.4182, -31.0987, -38.6717, -46.1108, -53.3907,
  -52.3711, -42.9697, -33.3236, -23.4882, -13.5195, -3.4744, 6.5903,
  16.6166,
};

static s16 Animation_for_L_shoe_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_shoe_motion_ch3_val[]={
  0.1231, 0.1343, 0.1455, 0.1567, 0.1679, 0.1791, 0.1903, 0.2015, 0.1933,
  0.1657, 0.1381, 0.1105, 0.0829, 0.0552, 0.0276, -0.0000, -0.0580,
  -0.1160, -0.1739, -0.2319, -0.2898, -0.3477, -0.4056, -0.3974, -0.3232,
  -0.2488, -0.1745, -0.1001, -0.0257, 0.0487, 0.1231,
};

static s16 Animation_for_L_shoe_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_shoe_motion_ch4_val[]={
  0.0013, 0.0016, 0.0019, 0.0022, 0.0025, 0.0028, 0.0032, 0.0036, 0.0033,
  0.0024, 0.0017, 0.0011, 0.0006, 0.0003, 0.0001, 0.0000, 0.0003, 0.0012,
  0.0027, 0.0047, 0.0074, 0.0106, 0.0143, 0.0138, 0.0091, 0.0054, 0.0027,
  0.0009, 0.0001, 0.0002, 0.0013,
};

static s16 Animation_for_L_shoe_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_shoe_motion_ch5_val[]={
  -0.0217, -0.0236, -0.0256, -0.0275, -0.0295, -0.0314, -0.0334, -0.0353,
  -0.0339, -0.0291, -0.0243, -0.0194, -0.0146, -0.0097, -0.0049, 0.0000,
  0.0102, 0.0204, 0.0305, 0.0405, 0.0504, 0.0601, 0.0696, 0.0683, 0.0560,
  0.0434, 0.0306, 0.0176, 0.0045, -0.0086, -0.0217,
};

static s16 Animation_for_L_shoe_motion_ch6_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_shoe_motion_ch6_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_L_shoe_motion_ch7_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_shoe_motion_ch7_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_L_shoe_motion_ch8_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_shoe_motion_ch8_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static NDR_ANIM_CHANNEL Animation_for_L_shoe_motion_chlist[]={
  {NDC_ANIMKIND_TRS_X, 0, 30, 31,
   Animation_for_L_shoe_motion_ch0_key, Animation_for_L_shoe_motion_ch0_val},
  {NDC_ANIMKIND_TRS_Y, 0, 30, 31,
   Animation_for_L_shoe_motion_ch1_key, Animation_for_L_shoe_motion_ch1_val},
  {NDC_ANIMKIND_TRS_Z, 0, 30, 31,
   Animation_for_L_shoe_motion_ch2_key, Animation_for_L_shoe_motion_ch2_val},
  {NDC_ANIMKIND_ROT_X, 0, 30, 31,
   Animation_for_L_shoe_motion_ch3_key, Animation_for_L_shoe_motion_ch3_val},
  {NDC_ANIMKIND_ROT_Y, 0, 30, 31,
   Animation_for_L_shoe_motion_ch4_key, Animation_for_L_shoe_motion_ch4_val},
  {NDC_ANIMKIND_ROT_Z, 0, 30, 31,
   Animation_for_L_shoe_motion_ch5_key, Animation_for_L_shoe_motion_ch5_val},
  {NDC_ANIMKIND_SCL_X, 0, 30, 31,
   Animation_for_L_shoe_motion_ch6_key, Animation_for_L_shoe_motion_ch6_val},
  {NDC_ANIMKIND_SCL_Y, 0, 30, 31,
   Animation_for_L_shoe_motion_ch7_key, Animation_for_L_shoe_motion_ch7_val},
  {NDC_ANIMKIND_SCL_Z, 0, 30, 31,
   Animation_for_L_shoe_motion_ch8_key, Animation_for_L_shoe_motion_ch8_val},
};

static NDR_ANIM Animation_for_L_shoe_motion={
  Animation_for_L_shoe_motion_chlist, 9, 30
};

static s16 Animation_for_L_leg_motion_ch0_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_leg_motion_ch0_val[]={
  -2.9680, -2.8560, -2.7424, -2.6273, -2.5107, -2.3925, -2.2729, -2.1519,
  -1.8184, -1.3274, -0.9105, -0.5698, -0.3074, -0.1245, -0.0219, 0.0000,
  -0.0019, -0.0138, -0.0355, -0.0662, -0.1050, -0.1508, -0.2021, -0.2287,
  -0.2937, -0.4590, -0.7354, -1.1281, -1.6365, -2.2540, -2.9680,
};

static s16 Animation_for_L_leg_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_leg_motion_ch1_val[]={
  12.5350, 12.6035, 12.6707, 12.7366, 12.8013, 12.8646, 12.9265, 12.9871,
  12.2027, 10.5374, 8.8303, 7.0908, 5.3285, 3.5529, 1.7736, 0.0000,
  -0.0310, -0.0598, -0.0815, -0.0915, -0.0852, -0.0579, -0.0053, 0.7773,
  2.3651, 4.0521, 5.7991, 7.5642, 9.3038, 10.9748, 12.5350,
};

static s16 Animation_for_L_leg_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_leg_motion_ch2_val[]={
  -1.5498, -1.5720, -1.5942, -1.6167, -1.6392, -1.6619, -1.6847, -1.7077,
  -1.4511, -0.9791, -0.5943, -0.2978, -0.0902, 0.0282, 0.0580, -0.0000,
  -0.0128, -0.0865, -0.2209, -0.4153, -0.6688, -0.9800, -1.3472, -1.1296,
  -0.4479, 0.0023, 0.2061, 0.1540, -0.1582, -0.7286, -1.5498,
};

static s16 Animation_for_L_leg_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_leg_motion_ch3_val[]={
  -0.5236, -0.5280, -0.5324, -0.5368, -0.5412, -0.5456, -0.5500, -0.5544,
  -0.5198, -0.4460, -0.3720, -0.2978, -0.2234, -0.1490, -0.0745, -0.0000,
  0.0722, 0.1444, 0.2166, 0.2886, 0.3606, 0.4324, 0.5041, 0.4694, 0.3279,
  0.1860, 0.0438, -0.0985, -0.2406, -0.3824, -0.5236,
};

static s16 Animation_for_L_leg_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_leg_motion_ch4_val[]={
  -0.0032, -0.0050, -0.0067, -0.0084, -0.0101, -0.0118, -0.0134, -0.0150,
  -0.0119, -0.0052, -0.0002, 0.0031, 0.0047, 0.0047, 0.0031, 0.0000,
  -0.0000, -0.0014, -0.0042, -0.0083, -0.0138, -0.0206, -0.0287, -0.0226,
  -0.0044, 0.0091, 0.0176, 0.0208, 0.0184, 0.0103, -0.0032,
};

static s16 Animation_for_L_leg_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_leg_motion_ch5_val[]={
  0.0826, 0.0715, 0.0603, 0.0492, 0.0381, 0.0269, 0.0158, 0.0047, -0.0009,
  -0.0008, -0.0004, 0.0001, 0.0005, 0.0007, 0.0006, 0.0000, -0.0028,
  -0.0056, -0.0086, -0.0118, -0.0153, -0.0192, -0.0237, -0.0156, 0.0031,
  0.0188, 0.0324, 0.0447, 0.0565, 0.0689, 0.0826,
};

static s16 Animation_for_L_leg_motion_ch6_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_leg_motion_ch6_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_L_leg_motion_ch7_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_leg_motion_ch7_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_L_leg_motion_ch8_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_leg_motion_ch8_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static NDR_ANIM_CHANNEL Animation_for_L_leg_motion_chlist[]={
  {NDC_ANIMKIND_TRS_X, 0, 30, 31,
   Animation_for_L_leg_motion_ch0_key, Animation_for_L_leg_motion_ch0_val},
  {NDC_ANIMKIND_TRS_Y, 0, 30, 31,
   Animation_for_L_leg_motion_ch1_key, Animation_for_L_leg_motion_ch1_val},
  {NDC_ANIMKIND_TRS_Z, 0, 30, 31,
   Animation_for_L_leg_motion_ch2_key, Animation_for_L_leg_motion_ch2_val},
  {NDC_ANIMKIND_ROT_X, 0, 30, 31,
   Animation_for_L_leg_motion_ch3_key, Animation_for_L_leg_motion_ch3_val},
  {NDC_ANIMKIND_ROT_Y, 0, 30, 31,
   Animation_for_L_leg_motion_ch4_key, Animation_for_L_leg_motion_ch4_val},
  {NDC_ANIMKIND_ROT_Z, 0, 30, 31,
   Animation_for_L_leg_motion_ch5_key, Animation_for_L_leg_motion_ch5_val},
  {NDC_ANIMKIND_SCL_X, 0, 30, 31,
   Animation_for_L_leg_motion_ch6_key, Animation_for_L_leg_motion_ch6_val},
  {NDC_ANIMKIND_SCL_Y, 0, 30, 31,
   Animation_for_L_leg_motion_ch7_key, Animation_for_L_leg_motion_ch7_val},
  {NDC_ANIMKIND_SCL_Z, 0, 30, 31,
   Animation_for_L_leg_motion_ch8_key, Animation_for_L_leg_motion_ch8_val},
};

static NDR_ANIM Animation_for_L_leg_motion={
  Animation_for_L_leg_motion_chlist, 9, 30
};

static s16 Animation_for_hip_motion_ch0_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_hip_motion_ch0_val[]={
  0.0000, -0.1743, -0.3482, -0.5216, -0.6944, -0.8665, -1.0380, -1.2087,
  -1.2695, -1.2219, -1.1753, -1.1298, -1.0854, -1.0421, -1.0000, -0.9590,
  -0.9093, -0.8594, -0.8093, -0.7589, -0.7083, -0.6575, -0.6065, -0.5431,
  -0.4670, -0.3905, -0.3133, -0.2357, -0.1576, -0.0790, 0.0000,
};

static s16 Animation_for_hip_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_hip_motion_ch1_val[]={
  0.0000, 0.0852, 0.1716, 0.2591, 0.3478, 0.4376, 0.5284, 0.6203, 0.6618,
  0.6513, 0.6397, 0.6271, 0.6135, 0.5990, 0.5835, 0.5670, 0.5337, 0.5009,
  0.4687, 0.4369, 0.4057, 0.3750, 0.3449, 0.3052, 0.2568, 0.2100, 0.1648,
  0.1211, 0.0791, 0.0387, 0.0000,
};

static s16 Animation_for_hip_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_hip_motion_ch2_val[]={
  -0.0000, -0.0015, -0.0061, -0.0138, -0.0245, -0.0382, -0.0551, -0.0750,
  -0.0833, -0.0779, -0.0728, -0.0678, -0.0631, -0.0587, -0.0544, -0.0504,
  -0.0451, -0.0402, -0.0355, -0.0311, -0.0270, -0.0232, -0.0197, -0.0157,
  -0.0115, -0.0080, -0.0051, -0.0029, -0.0013, -0.0003, -0.0000,
};

static s16 Animation_for_hip_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_hip_motion_ch3_val[]={
  0.0000, -0.0069, -0.0138, -0.0208, -0.0278, -0.0349, -0.0421, -0.0492,
  -0.0518, -0.0496, -0.0475, -0.0453, -0.0432, -0.0410, -0.0389, -0.0368,
  -0.0349, -0.0331, -0.0313, -0.0294, -0.0276, -0.0258, -0.0240, -0.0215,
  -0.0184, -0.0154, -0.0123, -0.0092, -0.0061, -0.0031, 0.0000,
};

static s16 Animation_for_hip_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_hip_motion_ch4_val[]={
  0.0000, -0.0141, -0.0283, -0.0424, -0.0565, -0.0706, -0.0847, -0.0988,
  -0.1042, -0.1009, -0.0978, -0.0946, -0.0916, -0.0885, -0.0855, -0.0826,
  -0.0781, -0.0736, -0.0691, -0.0646, -0.0602, -0.0557, -0.0512, -0.0457,
  -0.0391, -0.0325, -0.0259, -0.0194, -0.0129, -0.0064, 0.0000,
};

static s16 Animation_for_hip_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_hip_motion_ch5_val[]={
  0.0000, -0.0016, -0.0032, -0.0047, -0.0062, -0.0077, -0.0092, -0.0106,
  -0.0182, -0.0319, -0.0456, -0.0594, -0.0732, -0.0870, -0.1008, -0.1147,
  -0.1097, -0.1047, -0.0997, -0.0946, -0.0896, -0.0845, -0.0795, -0.0719,
  -0.0617, -0.0515, -0.0413, -0.0310, -0.0207, -0.0104, 0.0000,
};

static s16 Animation_for_hip_motion_ch6_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_hip_motion_ch6_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_hip_motion_ch7_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_hip_motion_ch7_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_hip_motion_ch8_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_hip_motion_ch8_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static NDR_ANIM_CHANNEL Animation_for_hip_motion_chlist[]={
  {NDC_ANIMKIND_TRS_X, 0, 30, 31,
   Animation_for_hip_motion_ch0_key, Animation_for_hip_motion_ch0_val},
  {NDC_ANIMKIND_TRS_Y, 0, 30, 31,
   Animation_for_hip_motion_ch1_key, Animation_for_hip_motion_ch1_val},
  {NDC_ANIMKIND_TRS_Z, 0, 30, 31,
   Animation_for_hip_motion_ch2_key, Animation_for_hip_motion_ch2_val},
  {NDC_ANIMKIND_ROT_X, 0, 30, 31,
   Animation_for_hip_motion_ch3_key, Animation_for_hip_motion_ch3_val},
  {NDC_ANIMKIND_ROT_Y, 0, 30, 31,
   Animation_for_hip_motion_ch4_key, Animation_for_hip_motion_ch4_val},
  {NDC_ANIMKIND_ROT_Z, 0, 30, 31,
   Animation_for_hip_motion_ch5_key, Animation_for_hip_motion_ch5_val},
  {NDC_ANIMKIND_SCL_X, 0, 30, 31,
   Animation_for_hip_motion_ch6_key, Animation_for_hip_motion_ch6_val},
  {NDC_ANIMKIND_SCL_Y, 0, 30, 31,
   Animation_for_hip_motion_ch7_key, Animation_for_hip_motion_ch7_val},
  {NDC_ANIMKIND_SCL_Z, 0, 30, 31,
   Animation_for_hip_motion_ch8_key, Animation_for_hip_motion_ch8_val},
};

static NDR_ANIM Animation_for_hip_motion={
  Animation_for_hip_motion_chlist, 9, 30
};

static s16 Animation_for_R_leg_motion_ch0_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_leg_motion_ch0_val[]={
  0.0000, 0.0532, 0.0661, 0.0425, -0.0135, -0.0977, -0.2057, -0.3330,
  -0.3196, -0.1611, -0.0172, 0.1053, 0.1986, 0.2537, 0.2610, 0.2100,
  0.2061, 0.2020, 0.1978, 0.1934, 0.1889, 0.1842, 0.1793, 0.1676, 0.1435,
  0.1149, 0.0850, 0.0566, 0.0319, 0.0126, 0.0000,
};

static s16 Animation_for_R_leg_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_leg_motion_ch1_val[]={
  0.0000, -1.0974, -2.1339, -3.1040, -4.0031, -4.8276, -5.5746, -6.2422,
  -6.1657, -5.2425, -4.1270, -2.8562, -1.4741, -0.0308, 1.4194, 2.8195,
  2.8800, 2.9403, 3.0004, 3.0603, 3.1200, 3.1795, 3.2388, 3.1503, 2.8590,
  2.5000, 2.0816, 1.6125, 1.1024, 0.5614, 0.0000,
};

static s16 Animation_for_R_leg_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_leg_motion_ch2_val[]={
  0.0000, -0.4433, -0.9447, -1.4975, -2.0944, -2.7283, -3.3915, -4.0765,
  -3.8953, -2.9692, -2.2555, -1.7916, -1.6079, -1.7267, -2.1604, -2.9112,
  -2.9507, -2.9904, -3.0303, -3.0706, -3.1110, -3.1518, -3.1928, -2.8432,
  -2.1601, -1.5594, -1.0481, -0.6320, -0.3162, -0.1045, 0.0000,
};

static s16 Animation_for_R_leg_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_leg_motion_ch3_val[]={
  0.0000, 0.0720, 0.1440, 0.2161, 0.2883, 0.3608, 0.4336, 0.5068, 0.4615,
  0.2978, 0.1346, -0.0283, -0.1909, -0.3530, -0.5145, -0.6750, -0.6802,
  -0.6855, -0.6908, -0.6960, -0.7013, -0.7066, -0.7118, -0.6671, -0.5723,
  -0.4773, -0.3821, -0.2868, -0.1913, -0.0957, 0.0000,
};

static s16 Animation_for_R_leg_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_leg_motion_ch4_val[]={
  0.0000, 0.0165, 0.0343, 0.0531, 0.0726, 0.0926, 0.1128, 0.1329, 0.1298,
  0.1054, 0.0845, 0.0684, 0.0582, 0.0550, 0.0594, 0.0719, 0.0728, 0.0738,
  0.0747, 0.0756, 0.0766, 0.0775, 0.0784, 0.0702, 0.0540, 0.0396, 0.0272,
  0.0169, 0.0088, 0.0032, 0.0000,
};

static s16 Animation_for_R_leg_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_leg_motion_ch5_val[]={
  0.0000, -0.0291, -0.0558, -0.0801, -0.1018, -0.1211, -0.1377, -0.1519,
  -0.1561, -0.1477, -0.1334, -0.1140, -0.0902, -0.0631, -0.0341, -0.0046,
  -0.0030, -0.0015, 0.0001, 0.0017, 0.0032, 0.0048, 0.0063, 0.0091,
  0.0119, 0.0129, 0.0125, 0.0108, 0.0080, 0.0043, 0.0000,
};

static s16 Animation_for_R_leg_motion_ch6_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_leg_motion_ch6_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_R_leg_motion_ch7_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_leg_motion_ch7_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_R_leg_motion_ch8_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_leg_motion_ch8_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static NDR_ANIM_CHANNEL Animation_for_R_leg_motion_chlist[]={
  {NDC_ANIMKIND_TRS_X, 0, 30, 31,
   Animation_for_R_leg_motion_ch0_key, Animation_for_R_leg_motion_ch0_val},
  {NDC_ANIMKIND_TRS_Y, 0, 30, 31,
   Animation_for_R_leg_motion_ch1_key, Animation_for_R_leg_motion_ch1_val},
  {NDC_ANIMKIND_TRS_Z, 0, 30, 31,
   Animation_for_R_leg_motion_ch2_key, Animation_for_R_leg_motion_ch2_val},
  {NDC_ANIMKIND_ROT_X, 0, 30, 31,
   Animation_for_R_leg_motion_ch3_key, Animation_for_R_leg_motion_ch3_val},
  {NDC_ANIMKIND_ROT_Y, 0, 30, 31,
   Animation_for_R_leg_motion_ch4_key, Animation_for_R_leg_motion_ch4_val},
  {NDC_ANIMKIND_ROT_Z, 0, 30, 31,
   Animation_for_R_leg_motion_ch5_key, Animation_for_R_leg_motion_ch5_val},
  {NDC_ANIMKIND_SCL_X, 0, 30, 31,
   Animation_for_R_leg_motion_ch6_key, Animation_for_R_leg_motion_ch6_val},
  {NDC_ANIMKIND_SCL_Y, 0, 30, 31,
   Animation_for_R_leg_motion_ch7_key, Animation_for_R_leg_motion_ch7_val},
  {NDC_ANIMKIND_SCL_Z, 0, 30, 31,
   Animation_for_R_leg_motion_ch8_key, Animation_for_R_leg_motion_ch8_val},
};

static NDR_ANIM Animation_for_R_leg_motion={
  Animation_for_R_leg_motion_chlist, 9, 30
};

static s16 Animation_for_R_lowleg_motion_ch0_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowleg_motion_ch0_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001,
  0.0001, 0.0001, 0.0001, 0.0001, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000,
};

static s16 Animation_for_R_lowleg_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowleg_motion_ch1_val[]={
  0.0000, -0.1175, -0.2692, -0.4549, -0.6747, -0.9285, -1.2161, -1.5374,
  -2.7825, -5.6340, -9.3968, -14.0217, -19.4481, -25.6052, -32.4122,
  -39.7802, -33.6029, -27.8046, -22.4380, -17.5519, -13.1906, -9.3940,
  -6.1963, -4.3016, -3.3290, -2.4737, -1.7372, -1.1207, -0.6252, -0.2514,
  0.0000,
};

static s16 Animation_for_R_lowleg_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowleg_motion_ch2_val[]={
  0.0000, 1.6047, 3.2065, 4.8047, 6.3986, 7.9874, 9.5704, 11.1470, 16.1136,
  24.2577, 32.0229, 39.3077, 46.0167, 52.0622, 57.3652, 61.8562, 58.1712,
  53.9148, 49.1254, 43.8469, 38.1269, 32.0177, 25.5747, 20.8178, 17.9305,
  15.0063, 12.0499, 9.0662, 6.0600, 3.0363, 0.0000,
};

static s16 Animation_for_R_lowleg_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowleg_motion_ch3_val[]={
  -0.0000, 0.0213, 0.0427, 0.0640, 0.0853, 0.1066, 0.1280, 0.1493, 0.2172,
  0.3316, 0.4460, 0.5605, 0.6749, 0.7893, 0.9038, 1.0182, 0.9228, 0.8275,
  0.7321, 0.6367, 0.5413, 0.4460, 0.3506, 0.2827, 0.2423, 0.2019, 0.1615,
  0.1212, 0.0808, 0.0404, -0.0000,
};

static s16 Animation_for_R_lowleg_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowleg_motion_ch4_val[]={
  0.0000, 0.0000, -0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000,
  -0.0000, -0.0000, 0.0000, -0.0000, 0.0000, 0.0000, 0.0000, -0.0000,
  -0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000,
  -0.0000, -0.0000, -0.0000, 0.0000, -0.0000, 0.0000,
};

static s16 Animation_for_R_lowleg_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowleg_motion_ch5_val[]={
  0.0000, 0.0000, -0.0000, -0.0000, 0.0000, 0.0000, -0.0000, 0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, 0.0000, 0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, 0.0000, 0.0000, 0.0000,
};

static s16 Animation_for_R_lowleg_motion_ch6_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowleg_motion_ch6_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_R_lowleg_motion_ch7_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowleg_motion_ch7_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_R_lowleg_motion_ch8_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowleg_motion_ch8_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static NDR_ANIM_CHANNEL Animation_for_R_lowleg_motion_chlist[]={
  {NDC_ANIMKIND_TRS_X, 0, 30, 31,
   Animation_for_R_lowleg_motion_ch0_key, Animation_for_R_lowleg_motion_ch0_val},
  {NDC_ANIMKIND_TRS_Y, 0, 30, 31,
   Animation_for_R_lowleg_motion_ch1_key, Animation_for_R_lowleg_motion_ch1_val},
  {NDC_ANIMKIND_TRS_Z, 0, 30, 31,
   Animation_for_R_lowleg_motion_ch2_key, Animation_for_R_lowleg_motion_ch2_val},
  {NDC_ANIMKIND_ROT_X, 0, 30, 31,
   Animation_for_R_lowleg_motion_ch3_key, Animation_for_R_lowleg_motion_ch3_val},
  {NDC_ANIMKIND_ROT_Y, 0, 30, 31,
   Animation_for_R_lowleg_motion_ch4_key, Animation_for_R_lowleg_motion_ch4_val},
  {NDC_ANIMKIND_ROT_Z, 0, 30, 31,
   Animation_for_R_lowleg_motion_ch5_key, Animation_for_R_lowleg_motion_ch5_val},
  {NDC_ANIMKIND_SCL_X, 0, 30, 31,
   Animation_for_R_lowleg_motion_ch6_key, Animation_for_R_lowleg_motion_ch6_val},
  {NDC_ANIMKIND_SCL_Y, 0, 30, 31,
   Animation_for_R_lowleg_motion_ch7_key, Animation_for_R_lowleg_motion_ch7_val},
  {NDC_ANIMKIND_SCL_Z, 0, 30, 31,
   Animation_for_R_lowleg_motion_ch8_key, Animation_for_R_lowleg_motion_ch8_val},
};

static NDR_ANIM Animation_for_R_lowleg_motion={
  Animation_for_R_lowleg_motion_chlist, 9, 30
};

static s16 Animation_for_R_shoe_motion_ch0_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_shoe_motion_ch0_val[]={
  0.0000, 1.2185, 1.4119, 0.5799, -1.2640, -4.0932, -7.8684, -12.5377,
  -13.5435, -10.4534, -7.6507, -5.1606, -3.0037, -1.1975, 0.2448, 1.3140,
  1.9706, 2.4573, 2.7737, 2.9204, 2.8995, 2.7145, 2.3692, 1.2131, -0.3251,
  -1.4292, -2.0815, -2.2696, -1.9861, -1.2286, 0.0000,
};

static s16 Animation_for_R_shoe_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_shoe_motion_ch1_val[]={
  0.0000, -0.2554, -1.0348, -2.3351, -4.1512, -6.4761, -9.3014, -12.6152,
  -12.3408, -8.5991, -5.5132, -3.0998, -1.3712, -0.3361, -0.0001, -0.3646,
  -1.1654, -2.4158, -4.1116, -6.2473, -8.8152, -11.8073, -15.2135, -14.9130,
  -11.0181, -7.6900, -4.9447, -2.7945, -1.2497, -0.3167, 0.0000,
};

static s16 Animation_for_R_shoe_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_shoe_motion_ch2_val[]={
  0.0000, -9.8307, -19.7246, -29.5653, -39.2367, -48.6256, -57.6221,
  -66.1222, -65.7843, -56.7613, -47.4054, -37.7709, -27.9136, -17.8918,
  -7.7645, 2.4082, 10.4511, 18.4487, 26.3721, 34.1927, 41.8834, 49.4160,
  56.7637, 56.6999, 49.1362, 41.2968, 33.2381, 25.0189, 16.6989, 8.3391,
  0.0000,
};

static s16 Animation_for_R_shoe_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_shoe_motion_ch3_val[]={
  0.0000, -0.0620, -0.1251, -0.1895, -0.2551, -0.3219, -0.3900, -0.4593,
  -0.4558, -0.3788, -0.3021, -0.2257, -0.1498, -0.0743, 0.0006, 0.0751,
  0.1347, 0.1941, 0.2532, 0.3120, 0.3706, 0.4288, 0.4868, 0.4804, 0.4101,
  0.3403, 0.2711, 0.2024, 0.1344, 0.0669, 0.0000,
};

static s16 Animation_for_R_shoe_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_shoe_motion_ch4_val[]={
  -0.0000, 0.0557, 0.1108, 0.1651, 0.2188, 0.2717, 0.3239, 0.3753, 0.3943,
  0.3805, 0.3659, 0.3503, 0.3338, 0.3164, 0.2980, 0.2787, 0.2672, 0.2551,
  0.2424, 0.2292, 0.2154, 0.2010, 0.1862, 0.1680, 0.1464, 0.1239, 0.1007,
  0.0768, 0.0520, 0.0264, -0.0000,
};

static s16 Animation_for_R_shoe_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_shoe_motion_ch5_val[]={
  0.0000, -0.0108, -0.0217, -0.0327, -0.0439, -0.0553, -0.0669, -0.0789,
  -0.0787, -0.0658, -0.0527, -0.0395, -0.0263, -0.0130, 0.0001, 0.0131,
  0.0234, 0.0336, 0.0436, 0.0534, 0.0630, 0.0723, 0.0814, 0.0803, 0.0693,
  0.0580, 0.0466, 0.0351, 0.0234, 0.0117, 0.0000,
};

static s16 Animation_for_R_shoe_motion_ch6_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_shoe_motion_ch6_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_R_shoe_motion_ch7_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_shoe_motion_ch7_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_R_shoe_motion_ch8_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_shoe_motion_ch8_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static NDR_ANIM_CHANNEL Animation_for_R_shoe_motion_chlist[]={
  {NDC_ANIMKIND_TRS_X, 0, 30, 31,
   Animation_for_R_shoe_motion_ch0_key, Animation_for_R_shoe_motion_ch0_val},
  {NDC_ANIMKIND_TRS_Y, 0, 30, 31,
   Animation_for_R_shoe_motion_ch1_key, Animation_for_R_shoe_motion_ch1_val},
  {NDC_ANIMKIND_TRS_Z, 0, 30, 31,
   Animation_for_R_shoe_motion_ch2_key, Animation_for_R_shoe_motion_ch2_val},
  {NDC_ANIMKIND_ROT_X, 0, 30, 31,
   Animation_for_R_shoe_motion_ch3_key, Animation_for_R_shoe_motion_ch3_val},
  {NDC_ANIMKIND_ROT_Y, 0, 30, 31,
   Animation_for_R_shoe_motion_ch4_key, Animation_for_R_shoe_motion_ch4_val},
  {NDC_ANIMKIND_ROT_Z, 0, 30, 31,
   Animation_for_R_shoe_motion_ch5_key, Animation_for_R_shoe_motion_ch5_val},
  {NDC_ANIMKIND_SCL_X, 0, 30, 31,
   Animation_for_R_shoe_motion_ch6_key, Animation_for_R_shoe_motion_ch6_val},
  {NDC_ANIMKIND_SCL_Y, 0, 30, 31,
   Animation_for_R_shoe_motion_ch7_key, Animation_for_R_shoe_motion_ch7_val},
  {NDC_ANIMKIND_SCL_Z, 0, 30, 31,
   Animation_for_R_shoe_motion_ch8_key, Animation_for_R_shoe_motion_ch8_val},
};

static NDR_ANIM Animation_for_R_shoe_motion={
  Animation_for_R_shoe_motion_chlist, 9, 30
};

static s16 Animation_for_R_upleg_motion_ch0_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_upleg_motion_ch0_val[]={
  0.0000, 0.0532, 0.0661, 0.0425, -0.0135, -0.0977, -0.2057, -0.3330,
  -0.3196, -0.1611, -0.0172, 0.1053, 0.1986, 0.2537, 0.2610, 0.2100,
  0.2061, 0.2020, 0.1978, 0.1934, 0.1889, 0.1842, 0.1793, 0.1676, 0.1435,
  0.1149, 0.0850, 0.0566, 0.0319, 0.0126, 0.0000,
};

static s16 Animation_for_R_upleg_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_upleg_motion_ch1_val[]={
  0.0000, -1.0974, -2.1339, -3.1040, -4.0031, -4.8276, -5.5746, -6.2422,
  -6.1657, -5.2425, -4.1270, -2.8562, -1.4741, -0.0308, 1.4194, 2.8195,
  2.8800, 2.9403, 3.0004, 3.0603, 3.1200, 3.1795, 3.2388, 3.1503, 2.8590,
  2.5000, 2.0816, 1.6125, 1.1024, 0.5614, 0.0000,
};

static s16 Animation_for_R_upleg_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_upleg_motion_ch2_val[]={
  0.0000, -0.4433, -0.9447, -1.4975, -2.0944, -2.7283, -3.3915, -4.0765,
  -3.8953, -2.9692, -2.2555, -1.7916, -1.6079, -1.7267, -2.1604, -2.9112,
  -2.9507, -2.9904, -3.0303, -3.0706, -3.1110, -3.1518, -3.1928, -2.8432,
  -2.1601, -1.5594, -1.0481, -0.6320, -0.3162, -0.1045, 0.0000,
};

static s16 Animation_for_R_upleg_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_upleg_motion_ch3_val[]={
  0.0000, 0.0720, 0.1440, 0.2161, 0.2883, 0.3608, 0.4336, 0.5068, 0.4615,
  0.2978, 0.1346, -0.0283, -0.1909, -0.3530, -0.5145, -0.6750, -0.6802,
  -0.6855, -0.6908, -0.6960, -0.7013, -0.7066, -0.7118, -0.6671, -0.5723,
  -0.4773, -0.3821, -0.2868, -0.1913, -0.0957, 0.0000,
};

static s16 Animation_for_R_upleg_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_upleg_motion_ch4_val[]={
  0.0000, 0.0165, 0.0343, 0.0531, 0.0726, 0.0926, 0.1128, 0.1329, 0.1298,
  0.1054, 0.0845, 0.0684, 0.0582, 0.0550, 0.0594, 0.0719, 0.0728, 0.0738,
  0.0747, 0.0756, 0.0766, 0.0775, 0.0784, 0.0702, 0.0540, 0.0396, 0.0272,
  0.0169, 0.0088, 0.0032, 0.0000,
};

static s16 Animation_for_R_upleg_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_upleg_motion_ch5_val[]={
  0.0000, -0.0291, -0.0558, -0.0801, -0.1018, -0.1211, -0.1377, -0.1519,
  -0.1561, -0.1477, -0.1334, -0.1140, -0.0902, -0.0631, -0.0341, -0.0046,
  -0.0030, -0.0015, 0.0001, 0.0017, 0.0032, 0.0048, 0.0063, 0.0091,
  0.0119, 0.0129, 0.0125, 0.0108, 0.0080, 0.0043, 0.0000,
};

static s16 Animation_for_R_upleg_motion_ch6_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_upleg_motion_ch6_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_R_upleg_motion_ch7_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_upleg_motion_ch7_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static s16 Animation_for_R_upleg_motion_ch8_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_upleg_motion_ch8_val[]={
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000, 1.0000,
  1.0000, 1.0000, 1.0000, 1.0000,
};

static NDR_ANIM_CHANNEL Animation_for_R_upleg_motion_chlist[]={
  {NDC_ANIMKIND_TRS_X, 0, 30, 31,
   Animation_for_R_upleg_motion_ch0_key, Animation_for_R_upleg_motion_ch0_val},
  {NDC_ANIMKIND_TRS_Y, 0, 30, 31,
   Animation_for_R_upleg_motion_ch1_key, Animation_for_R_upleg_motion_ch1_val},
  {NDC_ANIMKIND_TRS_Z, 0, 30, 31,
   Animation_for_R_upleg_motion_ch2_key, Animation_for_R_upleg_motion_ch2_val},
  {NDC_ANIMKIND_ROT_X, 0, 30, 31,
   Animation_for_R_upleg_motion_ch3_key, Animation_for_R_upleg_motion_ch3_val},
  {NDC_ANIMKIND_ROT_Y, 0, 30, 31,
   Animation_for_R_upleg_motion_ch4_key, Animation_for_R_upleg_motion_ch4_val},
  {NDC_ANIMKIND_ROT_Z, 0, 30, 31,
   Animation_for_R_upleg_motion_ch5_key, Animation_for_R_upleg_motion_ch5_val},
  {NDC_ANIMKIND_SCL_X, 0, 30, 31,
   Animation_for_R_upleg_motion_ch6_key, Animation_for_R_upleg_motion_ch6_val},
  {NDC_ANIMKIND_SCL_Y, 0, 30, 31,
   Animation_for_R_upleg_motion_ch7_key, Animation_for_R_upleg_motion_ch7_val},
  {NDC_ANIMKIND_SCL_Z, 0, 30, 31,
   Animation_for_R_upleg_motion_ch8_key, Animation_for_R_upleg_motion_ch8_val},
};

static NDR_ANIM Animation_for_R_upleg_motion={
  Animation_for_R_upleg_motion_chlist, 9, 30
};

/* ============================================================
          Object Structure Area
 ============================================================ */
/* Object #0 */
static NDR_OBJ _dummy_obj={
  NDC_TYPE_NULL, 0, 0x00000000,
  {0,0,0, 0,0,0, 1,1,1},
  -1,
  0x0000, 0x00000000, 0x00000000, 0x00000000,
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  -1, 0, 0, 0x010203,
  {-1, -1}, 0, NULL
};

/* Object #1 */
static u16 _body_link[]={ 1, 4, 5, 8, 9, 14, 13, 10 };
static NDR_OBJ _body={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {0.000000,0.000000,0.000000,
   0.000000,0.000000,0.000000,
   1.000000,1.000000,1.000000},
  0,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  0, 0, 2, 0x010203,
  {-1, -1}, 8, _body_link
};

/* Object #2 */
static u16 _R_uparm_link[]={ 1 };
static NDR_OBJ _R_uparm={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {25.170159,13.895111,-5.565050,
   0.055511,0.000101,0.268425,
   1.000000,1.000000,1.000000},
  1,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  1, 0, 2, 0x010203,
  {-1, -1}, 1, _R_uparm_link
};

/* Object #3 */
static u16 _R_lowarm_link[]={ 1 };
static NDR_OBJ _R_lowarm={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {1.274824,1.751316,-8.114614,
   -0.417670,0.287103,-0.061247,
   1.000000,1.000000,1.000000},
  2,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  2, 0, 2, 0x010203,
  {-1, -1}, 1, _R_lowarm_link
};

/* Object #4 */
static NDR_OBJ _R_hand={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {-30.255537,-40.494070,-67.186980,
   -1.570797,0.641539,-0.640677,
   1.000000,1.000000,1.000001},
  3,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  3, 0, 2, 0x010203,
  {-1, -1}, 0, NULL
};

/* Object #5 */
static NDR_OBJ _R_sode={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {25.170159,13.895111,-5.565050,
   0.055511,0.000101,0.268425,
   1.000000,1.000000,1.000000},
  4,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  4, 0, 2, 0x010203,
  {-1, -1}, 0, NULL
};

/* Object #6 */
static u16 _L_uparm_link[]={ 1 };
static NDR_OBJ _L_uparm={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {-6.625543,5.049620,-30.168858,
   0.414456,0.247633,0.001755,
   1.000000,1.000000,1.000000},
  5,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  5, 0, 2, 0x010203,
  {-1, -1}, 1, _L_uparm_link
};

/* Object #7 */
static u16 _L_lowarm_link[]={ 1 };
static NDR_OBJ _L_lowarm={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {-1.001889,1.376240,-12.218447,
   -0.642528,-0.411298,0.138618,
   1.000000,1.000001,1.000000},
  6,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  6, 0, 2, 0x010203,
  {-1, -1}, 1, _L_lowarm_link
};

/* Object #8 */
static NDR_OBJ _L_hand={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {-0.111365,0.114808,-1.477478,
   -0.015596,-0.014771,0.000142,
   1.000001,0.999999,1.000000},
  7,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  7, 0, 2, 0x010203,
  {-1, -1}, 0, NULL
};

/* Object #9 */
static NDR_OBJ _L_sode={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {-6.625543,5.049620,-30.168858,
   0.414456,0.247633,0.001755,
   1.000000,1.000000,1.000000},
  8,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  8, 0, 2, 0x010203,
  {-1, -1}, 0, NULL
};

/* Object #10 */
static NDR_OBJ _head={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {0.000000,0.000000,0.000000,
   0.000000,0.000000,0.000000,
   1.000000,1.000000,1.000000},
  9,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  9, 0, 2, 0x010203,
  {-1, -1}, 0, NULL
};

/* Object #11 */
static u16 _L_upleg_link[]={ 1 };
static NDR_OBJ _L_upleg={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {-2.968035,12.535017,-1.549795,
   -0.523554,-0.003201,0.082629,
   1.000000,1.000000,1.000000},
  10,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  10, 0, 2, 0x010203,
  {-1, -1}, 1, _L_upleg_link
};

/* Object #12 */
static u16 _L_lowleg_link[]={ 1 };
static NDR_OBJ _L_lowleg={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {0.000006,-31.410644,56.661229,
   0.887548,0.000002,0.000002,
   1.000000,1.000000,1.000000},
  11,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  11, 0, 2, 0x010203,
  {-1, -1}, 1, _L_lowleg_link
};

/* Object #13 */
static NDR_OBJ _L_shoe={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {2.929845,-1.070611,16.616549,
   0.123131,0.001335,-0.021658,
   1.000000,1.000001,1.000000},
  12,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  12, 0, 2, 0x010203,
  {-1, -1}, 0, NULL
};

/* Object #14 */
static NDR_OBJ _L_leg={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {-2.968035,12.535017,-1.549795,
   -0.523554,-0.003201,0.082629,
   1.000000,1.000000,1.000000},
  13,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  13, 0, 2, 0x010203,
  {-1, -1}, 0, NULL
};

/* Object #15 */
static u16 _hip_link[]={ 1, 4 };
static NDR_OBJ _hip={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {0.000000,0.000000,-0.000005,
   0.000000,0.000000,0.000000,
   1.000000,1.000000,1.000000},
  14,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  14, 0, 2, 0x010203,
  {-1, -1}, 2, _hip_link
};

/* Object #16 */
static u16 _R_leg_link[]={ 1 };
static NDR_OBJ _R_leg={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {0.000000,0.000000,0.000005,
   0.000000,0.000000,0.000000,
   1.000000,1.000000,1.000000},
  15,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  15, 0, 2, 0x010203,
  {-1, -1}, 1, _R_leg_link
};

/* Object #17 */
static u16 _R_lowleg_link[]={ 1 };
static NDR_OBJ _R_lowleg={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {0.000000,0.000000,0.000000,
   -0.000000,0.000000,0.000000,
   1.000000,1.000000,1.000000},
  16,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  16, 0, 2, 0x010203,
  {-1, -1}, 1, _R_lowleg_link
};

/* Object #18 */
static NDR_OBJ _R_shoe={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {0.000000,0.000000,0.000000,
   0.000000,-0.000000,0.000000,
   1.000000,1.000000,1.000000},
  17,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  17, 0, 2, 0x010203,
  {-1, -1}, 0, NULL
};

/* Object #19 */
static NDR_OBJ _R_upleg={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {0.000000,0.000000,0.000005,
   0.000000,0.000000,0.000000,
   1.000000,1.000000,1.000000},
  18,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  18, 0, 2, 0x010203,
  {-1, -1}, 0, NULL
};

/* Object #20 */
static NDR_OBJ _NULL_OBJECT_1={
  NDC_TYPE_NULL, 0, 0x00000000,
  {0,0,0, 0,0,0, 1,1,1},
  -1,
  0x0000, 0x00000000, 0x00000000, 0x00000000,
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  -1, 0, 0, 0x010203,
  {-1, -1}, 0, NULL
};

/* Object #21 */
static NDR_OBJ _NULL_OBJECT_2={
  NDC_TYPE_NULL, 0, 0x00000000,
  {0,0,0, 0,0,0, 1,1,1},
  -1,
  0x0000, 0x00000000, 0x00000000, 0x00000000,
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  -1, 0, 0, 0x010203,
  {-1, -1}, 0, NULL
};

/* Object #22 */
static NDR_OBJ _NULL_OBJECT_3={
  NDC_TYPE_NULL, 0, 0x00000000,
  {0,0,0, 0,0,0, 1,1,1},
  -1,
  0x0000, 0x00000000, 0x00000000, 0x00000000,
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  -1, 0, 0, 0x010203,
  {-1, -1}, 0, NULL
};

/* ============================================================
          Scene Structure Area
 ============================================================ */
static NDR_SCN_INST_OBJ walk_scene_instlist[]={
 {
  {0,0,0, 0,0,0, 1,1,1},
  20 /* NULL_OBJECT_1 */
 },
 {
  {0,0,0, 0,0,0, 1,1,1},
  22 /* NULL_OBJECT_3 */
 },
 {
  {0,0,0, 0,0,0, 1,1,1},
  21 /* NULL_OBJECT_2 */
 },
 {
  {0.000000,0.000000,0.000000,
   0.000000,0.000000,0.000000,
   1.000000,1.000000,1.000000},
  1 /* body */
 },
};

static NDR_SCN_LIGHT _walk_scene_light[]={
  { 0, 0xffffff00, 0.57735, 0.57735, 0.57735, 0, 0 }
};

static NDR_SCENE walk_scene={
  {950.000000, 1000.000000, 0x4c, 0x4c, 0x4c, 0xff},
  {1, 45.841393, 1.333330, 1.000000, 0, 0, 0, 0, 1, 10000, 0, 1, 2},
  { 0x202020ff, 1, _walk_scene_light },
  0x0000, 0xfffc,
  walk_scene_instlist, 4,
};

/* ============================================================
          Shape Structure List
 ============================================================ */
static NDR_SHAPE *shapelist_frake_walk[] = {
  &shape_Shape_of___POLYHEDRON_300_,
  &shape_Shape_of___POLYHEDRON_314_,
  &shape_Shape_of___POLYHEDRON_316_,
  &shape_Shape_of___POLYHEDRON_318_,
  &shape_Shape_of___POLYHEDRON_312_,
  &shape_Shape_of___POLYHEDRON_306_,
  &shape_Shape_of___POLYHEDRON_308_,
  &shape_Shape_of___POLYHEDRON_310_,
  &shape_Shape_of___POLYHEDRON_304_,
  &shape_Shape_of___POLYHEDRON_264_,
  &shape_Shape_of___POLYHEDRON_328_,
  &shape_Shape_of___POLYHEDRON_332_,
  &shape_Shape_of___POLYHEDRON_336_,
  &shape_Shape_of___POLYHEDRON_324_,
  &shape_Shape_of___POLYHEDRON_322_,
  &shape_Shape_of___POLYHEDRON_320_,
  &shape_Shape_of___POLYHEDRON_330_,
  &shape_Shape_of___POLYHEDRON_334_,
  &shape_Shape_of___POLYHEDRON_326_,
};

/* ============================================================
          Geometric Animation List
 ============================================================ */
static NDR_ANIM *animlist_frake_walk[]={
  &Animation_for_body_motion,
  &Animation_for_R_uparm_motion,
  &Animation_for_R_lowarm_motion,
  &Animation_for_R_hand_motion,
  &Animation_for_R_sode_motion,
  &Animation_for_L_uparm_motion,
  &Animation_for_L_lowarm_motion,
  &Animation_for_L_hand_motion,
  &Animation_for_L_sode_motion,
  &Animation_for_head_motion,
  &Animation_for_L_upleg_motion,
  &Animation_for_L_lowleg_motion,
  &Animation_for_L_shoe_motion,
  &Animation_for_L_leg_motion,
  &Animation_for_hip_motion,
  &Animation_for_R_leg_motion,
  &Animation_for_R_lowleg_motion,
  &Animation_for_R_shoe_motion,
  &Animation_for_R_upleg_motion,
};

/* ============================================================
          Object Struct List
 ============================================================ */
static NDR_OBJ *objlist_frake_walk[]={
  &_dummy_obj, /* 0 */
  &_body, /* 1 */
  &_R_uparm, /* 2 */
  &_R_lowarm, /* 3 */
  &_R_hand, /* 4 */
  &_R_sode, /* 5 */
  &_L_uparm, /* 6 */
  &_L_lowarm, /* 7 */
  &_L_hand, /* 8 */
  &_L_sode, /* 9 */
  &_head, /* 10 */
  &_L_upleg, /* 11 */
  &_L_lowleg, /* 12 */
  &_L_shoe, /* 13 */
  &_L_leg, /* 14 */
  &_hip, /* 15 */
  &_R_leg, /* 16 */
  &_R_lowleg, /* 17 */
  &_R_shoe, /* 18 */
  &_R_upleg, /* 19 */
  &_NULL_OBJECT_1, /* 20 */
  &_NULL_OBJECT_2, /* 21 */
  &_NULL_OBJECT_3, /* 22 */
};

/* ############################################################
          Resource Information
 ############################################################ */
NDR_RESINFO resinfo_frake_walk={
  &walk_scene,
  20.000000,
  objlist_frake_walk,
  shapelist_frake_walk,
  animlist_frake_walk,
  NULL,
  NULL,
  NULL,
  23, 19, 19, 0, 0, 0
};

