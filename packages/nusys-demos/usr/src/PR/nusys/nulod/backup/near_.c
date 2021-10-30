/* near_ in GBI format 
 *
 * File:  near_.c
 * Create Date: 1/22/1997 11:28:34
*/

#include "near_.h"

Gfx near__dl[] = {
  gsDPPipeSync(),
  gsDPSetCycleType(G_CYC_1CYCLE),
  gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
  gsSPClearGeometryMode((G_SHADE|G_SHADING_SMOOTH|G_LIGHTING|G_TEXTURE_GEN|G_TEXTURE_GEN_LINEAR|G_CULL_BOTH|G_FOG)),
  gsSPSetGeometryMode( G_ZBUFFER | G_CULL_BACK | G_SHADE | G_SHADING_SMOOTH | G_LIGHTING ),
  gsDPSetColorDither(G_CD_BAYER),
  gsDPSetCombineMode(G_CC_MULPRIMSHADE,     G_CC_MULPRIMSHADE), /* N64-SHADE-TYPE-G */
  gsSPVertex(&near__v[0],16,0),
  gsDPSetPrimColor(0,0,255,255,0,255),
  gsSP2Triangles(3,5,2,0,5,3,4,0), /* #<FACE 157> and #<FACE 90> */
  gsSP2Triangles(0,15,13,0,15,12,6,0), /* #<FACE 174> and #<FACE 145> */
  gsSP2Triangles(7,15,6,0,14,15,7,0), /* #<FACE 144> and #<FACE 143> */
  gsSP2Triangles(1,10,15,0,8,11,15,0), /* #<FACE 118> and #<FACE 117> */
  gsSP2Triangles(9,8,15,0,10,9,15,0), /* #<FACE 116> and #<FACE 115> */
  gsSP2Triangles(12,15,11,0,14,13,15,0), /* #<FACE 100> and #<FACE 74> */
  gsSP1Triangle(15,0,1,0), /* #<FACE 73> */
  gsSPVertex(&near__v[16],10,6),
  gsSP2Triangles(15,13,0,0,14,1,0,0), /* #<FACE 201> and #<FACE 175> */
  gsSP2Triangles(6,3,11,0,7,3,6,0), /* #<FACE 240> and #<FACE 239> */
  gsSP2Triangles(8,3,7,0,9,3,8,0), /* #<FACE 238> and #<FACE 237> */
  gsSP2Triangles(3,9,10,0,12,3,10,0), /* #<FACE 231> and #<FACE 230> */
  gsSP2Triangles(3,2,11,0,12,4,3,0), /* #<FACE 204> and #<FACE 158> */
  gsSPVertex(&near__v[26],13,0),
  gsSP2Triangles(3,5,4,0,6,5,3,0), /* #<FACE 123> and #<FACE 122> */
  gsSP2Triangles(0,3,2,0,0,1,3,0), /* #<FACE 112> and #<FACE 91> */
  gsSP2Triangles(7,15,10,0,8,7,10,0), /* #<FACE 247> and #<FACE 246> */
  gsSP2Triangles(14,10,11,0,13,9,10,0), /* #<FACE 236> and #<FACE 207> */
  gsSP2Triangles(9,12,10,0,12,11,10,0), /* #<FACE 206> and #<FACE 205> */
  gsSPVertex(&near__v[39],9,7),
  gsSP2Triangles(7,1,11,0,8,1,7,0), /* #<FACE 244> and #<FACE 243> */
  gsSP2Triangles(9,1,8,0,10,1,9,0), /* #<FACE 242> and #<FACE 241> */
  gsSP2Triangles(1,12,11,0,1,0,12,0), /* #<FACE 203> and #<FACE 156> */
  gsSP2Triangles(13,2,3,0,14,13,3,0), /* #<FACE 127> and #<FACE 126> */
  gsSP2Triangles(15,14,3,0,4,15,3,0), /* #<FACE 125> and #<FACE 124> */
  gsSPVertex(&near__v[48],15,0),
  gsSP2Triangles(0,3,2,0,2,4,1,0), /* #<FACE 199> and #<FACE 198> */
  gsSP2Triangles(4,2,3,0,5,9,13,0), /* #<FACE 49> and #<FACE 235> */
  gsSP2Triangles(6,5,13,0,7,6,13,0), /* #<FACE 234> and #<FACE 233> */
  gsSP2Triangles(8,7,13,0,13,11,8,0), /* #<FACE 232> and #<FACE 211> */
  gsSP2Triangles(10,13,9,0,10,12,13,0), /* #<FACE 208> and #<FACE 161> */
  gsSP2Triangles(13,14,11,0,14,13,12,0), /* #<FACE 160> and #<FACE 87> */
  gsSPVertex(&near__v[63],13,3),
  gsSP2Triangles(3,7,8,0,4,3,8,0), /* #<FACE 137> and #<FACE 136> */
  gsSP2Triangles(5,4,8,0,6,5,8,0), /* #<FACE 135> and #<FACE 134> */
  gsSP2Triangles(10,8,7,0,10,9,8,0), /* #<FACE 109> and #<FACE 92> */
  gsSP2Triangles(0,2,15,0,1,13,2,0), /* #<FACE 245> and #<FACE 221> */
  gsSP2Triangles(11,14,2,0,12,11,2,0), /* #<FACE 216> and #<FACE 215> */
  gsSP2Triangles(13,12,2,0,15,2,14,0), /* #<FACE 214> and #<FACE 202> */
  gsSPVertex(&near__v[76],16,0),
  gsSP2Triangles(5,6,4,0,6,7,4,0), /* #<FACE 107> and #<FACE 106> */
  gsSP2Triangles(7,2,4,0,3,8,4,0), /* #<FACE 105> and #<FACE 50> */
  gsSP2Triangles(9,15,13,0,10,15,9,0), /* #<FACE 133> and #<FACE 132> */
  gsSP2Triangles(11,15,10,0,12,15,11,0), /* #<FACE 131> and #<FACE 130> */
  gsSP2Triangles(1,15,12,0,15,14,13,0), /* #<FACE 111> and #<FACE 110> */
  gsSP1Triangle(1,0,15,0), /* #<FACE 88> */
  gsSPVertex(&near__v[92],10,6),
  gsSP2Triangles(14,13,15,0,8,10,6,0), /* #<FACE 80> and #<FACE 229> */
  gsSP2Triangles(7,9,10,0,8,7,10,0), /* #<FACE 228> and #<FACE 227> */
  gsSP2Triangles(0,10,9,0,0,1,10,0), /* #<FACE 210> and #<FACE 159> */
  gsSP2Triangles(11,10,1,0,3,4,2,0), /* #<FACE 89> and #<FACE 139> */
  gsSP2Triangles(12,15,4,0,15,5,4,0), /* #<FACE 138> and #<FACE 108> */
  gsSPVertex(&near__v[102],13,0),
  gsSP2Triangles(5,0,4,0,6,2,5,0), /* #<FACE 250> and #<FACE 41> */
  gsSP2Triangles(5,3,0,0,5,1,6,0), /* #<FACE 40> and #<FACE 30> */
  gsSP2Triangles(13,8,7,0,7,10,13,0), /* #<FACE 248> and #<FACE 220> */
  gsSP2Triangles(12,9,13,0,9,8,13,0), /* #<FACE 219> and #<FACE 218> */
  gsSP2Triangles(11,13,10,0,11,15,13,0), /* #<FACE 217> and #<FACE 168> */
  gsSP1Triangle(13,14,12,0), /* #<FACE 167> */
  gsSPVertex(&near__v[115],10,6),
  gsSP2Triangles(14,13,15,0,6,3,10,0), /* #<FACE 46> and #<FACE 277> */
  gsSP2Triangles(9,0,3,0,3,6,7,0), /* #<FACE 268> and #<FACE 265> */
  gsSP2Triangles(7,8,3,0,9,3,8,0), /* #<FACE 262> and #<FACE 261> */
  gsSP2Triangles(2,10,3,0,11,1,5,0), /* #<FACE 31> and #<FACE 278> */
  gsSP2Triangles(3,5,2,0,12,5,4,0), /* #<FACE 267> and #<FACE 256> */
  gsSP1Triangle(5,12,11,0), /* #<FACE 255> */
  gsSPVertex(&near__v[125],11,0),
  gsSP2Triangles(2,0,5,0,1,5,3,0), /* #<FACE 121> and #<FACE 120> */
  gsSP2Triangles(2,5,1,0,5,4,3,0), /* #<FACE 119> and #<FACE 114> */
  gsSP2Triangles(7,15,13,0,10,13,8,0), /* #<FACE 249> and #<FACE 171> */
  gsSP2Triangles(7,13,6,0,14,8,13,0), /* #<FACE 102> and #<FACE 77> */
  gsSP1Triangle(13,10,9,0), /* #<FACE 76> */
  gsSPVertex(&near__v[136],16,0),
  gsSP2Triangles(6,13,10,0,12,8,10,0), /* #<FACE 319> and #<FACE 311> */
  gsSP2Triangles(2,0,3,0,1,3,0,0), /* #<FACE 329> and #<FACE 326> */
  gsSP2Triangles(11,3,14,0,1,14,3,0), /* #<FACE 315> and #<FACE 7> */
  gsSP2Triangles(3,11,15,0,2,3,8,0), /* #<FACE 6> and #<FACE 2> */
  gsSP2Triangles(9,8,3,0,15,9,3,0), /* #<FACE 332> and #<FACE 331> */
  gsSP2Triangles(4,9,15,0,4,5,9,0), /* #<FACE 330> and #<FACE 328> */
  gsSP2Triangles(7,9,5,0,10,9,6,0), /* #<FACE 327> and #<FACE 310> */
  gsSP2Triangles(7,6,9,0,9,10,8,0), /* #<FACE 12> and #<FACE 11> */
  gsSPVertex(&near__v[152],10,0),
  gsSP2Triangles(0,5,2,0,1,5,0,0), /* #<FACE 224> and #<FACE 223> */
  gsSP2Triangles(5,3,2,0,5,4,3,0), /* #<FACE 212> and #<FACE 155> */
  gsSP2Triangles(6,11,7,0,6,12,11,0), /* #<FACE 325> and #<FACE 320> */
  gsSP2Triangles(8,15,11,0,14,7,11,0), /* #<FACE 316> and #<FACE 3> */
  gsSP2Triangles(9,10,13,0,10,11,12,0), /* #<FACE 324> and #<FACE 323> */
  gsSP2Triangles(8,11,10,0,9,8,10,0), /* #<FACE 322> and #<FACE 321> */
  gsSPVertex(&near__v[162],16,0),
  gsSP2Triangles(15,1,0,0,1,15,14,0), /* #<FACE 184> and #<FACE 63> */
  gsSP2Triangles(4,2,6,0,5,7,8,0), /* #<FACE 172> and #<FACE 166> */
  gsSP2Triangles(8,9,3,0,4,6,8,0), /* #<FACE 165> and #<FACE 149> */
  gsSP2Triangles(8,6,5,0,9,8,7,0), /* #<FACE 99> and #<FACE 82> */
  gsSP2Triangles(12,10,11,0,13,12,11,0), /* #<FACE 226> and #<FACE 225> */
  gsSPVertex(&near__v[178],14,0),
  gsSP2Triangles(3,10,1,0,2,6,4,0), /* #<FACE 259> and #<FACE 271> */
  gsSP2Triangles(0,7,6,0,5,4,6,0), /* #<FACE 44> and #<FACE 37> */
  gsSP2Triangles(6,2,0,0,3,9,10,0), /* #<FACE 36> and #<FACE 266> */
  gsSP2Triangles(9,6,7,0,7,8,9,0), /* #<FACE 257> and #<FACE 254> */
  gsSP2Triangles(11,9,8,0,11,10,9,0), /* #<FACE 253> and #<FACE 42> */
  gsSP2Triangles(13,15,12,0,13,14,15,0), /* #<FACE 209> and #<FACE 185> */
  gsSPVertex(&near__v[192],12,4),
  gsSP2Triangles(9,10,12,0,13,14,6,0), /* #<FACE 21> and #<FACE 306> */
  gsSP2Triangles(6,11,9,0,6,9,13,0), /* #<FACE 305> and #<FACE 304> */
  gsSP2Triangles(5,6,14,0,15,6,4,0), /* #<FACE 301> and #<FACE 282> */
  gsSP2Triangles(5,4,6,0,6,15,11,0), /* #<FACE 25> and #<FACE 24> */
  gsSP2Triangles(7,0,2,0,7,2,8,0), /* #<FACE 272> and #<FACE 260> */
  gsSP2Triangles(8,3,1,0,3,8,2,0), /* #<FACE 264> and #<FACE 263> */
  gsSPVertex(&near__v[204],9,0),
  gsSP2Triangles(3,11,15,0,6,0,13,0), /* #<FACE 283> and #<FACE 289> */
  gsSP2Triangles(13,0,4,0,13,4,14,0), /* #<FACE 18> and #<FACE 17> */
  gsSP2Triangles(13,12,6,0,7,12,5,0), /* #<FACE 308> and #<FACE 287> */
  gsSP2Triangles(1,12,10,0,12,1,5,0), /* #<FACE 286> and #<FACE 20> */
  gsSP2Triangles(7,6,12,0,9,12,13,0), /* #<FACE 15> and #<FACE 303> */
  gsSP2Triangles(8,9,11,0,10,9,2,0), /* #<FACE 302> and #<FACE 285> */
  gsSP1Triangle(8,2,9,0), /* #<FACE 22> */
  gsSPVertex(&near__v[213],11,5),
  gsSP2Triangles(5,7,8,0,6,5,8,0), /* #<FACE 142> and #<FACE 141> */
  gsSP2Triangles(10,8,7,0,10,9,8,0), /* #<FACE 103> and #<FACE 93> */
  gsSP2Triangles(4,0,1,0,13,11,4,0), /* #<FACE 298> and #<FACE 290> */
  gsSP2Triangles(1,12,4,0,2,3,12,0), /* #<FACE 294> and #<FACE 16> */
  gsSP2Triangles(15,12,3,0,15,4,12,0), /* #<FACE 299> and #<FACE 296> */
  gsSP2Triangles(13,4,15,0,15,14,13,0), /* #<FACE 295> and #<FACE 293> */
  gsSPVertex(&near__v[224],14,0),
  gsSP2Triangles(6,5,7,0,9,8,7,0), /* #<FACE 113> and #<FACE 64> */
  gsSP2Triangles(12,3,2,0,11,10,12,0), /* #<FACE 148> and #<FACE 81> */
  gsSP2Triangles(13,3,12,0,4,3,13,0), /* #<FACE 147> and #<FACE 146> */
  gsSP1Triangle(13,0,1,0), /* #<FACE 98> */
  gsSPVertex(&near__v[238],11,5),
  gsSP2Triangles(12,11,13,0,5,1,0,0), /* #<FACE 94> and #<FACE 150> */
  gsSP2Triangles(14,3,6,0,15,2,3,0), /* #<FACE 200> and #<FACE 101> */
  gsSP2Triangles(3,14,15,0,4,6,3,0), /* #<FACE 79> and #<FACE 48> */
  gsSP2Triangles(8,9,7,0,10,9,8,0), /* #<FACE 129> and #<FACE 128> */
  gsSPVertex(&near__v[249],11,0),
  gsSP2Triangles(1,0,2,0,2,15,14,0), /* #<FACE 170> and #<FACE 169> */
  gsSP2Triangles(2,14,1,0,3,4,5,0), /* #<FACE 47> and #<FACE 162> */
  gsSP2Triangles(6,5,4,0,7,9,8,0), /* #<FACE 86> and #<FACE 222> */
  gsSP2Triangles(9,11,8,0,9,13,11,0), /* #<FACE 213> and #<FACE 154> */
  gsSP1Triangle(11,12,10,0), /* #<FACE 153> */
  gsSPVertex(&near__v[260],16,0),
  gsSP2Triangles(15,0,1,0,0,15,14,0), /* #<FACE 181> and #<FACE 66> */
  gsSP2Triangles(1,3,4,0,4,5,2,0), /* #<FACE 180> and #<FACE 179> */
  gsSP2Triangles(5,4,3,0,6,7,8,0), /* #<FACE 68> and #<FACE 176> */
  gsSP2Triangles(9,8,7,0,10,11,12,0), /* #<FACE 72> and #<FACE 173> */
  gsSP1Triangle(13,12,11,0), /* #<FACE 75> */
  gsSPVertex(&near__v[276],14,0),
  gsSP2Triangles(1,2,0,0,3,5,6,0), /* #<FACE 53> and #<FACE 191> */
  gsSP2Triangles(6,7,4,0,7,6,5,0), /* #<FACE 190> and #<FACE 57> */
  gsSP2Triangles(8,10,11,0,11,12,9,0), /* #<FACE 187> and #<FACE 186> */
  gsSP2Triangles(12,11,10,0,13,14,15,0), /* #<FACE 61> and #<FACE 182> */
  gsSPVertex(&near__v[290],13,3),
  gsSP2Triangles(15,10,14,0,15,11,10,0), /* #<FACE 251> and #<FACE 38> */
  gsSP2Triangles(13,3,12,0,4,5,6,0), /* #<FACE 252> and #<FACE 197> */
  gsSP2Triangles(6,7,8,0,7,6,5,0), /* #<FACE 196> and #<FACE 51> */
  gsSP2Triangles(8,0,2,0,2,1,9,0), /* #<FACE 195> and #<FACE 194> */
  gsSPVertex(&near__v[303],9,0),
  gsSP2Triangles(11,15,1,0,11,1,8,0), /* #<FACE 269> and #<FACE 43> */
  gsSP2Triangles(3,0,2,0,1,14,2,0), /* #<FACE 258> and #<FACE 276> */
  gsSP2Triangles(14,1,15,0,2,13,3,0), /* #<FACE 39> and #<FACE 275> */
  gsSP2Triangles(13,2,14,0,3,12,6,0), /* #<FACE 32> and #<FACE 274> */
  gsSP2Triangles(12,3,13,0,6,7,4,0), /* #<FACE 33> and #<FACE 273> */
  gsSP2Triangles(5,4,7,0,7,6,12,0), /* #<FACE 35> and #<FACE 34> */
  gsSP1Triangle(8,10,11,0), /* #<FACE 270> */
  gsSPVertex(&near__v[312],14,2),
  gsSP2Triangles(2,3,4,0,5,4,3,0), /* #<FACE 140> and #<FACE 104> */
  gsSP2Triangles(7,6,5,0,14,8,12,0), /* #<FACE 95> and #<FACE 292> */
  gsSP2Triangles(9,12,8,0,8,11,9,0), /* #<FACE 297> and #<FACE 279> */
  gsSP2Triangles(10,9,11,0,10,11,13,0), /* #<FACE 28> and #<FACE 307> */
  gsSP2Triangles(15,13,11,0,12,13,14,0), /* #<FACE 300> and #<FACE 281> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 26> */
  gsSPVertex(&near__v[326],15,0),
  gsSP2Triangles(2,1,0,0,5,4,3,0), /* #<FACE 62> and #<FACE 67> */
  gsSP2Triangles(8,7,6,0,11,10,9,0), /* #<FACE 69> and #<FACE 78> */
  gsSP1Triangle(14,13,12,0), /* #<FACE 83> */
  gsSPVertex(&near__v[341],15,1),
  gsSP2Triangles(1,2,3,0,4,3,2,0), /* #<FACE 152> and #<FACE 96> */
  gsSP2Triangles(3,4,5,0,6,5,4,0), /* #<FACE 151> and #<FACE 97> */
  gsSP2Triangles(9,8,7,0,12,11,10,0), /* #<FACE 52> and #<FACE 54> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 58> */
  gsSPVertex(&near__v[356],16,0),
  gsSP2Triangles(0,1,2,0,3,2,1,0), /* #<FACE 183> and #<FACE 65> */
  gsSP2Triangles(4,5,6,0,7,6,5,0), /* #<FACE 178> and #<FACE 70> */
  gsSP2Triangles(6,7,8,0,9,8,7,0), /* #<FACE 177> and #<FACE 71> */
  gsSP2Triangles(10,11,12,0,13,12,11,0), /* #<FACE 164> and #<FACE 84> */
  gsSP2Triangles(12,13,14,0,15,14,13,0), /* #<FACE 163> and #<FACE 85> */
  gsSPVertex(&near__v[372],15,1),
  gsSP2Triangles(1,15,14,0,2,3,4,0), /* #<FACE 27> and #<FACE 193> */
  gsSP2Triangles(5,4,3,0,4,5,6,0), /* #<FACE 55> and #<FACE 192> */
  gsSP2Triangles(7,6,5,0,8,9,10,0), /* #<FACE 56> and #<FACE 189> */
  gsSP2Triangles(11,10,9,0,10,11,12,0), /* #<FACE 59> and #<FACE 188> */
  gsSP1Triangle(13,12,11,0), /* #<FACE 60> */
  gsSPVertex(&near__v[387],13,0),
  gsSP2Triangles(2,0,1,0,8,3,6,0), /* #<FACE 10> and #<FACE 291> */
  gsSP2Triangles(4,10,11,0,5,14,15,0), /* #<FACE 284> and #<FACE 280> */
  gsSP2Triangles(6,7,8,0,9,8,7,0), /* #<FACE 288> and #<FACE 19> */
  gsSP1Triangle(12,11,10,0), /* #<FACE 23> */
  gsSPVertex(&near__v[400],13,3),
  gsSP2Triangles(3,12,13,0,4,5,6,0), /* #<FACE 314> and #<FACE 309> */
  gsSP2Triangles(6,7,8,0,7,6,5,0), /* #<FACE 318> and #<FACE 13> */
  gsSP2Triangles(8,9,10,0,9,8,7,0), /* #<FACE 317> and #<FACE 4> */
  gsSP2Triangles(11,10,9,0,13,14,15,0), /* #<FACE 5> and #<FACE 313> */
  gsSP2Triangles(14,13,12,0,15,1,0,0), /* #<FACE 8> and #<FACE 312> */
  gsSP1Triangle(1,15,14,0), /* #<FACE 9> */
  gsSPEndDisplayList(),
};
