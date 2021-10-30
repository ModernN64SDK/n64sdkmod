/* far_ in GBI format 
 *
 * File:  far_.c
 * Create Date: 1/22/1997 14:43:51
*/

#include "far_.h"

Gfx far__dl[] = {
  gsDPPipeSync(),
  gsDPSetCycleType(G_CYC_1CYCLE),
  gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
  gsSPClearGeometryMode((G_SHADE|G_SHADING_SMOOTH|G_LIGHTING|G_TEXTURE_GEN|G_TEXTURE_GEN_LINEAR|G_CULL_BOTH|G_FOG)),
  gsSPSetGeometryMode( G_ZBUFFER | G_CULL_BACK | G_SHADE | G_SHADING_SMOOTH | G_LIGHTING ),

  gsDPSetColorDither(G_CD_BAYER),
  gsDPSetCombineMode(G_CC_MULPRIMSHADE,     G_CC_MULPRIMSHADE), /* N64-SHADE-TYPE-G */
  gsSPVertex(&far__v[0],15,0),
  gsDPSetPrimColor(0,0,0,255,255,255),
  gsSP2Triangles(0,3,4,0,2,13,4,0), /* #<FACE 187> and #<FACE 160> */
  gsSP2Triangles(4,14,1,0,14,11,5,0), /* #<FACE 159> and #<FACE 130> */
  gsSP2Triangles(6,14,5,0,12,14,6,0), /* #<FACE 129> and #<FACE 128> */
  gsSP2Triangles(13,9,14,0,7,10,14,0), /* #<FACE 103> and #<FACE 102> */
  gsSP2Triangles(8,7,14,0,9,8,14,0), /* #<FACE 101> and #<FACE 100> */
  gsSP2Triangles(11,14,10,0,12,1,14,0), /* #<FACE 85> and #<FACE 59> */
  gsSP1Triangle(14,4,13,0), /* #<FACE 58> */
  gsSPVertex(&far__v[15],11,5),
  gsSP2Triangles(15,11,10,0,5,10,12,0), /* #<FACE 107> and #<FACE 97> */
  gsSP2Triangles(5,14,10,0,10,13,15,0), /* #<FACE 76> and #<FACE 75> */
  gsSP2Triangles(6,0,4,0,1,6,4,0), /* #<FACE 231> and #<FACE 230> */
  gsSP2Triangles(2,4,8,0,3,7,4,0), /* #<FACE 220> and #<FACE 193> */
  gsSP2Triangles(7,9,4,0,9,8,4,0), /* #<FACE 192> and #<FACE 191> */
  gsSPVertex(&far__v[26],10,0),
  gsSP2Triangles(2,13,1,0,3,13,2,0), /* #<FACE 222> and #<FACE 221> */
  gsSP2Triangles(13,3,4,0,5,13,4,0), /* #<FACE 215> and #<FACE 214> */
  gsSP2Triangles(13,14,0,0,5,15,13,0), /* #<FACE 190> and #<FACE 143> */
  gsSP2Triangles(13,10,14,0,6,12,10,0), /* #<FACE 142> and #<FACE 112> */
  gsSP2Triangles(7,6,10,0,8,7,10,0), /* #<FACE 111> and #<FACE 110> */
  gsSP2Triangles(9,8,10,0,10,11,9,0), /* #<FACE 109> and #<FACE 108> */
  gsSPVertex(&far__v[36],12,4),
  gsSP2Triangles(9,12,8,0,10,6,12,0), /* #<FACE 229> and #<FACE 205> */
  gsSP2Triangles(4,7,12,0,5,4,12,0), /* #<FACE 202> and #<FACE 201> */
  gsSP2Triangles(6,5,12,0,8,12,7,0), /* #<FACE 200> and #<FACE 188> */
  gsSP2Triangles(9,11,12,0,12,13,10,0), /* #<FACE 184> and #<FACE 183> */
  gsSP2Triangles(13,12,11,0,14,15,0,0), /* #<FACE 34> and #<FACE 224> */
  gsSP1Triangle(1,15,14,0), /* #<FACE 223> */
  gsSPVertex(&far__v[48],16,0),
  gsSP2Triangles(4,3,2,0,5,2,1,0), /* #<FACE 119> and #<FACE 94> */
  gsSP2Triangles(5,0,2,0,7,14,15,0), /* #<FACE 77> and #<FACE 263> */
  gsSP2Triangles(6,11,15,0,6,15,10,0), /* #<FACE 253> and #<FACE 252> */
  gsSP2Triangles(8,15,9,0,15,8,7,0), /* #<FACE 241> and #<FACE 240> */
  gsSP2Triangles(15,12,9,0,13,10,15,0), /* #<FACE 235> and #<FACE 26> */
  gsSP2Triangles(12,15,11,0,15,14,13,0), /* #<FACE 25> and #<FACE 15> */
  gsSPVertex(&far__v[64],12,4),
  gsSP2Triangles(4,12,11,0,5,12,4,0), /* #<FACE 116> and #<FACE 115> */
  gsSP2Triangles(6,12,5,0,12,7,13,0), /* #<FACE 96> and #<FACE 95> */
  gsSP2Triangles(6,15,12,0,12,14,7,0), /* #<FACE 73> and #<FACE 72> */
  gsSP2Triangles(8,2,0,0,9,1,2,0), /* #<FACE 141> and #<FACE 122> */
  gsSP2Triangles(10,9,2,0,3,10,2,0), /* #<FACE 121> and #<FACE 120> */
  gsSPVertex(&far__v[76],10,0),
  gsSP2Triangles(8,6,4,0,0,1,6,0), /* #<FACE 124> and #<FACE 123> */
  gsSP2Triangles(1,2,6,0,2,3,6,0), /* #<FACE 93> and #<FACE 92> */
  gsSP2Triangles(3,5,6,0,5,4,6,0), /* #<FACE 91> and #<FACE 90> */
  gsSP2Triangles(8,7,6,0,14,12,15,0), /* #<FACE 35> and #<FACE 145> */
  gsSP2Triangles(9,12,13,0,11,12,9,0), /* #<FACE 118> and #<FACE 117> */
  gsSPVertex(&far__v[86],16,0),
  gsSP2Triangles(0,5,6,0,1,0,6,0), /* #<FACE 219> and #<FACE 218> */
  gsSP2Triangles(2,1,6,0,3,2,6,0), /* #<FACE 217> and #<FACE 216> */
  gsSP2Triangles(6,4,3,0,8,6,5,0), /* #<FACE 197> and #<FACE 194> */
  gsSP2Triangles(8,7,6,0,9,15,13,0), /* #<FACE 146> and #<FACE 228> */
  gsSP2Triangles(10,15,9,0,11,15,10,0), /* #<FACE 227> and #<FACE 226> */
  gsSP2Triangles(12,15,11,0,15,14,13,0), /* #<FACE 225> and #<FACE 189> */
  gsSPVertex(&far__v[102],16,0),
  gsSP2Triangles(2,5,10,0,10,3,2,0), /* #<FACE 286> and #<FACE 285> */
  gsSP2Triangles(3,10,4,0,10,8,4,0), /* #<FACE 284> and #<FACE 283> */
  gsSP2Triangles(7,10,5,0,9,10,6,0), /* #<FACE 281> and #<FACE 268> */
  gsSP2Triangles(7,6,10,0,10,9,8,0), /* #<FACE 9> and #<FACE 8> */
  gsSP2Triangles(13,11,15,0,12,15,14,0), /* #<FACE 106> and #<FACE 105> */
  gsSP2Triangles(13,15,12,0,15,1,14,0), /* #<FACE 104> and #<FACE 99> */
  gsSP1Triangle(15,0,1,0), /* #<FACE 74> */
  gsSPVertex(&far__v[118],13,3),
  gsSP2Triangles(15,0,14,0,3,4,0,0), /* #<FACE 213> and #<FACE 212> */
  gsSP2Triangles(15,3,0,0,5,0,4,0), /* #<FACE 211> and #<FACE 196> */
  gsSP2Triangles(5,1,0,0,7,8,9,0), /* #<FACE 144> and #<FACE 234> */
  gsSP2Triangles(12,9,10,0,7,9,6,0), /* #<FACE 156> and #<FACE 87> */
  gsSP2Triangles(11,10,9,0,9,12,13,0), /* #<FACE 62> and #<FACE 61> */
  gsSP1Triangle(11,9,8,0), /* #<FACE 31> */
  gsSPVertex(&far__v[131],10,0),
  gsSP2Triangles(3,4,14,0,14,5,15,0), /* #<FACE 170> and #<FACE 169> */
  gsSP2Triangles(5,14,4,0,7,12,0,0), /* #<FACE 48> and #<FACE 232> */
  gsSP2Triangles(6,13,12,0,6,12,8,0), /* #<FACE 157> and #<FACE 134> */
  gsSP2Triangles(8,12,7,0,2,10,1,0), /* #<FACE 84> and #<FACE 233> */
  gsSP2Triangles(10,2,0,0,12,10,0,0), /* #<FACE 204> and #<FACE 186> */
  gsSP2Triangles(10,11,9,0,9,1,10,0), /* #<FACE 155> and #<FACE 32> */
  gsSPVertex(&far__v[141],12,4),
  gsSP2Triangles(15,5,14,0,5,12,13,0), /* #<FACE 251> and #<FACE 242> */
  gsSP2Triangles(13,4,5,0,6,5,4,0), /* #<FACE 239> and #<FACE 238> */
  gsSP2Triangles(6,14,5,0,7,0,2,0), /* #<FACE 27> and #<FACE 203> */
  gsSP2Triangles(1,8,2,0,9,2,8,0), /* #<FACE 185> and #<FACE 33> */
  gsSP1Triangle(3,11,10,0), /* #<FACE 195> */
  gsSPVertex(&far__v[153],10,0),
  gsSP2Triangles(0,8,9,0,0,9,2,0), /* #<FACE 257> and #<FACE 245> */
  gsSP2Triangles(6,3,9,0,3,6,1,0), /* #<FACE 262> and #<FACE 250> */
  gsSP2Triangles(1,4,3,0,2,15,5,0), /* #<FACE 247> and #<FACE 249> */
  gsSP2Triangles(15,2,9,0,15,9,3,0), /* #<FACE 248> and #<FACE 16> */
  gsSP2Triangles(14,3,4,0,15,14,5,0), /* #<FACE 246> and #<FACE 244> */
  gsSP2Triangles(5,14,4,0,9,12,6,0), /* #<FACE 243> and #<FACE 256> */
  gsSP2Triangles(8,13,12,0,7,6,12,0), /* #<FACE 29> and #<FACE 22> */
  gsSP1Triangle(12,9,8,0), /* #<FACE 21> */
  gsSPVertex(&far__v[163],16,0),
  gsSP2Triangles(14,0,1,0,0,14,13,0), /* #<FACE 265> and #<FACE 11> */
  gsSP2Triangles(1,15,14,0,3,4,2,0), /* #<FACE 3> and #<FACE 114> */
  gsSP2Triangles(5,4,3,0,6,10,11,0), /* #<FACE 113> and #<FACE 140> */
  gsSP2Triangles(7,9,10,0,8,7,10,0), /* #<FACE 127> and #<FACE 126> */
  gsSP2Triangles(12,10,9,0,12,11,10,0), /* #<FACE 88> and #<FACE 78> */
  gsSPVertex(&far__v[179],13,0),
  gsSP2Triangles(1,6,4,0,6,2,0,0), /* #<FACE 152> and #<FACE 133> */
  gsSP2Triangles(3,2,6,0,5,4,6,0), /* #<FACE 132> and #<FACE 66> */
  gsSP2Triangles(6,1,3,0,15,7,10,0), /* #<FACE 65> and #<FACE 278> */
  gsSP2Triangles(9,10,7,0,9,8,10,0), /* #<FACE 275> and #<FACE 269> */
  gsSP2Triangles(10,14,15,0,10,11,14,0), /* #<FACE 280> and #<FACE 277> */
  gsSP2Triangles(12,14,11,0,12,13,14,0), /* #<FACE 276> and #<FACE 266> */
  gsSPVertex(&far__v[192],10,6),
  gsSP2Triangles(6,0,2,0,2,7,6,0), /* #<FACE 86> and #<FACE 64> */
  gsSP2Triangles(9,8,10,0,12,11,10,0), /* #<FACE 98> and #<FACE 49> */
  gsSP2Triangles(15,3,1,0,13,2,3,0), /* #<FACE 153> and #<FACE 131> */
  gsSP1Triangle(3,15,14,0), /* #<FACE 83> */
  gsSPVertex(&far__v[202],14,0),
  gsSP2Triangles(3,5,6,0,6,7,4,0), /* #<FACE 151> and #<FACE 150> */
  gsSP2Triangles(7,6,5,0,1,2,8,0), /* #<FACE 67> and #<FACE 210> */
  gsSP2Triangles(0,1,8,0,9,13,11,0), /* #<FACE 209> and #<FACE 208> */
  gsSP2Triangles(10,13,9,0,13,12,11,0), /* #<FACE 207> and #<FACE 198> */
  gsSPVertex(&far__v[216],13,3),
  gsSP2Triangles(0,3,1,0,3,0,15,0), /* #<FACE 166> and #<FACE 51> */
  gsSP2Triangles(1,5,2,0,2,6,4,0), /* #<FACE 165> and #<FACE 164> */
  gsSP2Triangles(6,2,5,0,7,8,9,0), /* #<FACE 53> and #<FACE 161> */
  gsSP2Triangles(10,9,8,0,11,12,13,0), /* #<FACE 57> and #<FACE 158> */
  gsSP1Triangle(14,13,12,0), /* #<FACE 60> */
  gsSPVertex(&far__v[229],15,0),
  gsSP2Triangles(0,2,3,0,3,4,1,0), /* #<FACE 176> and #<FACE 175> */
  gsSP2Triangles(4,3,2,0,5,6,8,0), /* #<FACE 42> and #<FACE 173> */
  gsSP2Triangles(7,9,8,0,7,8,6,0), /* #<FACE 46> and #<FACE 45> */
  gsSP2Triangles(10,8,9,0,9,12,10,0), /* #<FACE 172> and #<FACE 171> */
  gsSP2Triangles(12,11,10,0,14,15,13,0), /* #<FACE 47> and #<FACE 167> */
  gsSPVertex(&far__v[244],14,2),
  gsSP2Triangles(3,2,4,0,6,3,4,0), /* #<FACE 199> and #<FACE 206> */
  gsSP2Triangles(4,5,6,0,6,7,8,0), /* #<FACE 182> and #<FACE 181> */
  gsSP2Triangles(7,6,5,0,8,9,10,0), /* #<FACE 36> and #<FACE 180> */
  gsSP2Triangles(13,10,9,0,10,13,14,0), /* #<FACE 38> and #<FACE 179> */
  gsSP2Triangles(11,14,12,0,15,12,14,0), /* #<FACE 178> and #<FACE 40> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 39> */
  gsSPVertex(&far__v[258],15,0),
  gsSP2Triangles(13,14,0,0,13,0,10,0), /* #<FACE 254> and #<FACE 28> */
  gsSP2Triangles(0,11,1,0,11,0,14,0), /* #<FACE 261> and #<FACE 24> */
  gsSP2Triangles(1,3,4,0,3,1,11,0), /* #<FACE 260> and #<FACE 17> */
  gsSP2Triangles(4,7,8,0,3,2,7,0), /* #<FACE 259> and #<FACE 237> */
  gsSP2Triangles(7,4,3,0,8,9,5,0), /* #<FACE 18> and #<FACE 258> */
  gsSP2Triangles(6,5,9,0,9,8,7,0), /* #<FACE 20> and #<FACE 19> */
  gsSP2Triangles(10,12,13,0,14,12,11,0), /* #<FACE 255> and #<FACE 236> */
  gsSP1Triangle(14,13,12,0), /* #<FACE 23> */
  gsSPVertex(&far__v[273],15,1),
  gsSP2Triangles(14,1,13,0,14,15,1,0), /* #<FACE 282> and #<FACE 12> */
  gsSP2Triangles(4,3,2,0,5,10,11,0), /* #<FACE 71> and #<FACE 139> */
  gsSP2Triangles(6,7,10,0,11,12,8,0), /* #<FACE 125> and #<FACE 138> */
  gsSP2Triangles(12,10,7,0,9,8,12,0), /* #<FACE 89> and #<FACE 80> */
  gsSP1Triangle(12,11,10,0), /* #<FACE 79> */
  gsSPVertex(&far__v[288],12,0),
  gsSP2Triangles(2,1,0,0,6,5,8,0), /* #<FACE 68> and #<FACE 279> */
  gsSP2Triangles(3,8,5,0,3,11,8,0), /* #<FACE 274> and #<FACE 270> */
  gsSP2Triangles(15,14,4,0,4,13,6,0), /* #<FACE 264> and #<FACE 273> */
  gsSP2Triangles(13,4,14,0,6,7,5,0), /* #<FACE 13> and #<FACE 272> */
  gsSP2Triangles(10,5,7,0,7,6,13,0), /* #<FACE 5> and #<FACE 4> */
  gsSP2Triangles(7,9,10,0,9,8,11,0), /* #<FACE 287> and #<FACE 7> */
  gsSP1Triangle(11,10,9,0), /* #<FACE 2> */
  gsSPVertex(&far__v[300],16,0),
  gsSP2Triangles(0,3,1,0,3,2,1,0), /* #<FACE 177> and #<FACE 41> */
  gsSP2Triangles(6,5,4,0,9,8,7,0), /* #<FACE 43> and #<FACE 52> */
  gsSP2Triangles(14,15,10,0,13,12,11,0), /* #<FACE 54> and #<FACE 63> */
  gsSPVertex(&far__v[316],13,0),
  gsSP2Triangles(3,2,14,0,14,2,15,0), /* #<FACE 163> and #<FACE 55> */
  gsSP2Triangles(2,3,4,0,5,4,3,0), /* #<FACE 162> and #<FACE 56> */
  gsSP2Triangles(8,7,6,0,0,1,9,0), /* #<FACE 147> and #<FACE 135> */
  gsSP1Triangle(12,11,10,0), /* #<FACE 37> */
  gsSPVertex(&far__v[329],12,4),
  gsSP2Triangles(4,5,6,0,6,7,0,0), /* #<FACE 137> and #<FACE 136> */
  gsSP2Triangles(7,6,5,0,1,0,7,0), /* #<FACE 81> and #<FACE 82> */
  gsSP2Triangles(8,9,10,0,11,10,9,0), /* #<FACE 174> and #<FACE 44> */
  gsSP2Triangles(12,13,14,0,15,14,13,0), /* #<FACE 168> and #<FACE 50> */
  gsSPVertex(&far__v[341],14,0),
  gsSP2Triangles(0,9,10,0,1,6,7,0), /* #<FACE 149> and #<FACE 267> */
  gsSP2Triangles(2,3,4,0,5,4,3,0), /* #<FACE 271> and #<FACE 6> */
  gsSP2Triangles(8,7,6,0,10,11,12,0), /* #<FACE 10> and #<FACE 148> */
  gsSP2Triangles(11,10,9,0,13,12,11,0), /* #<FACE 69> and #<FACE 70> */
  gsSPVertex(&far__v[355],3,13),
  gsSP1Triangle(15,14,13,0), /* #<FACE 154> */
  gsSPEndDisplayList(),
};
