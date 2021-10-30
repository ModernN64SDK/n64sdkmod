
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
extern NDR_RESINFO resinfo_frake_down;
#define RESINFO_NAME resinfo_frake_down

static Gfx _frake_down_aligner0[] = { gsSPEndDisplayList() };

/* ============================================================
          TLUT
 ============================================================ */
static u16 tlut_frake_down[] = {
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
static u8 bmp_frake_down[] = {
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

static Gfx _frake_down_aligner1[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_300_[] = {
  {    44,     5,   -40,0, 0x0000,0x0000, 0x59,0xf9,0xf9,0xff }, /*     0 */
  {   -44,     5,   -40,0, 0x0000,0x0000, 0x41,0xb7,0xb7,0xff }, /*     1 */
  {    40,    31,   -38,0, 0x0000,0x0000, 0xca,0xff,0xff,0xff }, /*     2 */
  {   -11,   107,    -2,0, 0x0000,0x0000, 0x58,0x93,0x93,0xff }, /*     3 */
  {    11,   107,   -25,0, 0x0000,0x0000, 0x7e,0xd3,0xd3,0xff }, /*     4 */
  {   -11,   107,   -25,0, 0x0000,0x0000, 0x88,0xe3,0xe3,0xff }, /*     5 */
  {   -11,   107,    -2,0, 0x0000,0x0000, 0x58,0x93,0x93,0xff }, /*     6 */
  {    11,   107,    -2,0, 0x0000,0x0000, 0x53,0x8b,0x8b,0xff }, /*     7 */
  {    11,   107,   -25,0, 0x0000,0x0000, 0x7e,0xd3,0xd3,0xff }, /*     8 */
  {    41,    31,    28,0, 0x0000,0x0000, 0x95,0xf8,0xf8,0xff }, /*     9 */
  {    40,    31,   -38,0, 0x0000,0x0000, 0xca,0xff,0xff,0xff }, /*    10 */
  {    47,    98,     6,0, 0x0000,0x0000, 0x7c,0xcf,0xcf,0xff }, /*    11 */
  {   -40,    31,   -38,0, 0x0000,0x0000, 0xa5,0xff,0xff,0xff }, /*    12 */
  {   -47,    98,     6,0, 0x0000,0x0000, 0x6c,0xb5,0xb5,0xff }, /*    13 */
  {   -46,    98,   -33,0, 0x0000,0x0000, 0xa4,0xff,0xff,0xff }, /*    14 */
  {   -47,    98,     6,0, 0x0000,0x0000, 0x6c,0xb5,0xb5,0xff }, /*    15 */
  {   -11,   107,   -25,0, 0x0000,0x0000, 0x88,0xe3,0xe3,0xff }, /*    16 */
  {   -46,    98,   -33,0, 0x0000,0x0000, 0xa4,0xff,0xff,0xff }, /*    17 */
  {   -41,    31,    28,0, 0x0000,0x0000, 0x6a,0xb1,0xb1,0xff }, /*    18 */
  {    44,     5,    31,0, 0x0000,0x0000, 0x4a,0xce,0xce,0xff }, /*    19 */
  {    41,    31,    28,0, 0x0000,0x0000, 0x95,0xf8,0xf8,0xff }, /*    20 */
  {   -41,    31,    28,0, 0x0000,0x0000, 0x6a,0xb1,0xb1,0xff }, /*    21 */
  {   -44,     5,    31,0, 0x0000,0x0000, 0x36,0x97,0x97,0xff }, /*    22 */
  {    44,     5,    31,0, 0x0000,0x0000, 0x4a,0xce,0xce,0xff }, /*    23 */
  {   -46,    98,   -33,0, 0x0000,0x0000, 0xa4,0xff,0xff,0xff }, /*    24 */
  {    11,   107,   -25,0, 0x0000,0x0000, 0x7e,0xd3,0xd3,0xff }, /*    25 */
  {    46,    98,   -33,0, 0x0000,0x0000, 0xb2,0xff,0xff,0xff }, /*    26 */
  {   -46,    98,   -33,0, 0x0000,0x0000, 0xa4,0xff,0xff,0xff }, /*    27 */
  {   -11,   107,   -25,0, 0x0000,0x0000, 0x88,0xe3,0xe3,0xff }, /*    28 */
  {    11,   107,   -25,0, 0x0000,0x0000, 0x7e,0xd3,0xd3,0xff }, /*    29 */
  {    46,    98,   -33,0, 0x0000,0x0000, 0xb2,0xff,0xff,0xff }, /*    30 */
  {    11,   107,   -25,0, 0x0000,0x0000, 0x7e,0xd3,0xd3,0xff }, /*    31 */
  {    47,    98,     6,0, 0x0000,0x0000, 0x7c,0xcf,0xcf,0xff }, /*    32 */
  {    41,    31,    28,0, 0x0000,0x0000, 0x95,0xf8,0xf8,0xff }, /*    33 */
  {    44,     5,   -40,0, 0x0000,0x0000, 0x59,0xf9,0xf9,0xff }, /*    34 */
  {    40,    31,   -38,0, 0x0000,0x0000, 0xca,0xff,0xff,0xff }, /*    35 */
  {   -40,    31,   -38,0, 0x0000,0x0000, 0xa5,0xff,0xff,0xff }, /*    36 */
  {   -44,     5,   -40,0, 0x0000,0x0000, 0x41,0xb7,0xb7,0xff }, /*    37 */
  {   -41,    31,    28,0, 0x0000,0x0000, 0x6a,0xb1,0xb1,0xff }, /*    38 */
  {   -40,    31,   -38,0, 0x0000,0x0000, 0xa5,0xff,0xff,0xff }, /*    39 */
  {   -46,    98,   -33,0, 0x0000,0x0000, 0xa4,0xff,0xff,0xff }, /*    40 */
  {    40,    31,   -38,0, 0x0000,0x0000, 0xca,0xff,0xff,0xff }, /*    41 */
  {   -46,    98,   -33,0, 0x0000,0x0000, 0xa4,0xff,0xff,0xff }, /*    42 */
  {    46,    98,   -33,0, 0x0000,0x0000, 0xb2,0xff,0xff,0xff }, /*    43 */
  {    40,    31,   -38,0, 0x0000,0x0000, 0xca,0xff,0xff,0xff }, /*    44 */
  {   -44,     5,   -40,0, 0x0000,0x0000, 0x41,0xb7,0xb7,0xff }, /*    45 */
  {   -40,    31,   -38,0, 0x0000,0x0000, 0xa5,0xff,0xff,0xff }, /*    46 */
  {    40,    31,   -38,0, 0x0000,0x0000, 0xca,0xff,0xff,0xff }, /*    47 */
  {   -44,     5,   -40,0, 0x0000,0x0000, 0x41,0xb7,0xb7,0xff }, /*    48 */
  {   -44,     5,    31,0, 0x0000,0x0000, 0x36,0x97,0x97,0xff }, /*    49 */
  {   -41,    31,    28,0, 0x0000,0x0000, 0x6a,0xb1,0xb1,0xff }, /*    50 */
  {    41,    31,    28,0, 0x0000,0x0000, 0x95,0xf8,0xf8,0xff }, /*    51 */
  {    44,     5,    31,0, 0x0000,0x0000, 0x4a,0xce,0xce,0xff }, /*    52 */
  {    44,     5,   -40,0, 0x0000,0x0000, 0x59,0xf9,0xf9,0xff }, /*    53 */
  {    11,   107,   -25,0, 0x0000,0x0000, 0x7e,0xd3,0xd3,0xff }, /*    54 */
  {    11,   107,    -2,0, 0x0000,0x0000, 0x53,0x8b,0x8b,0xff }, /*    55 */
  {    47,    98,     6,0, 0x0000,0x0000, 0x7c,0xcf,0xcf,0xff }, /*    56 */
  {   -47,    98,     6,0, 0x0000,0x0000, 0x6c,0xb5,0xb5,0xff }, /*    57 */
  {   -11,   107,    -2,0, 0x0000,0x0000, 0x58,0x93,0x93,0xff }, /*    58 */
  {   -11,   107,   -25,0, 0x0000,0x0000, 0x88,0xe3,0xe3,0xff }, /*    59 */
  {   -40,    31,   -38,0, 0x0000,0x0000, 0xa5,0xff,0xff,0xff }, /*    60 */
  {   -41,    31,    28,0, 0x0000,0x0000, 0x6a,0xb1,0xb1,0xff }, /*    61 */
  {   -47,    98,     6,0, 0x0000,0x0000, 0x6c,0xb5,0xb5,0xff }, /*    62 */
  {    40,    31,   -38,0, 0x0000,0x0000, 0xca,0xff,0xff,0xff }, /*    63 */
  {    46,    98,   -33,0, 0x0000,0x0000, 0xb2,0xff,0xff,0xff }, /*    64 */
  {    47,    98,     6,0, 0x0000,0x0000, 0x7c,0xcf,0xcf,0xff }, /*    65 */
  {    41,    31,    28,0, 0x0779,0x0000, 0xf8,0xf8,0xf8,0xff }, /*    66 */
  {    47,    98,     6,0, 0x07ff,0x070b, 0xcf,0xcf,0xcf,0xff }, /*    67 */
  {   -41,    31,    28,0, 0x0086,0x0000, 0xb1,0xb1,0xb1,0xff }, /*    68 */
  {    47,    98,     6,0, 0x07ff,0x070b, 0xcf,0xcf,0xcf,0xff }, /*    69 */
  {   -11,   107,    -2,0, 0x0302,0x0800, 0x93,0x93,0x93,0xff }, /*    70 */
  {   -47,    98,     6,0, 0x0000,0x070b, 0xb5,0xb5,0xb5,0xff }, /*    71 */
  {    47,    98,     6,0, 0x07ff,0x070b, 0xcf,0xcf,0xcf,0xff }, /*    72 */
  {    11,   107,    -2,0, 0x04fd,0x0800, 0x8b,0x8b,0x8b,0xff }, /*    73 */
  {   -11,   107,    -2,0, 0x0302,0x0800, 0x93,0x93,0x93,0xff }, /*    74 */
  {    47,    98,     6,0, 0x07ff,0x070b, 0xcf,0xcf,0xcf,0xff }, /*    75 */
  {   -47,    98,     6,0, 0x0000,0x070b, 0xb5,0xb5,0xb5,0xff }, /*    76 */
  {   -41,    31,    28,0, 0x0086,0x0000, 0xb1,0xb1,0xb1,0xff }, /*    77 */
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
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP2Triangles( 15, 16, 17,  0, 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSP2Triangles( 24, 25, 26,  0, 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 30, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP1Triangle(  1,  2,  3,  0),
  gsSP1Triangle(  4,  5,  6,  0),
  gsSP2Triangles(  7,  8,  9,  0, 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsSP1Triangle( 16, 17, 18,  0),
  gsSP1Triangle( 19, 20, 21,  0),
  gsSP2Triangles( 22, 23, 24,  0, 25, 26, 27,  0),
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
  gsSP1Triangle(  5,  6,  7,  0),
  gsSP1Triangle(  8,  9, 10,  0),
  gsSP1Triangle( 11, 12, 13,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_300_ = {
  gfx_Shape_of___POLYHEDRON_300_,
  vtx_Shape_of___POLYHEDRON_300_, 78, 5,
  bmp_frake_down,
  tlut_frake_down,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_down_aligner2[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_314_[] = {
  {   -92,    45,     4,0, 0x0000,0x0000, 0x8e,0x67,0x4a,0xff }, /*     0 */
  {   -52,    54,     5,0, 0x0000,0x0000, 0xca,0x92,0x69,0xff }, /*     1 */
  {   -75,    71,     5,0, 0x0000,0x0000, 0xfc,0xb6,0x83,0xff }, /*     2 */
  {   -92,    45,     4,0, 0x0000,0x0000, 0x8e,0x67,0x4a,0xff }, /*     3 */
  {   -71,    30,     4,0, 0x0000,0x0000, 0xae,0x7e,0x5a,0xff }, /*     4 */
  {   -52,    54,     5,0, 0x0000,0x0000, 0xca,0x92,0x69,0xff }, /*     5 */
  {   -71,    30,   -20,0, 0x0000,0x0000, 0xf4,0xb1,0x7f,0xff }, /*     6 */
  {   -92,    45,   -20,0, 0x0000,0x0000, 0xff,0xc3,0x8c,0xff }, /*     7 */
  {   -52,    54,   -22,0, 0x0000,0x0000, 0xe8,0xa8,0x79,0xff }, /*     8 */
  {   -71,    30,     4,0, 0x0000,0x0000, 0xae,0x7e,0x5a,0xff }, /*     9 */
  {   -71,    30,   -20,0, 0x0000,0x0000, 0xf4,0xb1,0x7f,0xff }, /*    10 */
  {   -52,    54,     5,0, 0x0000,0x0000, 0xca,0x92,0x69,0xff }, /*    11 */
  {   -92,    45,   -20,0, 0x0000,0x0000, 0xff,0xc3,0x8c,0xff }, /*    12 */
  {   -75,    71,     5,0, 0x0000,0x0000, 0xfc,0xb6,0x83,0xff }, /*    13 */
  {   -75,    71,   -22,0, 0x0000,0x0000, 0xff,0xed,0xab,0xff }, /*    14 */
  {   -52,    54,     5,0, 0x0000,0x0000, 0xca,0x92,0x69,0xff }, /*    15 */
  {   -56,    73,    -8,0, 0x0000,0x0000, 0xff,0xdf,0xa0,0xff }, /*    16 */
  {   -75,    71,     5,0, 0x0000,0x0000, 0xfc,0xb6,0x83,0xff }, /*    17 */
  {   -75,    71,     5,0, 0x0000,0x0000, 0xfc,0xb6,0x83,0xff }, /*    18 */
  {   -56,    73,    -8,0, 0x0000,0x0000, 0xff,0xdf,0xa0,0xff }, /*    19 */
  {   -75,    71,   -22,0, 0x0000,0x0000, 0xff,0xed,0xab,0xff }, /*    20 */
  {   -75,    71,   -22,0, 0x0000,0x0000, 0xff,0xed,0xab,0xff }, /*    21 */
  {   -56,    73,    -8,0, 0x0000,0x0000, 0xff,0xdf,0xa0,0xff }, /*    22 */
  {   -52,    54,   -22,0, 0x0000,0x0000, 0xe8,0xa8,0x79,0xff }, /*    23 */
  {   -52,    54,   -22,0, 0x0000,0x0000, 0xe8,0xa8,0x79,0xff }, /*    24 */
  {   -56,    73,    -8,0, 0x0000,0x0000, 0xff,0xdf,0xa0,0xff }, /*    25 */
  {   -52,    54,     5,0, 0x0000,0x0000, 0xca,0x92,0x69,0xff }, /*    26 */
  {   -92,    45,   -20,0, 0x0000,0x0000, 0xff,0xc3,0x8c,0xff }, /*    27 */
  {   -92,    45,     4,0, 0x0000,0x0000, 0x8e,0x67,0x4a,0xff }, /*    28 */
  {   -75,    71,     5,0, 0x0000,0x0000, 0xfc,0xb6,0x83,0xff }, /*    29 */
  {   -71,    30,   -20,0, 0x0000,0x0000, 0xf4,0xb1,0x7f,0xff }, /*    30 */
  {   -52,    54,   -22,0, 0x0000,0x0000, 0xe8,0xa8,0x79,0xff }, /*    31 */
  {   -52,    54,     5,0, 0x0000,0x0000, 0xca,0x92,0x69,0xff }, /*    32 */
  {   -92,    45,   -20,0, 0x0000,0x0000, 0xff,0xc3,0x8c,0xff }, /*    33 */
  {   -75,    71,   -22,0, 0x0000,0x0000, 0xff,0xed,0xab,0xff }, /*    34 */
  {   -52,    54,   -22,0, 0x0000,0x0000, 0xe8,0xa8,0x79,0xff }, /*    35 */
  {   -71,    30,   -20,0, 0x0000,0x0000, 0xf4,0xb1,0x7f,0xff }, /*    36 */
  {   -88,    29,    -8,0, 0x0000,0x0000, 0xc0,0x53,0x3c,0xff }, /*    37 */
  {   -92,    45,   -20,0, 0x0000,0x0000, 0xff,0xc3,0x8c,0xff }, /*    38 */
  {   -92,    45,   -20,0, 0x0000,0x0000, 0xff,0xc3,0x8c,0xff }, /*    39 */
  {   -88,    29,    -8,0, 0x0000,0x0000, 0xc0,0x53,0x3c,0xff }, /*    40 */
  {   -92,    45,     4,0, 0x0000,0x0000, 0x8e,0x67,0x4a,0xff }, /*    41 */
  {   -92,    45,     4,0, 0x0000,0x0000, 0x8e,0x67,0x4a,0xff }, /*    42 */
  {   -88,    29,    -8,0, 0x0000,0x0000, 0xc0,0x53,0x3c,0xff }, /*    43 */
  {   -71,    30,     4,0, 0x0000,0x0000, 0xae,0x7e,0x5a,0xff }, /*    44 */
  {   -71,    30,     4,0, 0x0000,0x0000, 0xae,0x7e,0x5a,0xff }, /*    45 */
  {   -88,    29,    -8,0, 0x0000,0x0000, 0xc0,0x53,0x3c,0xff }, /*    46 */
  {   -71,    30,   -20,0, 0x0000,0x0000, 0xf4,0xb1,0x7f,0xff }, /*    47 */
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
  gsSP1Triangle(  7,  8,  9,  0),
  gsSP1Triangle( 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_314_ = {
  gfx_Shape_of___POLYHEDRON_314_,
  vtx_Shape_of___POLYHEDRON_314_, 48, 5,
  bmp_frake_down,
  tlut_frake_down,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_down_aligner3[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_316_[] = {
  {  -110,    15,    -1,0, 0x0000,0x0000, 0x90,0x68,0x4b,0xff }, /*     0 */
  {   -74,    29,     3,0, 0x0000,0x0000, 0xfd,0xb7,0x84,0xff }, /*     1 */
  {   -92,    42,     3,0, 0x0000,0x0000, 0xff,0xd7,0x9a,0xff }, /*     2 */
  {  -110,    15,    -1,0, 0x0000,0x0000, 0x90,0x68,0x4b,0xff }, /*     3 */
  {   -94,     3,    -1,0, 0x0000,0x0000, 0xac,0x7c,0x59,0xff }, /*     4 */
  {   -74,    29,     3,0, 0x0000,0x0000, 0xfd,0xb7,0x84,0xff }, /*     5 */
  {   -94,     3,   -15,0, 0x0000,0x0000, 0xf7,0xb3,0x80,0xff }, /*     6 */
  {  -110,    15,   -15,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*     7 */
  {   -74,    29,   -19,0, 0x0000,0x0000, 0xdd,0xa0,0x73,0xff }, /*     8 */
  {   -94,     3,    -1,0, 0x0000,0x0000, 0xac,0x7c,0x59,0xff }, /*     9 */
  {   -94,     3,   -15,0, 0x0000,0x0000, 0xf7,0xb3,0x80,0xff }, /*    10 */
  {   -74,    29,     3,0, 0x0000,0x0000, 0xfd,0xb7,0x84,0xff }, /*    11 */
  {  -110,    15,   -15,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*    12 */
  {   -92,    42,     3,0, 0x0000,0x0000, 0xff,0xd7,0x9a,0xff }, /*    13 */
  {   -92,    42,   -19,0, 0x0000,0x0000, 0xff,0xdc,0x9e,0xff }, /*    14 */
  {   -74,    29,     3,0, 0x0000,0x0000, 0xfd,0xb7,0x84,0xff }, /*    15 */
  {   -77,    44,    -8,0, 0x0000,0x0000, 0xff,0xf3,0xaf,0xff }, /*    16 */
  {   -92,    42,     3,0, 0x0000,0x0000, 0xff,0xd7,0x9a,0xff }, /*    17 */
  {   -92,    42,     3,0, 0x0000,0x0000, 0xff,0xd7,0x9a,0xff }, /*    18 */
  {   -77,    44,    -8,0, 0x0000,0x0000, 0xff,0xf3,0xaf,0xff }, /*    19 */
  {   -92,    42,   -19,0, 0x0000,0x0000, 0xff,0xdc,0x9e,0xff }, /*    20 */
  {   -92,    42,   -19,0, 0x0000,0x0000, 0xff,0xdc,0x9e,0xff }, /*    21 */
  {   -77,    44,    -8,0, 0x0000,0x0000, 0xff,0xf3,0xaf,0xff }, /*    22 */
  {   -74,    29,   -19,0, 0x0000,0x0000, 0xdd,0xa0,0x73,0xff }, /*    23 */
  {   -74,    29,   -19,0, 0x0000,0x0000, 0xdd,0xa0,0x73,0xff }, /*    24 */
  {   -77,    44,    -8,0, 0x0000,0x0000, 0xff,0xf3,0xaf,0xff }, /*    25 */
  {   -74,    29,     3,0, 0x0000,0x0000, 0xfd,0xb7,0x84,0xff }, /*    26 */
  {  -110,    15,   -15,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*    27 */
  {  -110,    15,    -1,0, 0x0000,0x0000, 0x90,0x68,0x4b,0xff }, /*    28 */
  {   -92,    42,     3,0, 0x0000,0x0000, 0xff,0xd7,0x9a,0xff }, /*    29 */
  {   -94,     3,   -15,0, 0x0000,0x0000, 0xf7,0xb3,0x80,0xff }, /*    30 */
  {   -74,    29,   -19,0, 0x0000,0x0000, 0xdd,0xa0,0x73,0xff }, /*    31 */
  {   -74,    29,     3,0, 0x0000,0x0000, 0xfd,0xb7,0x84,0xff }, /*    32 */
  {  -110,    15,   -15,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*    33 */
  {   -92,    42,   -19,0, 0x0000,0x0000, 0xff,0xdc,0x9e,0xff }, /*    34 */
  {   -74,    29,   -19,0, 0x0000,0x0000, 0xdd,0xa0,0x73,0xff }, /*    35 */
  {   -94,     3,   -15,0, 0x0000,0x0000, 0xf7,0xb3,0x80,0xff }, /*    36 */
  {  -107,     2,    -8,0, 0x0000,0x0000, 0xb3,0x81,0x5d,0xff }, /*    37 */
  {  -110,    15,   -15,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*    38 */
  {  -110,    15,   -15,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*    39 */
  {  -107,     2,    -8,0, 0x0000,0x0000, 0xb3,0x81,0x5d,0xff }, /*    40 */
  {  -110,    15,    -1,0, 0x0000,0x0000, 0x90,0x68,0x4b,0xff }, /*    41 */
  {  -110,    15,    -1,0, 0x0000,0x0000, 0x90,0x68,0x4b,0xff }, /*    42 */
  {  -107,     2,    -8,0, 0x0000,0x0000, 0xb3,0x81,0x5d,0xff }, /*    43 */
  {   -94,     3,    -1,0, 0x0000,0x0000, 0xac,0x7c,0x59,0xff }, /*    44 */
  {   -94,     3,    -1,0, 0x0000,0x0000, 0xac,0x7c,0x59,0xff }, /*    45 */
  {  -107,     2,    -8,0, 0x0000,0x0000, 0xb3,0x81,0x5d,0xff }, /*    46 */
  {   -94,     3,   -15,0, 0x0000,0x0000, 0xf7,0xb3,0x80,0xff }, /*    47 */
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
  bmp_frake_down,
  tlut_frake_down,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_down_aligner4[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_318_[] = {
  {  -127,    -9,     2,0, 0x0000,0x0000, 0xad,0x7d,0x5a,0xff }, /*     0 */
  {  -103,    -5,     3,0, 0x0000,0x0000, 0xff,0xd6,0x99,0xff }, /*     1 */
  {  -116,     3,     3,0, 0x0000,0x0000, 0xff,0xf4,0xaf,0xff }, /*     2 */
  {  -127,    -9,     2,0, 0x0000,0x0000, 0xad,0x7d,0x5a,0xff }, /*     3 */
  {  -113,   -19,     2,0, 0x0000,0x0000, 0xa1,0x74,0x54,0xff }, /*     4 */
  {  -103,    -5,     3,0, 0x0000,0x0000, 0xff,0xd6,0x99,0xff }, /*     5 */
  {  -113,   -19,   -24,0, 0x0000,0x0000, 0xee,0xac,0x7c,0xff }, /*     6 */
  {  -127,    -9,   -24,0, 0x0000,0x0000, 0xff,0xc0,0x8a,0xff }, /*     7 */
  {  -103,    -5,   -19,0, 0x0000,0x0000, 0xe2,0xa4,0x76,0xff }, /*     8 */
  {  -113,   -19,     2,0, 0x0000,0x0000, 0xa1,0x74,0x54,0xff }, /*     9 */
  {  -113,   -19,   -24,0, 0x0000,0x0000, 0xee,0xac,0x7c,0xff }, /*    10 */
  {  -103,    -5,     3,0, 0x0000,0x0000, 0xff,0xd6,0x99,0xff }, /*    11 */
  {  -127,    -9,   -24,0, 0x0000,0x0000, 0xff,0xc0,0x8a,0xff }, /*    12 */
  {  -116,     3,     3,0, 0x0000,0x0000, 0xff,0xf4,0xaf,0xff }, /*    13 */
  {  -116,     3,   -19,0, 0x0000,0x0000, 0xff,0xd6,0x99,0xff }, /*    14 */
  {  -127,    -9,   -24,0, 0x0000,0x0000, 0xff,0xc0,0x8a,0xff }, /*    15 */
  {  -127,    -9,     2,0, 0x0000,0x0000, 0xad,0x7d,0x5a,0xff }, /*    16 */
  {  -116,     3,     3,0, 0x0000,0x0000, 0xff,0xf4,0xaf,0xff }, /*    17 */
  {  -103,    -5,     3,0, 0x0000,0x0000, 0xff,0xd6,0x99,0xff }, /*    18 */
  {  -103,     7,    -8,0, 0x0000,0x0000, 0xff,0xf3,0xae,0xff }, /*    19 */
  {  -116,     3,     3,0, 0x0000,0x0000, 0xff,0xf4,0xaf,0xff }, /*    20 */
  {  -116,     3,     3,0, 0x0000,0x0000, 0xff,0xf4,0xaf,0xff }, /*    21 */
  {  -103,     7,    -8,0, 0x0000,0x0000, 0xff,0xf3,0xae,0xff }, /*    22 */
  {  -116,     3,   -19,0, 0x0000,0x0000, 0xff,0xd6,0x99,0xff }, /*    23 */
  {  -116,     3,   -19,0, 0x0000,0x0000, 0xff,0xd6,0x99,0xff }, /*    24 */
  {  -103,     7,    -8,0, 0x0000,0x0000, 0xff,0xf3,0xae,0xff }, /*    25 */
  {  -103,    -5,   -19,0, 0x0000,0x0000, 0xe2,0xa4,0x76,0xff }, /*    26 */
  {  -103,    -5,   -19,0, 0x0000,0x0000, 0xe2,0xa4,0x76,0xff }, /*    27 */
  {  -103,     7,    -8,0, 0x0000,0x0000, 0xff,0xf3,0xae,0xff }, /*    28 */
  {  -103,    -5,     3,0, 0x0000,0x0000, 0xff,0xd6,0x99,0xff }, /*    29 */
  {  -113,   -19,   -24,0, 0x0000,0x0000, 0xee,0xac,0x7c,0xff }, /*    30 */
  {  -103,    -5,   -19,0, 0x0000,0x0000, 0xe2,0xa4,0x76,0xff }, /*    31 */
  {  -103,    -5,     3,0, 0x0000,0x0000, 0xff,0xd6,0x99,0xff }, /*    32 */
  {  -127,    -9,   -24,0, 0x0000,0x0000, 0xff,0xc0,0x8a,0xff }, /*    33 */
  {  -116,     3,   -19,0, 0x0000,0x0000, 0xff,0xd6,0x99,0xff }, /*    34 */
  {  -103,    -5,   -19,0, 0x0000,0x0000, 0xe2,0xa4,0x76,0xff }, /*    35 */
  {  -113,   -19,   -24,0, 0x0000,0x0000, 0xee,0xac,0x7c,0xff }, /*    36 */
  {  -124,   -20,    -8,0, 0x0000,0x0000, 0x91,0x69,0x4b,0xff }, /*    37 */
  {  -127,    -9,   -24,0, 0x0000,0x0000, 0xff,0xc0,0x8a,0xff }, /*    38 */
  {  -127,    -9,   -24,0, 0x0000,0x0000, 0xff,0xc0,0x8a,0xff }, /*    39 */
  {  -124,   -20,    -8,0, 0x0000,0x0000, 0x91,0x69,0x4b,0xff }, /*    40 */
  {  -127,    -9,     2,0, 0x0000,0x0000, 0xad,0x7d,0x5a,0xff }, /*    41 */
  {  -127,    -9,     2,0, 0x0000,0x0000, 0xad,0x7d,0x5a,0xff }, /*    42 */
  {  -124,   -20,    -8,0, 0x0000,0x0000, 0x91,0x69,0x4b,0xff }, /*    43 */
  {  -113,   -19,     2,0, 0x0000,0x0000, 0xa1,0x74,0x54,0xff }, /*    44 */
  {  -113,   -19,     2,0, 0x0000,0x0000, 0xa1,0x74,0x54,0xff }, /*    45 */
  {  -124,   -20,    -8,0, 0x0000,0x0000, 0x91,0x69,0x4b,0xff }, /*    46 */
  {  -113,   -19,   -24,0, 0x0000,0x0000, 0xee,0xac,0x7c,0xff }, /*    47 */
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
  gsSP1Triangle(  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsSP2Triangles( 12, 13, 14,  0, 15, 16, 17,  0),
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
static NDR_SHAPE shape_Shape_of___POLYHEDRON_318_ = {
  gfx_Shape_of___POLYHEDRON_318_,
  vtx_Shape_of___POLYHEDRON_318_, 48, 5,
  bmp_frake_down,
  tlut_frake_down,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_down_aligner5[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_312_[] = {
  {   -81,    69,     9,0, 0x0000,0x0000, 0x59,0x94,0x94,0xff }, /*     0 */
  {   -34,    80,     5,0, 0x0000,0x0000, 0x8c,0xe9,0xe9,0xff }, /*     1 */
  {   -57,    97,     5,0, 0x0000,0x0000, 0xa7,0xff,0xff,0xff }, /*     2 */
  {   -81,    69,     9,0, 0x0000,0x0000, 0x59,0x94,0x94,0xff }, /*     3 */
  {   -51,    48,     9,0, 0x0000,0x0000, 0x6b,0xb3,0xb3,0xff }, /*     4 */
  {   -34,    80,     5,0, 0x0000,0x0000, 0x8c,0xe9,0xe9,0xff }, /*     5 */
  {   -51,    48,   -26,0, 0x0000,0x0000, 0x99,0xff,0xff,0xff }, /*     6 */
  {   -81,    69,   -26,0, 0x0000,0x0000, 0xa9,0xff,0xff,0xff }, /*     7 */
  {   -34,    80,   -22,0, 0x0000,0x0000, 0x96,0xfa,0xfa,0xff }, /*     8 */
  {   -51,    48,     9,0, 0x0000,0x0000, 0x6b,0xb3,0xb3,0xff }, /*     9 */
  {   -51,    48,   -26,0, 0x0000,0x0000, 0x99,0xff,0xff,0xff }, /*    10 */
  {   -34,    80,     5,0, 0x0000,0x0000, 0x8c,0xe9,0xe9,0xff }, /*    11 */
  {   -81,    69,   -26,0, 0x0000,0x0000, 0xa9,0xff,0xff,0xff }, /*    12 */
  {   -57,    97,     5,0, 0x0000,0x0000, 0xa7,0xff,0xff,0xff }, /*    13 */
  {   -57,    97,   -22,0, 0x0000,0x0000, 0xcf,0xff,0xff,0xff }, /*    14 */
  {   -34,    80,     5,0, 0x0000,0x0000, 0x8c,0xe9,0xe9,0xff }, /*    15 */
  {   -38,    99,    -8,0, 0x0000,0x0000, 0xba,0xff,0xff,0xff }, /*    16 */
  {   -57,    97,     5,0, 0x0000,0x0000, 0xa7,0xff,0xff,0xff }, /*    17 */
  {   -81,    69,   -26,0, 0x0000,0x0000, 0xa9,0xff,0xff,0xff }, /*    18 */
  {   -81,    69,     9,0, 0x0000,0x0000, 0x59,0x94,0x94,0xff }, /*    19 */
  {   -57,    97,     5,0, 0x0000,0x0000, 0xa7,0xff,0xff,0xff }, /*    20 */
  {   -57,    97,     5,0, 0x0000,0x0000, 0xa7,0xff,0xff,0xff }, /*    21 */
  {   -38,    99,    -8,0, 0x0000,0x0000, 0xba,0xff,0xff,0xff }, /*    22 */
  {   -57,    97,   -22,0, 0x0000,0x0000, 0xcf,0xff,0xff,0xff }, /*    23 */
  {   -57,    97,   -22,0, 0x0000,0x0000, 0xcf,0xff,0xff,0xff }, /*    24 */
  {   -38,    99,    -8,0, 0x0000,0x0000, 0xba,0xff,0xff,0xff }, /*    25 */
  {   -34,    80,   -22,0, 0x0000,0x0000, 0x96,0xfa,0xfa,0xff }, /*    26 */
  {   -34,    80,   -22,0, 0x0000,0x0000, 0x96,0xfa,0xfa,0xff }, /*    27 */
  {   -38,    99,    -8,0, 0x0000,0x0000, 0xba,0xff,0xff,0xff }, /*    28 */
  {   -34,    80,     5,0, 0x0000,0x0000, 0x8c,0xe9,0xe9,0xff }, /*    29 */
  {   -51,    48,   -26,0, 0x0000,0x0000, 0x99,0xff,0xff,0xff }, /*    30 */
  {   -34,    80,   -22,0, 0x0000,0x0000, 0x96,0xfa,0xfa,0xff }, /*    31 */
  {   -34,    80,     5,0, 0x0000,0x0000, 0x8c,0xe9,0xe9,0xff }, /*    32 */
  {   -81,    69,   -26,0, 0x0000,0x0000, 0xa9,0xff,0xff,0xff }, /*    33 */
  {   -57,    97,   -22,0, 0x0000,0x0000, 0xcf,0xff,0xff,0xff }, /*    34 */
  {   -34,    80,   -22,0, 0x0000,0x0000, 0x96,0xfa,0xfa,0xff }, /*    35 */
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
static NDR_SHAPE shape_Shape_of___POLYHEDRON_312_ = {
  gfx_Shape_of___POLYHEDRON_312_,
  vtx_Shape_of___POLYHEDRON_312_, 36, 5,
  bmp_frake_down,
  tlut_frake_down,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_down_aligner6[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_306_[] = {
  {    75,    71,     5,0, 0x0000,0x0000, 0xa1,0x75,0x54,0xff }, /*     0 */
  {    52,    54,     5,0, 0x0000,0x0000, 0xc9,0x91,0x68,0xff }, /*     1 */
  {    92,    45,     4,0, 0x0000,0x0000, 0x88,0x62,0x47,0xff }, /*     2 */
  {    52,    54,     5,0, 0x0000,0x0000, 0xc9,0x91,0x68,0xff }, /*     3 */
  {    71,    30,     4,0, 0x0000,0x0000, 0xfd,0xb7,0x83,0xff }, /*     4 */
  {    92,    45,     4,0, 0x0000,0x0000, 0x88,0x62,0x47,0xff }, /*     5 */
  {    52,    54,   -22,0, 0x0000,0x0000, 0xe6,0xa6,0x78,0xff }, /*     6 */
  {    92,    45,   -20,0, 0x0000,0x0000, 0xff,0xcc,0x93,0xff }, /*     7 */
  {    71,    30,   -20,0, 0x0000,0x0000, 0xff,0xd1,0x96,0xff }, /*     8 */
  {    52,    54,     5,0, 0x0000,0x0000, 0xc9,0x91,0x68,0xff }, /*     9 */
  {    71,    30,   -20,0, 0x0000,0x0000, 0xff,0xd1,0x96,0xff }, /*    10 */
  {    71,    30,     4,0, 0x0000,0x0000, 0xfd,0xb7,0x83,0xff }, /*    11 */
  {    52,    54,     5,0, 0x0000,0x0000, 0xc9,0x91,0x68,0xff }, /*    12 */
  {    52,    54,   -22,0, 0x0000,0x0000, 0xe6,0xa6,0x78,0xff }, /*    13 */
  {    71,    30,   -20,0, 0x0000,0x0000, 0xff,0xd1,0x96,0xff }, /*    14 */
  {    75,    71,   -22,0, 0x0000,0x0000, 0xff,0xc8,0x90,0xff }, /*    15 */
  {    75,    71,     5,0, 0x0000,0x0000, 0xa1,0x75,0x54,0xff }, /*    16 */
  {    92,    45,   -20,0, 0x0000,0x0000, 0xff,0xcc,0x93,0xff }, /*    17 */
  {    75,    71,     5,0, 0x0000,0x0000, 0xa1,0x75,0x54,0xff }, /*    18 */
  {    56,    73,    -8,0, 0x0000,0x0000, 0xff,0xc1,0x8b,0xff }, /*    19 */
  {    52,    54,     5,0, 0x0000,0x0000, 0xc9,0x91,0x68,0xff }, /*    20 */
  {    75,    71,   -22,0, 0x0000,0x0000, 0xff,0xc8,0x90,0xff }, /*    21 */
  {    56,    73,    -8,0, 0x0000,0x0000, 0xff,0xc1,0x8b,0xff }, /*    22 */
  {    75,    71,     5,0, 0x0000,0x0000, 0xa1,0x75,0x54,0xff }, /*    23 */
  {    52,    54,   -22,0, 0x0000,0x0000, 0xe6,0xa6,0x78,0xff }, /*    24 */
  {    56,    73,    -8,0, 0x0000,0x0000, 0xff,0xc1,0x8b,0xff }, /*    25 */
  {    75,    71,   -22,0, 0x0000,0x0000, 0xff,0xc8,0x90,0xff }, /*    26 */
  {    52,    54,     5,0, 0x0000,0x0000, 0xc9,0x91,0x68,0xff }, /*    27 */
  {    56,    73,    -8,0, 0x0000,0x0000, 0xff,0xc1,0x8b,0xff }, /*    28 */
  {    52,    54,   -22,0, 0x0000,0x0000, 0xe6,0xa6,0x78,0xff }, /*    29 */
  {    75,    71,     5,0, 0x0000,0x0000, 0xa1,0x75,0x54,0xff }, /*    30 */
  {    92,    45,     4,0, 0x0000,0x0000, 0x88,0x62,0x47,0xff }, /*    31 */
  {    92,    45,   -20,0, 0x0000,0x0000, 0xff,0xcc,0x93,0xff }, /*    32 */
  {    52,    54,   -22,0, 0x0000,0x0000, 0xe6,0xa6,0x78,0xff }, /*    33 */
  {    75,    71,   -22,0, 0x0000,0x0000, 0xff,0xc8,0x90,0xff }, /*    34 */
  {    92,    45,   -20,0, 0x0000,0x0000, 0xff,0xcc,0x93,0xff }, /*    35 */
  {    92,    45,   -20,0, 0x0000,0x0000, 0xff,0xcc,0x93,0xff }, /*    36 */
  {    88,    29,    -8,0, 0x0000,0x0000, 0xff,0x83,0x5e,0xff }, /*    37 */
  {    71,    30,   -20,0, 0x0000,0x0000, 0xff,0xd1,0x96,0xff }, /*    38 */
  {    92,    45,     4,0, 0x0000,0x0000, 0x88,0x62,0x47,0xff }, /*    39 */
  {    88,    29,    -8,0, 0x0000,0x0000, 0xff,0x83,0x5e,0xff }, /*    40 */
  {    92,    45,   -20,0, 0x0000,0x0000, 0xff,0xcc,0x93,0xff }, /*    41 */
  {    71,    30,     4,0, 0x0000,0x0000, 0xfd,0xb7,0x83,0xff }, /*    42 */
  {    88,    29,    -8,0, 0x0000,0x0000, 0xff,0x83,0x5e,0xff }, /*    43 */
  {    92,    45,     4,0, 0x0000,0x0000, 0x88,0x62,0x47,0xff }, /*    44 */
  {    71,    30,   -20,0, 0x0000,0x0000, 0xff,0xd1,0x96,0xff }, /*    45 */
  {    88,    29,    -8,0, 0x0000,0x0000, 0xff,0x83,0x5e,0xff }, /*    46 */
  {    71,    30,     4,0, 0x0000,0x0000, 0xfd,0xb7,0x83,0xff }, /*    47 */
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
  gsSP2Triangles(  9, 10, 11,  0, 12, 13, 14,  0),
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
static NDR_SHAPE shape_Shape_of___POLYHEDRON_306_ = {
  gfx_Shape_of___POLYHEDRON_306_,
  vtx_Shape_of___POLYHEDRON_306_, 48, 5,
  bmp_frake_down,
  tlut_frake_down,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_down_aligner7[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_308_[] = {
  {    92,    42,     3,0, 0x0000,0x0000, 0xaa,0x7b,0x58,0xff }, /*     0 */
  {    74,    29,     3,0, 0x0000,0x0000, 0xdf,0xa1,0x74,0xff }, /*     1 */
  {   110,    15,    -1,0, 0x0000,0x0000, 0x78,0x56,0x3e,0xff }, /*     2 */
  {    74,    29,     3,0, 0x0000,0x0000, 0xdf,0xa1,0x74,0xff }, /*     3 */
  {    94,     3,    -1,0, 0x0000,0x0000, 0xec,0xab,0x7b,0xff }, /*     4 */
  {   110,    15,    -1,0, 0x0000,0x0000, 0x78,0x56,0x3e,0xff }, /*     5 */
  {    74,    29,   -19,0, 0x0000,0x0000, 0xff,0xba,0x86,0xff }, /*     6 */
  {   110,    15,   -15,0, 0x0000,0x0000, 0xff,0xc8,0x8f,0xff }, /*     7 */
  {    94,     3,   -15,0, 0x0000,0x0000, 0xff,0xca,0x91,0xff }, /*     8 */
  {    74,    29,     3,0, 0x0000,0x0000, 0xdf,0xa1,0x74,0xff }, /*     9 */
  {    94,     3,   -15,0, 0x0000,0x0000, 0xff,0xca,0x91,0xff }, /*    10 */
  {    94,     3,    -1,0, 0x0000,0x0000, 0xec,0xab,0x7b,0xff }, /*    11 */
  {    74,    29,     3,0, 0x0000,0x0000, 0xdf,0xa1,0x74,0xff }, /*    12 */
  {    74,    29,   -19,0, 0x0000,0x0000, 0xff,0xba,0x86,0xff }, /*    13 */
  {    94,     3,   -15,0, 0x0000,0x0000, 0xff,0xca,0x91,0xff }, /*    14 */
  {    92,    42,   -19,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    15 */
  {    92,    42,     3,0, 0x0000,0x0000, 0xaa,0x7b,0x58,0xff }, /*    16 */
  {   110,    15,   -15,0, 0x0000,0x0000, 0xff,0xc8,0x8f,0xff }, /*    17 */
  {    92,    42,     3,0, 0x0000,0x0000, 0xaa,0x7b,0x58,0xff }, /*    18 */
  {    77,    44,    -8,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    19 */
  {    74,    29,     3,0, 0x0000,0x0000, 0xdf,0xa1,0x74,0xff }, /*    20 */
  {    92,    42,   -19,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    21 */
  {    77,    44,    -8,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    22 */
  {    92,    42,     3,0, 0x0000,0x0000, 0xaa,0x7b,0x58,0xff }, /*    23 */
  {    92,    42,     3,0, 0x0000,0x0000, 0xaa,0x7b,0x58,0xff }, /*    24 */
  {   110,    15,    -1,0, 0x0000,0x0000, 0x78,0x56,0x3e,0xff }, /*    25 */
  {   110,    15,   -15,0, 0x0000,0x0000, 0xff,0xc8,0x8f,0xff }, /*    26 */
  {    74,    29,   -19,0, 0x0000,0x0000, 0xff,0xba,0x86,0xff }, /*    27 */
  {    77,    44,    -8,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    28 */
  {    92,    42,   -19,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    29 */
  {    74,    29,     3,0, 0x0000,0x0000, 0xdf,0xa1,0x74,0xff }, /*    30 */
  {    77,    44,    -8,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    31 */
  {    74,    29,   -19,0, 0x0000,0x0000, 0xff,0xba,0x86,0xff }, /*    32 */
  {    74,    29,   -19,0, 0x0000,0x0000, 0xff,0xba,0x86,0xff }, /*    33 */
  {    92,    42,   -19,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    34 */
  {   110,    15,   -15,0, 0x0000,0x0000, 0xff,0xc8,0x8f,0xff }, /*    35 */
  {   110,    15,   -15,0, 0x0000,0x0000, 0xff,0xc8,0x8f,0xff }, /*    36 */
  {   107,     2,    -8,0, 0x0000,0x0000, 0xff,0xc0,0x8a,0xff }, /*    37 */
  {    94,     3,   -15,0, 0x0000,0x0000, 0xff,0xca,0x91,0xff }, /*    38 */
  {   110,    15,    -1,0, 0x0000,0x0000, 0x78,0x56,0x3e,0xff }, /*    39 */
  {   107,     2,    -8,0, 0x0000,0x0000, 0xff,0xc0,0x8a,0xff }, /*    40 */
  {   110,    15,   -15,0, 0x0000,0x0000, 0xff,0xc8,0x8f,0xff }, /*    41 */
  {    94,     3,    -1,0, 0x0000,0x0000, 0xec,0xab,0x7b,0xff }, /*    42 */
  {   107,     2,    -8,0, 0x0000,0x0000, 0xff,0xc0,0x8a,0xff }, /*    43 */
  {   110,    15,    -1,0, 0x0000,0x0000, 0x78,0x56,0x3e,0xff }, /*    44 */
  {    94,     3,   -15,0, 0x0000,0x0000, 0xff,0xca,0x91,0xff }, /*    45 */
  {   107,     2,    -8,0, 0x0000,0x0000, 0xff,0xc0,0x8a,0xff }, /*    46 */
  {    94,     3,    -1,0, 0x0000,0x0000, 0xec,0xab,0x7b,0xff }, /*    47 */
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
  gsSP1Triangle(  6,  7,  8,  0),
  gsSP2Triangles(  9, 10, 11,  0, 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
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
static NDR_SHAPE shape_Shape_of___POLYHEDRON_308_ = {
  gfx_Shape_of___POLYHEDRON_308_,
  vtx_Shape_of___POLYHEDRON_308_, 48, 5,
  bmp_frake_down,
  tlut_frake_down,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_down_aligner8[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_310_[] = {
  {   116,     3,     3,0, 0x0000,0x0000, 0xc2,0x8c,0x65,0xff }, /*     0 */
  {   103,    -5,     3,0, 0x0000,0x0000, 0xef,0xad,0x7c,0xff }, /*     1 */
  {   127,    -9,     2,0, 0x0000,0x0000, 0x96,0x6d,0x4e,0xff }, /*     2 */
  {   103,    -5,     3,0, 0x0000,0x0000, 0xef,0xad,0x7c,0xff }, /*     3 */
  {   113,   -19,     2,0, 0x0000,0x0000, 0xce,0x95,0x6b,0xff }, /*     4 */
  {   127,    -9,     2,0, 0x0000,0x0000, 0x96,0x6d,0x4e,0xff }, /*     5 */
  {   103,    -5,   -19,0, 0x0000,0x0000, 0xff,0xcc,0x93,0xff }, /*     6 */
  {   127,    -9,   -24,0, 0x0000,0x0000, 0xff,0xbb,0x87,0xff }, /*     7 */
  {   113,   -19,   -24,0, 0x0000,0x0000, 0xff,0xc2,0x8b,0xff }, /*     8 */
  {   103,    -5,     3,0, 0x0000,0x0000, 0xef,0xad,0x7c,0xff }, /*     9 */
  {   113,   -19,   -24,0, 0x0000,0x0000, 0xff,0xc2,0x8b,0xff }, /*    10 */
  {   113,   -19,     2,0, 0x0000,0x0000, 0xce,0x95,0x6b,0xff }, /*    11 */
  {   103,    -5,     3,0, 0x0000,0x0000, 0xef,0xad,0x7c,0xff }, /*    12 */
  {   103,    -5,   -19,0, 0x0000,0x0000, 0xff,0xcc,0x93,0xff }, /*    13 */
  {   113,   -19,   -24,0, 0x0000,0x0000, 0xff,0xc2,0x8b,0xff }, /*    14 */
  {   116,     3,   -19,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*    15 */
  {   116,     3,     3,0, 0x0000,0x0000, 0xc2,0x8c,0x65,0xff }, /*    16 */
  {   127,    -9,   -24,0, 0x0000,0x0000, 0xff,0xbb,0x87,0xff }, /*    17 */
  {   116,     3,     3,0, 0x0000,0x0000, 0xc2,0x8c,0x65,0xff }, /*    18 */
  {   103,     7,    -8,0, 0x0000,0x0000, 0xff,0xbf,0x89,0xff }, /*    19 */
  {   103,    -5,     3,0, 0x0000,0x0000, 0xef,0xad,0x7c,0xff }, /*    20 */
  {   116,     3,   -19,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*    21 */
  {   103,     7,    -8,0, 0x0000,0x0000, 0xff,0xbf,0x89,0xff }, /*    22 */
  {   116,     3,     3,0, 0x0000,0x0000, 0xc2,0x8c,0x65,0xff }, /*    23 */
  {   103,    -5,   -19,0, 0x0000,0x0000, 0xff,0xcc,0x93,0xff }, /*    24 */
  {   103,     7,    -8,0, 0x0000,0x0000, 0xff,0xbf,0x89,0xff }, /*    25 */
  {   116,     3,   -19,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*    26 */
  {   103,    -5,     3,0, 0x0000,0x0000, 0xef,0xad,0x7c,0xff }, /*    27 */
  {   103,     7,    -8,0, 0x0000,0x0000, 0xff,0xbf,0x89,0xff }, /*    28 */
  {   103,    -5,   -19,0, 0x0000,0x0000, 0xff,0xcc,0x93,0xff }, /*    29 */
  {   116,     3,     3,0, 0x0000,0x0000, 0xc2,0x8c,0x65,0xff }, /*    30 */
  {   127,    -9,     2,0, 0x0000,0x0000, 0x96,0x6d,0x4e,0xff }, /*    31 */
  {   127,    -9,   -24,0, 0x0000,0x0000, 0xff,0xbb,0x87,0xff }, /*    32 */
  {   103,    -5,   -19,0, 0x0000,0x0000, 0xff,0xcc,0x93,0xff }, /*    33 */
  {   116,     3,   -19,0, 0x0000,0x0000, 0xff,0xc5,0x8e,0xff }, /*    34 */
  {   127,    -9,   -24,0, 0x0000,0x0000, 0xff,0xbb,0x87,0xff }, /*    35 */
  {   127,    -9,   -24,0, 0x0000,0x0000, 0xff,0xbb,0x87,0xff }, /*    36 */
  {   124,   -20,    -8,0, 0x0000,0x0000, 0xc5,0x8e,0x66,0xff }, /*    37 */
  {   113,   -19,   -24,0, 0x0000,0x0000, 0xff,0xc2,0x8b,0xff }, /*    38 */
  {   127,    -9,     2,0, 0x0000,0x0000, 0x96,0x6d,0x4e,0xff }, /*    39 */
  {   124,   -20,    -8,0, 0x0000,0x0000, 0xc5,0x8e,0x66,0xff }, /*    40 */
  {   127,    -9,   -24,0, 0x0000,0x0000, 0xff,0xbb,0x87,0xff }, /*    41 */
  {   113,   -19,     2,0, 0x0000,0x0000, 0xce,0x95,0x6b,0xff }, /*    42 */
  {   124,   -20,    -8,0, 0x0000,0x0000, 0xc5,0x8e,0x66,0xff }, /*    43 */
  {   127,    -9,     2,0, 0x0000,0x0000, 0x96,0x6d,0x4e,0xff }, /*    44 */
  {   113,   -19,   -24,0, 0x0000,0x0000, 0xff,0xc2,0x8b,0xff }, /*    45 */
  {   124,   -20,    -8,0, 0x0000,0x0000, 0xc5,0x8e,0x66,0xff }, /*    46 */
  {   113,   -19,     2,0, 0x0000,0x0000, 0xce,0x95,0x6b,0xff }, /*    47 */
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
  gsSP1Triangle(  6,  7,  8,  0),
  gsSP2Triangles(  9, 10, 11,  0, 12, 13, 14,  0),
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
static NDR_SHAPE shape_Shape_of___POLYHEDRON_310_ = {
  gfx_Shape_of___POLYHEDRON_310_,
  vtx_Shape_of___POLYHEDRON_310_, 48, 5,
  bmp_frake_down,
  tlut_frake_down,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_down_aligner9[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_304_[] = {
  {    57,    97,     5,0, 0x0000,0x0000, 0x66,0xab,0xab,0xff }, /*     0 */
  {    34,    80,     5,0, 0x0000,0x0000, 0x7f,0xd4,0xd4,0xff }, /*     1 */
  {    81,    69,     9,0, 0x0000,0x0000, 0x5d,0x9c,0x9c,0xff }, /*     2 */
  {    34,    80,     5,0, 0x0000,0x0000, 0x7f,0xd4,0xd4,0xff }, /*     3 */
  {    51,    48,     9,0, 0x0000,0x0000, 0xa0,0xff,0xff,0xff }, /*     4 */
  {    81,    69,     9,0, 0x0000,0x0000, 0x5d,0x9c,0x9c,0xff }, /*     5 */
  {    34,    80,   -22,0, 0x0000,0x0000, 0x8a,0xe6,0xe6,0xff }, /*     6 */
  {    81,    69,   -26,0, 0x0000,0x0000, 0xb4,0xff,0xff,0xff }, /*     7 */
  {    51,    48,   -26,0, 0x0000,0x0000, 0xb4,0xff,0xff,0xff }, /*     8 */
  {    34,    80,     5,0, 0x0000,0x0000, 0x7f,0xd4,0xd4,0xff }, /*     9 */
  {    51,    48,   -26,0, 0x0000,0x0000, 0xb4,0xff,0xff,0xff }, /*    10 */
  {    51,    48,     9,0, 0x0000,0x0000, 0xa0,0xff,0xff,0xff }, /*    11 */
  {    57,    97,   -22,0, 0x0000,0x0000, 0xac,0xff,0xff,0xff }, /*    12 */
  {    57,    97,     5,0, 0x0000,0x0000, 0x66,0xab,0xab,0xff }, /*    13 */
  {    81,    69,   -26,0, 0x0000,0x0000, 0xb4,0xff,0xff,0xff }, /*    14 */
  {    57,    97,     5,0, 0x0000,0x0000, 0x66,0xab,0xab,0xff }, /*    15 */
  {    38,    99,    -8,0, 0x0000,0x0000, 0xa3,0xff,0xff,0xff }, /*    16 */
  {    34,    80,     5,0, 0x0000,0x0000, 0x7f,0xd4,0xd4,0xff }, /*    17 */
  {    57,    97,   -22,0, 0x0000,0x0000, 0xac,0xff,0xff,0xff }, /*    18 */
  {    38,    99,    -8,0, 0x0000,0x0000, 0xa3,0xff,0xff,0xff }, /*    19 */
  {    57,    97,     5,0, 0x0000,0x0000, 0x66,0xab,0xab,0xff }, /*    20 */
  {    34,    80,   -22,0, 0x0000,0x0000, 0x8a,0xe6,0xe6,0xff }, /*    21 */
  {    38,    99,    -8,0, 0x0000,0x0000, 0xa3,0xff,0xff,0xff }, /*    22 */
  {    57,    97,   -22,0, 0x0000,0x0000, 0xac,0xff,0xff,0xff }, /*    23 */
  {    34,    80,     5,0, 0x0000,0x0000, 0x7f,0xd4,0xd4,0xff }, /*    24 */
  {    38,    99,    -8,0, 0x0000,0x0000, 0xa3,0xff,0xff,0xff }, /*    25 */
  {    34,    80,   -22,0, 0x0000,0x0000, 0x8a,0xe6,0xe6,0xff }, /*    26 */
  {    57,    97,     5,0, 0x0000,0x0000, 0x66,0xab,0xab,0xff }, /*    27 */
  {    81,    69,     9,0, 0x0000,0x0000, 0x5d,0x9c,0x9c,0xff }, /*    28 */
  {    81,    69,   -26,0, 0x0000,0x0000, 0xb4,0xff,0xff,0xff }, /*    29 */
  {    34,    80,     5,0, 0x0000,0x0000, 0x7f,0xd4,0xd4,0xff }, /*    30 */
  {    34,    80,   -22,0, 0x0000,0x0000, 0x8a,0xe6,0xe6,0xff }, /*    31 */
  {    51,    48,   -26,0, 0x0000,0x0000, 0xb4,0xff,0xff,0xff }, /*    32 */
  {    34,    80,   -22,0, 0x0000,0x0000, 0x8a,0xe6,0xe6,0xff }, /*    33 */
  {    57,    97,   -22,0, 0x0000,0x0000, 0xac,0xff,0xff,0xff }, /*    34 */
  {    81,    69,   -26,0, 0x0000,0x0000, 0xb4,0xff,0xff,0xff }, /*    35 */
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
static NDR_SHAPE shape_Shape_of___POLYHEDRON_304_ = {
  gfx_Shape_of___POLYHEDRON_304_,
  vtx_Shape_of___POLYHEDRON_304_, 36, 5,
  bmp_frake_down,
  tlut_frake_down,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_down_aligner10[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_264_[] = {
  {    57,   188,     0,0, 0x0000,0x0000, 0x00,0x4d,0x9b,0xff }, /*     0 */
  {    36,   132,   -52,0, 0x0000,0x0000, 0x00,0x4c,0x98,0xff }, /*     1 */
  {    51,   178,   -65,0, 0x0000,0x0000, 0x00,0x4a,0x95,0xff }, /*     2 */
  {    51,   178,   -65,0, 0x0000,0x0000, 0x00,0x4a,0x95,0xff }, /*     3 */
  {     0,   132,   -74,0, 0x0000,0x0000, 0x00,0x5c,0xb8,0xff }, /*     4 */
  {     0,   168,   -91,0, 0x0000,0x0000, 0x00,0x4d,0x9a,0xff }, /*     5 */
  {     0,   168,   -91,0, 0x0000,0x0000, 0x00,0x4d,0x9a,0xff }, /*     6 */
  {     0,   132,   -74,0, 0x0000,0x0000, 0x00,0x5c,0xb8,0xff }, /*     7 */
  {   -51,   178,   -65,0, 0x0000,0x0000, 0x00,0x51,0xa2,0xff }, /*     8 */
  {   -51,   178,   -65,0, 0x0000,0x0000, 0x00,0x51,0xa2,0xff }, /*     9 */
  {   -36,   132,   -52,0, 0x0000,0x0000, 0x00,0x55,0xab,0xff }, /*    10 */
  {   -57,   188,     0,0, 0x0000,0x0000, 0x00,0x48,0x91,0xff }, /*    11 */
  {   -57,   188,     0,0, 0x0000,0x0000, 0xe9,0xa9,0x79,0xff }, /*    12 */
  {   -52,   132,     0,0, 0x0000,0x0000, 0xff,0xc8,0x90,0xff }, /*    13 */
  {   -36,   132,    52,0, 0x0000,0x0000, 0xdd,0xa0,0x73,0xff }, /*    14 */
  {    57,   188,     0,0, 0x0000,0x0000, 0xf9,0xb4,0x81,0xff }, /*    15 */
  {    42,   170,    65,0, 0x0000,0x0000, 0xb3,0x55,0x38,0xff }, /*    16 */
  {    36,   132,    52,0, 0x0000,0x0000, 0xc9,0x91,0x68,0xff }, /*    17 */
  {    36,   132,    52,0, 0x0000,0x0000, 0xc9,0x91,0x68,0xff }, /*    18 */
  {    18,   114,    20,0, 0x0000,0x0000, 0xd6,0x9b,0x6f,0xff }, /*    19 */
  {    15,   114,     0,0, 0x0000,0x0000, 0xcd,0x94,0x6b,0xff }, /*    20 */
  {    52,   132,     0,0, 0x0000,0x0000, 0xa5,0x77,0x56,0xff }, /*    21 */
  {    15,   114,     0,0, 0x0000,0x0000, 0xcd,0x94,0x6b,0xff }, /*    22 */
  {    36,   132,   -52,0, 0x0000,0x0000, 0xf5,0xb1,0x7f,0xff }, /*    23 */
  {    15,   114,     0,0, 0x0000,0x0000, 0xcd,0x94,0x6b,0xff }, /*    24 */
  {    18,   114,    20,0, 0x0000,0x0000, 0xd6,0x9b,0x6f,0xff }, /*    25 */
  {   -18,   114,    20,0, 0x0000,0x0000, 0xec,0xab,0x7b,0xff }, /*    26 */
  {    18,   114,    20,0, 0x0000,0x0000, 0xd6,0x9b,0x6f,0xff }, /*    27 */
  {     0,   114,    28,0, 0x0000,0x0000, 0xd1,0x97,0x6d,0xff }, /*    28 */
  {   -18,   114,    20,0, 0x0000,0x0000, 0xec,0xab,0x7b,0xff }, /*    29 */
  {    15,   114,     0,0, 0x0000,0x0000, 0xcd,0x94,0x6b,0xff }, /*    30 */
  {   -18,   114,    20,0, 0x0000,0x0000, 0xec,0xab,0x7b,0xff }, /*    31 */
  {   -15,   114,     0,0, 0x0000,0x0000, 0xe3,0xa4,0x76,0xff }, /*    32 */
  {    36,   132,   -52,0, 0x0000,0x0000, 0xf5,0xb1,0x7f,0xff }, /*    33 */
  {    11,   114,   -20,0, 0x0000,0x0000, 0xec,0xaa,0x7a,0xff }, /*    34 */
  {     0,   132,   -74,0, 0x0000,0x0000, 0xff,0xd6,0x99,0xff }, /*    35 */
  {     0,   132,   -74,0, 0x0000,0x0000, 0xff,0xd6,0x99,0xff }, /*    36 */
  {   -11,   114,   -20,0, 0x0000,0x0000, 0xff,0xd3,0x97,0xff }, /*    37 */
  {   -36,   132,   -52,0, 0x0000,0x0000, 0xff,0xc7,0x8f,0xff }, /*    38 */
  {   -36,   132,   -52,0, 0x0000,0x0000, 0xff,0xc7,0x8f,0xff }, /*    39 */
  {   -15,   114,     0,0, 0x0000,0x0000, 0xe3,0xa4,0x76,0xff }, /*    40 */
  {   -52,   132,     0,0, 0x0000,0x0000, 0xff,0xc8,0x90,0xff }, /*    41 */
  {   -52,   132,     0,0, 0x0000,0x0000, 0xff,0xc8,0x90,0xff }, /*    42 */
  {   -15,   114,     0,0, 0x0000,0x0000, 0xe3,0xa4,0x76,0xff }, /*    43 */
  {   -36,   132,    52,0, 0x0000,0x0000, 0xdd,0xa0,0x73,0xff }, /*    44 */
  {   -36,   132,    52,0, 0x0000,0x0000, 0xdd,0xa0,0x73,0xff }, /*    45 */
  {   -18,   114,    20,0, 0x0000,0x0000, 0xec,0xab,0x7b,0xff }, /*    46 */
  {     0,   114,    28,0, 0x0000,0x0000, 0xd1,0x97,0x6d,0xff }, /*    47 */
  {     0,   114,    28,0, 0x0000,0x0000, 0xd1,0x97,0x6d,0xff }, /*    48 */
  {    18,   114,    20,0, 0x0000,0x0000, 0xd6,0x9b,0x6f,0xff }, /*    49 */
  {    36,   132,    52,0, 0x0000,0x0000, 0xc9,0x91,0x68,0xff }, /*    50 */
  {    36,   132,    52,0, 0x0000,0x0000, 0xc9,0x91,0x68,0xff }, /*    51 */
  {    15,   114,     0,0, 0x0000,0x0000, 0xcd,0x94,0x6b,0xff }, /*    52 */
  {    52,   132,     0,0, 0x0000,0x0000, 0xa5,0x77,0x56,0xff }, /*    53 */
  {    53,   178,   -80,0, 0x0000,0x0000, 0x44,0x00,0x89,0xff }, /*    54 */
  {    57,   201,   -84,0, 0x0000,0x0000, 0x3f,0x00,0x7e,0xff }, /*    55 */
  {    76,   188,     0,0, 0x0000,0x0000, 0x34,0x00,0x69,0xff }, /*    56 */
  {    73,   214,     0,0, 0x0000,0x0000, 0x2f,0x00,0x5e,0xff }, /*    57 */
  {    51,   229,    48,0, 0x0000,0x0000, 0x34,0x00,0x69,0xff }, /*    58 */
  {    64,   204,    47,0, 0x0000,0x0000, 0x34,0x00,0x68,0xff }, /*    59 */
  {    64,   204,    47,0, 0x0000,0x0000, 0x34,0x00,0x68,0xff }, /*    60 */
  {    51,   229,    48,0, 0x0000,0x0000, 0x34,0x00,0x69,0xff }, /*    61 */
  {     0,   210,    68,0, 0x0000,0x0000, 0x51,0x00,0xa3,0xff }, /*    62 */
  {     0,   210,    68,0, 0x0000,0x0000, 0x51,0x00,0xa3,0xff }, /*    63 */
  {   -51,   229,    48,0, 0x0000,0x0000, 0x4c,0x00,0x99,0xff }, /*    64 */
  {   -64,   204,    47,0, 0x0000,0x0000, 0x4c,0x00,0x99,0xff }, /*    65 */
  {   -64,   204,    47,0, 0x0000,0x0000, 0x4c,0x00,0x99,0xff }, /*    66 */
  {   -51,   229,    48,0, 0x0000,0x0000, 0x4c,0x00,0x99,0xff }, /*    67 */
  {   -73,   214,     0,0, 0x0000,0x0000, 0x58,0x00,0xb1,0xff }, /*    68 */
  {   -76,   188,     0,0, 0x0000,0x0000, 0x5c,0x00,0xb8,0xff }, /*    69 */
  {   -57,   201,   -84,0, 0x0000,0x0000, 0x64,0x00,0xc8,0xff }, /*    70 */
  {   -53,   178,   -80,0, 0x0000,0x0000, 0x64,0x00,0xc9,0xff }, /*    71 */
  {   -53,   178,   -80,0, 0x0000,0x0000, 0x64,0x00,0xc9,0xff }, /*    72 */
  {     0,   193,  -119,0, 0x0000,0x0000, 0x58,0x00,0xb1,0xff }, /*    73 */
  {     0,   168,  -112,0, 0x0000,0x0000, 0x55,0x00,0xaa,0xff }, /*    74 */
  {     0,   168,  -112,0, 0x0000,0x0000, 0x55,0x00,0xaa,0xff }, /*    75 */
  {     0,   193,  -119,0, 0x0000,0x0000, 0x58,0x00,0xb1,0xff }, /*    76 */
  {    53,   178,   -80,0, 0x0000,0x0000, 0x44,0x00,0x89,0xff }, /*    77 */
  {     0,   193,  -119,0, 0x0000,0x0000, 0x58,0x00,0xb1,0xff }, /*    78 */
  {    57,   201,   -84,0, 0x0000,0x0000, 0x3f,0x00,0x7e,0xff }, /*    79 */
  {    53,   178,   -80,0, 0x0000,0x0000, 0x44,0x00,0x89,0xff }, /*    80 */
  {    36,   132,    52,0, 0x0000,0x0000, 0xc9,0x91,0x68,0xff }, /*    81 */
  {    52,   132,     0,0, 0x0000,0x0000, 0xa5,0x77,0x56,0xff }, /*    82 */
  {    57,   188,     0,0, 0x0000,0x0000, 0xf9,0xb4,0x81,0xff }, /*    83 */
  {   -36,   132,    52,0, 0x0000,0x0000, 0xdd,0xa0,0x73,0xff }, /*    84 */
  {   -42,   170,    65,0, 0x0000,0x0000, 0xf3,0x73,0x4c,0xff }, /*    85 */
  {   -57,   188,     0,0, 0x0000,0x0000, 0xe9,0xa9,0x79,0xff }, /*    86 */
  {     0,   114,    28,0, 0x0000,0x0000, 0xd1,0x97,0x6d,0xff }, /*    87 */
  {    36,   132,    52,0, 0x0000,0x0000, 0xc9,0x91,0x68,0xff }, /*    88 */
  {   -36,   132,    52,0, 0x0000,0x0000, 0xdd,0xa0,0x73,0xff }, /*    89 */
  {   -57,   188,     0,0, 0x0000,0x0000, 0x00,0x48,0x91,0xff }, /*    90 */
  {   -71,   160,    11,0, 0x0000,0x0000, 0x00,0x61,0xc2,0xff }, /*    91 */
  {   -76,   188,     0,0, 0x0000,0x0000, 0x00,0x5c,0xb8,0xff }, /*    92 */
  {   -76,   188,     0,0, 0x0000,0x0000, 0x00,0x5c,0xb8,0xff }, /*    93 */
  {   -64,   174,    43,0, 0x0000,0x0000, 0x00,0x46,0x8d,0xff }, /*    94 */
  {   -64,   204,    47,0, 0x0000,0x0000, 0x00,0x4c,0x99,0xff }, /*    95 */
  {   -64,   204,    47,0, 0x0000,0x0000, 0x00,0x4c,0x99,0xff }, /*    96 */
  {   -56,   174,    44,0, 0x0000,0x0000, 0x00,0x46,0x8c,0xff }, /*    97 */
  {   -42,   204,    49,0, 0x0000,0x0000, 0x00,0x51,0xa3,0xff }, /*    98 */
  {   -65,   160,    12,0, 0x0000,0x0000, 0x00,0x52,0xa5,0xff }, /*    99 */
  {   -66,   138,    24,0, 0x0000,0x0000, 0x00,0x00,0x5e,0xff }, /*   100 */
  {   -71,   160,    11,0, 0x0000,0x0000, 0x00,0x61,0xc2,0xff }, /*   101 */
  {    76,   188,     0,0, 0x0000,0x0000, 0x00,0x34,0x69,0xff }, /*   102 */
  {    71,   160,    11,0, 0x0000,0x0000, 0x00,0x4a,0x94,0xff }, /*   103 */
  {    57,   188,     0,0, 0x0000,0x0000, 0x00,0x4d,0x9b,0xff }, /*   104 */
  {   -71,   160,    11,0, 0x0000,0x0000, 0x00,0x61,0xc2,0xff }, /*   105 */
  {   -66,   138,    24,0, 0x0000,0x0000, 0x00,0x00,0x5e,0xff }, /*   106 */
  {   -64,   174,    43,0, 0x0000,0x0000, 0x00,0x46,0x8d,0xff }, /*   107 */
  {   -64,   174,    43,0, 0x0000,0x0000, 0x00,0x46,0x8d,0xff }, /*   108 */
  {   -66,   138,    24,0, 0x0000,0x0000, 0x00,0x00,0x5e,0xff }, /*   109 */
  {   -56,   174,    44,0, 0x0000,0x0000, 0x00,0x46,0x8c,0xff }, /*   110 */
  {    65,   158,    -6,0, 0x0000,0x0000, 0x00,0x50,0xa0,0xff }, /*   111 */
  {    57,   115,   -36,0, 0x0000,0x0000, 0x00,0x00,0x5b,0xff }, /*   112 */
  {    73,   158,    -6,0, 0x0000,0x0000, 0x00,0x39,0x72,0xff }, /*   113 */
  {    42,   204,    49,0, 0x0000,0x0000, 0x00,0x4e,0x9d,0xff }, /*   114 */
  {    56,   174,    44,0, 0x0000,0x0000, 0x00,0x4d,0x9a,0xff }, /*   115 */
  {    64,   204,    47,0, 0x0000,0x0000, 0x00,0x34,0x68,0xff }, /*   116 */
  {    64,   204,    47,0, 0x0000,0x0000, 0x00,0x34,0x68,0xff }, /*   117 */
  {    64,   174,    43,0, 0x0000,0x0000, 0x00,0x35,0x6b,0xff }, /*   118 */
  {    76,   188,     0,0, 0x0000,0x0000, 0x00,0x34,0x69,0xff }, /*   119 */
  {    56,   174,    44,0, 0x0000,0x0000, 0x00,0x4d,0x9a,0xff }, /*   120 */
  {    66,   138,    24,0, 0x0000,0x0000, 0x00,0x00,0x5c,0xff }, /*   121 */
  {    64,   174,    43,0, 0x0000,0x0000, 0x00,0x35,0x6b,0xff }, /*   122 */
  {    64,   174,    43,0, 0x0000,0x0000, 0x00,0x35,0x6b,0xff }, /*   123 */
  {    66,   138,    24,0, 0x0000,0x0000, 0x00,0x00,0x5c,0xff }, /*   124 */
  {    71,   160,    11,0, 0x0000,0x0000, 0x00,0x4a,0x94,0xff }, /*   125 */
  {    64,   174,    43,0, 0x0000,0x0000, 0x00,0x35,0x6b,0xff }, /*   126 */
  {    71,   160,    11,0, 0x0000,0x0000, 0x00,0x4a,0x94,0xff }, /*   127 */
  {    76,   188,     0,0, 0x0000,0x0000, 0x00,0x34,0x69,0xff }, /*   128 */
  {    71,   160,    11,0, 0x0000,0x0000, 0x00,0x4a,0x94,0xff }, /*   129 */
  {    66,   138,    24,0, 0x0000,0x0000, 0x00,0x00,0x5c,0xff }, /*   130 */
  {    65,   160,    12,0, 0x0000,0x0000, 0x00,0x69,0xd2,0xff }, /*   131 */
  {    57,   188,     0,0, 0x0000,0x0000, 0x00,0x4d,0x9b,0xff }, /*   132 */
  {    65,   158,    -6,0, 0x0000,0x0000, 0x00,0x50,0xa0,0xff }, /*   133 */
  {    76,   188,     0,0, 0x0000,0x0000, 0x00,0x34,0x69,0xff }, /*   134 */
  {   -76,   188,     0,0, 0x0000,0x0000, 0x00,0x5c,0xb8,0xff }, /*   135 */
  {   -65,   158,    -6,0, 0x0000,0x0000, 0x00,0x46,0x8c,0xff }, /*   136 */
  {   -57,   188,     0,0, 0x0000,0x0000, 0x00,0x48,0x91,0xff }, /*   137 */
  {   -76,   188,     0,0, 0x0000,0x0000, 0x00,0x5c,0xb8,0xff }, /*   138 */
  {   -73,   158,    -6,0, 0x0000,0x0000, 0x00,0x45,0x8a,0xff }, /*   139 */
  {   -65,   158,    -6,0, 0x0000,0x0000, 0x00,0x46,0x8c,0xff }, /*   140 */
  {    76,   188,     0,0, 0x0000,0x0000, 0x00,0x34,0x69,0xff }, /*   141 */
  {    73,   158,    -6,0, 0x0000,0x0000, 0x00,0x39,0x72,0xff }, /*   142 */
  {    53,   178,   -80,0, 0x0000,0x0000, 0x00,0x44,0x89,0xff }, /*   143 */
  {    53,   178,   -80,0, 0x0000,0x0000, 0x00,0x44,0x89,0xff }, /*   144 */
  {    54,   149,   -72,0, 0x0000,0x0000, 0x00,0x58,0xb0,0xff }, /*   145 */
  {    51,   178,   -65,0, 0x0000,0x0000, 0x00,0x4a,0x95,0xff }, /*   146 */
  {    73,   158,    -6,0, 0x0000,0x0000, 0x00,0x39,0x72,0xff }, /*   147 */
  {    57,   115,   -36,0, 0x0000,0x0000, 0x00,0x00,0x5b,0xff }, /*   148 */
  {    54,   149,   -72,0, 0x0000,0x0000, 0x00,0x58,0xb0,0xff }, /*   149 */
  {    57,   188,     0,0, 0x0000,0x0000, 0x00,0x4d,0x9b,0xff }, /*   150 */
  {    52,   132,     0,0, 0x0000,0x0000, 0x00,0x33,0x67,0xff }, /*   151 */
  {    36,   132,   -52,0, 0x0000,0x0000, 0x00,0x4c,0x98,0xff }, /*   152 */
  {    54,   149,   -72,0, 0x0000,0x0000, 0x00,0x58,0xb0,0xff }, /*   153 */
  {    57,   115,   -36,0, 0x0000,0x0000, 0x00,0x00,0x5b,0xff }, /*   154 */
  {    54,   149,   -60,0, 0x0000,0x0000, 0x00,0x62,0xc4,0xff }, /*   155 */
  {    54,   149,   -72,0, 0x0000,0x0000, 0x00,0x58,0xb0,0xff }, /*   156 */
  {    54,   149,   -60,0, 0x0000,0x0000, 0x00,0x62,0xc4,0xff }, /*   157 */
  {    51,   178,   -65,0, 0x0000,0x0000, 0x00,0x4a,0x95,0xff }, /*   158 */
  {   -53,   178,   -80,0, 0x0000,0x0000, 0x00,0x64,0xc9,0xff }, /*   159 */
  {   -73,   158,    -6,0, 0x0000,0x0000, 0x00,0x45,0x8a,0xff }, /*   160 */
  {   -76,   188,     0,0, 0x0000,0x0000, 0x00,0x5c,0xb8,0xff }, /*   161 */
  {   -51,   178,   -65,0, 0x0000,0x0000, 0x00,0x51,0xa2,0xff }, /*   162 */
  {   -54,   149,   -72,0, 0x0000,0x0000, 0x00,0x57,0xae,0xff }, /*   163 */
  {   -53,   178,   -80,0, 0x0000,0x0000, 0x00,0x64,0xc9,0xff }, /*   164 */
  {   -54,   149,   -72,0, 0x0000,0x0000, 0x00,0x57,0xae,0xff }, /*   165 */
  {   -57,   115,   -36,0, 0x0000,0x0000, 0x00,0x00,0x5e,0xff }, /*   166 */
  {   -73,   158,    -6,0, 0x0000,0x0000, 0x00,0x45,0x8a,0xff }, /*   167 */
  {   -73,   158,    -6,0, 0x0000,0x0000, 0x00,0x45,0x8a,0xff }, /*   168 */
  {   -57,   115,   -36,0, 0x0000,0x0000, 0x00,0x00,0x5e,0xff }, /*   169 */
  {   -65,   158,    -6,0, 0x0000,0x0000, 0x00,0x46,0x8c,0xff }, /*   170 */
  {   -54,   149,   -60,0, 0x0000,0x0000, 0x00,0x41,0x83,0xff }, /*   171 */
  {   -57,   115,   -36,0, 0x0000,0x0000, 0x00,0x00,0x5e,0xff }, /*   172 */
  {   -54,   149,   -72,0, 0x0000,0x0000, 0x00,0x57,0xae,0xff }, /*   173 */
  {   -51,   178,   -65,0, 0x0000,0x0000, 0x00,0x51,0xa2,0xff }, /*   174 */
  {   -54,   149,   -60,0, 0x0000,0x0000, 0x00,0x41,0x83,0xff }, /*   175 */
  {   -54,   149,   -72,0, 0x0000,0x0000, 0x00,0x57,0xae,0xff }, /*   176 */
  {    51,   178,   -65,0, 0x0000,0x0000, 0x00,0x4a,0x95,0xff }, /*   177 */
  {    47,   148,   -68,0, 0x0000,0x0000, 0x00,0x46,0x8c,0xff }, /*   178 */
  {    53,   178,   -80,0, 0x0000,0x0000, 0x00,0x44,0x89,0xff }, /*   179 */
  {    53,   178,   -80,0, 0x0000,0x0000, 0x00,0x44,0x89,0xff }, /*   180 */
  {     4,   140,  -108,0, 0x0000,0x0000, 0x00,0x59,0xb2,0xff }, /*   181 */
  {     0,   168,  -112,0, 0x0000,0x0000, 0x00,0x55,0xaa,0xff }, /*   182 */
  {     0,   168,  -112,0, 0x0000,0x0000, 0x00,0x55,0xaa,0xff }, /*   183 */
  {     4,   140,  -108,0, 0x0000,0x0000, 0x00,0x59,0xb2,0xff }, /*   184 */
  {     0,   168,   -91,0, 0x0000,0x0000, 0x00,0x4d,0x9a,0xff }, /*   185 */
  {    47,   148,   -68,0, 0x0000,0x0000, 0x00,0x46,0x8c,0xff }, /*   186 */
  {    28,   113,   -63,0, 0x0000,0x0000, 0x00,0x00,0x59,0xff }, /*   187 */
  {    56,   148,   -81,0, 0x0000,0x0000, 0x00,0x39,0x73,0xff }, /*   188 */
  {    56,   148,   -81,0, 0x0000,0x0000, 0x00,0x39,0x73,0xff }, /*   189 */
  {    28,   113,   -63,0, 0x0000,0x0000, 0x00,0x00,0x59,0xff }, /*   190 */
  {     4,   140,  -108,0, 0x0000,0x0000, 0x00,0x59,0xb2,0xff }, /*   191 */
  {     4,   140,  -108,0, 0x0000,0x0000, 0x00,0x59,0xb2,0xff }, /*   192 */
  {    28,   113,   -63,0, 0x0000,0x0000, 0x00,0x00,0x59,0xff }, /*   193 */
  {     4,   140,   -91,0, 0x0000,0x0000, 0x00,0x55,0xaa,0xff }, /*   194 */
  {     0,   168,   -91,0, 0x0000,0x0000, 0x00,0x4d,0x9a,0xff }, /*   195 */
  {    -4,   140,  -108,0, 0x0000,0x0000, 0x00,0x52,0xa5,0xff }, /*   196 */
  {     0,   168,  -112,0, 0x0000,0x0000, 0x00,0x55,0xaa,0xff }, /*   197 */
  {     0,   168,   -91,0, 0x0000,0x0000, 0x00,0x4d,0x9a,0xff }, /*   198 */
  {    -4,   140,   -91,0, 0x0000,0x0000, 0x00,0x40,0x81,0xff }, /*   199 */
  {    -4,   140,  -108,0, 0x0000,0x0000, 0x00,0x52,0xa5,0xff }, /*   200 */
  {     0,   168,  -112,0, 0x0000,0x0000, 0x00,0x55,0xaa,0xff }, /*   201 */
  {    -4,   140,  -108,0, 0x0000,0x0000, 0x00,0x52,0xa5,0xff }, /*   202 */
  {   -53,   178,   -80,0, 0x0000,0x0000, 0x00,0x64,0xc9,0xff }, /*   203 */
  {   -53,   178,   -80,0, 0x0000,0x0000, 0x00,0x64,0xc9,0xff }, /*   204 */
  {   -47,   148,   -68,0, 0x0000,0x0000, 0x00,0x4d,0x9a,0xff }, /*   205 */
  {   -51,   178,   -65,0, 0x0000,0x0000, 0x00,0x51,0xa2,0xff }, /*   206 */
  {    -4,   140,   -91,0, 0x0000,0x0000, 0x00,0x40,0x81,0xff }, /*   207 */
  {   -28,   113,   -63,0, 0x0000,0x0000, 0x00,0x00,0x58,0xff }, /*   208 */
  {    -4,   140,  -108,0, 0x0000,0x0000, 0x00,0x52,0xa5,0xff }, /*   209 */
  {    -4,   140,  -108,0, 0x0000,0x0000, 0x00,0x52,0xa5,0xff }, /*   210 */
  {   -28,   113,   -63,0, 0x0000,0x0000, 0x00,0x00,0x58,0xff }, /*   211 */
  {   -56,   148,   -81,0, 0x0000,0x0000, 0x00,0x64,0xc8,0xff }, /*   212 */
  {   -56,   148,   -81,0, 0x0000,0x0000, 0x00,0x64,0xc8,0xff }, /*   213 */
  {   -28,   113,   -63,0, 0x0000,0x0000, 0x00,0x00,0x58,0xff }, /*   214 */
  {   -47,   148,   -68,0, 0x0000,0x0000, 0x00,0x4d,0x9a,0xff }, /*   215 */
  {    42,   170,    65,0, 0x0000,0x0000, 0xb3,0x55,0x38,0xff }, /*   216 */
  {    57,   188,     0,0, 0x0000,0x0000, 0xf9,0xb4,0x81,0xff }, /*   217 */
  {    42,   204,    49,0, 0x0000,0x0000, 0xfd,0xb7,0x83,0xff }, /*   218 */
  {   -42,   170,    65,0, 0x0000,0x0000, 0xf3,0x73,0x4c,0xff }, /*   219 */
  {   -42,   204,    49,0, 0x0000,0x0000, 0xff,0xbe,0x89,0xff }, /*   220 */
  {   -57,   188,     0,0, 0x0000,0x0000, 0xe9,0xa9,0x79,0xff }, /*   221 */
  {    64,   204,    47,0, 0x0000,0x0000, 0x34,0x00,0x68,0xff }, /*   222 */
  {    76,   188,     0,0, 0x0000,0x0000, 0x34,0x00,0x69,0xff }, /*   223 */
  {    73,   214,     0,0, 0x0000,0x0000, 0x2f,0x00,0x5e,0xff }, /*   224 */
  {   -73,   214,     0,0, 0x0000,0x0000, 0x58,0x00,0xb1,0xff }, /*   225 */
  {   -76,   188,     0,0, 0x0000,0x0000, 0x5c,0x00,0xb8,0xff }, /*   226 */
  {   -64,   204,    47,0, 0x0000,0x0000, 0x4c,0x00,0x99,0xff }, /*   227 */
  {     0,   275,    27,0, 0x0000,0x0000, 0xcf,0x9a,0x00,0xff }, /*   228 */
  {    -6,   247,    66,0, 0x0000,0x0000, 0xc8,0x95,0x00,0xff }, /*   229 */
  {     0,   236,    62,0, 0x0000,0x0000, 0xe4,0xaa,0x00,0xff }, /*   230 */
  {     0,   236,    62,0, 0x0000,0x0000, 0xe4,0xaa,0x00,0xff }, /*   231 */
  {   -50,   241,    55,0, 0x0000,0x0000, 0xee,0xb1,0x00,0xff }, /*   232 */
  {   -51,   229,    48,0, 0x0000,0x0000, 0xff,0xbe,0x00,0xff }, /*   233 */
  {     0,   236,    62,0, 0x0000,0x0000, 0xe4,0xaa,0x00,0xff }, /*   234 */
  {    -6,   247,    66,0, 0x0000,0x0000, 0xc8,0x95,0x00,0xff }, /*   235 */
  {   -50,   241,    55,0, 0x0000,0x0000, 0xee,0xb1,0x00,0xff }, /*   236 */
  {   -51,   229,    48,0, 0x0000,0x0000, 0xff,0xbe,0x00,0xff }, /*   237 */
  {   -59,   273,    28,0, 0x0000,0x0000, 0xff,0xbe,0x00,0xff }, /*   238 */
  {   -59,   267,    16,0, 0x0000,0x0000, 0xff,0xc9,0x00,0xff }, /*   239 */
  {   -59,   267,    16,0, 0x0000,0x0000, 0xff,0xc9,0x00,0xff }, /*   240 */
  {    -6,   279,    37,0, 0x0000,0x0000, 0xbe,0x8e,0x00,0xff }, /*   241 */
  {     0,   275,    27,0, 0x0000,0x0000, 0xcf,0x9a,0x00,0xff }, /*   242 */
  {    51,   229,    48,0, 0x0000,0x0000, 0xb0,0x83,0x00,0xff }, /*   243 */
  {    50,   241,    55,0, 0x0000,0x0000, 0xc1,0x90,0x00,0xff }, /*   244 */
  {     0,   236,    62,0, 0x0000,0x0000, 0xe4,0xaa,0x00,0xff }, /*   245 */
  {     0,   236,    62,0, 0x0000,0x0000, 0xe4,0xaa,0x00,0xff }, /*   246 */
  {     6,   247,    66,0, 0x0000,0x0000, 0xc3,0x91,0x00,0xff }, /*   247 */
  {     0,   275,    27,0, 0x0000,0x0000, 0xcf,0x9a,0x00,0xff }, /*   248 */
  {     6,   247,    66,0, 0x0000,0x0000, 0xc3,0x91,0x00,0xff }, /*   249 */
  {     6,   279,    37,0, 0x0000,0x0000, 0xec,0xaf,0x00,0xff }, /*   250 */
  {     0,   275,    27,0, 0x0000,0x0000, 0xcf,0x9a,0x00,0xff }, /*   251 */
  {    53,   268,    29,0, 0x0000,0x0000, 0xc6,0x94,0x00,0xff }, /*   252 */
  {    46,   242,    50,0, 0x0000,0x0000, 0xe3,0xa9,0x00,0xff }, /*   253 */
  {    50,   241,    55,0, 0x0000,0x0000, 0xc1,0x90,0x00,0xff }, /*   254 */
  {     0,   275,    27,0, 0x0000,0x0000, 0xcf,0x9a,0x00,0xff }, /*   255 */
  {     6,   279,    37,0, 0x0000,0x0000, 0xec,0xaf,0x00,0xff }, /*   256 */
  {    59,   267,    16,0, 0x0000,0x0000, 0x6e,0x52,0x00,0xff }, /*   257 */
  {    59,   267,    16,0, 0x0000,0x0000, 0x6e,0x52,0x00,0xff }, /*   258 */
  {    59,   273,    28,0, 0x0000,0x0000, 0x5c,0x45,0x00,0xff }, /*   259 */
  {    51,   229,    48,0, 0x0000,0x0000, 0xb0,0x83,0x00,0xff }, /*   260 */
  {    59,   273,    28,0, 0x0000,0x0000, 0x5c,0x45,0x00,0xff }, /*   261 */
  {    50,   241,    55,0, 0x0000,0x0000, 0xc1,0x90,0x00,0xff }, /*   262 */
  {    51,   229,    48,0, 0x0000,0x0000, 0xb0,0x83,0x00,0xff }, /*   263 */
  {     6,   279,    37,0, 0x0000,0x0000, 0xec,0xaf,0x00,0xff }, /*   264 */
  {    59,   273,    28,0, 0x0000,0x0000, 0x5c,0x45,0x00,0xff }, /*   265 */
  {    59,   267,    16,0, 0x0000,0x0000, 0x6e,0x52,0x00,0xff }, /*   266 */
  {    50,   241,    55,0, 0x0000,0x0000, 0xc1,0x90,0x00,0xff }, /*   267 */
  {     6,   247,    66,0, 0x0000,0x0000, 0xc3,0x91,0x00,0xff }, /*   268 */
  {     0,   236,    62,0, 0x0000,0x0000, 0xe4,0xaa,0x00,0xff }, /*   269 */
  {   -59,   267,    16,0, 0x0000,0x0000, 0xff,0xc9,0x00,0xff }, /*   270 */
  {   -59,   273,    28,0, 0x0000,0x0000, 0xff,0xbe,0x00,0xff }, /*   271 */
  {    -6,   279,    37,0, 0x0000,0x0000, 0xbe,0x8e,0x00,0xff }, /*   272 */
  {   -51,   229,    48,0, 0x0000,0x0000, 0xff,0xbe,0x00,0xff }, /*   273 */
  {   -50,   241,    55,0, 0x0000,0x0000, 0xee,0xb1,0x00,0xff }, /*   274 */
  {   -59,   273,    28,0, 0x0000,0x0000, 0xff,0xbe,0x00,0xff }, /*   275 */
  {    59,   273,    28,0, 0x0000,0x0000, 0x5c,0x45,0x00,0xff }, /*   276 */
  {    53,   268,    29,0, 0x0000,0x0000, 0xc6,0x94,0x00,0xff }, /*   277 */
  {    50,   241,    55,0, 0x0000,0x0000, 0xc1,0x90,0x00,0xff }, /*   278 */
  {     0,   275,    27,0, 0x0000,0x0000, 0xcf,0x9a,0x00,0xff }, /*   279 */
  {    -6,   279,    37,0, 0x0000,0x0000, 0xbe,0x8e,0x00,0xff }, /*   280 */
  {    -6,   247,    66,0, 0x0000,0x0000, 0xc8,0x95,0x00,0xff }, /*   281 */
  {   -53,   178,   -80,0, 0x0000,0x0000, 0x00,0x64,0xc9,0xff }, /*   282 */
  {   -56,   148,   -81,0, 0x0000,0x0000, 0x00,0x64,0xc8,0xff }, /*   283 */
  {   -47,   148,   -68,0, 0x0000,0x0000, 0x00,0x4d,0x9a,0xff }, /*   284 */
  {    -4,   140,  -108,0, 0x0000,0x0000, 0x00,0x52,0xa5,0xff }, /*   285 */
  {   -56,   148,   -81,0, 0x0000,0x0000, 0x00,0x64,0xc8,0xff }, /*   286 */
  {   -53,   178,   -80,0, 0x0000,0x0000, 0x00,0x64,0xc9,0xff }, /*   287 */
  {     4,   140,  -108,0, 0x0000,0x0000, 0x00,0x59,0xb2,0xff }, /*   288 */
  {     4,   140,   -91,0, 0x0000,0x0000, 0x00,0x55,0xaa,0xff }, /*   289 */
  {     0,   168,   -91,0, 0x0000,0x0000, 0x00,0x4d,0x9a,0xff }, /*   290 */
  {    53,   178,   -80,0, 0x0000,0x0000, 0x00,0x44,0x89,0xff }, /*   291 */
  {    56,   148,   -81,0, 0x0000,0x0000, 0x00,0x39,0x73,0xff }, /*   292 */
  {     4,   140,  -108,0, 0x0000,0x0000, 0x00,0x59,0xb2,0xff }, /*   293 */
  {    47,   148,   -68,0, 0x0000,0x0000, 0x00,0x46,0x8c,0xff }, /*   294 */
  {    56,   148,   -81,0, 0x0000,0x0000, 0x00,0x39,0x73,0xff }, /*   295 */
  {    53,   178,   -80,0, 0x0000,0x0000, 0x00,0x44,0x89,0xff }, /*   296 */
  {   -53,   178,   -80,0, 0x0000,0x0000, 0x00,0x64,0xc9,0xff }, /*   297 */
  {   -54,   149,   -72,0, 0x0000,0x0000, 0x00,0x57,0xae,0xff }, /*   298 */
  {   -73,   158,    -6,0, 0x0000,0x0000, 0x00,0x45,0x8a,0xff }, /*   299 */
  {    73,   158,    -6,0, 0x0000,0x0000, 0x00,0x39,0x72,0xff }, /*   300 */
  {    54,   149,   -72,0, 0x0000,0x0000, 0x00,0x58,0xb0,0xff }, /*   301 */
  {    53,   178,   -80,0, 0x0000,0x0000, 0x00,0x44,0x89,0xff }, /*   302 */
  {    65,   158,    -6,0, 0x0000,0x0000, 0x00,0x50,0xa0,0xff }, /*   303 */
  {    73,   158,    -6,0, 0x0000,0x0000, 0x00,0x39,0x72,0xff }, /*   304 */
  {    76,   188,     0,0, 0x0000,0x0000, 0x00,0x34,0x69,0xff }, /*   305 */
  {    71,   160,    11,0, 0x0000,0x0000, 0x00,0x4a,0x94,0xff }, /*   306 */
  {    65,   160,    12,0, 0x0000,0x0000, 0x00,0x69,0xd2,0xff }, /*   307 */
  {    57,   188,     0,0, 0x0000,0x0000, 0x00,0x4d,0x9b,0xff }, /*   308 */
  {    56,   174,    44,0, 0x0000,0x0000, 0x00,0x4d,0x9a,0xff }, /*   309 */
  {    64,   174,    43,0, 0x0000,0x0000, 0x00,0x35,0x6b,0xff }, /*   310 */
  {    64,   204,    47,0, 0x0000,0x0000, 0x00,0x34,0x68,0xff }, /*   311 */
  {   -64,   204,    47,0, 0x0000,0x0000, 0x00,0x4c,0x99,0xff }, /*   312 */
  {   -64,   174,    43,0, 0x0000,0x0000, 0x00,0x46,0x8d,0xff }, /*   313 */
  {   -56,   174,    44,0, 0x0000,0x0000, 0x00,0x46,0x8c,0xff }, /*   314 */
  {   -76,   188,     0,0, 0x0000,0x0000, 0x00,0x5c,0xb8,0xff }, /*   315 */
  {   -71,   160,    11,0, 0x0000,0x0000, 0x00,0x61,0xc2,0xff }, /*   316 */
  {   -64,   174,    43,0, 0x0000,0x0000, 0x00,0x46,0x8d,0xff }, /*   317 */
  {   -57,   188,     0,0, 0x0000,0x0000, 0x00,0x48,0x91,0xff }, /*   318 */
  {   -65,   160,    12,0, 0x0000,0x0000, 0x00,0x52,0xa5,0xff }, /*   319 */
  {   -71,   160,    11,0, 0x0000,0x0000, 0x00,0x61,0xc2,0xff }, /*   320 */
  {   -53,   178,   -80,0, 0x0000,0x0000, 0x64,0x00,0xc9,0xff }, /*   321 */
  {   -57,   201,   -84,0, 0x0000,0x0000, 0x64,0x00,0xc8,0xff }, /*   322 */
  {     0,   193,  -119,0, 0x0000,0x0000, 0x58,0x00,0xb1,0xff }, /*   323 */
  {   -76,   188,     0,0, 0x0000,0x0000, 0x5c,0x00,0xb8,0xff }, /*   324 */
  {   -73,   214,     0,0, 0x0000,0x0000, 0x58,0x00,0xb1,0xff }, /*   325 */
  {   -57,   201,   -84,0, 0x0000,0x0000, 0x64,0x00,0xc8,0xff }, /*   326 */
  {     0,   210,    68,0, 0x0000,0x0000, 0x51,0x00,0xa3,0xff }, /*   327 */
  {     0,   236,    62,0, 0x0000,0x0000, 0x44,0x00,0x89,0xff }, /*   328 */
  {   -51,   229,    48,0, 0x0000,0x0000, 0x4c,0x00,0x99,0xff }, /*   329 */
  {    51,   229,    48,0, 0x0000,0x0000, 0x34,0x00,0x69,0xff }, /*   330 */
  {     0,   236,    62,0, 0x0000,0x0000, 0x44,0x00,0x89,0xff }, /*   331 */
  {     0,   210,    68,0, 0x0000,0x0000, 0x51,0x00,0xa3,0xff }, /*   332 */
  {    57,   201,   -84,0, 0x0000,0x0000, 0x3f,0x00,0x7e,0xff }, /*   333 */
  {    73,   214,     0,0, 0x0000,0x0000, 0x2f,0x00,0x5e,0xff }, /*   334 */
  {    76,   188,     0,0, 0x0000,0x0000, 0x34,0x00,0x69,0xff }, /*   335 */
  {   -15,   114,     0,0, 0x0000,0x0000, 0xe3,0xa4,0x76,0xff }, /*   336 */
  {   -18,   114,    20,0, 0x0000,0x0000, 0xec,0xab,0x7b,0xff }, /*   337 */
  {   -36,   132,    52,0, 0x0000,0x0000, 0xdd,0xa0,0x73,0xff }, /*   338 */
  {   -36,   132,   -52,0, 0x0000,0x0000, 0xff,0xc7,0x8f,0xff }, /*   339 */
  {   -11,   114,   -20,0, 0x0000,0x0000, 0xff,0xd3,0x97,0xff }, /*   340 */
  {   -15,   114,     0,0, 0x0000,0x0000, 0xe3,0xa4,0x76,0xff }, /*   341 */
  {     0,   132,   -74,0, 0x0000,0x0000, 0xff,0xd6,0x99,0xff }, /*   342 */
  {     0,   114,   -28,0, 0x0000,0x0000, 0xff,0xda,0x9c,0xff }, /*   343 */
  {   -11,   114,   -20,0, 0x0000,0x0000, 0xff,0xd3,0x97,0xff }, /*   344 */
  {    11,   114,   -20,0, 0x0000,0x0000, 0xec,0xaa,0x7a,0xff }, /*   345 */
  {     0,   114,   -28,0, 0x0000,0x0000, 0xff,0xda,0x9c,0xff }, /*   346 */
  {     0,   132,   -74,0, 0x0000,0x0000, 0xff,0xd6,0x99,0xff }, /*   347 */
  {    15,   114,     0,0, 0x0000,0x0000, 0xcd,0x94,0x6b,0xff }, /*   348 */
  {    11,   114,   -20,0, 0x0000,0x0000, 0xec,0xaa,0x7a,0xff }, /*   349 */
  {    36,   132,   -52,0, 0x0000,0x0000, 0xf5,0xb1,0x7f,0xff }, /*   350 */
  {   -36,   132,   -52,0, 0x0000,0x0000, 0x00,0x55,0xab,0xff }, /*   351 */
  {   -52,   132,     0,0, 0x0000,0x0000, 0x00,0x55,0xab,0xff }, /*   352 */
  {   -57,   188,     0,0, 0x0000,0x0000, 0x00,0x48,0x91,0xff }, /*   353 */
  {     0,   132,   -74,0, 0x0000,0x0000, 0x00,0x5c,0xb8,0xff }, /*   354 */
  {   -36,   132,   -52,0, 0x0000,0x0000, 0x00,0x55,0xab,0xff }, /*   355 */
  {   -51,   178,   -65,0, 0x0000,0x0000, 0x00,0x51,0xa2,0xff }, /*   356 */
  {    51,   178,   -65,0, 0x0000,0x0000, 0x00,0x4a,0x95,0xff }, /*   357 */
  {    36,   132,   -52,0, 0x0000,0x0000, 0x00,0x4c,0x98,0xff }, /*   358 */
  {     0,   132,   -74,0, 0x0000,0x0000, 0x00,0x5c,0xb8,0xff }, /*   359 */
  {     6,   247,    66,0, 0x0000,0x0000, 0xc3,0x91,0x00,0xff }, /*   360 */
  {     9,   247,    60,0, 0x0000,0x0000, 0x80,0x60,0x00,0xff }, /*   361 */
  {     6,   279,    37,0, 0x0000,0x0000, 0xec,0xaf,0x00,0xff }, /*   362 */
  {     6,   279,    37,0, 0x0000,0x0000, 0xec,0xaf,0x00,0xff }, /*   363 */
  {    53,   268,    29,0, 0x0000,0x0000, 0xc6,0x94,0x00,0xff }, /*   364 */
  {    59,   273,    28,0, 0x0000,0x0000, 0x5c,0x45,0x00,0xff }, /*   365 */
  {    -9,   247,    60,0, 0x0000,0x0000, 0xff,0xc5,0x00,0xff }, /*   366 */
  {   -46,   242,    50,0, 0x0000,0x0000, 0xdc,0xa4,0x00,0xff }, /*   367 */
  {   -50,   241,    55,0, 0x0000,0x0000, 0xee,0xb1,0x00,0xff }, /*   368 */
  {    50,   241,    55,0, 0x0000,0x0000, 0xc1,0x90,0x00,0xff }, /*   369 */
  {     9,   247,    60,0, 0x0000,0x0000, 0x80,0x60,0x00,0xff }, /*   370 */
  {     6,   247,    66,0, 0x0000,0x0000, 0xc3,0x91,0x00,0xff }, /*   371 */
  {    -6,   279,    37,0, 0x0000,0x0000, 0xbe,0x8e,0x00,0xff }, /*   372 */
  {    -9,   247,    60,0, 0x0000,0x0000, 0xff,0xc5,0x00,0xff }, /*   373 */
  {    -6,   247,    66,0, 0x0000,0x0000, 0xc8,0x95,0x00,0xff }, /*   374 */
  {    -6,   247,    66,0, 0x0000,0x0000, 0xc8,0x95,0x00,0xff }, /*   375 */
  {    -9,   247,    60,0, 0x0000,0x0000, 0xff,0xc5,0x00,0xff }, /*   376 */
  {   -50,   241,    55,0, 0x0000,0x0000, 0xee,0xb1,0x00,0xff }, /*   377 */
  {   -50,   241,    55,0, 0x0000,0x0000, 0xee,0xb1,0x00,0xff }, /*   378 */
  {   -53,   268,    29,0, 0x0000,0x0000, 0xbb,0x8b,0x00,0xff }, /*   379 */
  {   -59,   273,    28,0, 0x0000,0x0000, 0xff,0xbe,0x00,0xff }, /*   380 */
  {   -59,   273,    28,0, 0x0000,0x0000, 0xff,0xbe,0x00,0xff }, /*   381 */
  {   -53,   268,    29,0, 0x0000,0x0000, 0xbb,0x8b,0x00,0xff }, /*   382 */
  {    -6,   279,    37,0, 0x0000,0x0000, 0xbe,0x8e,0x00,0xff }, /*   383 */
  {   -53,   268,    29,0, 0x0000,0x0000, 0xbb,0x8b,0x00,0xff }, /*   384 */
  {    -9,   273,    37,0, 0x0000,0x0000, 0xf2,0xb4,0x00,0xff }, /*   385 */
  {    -6,   279,    37,0, 0x0000,0x0000, 0xbe,0x8e,0x00,0xff }, /*   386 */
  {   -50,   241,    55,0, 0x0000,0x0000, 0xee,0xb1,0x00,0xff }, /*   387 */
  {   -46,   242,    50,0, 0x0000,0x0000, 0xdc,0xa4,0x00,0xff }, /*   388 */
  {   -53,   268,    29,0, 0x0000,0x0000, 0xbb,0x8b,0x00,0xff }, /*   389 */
  {    -6,   279,    37,0, 0x0000,0x0000, 0xbe,0x8e,0x00,0xff }, /*   390 */
  {    -9,   273,    37,0, 0x0000,0x0000, 0xf2,0xb4,0x00,0xff }, /*   391 */
  {    -9,   247,    60,0, 0x0000,0x0000, 0xff,0xc5,0x00,0xff }, /*   392 */
  {    50,   241,    55,0, 0x0000,0x0000, 0xc1,0x90,0x00,0xff }, /*   393 */
  {    46,   242,    50,0, 0x0000,0x0000, 0xe3,0xa9,0x00,0xff }, /*   394 */
  {     9,   247,    60,0, 0x0000,0x0000, 0x80,0x60,0x00,0xff }, /*   395 */
  {     6,   279,    37,0, 0x0000,0x0000, 0xec,0xaf,0x00,0xff }, /*   396 */
  {     9,   273,    37,0, 0x0000,0x0000, 0xbc,0x8c,0x00,0xff }, /*   397 */
  {    53,   268,    29,0, 0x0000,0x0000, 0xc6,0x94,0x00,0xff }, /*   398 */
  {     9,   247,    60,0, 0x0000,0x0000, 0x80,0x60,0x00,0xff }, /*   399 */
  {     9,   273,    37,0, 0x0000,0x0000, 0xbc,0x8c,0x00,0xff }, /*   400 */
  {     6,   279,    37,0, 0x0000,0x0000, 0xec,0xaf,0x00,0xff }, /*   401 */
  {   -15,   114,     0,0, 0x0000,0x0000, 0xe3,0xa4,0x76,0xff }, /*   402 */
  {    15,    90,     0,0, 0x0000,0x0000, 0xca,0x92,0x69,0xff }, /*   403 */
  {    15,   114,     0,0, 0x0000,0x0000, 0xcd,0x94,0x6b,0xff }, /*   404 */
  {    15,   114,     0,0, 0x0000,0x0000, 0xcd,0x94,0x6b,0xff }, /*   405 */
  {    11,    90,   -20,0, 0x0000,0x0000, 0xf1,0xae,0x7d,0xff }, /*   406 */
  {    11,   114,   -20,0, 0x0000,0x0000, 0xec,0xaa,0x7a,0xff }, /*   407 */
  {    15,   114,     0,0, 0x0000,0x0000, 0xcd,0x94,0x6b,0xff }, /*   408 */
  {    15,    90,     0,0, 0x0000,0x0000, 0xca,0x92,0x69,0xff }, /*   409 */
  {    11,    90,   -20,0, 0x0000,0x0000, 0xf1,0xae,0x7d,0xff }, /*   410 */
  {    11,   114,   -20,0, 0x0000,0x0000, 0xec,0xaa,0x7a,0xff }, /*   411 */
  {     0,    90,   -28,0, 0x0000,0x0000, 0xff,0xd2,0x97,0xff }, /*   412 */
  {     0,   114,   -28,0, 0x0000,0x0000, 0xff,0xda,0x9c,0xff }, /*   413 */
  {    11,   114,   -20,0, 0x0000,0x0000, 0xec,0xaa,0x7a,0xff }, /*   414 */
  {    11,    90,   -20,0, 0x0000,0x0000, 0xf1,0xae,0x7d,0xff }, /*   415 */
  {     0,    90,   -28,0, 0x0000,0x0000, 0xff,0xd2,0x97,0xff }, /*   416 */
  {     0,   114,   -28,0, 0x0000,0x0000, 0xff,0xda,0x9c,0xff }, /*   417 */
  {   -11,    90,   -20,0, 0x0000,0x0000, 0xff,0xc9,0x91,0xff }, /*   418 */
  {   -11,   114,   -20,0, 0x0000,0x0000, 0xff,0xd3,0x97,0xff }, /*   419 */
  {     0,   114,   -28,0, 0x0000,0x0000, 0xff,0xda,0x9c,0xff }, /*   420 */
  {     0,    90,   -28,0, 0x0000,0x0000, 0xff,0xd2,0x97,0xff }, /*   421 */
  {   -11,    90,   -20,0, 0x0000,0x0000, 0xff,0xc9,0x91,0xff }, /*   422 */
  {   -11,   114,   -20,0, 0x0000,0x0000, 0xff,0xd3,0x97,0xff }, /*   423 */
  {   -15,    90,     0,0, 0x0000,0x0000, 0xda,0x9d,0x71,0xff }, /*   424 */
  {   -15,   114,     0,0, 0x0000,0x0000, 0xe3,0xa4,0x76,0xff }, /*   425 */
  {   -11,   114,   -20,0, 0x0000,0x0000, 0xff,0xd3,0x97,0xff }, /*   426 */
  {   -11,    90,   -20,0, 0x0000,0x0000, 0xff,0xc9,0x91,0xff }, /*   427 */
  {   -15,    90,     0,0, 0x0000,0x0000, 0xda,0x9d,0x71,0xff }, /*   428 */
  {   -15,   114,     0,0, 0x0000,0x0000, 0xe3,0xa4,0x76,0xff }, /*   429 */
  {   -15,    90,     0,0, 0x0000,0x0000, 0xda,0x9d,0x71,0xff }, /*   430 */
  {    15,    90,     0,0, 0x0000,0x0000, 0xca,0x92,0x69,0xff }, /*   431 */
  {   -42,   204,    49,0, 0x0331,0xfec5, 0xf5,0xf5,0xf5,0xff }, /*   432 */
  {     0,   210,    49,0, 0x0400,0xff09, 0xf7,0xf7,0xf7,0xff }, /*   433 */
  {     0,   210,    68,0, 0x0400,0xfe90, 0xf5,0xf5,0xf5,0xff }, /*   434 */
  {   -23,   300,   -84,0, 0x01b2,0x06b3, 0x70,0xff,0xff,0xff }, /*   435 */
  {   -23,   302,  -104,0, 0x011e,0x0749, 0x6b,0xff,0xff,0xff }, /*   436 */
  {   -57,   201,   -84,0, 0x012b,0x01f1, 0xff,0xff,0xff,0xff }, /*   437 */
  {     0,   210,    68,0, 0x0400,0xfe90, 0xf5,0xf5,0xf5,0xff }, /*   438 */
  {    42,   204,    49,0, 0x04ce,0xfec5, 0xec,0xec,0xec,0xff }, /*   439 */
  {    64,   204,    47,0, 0x0516,0xfecf, 0x9d,0x9d,0x9d,0xff }, /*   440 */
  {     0,   302,   -81,0, 0x07ff,0x06b4, 0x4b,0xbd,0xbd,0xff }, /*   441 */
  {   -23,   300,   -84,0, 0x01b2,0x06b3, 0x70,0xff,0xff,0xff }, /*   442 */
  {   -20,   319,   -81,0, 0x023e,0x0781, 0x5d,0xea,0xea,0xff }, /*   443 */
  {   -59,   267,    16,0, 0x02fa,0x0294, 0xf3,0xf3,0xf3,0xff }, /*   444 */
  {   -23,   300,   -84,0, 0x01b2,0x06b3, 0x70,0xff,0xff,0xff }, /*   445 */
  {   -73,   214,     0,0, 0x0265,0x007a, 0xff,0xff,0xff,0xff }, /*   446 */
  {   -73,   214,     0,0, 0x0265,0x007a, 0xff,0xff,0xff,0xff }, /*   447 */
  {   -23,   300,   -84,0, 0x01b2,0x06b3, 0x70,0xff,0xff,0xff }, /*   448 */
  {   -57,   201,   -84,0, 0x012b,0x01f1, 0xff,0xff,0xff,0xff }, /*   449 */
  {   -57,   201,   -84,0, 0x012b,0x01f1, 0xff,0xff,0xff,0xff }, /*   450 */
  {     0,   292,  -111,0, 0x0000,0x06f1, 0x66,0xff,0xff,0xff }, /*   451 */
  {     0,   193,  -119,0, 0x0000,0x0267, 0xff,0xff,0xff,0xff }, /*   452 */
  {     0,   302,   -81,0, 0x07ff,0x06b4, 0x4b,0xbd,0xbd,0xff }, /*   453 */
  {     0,   313,   -51,0, 0x0400,0x0676, 0x49,0xb7,0xb7,0xff }, /*   454 */
  {    20,   319,   -81,0, 0x05c1,0x0781, 0x40,0xa0,0xa0,0xff }, /*   455 */
  {     0,   302,   -81,0, 0x07ff,0x06b4, 0x4b,0xbd,0xbd,0xff }, /*   456 */
  {    20,   319,   -81,0, 0x05c1,0x0781, 0x40,0xa0,0xa0,0xff }, /*   457 */
  {    23,   300,   -84,0, 0x064d,0x06b3, 0x2b,0x6c,0x6c,0xff }, /*   458 */
  {   -57,   201,   -84,0, 0x012b,0x01f1, 0xff,0xff,0xff,0xff }, /*   459 */
  {   -23,   302,  -104,0, 0x011e,0x0749, 0x6b,0xff,0xff,0xff }, /*   460 */
  {     0,   292,  -111,0, 0x0000,0x06f1, 0x66,0xff,0xff,0xff }, /*   461 */
  {   -59,   267,    16,0, 0x02fa,0x0294, 0xf3,0xf3,0xf3,0xff }, /*   462 */
  {   -20,   319,   -81,0, 0x023e,0x0781, 0x5d,0xea,0xea,0xff }, /*   463 */
  {   -23,   300,   -84,0, 0x01b2,0x06b3, 0x70,0xff,0xff,0xff }, /*   464 */
  {    57,   201,   -84,0, 0x06d4,0x01f1, 0xbe,0xbe,0xbe,0xff }, /*   465 */
  {    23,   300,   -84,0, 0x064d,0x06b3, 0x2b,0x6c,0x6c,0xff }, /*   466 */
  {    73,   214,     0,0, 0x059a,0x007a, 0x8e,0x8e,0x8e,0xff }, /*   467 */
  {    73,   214,     0,0, 0x059a,0x007a, 0x8e,0x8e,0x8e,0xff }, /*   468 */
  {    23,   300,   -84,0, 0x064d,0x06b3, 0x2b,0x6c,0x6c,0xff }, /*   469 */
  {    59,   267,    16,0, 0x0505,0x0294, 0x63,0x63,0x63,0xff }, /*   470 */
  {     0,   193,  -119,0, 0x0800,0x0267, 0xff,0xff,0xff,0xff }, /*   471 */
  {     0,   292,  -111,0, 0x07ff,0x06f1, 0x66,0xff,0xff,0xff }, /*   472 */
  {    57,   201,   -84,0, 0x06d4,0x01f1, 0xbe,0xbe,0xbe,0xff }, /*   473 */
  {    23,   302,  -104,0, 0x06e1,0x0749, 0x3a,0x92,0x92,0xff }, /*   474 */
  {     0,   292,  -111,0, 0x07ff,0x06f1, 0x66,0xff,0xff,0xff }, /*   475 */
  {     0,   302,   -81,0, 0x07ff,0x06b4, 0x4b,0xbd,0xbd,0xff }, /*   476 */
  {     0,   275,    27,0, 0x0400,0x02af, 0xba,0xba,0xba,0xff }, /*   477 */
  {     0,   313,   -51,0, 0x0400,0x0676, 0x49,0xb7,0xb7,0xff }, /*   478 */
  {   -59,   267,    16,0, 0x02fa,0x0294, 0xf3,0xf3,0xf3,0xff }, /*   479 */
  {    59,   267,    16,0, 0x0505,0x0294, 0x63,0x63,0x63,0xff }, /*   480 */
  {     0,   313,   -51,0, 0x0400,0x0676, 0x49,0xb7,0xb7,0xff }, /*   481 */
  {     0,   275,    27,0, 0x0400,0x02af, 0xba,0xba,0xba,0xff }, /*   482 */
  {     0,   210,    68,0, 0x0400,0xfe90, 0xf5,0xf5,0xf5,0xff }, /*   483 */
  {   -64,   204,    47,0, 0x02e9,0xfecf, 0xe5,0xe5,0xe5,0xff }, /*   484 */
  {   -42,   204,    49,0, 0x0331,0xfec5, 0xf5,0xf5,0xf5,0xff }, /*   485 */
  {    59,   267,    16,0, 0x0505,0x0294, 0x63,0x63,0x63,0xff }, /*   486 */
  {    51,   229,    48,0, 0x04d7,0xfff9, 0x9e,0x9e,0x9e,0xff }, /*   487 */
  {    73,   214,     0,0, 0x059a,0x007a, 0x8e,0x8e,0x8e,0xff }, /*   488 */
  {   -73,   214,     0,0, 0x0265,0x007a, 0xff,0xff,0xff,0xff }, /*   489 */
  {   -51,   229,    48,0, 0x0328,0xfff9, 0xe6,0xe6,0xe6,0xff }, /*   490 */
  {   -59,   267,    16,0, 0x02fa,0x0294, 0xf3,0xf3,0xf3,0xff }, /*   491 */
  {     0,   302,   -81,0, 0x07ff,0x06b4, 0x4b,0xbd,0xbd,0xff }, /*   492 */
  {    23,   300,   -84,0, 0x064d,0x06b3, 0x2b,0x6c,0x6c,0xff }, /*   493 */
  {    23,   302,  -104,0, 0x06e1,0x0749, 0x3a,0x92,0x92,0xff }, /*   494 */
  {   -20,   319,   -81,0, 0x023e,0x0781, 0x5d,0xea,0xea,0xff }, /*   495 */
  {     0,   313,   -51,0, 0x0400,0x0676, 0x49,0xb7,0xb7,0xff }, /*   496 */
  {     0,   302,   -81,0, 0x07ff,0x06b4, 0x4b,0xbd,0xbd,0xff }, /*   497 */
  {     0,   302,   -81,0, 0x07ff,0x06b4, 0x4b,0xbd,0xbd,0xff }, /*   498 */
  {   -23,   302,  -104,0, 0x011e,0x0749, 0x6b,0xff,0xff,0xff }, /*   499 */
  {   -23,   300,   -84,0, 0x01b2,0x06b3, 0x70,0xff,0xff,0xff }, /*   500 */
  {     0,   302,   -81,0, 0x07ff,0x06b4, 0x4b,0xbd,0xbd,0xff }, /*   501 */
  {     0,   292,  -111,0, 0x07ff,0x06f1, 0x66,0xff,0xff,0xff }, /*   502 */
  {   -23,   302,  -104,0, 0x091e,0x0749, 0x6b,0xff,0xff,0xff }, /*   503 */
  {    59,   267,    16,0, 0x0505,0x0294, 0x63,0x63,0x63,0xff }, /*   504 */
  {    20,   319,   -81,0, 0x05c1,0x0781, 0x40,0xa0,0xa0,0xff }, /*   505 */
  {     0,   313,   -51,0, 0x0400,0x0676, 0x49,0xb7,0xb7,0xff }, /*   506 */
  {     0,   313,   -51,0, 0x0400,0x0676, 0x49,0xb7,0xb7,0xff }, /*   507 */
  {   -20,   319,   -81,0, 0x023e,0x0781, 0x5d,0xea,0xea,0xff }, /*   508 */
  {   -59,   267,    16,0, 0x02fa,0x0294, 0xf3,0xf3,0xf3,0xff }, /*   509 */
  {     0,   292,  -111,0, 0x07ff,0x06f1, 0x66,0xff,0xff,0xff }, /*   510 */
  {    23,   302,  -104,0, 0x06e1,0x0749, 0x3a,0x92,0x92,0xff }, /*   511 */
  {    57,   201,   -84,0, 0x06d4,0x01f1, 0xbe,0xbe,0xbe,0xff }, /*   512 */
  {    23,   300,   -84,0, 0x064d,0x06b3, 0x2b,0x6c,0x6c,0xff }, /*   513 */
  {    20,   319,   -81,0, 0x05c1,0x0781, 0x40,0xa0,0xa0,0xff }, /*   514 */
  {    59,   267,    16,0, 0x0505,0x0294, 0x63,0x63,0x63,0xff }, /*   515 */
  {    57,   201,   -84,0, 0x06d4,0x01f1, 0xbe,0xbe,0xbe,0xff }, /*   516 */
  {    23,   302,  -104,0, 0x06e1,0x0749, 0x3a,0x92,0x92,0xff }, /*   517 */
  {    23,   300,   -84,0, 0x064d,0x06b3, 0x2b,0x6c,0x6c,0xff }, /*   518 */
  {     0,   210,    68,0, 0x0400,0xfe90, 0xf5,0xf5,0xf5,0xff }, /*   519 */
  {     0,   210,    49,0, 0x0400,0xff09, 0xf7,0xf7,0xf7,0xff }, /*   520 */
  {    42,   204,    49,0, 0x04ce,0xfec5, 0xec,0xec,0xec,0xff }, /*   521 */
  {    36,   132,    52,0, 0x077e,0x0000, 0xd0,0xd0,0xd0,0xff }, /*   522 */
  {     0,   170,    73,0, 0x0400,0x07ff, 0xe4,0xb6,0x89,0xff }, /*   523 */
  {   -36,   132,    52,0, 0x0081,0x0000, 0xe5,0xe5,0xe5,0xff }, /*   524 */
  {     0,   170,    73,0, 0x0400,0x07ff, 0xe4,0xb6,0x89,0xff }, /*   525 */
  {   -42,   170,    65,0, 0x0000,0x0755, 0xfc,0xa5,0x97,0xff }, /*   526 */
  {   -36,   132,    52,0, 0x0081,0x0000, 0xe5,0xe5,0xe5,0xff }, /*   527 */
  {    36,   132,    52,0, 0x077e,0x0000, 0xd0,0xd0,0xd0,0xff }, /*   528 */
  {    42,   170,    65,0, 0x0800,0x0755, 0xba,0x79,0x6f,0xff }, /*   529 */
  {     0,   170,    73,0, 0x0400,0x07ff, 0xe4,0xb6,0x89,0xff }, /*   530 */
  {   -42,   170,    65,0, 0xf800,0x0000, 0xfc,0xa5,0x97,0xff }, /*   531 */
  {     0,   210,    49,0, 0x0000,0x0800, 0xff,0xff,0xff,0xff }, /*   532 */
  {   -42,   204,    49,0, 0xf800,0x06e5, 0xff,0xff,0xff,0xff }, /*   533 */
  {    42,   170,    65,0, 0x0800,0x0000, 0xba,0x79,0x6f,0xff }, /*   534 */
  {    42,   204,    49,0, 0x0800,0x06e5, 0xff,0xff,0xff,0xff }, /*   535 */
  {     0,   210,    49,0, 0x0000,0x0800, 0xff,0xff,0xff,0xff }, /*   536 */
  {   -42,   170,    65,0, 0xf800,0x0000, 0xfc,0xa5,0x97,0xff }, /*   537 */
  {     0,   170,    73,0, 0x0000,0x0000, 0xe4,0xb6,0x89,0xff }, /*   538 */
  {     0,   210,    49,0, 0x0000,0x0800, 0xff,0xff,0xff,0xff }, /*   539 */
  {     0,   170,    73,0, 0x0000,0x0000, 0xe4,0xb6,0x89,0xff }, /*   540 */
  {    42,   170,    65,0, 0x0800,0x0000, 0xba,0x79,0x6f,0xff }, /*   541 */
  {     0,   210,    49,0, 0x0000,0x0800, 0xff,0xff,0xff,0xff }, /*   542 */
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
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP1Triangle(  3,  4,  5,  0),
  gsSP1Triangle(  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP2Triangles( 15, 16, 17,  0, 18, 19, 20,  0),
  gsSP2Triangles( 21, 22, 23,  0, 24, 25, 26,  0),
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
  gsSPVertex( SEGADRS(5, 1024), 31, 0 ),
  gsSP1Triangle( 31,  0,  1,  0),
  gsSP1Triangle(  2,  3,  4,  0),
  gsSP1Triangle(  5,  6,  7,  0),
  gsSP1Triangle(  8,  9, 10,  0),
  gsSP2Triangles( 11, 12, 13,  0, 14, 15, 16,  0),
  gsSP1Triangle( 17, 18, 19,  0),
  gsSP1Triangle( 20, 21, 22,  0),
  gsSP1Triangle( 23, 24, 25,  0),
  gsSP1Triangle( 26, 27, 28,  0),
  gsSPVertex( SEGADRS(5, 1520), 1, 31 ),
  gsSP1Triangle( 29, 30, 31,  0),
  gsSPVertex( SEGADRS(5, 1536), 29, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP2Triangles(  3,  4,  5,  0,  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsSP2Triangles( 12, 13, 14,  0, 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSPVertex( SEGADRS(5, 2000), 3, 29 ),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 2048), 30, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP1Triangle(  1,  2,  3,  0),
  gsSP2Triangles(  4,  5,  6,  0,  7,  8,  9,  0),
  gsSP1Triangle( 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsSP1Triangle( 16, 17, 18,  0),
  gsSP2Triangles( 19, 20, 21,  0, 22, 23, 24,  0),
  gsSP1Triangle( 25, 26, 27,  0),
  gsSPVertex( SEGADRS(5, 2528), 2, 30 ),
  gsSP1Triangle( 28, 29, 30,  0),
  gsSPVertex( SEGADRS(5, 2560), 31, 0 ),
  gsSP1Triangle( 31,  0,  1,  0),
  gsSP1Triangle(  2,  3,  4,  0),
  gsSP1Triangle(  5,  6,  7,  0),
  gsSP1Triangle(  8,  9, 10,  0),
  gsSP2Triangles( 11, 12, 13,  0, 14, 15, 16,  0),
  gsSP1Triangle( 17, 18, 19,  0),
  gsSP1Triangle( 20, 21, 22,  0),
  gsSP1Triangle( 23, 24, 25,  0),
  gsSP1Triangle( 26, 27, 28,  0),
  gsSPVertex( SEGADRS(5, 3056), 1, 31 ),
  gsSP1Triangle( 29, 30, 31,  0),
  gsSPVertex( SEGADRS(5, 3072), 29, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP2Triangles(  3,  4,  5,  0,  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSPVertex( SEGADRS(5, 3536), 3, 29 ),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 3584), 4, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP1Triangle(  1,  2,  3,  0),
  gsSPClearGeometryMode( G_SHADING_SMOOTH ),
  gsSPVertex( SEGADRS(5, 3648), 28, 4 ),
  gsSP1Triangle(  4,  5,  6,  0),
  gsSP2Triangles(  7,  8,  9,  0, 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsSP1Triangle( 16, 17, 18,  0),
  gsSP2Triangles( 19, 20, 21,  0, 22, 23, 24,  0),
  gsSP2Triangles( 25, 26, 27,  0, 28, 29, 30,  0),
  gsSPVertex( SEGADRS(5, 4096), 26, 0 ),
  gsSP1Triangle( 31,  0,  1,  0),
  gsSP1Triangle(  2,  3,  4,  0),
  gsSP1Triangle(  5,  6,  7,  0),
  gsSP1Triangle(  8,  9, 10,  0),
  gsSP1Triangle( 11, 12, 13,  0),
  gsSP1Triangle( 14, 15, 16,  0),
  gsSP2Triangles( 17, 18, 19,  0, 20, 21, 22,  0),
  gsSP1Triangle( 23, 24, 25,  0),
  gsSPSetGeometryMode( G_SHADING_SMOOTH ),
  gsSPVertex( SEGADRS(5, 4512), 6, 26 ),
  gsSP1Triangle( 26, 27, 28,  0),
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
  gsSP1Triangle(  1,  2,  3,  0),
  gsSP1Triangle(  4,  5,  6,  0),
  gsSP1Triangle(  7,  8,  9,  0),
  gsSP1Triangle( 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsSP1Triangle( 16, 17, 18,  0),
  gsSP1Triangle( 19, 20, 21,  0),
  gsSP1Triangle( 22, 23, 24,  0),
  gsSP1Triangle( 25, 26, 27,  0),
  gsSPVertex( SEGADRS(5, 5600), 2, 30 ),
  gsSP1Triangle( 28, 29, 30,  0),
  gsSPVertex( SEGADRS(5, 5632), 8, 0 ),
  gsSP1Triangle( 31,  0,  1,  0),
  gsSP1Triangle(  2,  3,  4,  0),
  gsSP1Triangle(  5,  6,  7,  0),
  gsSPClearGeometryMode( G_SHADING_SMOOTH ),
  gsSPVertex( SEGADRS(5, 5760), 24, 8 ),
  gsSP1Triangle(  8,  9, 10,  0),
  gsSP2Triangles( 11, 12, 13,  0, 14, 15, 16,  0),
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
  gsSP1Triangle( 22, 23, 24,  0),
  gsSP1Triangle( 25, 26, 27,  0),
  gsSPVertex( SEGADRS(5, 7136), 2, 30 ),
  gsSP1Triangle( 28, 29, 30,  0),
  gsSPVertex( SEGADRS(5, 7168), 31, 0 ),
  gsSP2Triangles( 31,  0,  1,  0,  2,  3,  4,  0),
  gsSP2Triangles(  5,  6,  7,  0,  8,  9, 10,  0),
  gsSP2Triangles( 11, 12, 13,  0, 14, 15, 16,  0),
  gsSP1Triangle( 17, 18, 19,  0),
  gsSP1Triangle( 20, 21, 22,  0),
  gsSP1Triangle( 23, 24, 25,  0),
  gsSP1Triangle( 26, 27, 28,  0),
  gsSPVertex( SEGADRS(5, 7664), 1, 31 ),
  gsSP1Triangle( 29, 30, 31,  0),
  gsSPVertex( SEGADRS(5, 7680), 29, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP1Triangle(  3,  4,  5,  0),
  gsSP1Triangle(  6,  7,  8,  0),
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
  gsSP2Triangles( 10, 11, 12,  0, 13, 14, 15,  0),
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
  bmp_frake_down,
  tlut_frake_down,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_down_aligner11[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_328_[] = {
  {    38,   -53,     3,0, 0x0000,0x0000, 0xff,0xbc,0x87,0xff }, /*     0 */
  {    38,   -75,     3,0, 0x0000,0x0000, 0xff,0xe7,0xa6,0xff }, /*     1 */
  {    38,   -53,   -22,0, 0x0000,0x0000, 0xff,0xc9,0x90,0xff }, /*     2 */
  {    38,   -75,     3,0, 0x0000,0x0000, 0xff,0xe7,0xa6,0xff }, /*     3 */
  {    38,   -75,   -22,0, 0x0000,0x0000, 0xff,0xbe,0x88,0xff }, /*     4 */
  {    38,   -53,   -22,0, 0x0000,0x0000, 0xff,0xc9,0x90,0xff }, /*     5 */
  {    38,   -53,   -22,0, 0x0000,0x0000, 0xff,0xc9,0x90,0xff }, /*     6 */
  {    38,   -75,   -22,0, 0x0000,0x0000, 0xff,0xbe,0x88,0xff }, /*     7 */
  {    13,   -53,   -22,0, 0x0000,0x0000, 0xff,0xbb,0x86,0xff }, /*     8 */
  {    38,   -75,   -22,0, 0x0000,0x0000, 0xff,0xbe,0x88,0xff }, /*     9 */
  {    13,   -75,   -22,0, 0x0000,0x0000, 0xa7,0x79,0x57,0xff }, /*    10 */
  {    13,   -53,   -22,0, 0x0000,0x0000, 0xff,0xbb,0x86,0xff }, /*    11 */
  {    13,   -53,   -22,0, 0x0000,0x0000, 0xff,0xbb,0x86,0xff }, /*    12 */
  {    13,   -75,     3,0, 0x0000,0x0000, 0xc3,0x8d,0x65,0xff }, /*    13 */
  {    13,   -53,     3,0, 0x0000,0x0000, 0xad,0x7d,0x5a,0xff }, /*    14 */
  {    13,   -53,     3,0, 0x0000,0x0000, 0xad,0x7d,0x5a,0xff }, /*    15 */
  {    13,   -75,     3,0, 0x0000,0x0000, 0xc3,0x8d,0x65,0xff }, /*    16 */
  {    38,   -53,     3,0, 0x0000,0x0000, 0xff,0xbc,0x87,0xff }, /*    17 */
  {    13,   -75,     3,0, 0x0000,0x0000, 0xc3,0x8d,0x65,0xff }, /*    18 */
  {    38,   -75,     3,0, 0x0000,0x0000, 0xff,0xe7,0xa6,0xff }, /*    19 */
  {    38,   -53,     3,0, 0x0000,0x0000, 0xff,0xbc,0x87,0xff }, /*    20 */
  {    13,   -53,   -22,0, 0x0000,0x0000, 0xff,0xbb,0x86,0xff }, /*    21 */
  {    13,   -75,   -22,0, 0x0000,0x0000, 0xa7,0x79,0x57,0xff }, /*    22 */
  {    13,   -75,     3,0, 0x0000,0x0000, 0xc3,0x8d,0x65,0xff }, /*    23 */
  {    38,   -75,   -22,0, 0x0000,0x0000, 0xff,0xbe,0x88,0xff }, /*    24 */
  {    26,   -86,    -9,0, 0x0000,0x0000, 0xfa,0x6c,0x4e,0xff }, /*    25 */
  {    13,   -75,   -22,0, 0x0000,0x0000, 0xa7,0x79,0x57,0xff }, /*    26 */
  {    38,   -75,     3,0, 0x0000,0x0000, 0xff,0xe7,0xa6,0xff }, /*    27 */
  {    26,   -86,    -9,0, 0x0000,0x0000, 0xfa,0x6c,0x4e,0xff }, /*    28 */
  {    38,   -75,   -22,0, 0x0000,0x0000, 0xff,0xbe,0x88,0xff }, /*    29 */
  {    13,   -75,     3,0, 0x0000,0x0000, 0xc3,0x8d,0x65,0xff }, /*    30 */
  {    26,   -86,    -9,0, 0x0000,0x0000, 0xfa,0x6c,0x4e,0xff }, /*    31 */
  {    38,   -75,     3,0, 0x0000,0x0000, 0xff,0xe7,0xa6,0xff }, /*    32 */
  {    13,   -75,   -22,0, 0x0000,0x0000, 0xa7,0x79,0x57,0xff }, /*    33 */
  {    26,   -86,    -9,0, 0x0000,0x0000, 0xfa,0x6c,0x4e,0xff }, /*    34 */
  {    13,   -75,     3,0, 0x0000,0x0000, 0xc3,0x8d,0x65,0xff }, /*    35 */
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
static NDR_SHAPE shape_Shape_of___POLYHEDRON_328_ = {
  gfx_Shape_of___POLYHEDRON_328_,
  vtx_Shape_of___POLYHEDRON_328_, 36, 5,
  bmp_frake_down,
  tlut_frake_down,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_down_aligner12[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_332_[] = {
  {    38,  -104,     3,0, 0x0000,0x0000, 0xff,0xd0,0x96,0xff }, /*     0 */
  {    38,   -80,     3,0, 0x0000,0x0000, 0xff,0xe5,0xa5,0xff }, /*     1 */
  {    13,  -104,     3,0, 0x0000,0x0000, 0x9f,0x73,0x52,0xff }, /*     2 */
  {    38,   -80,     3,0, 0x0000,0x0000, 0xff,0xe5,0xa5,0xff }, /*     3 */
  {    13,   -80,     3,0, 0x0000,0x0000, 0xb7,0x84,0x5f,0xff }, /*     4 */
  {    13,  -104,     3,0, 0x0000,0x0000, 0x9f,0x73,0x52,0xff }, /*     5 */
  {    13,  -104,   -22,0, 0x0000,0x0000, 0xd8,0x9c,0x70,0xff }, /*     6 */
  {    13,   -80,   -22,0, 0x0000,0x0000, 0xc1,0x8c,0x64,0xff }, /*     7 */
  {    38,  -104,   -22,0, 0x0000,0x0000, 0xff,0xbf,0x89,0xff }, /*     8 */
  {    13,   -80,     3,0, 0x0000,0x0000, 0xb7,0x84,0x5f,0xff }, /*     9 */
  {    13,   -80,   -22,0, 0x0000,0x0000, 0xc1,0x8c,0x64,0xff }, /*    10 */
  {    13,  -104,     3,0, 0x0000,0x0000, 0x9f,0x73,0x52,0xff }, /*    11 */
  {    38,   -80,   -22,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    12 */
  {    38,   -80,     3,0, 0x0000,0x0000, 0xff,0xe5,0xa5,0xff }, /*    13 */
  {    38,  -104,   -22,0, 0x0000,0x0000, 0xff,0xbf,0x89,0xff }, /*    14 */
  {    38,   -80,     3,0, 0x0000,0x0000, 0xff,0xe5,0xa5,0xff }, /*    15 */
  {    26,   -67,    -9,0, 0x0000,0x0000, 0xc7,0x56,0x3e,0xff }, /*    16 */
  {    13,   -80,     3,0, 0x0000,0x0000, 0xb7,0x84,0x5f,0xff }, /*    17 */
  {    38,   -80,   -22,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    18 */
  {    26,   -67,    -9,0, 0x0000,0x0000, 0xc7,0x56,0x3e,0xff }, /*    19 */
  {    38,   -80,     3,0, 0x0000,0x0000, 0xff,0xe5,0xa5,0xff }, /*    20 */
  {    13,   -80,   -22,0, 0x0000,0x0000, 0xc1,0x8c,0x64,0xff }, /*    21 */
  {    26,   -67,    -9,0, 0x0000,0x0000, 0xc7,0x56,0x3e,0xff }, /*    22 */
  {    38,   -80,   -22,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    23 */
  {    13,   -80,     3,0, 0x0000,0x0000, 0xb7,0x84,0x5f,0xff }, /*    24 */
  {    26,   -67,    -9,0, 0x0000,0x0000, 0xc7,0x56,0x3e,0xff }, /*    25 */
  {    13,   -80,   -22,0, 0x0000,0x0000, 0xc1,0x8c,0x64,0xff }, /*    26 */
  {    38,  -104,     3,0, 0x0000,0x0000, 0xb3,0x59,0x00,0xff }, /*    27 */
  {    38,  -138,     3,0, 0x0000,0x0000, 0x8e,0x47,0x00,0xff }, /*    28 */
  {    38,  -104,   -22,0, 0x0000,0x0000, 0xa4,0x52,0x00,0xff }, /*    29 */
  {    38,  -138,     3,0, 0x0000,0x0000, 0x8e,0x47,0x00,0xff }, /*    30 */
  {    38,  -138,   -22,0, 0x0000,0x0000, 0xac,0x56,0x00,0xff }, /*    31 */
  {    38,  -104,   -22,0, 0x0000,0x0000, 0xa4,0x52,0x00,0xff }, /*    32 */
  {    38,  -104,   -22,0, 0x0000,0x0000, 0xa4,0x52,0x00,0xff }, /*    33 */
  {    38,  -138,   -22,0, 0x0000,0x0000, 0xac,0x56,0x00,0xff }, /*    34 */
  {    13,  -104,   -22,0, 0x0000,0x0000, 0x86,0x43,0x00,0xff }, /*    35 */
  {    38,  -138,   -22,0, 0x0000,0x0000, 0xac,0x56,0x00,0xff }, /*    36 */
  {    13,  -138,   -22,0, 0x0000,0x0000, 0x9e,0x4f,0x00,0xff }, /*    37 */
  {    13,  -104,   -22,0, 0x0000,0x0000, 0x86,0x43,0x00,0xff }, /*    38 */
  {    13,  -104,   -22,0, 0x0000,0x0000, 0x86,0x43,0x00,0xff }, /*    39 */
  {    13,  -138,     3,0, 0x0000,0x0000, 0x69,0x34,0x00,0xff }, /*    40 */
  {    13,  -104,     3,0, 0x0000,0x0000, 0x62,0x31,0x00,0xff }, /*    41 */
  {    13,  -104,     3,0, 0x0000,0x0000, 0x62,0x31,0x00,0xff }, /*    42 */
  {    13,  -138,     3,0, 0x0000,0x0000, 0x69,0x34,0x00,0xff }, /*    43 */
  {    38,  -104,     3,0, 0x0000,0x0000, 0xb3,0x59,0x00,0xff }, /*    44 */
  {    13,  -138,     3,0, 0x0000,0x0000, 0x69,0x34,0x00,0xff }, /*    45 */
  {    38,  -138,     3,0, 0x0000,0x0000, 0x8e,0x47,0x00,0xff }, /*    46 */
  {    38,  -104,     3,0, 0x0000,0x0000, 0xb3,0x59,0x00,0xff }, /*    47 */
  {    13,  -104,   -22,0, 0x0000,0x0000, 0x86,0x43,0x00,0xff }, /*    48 */
  {    13,  -138,   -22,0, 0x0000,0x0000, 0x9e,0x4f,0x00,0xff }, /*    49 */
  {    13,  -138,     3,0, 0x0000,0x0000, 0x69,0x34,0x00,0xff }, /*    50 */
  {    38,   -80,     3,0, 0x0000,0x0000, 0xff,0xe5,0xa5,0xff }, /*    51 */
  {    38,  -104,     3,0, 0x0000,0x0000, 0xff,0xd0,0x96,0xff }, /*    52 */
  {    38,  -104,   -22,0, 0x0000,0x0000, 0xff,0xbf,0x89,0xff }, /*    53 */
  {    13,   -80,   -22,0, 0x0000,0x0000, 0xc1,0x8c,0x64,0xff }, /*    54 */
  {    13,  -104,   -22,0, 0x0000,0x0000, 0xd8,0x9c,0x70,0xff }, /*    55 */
  {    13,  -104,     3,0, 0x0000,0x0000, 0x9f,0x73,0x52,0xff }, /*    56 */
  {    13,   -80,   -22,0, 0x0000,0x0000, 0xc1,0x8c,0x64,0xff }, /*    57 */
  {    38,   -80,   -22,0, 0x0000,0x0000, 0xff,0xc6,0x8e,0xff }, /*    58 */
  {    38,  -104,   -22,0, 0x0000,0x0000, 0xff,0xbf,0x89,0xff }, /*    59 */
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
  gsSP1Triangle(  6,  7,  8,  0),
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
static NDR_SHAPE shape_Shape_of___POLYHEDRON_332_ = {
  gfx_Shape_of___POLYHEDRON_332_,
  vtx_Shape_of___POLYHEDRON_332_, 60, 5,
  bmp_frake_down,
  tlut_frake_down,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_down_aligner13[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_336_[] = {
  {    10,  -129,   -24,0, 0x0000,0x0000, 0x7a,0x3d,0x00,0xff }, /*     0 */
  {    44,  -151,   -30,0, 0x0000,0x0000, 0x9b,0x4d,0x00,0xff }, /*     1 */
  {     5,  -151,   -23,0, 0x0000,0x0000, 0x97,0x4b,0x00,0xff }, /*     2 */
  {    55,  -119,     8,0, 0x0000,0x0000, 0xa1,0x50,0x00,0xff }, /*     3 */
  {    10,  -119,    16,0, 0x0000,0x0000, 0x80,0x40,0x00,0xff }, /*     4 */
  {    52,  -109,    39,0, 0x0000,0x0000, 0xaa,0x55,0x00,0xff }, /*     5 */
  {    10,  -119,    16,0, 0x0000,0x0000, 0x80,0x40,0x00,0xff }, /*     6 */
  {    18,  -109,    45,0, 0x0000,0x0000, 0x7b,0x3d,0x00,0xff }, /*     7 */
  {    52,  -109,    39,0, 0x0000,0x0000, 0xaa,0x55,0x00,0xff }, /*     8 */
  {    59,  -151,     7,0, 0x0000,0x0000, 0xc1,0x60,0x00,0xff }, /*     9 */
  {    13,  -151,    46,0, 0x0000,0x0000, 0x65,0x32,0x00,0xff }, /*    10 */
  {     5,  -151,    17,0, 0x0000,0x0000, 0x65,0x32,0x00,0xff }, /*    11 */
  {     5,  -151,    17,0, 0x0000,0x0000, 0x65,0x32,0x00,0xff }, /*    12 */
  {    44,  -151,   -30,0, 0x0000,0x0000, 0x9b,0x4d,0x00,0xff }, /*    13 */
  {    59,  -151,     7,0, 0x0000,0x0000, 0xc1,0x60,0x00,0xff }, /*    14 */
  {    18,  -109,    45,0, 0x0000,0x0000, 0x7b,0x3d,0x00,0xff }, /*    15 */
  {    10,  -119,    16,0, 0x0000,0x0000, 0x80,0x40,0x00,0xff }, /*    16 */
  {    13,  -151,    46,0, 0x0000,0x0000, 0x65,0x32,0x00,0xff }, /*    17 */
  {    55,  -119,     8,0, 0x0000,0x0000, 0xa1,0x50,0x00,0xff }, /*    18 */
  {    65,  -151,    37,0, 0x0000,0x0000, 0xac,0x56,0x00,0xff }, /*    19 */
  {    59,  -151,     7,0, 0x0000,0x0000, 0xc1,0x60,0x00,0xff }, /*    20 */
  {    13,  -151,    46,0, 0x0000,0x0000, 0x65,0x32,0x00,0xff }, /*    21 */
  {    39,  -138,    62,0, 0x0000,0x0000, 0x9c,0x4e,0x00,0xff }, /*    22 */
  {    18,  -109,    45,0, 0x0000,0x0000, 0x7b,0x3d,0x00,0xff }, /*    23 */
  {    65,  -151,    37,0, 0x0000,0x0000, 0xac,0x56,0x00,0xff }, /*    24 */
  {    39,  -138,    62,0, 0x0000,0x0000, 0x9c,0x4e,0x00,0xff }, /*    25 */
  {    13,  -151,    46,0, 0x0000,0x0000, 0x65,0x32,0x00,0xff }, /*    26 */
  {    52,  -109,    39,0, 0x0000,0x0000, 0xaa,0x55,0x00,0xff }, /*    27 */
  {    39,  -138,    62,0, 0x0000,0x0000, 0x9c,0x4e,0x00,0xff }, /*    28 */
  {    65,  -151,    37,0, 0x0000,0x0000, 0xac,0x56,0x00,0xff }, /*    29 */
  {    18,  -109,    45,0, 0x0000,0x0000, 0x7b,0x3d,0x00,0xff }, /*    30 */
  {    39,  -138,    62,0, 0x0000,0x0000, 0x9c,0x4e,0x00,0xff }, /*    31 */
  {    52,  -109,    39,0, 0x0000,0x0000, 0xaa,0x55,0x00,0xff }, /*    32 */
  {    55,  -119,     8,0, 0x0000,0x0000, 0xa1,0x50,0x00,0xff }, /*    33 */
  {    10,  -129,   -24,0, 0x0000,0x0000, 0x7a,0x3d,0x00,0xff }, /*    34 */
  {    10,  -119,    16,0, 0x0000,0x0000, 0x80,0x40,0x00,0xff }, /*    35 */
  {    10,  -119,    16,0, 0x0000,0x0000, 0x80,0x40,0x00,0xff }, /*    36 */
  {     5,  -151,   -23,0, 0x0000,0x0000, 0x97,0x4b,0x00,0xff }, /*    37 */
  {     5,  -151,    17,0, 0x0000,0x0000, 0x65,0x32,0x00,0xff }, /*    38 */
  {    59,  -151,     7,0, 0x0000,0x0000, 0xc1,0x60,0x00,0xff }, /*    39 */
  {    41,  -129,   -29,0, 0x0000,0x0000, 0xb8,0x5c,0x00,0xff }, /*    40 */
  {    55,  -119,     8,0, 0x0000,0x0000, 0xa1,0x50,0x00,0xff }, /*    41 */
  {    59,  -151,     7,0, 0x0000,0x0000, 0xc1,0x60,0x00,0xff }, /*    42 */
  {    44,  -151,   -30,0, 0x0000,0x0000, 0x9b,0x4d,0x00,0xff }, /*    43 */
  {    41,  -129,   -29,0, 0x0000,0x0000, 0xb8,0x5c,0x00,0xff }, /*    44 */
  {     5,  -151,    17,0, 0x0000,0x0000, 0x65,0x32,0x00,0xff }, /*    45 */
  {     5,  -151,   -23,0, 0x0000,0x0000, 0x97,0x4b,0x00,0xff }, /*    46 */
  {    44,  -151,   -30,0, 0x0000,0x0000, 0x9b,0x4d,0x00,0xff }, /*    47 */
  {    10,  -119,    16,0, 0x0000,0x0000, 0x80,0x40,0x00,0xff }, /*    48 */
  {    10,  -129,   -24,0, 0x0000,0x0000, 0x7a,0x3d,0x00,0xff }, /*    49 */
  {     5,  -151,   -23,0, 0x0000,0x0000, 0x97,0x4b,0x00,0xff }, /*    50 */
  {    10,  -119,    16,0, 0x0000,0x0000, 0x80,0x40,0x00,0xff }, /*    51 */
  {     5,  -151,    17,0, 0x0000,0x0000, 0x65,0x32,0x00,0xff }, /*    52 */
  {    13,  -151,    46,0, 0x0000,0x0000, 0x65,0x32,0x00,0xff }, /*    53 */
  {    55,  -119,     8,0, 0x0000,0x0000, 0xa1,0x50,0x00,0xff }, /*    54 */
  {    41,  -129,   -29,0, 0x0000,0x0000, 0xb8,0x5c,0x00,0xff }, /*    55 */
  {    10,  -129,   -24,0, 0x0000,0x0000, 0x7a,0x3d,0x00,0xff }, /*    56 */
  {    55,  -119,     8,0, 0x0000,0x0000, 0xa1,0x50,0x00,0xff }, /*    57 */
  {    52,  -109,    39,0, 0x0000,0x0000, 0xaa,0x55,0x00,0xff }, /*    58 */
  {    65,  -151,    37,0, 0x0000,0x0000, 0xac,0x56,0x00,0xff }, /*    59 */
  {    59,  -151,     7,0, 0x0000,0x0000, 0xc1,0x60,0x00,0xff }, /*    60 */
  {    65,  -151,    37,0, 0x0000,0x0000, 0xac,0x56,0x00,0xff }, /*    61 */
  {    13,  -151,    46,0, 0x0000,0x0000, 0x65,0x32,0x00,0xff }, /*    62 */
  {    10,  -129,   -24,0, 0x0000,0x0000, 0x7a,0x3d,0x00,0xff }, /*    63 */
  {    41,  -129,   -29,0, 0x0000,0x0000, 0xb8,0x5c,0x00,0xff }, /*    64 */
  {    44,  -151,   -30,0, 0x0000,0x0000, 0x9b,0x4d,0x00,0xff }, /*    65 */
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
  gsSP1Triangle(  1,  2,  3,  0),
  gsSP1Triangle(  4,  5,  6,  0),
  gsSP1Triangle(  7,  8,  9,  0),
  gsSP1Triangle( 10, 11, 12,  0),
  gsSP1Triangle( 13, 14, 15,  0),
  gsSP2Triangles( 16, 17, 18,  0, 19, 20, 21,  0),
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
  bmp_frake_down,
  tlut_frake_down,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_down_aligner14[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_324_[] = {
  {    46,   -16,    12,0, 0x0000,0x0000, 0x00,0x58,0x00,0xff }, /*     0 */
  {     0,   -70,    15,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*     1 */
  {    53,   -70,    15,0, 0x0000,0x0000, 0x00,0x6a,0x00,0xff }, /*     2 */
  {    46,   -16,    12,0, 0x0000,0x0000, 0x00,0x58,0x00,0xff }, /*     3 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x3c,0x00,0xff }, /*     4 */
  {     0,   -70,    15,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*     5 */
  {     0,    -9,   -35,0, 0x0000,0x0000, 0x00,0x53,0x00,0xff }, /*     6 */
  {    45,   -16,   -35,0, 0x0000,0x0000, 0x00,0x63,0x00,0xff }, /*     7 */
  {     0,   -70,   -37,0, 0x0000,0x0000, 0x00,0x39,0x00,0xff }, /*     8 */
  {    45,   -16,   -35,0, 0x0000,0x0000, 0x00,0x63,0x00,0xff }, /*     9 */
  {    53,   -70,   -37,0, 0x0000,0x0000, 0x00,0x52,0x00,0xff }, /*    10 */
  {     0,   -70,   -37,0, 0x0000,0x0000, 0x00,0x39,0x00,0xff }, /*    11 */
  {    24,     7,   -24,0, 0x0000,0x0000, 0x00,0x5c,0x00,0xff }, /*    12 */
  {     8,     7,   -24,0, 0x0000,0x0000, 0x00,0x4f,0x00,0xff }, /*    13 */
  {    24,     7,    -3,0, 0x0000,0x0000, 0x00,0x61,0x00,0xff }, /*    14 */
  {     8,     7,   -24,0, 0x0000,0x0000, 0x00,0x4f,0x00,0xff }, /*    15 */
  {     8,     7,    -3,0, 0x0000,0x0000, 0x00,0x48,0x00,0xff }, /*    16 */
  {    24,     7,    -3,0, 0x0000,0x0000, 0x00,0x61,0x00,0xff }, /*    17 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x3c,0x00,0xff }, /*    18 */
  {     0,   -70,   -37,0, 0x0000,0x0000, 0x00,0x39,0x00,0xff }, /*    19 */
  {     0,   -70,    15,0, 0x0000,0x0000, 0x00,0x3e,0x00,0xff }, /*    20 */
  {    45,   -16,   -35,0, 0x0000,0x0000, 0x00,0x63,0x00,0xff }, /*    21 */
  {    46,   -16,    12,0, 0x0000,0x0000, 0x00,0x58,0x00,0xff }, /*    22 */
  {    53,   -70,   -37,0, 0x0000,0x0000, 0x00,0x52,0x00,0xff }, /*    23 */
  {    46,   -16,    12,0, 0x0000,0x0000, 0x00,0x58,0x00,0xff }, /*    24 */
  {    24,     7,    -3,0, 0x0000,0x0000, 0x00,0x61,0x00,0xff }, /*    25 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x3c,0x00,0xff }, /*    26 */
  {    45,   -16,   -35,0, 0x0000,0x0000, 0x00,0x63,0x00,0xff }, /*    27 */
  {    24,     7,   -24,0, 0x0000,0x0000, 0x00,0x5c,0x00,0xff }, /*    28 */
  {    46,   -16,    12,0, 0x0000,0x0000, 0x00,0x58,0x00,0xff }, /*    29 */
  {     0,    -9,   -35,0, 0x0000,0x0000, 0x00,0x53,0x00,0xff }, /*    30 */
  {    24,     7,   -24,0, 0x0000,0x0000, 0x00,0x5c,0x00,0xff }, /*    31 */
  {    45,   -16,   -35,0, 0x0000,0x0000, 0x00,0x63,0x00,0xff }, /*    32 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x3c,0x00,0xff }, /*    33 */
  {     8,     7,   -24,0, 0x0000,0x0000, 0x00,0x4f,0x00,0xff }, /*    34 */
  {     0,    -9,   -35,0, 0x0000,0x0000, 0x00,0x53,0x00,0xff }, /*    35 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x3c,0x00,0xff }, /*    36 */
  {     8,     7,    -3,0, 0x0000,0x0000, 0x00,0x48,0x00,0xff }, /*    37 */
  {     8,     7,   -24,0, 0x0000,0x0000, 0x00,0x4f,0x00,0xff }, /*    38 */
  {     0,    -9,   -35,0, 0x0000,0x0000, 0x00,0x53,0x00,0xff }, /*    39 */
  {     8,     7,   -24,0, 0x0000,0x0000, 0x00,0x4f,0x00,0xff }, /*    40 */
  {    24,     7,   -24,0, 0x0000,0x0000, 0x00,0x5c,0x00,0xff }, /*    41 */
  {    24,     7,   -24,0, 0x0000,0x0000, 0x00,0x5c,0x00,0xff }, /*    42 */
  {    24,     7,    -3,0, 0x0000,0x0000, 0x00,0x61,0x00,0xff }, /*    43 */
  {    46,   -16,    12,0, 0x0000,0x0000, 0x00,0x58,0x00,0xff }, /*    44 */
  {    24,     7,    -3,0, 0x0000,0x0000, 0x00,0x61,0x00,0xff }, /*    45 */
  {     8,     7,    -3,0, 0x0000,0x0000, 0x00,0x48,0x00,0xff }, /*    46 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x3c,0x00,0xff }, /*    47 */
  {    46,   -16,    12,0, 0x0000,0x0000, 0x00,0x58,0x00,0xff }, /*    48 */
  {    53,   -70,    15,0, 0x0000,0x0000, 0x00,0x6a,0x00,0xff }, /*    49 */
  {    53,   -70,   -37,0, 0x0000,0x0000, 0x00,0x52,0x00,0xff }, /*    50 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x3c,0x00,0xff }, /*    51 */
  {     0,    -9,   -35,0, 0x0000,0x0000, 0x00,0x53,0x00,0xff }, /*    52 */
  {     0,   -70,   -37,0, 0x0000,0x0000, 0x00,0x39,0x00,0xff }, /*    53 */
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
  gsSP1Triangle( 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 22, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP2Triangles(  1,  2,  3,  0,  4,  5,  6,  0),
  gsSP1Triangle(  7,  8,  9,  0),
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
  bmp_frake_down,
  tlut_frake_down,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_down_aligner15[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_322_[] = {
  {     5,    -7,    -4,0, 0x0000,0x0000, 0x00,0x3d,0x00,0xff }, /*     0 */
  {   -30,    11,    22,0, 0x0000,0x0000, 0x00,0x44,0x00,0xff }, /*     1 */
  {    -5,    -7,    -4,0, 0x0000,0x0000, 0x00,0x3f,0x00,0xff }, /*     2 */
  {     5,    -7,    -4,0, 0x0000,0x0000, 0x00,0x3d,0x00,0xff }, /*     3 */
  {    30,    11,    22,0, 0x0000,0x0000, 0x00,0x5b,0x00,0xff }, /*     4 */
  {   -30,    11,    22,0, 0x0000,0x0000, 0x00,0x44,0x00,0xff }, /*     5 */
  {    -6,    -7,   -15,0, 0x0000,0x0000, 0x00,0x3f,0x00,0xff }, /*     6 */
  {    30,    11,   -37,0, 0x0000,0x0000, 0x00,0x5d,0x00,0xff }, /*     7 */
  {     6,    -7,   -15,0, 0x0000,0x0000, 0x00,0x4a,0x00,0xff }, /*     8 */
  {    -6,    -7,   -15,0, 0x0000,0x0000, 0x00,0x3f,0x00,0xff }, /*     9 */
  {   -30,    11,   -37,0, 0x0000,0x0000, 0x00,0x4b,0x00,0xff }, /*    10 */
  {    30,    11,   -37,0, 0x0000,0x0000, 0x00,0x5d,0x00,0xff }, /*    11 */
  {   -30,    11,    22,0, 0x0000,0x0000, 0x00,0x44,0x00,0xff }, /*    12 */
  {    30,    11,   -37,0, 0x0000,0x0000, 0x00,0x5d,0x00,0xff }, /*    13 */
  {   -30,    11,   -37,0, 0x0000,0x0000, 0x00,0x4b,0x00,0xff }, /*    14 */
  {   -30,    11,    22,0, 0x0000,0x0000, 0x00,0x44,0x00,0xff }, /*    15 */
  {    30,    11,    22,0, 0x0000,0x0000, 0x00,0x5b,0x00,0xff }, /*    16 */
  {    30,    11,   -37,0, 0x0000,0x0000, 0x00,0x5d,0x00,0xff }, /*    17 */
  {     6,    -7,   -15,0, 0x0000,0x0000, 0x00,0x4a,0x00,0xff }, /*    18 */
  {    30,    11,    22,0, 0x0000,0x0000, 0x00,0x5b,0x00,0xff }, /*    19 */
  {     5,    -7,    -4,0, 0x0000,0x0000, 0x00,0x3d,0x00,0xff }, /*    20 */
  {     6,    -7,   -15,0, 0x0000,0x0000, 0x00,0x4a,0x00,0xff }, /*    21 */
  {    30,    11,   -37,0, 0x0000,0x0000, 0x00,0x5d,0x00,0xff }, /*    22 */
  {    30,    11,    22,0, 0x0000,0x0000, 0x00,0x5b,0x00,0xff }, /*    23 */
  {    -5,    -7,    -4,0, 0x0000,0x0000, 0x00,0x3f,0x00,0xff }, /*    24 */
  {   -30,    11,    22,0, 0x0000,0x0000, 0x00,0x44,0x00,0xff }, /*    25 */
  {    -6,    -7,   -15,0, 0x0000,0x0000, 0x00,0x3f,0x00,0xff }, /*    26 */
  {     6,    -7,   -15,0, 0x0000,0x0000, 0x00,0x4a,0x00,0xff }, /*    27 */
  {    -5,    -7,    -4,0, 0x0000,0x0000, 0x00,0x3f,0x00,0xff }, /*    28 */
  {    -6,    -7,   -15,0, 0x0000,0x0000, 0x00,0x3f,0x00,0xff }, /*    29 */
  {     6,    -7,   -15,0, 0x0000,0x0000, 0x00,0x4a,0x00,0xff }, /*    30 */
  {     5,    -7,    -4,0, 0x0000,0x0000, 0x00,0x3d,0x00,0xff }, /*    31 */
  {    -5,    -7,    -4,0, 0x0000,0x0000, 0x00,0x3f,0x00,0xff }, /*    32 */
  {   -30,    11,    22,0, 0x0000,0x0000, 0x00,0x44,0x00,0xff }, /*    33 */
  {   -30,    11,   -37,0, 0x0000,0x0000, 0x00,0x4b,0x00,0xff }, /*    34 */
  {    -6,    -7,   -15,0, 0x0000,0x0000, 0x00,0x3f,0x00,0xff }, /*    35 */
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
  bmp_frake_down,
  tlut_frake_down,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_down_aligner16[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_320_[] = {
  {   -53,   -70,    15,0, 0x0000,0x0000, 0x00,0x36,0x00,0xff }, /*     0 */
  {     0,   -70,    15,0, 0x0000,0x0000, 0x00,0x50,0x00,0xff }, /*     1 */
  {   -46,   -16,    12,0, 0x0000,0x0000, 0x00,0x3a,0x00,0xff }, /*     2 */
  {     0,   -70,   -37,0, 0x0000,0x0000, 0x00,0x54,0x00,0xff }, /*     3 */
  {   -45,   -16,   -35,0, 0x0000,0x0000, 0x00,0x5b,0x00,0xff }, /*     4 */
  {     1,    -9,   -35,0, 0x0000,0x0000, 0x00,0x6d,0x00,0xff }, /*     5 */
  {     0,   -70,   -37,0, 0x0000,0x0000, 0x00,0x54,0x00,0xff }, /*     6 */
  {   -53,   -70,   -37,0, 0x0000,0x0000, 0x00,0x49,0x00,0xff }, /*     7 */
  {   -45,   -16,   -35,0, 0x0000,0x0000, 0x00,0x5b,0x00,0xff }, /*     8 */
  {   -24,     7,    -3,0, 0x0000,0x0000, 0x00,0x49,0x00,0xff }, /*     9 */
  {    -8,     7,   -24,0, 0x0000,0x0000, 0x00,0x66,0x00,0xff }, /*    10 */
  {   -24,     7,   -24,0, 0x0000,0x0000, 0x00,0x61,0x00,0xff }, /*    11 */
  {   -24,     7,    -3,0, 0x0000,0x0000, 0x00,0x49,0x00,0xff }, /*    12 */
  {    -8,     7,    -3,0, 0x0000,0x0000, 0x00,0x51,0x00,0xff }, /*    13 */
  {    -8,     7,   -24,0, 0x0000,0x0000, 0x00,0x66,0x00,0xff }, /*    14 */
  {     0,   -70,    15,0, 0x0000,0x0000, 0x00,0x50,0x00,0xff }, /*    15 */
  {     0,   -70,   -37,0, 0x0000,0x0000, 0x00,0x54,0x00,0xff }, /*    16 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x4c,0x00,0xff }, /*    17 */
  {   -53,   -70,   -37,0, 0x0000,0x0000, 0x00,0x49,0x00,0xff }, /*    18 */
  {   -46,   -16,    12,0, 0x0000,0x0000, 0x00,0x3a,0x00,0xff }, /*    19 */
  {   -45,   -16,   -35,0, 0x0000,0x0000, 0x00,0x5b,0x00,0xff }, /*    20 */
  {   -53,   -70,   -37,0, 0x0000,0x0000, 0x00,0x49,0x00,0xff }, /*    21 */
  {   -53,   -70,    15,0, 0x0000,0x0000, 0x00,0x36,0x00,0xff }, /*    22 */
  {   -46,   -16,    12,0, 0x0000,0x0000, 0x00,0x3a,0x00,0xff }, /*    23 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x4c,0x00,0xff }, /*    24 */
  {   -24,     7,    -3,0, 0x0000,0x0000, 0x00,0x49,0x00,0xff }, /*    25 */
  {   -46,   -16,    12,0, 0x0000,0x0000, 0x00,0x3a,0x00,0xff }, /*    26 */
  {   -46,   -16,    12,0, 0x0000,0x0000, 0x00,0x3a,0x00,0xff }, /*    27 */
  {   -24,     7,   -24,0, 0x0000,0x0000, 0x00,0x61,0x00,0xff }, /*    28 */
  {   -45,   -16,   -35,0, 0x0000,0x0000, 0x00,0x5b,0x00,0xff }, /*    29 */
  {   -46,   -16,    12,0, 0x0000,0x0000, 0x00,0x3a,0x00,0xff }, /*    30 */
  {   -24,     7,    -3,0, 0x0000,0x0000, 0x00,0x49,0x00,0xff }, /*    31 */
  {   -24,     7,   -24,0, 0x0000,0x0000, 0x00,0x61,0x00,0xff }, /*    32 */
  {   -45,   -16,   -35,0, 0x0000,0x0000, 0x00,0x5b,0x00,0xff }, /*    33 */
  {   -24,     7,   -24,0, 0x0000,0x0000, 0x00,0x61,0x00,0xff }, /*    34 */
  {     1,    -9,   -35,0, 0x0000,0x0000, 0x00,0x6d,0x00,0xff }, /*    35 */
  {   -24,     7,   -24,0, 0x0000,0x0000, 0x00,0x61,0x00,0xff }, /*    36 */
  {    -8,     7,   -24,0, 0x0000,0x0000, 0x00,0x66,0x00,0xff }, /*    37 */
  {     1,    -9,   -35,0, 0x0000,0x0000, 0x00,0x6d,0x00,0xff }, /*    38 */
  {     1,    -9,   -35,0, 0x0000,0x0000, 0x00,0x6d,0x00,0xff }, /*    39 */
  {    -8,     7,   -24,0, 0x0000,0x0000, 0x00,0x66,0x00,0xff }, /*    40 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x4c,0x00,0xff }, /*    41 */
  {    -8,     7,   -24,0, 0x0000,0x0000, 0x00,0x66,0x00,0xff }, /*    42 */
  {    -8,     7,    -3,0, 0x0000,0x0000, 0x00,0x51,0x00,0xff }, /*    43 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x4c,0x00,0xff }, /*    44 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x4c,0x00,0xff }, /*    45 */
  {    -8,     7,    -3,0, 0x0000,0x0000, 0x00,0x51,0x00,0xff }, /*    46 */
  {   -24,     7,    -3,0, 0x0000,0x0000, 0x00,0x49,0x00,0xff }, /*    47 */
  {     0,   -70,   -37,0, 0x0000,0x0000, 0x00,0x54,0x00,0xff }, /*    48 */
  {     1,    -9,   -35,0, 0x0000,0x0000, 0x00,0x6d,0x00,0xff }, /*    49 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x4c,0x00,0xff }, /*    50 */
  {     0,   -70,    15,0, 0x0000,0x0000, 0x00,0x50,0x00,0xff }, /*    51 */
  {     0,    -9,    12,0, 0x0000,0x0000, 0x00,0x4c,0x00,0xff }, /*    52 */
  {   -46,   -16,    12,0, 0x0000,0x0000, 0x00,0x3a,0x00,0xff }, /*    53 */
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
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP2Triangles(  1,  2,  3,  0,  4,  5,  6,  0),
  gsSP2Triangles(  7,  8,  9,  0, 10, 11, 12,  0),
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
  bmp_frake_down,
  tlut_frake_down,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_down_aligner17[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_330_[] = {
  {   -13,  -104,     3,0, 0x0000,0x0000, 0xff,0xbc,0x87,0xff }, /*     0 */
  {   -38,   -80,     3,0, 0x0000,0x0000, 0xa5,0x78,0x56,0xff }, /*     1 */
  {   -38,  -104,     3,0, 0x0000,0x0000, 0xc2,0x8c,0x65,0xff }, /*     2 */
  {   -38,  -104,   -22,0, 0x0000,0x0000, 0xff,0xc4,0x8c,0xff }, /*     3 */
  {   -13,   -80,   -22,0, 0x0000,0x0000, 0xff,0xe3,0xa3,0xff }, /*     4 */
  {   -13,  -104,   -22,0, 0x0000,0x0000, 0xff,0xf3,0xaf,0xff }, /*     5 */
  {   -38,  -104,   -22,0, 0x0000,0x0000, 0xff,0xc4,0x8c,0xff }, /*     6 */
  {   -38,   -80,   -22,0, 0x0000,0x0000, 0xf7,0xb2,0x80,0xff }, /*     7 */
  {   -13,   -80,   -22,0, 0x0000,0x0000, 0xff,0xe3,0xa3,0xff }, /*     8 */
  {   -13,  -104,     3,0, 0x0000,0x0000, 0xff,0xbc,0x87,0xff }, /*     9 */
  {   -13,   -80,   -22,0, 0x0000,0x0000, 0xff,0xe3,0xa3,0xff }, /*    10 */
  {   -13,   -80,     3,0, 0x0000,0x0000, 0xf8,0xb3,0x81,0xff }, /*    11 */
  {   -38,  -104,   -22,0, 0x0000,0x0000, 0xff,0xc4,0x8c,0xff }, /*    12 */
  {   -38,   -80,     3,0, 0x0000,0x0000, 0xa5,0x78,0x56,0xff }, /*    13 */
  {   -38,   -80,   -22,0, 0x0000,0x0000, 0xf7,0xb2,0x80,0xff }, /*    14 */
  {   -13,   -80,     3,0, 0x0000,0x0000, 0xf8,0xb3,0x81,0xff }, /*    15 */
  {   -26,   -67,    -9,0, 0x0000,0x0000, 0xc3,0x54,0x3c,0xff }, /*    16 */
  {   -38,   -80,     3,0, 0x0000,0x0000, 0xa5,0x78,0x56,0xff }, /*    17 */
  {   -38,   -80,     3,0, 0x0000,0x0000, 0xa5,0x78,0x56,0xff }, /*    18 */
  {   -26,   -67,    -9,0, 0x0000,0x0000, 0xc3,0x54,0x3c,0xff }, /*    19 */
  {   -38,   -80,   -22,0, 0x0000,0x0000, 0xf7,0xb2,0x80,0xff }, /*    20 */
  {   -38,   -80,   -22,0, 0x0000,0x0000, 0xf7,0xb2,0x80,0xff }, /*    21 */
  {   -26,   -67,    -9,0, 0x0000,0x0000, 0xc3,0x54,0x3c,0xff }, /*    22 */
  {   -13,   -80,   -22,0, 0x0000,0x0000, 0xff,0xe3,0xa3,0xff }, /*    23 */
  {   -13,   -80,   -22,0, 0x0000,0x0000, 0xff,0xe3,0xa3,0xff }, /*    24 */
  {   -26,   -67,    -9,0, 0x0000,0x0000, 0xc3,0x54,0x3c,0xff }, /*    25 */
  {   -13,   -80,     3,0, 0x0000,0x0000, 0xf8,0xb3,0x81,0xff }, /*    26 */
  {   -38,  -104,   -22,0, 0x0000,0x0000, 0xa8,0x54,0x00,0xff }, /*    27 */
  {   -38,  -138,     3,0, 0x0000,0x0000, 0x7f,0x3f,0x00,0xff }, /*    28 */
  {   -38,  -104,     3,0, 0x0000,0x0000, 0x78,0x3c,0x00,0xff }, /*    29 */
  {   -38,  -104,   -22,0, 0x0000,0x0000, 0xa8,0x54,0x00,0xff }, /*    30 */
  {   -38,  -138,   -22,0, 0x0000,0x0000, 0xa9,0x54,0x00,0xff }, /*    31 */
  {   -38,  -138,     3,0, 0x0000,0x0000, 0x7f,0x3f,0x00,0xff }, /*    32 */
  {   -13,  -104,   -22,0, 0x0000,0x0000, 0xd1,0x68,0x00,0xff }, /*    33 */
  {   -38,  -138,   -22,0, 0x0000,0x0000, 0xa9,0x54,0x00,0xff }, /*    34 */
  {   -38,  -104,   -22,0, 0x0000,0x0000, 0xa8,0x54,0x00,0xff }, /*    35 */
  {   -13,  -104,   -22,0, 0x0000,0x0000, 0xd1,0x68,0x00,0xff }, /*    36 */
  {   -13,  -138,   -22,0, 0x0000,0x0000, 0xcc,0x66,0x00,0xff }, /*    37 */
  {   -38,  -138,   -22,0, 0x0000,0x0000, 0xa9,0x54,0x00,0xff }, /*    38 */
  {   -13,  -104,     3,0, 0x0000,0x0000, 0xa2,0x51,0x00,0xff }, /*    39 */
  {   -13,  -138,     3,0, 0x0000,0x0000, 0x98,0x4c,0x00,0xff }, /*    40 */
  {   -13,  -104,   -22,0, 0x0000,0x0000, 0xd1,0x68,0x00,0xff }, /*    41 */
  {   -38,  -104,     3,0, 0x0000,0x0000, 0x78,0x3c,0x00,0xff }, /*    42 */
  {   -13,  -138,     3,0, 0x0000,0x0000, 0x98,0x4c,0x00,0xff }, /*    43 */
  {   -13,  -104,     3,0, 0x0000,0x0000, 0xa2,0x51,0x00,0xff }, /*    44 */
  {   -38,  -104,     3,0, 0x0000,0x0000, 0x78,0x3c,0x00,0xff }, /*    45 */
  {   -38,  -138,     3,0, 0x0000,0x0000, 0x7f,0x3f,0x00,0xff }, /*    46 */
  {   -13,  -138,     3,0, 0x0000,0x0000, 0x98,0x4c,0x00,0xff }, /*    47 */
  {   -13,  -138,     3,0, 0x0000,0x0000, 0x98,0x4c,0x00,0xff }, /*    48 */
  {   -13,  -138,   -22,0, 0x0000,0x0000, 0xcc,0x66,0x00,0xff }, /*    49 */
  {   -13,  -104,   -22,0, 0x0000,0x0000, 0xd1,0x68,0x00,0xff }, /*    50 */
  {   -38,  -104,   -22,0, 0x0000,0x0000, 0xff,0xc4,0x8c,0xff }, /*    51 */
  {   -38,  -104,     3,0, 0x0000,0x0000, 0xc2,0x8c,0x65,0xff }, /*    52 */
  {   -38,   -80,     3,0, 0x0000,0x0000, 0xa5,0x78,0x56,0xff }, /*    53 */
  {   -13,  -104,     3,0, 0x0000,0x0000, 0xff,0xbc,0x87,0xff }, /*    54 */
  {   -13,  -104,   -22,0, 0x0000,0x0000, 0xff,0xf3,0xaf,0xff }, /*    55 */
  {   -13,   -80,   -22,0, 0x0000,0x0000, 0xff,0xe3,0xa3,0xff }, /*    56 */
  {   -13,  -104,     3,0, 0x0000,0x0000, 0xff,0xbc,0x87,0xff }, /*    57 */
  {   -13,   -80,     3,0, 0x0000,0x0000, 0xf8,0xb3,0x81,0xff }, /*    58 */
  {   -38,   -80,     3,0, 0x0000,0x0000, 0xa5,0x78,0x56,0xff }, /*    59 */
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
  bmp_frake_down,
  tlut_frake_down,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_down_aligner18[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_334_[] = {
  {    -5,  -151,   -23,0, 0x0000,0x0000, 0xcd,0x66,0x00,0xff }, /*     0 */
  {   -44,  -151,   -30,0, 0x0000,0x0000, 0xad,0x56,0x00,0xff }, /*     1 */
  {   -10,  -129,   -24,0, 0x0000,0x0000, 0x95,0x4a,0x00,0xff }, /*     2 */
  {   -52,  -109,    39,0, 0x0000,0x0000, 0x70,0x38,0x00,0xff }, /*     3 */
  {   -10,  -119,    16,0, 0x0000,0x0000, 0x95,0x4a,0x00,0xff }, /*     4 */
  {   -55,  -119,     8,0, 0x0000,0x0000, 0x7a,0x3d,0x00,0xff }, /*     5 */
  {   -10,  -129,   -24,0, 0x0000,0x0000, 0x95,0x4a,0x00,0xff }, /*     6 */
  {   -41,  -129,   -29,0, 0x0000,0x0000, 0x7f,0x3f,0x00,0xff }, /*     7 */
  {   -55,  -119,     8,0, 0x0000,0x0000, 0x7a,0x3d,0x00,0xff }, /*     8 */
  {   -52,  -109,    39,0, 0x0000,0x0000, 0x70,0x38,0x00,0xff }, /*     9 */
  {   -18,  -109,    45,0, 0x0000,0x0000, 0x8c,0x46,0x00,0xff }, /*    10 */
  {   -10,  -119,    16,0, 0x0000,0x0000, 0x95,0x4a,0x00,0xff }, /*    11 */
  {    -5,  -151,    17,0, 0x0000,0x0000, 0xbc,0x5e,0x00,0xff }, /*    12 */
  {   -13,  -151,    46,0, 0x0000,0x0000, 0xa2,0x51,0x00,0xff }, /*    13 */
  {   -59,  -151,     7,0, 0x0000,0x0000, 0x9c,0x4e,0x00,0xff }, /*    14 */
  {   -59,  -151,     7,0, 0x0000,0x0000, 0x9c,0x4e,0x00,0xff }, /*    15 */
  {   -44,  -151,   -30,0, 0x0000,0x0000, 0xad,0x56,0x00,0xff }, /*    16 */
  {    -5,  -151,    17,0, 0x0000,0x0000, 0xbc,0x5e,0x00,0xff }, /*    17 */
  {   -13,  -151,    46,0, 0x0000,0x0000, 0xa2,0x51,0x00,0xff }, /*    18 */
  {   -10,  -119,    16,0, 0x0000,0x0000, 0x95,0x4a,0x00,0xff }, /*    19 */
  {   -18,  -109,    45,0, 0x0000,0x0000, 0x8c,0x46,0x00,0xff }, /*    20 */
  {   -59,  -151,     7,0, 0x0000,0x0000, 0x9c,0x4e,0x00,0xff }, /*    21 */
  {   -65,  -151,    37,0, 0x0000,0x0000, 0x85,0x42,0x00,0xff }, /*    22 */
  {   -55,  -119,     8,0, 0x0000,0x0000, 0x7a,0x3d,0x00,0xff }, /*    23 */
  {   -18,  -109,    45,0, 0x0000,0x0000, 0x8c,0x46,0x00,0xff }, /*    24 */
  {   -39,  -138,    62,0, 0x0000,0x0000, 0x70,0x38,0x00,0xff }, /*    25 */
  {   -13,  -151,    46,0, 0x0000,0x0000, 0xa2,0x51,0x00,0xff }, /*    26 */
  {   -13,  -151,    46,0, 0x0000,0x0000, 0xa2,0x51,0x00,0xff }, /*    27 */
  {   -39,  -138,    62,0, 0x0000,0x0000, 0x70,0x38,0x00,0xff }, /*    28 */
  {   -65,  -151,    37,0, 0x0000,0x0000, 0x85,0x42,0x00,0xff }, /*    29 */
  {   -65,  -151,    37,0, 0x0000,0x0000, 0x85,0x42,0x00,0xff }, /*    30 */
  {   -39,  -138,    62,0, 0x0000,0x0000, 0x70,0x38,0x00,0xff }, /*    31 */
  {   -52,  -109,    39,0, 0x0000,0x0000, 0x70,0x38,0x00,0xff }, /*    32 */
  {   -52,  -109,    39,0, 0x0000,0x0000, 0x70,0x38,0x00,0xff }, /*    33 */
  {   -39,  -138,    62,0, 0x0000,0x0000, 0x70,0x38,0x00,0xff }, /*    34 */
  {   -18,  -109,    45,0, 0x0000,0x0000, 0x8c,0x46,0x00,0xff }, /*    35 */
  {   -10,  -119,    16,0, 0x0000,0x0000, 0x95,0x4a,0x00,0xff }, /*    36 */
  {   -10,  -129,   -24,0, 0x0000,0x0000, 0x95,0x4a,0x00,0xff }, /*    37 */
  {   -55,  -119,     8,0, 0x0000,0x0000, 0x7a,0x3d,0x00,0xff }, /*    38 */
  {    -5,  -151,    17,0, 0x0000,0x0000, 0xbc,0x5e,0x00,0xff }, /*    39 */
  {    -5,  -151,   -23,0, 0x0000,0x0000, 0xcd,0x66,0x00,0xff }, /*    40 */
  {   -10,  -119,    16,0, 0x0000,0x0000, 0x95,0x4a,0x00,0xff }, /*    41 */
  {   -55,  -119,     8,0, 0x0000,0x0000, 0x7a,0x3d,0x00,0xff }, /*    42 */
  {   -41,  -129,   -29,0, 0x0000,0x0000, 0x7f,0x3f,0x00,0xff }, /*    43 */
  {   -59,  -151,     7,0, 0x0000,0x0000, 0x9c,0x4e,0x00,0xff }, /*    44 */
  {   -65,  -151,    37,0, 0x0000,0x0000, 0x85,0x42,0x00,0xff }, /*    45 */
  {   -52,  -109,    39,0, 0x0000,0x0000, 0x70,0x38,0x00,0xff }, /*    46 */
  {   -55,  -119,     8,0, 0x0000,0x0000, 0x7a,0x3d,0x00,0xff }, /*    47 */
  {   -41,  -129,   -29,0, 0x0000,0x0000, 0x7f,0x3f,0x00,0xff }, /*    48 */
  {   -44,  -151,   -30,0, 0x0000,0x0000, 0xad,0x56,0x00,0xff }, /*    49 */
  {   -59,  -151,     7,0, 0x0000,0x0000, 0x9c,0x4e,0x00,0xff }, /*    50 */
  {   -44,  -151,   -30,0, 0x0000,0x0000, 0xad,0x56,0x00,0xff }, /*    51 */
  {    -5,  -151,   -23,0, 0x0000,0x0000, 0xcd,0x66,0x00,0xff }, /*    52 */
  {    -5,  -151,    17,0, 0x0000,0x0000, 0xbc,0x5e,0x00,0xff }, /*    53 */
  {    -5,  -151,   -23,0, 0x0000,0x0000, 0xcd,0x66,0x00,0xff }, /*    54 */
  {   -10,  -129,   -24,0, 0x0000,0x0000, 0x95,0x4a,0x00,0xff }, /*    55 */
  {   -10,  -119,    16,0, 0x0000,0x0000, 0x95,0x4a,0x00,0xff }, /*    56 */
  {   -13,  -151,    46,0, 0x0000,0x0000, 0xa2,0x51,0x00,0xff }, /*    57 */
  {    -5,  -151,    17,0, 0x0000,0x0000, 0xbc,0x5e,0x00,0xff }, /*    58 */
  {   -10,  -119,    16,0, 0x0000,0x0000, 0x95,0x4a,0x00,0xff }, /*    59 */
  {   -13,  -151,    46,0, 0x0000,0x0000, 0xa2,0x51,0x00,0xff }, /*    60 */
  {   -65,  -151,    37,0, 0x0000,0x0000, 0x85,0x42,0x00,0xff }, /*    61 */
  {   -59,  -151,     7,0, 0x0000,0x0000, 0x9c,0x4e,0x00,0xff }, /*    62 */
  {   -44,  -151,   -30,0, 0x0000,0x0000, 0xad,0x56,0x00,0xff }, /*    63 */
  {   -41,  -129,   -29,0, 0x0000,0x0000, 0x7f,0x3f,0x00,0xff }, /*    64 */
  {   -10,  -129,   -24,0, 0x0000,0x0000, 0x95,0x4a,0x00,0xff }, /*    65 */
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
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 30, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP1Triangle(  1,  2,  3,  0),
  gsSP1Triangle(  4,  5,  6,  0),
  gsSP1Triangle(  7,  8,  9,  0),
  gsSP2Triangles( 10, 11, 12,  0, 13, 14, 15,  0),
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
  bmp_frake_down,
  tlut_frake_down,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _frake_down_aligner19[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_326_[] = {
  {   -38,   -53,   -22,0, 0x0000,0x0000, 0xff,0xc8,0x90,0xff }, /*     0 */
  {   -38,   -75,     3,0, 0x0000,0x0000, 0xa3,0x76,0x55,0xff }, /*     1 */
  {   -38,   -53,     3,0, 0x0000,0x0000, 0xa9,0x7a,0x58,0xff }, /*     2 */
  {   -38,   -53,   -22,0, 0x0000,0x0000, 0xff,0xc8,0x90,0xff }, /*     3 */
  {   -38,   -75,   -22,0, 0x0000,0x0000, 0xdb,0x9e,0x71,0xff }, /*     4 */
  {   -38,   -75,     3,0, 0x0000,0x0000, 0xa3,0x76,0x55,0xff }, /*     5 */
  {   -13,   -53,   -22,0, 0x0000,0x0000, 0xff,0xf0,0xac,0xff }, /*     6 */
  {   -38,   -75,   -22,0, 0x0000,0x0000, 0xdb,0x9e,0x71,0xff }, /*     7 */
  {   -38,   -53,   -22,0, 0x0000,0x0000, 0xff,0xc8,0x90,0xff }, /*     8 */
  {   -13,   -53,   -22,0, 0x0000,0x0000, 0xff,0xf0,0xac,0xff }, /*     9 */
  {   -13,   -75,   -22,0, 0x0000,0x0000, 0xff,0xcc,0x93,0xff }, /*    10 */
  {   -38,   -75,   -22,0, 0x0000,0x0000, 0xdb,0x9e,0x71,0xff }, /*    11 */
  {   -13,   -53,     3,0, 0x0000,0x0000, 0xf8,0xb3,0x81,0xff }, /*    12 */
  {   -13,   -75,     3,0, 0x0000,0x0000, 0xf8,0xb3,0x81,0xff }, /*    13 */
  {   -13,   -53,   -22,0, 0x0000,0x0000, 0xff,0xf0,0xac,0xff }, /*    14 */
  {   -38,   -53,     3,0, 0x0000,0x0000, 0xa9,0x7a,0x58,0xff }, /*    15 */
  {   -13,   -75,     3,0, 0x0000,0x0000, 0xf8,0xb3,0x81,0xff }, /*    16 */
  {   -13,   -53,     3,0, 0x0000,0x0000, 0xf8,0xb3,0x81,0xff }, /*    17 */
  {   -38,   -53,     3,0, 0x0000,0x0000, 0xa9,0x7a,0x58,0xff }, /*    18 */
  {   -38,   -75,     3,0, 0x0000,0x0000, 0xa3,0x76,0x55,0xff }, /*    19 */
  {   -13,   -75,     3,0, 0x0000,0x0000, 0xf8,0xb3,0x81,0xff }, /*    20 */
  {   -13,   -75,     3,0, 0x0000,0x0000, 0xf8,0xb3,0x81,0xff }, /*    21 */
  {   -13,   -75,   -22,0, 0x0000,0x0000, 0xff,0xcc,0x93,0xff }, /*    22 */
  {   -13,   -53,   -22,0, 0x0000,0x0000, 0xff,0xf0,0xac,0xff }, /*    23 */
  {   -13,   -75,   -22,0, 0x0000,0x0000, 0xff,0xcc,0x93,0xff }, /*    24 */
  {   -26,   -86,    -9,0, 0x0000,0x0000, 0xbc,0x51,0x3a,0xff }, /*    25 */
  {   -38,   -75,   -22,0, 0x0000,0x0000, 0xdb,0x9e,0x71,0xff }, /*    26 */
  {   -38,   -75,   -22,0, 0x0000,0x0000, 0xdb,0x9e,0x71,0xff }, /*    27 */
  {   -26,   -86,    -9,0, 0x0000,0x0000, 0xbc,0x51,0x3a,0xff }, /*    28 */
  {   -38,   -75,     3,0, 0x0000,0x0000, 0xa3,0x76,0x55,0xff }, /*    29 */
  {   -38,   -75,     3,0, 0x0000,0x0000, 0xa3,0x76,0x55,0xff }, /*    30 */
  {   -26,   -86,    -9,0, 0x0000,0x0000, 0xbc,0x51,0x3a,0xff }, /*    31 */
  {   -13,   -75,     3,0, 0x0000,0x0000, 0xf8,0xb3,0x81,0xff }, /*    32 */
  {   -13,   -75,     3,0, 0x0000,0x0000, 0xf8,0xb3,0x81,0xff }, /*    33 */
  {   -26,   -86,    -9,0, 0x0000,0x0000, 0xbc,0x51,0x3a,0xff }, /*    34 */
  {   -13,   -75,   -22,0, 0x0000,0x0000, 0xff,0xcc,0x93,0xff }, /*    35 */
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
  bmp_frake_down,
  tlut_frake_down,
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
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0002, 0.0002, 0.0002, 0.0000,
};

static s16 Animation_for_body_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_body_motion_ch1_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.5168, 1.0327, 1.5467,
  2.0581, 2.5659, 3.0691, 3.5670, 4.0586, 4.5430, 5.0196, 5.0196, 5.0196,
  5.0196, 5.0196, 5.0196, 5.0196, 5.0196, 5.0196, 5.0196, 5.0196, 5.0196,
  -1.3418, -7.3380, -11.3366, -12.2493,
};

static s16 Animation_for_body_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_body_motion_ch2_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0108, -0.0433,
  -0.0973, -0.1728, -0.2697, -0.3877, -0.5267, -0.6865, -0.8667, -1.0670,
  -1.0670, -1.0670, -1.0670, -1.0670, -1.0670, -1.0670, -1.0670, -1.0670,
  -1.0670, -1.0670, -1.0670, -0.0732, -2.4187, -7.4650, -13.8386,
};

static s16 Animation_for_body_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_body_motion_ch3_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0419, -0.0838,
  -0.1257, -0.1676, -0.2094, -0.2513, -0.2932, -0.3351, -0.3770, -0.4189,
  -0.4189, -0.4189, -0.4189, -0.4189, -0.4189, -0.4189, -0.4189, -0.4189,
  -0.4189, -0.4189, -0.4189, 0.1089, 0.6368, 1.1646, 1.6925,
};

static s16 Animation_for_body_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_body_motion_ch4_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000,
};

static s16 Animation_for_body_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_body_motion_ch5_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000,
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
  {NDC_ANIMKIND_SCL_Y, 0, 30, 31,
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
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000, -0.0000,
  0.0000, 0.0000, -0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  -42.0738, -81.9938, -112.2193, -127.1929,
};

static s16 Animation_for_R_uparm_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_uparm_motion_ch1_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -6.0224, 6.2809, 34.1720, 71.8978,
};

static s16 Animation_for_R_uparm_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_uparm_motion_ch2_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  0.0000, 0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 12.2992, 25.2126, 37.7581, 48.9572,
};

static s16 Animation_for_R_uparm_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_uparm_motion_ch3_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000, -0.0000,
  0.0000, 0.0000, -0.0000, -0.0000, 0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0779, -0.1629, -0.2549, -0.3566,
};

static s16 Animation_for_R_uparm_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_uparm_motion_ch4_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  0.0000, -0.0000, -0.0000, -0.0000, 0.0000, -0.0000, 0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.1165, -0.2489, -0.3940, -0.5462,
};

static s16 Animation_for_R_uparm_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_uparm_motion_ch5_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, 0.0000, -0.0000, -0.0000, -0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, -0.3847, -0.7459, -1.0817, -1.3906,
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
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, 0.0000, -0.0000,
  -0.0000, 0.0000, -0.0000, 0.0000, 0.0000, -0.0000, -0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.3779, 0.6921, 0.9405, 1.1207,
};

static s16 Animation_for_R_lowarm_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowarm_motion_ch1_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.5193, 0.9509, 1.2919, 1.5399,
};

static s16 Animation_for_R_lowarm_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowarm_motion_ch2_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000,
  -0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -1.2649, -2.5794, -3.9338, -5.3190,
};

static s16 Animation_for_R_lowarm_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowarm_motion_ch3_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000, -0.0000,
  -0.0000, -0.0000, 0.0000, -0.0000, 0.0000, -0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, -0.0679, -0.1362, -0.2051, -0.2750,
};

static s16 Animation_for_R_lowarm_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowarm_motion_ch4_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  -0.0000, 0.0000, 0.0000, -0.0000, -0.0000, 0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, 0.0494, 0.0985, 0.1472, 0.1951,
};

static s16 Animation_for_R_lowarm_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowarm_motion_ch5_val[]={
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  0.0000, 0.0000, 0.0000, -0.0000, 0.0000, 0.0000, 0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0017, -0.0067, -0.0152, -0.0271,
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
  -0.0000, -0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, -0.0000, -0.0000, -0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.2714, 0.3478, 0.2290, -0.0845,
};

static s16 Animation_for_R_hand_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_hand_motion_ch1_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.3914, 0.5225, 0.3928, 0.0027,
};

static s16 Animation_for_R_hand_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_hand_motion_ch2_val[]={
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, 0.0000, -0.0000, -0.0000,
  -0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  -0.0000, -0.0000, -4.3803, -8.7858, -13.1925, -17.5764,
};

static s16 Animation_for_R_hand_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_hand_motion_ch3_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000, -0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0629, -0.1261, -0.1897, -0.2541,
};

static s16 Animation_for_R_hand_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_hand_motion_ch4_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000,
  0.0000, -0.0000, 0.0000, 0.0000, -0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0444, 0.0887, 0.1327, 0.1762,
};

static s16 Animation_for_R_hand_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_hand_motion_ch5_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, -0.0013, -0.0053, -0.0119, -0.0213,
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
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000, -0.0000,
  0.0000, 0.0000, -0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  -42.0738, -81.9938, -112.2193, -127.1929,
};

static s16 Animation_for_R_sode_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_sode_motion_ch1_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -6.0224, 6.2809, 34.1720, 71.8978,
};

static s16 Animation_for_R_sode_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_sode_motion_ch2_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  0.0000, 0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 12.2992, 25.2126, 37.7581, 48.9572,
};

static s16 Animation_for_R_sode_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_sode_motion_ch3_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000, -0.0000,
  0.0000, 0.0000, -0.0000, -0.0000, 0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0779, -0.1629, -0.2549, -0.3566,
};

static s16 Animation_for_R_sode_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_sode_motion_ch4_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  0.0000, -0.0000, -0.0000, -0.0000, 0.0000, -0.0000, 0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.1165, -0.2489, -0.3940, -0.5462,
};

static s16 Animation_for_R_sode_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_sode_motion_ch5_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, 0.0000, -0.0000, -0.0000, -0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, -0.3847, -0.7459, -1.0817, -1.3906,
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
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  0.0000, 0.0000, 0.0000, -0.0000, 0.0000, 0.0000, 0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, 35.6837, 70.9965, 101.7826, 124.6398,
};

static s16 Animation_for_L_uparm_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_uparm_motion_ch1_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  0.0000, -0.0001, -0.0001, -0.0000, -0.0001, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -7.0339, -1.4849, 15.5217, 41.5833,
};

static s16 Animation_for_L_uparm_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_uparm_motion_ch2_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000, -0.0000,
  0.0000, -0.0000, -0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  -0.0000, 5.6367, 11.9329, 18.4568, 24.7668,
};

static s16 Animation_for_L_uparm_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_uparm_motion_ch3_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000, 0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0212, -0.0459, -0.0722, -0.0988,
};

static s16 Animation_for_L_uparm_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_uparm_motion_ch4_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  0.0000, -0.0000, 0.0000, 0.0000, -0.0000, 0.0000, -0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000,
  -0.0000, -0.0000, 0.0892, 0.1908, 0.3040, 0.4270,
};

static s16 Animation_for_L_uparm_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_uparm_motion_ch5_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  -0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.3315, 0.6509, 0.9576, 1.2516,
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
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.3054, -0.5709, -0.7951, -0.9772,
};

static s16 Animation_for_L_lowarm_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowarm_motion_ch1_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0001, 0.0001, 0.0000, 0.0001, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.4197, 0.7843, 1.0924, 1.3423,
};

static s16 Animation_for_L_lowarm_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowarm_motion_ch2_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000,
  -0.0000, 0.0000, 0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, -1.0018, -2.0359, -3.0979, -4.1830,
};

static s16 Animation_for_L_lowarm_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowarm_motion_ch3_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000, -0.0000,
  0.0000, -0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0540, -0.1082, -0.1627, -0.2177,
};

static s16 Animation_for_L_lowarm_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowarm_motion_ch4_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000,
  0.0000, 0.0000, -0.0000, 0.0000, -0.0000, 0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0393, -0.0784, -0.1173, -0.1559,
};

static s16 Animation_for_L_lowarm_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowarm_motion_ch5_val[]={
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, 0.0011, 0.0042, 0.0096, 0.0171,
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
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, 0.0001, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, -0.3223, -0.2670, 0.1650, 0.9694,
};

static s16 Animation_for_L_hand_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_hand_motion_ch1_val[]={
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, 0.4242, 0.3431, -0.2423, -1.3264,
};

static s16 Animation_for_L_hand_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_hand_motion_ch2_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, -0.0000, -0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000, -0.0000, 0.0000,
  0.0000, 0.0000, -6.1048, -12.2316, -18.3148, -24.2905,
};

static s16 Animation_for_L_hand_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_hand_motion_ch3_val[]={
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, 0.0000,
  -0.0000, 0.0000, -0.0000, -0.0000, -0.0000, 0.0000, -0.0000, 0.0000,
  0.0000, 0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  0.0000, 0.0000, 0.0000, -0.0818, -0.1642, -0.2478, -0.3334,
};

static s16 Animation_for_L_hand_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_hand_motion_ch4_val[]={
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, 0.0000,
  0.0000, 0.0000, -0.0000, -0.0000, -0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, -0.0616, -0.1227, -0.1830, -0.2420,
};

static s16 Animation_for_L_hand_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_hand_motion_ch5_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, -0.0000, -0.0000, -0.0000, 0.0000, 0.0000,
  0.0000, 0.0025, 0.0102, 0.0231, 0.0413,
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
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  0.0000, 0.0000, 0.0000, -0.0000, 0.0000, 0.0000, 0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, 35.6837, 70.9965, 101.7826, 124.6398,
};

static s16 Animation_for_L_sode_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_sode_motion_ch1_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  0.0000, -0.0001, -0.0001, -0.0000, -0.0001, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -7.0339, -1.4849, 15.5217, 41.5833,
};

static s16 Animation_for_L_sode_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_sode_motion_ch2_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000, -0.0000,
  0.0000, -0.0000, -0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  -0.0000, 5.6367, 11.9329, 18.4568, 24.7668,
};

static s16 Animation_for_L_sode_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_sode_motion_ch3_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000, 0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0212, -0.0459, -0.0722, -0.0988,
};

static s16 Animation_for_L_sode_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_sode_motion_ch4_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  0.0000, -0.0000, 0.0000, 0.0000, -0.0000, 0.0000, -0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000,
  -0.0000, -0.0000, 0.0892, 0.1908, 0.3040, 0.4270,
};

static s16 Animation_for_L_sode_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_sode_motion_ch5_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  -0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.3315, 0.6509, 0.9576, 1.2516,
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
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0009, -0.0017,
  -0.0025, -0.0032, -0.0039, -0.0044, -0.0049, -0.0052, -0.0055, -0.0056,
  -0.0056, -0.0056, -0.0056, -0.0056, -0.0056, -0.0056, -0.0056, -0.0056,
  -0.0056, -0.0056, -0.0056, -4.7304, -8.7366, -11.4139, -12.3544,
};

static s16 Animation_for_head_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_head_motion_ch1_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.8316, 1.9644, 3.3949,
  5.1187, 7.1307, 9.4247, 11.9937, 14.8301, 17.9250, 21.2692, 21.2692,
  21.2692, 21.2692, 21.2692, 21.2692, 21.2692, 21.2692, 21.2692, 21.2692,
  21.2692, 21.2692, 18.9232, 15.8844, 12.4775, 9.0710,
};

static s16 Animation_for_head_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_head_motion_ch2_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 5.4898, 10.9255, 16.2906,
  21.5686, 26.7438, 31.8002, 36.7226, 41.4959, 46.1059, 50.5383, 50.5383,
  50.5383, 50.5383, 50.5383, 50.5383, 50.5383, 50.5383, 50.5383, 50.5383,
  50.5383, 50.5383, 47.2956, 42.4365, 36.2580, 29.2716,
};

static s16 Animation_for_head_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_head_motion_ch3_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0551, -0.1102,
  -0.1653, -0.2205, -0.2756, -0.3307, -0.3858, -0.4409, -0.4960, -0.5511,
  -0.5511, -0.5511, -0.5511, -0.5511, -0.5511, -0.5511, -0.5511, -0.5511,
  -0.5511, -0.5511, -0.5511, -0.5554, -0.6463, -0.9126, -1.5708,
};

static s16 Animation_for_head_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_head_motion_ch4_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0001, 0.0001, 0.0002,
  0.0002, 0.0002, 0.0003, 0.0003, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004,
  0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004,
  -0.3381, -0.6735, -0.9754, -1.1416,
};

static s16 Animation_for_head_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_head_motion_ch5_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  -0.0001, -0.0001, -0.0001, -0.0002, -0.0002, -0.0003, -0.0004, -0.0004,
  -0.0004, -0.0004, -0.0004, -0.0004, -0.0004, -0.0004, -0.0004, -0.0004,
  -0.0004, -0.0004, -0.0004, 0.2028, 0.4396, 0.8194, 1.5706,
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

static s16 Animation_for_head_motion_ch8_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_head_motion_ch8_val[]={
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
  {NDC_ANIMKIND_SCL_Y, 0, 30, 31,
   Animation_for_head_motion_ch7_key, Animation_for_head_motion_ch7_val},
  {NDC_ANIMKIND_SCL_Z, 0, 30, 31,
   Animation_for_head_motion_ch8_key, Animation_for_head_motion_ch8_val},
};

static NDR_ANIM Animation_for_head_motion={
  Animation_for_head_motion_chlist, 9, 30
};

static s16 Animation_for_L_upleg_motion_ch0_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_upleg_motion_ch0_val[]={
  0.0000, -0.5514, -1.2201, -2.0423, -3.0591, -4.3109, -3.6623, -3.0728,
  -2.5387, -2.0557, -1.6196, -1.2261, -0.8709, -0.5504, -0.2611, -0.0003,
  0.0089, -0.0141, -0.0675, -0.1495, -0.2579, -0.3903, -0.5441, -0.7165,
  -0.9047, -1.1057, -1.3162, -1.3162, -1.3162, -1.3162, -1.3162,
};

static s16 Animation_for_L_upleg_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_upleg_motion_ch1_val[]={
  0.0000, 1.5243, 2.9889, 4.3957, 5.7316, 6.9679, 6.3082, 5.7417, 5.2747,
  4.9114, 4.6547, 4.5050, 4.4612, 4.5206, 4.6785, 4.9292, 5.2564, 5.5642,
  5.8486, 6.1064, 6.3342, 6.5292, 6.6889, 6.8108, 6.8930, 6.9338, 6.9318,
  5.6163, 4.1983, 2.7038, 1.1599,
};

static s16 Animation_for_L_upleg_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_upleg_motion_ch2_val[]={
  -0.0000, -2.8685, -5.8885, -9.0231, -12.2356, -15.4892, -14.1280,
  -12.7225, -11.2818, -9.8156, -8.3338, -6.8464, -5.3634, -3.8941, -2.4474,
  -1.0312, -1.0905, -1.1796, -1.2968, -1.4405, -1.6090, -1.8006, -2.0135,
  -2.2462, -2.4968, -2.7639, -3.0457, -2.1965, -1.5323, -1.0652, -0.8037,
 };

static s16 Animation_for_L_upleg_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_upleg_motion_ch3_val[]={
  -0.0000, 0.0087, 0.0182, 0.0292, 0.0428, 0.0599, 0.0131, -0.0330,
  -0.0787, -0.1244, -0.1702, -0.2162, -0.2622, -0.3082, -0.3538, -0.3985,
  -0.3742, -0.3474, -0.3181, -0.2862, -0.2515, -0.2139, -0.1733, -0.1295,
  -0.0823, -0.0317, 0.0227, 0.1203, 0.2113, 0.2980, 0.3825,
};

static s16 Animation_for_L_upleg_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_upleg_motion_ch4_val[]={
  0.0000, -0.1050, -0.2063, -0.3040, -0.3981, -0.4888, -0.4230, -0.3599,
  -0.2998, -0.2433, -0.1907, -0.1424, -0.0988, -0.0602, -0.0269, 0.0010,
  0.0379, 0.0759, 0.1147, 0.1542, 0.1942, 0.2345, 0.2747, 0.3147, 0.3543,
  0.3931, 0.4308, 0.3305, 0.2274, 0.1226, 0.0169,
};

static s16 Animation_for_L_upleg_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_upleg_motion_ch5_val[]={
  0.0000, 0.1190, 0.2359, 0.3501, 0.4609, 0.5670, 0.5306, 0.4893, 0.4432,
  0.3925, 0.3374, 0.2780, 0.2145, 0.1473, 0.0765, 0.0026, 0.0745, 0.1461,
  0.2177, 0.2896, 0.3618, 0.4347, 0.5084, 0.5833, 0.6597, 0.7377, 0.8177,
  0.8540, 0.8791, 0.8942, 0.9001,
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
  0.0000, -0.0000, -0.0001, -0.0000, -0.0000, -0.0000, -0.0001, 0.0000,
  0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, 0.0000, -0.0000,
  0.0000, -0.0000, 0.0000, 0.0001, 0.0000, 0.0000, 0.0000, 0.0000, 0.0001,
  0.0001, 0.0001, 0.0001, 0.0001, 0.0002, 0.0001,
};

static s16 Animation_for_L_lowleg_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowleg_motion_ch1_val[]={
  0.0000, -0.9668, -2.8253, -5.5533, -9.1180, -13.4768, -12.7802, -12.1002,
  -11.4370, -10.7908, -10.1616, -9.5498, -8.9555, -8.3787, -7.8197,
  -7.2786, -11.0100, -15.4279, -20.4851, -26.1277, -32.2953, -38.9220,
  -45.9372, -53.2660, -60.8300, -68.5485, -76.3391, -76.3392, -76.3393,
  -76.3394, -76.3395,
};

static s16 Animation_for_L_lowleg_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowleg_motion_ch2_val[]={
  0.0000, 8.2070, 16.2591, 24.0596, 31.5151, 38.5360, 37.5229, 36.4986,
  35.4633, 34.4174, 33.3610, 32.2946, 31.2184, 30.1326, 29.0375, 27.9335,
  34.7788, 41.2023, 47.1357, 52.5155, 57.2843, 61.3912, 64.7923, 67.4512,
  69.3397, 70.4374, 70.7329, 70.7329, 70.7329, 70.7329, 70.7329,
};

static s16 Animation_for_L_lowleg_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowleg_motion_ch3_val[]={
  0.0000, 0.1096, 0.2192, 0.3288, 0.4383, 0.5479, 0.5316, 0.5153, 0.4990,
  0.4828, 0.4665, 0.4502, 0.4339, 0.4176, 0.4013, 0.3850, 0.4884, 0.5917,
  0.6951, 0.7985, 0.9019, 1.0053, 1.1087, 1.2120, 1.3154, 1.4188, 1.5222,
  1.5222, 1.5222, 1.5222, 1.5222,
};

static s16 Animation_for_L_lowleg_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowleg_motion_ch4_val[]={
  -0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000,
};

static s16 Animation_for_L_lowleg_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_lowleg_motion_ch5_val[]={
  -0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000,
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
  0.0000, -0.0000, 0.0002, 0.0000, 0.0000, 0.0000, -0.0001, 0.0001,
  -0.0001, -0.0000, 0.0002, 0.0001, -0.0001, -0.0001, 0.0000, 0.0000,
  -0.5098, -1.0190, -1.5282, -2.0364, -2.5435, -3.0498, -3.5544, -4.0577,
  -4.5589, -5.0577, -5.5543, -5.5542, -5.5542, -5.5543, -5.5542,
};

static s16 Animation_for_L_shoe_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_shoe_motion_ch1_val[]={
  -0.0000, -0.0006, -0.0007, -0.0015, -0.0020, -0.0021, -0.0021, -0.0018,
  -0.0017, -0.0012, -0.0010, -0.0011, -0.0007, -0.0006, -0.0002, -0.0001,
  -0.0294, -0.1223, -0.2789, -0.4989, -0.7827, -1.1295, -1.5398, -2.0128,
  -2.5485, -3.1468, -3.8075, -3.8075, -3.8075, -3.8075, -3.8075,
};

static s16 Animation_for_L_shoe_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_shoe_motion_ch2_val[]={
  0.0000, -0.0000, 0.0000, -0.0000, 0.0000, -0.0001, 0.0000, -0.0001,
  0.0000, -0.0001, 0.0000, 0.0001, 0.0000, 0.0001, 0.0000, -0.0000,
  -2.8905, -5.7800, -8.6666, -11.5491, -14.4262, -17.2968, -20.1591,
  -23.0120, -25.8542, -28.6842, -31.5009, -31.5008, -31.5008, -31.5009,
  -31.5009,
};

static s16 Animation_for_L_shoe_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_shoe_motion_ch3_val[]={
  -0.0000, -0.0000, 0.0000, -0.0000, 0.0000, -0.0000, 0.0000, -0.0000,
  0.0000, -0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000,
  -0.0214, -0.0427, -0.0641, -0.0855, -0.1068, -0.1282, -0.1495, -0.1709,
  -0.1923, -0.2136, -0.2350, -0.2350, -0.2350, -0.2350, -0.2350,
};

static s16 Animation_for_L_shoe_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_shoe_motion_ch4_val[]={
  0.0000, 0.0000, 0.0000, -0.0000, -0.0000, -0.0000, 0.0000, 0.0000,
  -0.0000, -0.0000, -0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000,
  0.0000, 0.0002, 0.0004, 0.0006, 0.0010, 0.0014, 0.0020, 0.0026, 0.0033,
  0.0040, 0.0048, 0.0048, 0.0048, 0.0048, 0.0048,
};

static s16 Animation_for_L_shoe_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_shoe_motion_ch5_val[]={
  0.0000, -0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000,
  0.0038, 0.0075, 0.0113, 0.0150, 0.0188, 0.0225, 0.0263, 0.0300, 0.0337,
  0.0374, 0.0411, 0.0411, 0.0411, 0.0411, 0.0411,
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
  0.0000, -0.5514, -1.2201, -2.0423, -3.0591, -4.3109, -3.6623, -3.0728,
  -2.5387, -2.0557, -1.6196, -1.2261, -0.8709, -0.5504, -0.2611, -0.0003,
  0.0089, -0.0141, -0.0675, -0.1495, -0.2579, -0.3903, -0.5441, -0.7165,
  -0.9047, -1.1057, -1.3162, -1.3162, -1.3162, -1.3162, -1.3162,
};

static s16 Animation_for_L_leg_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_leg_motion_ch1_val[]={
  0.0000, 1.5243, 2.9889, 4.3957, 5.7316, 6.9679, 6.3082, 5.7417, 5.2747,
  4.9114, 4.6547, 4.5050, 4.4612, 4.5206, 4.6785, 4.9292, 5.2564, 5.5642,
  5.8486, 6.1064, 6.3342, 6.5292, 6.6889, 6.8108, 6.8930, 6.9338, 6.9318,
  5.6163, 4.1983, 2.7038, 1.1599,
};

static s16 Animation_for_L_leg_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_leg_motion_ch2_val[]={
  -0.0000, -2.8685, -5.8885, -9.0231, -12.2356, -15.4892, -14.1280,
  -12.7225, -11.2818, -9.8156, -8.3338, -6.8464, -5.3634, -3.8941, -2.4474,
  -1.0312, -1.0905, -1.1796, -1.2968, -1.4405, -1.6090, -1.8006, -2.0135,
  -2.2462, -2.4968, -2.7639, -3.0457, -2.1965, -1.5323, -1.0652, -0.8037,
 };

static s16 Animation_for_L_leg_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_leg_motion_ch3_val[]={
  -0.0000, 0.0087, 0.0182, 0.0292, 0.0428, 0.0599, 0.0131, -0.0330,
  -0.0787, -0.1244, -0.1702, -0.2162, -0.2622, -0.3082, -0.3538, -0.3985,
  -0.3742, -0.3474, -0.3181, -0.2862, -0.2515, -0.2139, -0.1733, -0.1295,
  -0.0823, -0.0317, 0.0227, 0.1203, 0.2113, 0.2980, 0.3825,
};

static s16 Animation_for_L_leg_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_leg_motion_ch4_val[]={
  0.0000, -0.1050, -0.2063, -0.3040, -0.3981, -0.4888, -0.4230, -0.3599,
  -0.2998, -0.2433, -0.1907, -0.1424, -0.0988, -0.0602, -0.0269, 0.0010,
  0.0379, 0.0759, 0.1147, 0.1542, 0.1942, 0.2345, 0.2747, 0.3147, 0.3543,
  0.3931, 0.4308, 0.3305, 0.2274, 0.1226, 0.0169,
};

static s16 Animation_for_L_leg_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_L_leg_motion_ch5_val[]={
  0.0000, 0.1190, 0.2359, 0.3501, 0.4609, 0.5670, 0.5306, 0.4893, 0.4432,
  0.3925, 0.3374, 0.2780, 0.2145, 0.1473, 0.0765, 0.0026, 0.0745, 0.1461,
  0.2177, 0.2896, 0.3618, 0.4347, 0.5084, 0.5833, 0.6597, 0.7377, 0.8177,
  0.8540, 0.8791, 0.8942, 0.9001,
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
  0.0000, 2.5217, 5.1239, 7.4730, 9.3379, 10.6230, 10.0543, 9.3396,
  8.4775, 7.4756, 6.3500, 5.1258, 3.8362, 2.5206, 1.2238, -0.0072, 0.0101,
  0.0258, 0.0399, 0.0523, 0.0631, 0.0722, 0.0796, 0.0854, 0.0895, 0.0919,
  0.0927, 0.1258, 0.1613, 0.1992, 0.2394,
};

static s16 Animation_for_hip_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_hip_motion_ch1_val[]={
  0.0000, 2.1037, 3.5000, 4.2139, 4.3912, 4.2404, 4.5753, 4.9382, 5.3005,
  5.6288, 5.8869, 6.0389, 6.0520, 5.8989, 5.5600, 5.0252, 4.9012, 4.7766,
  4.6512, 4.5252, 4.3986, 4.2713, 4.1435, 4.0152, 3.8864, 3.7571, 3.6273,
  1.9129, 0.1603, -1.5945, -3.3160,
};

static s16 Animation_for_hip_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_hip_motion_ch2_val[]={
  -0.0000, -0.4450, -1.6736, -3.4700, -5.5722, -7.7081, -6.8390, -5.9627,
  -5.1069, -4.2956, -3.5479, -2.8777, -2.2934, -1.7983, -1.3917, -1.0695,
  -1.0151, -0.9619, -0.9102, -0.8597, -0.8107, -0.7630, -0.7167, -0.6718,
  -0.6283, -0.5862, -0.5455, -0.1498, -0.0021, -0.1051, -0.4563,
};

static s16 Animation_for_hip_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_hip_motion_ch3_val[]={
  0.0000, -0.1883, -0.3615, -0.5218, -0.6700, -0.8048, -0.7984, -0.7768,
  -0.7461, -0.7094, -0.6682, -0.6238, -0.5765, -0.5267, -0.4743, -0.4194,
  -0.4084, -0.3974, -0.3864, -0.3754, -0.3644, -0.3533, -0.3423, -0.3312,
  -0.3201, -0.3090, -0.2979, -0.1560, -0.0135, 0.1292, 0.2721,
};

static s16 Animation_for_hip_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_hip_motion_ch4_val[]={
  0.0000, 0.1938, 0.3921, 0.5932, 0.7958, 0.9986, 0.8779, 0.7602, 0.6464,
  0.5372, 0.4329, 0.3341, 0.2410, 0.1539, 0.0730, -0.0017, -0.0033,
  -0.0049, -0.0064, -0.0080, -0.0095, -0.0110, -0.0124, -0.0139, -0.0153,
  -0.0166, -0.0180, 0.0001, 0.0125, 0.0188, 0.0188,
};

static s16 Animation_for_hip_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_hip_motion_ch5_val[]={
  0.0000, -0.0848, -0.1809, -0.2860, -0.3975, -0.5110, -0.5004, -0.4731,
  -0.4345, -0.3879, -0.3347, -0.2762, -0.2131, -0.1461, -0.0756, -0.0023,
  -0.0097, -0.0170, -0.0244, -0.0318, -0.0392, -0.0467, -0.0541, -0.0616,
  -0.0690, -0.0765, -0.0840, -0.0651, -0.0432, -0.0195, 0.0047,
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
  0.0000, -0.2505, -1.1691, -2.7093, -4.8083, -7.3907, -6.0414, -4.8026,
  -3.6849, -2.6984, -1.8526, -1.1561, -0.6164, -0.2403, -0.0333, 0.0005,
  -0.0178, -0.0286, -0.0317, -0.0270, -0.0146, 0.0052, 0.0316, 0.0639,
  0.1009, 0.1413, 0.1835, 0.2078, 0.2272, 0.2417, 0.2512,
};

static s16 Animation_for_R_leg_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_leg_motion_ch1_val[]={
  0.0000, 4.1290, 8.1353, 11.9103, 15.3583, 18.3997, 16.9296, 15.3479,
  13.6630, 11.8842, 10.0220, 8.0881, 6.0948, 4.0555, 1.9844, -0.1036,
  -0.2443, -0.3976, -0.5601, -0.7281, -0.8979, -1.0660, -1.2285, -1.3817,
  -1.5221, -1.6461, -1.7502, -1.9750, -2.1852, -2.3804, -2.5605,
};

static s16 Animation_for_R_leg_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_leg_motion_ch2_val[]={
  0.0000, -0.4552, -0.7782, -0.9734, -1.0499, -1.0205, -1.0419, -1.0398,
  -1.0123, -0.9580, -0.8754, -0.7632, -0.6205, -0.4465, -0.2410, -0.0038,
  0.0911, 0.1394, 0.1401, 0.0921, -0.0050, -0.1517, -0.3481, -0.5941,
  -0.8893, -1.2329, -1.6241, -1.9883, -2.3629, -2.7471, -3.1405,
};

static s16 Animation_for_R_leg_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_leg_motion_ch3_val[]={
  0.0000, -0.0320, -0.0650, -0.1001, -0.1380, -0.1796, -0.1562, -0.1338,
  -0.1123, -0.0916, -0.0717, -0.0524, -0.0336, -0.0151, 0.0031, 0.0211,
  0.0842, 0.1473, 0.2104, 0.2734, 0.3363, 0.3991, 0.4618, 0.5243, 0.5867,
  0.6488, 0.7107, 0.7463, 0.7819, 0.8175, 0.8530,
};

static s16 Animation_for_R_leg_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_leg_motion_ch4_val[]={
  0.0000, 0.0174, 0.0295, 0.0364, 0.0385, 0.0362, 0.0377, 0.0382, 0.0377,
  0.0360, 0.0331, 0.0290, 0.0237, 0.0171, 0.0093, 0.0001, -0.0041, -0.0073,
  -0.0094, -0.0103, -0.0099, -0.0084, -0.0056, -0.0015, 0.0039, 0.0106,
  0.0186, 0.0283, 0.0382, 0.0483, 0.0586,
};

static s16 Animation_for_R_leg_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_leg_motion_ch5_val[]={
  0.0000, 0.1531, 0.3047, 0.4544, 0.6019, 0.7471, 0.6750, 0.6022, 0.5288,
  0.4548, 0.3802, 0.3051, 0.2294, 0.1533, 0.0768, -0.0002, 0.0058, 0.0113,
  0.0163, 0.0209, 0.0252, 0.0293, 0.0334, 0.0374, 0.0415, 0.0458, 0.0505,
  0.0466, 0.0432, 0.0401, 0.0375,
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
  0.0000, 0.0000, 0.0001, 0.0001, 0.0001, 0.0000, 0.0001, 0.0001, 0.0001,
  0.0001, 0.0001, 0.0001, 0.0001, 0.0000, 0.0001, 0.0000, 0.0001, 0.0001,
  0.0001, 0.0001, 0.0001, 0.0000, 0.0001, 0.0001, 0.0000, 0.0001, 0.0000,
  0.0001, 0.0002, 0.0001, 0.0001,
};

static s16 Animation_for_R_lowleg_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowleg_motion_ch1_val[]={
  0.0000, -5.0123, -16.7049, -33.9668, -55.1571, -78.2621, -69.4966,
  -60.8094, -52.3179, -44.1367, -36.3764, -29.1415, -22.5300, -16.6310,
  -11.5241, -7.2784, -11.1850, -15.8358, -21.1771, -27.1467, -33.6753,
  -40.6872, -48.1009, -55.8304, -63.7859, -71.8751, -80.0040, -73.2280,
  -66.4686, -59.7806, -53.2178,
};

static s16 Animation_for_R_lowleg_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowleg_motion_ch2_val[]={
  0.0000, 22.7130, 42.8201, 58.4100, 68.0011, 70.6817, 70.5163, 69.3352,
  67.1542, 64.0030, 59.9240, 54.9724, 49.2149, 42.7294, 35.6033, 27.9330,
  35.0621, 41.7294, 47.8576, 53.3756, 58.2192, 62.3323, 65.6671, 68.1848,
  69.8564, 70.6622, 70.5930, 70.7115, 70.2214, 69.1267, 67.4363,
};

static s16 Animation_for_R_lowleg_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowleg_motion_ch3_val[]={
  -0.0000, 0.3095, 0.6191, 0.9286, 1.2381, 1.5476, 1.4314, 1.3151, 1.1988,
  1.0826, 0.9663, 0.8500, 0.7338, 0.6175, 0.5012, 0.3850, 0.4928, 0.6006,
  0.7084, 0.8162, 0.9240, 1.0318, 1.1396, 1.2474, 1.3552, 1.4630, 1.5708,
  1.4809, 1.3911, 1.3012, 1.2114,
};

static s16 Animation_for_R_lowleg_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowleg_motion_ch4_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000, -0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000,
};

static s16 Animation_for_R_lowleg_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_lowleg_motion_ch5_val[]={
  0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
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
  0.0000, 0.0000, 0.0001, -0.0001, -0.0000, 0.0000, -0.0001, 0.0000,
  0.0001, -0.0001, -0.0000, 0.0001, -0.0001, 0.0000, 0.0000, 0.0000,
  -0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000, 0.0000, -0.0000,
  0.0000, 0.0000, -0.0000, -1.7918, -3.1250, -3.9921, -4.3935,
};

static s16 Animation_for_R_shoe_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_shoe_motion_ch1_val[]={
  0.0000, -0.0006, -0.0013, -0.0015, -0.0021, -0.0025, -0.0024, -0.0021,
  -0.0017, -0.0015, -0.0012, -0.0011, -0.0008, -0.0006, -0.0003, 0.0001,
  0.0001, -0.0001, -0.0000, 0.0001, 0.0001, -0.0001, 0.0000, -0.0001,
  -0.0003, 0.0000, 0.0001, -0.5534, -2.1887, -4.8925, -8.6436,
};

static s16 Animation_for_R_shoe_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_shoe_motion_ch2_val[]={
  0.0000, 0.0001, 0.0000, -0.0001, 0.0000, -0.0001, -0.0000, -0.0001,
  -0.0001, -0.0000, 0.0001, -0.0000, -0.0001, -0.0000, -0.0000, -0.0000,
  0.0000, 0.0000, -0.0000, -0.0001, 0.0000, -0.0000, -0.0000, -0.0000,
  0.0000, -0.0000, -0.0000, 11.4698, 22.9074, 34.2043, 45.2514,
};

static s16 Animation_for_R_shoe_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_shoe_motion_ch3_val[]={
  0.0000, 0.0000, 0.0000, -0.0000, 0.0000, -0.0000, -0.0000, -0.0000,
  -0.0000, -0.0000, 0.0000, -0.0000, -0.0000, -0.0000, -0.0000, -0.0000,
  0.0000, 0.0000, -0.0000, -0.0000, 0.0000, -0.0000, -0.0000, -0.0000,
  0.0000, -0.0000, -0.0000, 0.0886, 0.1777, 0.2673, 0.3575,
};

static s16 Animation_for_R_shoe_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_shoe_motion_ch4_val[]={
  -0.0000, -0.0000, 0.0000, -0.0000, 0.0000, -0.0000, -0.0000, 0.0000,
  0.0000, 0.0000, -0.0000, 0.0000, -0.0000, -0.0000, -0.0000, 0.0000,
  0.0000, 0.0000, -0.0000, 0.0000, -0.0000, 0.0000, 0.0000, -0.0000,
  0.0000, -0.0000, 0.0000, 0.0183, 0.0352, 0.0507, 0.0649,
};

static s16 Animation_for_R_shoe_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_shoe_motion_ch5_val[]={
  0.0000, 0.0000, 0.0000, 0.0000, -0.0000, -0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, -0.0000, 0.0000, 0.0000, -0.0000, 0.0000,
  -0.0000, 0.0000, -0.0000, 0.0000, 0.0000, 0.0000, -0.0000, 0.0000,
  0.0000, -0.0000, -0.0000, 0.0155, 0.0310, 0.0462, 0.0611,
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
  0.0000, -0.2505, -1.1691, -2.7093, -4.8083, -7.3907, -6.0414, -4.8026,
  -3.6849, -2.6984, -1.8526, -1.1561, -0.6164, -0.2403, -0.0333, 0.0005,
  -0.0178, -0.0286, -0.0317, -0.0270, -0.0146, 0.0052, 0.0316, 0.0639,
  0.1009, 0.1413, 0.1835, 0.2078, 0.2272, 0.2417, 0.2512,
};

static s16 Animation_for_R_upleg_motion_ch1_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_upleg_motion_ch1_val[]={
  0.0000, 4.1290, 8.1353, 11.9103, 15.3583, 18.3997, 16.9296, 15.3479,
  13.6630, 11.8842, 10.0220, 8.0881, 6.0948, 4.0555, 1.9844, -0.1036,
  -0.2443, -0.3976, -0.5601, -0.7281, -0.8979, -1.0660, -1.2285, -1.3817,
  -1.5221, -1.6461, -1.7502, -1.9750, -2.1852, -2.3804, -2.5605,
};

static s16 Animation_for_R_upleg_motion_ch2_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_upleg_motion_ch2_val[]={
  0.0000, -0.4552, -0.7782, -0.9734, -1.0499, -1.0205, -1.0419, -1.0398,
  -1.0123, -0.9580, -0.8754, -0.7632, -0.6205, -0.4465, -0.2410, -0.0038,
  0.0911, 0.1394, 0.1401, 0.0921, -0.0050, -0.1517, -0.3481, -0.5941,
  -0.8893, -1.2329, -1.6241, -1.9883, -2.3629, -2.7471, -3.1405,
};

static s16 Animation_for_R_upleg_motion_ch3_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_upleg_motion_ch3_val[]={
  0.0000, -0.0320, -0.0650, -0.1001, -0.1380, -0.1796, -0.1562, -0.1338,
  -0.1123, -0.0916, -0.0717, -0.0524, -0.0336, -0.0151, 0.0031, 0.0211,
  0.0842, 0.1473, 0.2104, 0.2734, 0.3363, 0.3991, 0.4618, 0.5243, 0.5867,
  0.6488, 0.7107, 0.7463, 0.7819, 0.8175, 0.8530,
};

static s16 Animation_for_R_upleg_motion_ch4_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_upleg_motion_ch4_val[]={
  0.0000, 0.0174, 0.0295, 0.0364, 0.0385, 0.0362, 0.0377, 0.0382, 0.0377,
  0.0360, 0.0331, 0.0290, 0.0237, 0.0171, 0.0093, 0.0001, -0.0041, -0.0073,
  -0.0094, -0.0103, -0.0099, -0.0084, -0.0056, -0.0015, 0.0039, 0.0106,
  0.0186, 0.0283, 0.0382, 0.0483, 0.0586,
};

static s16 Animation_for_R_upleg_motion_ch5_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_R_upleg_motion_ch5_val[]={
  0.0000, 0.1531, 0.3047, 0.4544, 0.6019, 0.7471, 0.6750, 0.6022, 0.5288,
  0.4548, 0.3802, 0.3051, 0.2294, 0.1533, 0.0768, -0.0002, 0.0058, 0.0113,
  0.0163, 0.0209, 0.0252, 0.0293, 0.0334, 0.0374, 0.0415, 0.0458, 0.0505,
  0.0466, 0.0432, 0.0401, 0.0375,
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
static u16 _body_link[]={ 1, 4, 5, 8, 9, 14, 10, 13 };
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
  {0.000000,0.000000,0.000000,
   0.000000,0.000000,0.000000,
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
  {-0.000005,0.000000,0.000001,
   0.000000,0.000000,-0.000000,
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
  {-0.000005,0.000004,-0.000001,
   0.000000,0.000000,0.000000,
   1.000000,1.000000,1.000000},
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
  {0.000000,0.000000,0.000000,
   0.000000,0.000000,0.000000,
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
  {0.000000,0.000000,0.000000,
   0.000000,0.000000,0.000000,
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
  {0.000000,0.000005,0.000000,
   0.000000,0.000000,-0.000000,
   1.000000,1.000000,1.000000},
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
  {0.000000,-0.000010,0.000000,
   -0.000000,-0.000000,0.000000,
   1.000000,1.000000,1.000000},
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
  {0.000000,0.000000,0.000000,
   0.000000,0.000000,0.000000,
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
  {0.000000,0.000000,-0.000000,
   -0.000000,0.000000,0.000000,
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
  {0.000000,0.000005,0.000000,
   0.000000,-0.000000,-0.000000,
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
  {0.000000,-0.000005,0.000000,
   -0.000000,0.000000,0.000000,
   1.000000,1.000000,1.000000},
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
  {0.000000,0.000000,-0.000000,
   -0.000000,0.000000,0.000000,
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
static NDR_SCN_INST_OBJ down_scene_instlist[]={
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

static NDR_SCN_LIGHT _down_scene_light[]={
  { 0, 0xffffff00, 0.57735, 0.57735, 0.57735, 0, 0 }
};

static NDR_SCENE down_scene={
  {950.000000, 1000.000000, 0x4c, 0x4c, 0x4c, 0xff},
  {1, 45.841393, 1.333330, 1.000000, 0, 0, 0, 0, 1, 10000, 0, 1, 2},
  { 0x202020ff, 1, _down_scene_light },
  0x0000, 0xfffc,
  down_scene_instlist, 4,
};

/* ============================================================
          Shape Structure List
 ============================================================ */
static NDR_SHAPE *shapelist_frake_down[] = {
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
static NDR_ANIM *animlist_frake_down[]={
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
static NDR_OBJ *objlist_frake_down[]={
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
NDR_RESINFO resinfo_frake_down={
  &down_scene,
  20.000000,
  objlist_frake_down,
  shapelist_frake_down,
  animlist_frake_down,
  NULL,
  NULL,
  NULL,
  23, 19, 19, 0, 0, 0
};

