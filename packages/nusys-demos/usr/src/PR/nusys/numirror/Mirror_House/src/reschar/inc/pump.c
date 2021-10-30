
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
extern NDR_RESINFO resinfo_pump;
#define RESINFO_NAME resinfo_pump

static Gfx _pump_aligner0[] = { gsSPEndDisplayList() };

static Gfx _pump_aligner1[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_139_[] = {
  {    -6,    29,     0,0, 0x0000,0x0000, 0xff,0xff,0xff,0xff }, /*     0 */
  {     6,     3,     0,0, 0x0000,0x0000, 0xff,0xff,0xff,0xff }, /*     1 */
  {     6,    29,     0,0, 0x0000,0x0000, 0xff,0xff,0xff,0xff }, /*     2 */
  {    -6,    29,     0,0, 0x0000,0x0000, 0xff,0xff,0xff,0xff }, /*     3 */
  {    -6,     3,     0,0, 0x0000,0x0000, 0xc3,0xc3,0xc3,0xff }, /*     4 */
  {     6,     3,     0,0, 0x0000,0x0000, 0xff,0xff,0xff,0xff }, /*     5 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_139_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 6, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP1Triangle(  3,  4,  5,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_139_ = {
  gfx_Shape_of___POLYHEDRON_139_,
  vtx_Shape_of___POLYHEDRON_139_, 6, 5,
  NULL,
  NULL,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _pump_aligner2[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_136_[] = {
  {     0,    37,    28,0, 0x0000,0x0000, 0xff,0xff,0x00,0xff }, /*     0 */
  {    24,    24,   -14,0, 0x0000,0x0000, 0xff,0xff,0x00,0xff }, /*     1 */
  {    24,    37,   -14,0, 0x0000,0x0000, 0xff,0xff,0x00,0xff }, /*     2 */
  {     0,    37,    28,0, 0x0000,0x0000, 0xff,0xff,0x00,0xff }, /*     3 */
  {    24,    37,   -14,0, 0x0000,0x0000, 0xff,0xff,0x00,0xff }, /*     4 */
  {   -24,    37,   -14,0, 0x0000,0x0000, 0xec,0xec,0x00,0xff }, /*     5 */
  {     0,    37,    28,0, 0x0000,0x0000, 0xff,0xff,0x00,0xff }, /*     6 */
  {     0,    24,    28,0, 0x0000,0x0000, 0xb3,0xb3,0x00,0xff }, /*     7 */
  {    24,    24,   -14,0, 0x0000,0x0000, 0xff,0xff,0x00,0xff }, /*     8 */
  {   -24,    37,   -14,0, 0x0000,0x0000, 0xec,0xec,0x00,0xff }, /*     9 */
  {     0,    24,    28,0, 0x0000,0x0000, 0xb3,0xb3,0x00,0xff }, /*    10 */
  {     0,    37,    28,0, 0x0000,0x0000, 0xff,0xff,0x00,0xff }, /*    11 */
  {   -24,    37,   -14,0, 0x0000,0x0000, 0xec,0xec,0x00,0xff }, /*    12 */
  {   -24,    24,   -14,0, 0x0000,0x0000, 0xe0,0xe0,0x00,0xff }, /*    13 */
  {     0,    24,    28,0, 0x0000,0x0000, 0xb3,0xb3,0x00,0xff }, /*    14 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_136_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 15, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP1Triangle(  6,  7,  8,  0),
  gsSP2Triangles(  9, 10, 11,  0, 12, 13, 14,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_136_ = {
  gfx_Shape_of___POLYHEDRON_136_,
  vtx_Shape_of___POLYHEDRON_136_, 15, 5,
  NULL,
  NULL,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _pump_aligner3[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_146_[] = {
  {   -41,    -9,   -14,0, 0x0000,0x0000, 0x00,0x00,0x7f,0xff }, /*     0 */
  {   -65,     2,   -14,0, 0x0000,0x0000, 0x97,0x46,0x00,0xff }, /*     1 */
  {   -41,   -17,   -14,0, 0x0000,0x0000, 0xd4,0x89,0x00,0xff }, /*     2 */
  {   -41,   -17,   -14,0, 0x0000,0x0000, 0x00,0x00,0x7f,0xff }, /*     3 */
  {   -17,    -9,   -14,0, 0x0000,0x0000, 0x59,0x59,0x00,0xff }, /*     4 */
  {   -41,    -9,   -14,0, 0x0000,0x0000, 0x1d,0x7b,0x00,0xff }, /*     5 */
  {   -41,   -17,   -14,0, 0x0000,0x0000, 0x00,0x00,0x7f,0xff }, /*     6 */
  {   -17,   -17,   -14,0, 0x0000,0x0000, 0x59,0xa7,0x00,0xff }, /*     7 */
  {   -17,    -9,   -14,0, 0x0000,0x0000, 0x59,0x59,0x00,0xff }, /*     8 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_146_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsDPSetEnvColor(0x00, 0xff, 0xff, 0xff),
  gsDPSetCombineLERP(1, 0, ENVIRONMENT, 0, 0, 0, ENVIRONMENT, 0, 1, 0, ENVIRONMENT, 0, 0, 0, ENVIRONMENT, 0),
  gsSPVertex( SEGADRS(5, 0), 9, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP1Triangle(  6,  7,  8,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_146_ = {
  gfx_Shape_of___POLYHEDRON_146_,
  vtx_Shape_of___POLYHEDRON_146_, 9, 5,
  NULL,
  NULL,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

static Gfx _pump_aligner4[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_133_[] = {
  {     0,    23,    28,0, 0x0000,0x0000, 0xff,0x00,0x00,0xff }, /*     0 */
  {    24,   -23,   -14,0, 0x0000,0x0000, 0xff,0x00,0x00,0xff }, /*     1 */
  {    24,    23,   -14,0, 0x0000,0x0000, 0xff,0x00,0x00,0xff }, /*     2 */
  {     0,    23,    28,0, 0x0000,0x0000, 0xff,0x00,0x00,0xff }, /*     3 */
  {    24,    23,   -14,0, 0x0000,0x0000, 0xff,0x00,0x00,0xff }, /*     4 */
  {   -24,    23,   -14,0, 0x0000,0x0000, 0xef,0x00,0x00,0xff }, /*     5 */
  {     0,    23,    28,0, 0x0000,0x0000, 0xff,0x00,0x00,0xff }, /*     6 */
  {     0,   -23,    28,0, 0x0000,0x0000, 0xad,0x00,0x00,0xff }, /*     7 */
  {    24,   -23,   -14,0, 0x0000,0x0000, 0xff,0x00,0x00,0xff }, /*     8 */
  {   -24,    23,   -14,0, 0x0000,0x0000, 0xef,0x00,0x00,0xff }, /*     9 */
  {     0,   -23,    28,0, 0x0000,0x0000, 0xad,0x00,0x00,0xff }, /*    10 */
  {     0,    23,    28,0, 0x0000,0x0000, 0xff,0x00,0x00,0xff }, /*    11 */
  {   -24,    23,   -14,0, 0x0000,0x0000, 0xef,0x00,0x00,0xff }, /*    12 */
  {   -24,   -23,   -14,0, 0x0000,0x0000, 0xd6,0x00,0x00,0xff }, /*    13 */
  {     0,   -23,    28,0, 0x0000,0x0000, 0xad,0x00,0x00,0xff }, /*    14 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_133_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(1, 0, SHADE, 0, 0, 0, 0, SHADE, 1, 0, SHADE, 0, 0, 0, 0, SHADE),
  gsSPVertex( SEGADRS(5, 0), 15, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP1Triangle(  6,  7,  8,  0),
  gsSP2Triangles(  9, 10, 11,  0, 12, 13, 14,  0),
  gsSPEndDisplayList(),
};

/* ============================================================
          Shape Structure
 ============================================================ */
static NDR_SHAPE shape_Shape_of___POLYHEDRON_133_ = {
  gfx_Shape_of___POLYHEDRON_133_,
  vtx_Shape_of___POLYHEDRON_133_, 15, 5,
  NULL,
  NULL,
  6, 7,
  NULL,
  NULL,
  NULL,
  NULL
};

/* ============================================================
          Geometric Animation Data Area
 ============================================================ */
static s16 Animation_for_pumpfuta_Group_motion_ch0_key[]={
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
};
static float Animation_for_pumpfuta_Group_motion_ch0_val[]={
  0.0000, 4.0000, 8.0000, 12.0000, 16.0000, 20.0000, 16.0000, 12.0000,
  8.0000, 4.0000, 0.0000, 4.0000, 8.0000, 12.0000, 16.0000, 20.0000,
  16.0000, 12.0000, 8.0000, 4.0000, 0.0000, 0.0000, 0.0000, 0.0000,
  0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
};

static NDR_ANIM_CHANNEL Animation_for_pumpfuta_Group_motion_chlist[]={
  {NDC_ANIMKIND_TRS_Y, 0, 30, 31,
   Animation_for_pumpfuta_Group_motion_ch0_key, Animation_for_pumpfuta_Group_motion_ch0_val},
};

static NDR_ANIM Animation_for_pumpfuta_Group_motion={
  Animation_for_pumpfuta_Group_motion_chlist, 1, 30
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
u16 _pumpGroup_link[]={ 1, 4, 5 };
static NDR_OBJ _pumpGroup={
  NDC_TYPE_NULL, 0, 0x00000000,
  {0.000000,0.000000,0.000000,
   0.000000,0.000000,0.000000,
   1.000000,1.000000,1.000000},
  -1,
  0x0000, 0x00000000, 0x00000000, 0x00000000,
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  -1, 0, 2, 0x010203,
  {-1, -1}, 3, _pumpGroup_link
};

/* Object #2 */
u16 _pumpfuta_Group_link[]={ 1, 2 };
static NDR_OBJ _pumpfuta_Group={
  NDC_TYPE_NULL, 0, 0x00000000,
  {0.000000,0.000000,0.000000,
   0.000000,0.000000,0.000000,
   1.000000,1.000000,1.000000},
  -1,
  0x0000, 0x00000000, 0x00000000, 0x00000000,
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  0, 0, 2, 0x010203,
  {-1, -1}, 2, _pumpfuta_Group_link
};

/* Object #3 */
static NDR_OBJ _futaparts={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {0.000000,0.000000,0.000000,
   0.000000,0.000000,0.000000,
   1.000000,1.000000,1.000000},
  0,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  -1, 0, 2, 0x010203,
  {-1, -1}, 0, NULL
};

/* Object #4 */
static NDR_OBJ _pumpfuta={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {0.000000,0.000000,0.000000,
   0.000000,0.000000,0.000000,
   1.000000,1.000000,1.000000},
  1,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  -1, 0, 2, 0x010203,
  {-1, -1}, 0, NULL
};

/* Object #5 */
static NDR_OBJ _tube={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {0.000000,0.000000,0.000000,
   0.000000,0.000000,0.000000,
   1.000000,1.000000,1.000000},
  2,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  -1, 0, 2, 0x010203,
  {-1, -1}, 0, NULL
};

/* Object #6 */
static NDR_OBJ _pumpbody={
  NDC_TYPE_3D, 0, 0xd00 + 0x00,
  {0.000000,0.000000,0.000000,
   0.000000,0.000000,0.000000,
   1.000000,1.000000,1.000000},
  3,
  0x0000, 0x00002001, 0xf0800505, 0x00000000,
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  {0x0f,0x0f,0x0f,0x04,0x0f,0x0f,0x0f,0x04},
  -1, 0, 2, 0x010203,
  {-1, -1}, 0, NULL
};

/* Object #7 */
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

/* Object #8 */
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

/* Object #9 */
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
static NDR_SCN_INST_OBJ pump_scene_instlist[]={
 {
  {0,0,0, 0,0,0, 1,1,1},
  7 /* NULL_OBJECT_1 */
 },
 {
  {0,0,0, 0,0,0, 1,1,1},
  9 /* NULL_OBJECT_3 */
 },
 {
  {0,0,0, 0,0,0, 1,1,1},
  8 /* NULL_OBJECT_2 */
 },
 {
  {0.000000,0.000000,0.000000,
   0.000000,0.000000,0.000000,
   1.000000,1.000000,1.000000},
  1 /* pumpGroup */
 },
};

static NDR_SCN_LIGHT _pump_scene_light[]={
  { 0, 0xffffff00, 0.57735, 0.57735, 0.57735, 0, 0 }
};

static NDR_SCENE pump_scene={
  {950.000000, 1000.000000, 0x4c, 0x4c, 0x4c, 0xff},
  {1, 45.841393, 1.333330, 1.000000, 0, 0, 0, 0, 1, 10000, 0, 1, 2},
  { 0x202020ff, 1, _pump_scene_light },
  0x0000, 0xfffc,
  pump_scene_instlist, 4,
};

/* ============================================================
          Shape Structure List
 ============================================================ */
static NDR_SHAPE *shapelist_pump[] = {
  &shape_Shape_of___POLYHEDRON_139_,
  &shape_Shape_of___POLYHEDRON_136_,
  &shape_Shape_of___POLYHEDRON_146_,
  &shape_Shape_of___POLYHEDRON_133_,
};

/* ============================================================
          Geometric Animation List
 ============================================================ */
static NDR_ANIM *animlist_pump[]={
  &Animation_for_pumpfuta_Group_motion,
};

/* ============================================================
          Object Struct List
 ============================================================ */
static NDR_OBJ *objlist_pump[]={
  &_dummy_obj, /* 0 */
  &_pumpGroup, /* 1 */
  &_pumpfuta_Group, /* 2 */
  &_futaparts, /* 3 */
  &_pumpfuta, /* 4 */
  &_tube, /* 5 */
  &_pumpbody, /* 6 */
  &_NULL_OBJECT_1, /* 7 */
  &_NULL_OBJECT_2, /* 8 */
  &_NULL_OBJECT_3, /* 9 */
};

/* ############################################################
          Resource Information
 ############################################################ */
NDR_RESINFO resinfo_pump={
  &pump_scene,
  20.000000,
  objlist_pump,
  shapelist_pump,
  animlist_pump,
  NULL,
  NULL,
  NULL,
  10, 4, 1, 0, 0, 0
};

