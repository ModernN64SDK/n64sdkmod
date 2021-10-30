/* middle_ in GBI format 
 *
 * File:  middle_.c
 * Create Date: 1/22/1997 11:31:37
*/

#include "middle_.h"

Gfx middle__dl[] = {
  gsDPPipeSync(),
  gsDPSetCycleType(G_CYC_1CYCLE),
  gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
  gsSPClearGeometryMode((G_SHADE|G_SHADING_SMOOTH|G_LIGHTING|G_TEXTURE_GEN|G_TEXTURE_GEN_LINEAR|G_CULL_BOTH|G_FOG)),
  gsSPSetGeometryMode( G_ZBUFFER | G_CULL_BACK | G_SHADE | G_SHADING_SMOOTH | G_LIGHTING ),
  gsDPSetColorDither(G_CD_BAYER),
  gsDPSetCombineMode(G_CC_MULPRIMSHADE,     G_CC_MULPRIMSHADE), /* N64-SHADE-TYPE-G */
  gsSPVertex(&middle__v[0],16,0),
  gsDPSetPrimColor(0,0,0,255,0,255),
  gsSP2Triangles(0,5,6,0,1,7,6,0), /* #<FACE 571> and #<FACE 570> */
  gsSP2Triangles(0,6,4,0,7,2,6,0), /* #<FACE 513> and #<FACE 454> */
  gsSP2Triangles(6,3,1,0,3,6,2,0), /* #<FACE 453> and #<FACE 30> */
  gsSP2Triangles(4,6,5,0,5,6,7,0), /* #<FACE 27> and #<FACE 26> */
  gsSP2Triangles(7,6,5,0,8,13,14,0), /* #<FACE 25> and #<FACE 395> */
  gsSP2Triangles(9,15,14,0,8,14,12,0), /* #<FACE 394> and #<FACE 337> */
  gsSP2Triangles(15,10,14,0,14,11,9,0), /* #<FACE 278> and #<FACE 277> */
  gsSP2Triangles(11,14,10,0,12,14,13,0), /* #<FACE 95> and #<FACE 92> */
  gsSP2Triangles(13,14,15,0,15,14,13,0), /* #<FACE 91> and #<FACE 90> */
  gsSPVertex(&middle__v[16],16,0),
  gsSP2Triangles(9,12,10,0,1,10,0,0), /* #<FACE 604> and #<FACE 582> */
  gsSP2Triangles(11,0,10,0,10,1,2,0), /* #<FACE 12> and #<FACE 11> */
  gsSP2Triangles(2,13,10,0,8,15,12,0), /* #<FACE 2> and #<FACE 609> */
  gsSP2Triangles(14,8,12,0,4,3,8,0), /* #<FACE 608> and #<FACE 607> */
  gsSP2Triangles(6,8,3,0,4,8,14,0), /* #<FACE 603> and #<FACE 602> */
  gsSP2Triangles(7,8,5,0,6,5,8,0), /* #<FACE 579> and #<FACE 15> */
  gsSP1Triangle(8,7,15,0), /* #<FACE 14> */
  gsSPVertex(&middle__v[32],9,0),
  gsSP2Triangles(15,11,12,0,1,12,0,0), /* #<FACE 601> and #<FACE 587> */
  gsSP2Triangles(9,0,12,0,12,1,14,0), /* #<FACE 7> and #<FACE 6> */
  gsSP2Triangles(2,3,6,0,4,6,3,0), /* #<FACE 365> and #<FACE 364> */
  gsSP2Triangles(4,5,6,0,7,6,5,0), /* #<FACE 279> and #<FACE 94> */
  gsSP2Triangles(8,10,13,0,11,10,12,0), /* #<FACE 610> and #<FACE 606> */
  gsSP1Triangle(9,10,8,0), /* #<FACE 605> */
  gsSPVertex(&middle__v[41],14,2),
  gsSP2Triangles(12,1,3,0,10,2,1,0), /* #<FACE 594> and #<FACE 588> */
  gsSP2Triangles(5,4,6,0,3,1,6,0), /* #<FACE 599> and #<FACE 598> */
  gsSP2Triangles(0,6,1,0,8,6,4,0), /* #<FACE 593> and #<FACE 592> */
  gsSP2Triangles(5,6,0,0,8,7,6,0), /* #<FACE 591> and #<FACE 583> */
  gsSP2Triangles(9,13,14,0,13,1,12,0), /* #<FACE 600> and #<FACE 597> */
  gsSP2Triangles(10,1,13,0,10,13,9,0), /* #<FACE 596> and #<FACE 595> */
  gsSP2Triangles(12,11,13,0,15,14,13,0), /* #<FACE 580> and #<FACE 3> */
  gsSPVertex(&middle__v[55],16,0),
  gsSP2Triangles(3,2,8,0,2,3,1,0), /* #<FACE 411> and #<FACE 72> */
  gsSP2Triangles(2,0,9,0,5,4,9,0), /* #<FACE 504> and #<FACE 495> */
  gsSP2Triangles(7,9,4,0,5,9,0,0), /* #<FACE 473> and #<FACE 465> */
  gsSP2Triangles(8,9,6,0,7,6,9,0), /* #<FACE 410> and #<FACE 74> */
  gsSP2Triangles(9,8,2,0,10,11,14,0), /* #<FACE 73> and #<FACE 541> */
  gsSP2Triangles(12,14,11,0,12,13,14,0), /* #<FACE 540> and #<FACE 455> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 29> */
  gsSPVertex(&middle__v[71],12,4),
  gsSP2Triangles(12,13,14,0,4,9,6,0), /* #<FACE 34> and #<FACE 488> */
  gsSP2Triangles(5,9,4,0,1,9,5,0), /* #<FACE 487> and #<FACE 481> */
  gsSP2Triangles(9,7,6,0,8,9,15,0), /* #<FACE 480> and #<FACE 413> */
  gsSP2Triangles(1,15,9,0,9,8,7,0), /* #<FACE 71> and #<FACE 70> */
  gsSP2Triangles(10,0,2,0,10,2,11,0), /* #<FACE 503> and #<FACE 490> */
  gsSP1Triangle(2,1,11,0), /* #<FACE 478> */
  gsSPVertex(&middle__v[83],12,0),
  gsSP2Triangles(15,2,1,0,1,6,0,0), /* #<FACE 412> and #<FACE 546> */
  gsSP2Triangles(5,6,3,0,6,4,3,0), /* #<FACE 543> and #<FACE 538> */
  gsSP2Triangles(0,6,5,0,14,9,12,0), /* #<FACE 536> and #<FACE 510> */
  gsSP2Triangles(11,12,7,0,8,7,12,0), /* #<FACE 507> and #<FACE 462> */
  gsSP2Triangles(9,8,12,0,13,12,10,0), /* #<FACE 459> and #<FACE 449> */
  gsSP1Triangle(11,10,12,0), /* #<FACE 35> */
  gsSPVertex(&middle__v[95],14,2),
  gsSP2Triangles(15,12,13,0,14,2,13,0), /* #<FACE 515> and #<FACE 450> */
  gsSP2Triangles(3,4,6,0,5,3,6,0), /* #<FACE 555> and #<FACE 554> */
  gsSP2Triangles(8,6,4,0,6,11,5,0), /* #<FACE 528> and #<FACE 523> */
  gsSP2Triangles(8,7,6,0,9,10,1,0), /* #<FACE 414> and #<FACE 563> */
  gsSP2Triangles(9,1,0,0,11,1,10,0), /* #<FACE 545> and #<FACE 520> */
  gsSPVertex(&middle__v[109],12,0),
  gsSP2Triangles(3,2,8,0,2,3,1,0), /* #<FACE 235> and #<FACE 137> */
  gsSP2Triangles(2,0,9,0,5,4,9,0), /* #<FACE 328> and #<FACE 319> */
  gsSP2Triangles(7,9,4,0,5,9,0,0), /* #<FACE 297> and #<FACE 289> */
  gsSP2Triangles(8,9,6,0,7,6,9,0), /* #<FACE 234> and #<FACE 139> */
  gsSP2Triangles(9,8,2,0,10,15,13,0), /* #<FACE 138> and #<FACE 568> */
  gsSP2Triangles(14,13,11,0,12,11,13,0), /* #<FACE 542> and #<FACE 539> */
  gsSPVertex(&middle__v[121],12,4),
  gsSP2Triangles(13,14,15,0,4,10,6,0), /* #<FACE 99> and #<FACE 312> */
  gsSP2Triangles(5,10,4,0,1,10,5,0), /* #<FACE 311> and #<FACE 305> */
  gsSP2Triangles(10,8,6,0,9,10,7,0), /* #<FACE 304> and #<FACE 237> */
  gsSP2Triangles(1,7,10,0,10,9,8,0), /* #<FACE 136> and #<FACE 135> */
  gsSP2Triangles(11,0,2,0,11,2,12,0), /* #<FACE 327> and #<FACE 314> */
  gsSP1Triangle(2,1,12,0), /* #<FACE 302> */
  gsSPVertex(&middle__v[133],12,0),
  gsSP2Triangles(2,5,0,0,1,0,5,0), /* #<FACE 391> and #<FACE 340> */
  gsSP2Triangles(2,4,5,0,5,6,3,0), /* #<FACE 248> and #<FACE 247> */
  gsSP2Triangles(6,5,4,0,15,9,13,0), /* #<FACE 125> and #<FACE 334> */
  gsSP2Triangles(11,13,7,0,8,7,13,0), /* #<FACE 331> and #<FACE 286> */
  gsSP2Triangles(9,8,13,0,14,13,10,0), /* #<FACE 283> and #<FACE 273> */
  gsSP1Triangle(11,10,13,0), /* #<FACE 100> */
  gsSPVertex(&middle__v[145],16,0),
  gsSP2Triangles(0,1,3,0,2,0,3,0), /* #<FACE 379> and #<FACE 378> */
  gsSP2Triangles(5,3,1,0,3,9,2,0), /* #<FACE 352> and #<FACE 347> */
  gsSP2Triangles(5,4,3,0,6,7,10,0), /* #<FACE 238> and #<FACE 387> */
  gsSP2Triangles(6,10,15,0,9,10,7,0), /* #<FACE 369> and #<FACE 344> */
  gsSP2Triangles(9,8,10,0,10,14,15,0), /* #<FACE 236> and #<FACE 370> */
  gsSP2Triangles(13,14,11,0,14,12,11,0), /* #<FACE 367> and #<FACE 362> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 360> */
  gsSPVertex(&middle__v[161],16,0),
  gsSP2Triangles(0,2,4,0,3,4,2,0), /* #<FACE 181> and #<FACE 174> */
  gsSP2Triangles(2,0,1,0,5,8,9,0), /* #<FACE 173> and #<FACE 392> */
  gsSP2Triangles(11,9,6,0,7,6,9,0), /* #<FACE 366> and #<FACE 363> */
  gsSP2Triangles(8,7,9,0,11,10,9,0), /* #<FACE 339> and #<FACE 274> */
  gsSP2Triangles(14,15,12,0,15,14,13,0), /* #<FACE 246> and #<FACE 126> */
  gsSPVertex(&middle__v[177],11,5),
  gsSP2Triangles(11,1,0,0,7,13,5,0), /* #<FACE 182> and #<FACE 207> */
  gsSP2Triangles(9,7,15,0,5,15,7,0), /* #<FACE 188> and #<FACE 167> */
  gsSP2Triangles(7,9,14,0,8,13,7,0), /* #<FACE 166> and #<FACE 202> */
  gsSP2Triangles(6,8,1,0,10,8,12,0), /* #<FACE 201> and #<FACE 184> */
  gsSP2Triangles(6,12,8,0,8,10,13,0), /* #<FACE 171> and #<FACE 170> */
  gsSP2Triangles(7,14,2,0,2,1,8,0), /* #<FACE 205> and #<FACE 204> */
  gsSP2Triangles(2,8,7,0,3,2,14,0), /* #<FACE 203> and #<FACE 200> */
  gsSPVertex(&middle__v[188],8,0),
  gsSP2Triangles(3,2,0,0,3,0,1,0), /* #<FACE 159> and #<FACE 155> */
  gsSP2Triangles(9,15,4,0,5,14,9,0), /* #<FACE 197> and #<FACE 189> */
  gsSP2Triangles(4,10,9,0,4,13,10,0), /* #<FACE 193> and #<FACE 185> */
  gsSP2Triangles(12,11,10,0,7,10,11,0), /* #<FACE 165> and #<FACE 198> */
  gsSP2Triangles(7,9,10,0,5,9,7,0), /* #<FACE 195> and #<FACE 194> */
  gsSP1Triangle(7,6,5,0), /* #<FACE 192> */
  gsSPVertex(&middle__v[196],12,4),
  gsSP2Triangles(14,4,15,0,4,14,13,0), /* #<FACE 422> and #<FACE 61> */
  gsSP2Triangles(5,2,8,0,5,6,2,0), /* #<FACE 216> and #<FACE 215> */
  gsSP2Triangles(7,2,6,0,7,0,2,0), /* #<FACE 214> and #<FACE 211> */
  gsSP2Triangles(11,8,2,0,9,3,1,0), /* #<FACE 156> and #<FACE 219> */
  gsSP2Triangles(9,10,3,0,12,3,10,0), /* #<FACE 218> and #<FACE 217> */
  gsSP2Triangles(2,3,11,0,12,11,3,0), /* #<FACE 210> and #<FACE 160> */
  gsSPVertex(&middle__v[208],13,0),
  gsSP2Triangles(0,3,6,0,6,2,0,0), /* #<FACE 506> and #<FACE 464> */
  gsSP2Triangles(1,6,7,0,6,1,2,0), /* #<FACE 399> and #<FACE 84> */
  gsSP2Triangles(8,6,3,0,7,8,4,0), /* #<FACE 463> and #<FACE 398> */
  gsSP2Triangles(5,4,8,0,8,7,6,0), /* #<FACE 86> and #<FACE 85> */
  gsSP2Triangles(10,12,9,0,15,9,12,0), /* #<FACE 567> and #<FACE 516> */
  gsSP2Triangles(10,11,12,0,13,12,11,0), /* #<FACE 424> and #<FACE 60> */
  gsSP1Triangle(12,13,14,0), /* #<FACE 423> */
  gsSPVertex(&middle__v[221],12,4),
  gsSP2Triangles(15,4,5,0,5,13,15,0), /* #<FACE 500> and #<FACE 469> */
  gsSP2Triangles(14,5,6,0,5,14,13,0), /* #<FACE 403> and #<FACE 80> */
  gsSP2Triangles(4,7,8,0,8,5,4,0), /* #<FACE 501> and #<FACE 468> */
  gsSP2Triangles(6,8,9,0,8,6,5,0), /* #<FACE 402> and #<FACE 81> */
  gsSP2Triangles(7,10,11,0,11,8,7,0), /* #<FACE 502> and #<FACE 467> */
  gsSP2Triangles(9,11,12,0,11,9,8,0), /* #<FACE 401> and #<FACE 82> */
  gsSP2Triangles(10,0,2,0,2,11,10,0), /* #<FACE 505> and #<FACE 466> */
  gsSP2Triangles(12,2,1,0,2,12,11,0), /* #<FACE 400> and #<FACE 83> */
  gsSPVertex(&middle__v[233],12,0),
  gsSP2Triangles(0,3,4,0,4,2,0,0), /* #<FACE 496> and #<FACE 474> */
  gsSP2Triangles(1,4,5,0,4,1,2,0), /* #<FACE 407> and #<FACE 76> */
  gsSP2Triangles(3,6,7,0,7,4,3,0), /* #<FACE 497> and #<FACE 472> */
  gsSP2Triangles(5,7,8,0,7,5,4,0), /* #<FACE 406> and #<FACE 77> */
  gsSP2Triangles(6,9,10,0,10,7,6,0), /* #<FACE 498> and #<FACE 471> */
  gsSP2Triangles(8,10,11,0,10,8,7,0), /* #<FACE 405> and #<FACE 78> */
  gsSP2Triangles(9,15,13,0,13,10,9,0), /* #<FACE 499> and #<FACE 470> */
  gsSP2Triangles(11,13,14,0,13,11,10,0), /* #<FACE 404> and #<FACE 79> */
  gsSPVertex(&middle__v[245],13,3),
  gsSP2Triangles(3,4,15,0,4,13,15,0), /* #<FACE 489> and #<FACE 479> */
  gsSP2Triangles(4,14,13,0,7,6,5,0), /* #<FACE 69> and #<FACE 486> */
  gsSP2Triangles(8,10,11,0,9,11,12,0), /* #<FACE 493> and #<FACE 409> */
  gsSP2Triangles(10,0,2,0,2,11,10,0), /* #<FACE 494> and #<FACE 475> */
  gsSP2Triangles(12,2,1,0,2,12,11,0), /* #<FACE 408> and #<FACE 75> */
  gsSPVertex(&middle__v[258],12,0),
  gsSP2Triangles(1,5,2,0,5,4,2,0), /* #<FACE 492> and #<FACE 476> */
  gsSP2Triangles(3,5,6,0,5,3,4,0), /* #<FACE 418> and #<FACE 65> */
  gsSP2Triangles(0,7,1,0,7,5,1,0), /* #<FACE 491> and #<FACE 477> */
  gsSP2Triangles(6,7,8,0,7,6,5,0), /* #<FACE 417> and #<FACE 66> */
  gsSP2Triangles(9,10,0,0,10,7,0,0), /* #<FACE 485> and #<FACE 482> */
  gsSP2Triangles(8,10,11,0,10,8,7,0), /* #<FACE 416> and #<FACE 67> */
  gsSP2Triangles(15,13,9,0,13,10,9,0), /* #<FACE 484> and #<FACE 483> */
  gsSP2Triangles(11,13,14,0,13,11,10,0), /* #<FACE 415> and #<FACE 68> */
  gsSPVertex(&middle__v[270],10,6),
  gsSP2Triangles(15,1,6,0,1,15,0,0), /* #<FACE 426> and #<FACE 57> */
  gsSP2Triangles(6,2,7,0,2,6,1,0), /* #<FACE 425> and #<FACE 58> */
  gsSP2Triangles(12,7,2,0,8,9,10,0), /* #<FACE 59> and #<FACE 28> */
  gsSP2Triangles(12,13,9,0,13,10,9,0), /* #<FACE 511> and #<FACE 458> */
  gsSP2Triangles(11,13,14,0,13,11,10,0), /* #<FACE 420> and #<FACE 63> */
  gsSP2Triangles(2,4,12,0,4,13,12,0), /* #<FACE 508> and #<FACE 461> */
  gsSP2Triangles(14,4,3,0,4,14,13,0), /* #<FACE 419> and #<FACE 64> */
  gsSPVertex(&middle__v[280],15,0),
  gsSP2Triangles(1,3,4,0,3,1,0,0), /* #<FACE 433> and #<FACE 50> */
  gsSP2Triangles(4,5,2,0,6,2,5,0), /* #<FACE 432> and #<FACE 52> */
  gsSP2Triangles(5,4,3,0,7,8,9,0), /* #<FACE 51> and #<FACE 430> */
  gsSP2Triangles(8,7,6,0,9,10,11,0), /* #<FACE 53> and #<FACE 429> */
  gsSP2Triangles(10,9,8,0,11,12,13,0), /* #<FACE 54> and #<FACE 428> */
  gsSP2Triangles(12,11,10,0,13,14,15,0), /* #<FACE 55> and #<FACE 427> */
  gsSP1Triangle(14,13,12,0), /* #<FACE 56> */
  gsSPVertex(&middle__v[295],14,2),
  gsSP2Triangles(15,2,3,0,2,15,14,0), /* #<FACE 440> and #<FACE 43> */
  gsSP2Triangles(3,4,5,0,4,3,2,0), /* #<FACE 439> and #<FACE 44> */
  gsSP2Triangles(5,7,8,0,7,5,4,0), /* #<FACE 438> and #<FACE 45> */
  gsSP2Triangles(8,9,6,0,11,6,9,0), /* #<FACE 437> and #<FACE 47> */
  gsSP2Triangles(9,8,7,0,12,13,10,0), /* #<FACE 46> and #<FACE 435> */
  gsSP2Triangles(0,10,13,0,13,12,11,0), /* #<FACE 49> and #<FACE 48> */
  gsSPVertex(&middle__v[309],14,0),
  gsSP2Triangles(0,2,3,0,2,0,1,0), /* #<FACE 447> and #<FACE 36> */
  gsSP2Triangles(3,4,5,0,4,3,2,0), /* #<FACE 446> and #<FACE 37> */
  gsSP2Triangles(5,6,7,0,6,5,4,0), /* #<FACE 445> and #<FACE 38> */
  gsSP2Triangles(7,8,9,0,8,7,6,0), /* #<FACE 444> and #<FACE 39> */
  gsSP2Triangles(9,10,11,0,10,9,8,0), /* #<FACE 443> and #<FACE 40> */
  gsSP2Triangles(11,12,13,0,12,11,10,0), /* #<FACE 442> and #<FACE 41> */
  gsSP2Triangles(13,14,15,0,14,13,12,0), /* #<FACE 441> and #<FACE 42> */
  gsSPVertex(&middle__v[323],14,2),
  gsSP2Triangles(2,3,4,0,10,4,3,0), /* #<FACE 397> and #<FACE 87> */
  gsSP2Triangles(9,13,5,0,4,5,13,0), /* #<FACE 569> and #<FACE 514> */
  gsSP2Triangles(4,10,5,0,11,5,10,0), /* #<FACE 396> and #<FACE 88> */
  gsSP2Triangles(6,7,8,0,8,7,12,0), /* #<FACE 457> and #<FACE 512> */
  gsSP2Triangles(7,11,12,0,9,12,13,0), /* #<FACE 456> and #<FACE 451> */
  gsSP2Triangles(12,9,8,0,12,11,14,0), /* #<FACE 32> and #<FACE 509> */
  gsSP2Triangles(11,10,14,0,14,13,12,0), /* #<FACE 460> and #<FACE 33> */
  gsSP1Triangle(15,1,0,0), /* #<FACE 448> */
  gsSPVertex(&middle__v[337],15,0),
  gsSP2Triangles(2,1,0,0,8,4,6,0), /* #<FACE 113> and #<FACE 310> */
  gsSP2Triangles(7,8,5,0,6,5,8,0), /* #<FACE 257> and #<FACE 116> */
  gsSP2Triangles(8,7,3,0,9,12,11,0), /* #<FACE 115> and #<FACE 565> */
  gsSP2Triangles(10,12,9,0,11,14,15,0), /* #<FACE 519> and #<FACE 566> */
  gsSP2Triangles(12,14,11,0,14,13,15,0), /* #<FACE 518> and #<FACE 517> */
  gsSPVertex(&middle__v[352],12,4),
  gsSP2Triangles(5,13,15,0,15,4,5,0), /* #<FACE 320> and #<FACE 298> */
  gsSP2Triangles(14,5,6,0,5,14,13,0), /* #<FACE 263> and #<FACE 109> */
  gsSP2Triangles(9,5,4,0,4,7,9,0), /* #<FACE 318> and #<FACE 299> */
  gsSP2Triangles(6,9,10,0,9,6,5,0), /* #<FACE 262> and #<FACE 110> */
  gsSP2Triangles(11,9,7,0,10,11,8,0), /* #<FACE 317> and #<FACE 261> */
  gsSP2Triangles(0,8,11,0,11,10,9,0), /* #<FACE 112> and #<FACE 111> */
  gsSP2Triangles(1,2,12,0,3,12,2,0), /* #<FACE 259> and #<FACE 114> */
  gsSPVertex(&middle__v[364],13,0),
  gsSP2Triangles(0,1,2,0,5,4,3,0), /* #<FACE 527> and #<FACE 560> */
  gsSP2Triangles(6,9,7,0,9,8,7,0), /* #<FACE 544> and #<FACE 537> */
  gsSP2Triangles(13,11,10,0,10,15,13,0), /* #<FACE 321> and #<FACE 296> */
  gsSP2Triangles(12,13,14,0,13,12,11,0), /* #<FACE 264> and #<FACE 108> */
  gsSPVertex(&middle__v[377],13,3),
  gsSP2Triangles(4,3,6,0,7,5,8,0), /* #<FACE 421> and #<FACE 562> */
  gsSP2Triangles(6,8,5,0,9,7,10,0), /* #<FACE 521> and #<FACE 561> */
  gsSP2Triangles(8,10,7,0,11,9,12,0), /* #<FACE 522> and #<FACE 558> */
  gsSP2Triangles(10,12,9,0,13,11,14,0), /* #<FACE 525> and #<FACE 557> */
  gsSP2Triangles(12,14,11,0,2,13,0,0), /* #<FACE 526> and #<FACE 559> */
  gsSP2Triangles(14,0,13,0,15,2,1,0), /* #<FACE 524> and #<FACE 556> */
  gsSPVertex(&middle__v[390],15,0),
  gsSP2Triangles(4,1,0,0,0,3,4,0), /* #<FACE 325> and #<FACE 292> */
  gsSP2Triangles(2,4,5,0,4,2,1,0), /* #<FACE 268> and #<FACE 104> */
  gsSP2Triangles(7,4,3,0,3,6,7,0), /* #<FACE 324> and #<FACE 293> */
  gsSP2Triangles(5,7,8,0,7,5,4,0), /* #<FACE 267> and #<FACE 105> */
  gsSP2Triangles(12,7,6,0,6,10,12,0), /* #<FACE 323> and #<FACE 294> */
  gsSP2Triangles(8,12,13,0,12,8,7,0), /* #<FACE 266> and #<FACE 106> */
  gsSP2Triangles(14,12,10,0,10,9,14,0), /* #<FACE 322> and #<FACE 295> */
  gsSP2Triangles(13,14,11,0,14,13,12,0), /* #<FACE 265> and #<FACE 107> */
  gsSPVertex(&middle__v[405],16,0),
  gsSP2Triangles(0,14,15,0,1,14,0,0), /* #<FACE 549> and #<FACE 534> */
  gsSP2Triangles(2,1,0,0,3,1,2,0), /* #<FACE 548> and #<FACE 535> */
  gsSP2Triangles(4,3,2,0,7,6,5,0), /* #<FACE 547> and #<FACE 436> */
  gsSP2Triangles(10,9,8,0,13,12,11,0), /* #<FACE 434> and #<FACE 431> */
  gsSPVertex(&middle__v[421],13,0),
  gsSP2Triangles(2,1,0,0,5,4,3,0), /* #<FACE 452> and #<FACE 564> */
  gsSP2Triangles(6,4,5,0,7,6,5,0), /* #<FACE 529> and #<FACE 553> */
  gsSP2Triangles(8,6,7,0,9,8,7,0), /* #<FACE 530> and #<FACE 552> */
  gsSP2Triangles(10,8,9,0,11,10,9,0), /* #<FACE 531> and #<FACE 551> */
  gsSP2Triangles(12,10,11,0,15,12,11,0), /* #<FACE 532> and #<FACE 550> */
  gsSP1Triangle(14,12,15,0), /* #<FACE 533> */
  gsSPVertex(&middle__v[434],15,1),
  gsSP2Triangles(1,2,3,0,3,14,1,0), /* #<FACE 326> and #<FACE 291> */
  gsSP2Triangles(15,3,4,0,3,15,14,0), /* #<FACE 225> and #<FACE 147> */
  gsSP2Triangles(2,5,6,0,6,3,2,0), /* #<FACE 329> and #<FACE 290> */
  gsSP2Triangles(4,6,7,0,6,4,3,0), /* #<FACE 224> and #<FACE 148> */
  gsSP2Triangles(5,8,11,0,11,6,5,0), /* #<FACE 330> and #<FACE 288> */
  gsSP2Triangles(7,11,12,0,11,7,6,0), /* #<FACE 223> and #<FACE 149> */
  gsSP2Triangles(13,11,8,0,12,13,9,0), /* #<FACE 287> and #<FACE 222> */
  gsSP2Triangles(10,9,13,0,13,12,11,0), /* #<FACE 151> and #<FACE 150> */
  gsSPVertex(&middle__v[449],14,0),
  gsSP2Triangles(0,2,3,0,2,0,1,0), /* #<FACE 232> and #<FACE 140> */
  gsSP2Triangles(3,4,5,0,4,3,2,0), /* #<FACE 231> and #<FACE 141> */
  gsSP2Triangles(5,6,7,0,6,5,4,0), /* #<FACE 230> and #<FACE 142> */
  gsSP2Triangles(7,8,9,0,8,7,6,0), /* #<FACE 229> and #<FACE 143> */
  gsSP2Triangles(9,10,11,0,10,9,8,0), /* #<FACE 228> and #<FACE 144> */
  gsSP2Triangles(11,12,13,0,12,11,10,0), /* #<FACE 227> and #<FACE 145> */
  gsSP2Triangles(13,14,15,0,14,13,12,0), /* #<FACE 226> and #<FACE 146> */
  gsSPVertex(&middle__v[463],14,2),
  gsSP2Triangles(15,12,2,0,12,15,13,0), /* #<FACE 241> and #<FACE 131> */
  gsSP2Triangles(3,4,14,0,4,12,14,0), /* #<FACE 309> and #<FACE 306> */
  gsSP2Triangles(2,4,5,0,4,2,12,0), /* #<FACE 240> and #<FACE 132> */
  gsSP2Triangles(7,8,3,0,8,4,3,0), /* #<FACE 308> and #<FACE 307> */
  gsSP2Triangles(5,8,9,0,8,5,4,0), /* #<FACE 239> and #<FACE 133> */
  gsSP2Triangles(6,10,7,0,10,8,7,0), /* #<FACE 313> and #<FACE 303> */
  gsSP2Triangles(10,9,8,0,11,1,0,0), /* #<FACE 134> and #<FACE 233> */
  gsSPVertex(&middle__v[477],10,0),
  gsSP2Triangles(3,4,0,0,4,1,0,0), /* #<FACE 335> and #<FACE 282> */
  gsSP2Triangles(2,4,5,0,4,2,1,0), /* #<FACE 244> and #<FACE 128> */
  gsSP2Triangles(6,7,3,0,7,4,3,0), /* #<FACE 332> and #<FACE 285> */
  gsSP2Triangles(5,7,8,0,7,5,4,0), /* #<FACE 243> and #<FACE 129> */
  gsSP2Triangles(9,13,6,0,13,7,6,0), /* #<FACE 316> and #<FACE 300> */
  gsSP2Triangles(8,13,15,0,13,8,7,0), /* #<FACE 242> and #<FACE 130> */
  gsSP2Triangles(14,12,9,0,12,13,9,0), /* #<FACE 315> and #<FACE 301> */
  gsSPVertex(&middle__v[487],16,0),
  gsSP2Triangles(0,14,2,0,14,0,15,0), /* #<FACE 275> and #<FACE 97> */
  gsSP2Triangles(14,13,3,0,13,1,3,0), /* #<FACE 333> and #<FACE 284> */
  gsSP2Triangles(3,2,14,0,4,5,6,0), /* #<FACE 98> and #<FACE 272> */
  gsSP2Triangles(6,7,8,0,7,6,5,0), /* #<FACE 271> and #<FACE 101> */
  gsSP2Triangles(8,10,11,0,10,8,7,0), /* #<FACE 270> and #<FACE 102> */
  gsSP2Triangles(11,12,9,0,12,11,10,0), /* #<FACE 269> and #<FACE 103> */
  gsSPVertex(&middle__v[503],13,0),
  gsSP2Triangles(1,4,0,0,3,0,4,0), /* #<FACE 382> and #<FACE 349> */
  gsSP2Triangles(1,2,4,0,5,4,2,0), /* #<FACE 251> and #<FACE 122> */
  gsSP2Triangles(4,8,3,0,6,3,8,0), /* #<FACE 385> and #<FACE 346> */
  gsSP2Triangles(4,5,8,0,9,8,5,0), /* #<FACE 250> and #<FACE 123> */
  gsSP2Triangles(8,10,6,0,7,6,10,0), /* #<FACE 386> and #<FACE 345> */
  gsSP2Triangles(8,9,10,0,11,10,9,0), /* #<FACE 249> and #<FACE 124> */
  gsSP2Triangles(15,12,14,0,12,13,14,0), /* #<FACE 336> and #<FACE 280> */
  gsSPVertex(&middle__v[516],13,3),
  gsSP2Triangles(15,3,4,0,4,5,7,0), /* #<FACE 256> and #<FACE 255> */
  gsSP2Triangles(8,7,5,0,7,10,6,0), /* #<FACE 118> and #<FACE 380> */
  gsSP2Triangles(9,6,10,0,7,8,10,0), /* #<FACE 351> and #<FACE 254> */
  gsSP2Triangles(11,10,8,0,10,13,9,0), /* #<FACE 119> and #<FACE 383> */
  gsSP2Triangles(12,9,13,0,10,11,13,0), /* #<FACE 348> and #<FACE 253> */
  gsSP2Triangles(14,13,11,0,13,1,12,0), /* #<FACE 120> and #<FACE 381> */
  gsSP2Triangles(0,12,1,0,13,14,1,0), /* #<FACE 350> and #<FACE 252> */
  gsSP1Triangle(2,1,14,0), /* #<FACE 121> */
  gsSPVertex(&middle__v[529],15,0),
  gsSP2Triangles(2,0,1,0,3,2,1,0), /* #<FACE 358> and #<FACE 372> */
  gsSP2Triangles(4,2,3,0,5,4,3,0), /* #<FACE 359> and #<FACE 371> */
  gsSP2Triangles(6,4,5,0,7,6,5,0), /* #<FACE 361> and #<FACE 368> */
  gsSP2Triangles(10,9,8,0,12,11,14,0), /* #<FACE 260> and #<FACE 258> */
  gsSP1Triangle(14,15,13,0), /* #<FACE 384> */
  gsSPVertex(&middle__v[544],14,2),
  gsSP2Triangles(13,4,2,0,12,4,13,0), /* #<FACE 390> and #<FACE 342> */
  gsSP2Triangles(4,3,2,0,7,6,5,0), /* #<FACE 341> and #<FACE 153> */
  gsSP2Triangles(10,9,8,0,11,14,15,0), /* #<FACE 281> and #<FACE 374> */
  gsSP2Triangles(0,14,11,0,1,0,11,0), /* #<FACE 357> and #<FACE 373> */
  gsSPVertex(&middle__v[558],12,0),
  gsSP2Triangles(2,3,0,0,3,2,1,0), /* #<FACE 127> and #<FACE 245> */
  gsSP2Triangles(15,5,4,0,14,5,15,0), /* #<FACE 375> and #<FACE 356> */
  gsSP2Triangles(4,7,6,0,5,7,4,0), /* #<FACE 376> and #<FACE 355> */
  gsSP2Triangles(6,9,8,0,7,9,6,0), /* #<FACE 377> and #<FACE 354> */
  gsSP2Triangles(8,11,10,0,9,11,8,0), /* #<FACE 388> and #<FACE 353> */
  gsSP2Triangles(10,12,13,0,11,12,10,0), /* #<FACE 389> and #<FACE 343> */
  gsSPVertex(&middle__v[570],16,0),
  gsSP2Triangles(0,13,12,0,12,1,0,0), /* #<FACE 169> and #<FACE 164> */
  gsSP2Triangles(8,9,2,0,3,2,9,0), /* #<FACE 206> and #<FACE 199> */
  gsSP2Triangles(10,2,11,0,3,11,2,0), /* #<FACE 180> and #<FACE 175> */
  gsSP2Triangles(5,4,6,0,6,7,15,0), /* #<FACE 276> and #<FACE 393> */
  gsSP1Triangle(14,15,7,0), /* #<FACE 338> */
  gsSPVertex(&middle__v[586],7,0),
  gsSP2Triangles(0,1,14,0,2,14,1,0), /* #<FACE 221> and #<FACE 152> */
  gsSP2Triangles(14,2,15,0,4,13,3,0), /* #<FACE 220> and #<FACE 190> */
  gsSP2Triangles(4,12,13,0,11,5,10,0), /* #<FACE 186> and #<FACE 191> */
  gsSP2Triangles(6,10,5,0,5,9,6,0), /* #<FACE 196> and #<FACE 178> */
  gsSP1Triangle(8,6,9,0), /* #<FACE 177> */
  gsSPVertex(&middle__v[593],15,1),
  gsSP2Triangles(14,13,15,0,3,2,1,0), /* #<FACE 18> and #<FACE 31> */
  gsSP2Triangles(6,5,4,0,9,8,7,0), /* #<FACE 117> and #<FACE 93> */
  gsSP1Triangle(12,11,10,0), /* #<FACE 62> */
  gsSPVertex(&middle__v[608],12,0),
  gsSP2Triangles(0,4,5,0,3,2,1,0), /* #<FACE 581> and #<FACE 96> */
  gsSP2Triangles(6,5,4,0,8,10,11,0), /* #<FACE 13> and #<FACE 574> */
  gsSP2Triangles(11,9,8,0,9,14,7,0), /* #<FACE 576> and #<FACE 572> */
  gsSP2Triangles(7,8,9,0,7,15,8,0), /* #<FACE 19> and #<FACE 575> */
  gsSP2Triangles(15,7,14,0,10,8,15,0), /* #<FACE 23> and #<FACE 20> */
  gsSP2Triangles(10,15,13,0,11,13,9,0), /* #<FACE 577> and #<FACE 573> */
  gsSP2Triangles(14,9,13,0,13,11,10,0), /* #<FACE 22> and #<FACE 21> */
  gsSPVertex(&middle__v[620],16,0),
  gsSP2Triangles(0,1,2,0,2,3,4,0), /* #<FACE 578> and #<FACE 590> */
  gsSP2Triangles(3,2,1,0,4,5,6,0), /* #<FACE 16> and #<FACE 589> */
  gsSP2Triangles(5,4,3,0,7,6,5,0), /* #<FACE 4> and #<FACE 5> */
  gsSP2Triangles(8,9,10,0,10,13,14,0), /* #<FACE 586> and #<FACE 585> */
  gsSP2Triangles(13,10,9,0,14,15,11,0), /* #<FACE 8> and #<FACE 584> */
  gsSP2Triangles(12,11,15,0,15,14,13,0), /* #<FACE 10> and #<FACE 9> */
  gsSPVertex(&middle__v[636],15,0),
  gsSP2Triangles(2,1,0,0,3,9,10,0), /* #<FACE 161> and #<FACE 183> */
  gsSP2Triangles(4,12,13,0,5,6,7,0), /* #<FACE 179> and #<FACE 187> */
  gsSP2Triangles(8,7,6,0,11,10,9,0), /* #<FACE 168> and #<FACE 172> */
  gsSP1Triangle(14,13,12,0), /* #<FACE 176> */
  gsSPVertex(&middle__v[651],7,9),
  gsSP2Triangles(9,0,1,0,14,15,12,0), /* #<FACE 209> and #<FACE 213> */
  gsSP2Triangles(12,13,10,0,11,10,13,0), /* #<FACE 212> and #<FACE 158> */
  gsSP2Triangles(13,12,15,0,1,2,14,0), /* #<FACE 157> and #<FACE 208> */
  gsSP1Triangle(15,14,2,0), /* #<FACE 162> */
  gsSPEndDisplayList(),
};
