/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 1

		Copyright (C) 1997, NINTENDO Co,Ltd.

============================================================================*/
#include "fiter_1s1.h"
#define G_CC_MULPRIMSHADE PRIMITIVE,0,SHADE,0,PRIMITIVE,0,SHADE,0

Gfx fiter_1_dl[] = {
  gsDPPipeSync(),
  gsDPSetCycleType(G_CYC_2CYCLE),
  gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
  gsSPClearGeometryMode((G_SHADE|G_SHADING_SMOOTH|G_LIGHTING|G_TEXTURE_GEN|G_TEXTURE_GEN_LINEAR|G_CULL_BOTH|G_FOG)),
  gsSPSetGeometryMode( G_ZBUFFER | G_CULL_BACK | G_SHADE | G_SHADING_SMOOTH | G_LIGHTING | G_FOG),
  gsDPSetColorDither(G_CD_BAYER),
  gsDPSetCombineMode(G_CC_MULPRIMSHADE,     G_CC_MULPRIMSHADE), /* N64-SHADE-TYPE-G */
  gsDPPipeSync(),
  gsSPVertex(&fiter_1s1_v[0],16,0),
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(1,5,3,0), /* #<FACE 78> */
  gsSP1Triangle(4,5,6,0), /* #<FACE 77> */
  gsSP1Triangle(0,2,5,0), /* #<FACE 75> */
  gsSP1Triangle(14,15,10,0), /* #<FACE 222> */
  gsSP1Triangle(7,13,14,0), /* #<FACE 219> */
  gsSP1Triangle(12,14,9,0), /* #<FACE 217> */
  gsSP1Triangle(7,14,8,0), /* #<FACE 216> */
  gsSP1Triangle(8,14,11,0), /* #<FACE 109> */
  gsSP1Triangle(10,9,14,0), /* #<FACE 108> */
  gsSP1Triangle(12,11,14,0), /* #<FACE 107> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 105> */
  gsSPVertex(&fiter_1s1_v[16],9,7),
  gsSP1Triangle(15,12,9,0), /* #<FACE 17> */
  gsSP1Triangle(11,10,9,0), /* #<FACE 16> */
  gsSP1Triangle(14,9,13,0), /* #<FACE 14> */
  gsSP1Triangle(5,1,0,0), /* #<FACE 252> */
  gsSP1Triangle(7,5,2,0), /* #<FACE 249> */
  gsSP1Triangle(4,3,5,0), /* #<FACE 247> */
  gsSP1Triangle(7,8,5,0), /* #<FACE 246> */
  gsSP1Triangle(8,6,5,0), /* #<FACE 79> */
  gsSPVertex(&fiter_1s1_v[25],9,0),
  gsSP1Triangle(1,5,3,0), /* #<FACE 138> */
  gsSP1Triangle(4,5,6,0), /* #<FACE 137> */
  gsSP1Triangle(0,2,5,0), /* #<FACE 135> */
  gsSP1Triangle(9,14,15,0), /* #<FACE 303> */
  gsSP1Triangle(7,13,9,0), /* #<FACE 300> */
  gsSP1Triangle(11,9,12,0), /* #<FACE 298> */
  gsSP1Triangle(7,9,8,0), /* #<FACE 297> */
  gsSP1Triangle(8,9,10,0), /* #<FACE 18> */
  gsSPVertex(&fiter_1s1_v[34],8,8),
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(8,11,12,0), /* #<FACE 113> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(14,13,11,0), /* #<FACE 110> */
  gsSP1Triangle(15,11,8,0), /* #<FACE 106> */
  gsSP1Triangle(5,1,0,0), /* #<FACE 192> */
  gsSP1Triangle(9,5,2,0), /* #<FACE 189> */
  gsSP1Triangle(4,3,5,0), /* #<FACE 187> */
  gsSP1Triangle(9,10,5,0), /* #<FACE 186> */
  gsSP1Triangle(10,6,5,0), /* #<FACE 139> */
  gsSPVertex(&fiter_1s1_v[42],11,0),
  gsSP1Triangle(12,5,0,0), /* #<FACE 226> */
  gsSP1Triangle(12,9,5,0), /* #<FACE 225> */
  gsDPSetPrimColor(0,0,255,255,255,255),
  gsSP1Triangle(5,3,1,0), /* #<FACE 205> */
  gsSP1Triangle(4,2,5,0), /* #<FACE 204> */
  gsSP1Triangle(3,5,2,0), /* #<FACE 121> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(4,5,9,0), /* #<FACE 99> */
  gsSP1Triangle(1,0,5,0), /* #<FACE 98> */
  gsSP1Triangle(14,15,6,0), /* #<FACE 221> */
  gsSP1Triangle(8,6,15,0), /* #<FACE 103> */
  gsSP1Triangle(8,15,7,0), /* #<FACE 102> */
  gsSP1Triangle(14,11,15,0), /* #<FACE 218> */
  gsSP1Triangle(10,11,13,0), /* #<FACE 215> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(9,12,11,0), /* #<FACE 212> */
  gsSP1Triangle(11,10,9,0), /* #<FACE 115> */
  gsSPVertex(&fiter_1s1_v[53],13,3),
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(15,14,7,0), /* #<FACE 248> */
  gsSP1Triangle(3,6,7,0), /* #<FACE 245> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(7,8,5,0), /* #<FACE 242> */
  gsSP1Triangle(3,7,4,0), /* #<FACE 240> */
  gsSP1Triangle(5,4,7,0), /* #<FACE 85> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(15,7,6,0), /* #<FACE 80> */
  gsSP1Triangle(14,8,7,0), /* #<FACE 76> */
  gsSP1Triangle(12,0,13,0), /* #<FACE 227> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(11,9,0,0), /* #<FACE 213> */
  gsSP1Triangle(10,0,9,0), /* #<FACE 112> */
  gsSP1Triangle(0,12,11,0), /* #<FACE 111> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(1,13,0,0), /* #<FACE 97> */
  gsSPVertex(&fiter_1s1_v[66],14,0),
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(0,1,2,0), /* #<FACE 51> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(3,8,9,0), /* #<FACE 257> */
  gsSP1Triangle(4,9,7,0), /* #<FACE 256> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(3,9,5,0), /* #<FACE 244> */
  gsSP1Triangle(9,4,6,0), /* #<FACE 243> */
  gsSP1Triangle(6,5,9,0), /* #<FACE 81> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(10,7,9,0), /* #<FACE 68> */
  gsSP1Triangle(10,9,8,0), /* #<FACE 67> */
  gsSP1Triangle(15,11,14,0), /* #<FACE 251> */
  gsSP1Triangle(13,14,11,0), /* #<FACE 73> */
  gsSP1Triangle(13,12,14,0), /* #<FACE 72> */
  gsSPVertex(&fiter_1s1_v[80],13,3),
  gsSP1Triangle(13,6,12,0), /* #<FACE 299> */
  gsSP1Triangle(4,6,5,0), /* #<FACE 296> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(3,14,6,0), /* #<FACE 293> */
  gsSP1Triangle(6,4,3,0), /* #<FACE 24> */
  gsSP1Triangle(15,6,14,0), /* #<FACE 22> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(13,5,6,0), /* #<FACE 19> */
  gsSP1Triangle(12,6,15,0), /* #<FACE 15> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(0,2,11,0), /* #<FACE 273> */
  gsSP1Triangle(10,2,8,0), /* #<FACE 265> */
  gsSP1Triangle(1,7,2,0), /* #<FACE 264> */
  gsSP1Triangle(9,2,7,0), /* #<FACE 61> */
  gsSP1Triangle(2,9,8,0), /* #<FACE 60> */
  gsSP1Triangle(11,2,10,0), /* #<FACE 53> */
  gsSPVertex(&fiter_1s1_v[93],12,0),
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(0,1,2,0), /* #<FACE 136> */
  gsSP1Triangle(4,6,7,0), /* #<FACE 308> */
  gsSP1Triangle(14,5,6,0), /* #<FACE 307> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(3,15,6,0), /* #<FACE 294> */
  gsSP1Triangle(14,6,15,0), /* #<FACE 21> */
  gsSP1Triangle(6,4,3,0), /* #<FACE 20> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(8,6,5,0), /* #<FACE 7> */
  gsSP1Triangle(8,7,6,0), /* #<FACE 6> */
  gsSP1Triangle(13,12,9,0), /* #<FACE 302> */
  gsSP1Triangle(11,9,12,0), /* #<FACE 12> */
  gsSP1Triangle(11,12,10,0), /* #<FACE 11> */
  gsSPVertex(&fiter_1s1_v[105],12,4),
  gsSP1Triangle(13,14,11,0), /* #<FACE 129> */
  gsSP1Triangle(12,11,15,0), /* #<FACE 128> */
  gsSP1Triangle(10,4,0,0), /* #<FACE 191> */
  gsSP1Triangle(6,0,4,0), /* #<FACE 133> */
  gsSP1Triangle(6,5,0,0), /* #<FACE 132> */
  gsSP1Triangle(10,0,2,0), /* #<FACE 188> */
  gsSP1Triangle(7,9,2,0), /* #<FACE 185> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(2,1,14,0), /* #<FACE 182> */
  gsSP1Triangle(7,2,8,0), /* #<FACE 180> */
  gsSP1Triangle(14,8,2,0), /* #<FACE 145> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(10,2,9,0), /* #<FACE 140> */
  gsSPVertex(&fiter_1s1_v[117],11,0),
  gsSP1Triangle(2,3,0,0), /* #<FACE 223> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(3,2,1,0), /* #<FACE 210> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(4,7,15,0), /* #<FACE 197> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(4,15,5,0), /* #<FACE 184> */
  gsSP1Triangle(15,8,6,0), /* #<FACE 183> */
  gsSP1Triangle(6,5,15,0), /* #<FACE 141> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(12,15,7,0), /* #<FACE 127> */
  gsSP1Triangle(8,15,11,0), /* #<FACE 196> */
  gsSP1Triangle(8,11,14,0), /* #<FACE 195> */
  gsDPSetPrimColor(0,0,255,255,255,255),
  gsSP1Triangle(13,11,9,0), /* #<FACE 179> */
  gsSP1Triangle(11,12,10,0), /* #<FACE 178> */
  gsSP1Triangle(10,9,11,0), /* #<FACE 146> */
  gsSPVertex(&fiter_1s1_v[128],16,0),
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(15,0,14,0), /* #<FACE 82> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(1,2,3,0), /* #<FACE 220> */
  gsSP1Triangle(4,3,2,0), /* #<FACE 104> */
  gsSP1Triangle(7,11,12,0), /* #<FACE 224> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(6,5,11,0), /* #<FACE 214> */
  gsSP1Triangle(11,7,6,0), /* #<FACE 211> */
  gsDPSetPrimColor(0,0,255,255,255,255),
  gsSP1Triangle(13,10,12,0), /* #<FACE 207> */
  gsSP1Triangle(8,9,13,0), /* #<FACE 206> */
  gsSP1Triangle(10,13,9,0), /* #<FACE 119> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(13,12,11,0), /* #<FACE 100> */
  gsSPVertex(&fiter_1s1_v[144],13,0),
  gsSP1Triangle(0,4,3,0), /* #<FACE 70> */
  gsDPSetPrimColor(0,0,255,255,255,255),
  gsSP1Triangle(0,2,6,0), /* #<FACE 237> */
  gsSP1Triangle(5,6,2,0), /* #<FACE 88> */
  gsSP1Triangle(5,7,6,0), /* #<FACE 87> */
  gsSP1Triangle(10,6,7,0), /* #<FACE 238> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(14,10,12,0), /* #<FACE 255> */
  gsSP1Triangle(12,11,9,0), /* #<FACE 253> */
  gsDPSetPrimColor(0,0,255,255,255,255),
  gsSP1Triangle(10,7,11,0), /* #<FACE 239> */
  gsSP1Triangle(11,8,1,0), /* #<FACE 234> */
  gsSP1Triangle(8,11,7,0), /* #<FACE 86> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(1,9,11,0), /* #<FACE 71> */
  gsSP1Triangle(11,12,10,0), /* #<FACE 69> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(14,12,15,0), /* #<FACE 83> */
  gsSPVertex(&fiter_1s1_v[157],9,7),
  gsDPSetPrimColor(0,0,255,255,255,255),
  gsSP1Triangle(0,3,2,0), /* #<FACE 236> */
  gsSP1Triangle(1,7,3,0), /* #<FACE 235> */
  gsSP1Triangle(8,3,7,0), /* #<FACE 90> */
  gsSP1Triangle(8,2,3,0), /* #<FACE 89> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(9,10,11,0), /* #<FACE 250> */
  gsSP1Triangle(12,11,10,0), /* #<FACE 74> */
  gsSP1Triangle(15,3,4,0), /* #<FACE 254> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(13,14,4,0), /* #<FACE 241> */
  gsSP1Triangle(15,4,14,0), /* #<FACE 84> */
  gsSPVertex(&fiter_1s1_v[166],14,0),
  gsSP1Triangle(0,5,6,0), /* #<FACE 276> */
  gsSP1Triangle(7,4,6,0), /* #<FACE 274> */
  gsSP1Triangle(3,2,6,0), /* #<FACE 272> */
  gsSP1Triangle(6,2,1,0), /* #<FACE 54> */
  gsSP1Triangle(6,4,3,0), /* #<FACE 52> */
  gsSP1Triangle(7,6,5,0), /* #<FACE 49> */
  gsDPSetPrimColor(0,0,153,255,255,255),
  gsSP1Triangle(10,11,12,0), /* #<FACE 282> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(11,9,8,0), /* #<FACE 271> */
  gsDPSetPrimColor(0,0,153,255,255,255),
  gsSP1Triangle(11,10,9,0), /* #<FACE 43> */
  gsSP1Triangle(13,12,11,0), /* #<FACE 42> */
  gsSPVertex(&fiter_1s1_v[180],15,1),
  gsDPSetPrimColor(0,0,255,255,255,255),
  gsSP1Triangle(3,2,15,0), /* #<FACE 288> */
  gsSP1Triangle(4,1,3,0), /* #<FACE 287> */
  gsSP1Triangle(2,3,1,0), /* #<FACE 28> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(3,15,14,0), /* #<FACE 9> */
  gsDPSetPrimColor(0,0,255,255,255,255),
  gsSP1Triangle(12,5,4,0), /* #<FACE 286> */
  gsSP1Triangle(13,7,6,0), /* #<FACE 290> */
  gsSP1Triangle(12,13,5,0), /* #<FACE 285> */
  gsSP1Triangle(6,5,13,0), /* #<FACE 30> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(7,13,10,0), /* #<FACE 10> */
  gsSP1Triangle(8,11,12,0), /* #<FACE 306> */
  gsSP1Triangle(11,10,13,0), /* #<FACE 304> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(10,11,9,0), /* #<FACE 291> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(13,12,11,0), /* #<FACE 8> */
  gsSPVertex(&fiter_1s1_v[195],13,0),
  gsSP1Triangle(2,1,0,0), /* #<FACE 193> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(3,2,5,0), /* #<FACE 143> */
  gsSP1Triangle(5,4,3,0), /* #<FACE 142> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(6,7,8,0), /* #<FACE 301> */
  gsSP1Triangle(9,8,7,0), /* #<FACE 13> */
  gsSP1Triangle(12,14,15,0), /* #<FACE 305> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(11,10,14,0), /* #<FACE 295> */
  gsSP1Triangle(14,12,11,0), /* #<FACE 292> */
  gsSPVertex(&fiter_1s1_v[208],16,0),
  gsDPSetPrimColor(0,0,255,255,255,255),
  gsSP1Triangle(0,11,10,0), /* #<FACE 150> */
  gsSP1Triangle(0,13,11,0), /* #<FACE 149> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(1,2,15,0), /* #<FACE 190> */
  gsSP1Triangle(14,15,2,0), /* #<FACE 134> */
  gsSP1Triangle(5,11,6,0), /* #<FACE 194> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(3,4,6,0), /* #<FACE 181> */
  gsSP1Triangle(5,6,4,0), /* #<FACE 144> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(12,6,11,0), /* #<FACE 130> */
  gsDPSetPrimColor(0,0,255,255,255,255),
  gsSP1Triangle(12,13,7,0), /* #<FACE 177> */
  gsSP1Triangle(9,7,13,0), /* #<FACE 148> */
  gsSP1Triangle(9,8,7,0), /* #<FACE 147> */
  gsSPVertex(&fiter_1s1_v[224],10,0),
  gsSP1Triangle(3,0,1,0), /* #<FACE 209> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(0,3,2,0), /* #<FACE 101> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(6,5,4,0), /* #<FACE 114> */
  gsDPSetPrimColor(0,0,255,255,153,255),
  gsSP1Triangle(7,8,14,0), /* #<FACE 200> */
  gsSP1Triangle(14,8,15,0), /* #<FACE 123> */
  gsDPSetPrimColor(0,0,255,255,255,255),
  gsSP1Triangle(12,11,13,0), /* #<FACE 176> */
  gsSP1Triangle(9,10,11,0), /* #<FACE 175> */
  gsSPVertex(&fiter_1s1_v[234],14,2),
  gsDPSetPrimColor(0,0,255,255,153,255),
  gsSP1Triangle(15,14,2,0), /* #<FACE 63> */
  gsSP1Triangle(3,6,4,0), /* #<FACE 230> */
  gsSP1Triangle(6,5,4,0), /* #<FACE 93> */
  gsDPSetPrimColor(0,0,255,255,255,255),
  gsSP1Triangle(9,8,7,0), /* #<FACE 118> */
  gsSP1Triangle(8,9,0,0), /* #<FACE 208> */
  gsSP1Triangle(10,0,9,0), /* #<FACE 117> */
  gsSP1Triangle(10,1,0,0), /* #<FACE 116> */
  gsSP1Triangle(13,12,11,0), /* #<FACE 120> */
  gsSPVertex(&fiter_1s1_v[248],14,0),
  gsDPSetPrimColor(0,0,153,255,255,255),
  gsSP1Triangle(4,2,3,0), /* #<FACE 41> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(0,8,9,0), /* #<FACE 270> */
  gsSP1Triangle(5,7,6,0), /* #<FACE 275> */
  gsSP1Triangle(6,1,2,0), /* #<FACE 59> */
  gsSP1Triangle(7,1,6,0), /* #<FACE 50> */
  gsSP1Triangle(10,9,8,0), /* #<FACE 57> */
  gsSP1Triangle(10,11,9,0), /* #<FACE 56> */
  gsSP1Triangle(12,11,10,0), /* #<FACE 55> */
  gsDPSetPrimColor(0,0,255,255,153,255),
  gsSP1Triangle(13,14,15,0), /* #<FACE 260> */
  gsSPVertex(&fiter_1s1_v[262],12,4),
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(6,5,4,0), /* #<FACE 23> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(7,12,13,0), /* #<FACE 281> */
  gsSP1Triangle(11,12,0,0), /* #<FACE 280> */
  gsSP1Triangle(12,8,0,0), /* #<FACE 45> */
  gsSP1Triangle(9,14,11,0), /* #<FACE 279> */
  gsSP1Triangle(9,15,14,0), /* #<FACE 278> */
  gsSP1Triangle(14,15,10,0), /* #<FACE 47> */
  gsSP1Triangle(12,11,14,0), /* #<FACE 46> */
  gsSP1Triangle(14,13,12,0), /* #<FACE 44> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(15,2,1,0), /* #<FACE 267> */
  gsSPVertex(&fiter_1s1_v[274],16,0),
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(0,2,1,0), /* #<FACE 131> */
  gsDPSetPrimColor(0,0,255,255,153,255),
  gsSP1Triangle(3,6,4,0), /* #<FACE 311> */
  gsSP1Triangle(6,5,4,0), /* #<FACE 2> */
  gsDPSetPrimColor(0,0,255,255,255,255),
  gsSP1Triangle(9,8,7,0), /* #<FACE 27> */
  gsSP1Triangle(8,9,10,0), /* #<FACE 289> */
  gsSP1Triangle(12,10,9,0), /* #<FACE 26> */
  gsSP1Triangle(12,11,10,0), /* #<FACE 25> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 29> */
  gsSPVertex(&fiter_1s1_v[290],12,4),
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(4,8,9,0), /* #<FACE 277> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(7,6,5,0), /* #<FACE 269> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(10,9,8,0), /* #<FACE 48> */
  gsDPSetPrimColor(0,0,255,255,255,255),
  gsSP1Triangle(13,12,11,0), /* #<FACE 91> */
  gsSP1Triangle(14,0,15,0), /* #<FACE 151> */
  gsSP1Triangle(1,15,0,0), /* #<FACE 174> */
  gsSPVertex(&fiter_1s1_v[302],16,0),
  gsSP1Triangle(0,2,3,0), /* #<FACE 170> */
  gsSP1Triangle(1,0,3,0), /* #<FACE 173> */
  gsSP1Triangle(1,2,0,0), /* #<FACE 172> */
  gsSP1Triangle(3,2,1,0), /* #<FACE 171> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(8,4,7,0), /* #<FACE 153> */
  gsSP1Triangle(5,4,8,0), /* #<FACE 152> */
  gsSP1Triangle(7,4,6,0), /* #<FACE 157> */
  gsSP1Triangle(5,6,4,0), /* #<FACE 156> */
  gsSP1Triangle(5,8,6,0), /* #<FACE 155> */
  gsSP1Triangle(8,7,6,0), /* #<FACE 154> */
  gsDPSetPrimColor(0,0,0,0,255,255),
  gsSP1Triangle(9,12,10,0), /* #<FACE 268> */
  gsSP1Triangle(12,11,10,0), /* #<FACE 58> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 266> */
  gsSPVertex(&fiter_1s1_v[318],12,4),
  gsDPSetPrimColor(0,0,255,255,255,255),
  gsSP1Triangle(7,4,6,0), /* #<FACE 158> */
  gsSP1Triangle(6,4,5,0), /* #<FACE 161> */
  gsSP1Triangle(4,7,5,0), /* #<FACE 160> */
  gsSP1Triangle(7,6,5,0), /* #<FACE 159> */
  gsSP1Triangle(8,10,11,0), /* #<FACE 162> */
  gsSP1Triangle(9,8,11,0), /* #<FACE 165> */
  gsSP1Triangle(9,10,8,0), /* #<FACE 164> */
  gsSP1Triangle(11,10,9,0), /* #<FACE 163> */
  gsSP1Triangle(15,12,14,0), /* #<FACE 166> */
  gsSP1Triangle(14,12,13,0), /* #<FACE 169> */
  gsSP1Triangle(12,15,13,0), /* #<FACE 168> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 167> */
  gsDPPipeSync(),
  gsDPSetCycleType(G_CYC_1CYCLE),
  gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
  gsSPClearGeometryMode((G_SHADE|G_SHADING_SMOOTH|G_LIGHTING|G_TEXTURE_GEN|G_TEXTURE_GEN_LINEAR|G_CULL_BOTH|G_FOG)),
  gsSPSetGeometryMode( G_ZBUFFER | G_CULL_BACK),
  gsDPSetCombineMode(G_CC_PRIMITIVE,        G_CC_PRIMITIVE), /* N64-SHADE-TYPE-H */
  gsSPVertex(&fiter_1s1_v[330],15,0),
  gsDPSetPrimColor(0,0,153,204,255,255),
  gsSP1Triangle(2,1,0,0), /* #<FACE 92> */
  gsSP1Triangle(3,6,7,0), /* #<FACE 314> */
  gsSP1Triangle(4,3,7,0), /* #<FACE 313> */
  gsSP1Triangle(5,4,7,0), /* #<FACE 312> */
  gsSP1Triangle(8,7,6,0), /* #<FACE 1> */
  gsDPSetPrimColor(0,0,255,255,0,255),
  gsSP1Triangle(11,12,13,0), /* #<FACE 199> */
  gsSP1Triangle(9,12,10,0), /* #<FACE 198> */
  gsSP1Triangle(11,10,12,0), /* #<FACE 126> */
  gsSP1Triangle(14,13,12,0), /* #<FACE 124> */
  gsSPVertex(&fiter_1s1_v[345],14,2),
  gsSP1Triangle(15,2,3,0), /* #<FACE 310> */
  gsSP1Triangle(3,14,15,0), /* #<FACE 4> */
  gsSP1Triangle(4,3,2,0), /* #<FACE 3> */
  gsSP1Triangle(7,8,9,0), /* #<FACE 259> */
  gsSP1Triangle(5,8,6,0), /* #<FACE 258> */
  gsSP1Triangle(7,6,8,0), /* #<FACE 65> */
  gsSP1Triangle(10,9,8,0), /* #<FACE 64> */
  gsDPSetPrimColor(0,0,153,204,255,255),
  gsSP1Triangle(11,0,1,0), /* #<FACE 233> */
  gsSP1Triangle(12,11,1,0), /* #<FACE 232> */
  gsSP1Triangle(13,12,1,0), /* #<FACE 231> */
  gsSPVertex(&fiter_1s1_v[359],14,0),
  gsDPSetPrimColor(0,0,255,255,0,255),
  gsSP1Triangle(2,1,0,0), /* #<FACE 95> */
  gsDPSetPrimColor(0,0,153,204,255,255),
  gsSP1Triangle(3,5,4,0), /* #<FACE 201> */
  gsSP1Triangle(4,6,7,0), /* #<FACE 203> */
  gsSP1Triangle(5,6,4,0), /* #<FACE 202> */
  gsSP1Triangle(8,7,6,0), /* #<FACE 122> */
  gsDPSetPrimColor(0,0,255,255,0,255),
  gsSP1Triangle(11,10,9,0), /* #<FACE 125> */
  gsSP1Triangle(12,15,14,0), /* #<FACE 309> */
  gsSP1Triangle(14,13,12,0), /* #<FACE 5> */
  gsSPVertex(&fiter_1s1_v[373],13,3),
  gsDPSetPrimColor(0,0,153,204,255,255),
  gsSP1Triangle(3,5,4,0), /* #<FACE 261> */
  gsSP1Triangle(4,6,7,0), /* #<FACE 263> */
  gsSP1Triangle(5,6,4,0), /* #<FACE 262> */
  gsSP1Triangle(8,7,6,0), /* #<FACE 62> */
  gsDPSetPrimColor(0,0,255,255,0,255),
  gsSP1Triangle(11,10,9,0), /* #<FACE 66> */
  gsSP1Triangle(15,12,13,0), /* #<FACE 94> */
  gsSP1Triangle(13,14,15,0), /* #<FACE 228> */
  gsSP1Triangle(14,0,1,0), /* #<FACE 229> */
  gsSP1Triangle(1,15,14,0), /* #<FACE 96> */
  gsSPVertex(&fiter_1s1_v[386],16,0),
  gsSP1Triangle(0,2,3,0), /* #<FACE 40> */
  gsSP1Triangle(3,2,1,0), /* #<FACE 39> */
  gsSP1Triangle(4,12,11,0), /* #<FACE 31> */
  gsSP1Triangle(5,6,7,0), /* #<FACE 32> */
  gsSP1Triangle(9,7,8,0), /* #<FACE 284> */
  gsSP1Triangle(7,6,8,0), /* #<FACE 34> */
  gsSP1Triangle(10,9,8,0), /* #<FACE 33> */
  gsSP1Triangle(11,12,14,0), /* #<FACE 283> */
  gsSP1Triangle(12,13,14,0), /* #<FACE 36> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 35> */
  gsSPVertex(&fiter_1s1_v[402],4,12),
  gsSP1Triangle(14,12,13,0), /* #<FACE 38> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 37> */
  gsSPPopMatrix(0),
  gsSPEndDisplayList(),

};

