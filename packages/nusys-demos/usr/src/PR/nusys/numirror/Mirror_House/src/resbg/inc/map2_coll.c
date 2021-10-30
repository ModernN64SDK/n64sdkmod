static Gfx _map2_collision_group_niff_aligner0[] = { gsSPEndDisplayList() };

static Gfx _map2_collision_group_niff_aligner1[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_8866_[] = {
  {  4000,     0,  4000,0, 0x0800,0x0000, 0xc7,0xc7,0xc7,0xff }, /*     0 */
  {  6000,     0,  6000,0, 0x0000,0x0800, 0xa9,0xa9,0xa9,0xff }, /*     1 */
  {  8000,     0,  4000,0, 0xf800,0x0000, 0xaf,0xaf,0xaf,0xff }, /*     2 */
  {  6000,     0,  8000,0, 0x0000,0x0000, 0x8f,0x8f,0x8f,0xff }, /*     3 */
  {  8000,     0,  6000,0, 0x0800,0x0800, 0xa4,0xa4,0xa4,0xff }, /*     4 */
  {  6000,     0,  6000,0, 0x0000,0x0800, 0xa9,0xa9,0xa9,0xff }, /*     5 */
  {  6000,     0,  6000,0, 0x0000,0x0800, 0xa9,0xa9,0xa9,0xff }, /*     6 */
  {  8000,     0,  6000,0, 0xf801,0x0800, 0xa4,0xa4,0xa4,0xff }, /*     7 */
  {  8000,     0,  4000,0, 0xf800,0x0000, 0xaf,0xaf,0xaf,0xff }, /*     8 */
  {  6000,     0,  6000,0, 0x0000,0x0800, 0xa9,0xa9,0xa9,0xff }, /*     9 */
  {  4000,   555,  8000,0, 0x0800,0x0000, 0x79,0x79,0x79,0xff }, /*    10 */
  {  6000,     0,  8000,0, 0x0000,0x0000, 0x8f,0x8f,0x8f,0xff }, /*    11 */
  {  6000,     0,  6000,0, 0x0000,0x0800, 0xa9,0xa9,0xa9,0xff }, /*    12 */
  {  4000,   555,  6000,0, 0x0800,0x0800, 0xb8,0xb8,0xb8,0xff }, /*    13 */
  {  4000,   555,  8000,0, 0x0800,0x0000, 0x79,0x79,0x79,0xff }, /*    14 */
  {  4000,     0,  4000,0, 0x0800,0x0000, 0xc7,0xc7,0xc7,0xff }, /*    15 */
  {  4000,   555,  6000,0, 0x0800,0x07ff, 0xb8,0xb8,0xb8,0xff }, /*    16 */
  {  6000,     0,  6000,0, 0x0000,0x0800, 0xa9,0xa9,0xa9,0xff }, /*    17 */
  {  6000,     0,  8000,0, 0x0000,0x0000, 0x8f,0x8f,0x8f,0xff }, /*    18 */
  {  8000,     0,  8000,0, 0x0800,0x0000, 0x97,0x97,0x97,0xff }, /*    19 */
  {  8000,     0,  6000,0, 0x0800,0x0800, 0xa4,0xa4,0xa4,0xff }, /*    20 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_8866_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0, TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0),
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,0), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    2, 2, 5, 5, 0, 0 ),
  gsSPVertex( SEGADRS(5, 0), 21, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP2Triangles(  3,  4,  5,  0,  6,  7,  8,  0),
  gsSP2Triangles(  9, 10, 11,  0, 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSPEndDisplayList(),
};

static Gfx _map2_collision_group_niff_aligner2[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_8898_[] = {
  {  4000,     0,  4000,0, 0x0800,0x0000, 0xcb,0xcb,0xcb,0xff }, /*     0 */
  {     3,     0,  4000,0, 0xf804,0x0000, 0xd5,0xd5,0xd5,0xff }, /*     1 */
  {  4000,   555,  6000,0, 0x0800,0x0800, 0xbc,0xbc,0xbc,0xff }, /*     2 */
  {     3,     0,  4000,0, 0xf804,0x0000, 0xd5,0xd5,0xd5,0xff }, /*     3 */
  {     0,   555,  6000,0, 0xf800,0x0800, 0xc2,0xc2,0xc2,0xff }, /*     4 */
  {  4000,   555,  6000,0, 0x0800,0x0800, 0xbc,0xbc,0xbc,0xff }, /*     5 */
  {     0,   555,  6000,0, 0x0800,0x0800, 0xc2,0xc2,0xc2,0xff }, /*     6 */
  {  4000,   555,  8000,0, 0xf800,0x0000, 0x79,0x79,0x79,0xff }, /*     7 */
  {  4000,   555,  6000,0, 0xf800,0x0800, 0xbc,0xbc,0xbc,0xff }, /*     8 */
  {     0,   555,  6000,0, 0x0800,0x0800, 0xc2,0xc2,0xc2,0xff }, /*     9 */
  {     0,   555,  8000,0, 0x07ff,0x0000, 0x7e,0x7e,0x7e,0xff }, /*    10 */
  {  4000,   555,  8000,0, 0xf800,0x0000, 0x79,0x79,0x79,0xff }, /*    11 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_8898_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0, TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0),
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,0), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    2, 2, 5, 5, 0, 0 ),
  gsSPVertex( SEGADRS(5, 0), 12, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP2Triangles(  6,  7,  8,  0,  9, 10, 11,  0),
  gsSPEndDisplayList(),
};

static Gfx _map2_collision_group_niff_aligner3[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_8930_[] = {
  {     0,   555,  6000,0, 0x0800,0x0800, 0xbf,0xbf,0xbf,0xff }, /*     0 */
  { -2000,   555,  8000,0, 0x0000,0x0000, 0x72,0x72,0x72,0xff }, /*     1 */
  {     0,   555,  8000,0, 0x0800,0x0000, 0x81,0x81,0x81,0xff }, /*     2 */
  {     0,   555,  6000,0, 0x0800,0x0800, 0xbf,0xbf,0xbf,0xff }, /*     3 */
  { -2000,   555,  6000,0, 0x0000,0x0800, 0xb2,0xb2,0xb2,0xff }, /*     4 */
  { -2000,   555,  8000,0, 0x0000,0x0000, 0x72,0x72,0x72,0xff }, /*     5 */
  {     3,     0,  4000,0, 0x0803,0xfc01, 0xd5,0xd5,0xd5,0xff }, /*     6 */
  { -2000,     0,  4000,0, 0x0000,0xfc01, 0xcc,0xcc,0xcc,0xff }, /*     7 */
  {     0,   555,  6000,0, 0x0800,0x0800, 0xbf,0xbf,0xbf,0xff }, /*     8 */
  { -4000,     0,  4666,0, 0x0800,0x0000, 0xad,0xad,0xad,0xff }, /*     9 */
  { -4000,     0,  6000,0, 0x0800,0x0800, 0x9d,0x9d,0x9d,0xff }, /*    10 */
  { -2000,   185,  4666,0, 0x0000,0x0000, 0xd1,0xd1,0xd1,0xff }, /*    11 */
  { -4000,     0,  6000,0, 0x0800,0x0800, 0x9d,0x9d,0x9d,0xff }, /*    12 */
  { -2000,   555,  8000,0, 0x0000,0x0000, 0x72,0x72,0x72,0xff }, /*    13 */
  { -2000,   555,  6000,0, 0x0000,0x0800, 0xb2,0xb2,0xb2,0xff }, /*    14 */
  { -4000,     0,  6000,0, 0x0800,0x0800, 0x9d,0x9d,0x9d,0xff }, /*    15 */
  { -2000,   555,  6000,0, 0x0000,0x0800, 0xb2,0xb2,0xb2,0xff }, /*    16 */
  { -2000,   185,  4666,0, 0x0000,0x0000, 0xd1,0xd1,0xd1,0xff }, /*    17 */
  { -4000,     0,  6000,0, 0x0800,0x0800, 0x9d,0x9d,0x9d,0xff }, /*    18 */
  { -4000,     0,  8000,0, 0x0800,0x0000, 0x87,0x87,0x87,0xff }, /*    19 */
  { -2000,   555,  8000,0, 0x0000,0x0000, 0x72,0x72,0x72,0xff }, /*    20 */
  { -2000,     0,  4000,0, 0x0000,0xfc01, 0xcc,0xcc,0xcc,0xff }, /*    21 */
  { -2000,   185,  4666,0, 0x0000,0x0000, 0xd1,0xd1,0xd1,0xff }, /*    22 */
  {     0,   555,  6000,0, 0x0800,0x0800, 0xbf,0xbf,0xbf,0xff }, /*    23 */
  { -2000,   185,  4666,0, 0x0000,0x0000, 0xd1,0xd1,0xd1,0xff }, /*    24 */
  { -2000,   555,  6000,0, 0x0000,0x0800, 0xb2,0xb2,0xb2,0xff }, /*    25 */
  {     0,   555,  6000,0, 0x0800,0x0800, 0xbf,0xbf,0xbf,0xff }, /*    26 */
  { -2000,   185,  4666,0, 0x0000,0x0040, 0xd1,0xd1,0xd1,0xff }, /*    27 */
  { -2000,     0,  4000,0, 0x07c0,0x0040, 0xcc,0xcc,0xcc,0xff }, /*    28 */
  { -4000,     0,  4666,0, 0x0000,0x0800, 0xad,0xad,0xad,0xff }, /*    29 */
  { -2000,     0,  4000,0, 0x07c0,0x0040, 0xcc,0xcc,0xcc,0xff }, /*    30 */
  { -4000,     0,  4000,0, 0x07c0,0x0800, 0xb2,0xb2,0xb2,0xff }, /*    31 */
  { -4000,     0,  4666,0, 0x0000,0x0800, 0xad,0xad,0xad,0xff }, /*    32 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_8930_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0, TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0),
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,0), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    2, 2, 5, 5, 0, 0 ),
  gsSPVertex( SEGADRS(5, 0), 32, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP1Triangle(  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSP1Triangle( 24, 25, 26,  0),
  gsDPPipeSync(),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,2048), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    0, 0, 5, 5, 0, 0 ),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 1, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSPEndDisplayList(),
};

static Gfx _map2_collision_group_niff_aligner4[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_8960_[] = {
  { -8000,     0,  4000,0, 0xf800,0x1000, 0x9c,0x9c,0x9c,0xff }, /*     0 */
  { -8000,     0,  8000,0, 0xf800,0x0000, 0x89,0x89,0x89,0xff }, /*     1 */
  { -4000,     0,  4000,0, 0x0800,0x1000, 0xb4,0xb4,0xb4,0xff }, /*     2 */
  { -8000,     0,  8000,0, 0xf800,0x0000, 0x89,0x89,0x89,0xff }, /*     3 */
  { -4000,     0,  6000,0, 0x0800,0x0800, 0x96,0x96,0x96,0xff }, /*     4 */
  { -4000,     0,  4000,0, 0x0800,0x1000, 0xb4,0xb4,0xb4,0xff }, /*     5 */
  { -8000,     0,  8000,0, 0xf800,0x0000, 0x89,0x89,0x89,0xff }, /*     6 */
  { -4000,     0,  8000,0, 0x0800,0x0000, 0x87,0x87,0x87,0xff }, /*     7 */
  { -4000,     0,  6000,0, 0x0800,0x0800, 0x96,0x96,0x96,0xff }, /*     8 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_8960_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0, TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0),
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,0), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    2, 2, 5, 5, 0, 0 ),
  gsSPVertex( SEGADRS(5, 0), 9, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP2Triangles(  3,  4,  5,  0,  6,  7,  8,  0),
  gsSPEndDisplayList(),
};

static Gfx _map2_collision_group_niff_aligner5[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_8987_[] = {
  {  8000,     0,  4000,0, 0x0800,0x0000, 0xaf,0xaf,0xaf,0xff }, /*     0 */
  {  4000,    86,  2000,0, 0xf800,0x0800, 0x92,0x92,0x92,0xff }, /*     1 */
  {  4000,     0,  4000,0, 0xf800,0x0000, 0xcc,0xcc,0xcc,0xff }, /*     2 */
  {  8000,     0,  4000,0, 0x0800,0x0000, 0xaf,0xaf,0xaf,0xff }, /*     3 */
  {  8000,    86,  2000,0, 0x0800,0x0800, 0x85,0x85,0x85,0xff }, /*     4 */
  {  4000,    86,  2000,0, 0xf800,0x0800, 0x92,0x92,0x92,0xff }, /*     5 */
  {  4000,  1843,     0,0, 0x0800,0x0000, 0x6f,0x6f,0x6f,0xff }, /*     6 */
  {  8000,    86,  2000,0, 0xf800,0x0800, 0x85,0x85,0x85,0xff }, /*     7 */
  {  8000,  1843,     0,0, 0xf800,0x0000, 0x41,0x41,0x41,0xff }, /*     8 */
  {  4000,  1843,     0,0, 0x0800,0x0000, 0x6f,0x6f,0x6f,0xff }, /*     9 */
  {  4000,    86,  2000,0, 0x0800,0x0800, 0x92,0x92,0x92,0xff }, /*    10 */
  {  8000,    86,  2000,0, 0xf800,0x0800, 0x85,0x85,0x85,0xff }, /*    11 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_8987_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0, TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0),
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,0), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    2, 2, 5, 5, 0, 0 ),
  gsSPVertex( SEGADRS(5, 0), 12, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP1Triangle(  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsSPEndDisplayList(),
};

static Gfx _map2_collision_group_niff_aligner6[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_9018_[] = {
  {  2000,  1843,     0,0, 0xf000,0x0000, 0x6d,0x6d,0x6d,0xff }, /*     0 */
  {  4000,    86,  2000,0, 0x0800,0x0800, 0x8e,0x8e,0x8e,0xff }, /*     1 */
  {  4000,  1843,     0,0, 0x0800,0x0000, 0x6a,0x6a,0x6a,0xff }, /*     2 */
  {  4000,    86,  2000,0, 0x0800,0x0800, 0x8e,0x8e,0x8e,0xff }, /*     3 */
  {  2000,     0,  4000,0, 0xfc01,0x0000, 0xd5,0xd5,0xd5,0xff }, /*     4 */
  {  4000,     0,  4000,0, 0x0800,0x0000, 0xcc,0xcc,0xcc,0xff }, /*     5 */
  {  4000,    86,  2000,0, 0x0800,0x0800, 0x8e,0x8e,0x8e,0xff }, /*     6 */
  {  2000,    86,  2000,0, 0xfc01,0x0800, 0xa6,0xa6,0xa6,0xff }, /*     7 */
  {  2000,     0,  4000,0, 0xfc01,0x0000, 0xd5,0xd5,0xd5,0xff }, /*     8 */
  {  2000,  1843,     0,0, 0xf000,0x0000, 0x6d,0x6d,0x6d,0xff }, /*     9 */
  {  2000,    86,  2000,0, 0xf000,0x0800, 0xa6,0xa6,0xa6,0xff }, /*    10 */
  {  4000,    86,  2000,0, 0x0800,0x0800, 0x8e,0x8e,0x8e,0xff }, /*    11 */
  {     0,     0,  2000,0, 0x0800,0x0800, 0xd3,0xd3,0xd3,0xff }, /*    12 */
  {     3,     0,  4000,0, 0x07fc,0x0000, 0xd5,0xd5,0xd5,0xff }, /*    13 */
  {  2000,    86,  2000,0, 0x0000,0x0800, 0xa6,0xa6,0xa6,0xff }, /*    14 */
  {     0,     0,     0,0, 0x0800,0x0000, 0xc5,0xc5,0xc5,0xff }, /*    15 */
  {  2000,    86,  2000,0, 0x0000,0x0800, 0xa6,0xa6,0xa6,0xff }, /*    16 */
  {  2000,  1843,     0,0, 0x0000,0x0000, 0x6d,0x6d,0x6d,0xff }, /*    17 */
  {     0,     0,     0,0, 0x0800,0x0000, 0xc5,0xc5,0xc5,0xff }, /*    18 */
  {     0,     0,  2000,0, 0x0800,0x0800, 0xd3,0xd3,0xd3,0xff }, /*    19 */
  {  2000,    86,  2000,0, 0x0000,0x0800, 0xa6,0xa6,0xa6,0xff }, /*    20 */
  {     3,     0,  4000,0, 0x07fc,0x0000, 0xd5,0xd5,0xd5,0xff }, /*    21 */
  {  2000,     0,  4000,0, 0x0000,0x0000, 0xd5,0xd5,0xd5,0xff }, /*    22 */
  {  2000,    86,  2000,0, 0x0000,0x0800, 0xa6,0xa6,0xa6,0xff }, /*    23 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_9018_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0, TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0),
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,0), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    2, 2, 5, 5, 0, 0 ),
  gsSPVertex( SEGADRS(5, 0), 24, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP2Triangles(  3,  4,  5,  0,  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsDPPipeSync(),
  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,4096), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    2, 2, 5, 5, 0, 0 ),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSPEndDisplayList(),
};

static Gfx _map2_collision_group_niff_aligner7[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_9048_[] = {
  { -4000,     0,     0,0, 0x0800,0x0000, 0xcd,0xcd,0xcd,0xff }, /*     0 */
  { -4000,     0,  4000,0, 0x0800,0x1000, 0xb4,0xb4,0xb4,0xff }, /*     1 */
  {     0,     0,     0,0, 0xf800,0x0000, 0xc5,0xc5,0xc5,0xff }, /*     2 */
  { -4000,     0,  4000,0, 0x0800,0x1000, 0xb4,0xb4,0xb4,0xff }, /*     3 */
  {     3,     0,  4000,0, 0xf7fd,0x1000, 0xd5,0xd5,0xd5,0xff }, /*     4 */
  {     0,     0,     0,0, 0xf800,0x0000, 0xc5,0xc5,0xc5,0xff }, /*     5 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_9048_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0),
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,4096), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    2, 2, 5, 5, 0, 0 ),
  gsSPVertex( SEGADRS(5, 0), 6, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP1Triangle(  3,  4,  5,  0),
  gsSPEndDisplayList(),
};

static Gfx _map2_collision_group_niff_aligner8[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_9072_[] = {
  { -4000,     0,     0,0, 0x0000,0x0800, 0xcd,0xcd,0xcd,0xff }, /*     0 */
  { -8000,     0,     0,0, 0x0000,0x0800, 0xab,0xab,0xab,0xff }, /*     1 */
  { -8000,     0,  4000,0, 0x0000,0x0800, 0x9c,0x9c,0x9c,0xff }, /*     2 */
  { -8000,     0,  4000,0, 0x0000,0x0800, 0x9c,0x9c,0x9c,0xff }, /*     3 */
  { -4000,     0,  4000,0, 0x0000,0x0800, 0xb4,0xb4,0xb4,0xff }, /*     4 */
  { -4000,     0,     0,0, 0x0000,0x0800, 0xcd,0xcd,0xcd,0xff }, /*     5 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_9072_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0, TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0),
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,2048), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    0, 0, 5, 5, 0, 0 ),
  gsSPVertex( SEGADRS(5, 0), 6, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSPEndDisplayList(),
};

static Gfx _map2_collision_group_niff_aligner9[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_9101_[] = {
  {  4000,  3164, -2000,0, 0xf800,0x0800, 0xc7,0xc7,0xc7,0xff }, /*     0 */
  {  8000,  1843,     0,0, 0x0800,0x0000, 0x41,0x41,0x41,0xff }, /*     1 */
  {  8000,  3164, -2000,0, 0x0800,0x0800, 0x75,0x75,0x75,0xff }, /*     2 */
  {  4000,  3164, -4000,0, 0xf800,0x0000, 0xf4,0xf4,0xf4,0xff }, /*     3 */
  {  8000,  3164, -2000,0, 0x0800,0x0800, 0x75,0x75,0x75,0xff }, /*     4 */
  {  8000,  3164, -4000,0, 0x0800,0x0000, 0x8a,0x8a,0x8a,0xff }, /*     5 */
  {  4000,  3164, -4000,0, 0xf800,0x0000, 0xf4,0xf4,0xf4,0xff }, /*     6 */
  {  4000,  3164, -2000,0, 0xf800,0x0800, 0xc7,0xc7,0xc7,0xff }, /*     7 */
  {  8000,  3164, -2000,0, 0x0800,0x0800, 0x75,0x75,0x75,0xff }, /*     8 */
  {  4000,  3164, -2000,0, 0xf800,0x0800, 0xc7,0xc7,0xc7,0xff }, /*     9 */
  {  4000,  1843,     0,0, 0xf800,0x0000, 0x6f,0x6f,0x6f,0xff }, /*    10 */
  {  8000,  1843,     0,0, 0x0800,0x0000, 0x41,0x41,0x41,0xff }, /*    11 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_9101_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0, TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0),
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,0), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    2, 2, 5, 5, 0, 0 ),
  gsSPVertex( SEGADRS(5, 0), 12, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP1Triangle(  3,  4,  5,  0),
  gsSP1Triangle(  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsSPEndDisplayList(),
};

static Gfx _map2_collision_group_niff_aligner10[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_9129_[] = {
  {  4000,  3164, -4000,0, 0x0800,0x0800, 0xf4,0xf4,0xf4,0xff }, /*     0 */
  {  2000,  3164, -2000,0, 0xfc01,0x0000, 0xb3,0xb3,0xb3,0xff }, /*     1 */
  {  4000,  3164, -2000,0, 0x0800,0x0000, 0xc5,0xc5,0xc5,0xff }, /*     2 */
  {  4000,  3164, -4000,0, 0x0800,0x0800, 0xf4,0xf4,0xf4,0xff }, /*     3 */
  {  2000,  3164, -4000,0, 0xfc01,0x0800, 0xfe,0xfe,0xfe,0xff }, /*     4 */
  {  2000,  3164, -2000,0, 0xfc01,0x0000, 0xb3,0xb3,0xb3,0xff }, /*     5 */
  {  4000,  3164, -2000,0, 0x0800,0x0000, 0xc5,0xc5,0xc5,0xff }, /*     6 */
  {  2000,  1843,     0,0, 0xfc01,0xf800, 0x6d,0x6d,0x6d,0xff }, /*     7 */
  {  4000,  1843,     0,0, 0x0800,0xf800, 0x67,0x67,0x67,0xff }, /*     8 */
  {  4000,  3164, -2000,0, 0x0800,0x0000, 0xc5,0xc5,0xc5,0xff }, /*     9 */
  {  2000,  3164, -2000,0, 0xfc01,0x0000, 0xb3,0xb3,0xb3,0xff }, /*    10 */
  {  2000,  1843,     0,0, 0xfc01,0xf800, 0x6d,0x6d,0x6d,0xff }, /*    11 */
  {     0,     0, -2000,0, 0x0800,0x0800, 0x9d,0x9d,0x9d,0xff }, /*    12 */
  {  2000,  1843,     0,0, 0x0000,0x0000, 0x6d,0x6d,0x6d,0xff }, /*    13 */
  {  2000,  3164, -2000,0, 0x0000,0x0800, 0xb3,0xb3,0xb3,0xff }, /*    14 */
  {     0,  3164, -4000,0, 0x07ff,0x0000, 0xc0,0xc0,0xc0,0xff }, /*    15 */
  {  2000,  3164, -2000,0, 0x0000,0x07ff, 0xb3,0xb3,0xb3,0xff }, /*    16 */
  {  2000,  3164, -4000,0, 0x0000,0x0000, 0xfe,0xfe,0xfe,0xff }, /*    17 */
  {     0,  3164, -4000,0, 0x07ff,0x0000, 0xc0,0xc0,0xc0,0xff }, /*    18 */
  {     0,     0, -2000,0, 0x0800,0x0800, 0x9d,0x9d,0x9d,0xff }, /*    19 */
  {  2000,  3164, -2000,0, 0x0000,0x07ff, 0xb3,0xb3,0xb3,0xff }, /*    20 */
  {     0,     0, -2000,0, 0x0800,0x0800, 0x9d,0x9d,0x9d,0xff }, /*    21 */
  {     0,     0,     0,0, 0x0800,0x0000, 0xc5,0xc5,0xc5,0xff }, /*    22 */
  {  2000,  1843,     0,0, 0x0000,0x0000, 0x6d,0x6d,0x6d,0xff }, /*    23 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_9129_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0, TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0),
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,0), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    2, 2, 5, 5, 0, 0 ),
  gsSPVertex( SEGADRS(5, 0), 24, 0 ),
  gsSP2Triangles(  0,  1,  2,  0,  3,  4,  5,  0),
  gsSP2Triangles(  6,  7,  8,  0,  9, 10, 11,  0),
  gsDPPipeSync(),
  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,4096), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    2, 2, 5, 5, 0, 0 ),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSPEndDisplayList(),
};

static Gfx _map2_collision_group_niff_aligner11[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_9161_[] = {
  { -2000,     0,     0,0, 0x0000,0x0000, 0xdb,0xdb,0xdb,0xff }, /*     0 */
  { -2000,     0, -2000,0, 0x0000,0x0800, 0xbe,0xbe,0xbe,0xff }, /*     1 */
  { -4000,     0,     0,0, 0x07f8,0x0000, 0xcd,0xcd,0xcd,0xff }, /*     2 */
  { -2017,  2258, -4000,0, 0x0000,0x0000, 0x88,0x88,0x88,0xff }, /*     3 */
  {     0,     0, -2000,0, 0x07ff,0x0800, 0x9d,0x9d,0x9d,0xff }, /*     4 */
  {     0,  3164, -4000,0, 0x07ff,0x0000, 0xc0,0xc0,0xc0,0xff }, /*     5 */
  { -4044,  1113, -4000,0, 0x07ff,0x0000, 0x9d,0x9d,0x9d,0xff }, /*     6 */
  { -2000,     0, -2000,0, 0x0000,0x0800, 0xbe,0xbe,0xbe,0xff }, /*     7 */
  { -2017,  2258, -4000,0, 0x0011,0x0000, 0x88,0x88,0x88,0xff }, /*     8 */
  {     0,     0,     0,0, 0x0800,0x0000, 0xc5,0xc5,0xc5,0xff }, /*     9 */
  {     0,     0, -2000,0, 0x0800,0x0800, 0x9d,0x9d,0x9d,0xff }, /*    10 */
  { -2000,     0,     0,0, 0x0000,0x0000, 0xdb,0xdb,0xdb,0xff }, /*    11 */
  {     0,     0, -2000,0, 0x0800,0x0800, 0x9d,0x9d,0x9d,0xff }, /*    12 */
  { -2000,     0, -2000,0, 0x0000,0x0800, 0xbe,0xbe,0xbe,0xff }, /*    13 */
  { -2000,     0,     0,0, 0x0000,0x0000, 0xdb,0xdb,0xdb,0xff }, /*    14 */
  { -4044,  1113, -4000,0, 0x07ff,0x0000, 0x9d,0x9d,0x9d,0xff }, /*    15 */
  { -4007,     0, -2000,0, 0x07da,0x0800, 0xc8,0xc8,0xc8,0xff }, /*    16 */
  { -2000,     0, -2000,0, 0x0000,0x0800, 0xbe,0xbe,0xbe,0xff }, /*    17 */
  { -2017,  2258, -4000,0, 0x0000,0x0000, 0x88,0x88,0x88,0xff }, /*    18 */
  { -2000,     0, -2000,0, 0x0011,0x0800, 0xbe,0xbe,0xbe,0xff }, /*    19 */
  {     0,     0, -2000,0, 0x07ff,0x0800, 0x9d,0x9d,0x9d,0xff }, /*    20 */
  { -2000,     0, -2000,0, 0x0000,0x0800, 0xbe,0xbe,0xbe,0xff }, /*    21 */
  { -4007,     0, -2000,0, 0x0800,0x0800, 0xc8,0xc8,0xc8,0xff }, /*    22 */
  { -4000,     0,     0,0, 0x07f8,0x0000, 0xcd,0xcd,0xcd,0xff }, /*    23 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_9161_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0),
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,4096), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    2, 2, 5, 5, 0, 0 ),
  gsSPVertex( SEGADRS(5, 0), 24, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP1Triangle(  3,  4,  5,  0),
  gsSP1Triangle(  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSPEndDisplayList(),
};

static Gfx _map2_collision_group_niff_aligner12[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_9182_[] = {
  { -8027,   639, -4000,0, 0x07ff,0x0000, 0x7e,0x7e,0x7e,0xff }, /*     0 */
  { -8002,    56, -2000,0, 0x07e6,0x0800, 0x9a,0x9a,0x9a,0xff }, /*     1 */
  { -6044,  1027, -4000,0, 0x0025,0x0000, 0xab,0xab,0xab,0xff }, /*     2 */
  { -6044,  1027, -4000,0, 0x0000,0x0000, 0xab,0xab,0xab,0xff }, /*     3 */
  { -4007,     0, -2000,0, 0x0be4,0x0800, 0xc8,0xc8,0xc8,0xff }, /*     4 */
  { -4044,  1113, -4000,0, 0x0bad,0x0000, 0x9a,0x9a,0x9a,0xff }, /*     5 */
  { -6000,     0,     0,0, 0x0000,0x0000, 0xba,0xba,0xba,0xff }, /*     6 */
  { -6007,   170, -2000,0, 0x0007,0x0800, 0xb5,0xb5,0xb5,0xff }, /*     7 */
  { -8000,     0,     0,0, 0x07fd,0x0000, 0xaa,0xaa,0xaa,0xff }, /*     8 */
  { -6007,   170, -2000,0, 0x0007,0x0800, 0xb5,0xb5,0xb5,0xff }, /*     9 */
  { -8002,    56, -2000,0, 0x07ff,0x0800, 0x9a,0x9a,0x9a,0xff }, /*    10 */
  { -8000,     0,     0,0, 0x07fd,0x0000, 0xaa,0xaa,0xaa,0xff }, /*    11 */
  { -6044,  1027, -4000,0, 0x0000,0x0000, 0xab,0xab,0xab,0xff }, /*    12 */
  { -6007,   170, -2000,0, 0x0037,0x0800, 0xb5,0xb5,0xb5,0xff }, /*    13 */
  { -4007,     0, -2000,0, 0x0be4,0x0800, 0xc8,0xc8,0xc8,0xff }, /*    14 */
  { -8002,    56, -2000,0, 0x07e6,0x0800, 0x9a,0x9a,0x9a,0xff }, /*    15 */
  { -6007,   170, -2000,0, 0x0000,0x0800, 0xb5,0xb5,0xb5,0xff }, /*    16 */
  { -6044,  1027, -4000,0, 0x0025,0x0000, 0xab,0xab,0xab,0xff }, /*    17 */
  { -4007,     0, -2000,0, 0x07c0,0x0800, 0xc8,0xc8,0xc8,0xff }, /*    18 */
  { -6007,   170, -2000,0, 0x0000,0x0800, 0xb5,0xb5,0xb5,0xff }, /*    19 */
  { -4000,     0,     0,0, 0x07c0,0x0040, 0xcd,0xcd,0xcd,0xff }, /*    20 */
  { -6007,   170, -2000,0, 0x0000,0x0800, 0xb5,0xb5,0xb5,0xff }, /*    21 */
  { -6000,     0,     0,0, 0x0000,0x0800, 0xba,0xba,0xba,0xff }, /*    22 */
  { -4000,     0,     0,0, 0x07c0,0x0040, 0xcd,0xcd,0xcd,0xff }, /*    23 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_9182_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0, TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0),
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,0), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    2, 2, 5, 5, 0, 0 ),
  gsSPVertex( SEGADRS(5, 0), 24, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP1Triangle(  3,  4,  5,  0),
  gsSP1Triangle(  6,  7,  8,  0),
  gsSP2Triangles(  9, 10, 11,  0, 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsDPPipeSync(),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,2048), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    0, 0, 5, 5, 0, 0 ),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSPEndDisplayList(),
};

static Gfx _map2_collision_group_niff_aligner13[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_9209_[] = {
  {  4000,  3164, -8000,0, 0xf800,0x0000, 0xc7,0xc7,0xc7,0xff }, /*     0 */
  {  8000,  3164, -4000,0, 0x0800,0x1000, 0x8a,0x8a,0x8a,0xff }, /*     1 */
  {  8000,  3164, -8000,0, 0x0800,0x0000, 0x6a,0x6a,0x6a,0xff }, /*     2 */
  {  4000,  3164, -8000,0, 0xf800,0x0000, 0xc7,0xc7,0xc7,0xff }, /*     3 */
  {  4000,  3164, -4000,0, 0xf800,0x1000, 0xfd,0xfd,0xfd,0xff }, /*     4 */
  {  8000,  3164, -4000,0, 0x0800,0x1000, 0x8a,0x8a,0x8a,0xff }, /*     5 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_9209_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0, TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0),
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,0), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    2, 2, 5, 5, 0, 0 ),
  gsSPVertex( SEGADRS(5, 0), 6, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP1Triangle(  3,  4,  5,  0),
  gsSPEndDisplayList(),
};

static Gfx _map2_collision_group_niff_aligner14[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_9235_[] = {
  {  4000,  3164, -4000,0, 0x0000,0x0800, 0xf4,0xf4,0xf4,0xff }, /*     0 */
  {  4000,  3164, -8000,0, 0x0000,0x0800, 0xab,0xab,0xab,0xff }, /*     1 */
  {     0,  3164, -4000,0, 0x0000,0x0800, 0xc1,0xc1,0xc1,0xff }, /*     2 */
  {  4000,  3164, -8000,0, 0x0000,0x0800, 0xab,0xab,0xab,0xff }, /*     3 */
  {     0,  3164, -8000,0, 0x0000,0x0800, 0xad,0xad,0xad,0xff }, /*     4 */
  {     0,  3164, -4000,0, 0x0000,0x0800, 0xc1,0xc1,0xc1,0xff }, /*     5 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_9235_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0, TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0),
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,6144), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    2, 2, 5, 5, 0, 0 ),
  gsSPVertex( SEGADRS(5, 0), 6, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP1Triangle(  3,  4,  5,  0),
  gsSPEndDisplayList(),
};

static Gfx _map2_collision_group_niff_aligner15[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_9271_[] = {
  { -2030,  2561, -8000,0, 0x0000,0x0000, 0x89,0x89,0x89,0xff }, /*     0 */
  { -2030,  2561, -6000,0, 0x0000,0x0800, 0xa8,0xa8,0xa8,0xff }, /*     1 */
  {     0,  3164, -8000,0, 0x0800,0x0000, 0xad,0xad,0xad,0xff }, /*     2 */
  { -2030,  2561, -8000,0, 0x0000,0x0000, 0x89,0x89,0x89,0xff }, /*     3 */
  { -4066,  1629, -8000,0, 0x0800,0x0000, 0x8d,0x8d,0x8d,0xff }, /*     4 */
  { -2030,  2561, -6000,0, 0x0000,0x0800, 0xa8,0xa8,0xa8,0xff }, /*     5 */
  { -2030,  2561, -6000,0, 0x0000,0x0800, 0xa8,0xa8,0xa8,0xff }, /*     6 */
  { -2022,  2359, -4666,0, 0x0008,0x0000, 0x9e,0x9e,0x9e,0xff }, /*     7 */
  {     0,  3164, -6000,0, 0x07ff,0x0800, 0xe2,0xe2,0xe2,0xff }, /*     8 */
  { -4066,  1629, -6000,0, 0x0800,0x0800, 0x92,0x92,0x92,0xff }, /*     9 */
  { -4051,  1285, -4666,0, 0x07f1,0x0000, 0x89,0x89,0x89,0xff }, /*    10 */
  { -2030,  2561, -6000,0, 0x0008,0x0800, 0xa8,0xa8,0xa8,0xff }, /*    11 */
  { -4051,  1285, -4666,0, 0x07f1,0x0000, 0x89,0x89,0x89,0xff }, /*    12 */
  { -2022,  2359, -4666,0, 0x0000,0x0000, 0x9e,0x9e,0x9e,0xff }, /*    13 */
  { -2030,  2561, -6000,0, 0x0008,0x0800, 0xa8,0xa8,0xa8,0xff }, /*    14 */
  { -2022,  2359, -4666,0, 0x0008,0x0000, 0x9e,0x9e,0x9e,0xff }, /*    15 */
  {     0,  3164, -4666,0, 0x0800,0x0000, 0xe2,0xe2,0xe2,0xff }, /*    16 */
  {     0,  3164, -6000,0, 0x07ff,0x0800, 0xe2,0xe2,0xe2,0xff }, /*    17 */
  { -4066,  1629, -8000,0, 0x0800,0x0000, 0x8d,0x8d,0x8d,0xff }, /*    18 */
  { -4066,  1629, -6000,0, 0x0800,0x0800, 0x92,0x92,0x92,0xff }, /*    19 */
  { -2030,  2561, -6000,0, 0x0000,0x0800, 0xa8,0xa8,0xa8,0xff }, /*    20 */
  { -2030,  2561, -6000,0, 0x0000,0x0800, 0xa8,0xa8,0xa8,0xff }, /*    21 */
  {     0,  3164, -6000,0, 0x0800,0x0800, 0xe2,0xe2,0xe2,0xff }, /*    22 */
  {     0,  3164, -8000,0, 0x0800,0x0000, 0xad,0xad,0xad,0xff }, /*    23 */
  { -4051,  1285, -4666,0, 0x0000,0x0040, 0x89,0x89,0x89,0xff }, /*    24 */
  { -4044,  1113, -4000,0, 0x07c0,0x0040, 0x9a,0x9a,0x9a,0xff }, /*    25 */
  { -2022,  2359, -4666,0, 0x0000,0x0800, 0x9e,0x9e,0x9e,0xff }, /*    26 */
  { -2022,  2359, -4666,0, 0x0000,0x0040, 0x9e,0x9e,0x9e,0xff }, /*    27 */
  { -2017,  2258, -4000,0, 0x07c0,0x0040, 0x85,0x85,0x85,0xff }, /*    28 */
  {     0,  3164, -4666,0, 0x0000,0x0800, 0xe2,0xe2,0xe2,0xff }, /*    29 */
  { -2017,  2258, -4000,0, 0x07c0,0x0040, 0x85,0x85,0x85,0xff }, /*    30 */
  {     0,  3164, -4000,0, 0x07c0,0x0800, 0xc0,0xc0,0xc0,0xff }, /*    31 */
  {     0,  3164, -4666,0, 0x0000,0x0800, 0xe2,0xe2,0xe2,0xff }, /*    32 */
  { -4044,  1113, -4000,0, 0x07c0,0x0040, 0x9a,0x9a,0x9a,0xff }, /*    33 */
  { -2017,  2258, -4000,0, 0x07c0,0x0800, 0x85,0x85,0x85,0xff }, /*    34 */
  { -2022,  2359, -4666,0, 0x0000,0x0800, 0x9e,0x9e,0x9e,0xff }, /*    35 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_9271_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0, TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0),
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,0), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    2, 2, 5, 5, 0, 0 ),
  gsSPVertex( SEGADRS(5, 0), 32, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP1Triangle(  3,  4,  5,  0),
  gsSP1Triangle(  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsSP1Triangle( 21, 22, 23,  0),
  gsDPPipeSync(),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,2048), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    0, 0, 5, 5, 0, 0 ),
  gsSP1Triangle( 24, 25, 26,  0),
  gsSP1Triangle( 27, 28, 29,  0),
  gsSPVertex( SEGADRS(5, 512), 4, 0 ),
  gsSP1Triangle( 30, 31,  0,  0),
  gsSP1Triangle(  1,  2,  3,  0),
  gsSPEndDisplayList(),
};

static Gfx _map2_collision_group_niff_aligner16[] = { gsSPEndDisplayList() };

/* ============================================================
          Vtx
 ============================================================ */
static Vtx vtx_Shape_of___POLYHEDRON_9300_[] = {
  { -8044,  1027, -8000,0, 0x0800,0x0000, 0x70,0x70,0x70,0xff }, /*     0 */
  { -8044,  1027, -6000,0, 0x0800,0x0800, 0x81,0x81,0x81,0xff }, /*     1 */
  { -6066,  1027, -6000,0, 0x0000,0x0800, 0xa1,0xa1,0xa1,0xff }, /*     2 */
  { -6044,  1027, -4000,0, 0x0000,0x0000, 0xa4,0xa4,0xa4,0xff }, /*     3 */
  { -8044,  1027, -6000,0, 0x0800,0x0800, 0x81,0x81,0x81,0xff }, /*     4 */
  { -8027,   639, -4000,0, 0x07ee,0x0000, 0x8d,0x8d,0x8d,0xff }, /*     5 */
  { -6066,  1027, -6000,0, 0x0000,0x0800, 0xa1,0xa1,0xa1,0xff }, /*     6 */
  { -6044,  1027, -4000,0, 0x0016,0x0000, 0xa4,0xa4,0xa4,0xff }, /*     7 */
  { -4066,  1629, -6000,0, 0x07f0,0x0800, 0x9d,0x9d,0x9d,0xff }, /*     8 */
  { -4066,  1629, -6000,0, 0x0800,0x0800, 0x9d,0x9d,0x9d,0xff }, /*     9 */
  { -6066,  1349, -8000,0, 0x0000,0x0000, 0x93,0x93,0x93,0xff }, /*    10 */
  { -6066,  1027, -6000,0, 0x0000,0x0800, 0xa1,0xa1,0xa1,0xff }, /*    11 */
  { -6066,  1027, -6000,0, 0x0000,0x0800, 0xa1,0xa1,0xa1,0xff }, /*    12 */
  { -6066,  1349, -8000,0, 0x0000,0x0000, 0x93,0x93,0x93,0xff }, /*    13 */
  { -8044,  1027, -8000,0, 0x0800,0x0000, 0x70,0x70,0x70,0xff }, /*    14 */
  { -4066,  1629, -6000,0, 0x0800,0x0800, 0x9d,0x9d,0x9d,0xff }, /*    15 */
  { -4066,  1629, -8000,0, 0x0800,0x0000, 0x8e,0x8e,0x8e,0xff }, /*    16 */
  { -6066,  1349, -8000,0, 0x0000,0x0000, 0x93,0x93,0x93,0xff }, /*    17 */
  { -6044,  1027, -4000,0, 0x0000,0x0000, 0xa4,0xa4,0xa4,0xff }, /*    18 */
  { -6066,  1027, -6000,0, 0x0016,0x0800, 0xa1,0xa1,0xa1,0xff }, /*    19 */
  { -8044,  1027, -6000,0, 0x0800,0x0800, 0x81,0x81,0x81,0xff }, /*    20 */
  { -4044,  1113, -4000,0, 0x06c0,0x0100, 0x9f,0x9f,0x9f,0xff }, /*    21 */
  { -4066,  1629, -6000,0, 0x0000,0x0800, 0x9d,0x9d,0x9d,0xff }, /*    22 */
  { -6044,  1027, -4000,0, 0x0000,0x0800, 0xa4,0xa4,0xa4,0xff }, /*    23 */
};

/* ============================================================
          Gfx
 ============================================================ */
static Gfx gfx_Shape_of___POLYHEDRON_9300_[] = {
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
  gsSPSetGeometryMode( G_SHADE|G_SHADING_SMOOTH ),
  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0, TEXEL0, 0, SHADE, 0, SHADE, 0, TEXEL0, 0),
  gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,0), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    2, 2, 5, 5, 0, 0 ),
  gsSPVertex( SEGADRS(5, 0), 24, 0 ),
  gsSP1Triangle(  0,  1,  2,  0),
  gsSP1Triangle(  3,  4,  5,  0),
  gsSP1Triangle(  6,  7,  8,  0),
  gsSP1Triangle(  9, 10, 11,  0),
  gsSP1Triangle( 12, 13, 14,  0),
  gsSP1Triangle( 15, 16, 17,  0),
  gsSP1Triangle( 18, 19, 20,  0),
  gsDPPipeSync(),
  gsDPSetTextureLUT( G_TT_NONE ),
  gsDPLoadMultiBlock( SEGADRS(6,6144), 0, 0,
    G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
    2, 2, 5, 5, 0, 0 ),
  gsSP1Triangle( 21, 22, 23,  0),
  gsSPEndDisplayList(),
};

