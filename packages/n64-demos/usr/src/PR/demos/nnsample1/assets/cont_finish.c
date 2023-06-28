/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 1

		Copyright (C) 1997, NINTENDO Co,Ltd.

============================================================================*/

#include "cont_finish.h"
#include "reflect.h"


#define G_CC_MULPRIMSHADE PRIMITIVE,0,SHADE,0,PRIMITIVE,0,SHADE,0

Gfx cont_finish_dl[] = {
/*  gsSPMatrix(&frame_matrices_for_object[0][0], G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_PUSH),
*/
  gsDPPipeSync(),
  gsDPSetCycleType(G_CYC_2CYCLE),
  gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
  gsSPClearGeometryMode((G_SHADE|G_SHADING_SMOOTH|G_LIGHTING|G_TEXTURE_GEN|G_TEXTURE_GEN_LINEAR|G_CULL_BOTH|G_FOG)),
  gsSPSetGeometryMode( G_ZBUFFER | G_CULL_BACK | G_FOG),
  gsSPFogPosition(990, 1000),
  gsDPSetFogColor(0, 0, 0, 255),

  gsDPSetCombineMode(G_CC_PRIMITIVE, G_CC_PRIMITIVE), /* N64-SHADE-TYPE-H */
  gsSPVertex(&cont_finish_v[0],4,0),
  gsDPSetPrimColor(0,0,0,0,0,255),
  gsSP2Triangles(0,3,1,0,3,2,1,0), /* #<FACE 3215> and #<FACE 169> */
  gsDPPipeSync(),
  gsDPSetCycleType(G_CYC_2CYCLE),
/*  gsDPSetRenderMode(G_RM_PASS, G_RM_AA_ZB_OPA_SURF2),*/
  gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
  gsSPClearGeometryMode((G_SHADE|G_SHADING_SMOOTH|G_LIGHTING|G_TEXTURE_GEN|G_TEXTURE_GEN_LINEAR|G_CULL_BOTH|G_FOG)),
  gsSPSetGeometryMode( G_ZBUFFER | G_CULL_BACK | G_SHADE | G_SHADING_SMOOTH | G_LIGHTING | G_TEXTURE_GEN | G_FOG),
  gsSPTexture (32<<6, 32<<6, 0, G_TX_RENDERTILE, G_ON),
  gsDPSetTexturePersp (G_TP_PERSP),
  gsDPSetTextureDetail (G_TD_CLAMP),
  gsDPSetTextureLOD (G_TL_TILE),
  gsDPSetTextureLUT (G_TT_NONE),
  gsDPLoadTextureTile   (Silver_Reflection, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32,32,
                       0,0,31,31,0,G_TX_WRAP,G_TX_WRAP,
                       5,5,G_TX_NOLOD, G_TX_NOLOD),
  gsDPSetTextureFilter (G_TF_BILERP),
  gsDPSetTextureConvert(G_TC_FILT),
/*  gsSPSetLights1(NWorldLight),*/

  gsSPFogPosition(990, 1000),

  gsDPSetFogColor(0, 0, 0, 255),
/*  gsDPSetColorDither(G_CD_BAYER),*/
  gsDPSetColorDither(G_CD_DISABLE),
  gsDPSetCombineMode(G_CC_MODULATERGBA,G_CC_MODULATERGBA_PRIM2), /* N64-SHADE-TYPE-B */
  gsSPVertex(&cont_finish_v[4],16,0),
  gsDPSetPrimColor(0,0,179,179,179,255),
  gsSP2Triangles(1,5,6,0,4,5,2,0), /* #<FACE 1108> and #<FACE 454> */
  gsSP2Triangles(5,0,3,0,12,15,8,0), /* #<FACE 450> and #<FACE 2430> */
  gsSP2Triangles(13,7,15,0,7,9,15,0), /* #<FACE 2426> and #<FACE 1243> */
  gsSP2Triangles(11,8,15,0,15,9,10,0), /* #<FACE 1239> and #<FACE 698> */
  gsSP2Triangles(11,15,10,0,12,14,15,0), /* #<FACE 697> and #<FACE 587> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 582> */
  gsSPVertex(&cont_finish_v[20],8,8),
  gsSP2Triangles(9,10,11,0,15,10,2,0), /* #<FACE 2939> and #<FACE 2929> */
  gsSP2Triangles(12,10,15,0,4,10,14,0), /* #<FACE 2928> and #<FACE 2552> */
  gsSP2Triangles(8,14,10,0,9,8,10,0), /* #<FACE 1117> and #<FACE 1105> */
  gsSP2Triangles(2,10,4,0,12,11,10,0), /* #<FACE 455> and #<FACE 451> */
  gsSP2Triangles(1,0,5,0,13,2,5,0), /* #<FACE 2940> and #<FACE 2932> */
  gsSP2Triangles(3,13,5,0,4,14,5,0), /* #<FACE 2931> and #<FACE 2549> */
  gsSP1Triangle(6,5,14,0), /* #<FACE 1120> */
  gsSPVertex(&cont_finish_v[28],15,0),
  gsSP2Triangles(3,2,5,0,4,3,5,0), /* #<FACE 676> and #<FACE 675> */
  gsSP2Triangles(1,3,6,0,0,6,3,0), /* #<FACE 588> and #<FACE 583> */
  gsSP2Triangles(10,12,13,0,7,12,15,0), /* #<FACE 2942> and #<FACE 2938> */
  gsSP2Triangles(8,12,7,0,14,12,8,0), /* #<FACE 2937> and #<FACE 2936> */
  gsSP2Triangles(12,9,11,0,10,9,12,0), /* #<FACE 1886> and #<FACE 1103> */
  gsSP2Triangles(15,12,11,0,14,13,12,0), /* #<FACE 452> and #<FACE 448> */
  gsSPVertex(&cont_finish_v[43],11,5),
  gsSP2Triangles(10,13,6,0,11,5,13,0), /* #<FACE 2434> and #<FACE 2422> */
  gsSP2Triangles(5,7,13,0,9,6,13,0), /* #<FACE 1247> and #<FACE 1235> */
  gsSP2Triangles(13,7,8,0,9,13,8,0), /* #<FACE 651> and #<FACE 650> */
  gsSP2Triangles(10,12,13,0,13,12,11,0), /* #<FACE 589> and #<FACE 585> */
  gsSP2Triangles(0,3,15,0,1,14,3,0), /* #<FACE 2432> and #<FACE 2416> */
  gsSP2Triangles(14,2,3,0,4,15,3,0), /* #<FACE 1253> and #<FACE 1237> */
  gsSPVertex(&cont_finish_v[54],16,0),
  gsSP2Triangles(4,0,6,0,2,4,6,0), /* #<FACE 2554> and #<FACE 1110> */
  gsSP2Triangles(0,4,3,0,5,4,1,0), /* #<FACE 453> and #<FACE 449> */
  gsSP2Triangles(12,15,8,0,13,7,15,0), /* #<FACE 2436> and #<FACE 2418> */
  gsSP2Triangles(7,9,15,0,11,8,15,0), /* #<FACE 1251> and #<FACE 1233> */
  gsSP2Triangles(15,9,10,0,11,15,10,0), /* #<FACE 626> and #<FACE 625> */
  gsSP2Triangles(12,14,15,0,15,14,13,0), /* #<FACE 586> and #<FACE 584> */
  gsSPVertex(&cont_finish_v[70],8,8),
  gsSP2Triangles(9,8,11,0,14,11,13,0), /* #<FACE 2927> and #<FACE 458> */
  gsSP2Triangles(10,15,5,0,11,14,15,0), /* #<FACE 2933> and #<FACE 126> */
  gsSP2Triangles(0,3,12,0,14,13,3,0), /* #<FACE 2930> and #<FACE 457> */
  gsSP2Triangles(14,3,15,0,2,1,4,0), /* #<FACE 456> and #<FACE 2941> */
  gsSP2Triangles(15,3,4,0,5,15,4,0), /* #<FACE 2935> and #<FACE 2934> */
  gsSPVertex(&cont_finish_v[78],16,0),
  gsSP2Triangles(3,4,5,0,6,3,5,0), /* #<FACE 669> and #<FACE 668> */
  gsSP2Triangles(6,2,3,0,10,9,1,0), /* #<FACE 421> and #<FACE 2970> */
  gsSP2Triangles(9,7,8,0,0,9,8,0), /* #<FACE 694> and #<FACE 693> */
  gsSP2Triangles(0,1,9,0,10,13,14,0), /* #<FACE 419> and #<FACE 2971> */
  gsSP2Triangles(14,15,11,0,12,14,11,0), /* #<FACE 702> and #<FACE 701> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 418> */
  gsSPVertex(&cont_finish_v[94],12,4),
  gsSP2Triangles(15,1,8,0,4,8,7,0), /* #<FACE 2969> and #<FACE 679> */
  gsSP2Triangles(0,5,9,0,5,6,9,0), /* #<FACE 2414> and #<FACE 1255> */
  gsSP2Triangles(9,6,7,0,8,9,7,0), /* #<FACE 681> and #<FACE 680> */
  gsSP2Triangles(9,1,0,0,9,8,1,0), /* #<FACE 441> and #<FACE 420> */
  gsSP2Triangles(12,10,14,0,11,12,13,0), /* #<FACE 2558> and #<FACE 2556> */
  gsSP2Triangles(14,13,12,0,15,3,2,0), /* #<FACE 1113> and #<FACE 2968> */
  gsSPVertex(&cont_finish_v[106],16,0),
  gsSP2Triangles(2,0,1,0,2,3,0,0), /* #<FACE 443> and #<FACE 424> */
  gsSP2Triangles(7,6,0,0,6,4,5,0), /* #<FACE 2966> and #<FACE 644> */
  gsSP2Triangles(1,6,5,0,1,0,6,0), /* #<FACE 643> and #<FACE 423> */
  gsSP2Triangles(7,13,14,0,8,14,11,0), /* #<FACE 2967> and #<FACE 654> */
  gsSP2Triangles(12,9,15,0,9,10,15,0), /* #<FACE 2424> and #<FACE 1245> */
  gsSP2Triangles(15,10,11,0,14,15,11,0), /* #<FACE 656> and #<FACE 655> */
  gsSP2Triangles(15,13,12,0,15,14,13,0), /* #<FACE 444> and #<FACE 422> */
  gsSPVertex(&cont_finish_v[122],10,6),
  gsSP2Triangles(6,8,9,0,8,7,10,0), /* #<FACE 1880> and #<FACE 1874> */
  gsSP2Triangles(10,9,8,0,15,0,3,0), /* #<FACE 1789> and #<FACE 2965> */
  gsSP2Triangles(11,3,14,0,1,12,2,0), /* #<FACE 629> and #<FACE 2420> */
  gsSP2Triangles(12,13,2,0,2,13,14,0), /* #<FACE 1249> and #<FACE 631> */
  gsSP1Triangle(3,2,14,0), /* #<FACE 630> */
  gsSPVertex(&cont_finish_v[132],14,0),
  gsSP2Triangles(3,1,0,0,3,8,1,0), /* #<FACE 703> and #<FACE 2428> */
  gsSP2Triangles(2,1,8,0,2,8,5,0), /* #<FACE 1241> and #<FACE 617> */
  gsSP2Triangles(3,7,8,0,15,6,7,0), /* #<FACE 442> and #<FACE 2972> */
  gsSP2Triangles(6,4,5,0,8,6,5,0), /* #<FACE 619> and #<FACE 618> */
  gsSP2Triangles(8,7,6,0,9,11,12,0), /* #<FACE 417> and #<FACE 1883> */
  gsSP2Triangles(12,13,10,0,13,12,11,0), /* #<FACE 1881> and #<FACE 1786> */
  gsSPVertex(&cont_finish_v[146],16,0),
  gsSP2Triangles(14,1,0,0,13,1,14,0), /* #<FACE 1861> and #<FACE 1801> */
  gsSP2Triangles(4,3,2,0,7,6,5,0), /* #<FACE 642> and #<FACE 667> */
  gsSP2Triangles(9,15,8,0,12,11,10,0), /* #<FACE 1115> and #<FACE 692> */
  gsSPVertex(&cont_finish_v[162],11,0),
  gsSP2Triangles(2,1,0,0,4,3,15,0), /* #<FACE 1788> and #<FACE 2559> */
  gsSP2Triangles(6,9,5,0,9,8,7,0), /* #<FACE 2566> and #<FACE 1783> */
  gsSP1Triangle(10,13,14,0), /* #<FACE 1868> */
  gsSPVertex(&cont_finish_v[173],15,1),
  gsSP2Triangles(3,2,1,0,6,5,4,0), /* #<FACE 2564> and #<FACE 2561> */
  gsSP2Triangles(9,8,7,0,12,11,10,0), /* #<FACE 1795> and #<FACE 1808> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 1111> */
  gsDPPipeSync(),
  gsDPSetCycleType(G_CYC_2CYCLE),
  gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
  gsSPClearGeometryMode((G_SHADE|G_SHADING_SMOOTH|G_LIGHTING|G_TEXTURE_GEN|G_TEXTURE_GEN_LINEAR|G_CULL_BOTH|G_FOG)),
  gsSPFogPosition(990, 1000),
  gsSPSetGeometryMode( G_ZBUFFER | G_CULL_BACK | G_SHADE | G_SHADING_SMOOTH | G_LIGHTING | G_FOG),
/*  gsSPSetLights1(NWorldLight),*/



  gsDPSetFogColor(0, 0, 0, 255),

/*  gsDPSetColorDither(G_CD_BAYER),*/
  gsDPSetColorDither(G_CD_DISABLE),
  gsDPSetCombineMode(G_CC_MULPRIMSHADE,     G_CC_MULPRIMSHADE), /* N64-SHADE-TYPE-G */
  gsSPVertex(&cont_finish_v[188],16,0),
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP2Triangles(2,3,4,0,0,1,4,0), /* #<FACE 708> and #<FACE 707> */
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(14,10,9,0,5,13,14,0), /* #<FACE 2973> and #<FACE 2949> */
  gsSP2Triangles(6,5,14,0,7,6,14,0), /* #<FACE 2948> and #<FACE 2947> */
  gsSP2Triangles(8,7,14,0,9,8,14,0), /* #<FACE 2946> and #<FACE 2945> */
  gsSP2Triangles(14,12,10,0,14,15,11,0), /* #<FACE 2441> and #<FACE 689> */
  gsSP2Triangles(12,14,11,0,15,14,13,0), /* #<FACE 688> and #<FACE 440> */
  gsSPVertex(&cont_finish_v[204],11,5),
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP2Triangles(13,11,9,0,15,14,9,0), /* #<FACE 712> and #<FACE 711> */
  gsSP2Triangles(0,4,8,0,12,4,7,0), /* #<FACE 2348> and #<FACE 2347> */
  gsSP2Triangles(1,6,4,0,10,5,4,0), /* #<FACE 2346> and #<FACE 2345> */
  gsSP2Triangles(5,2,4,0,10,4,6,0), /* #<FACE 1324> and #<FACE 1323> */
  gsSP2Triangles(3,7,4,0,4,12,8,0), /* #<FACE 1322> and #<FACE 1321> */
  gsSPVertex(&cont_finish_v[215],9,0),
  gsSP2Triangles(1,2,4,0,0,4,3,0), /* #<FACE 385> and #<FACE 384> */
  gsSP2Triangles(15,9,8,0,10,9,7,0), /* #<FACE 2344> and #<FACE 2343> */
  gsSP2Triangles(14,6,9,0,12,5,9,0), /* #<FACE 2342> and #<FACE 2341> */
  gsSP2Triangles(5,13,9,0,12,9,6,0), /* #<FACE 1328> and #<FACE 1327> */
  gsSP2Triangles(11,7,9,0,9,10,8,0), /* #<FACE 1326> and #<FACE 1325> */
  gsSPVertex(&cont_finish_v[224],11,5),
  gsSP2Triangles(10,9,13,0,14,12,9,0), /* #<FACE 1290> and #<FACE 389> */
  gsSP2Triangles(15,9,11,0,5,4,0,0), /* #<FACE 388> and #<FACE 2992> */
  gsSP2Triangles(5,1,4,0,2,7,4,0), /* #<FACE 2991> and #<FACE 2371> */
  gsSP2Triangles(3,4,6,0,6,4,8,0), /* #<FACE 2369> and #<FACE 1300> */
  gsSP1Triangle(8,4,7,0), /* #<FACE 1298> */
  gsSPVertex(&cont_finish_v[235],9,0),
  gsSP2Triangles(0,12,6,0,0,6,4,0), /* #<FACE 2994> and #<FACE 2993> */
  gsSP2Triangles(5,6,2,0,14,1,6,0), /* #<FACE 2384> and #<FACE 2382> */
  gsSP2Triangles(1,3,6,0,3,2,6,0), /* #<FACE 1287> and #<FACE 1285> */
  gsSP2Triangles(5,4,6,0,14,6,12,0), /* #<FACE 383> and #<FACE 382> */
  gsSP2Triangles(7,9,15,0,7,14,9,0), /* #<FACE 2988> and #<FACE 2987> */
  gsSP2Triangles(12,13,9,0,11,9,8,0), /* #<FACE 2379> and #<FACE 2377> */
  gsSP1Triangle(8,9,10,0), /* #<FACE 1292> */
  gsSPVertex(&cont_finish_v[244],16,0),
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP2Triangles(11,0,12,0,13,12,0,0), /* #<FACE 1119> and #<FACE 1118> */
  gsSP2Triangles(1,14,12,0,1,12,15,0), /* #<FACE 535> and #<FACE 534> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP2Triangles(2,8,9,0,2,9,6,0), /* #<FACE 2990> and #<FACE 2989> */
  gsSP2Triangles(7,9,4,0,10,3,9,0), /* #<FACE 2392> and #<FACE 2390> */
  gsSP2Triangles(3,5,9,0,5,4,9,0), /* #<FACE 1279> and #<FACE 1277> */
  gsSP2Triangles(7,6,9,0,10,9,8,0), /* #<FACE 387> and #<FACE 386> */
  gsSPVertex(&cont_finish_v[260],11,0),
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP2Triangles(2,6,7,0,0,5,6,0), /* #<FACE 2895> and #<FACE 2888> */
  gsSP2Triangles(1,0,6,0,3,1,6,0), /* #<FACE 2887> and #<FACE 2886> */
  gsSP2Triangles(6,2,4,0,6,4,3,0), /* #<FACE 2560> and #<FACE 2553> */
  gsSP2Triangles(8,6,5,0,8,7,6,0), /* #<FACE 512> and #<FACE 505> */
  gsSP2Triangles(9,15,12,0,10,12,14,0), /* #<FACE 2864> and #<FACE 2863> */
  gsSP2Triangles(9,12,13,0,10,11,12,0), /* #<FACE 2551> and #<FACE 2550> */
  gsSPVertex(&cont_finish_v[271],16,0),
  gsSP2Triangles(9,13,10,0,12,14,10,0), /* #<FACE 1047> and #<FACE 531> */
  gsSP2Triangles(11,10,15,0,0,6,7,0), /* #<FACE 530> and #<FACE 2877> */
  gsSP2Triangles(0,4,6,0,5,3,6,0), /* #<FACE 2870> and #<FACE 2616> */
  gsSP2Triangles(8,6,1,0,2,1,6,0), /* #<FACE 2606> and #<FACE 1063> */
  gsSP2Triangles(3,2,6,0,5,6,4,0), /* #<FACE 1053> and #<FACE 528> */
  gsSP1Triangle(8,7,6,0), /* #<FACE 521> */
  gsSPVertex(&cont_finish_v[287],9,0),
  gsSP2Triangles(2,1,3,0,2,3,4,0), /* #<FACE 561> and #<FACE 560> */
  gsSP2Triangles(2,4,5,0,2,5,6,0), /* #<FACE 559> and #<FACE 558> */
  gsSP2Triangles(2,6,0,0,7,15,10,0), /* #<FACE 447> and #<FACE 2868> */
  gsSP2Triangles(7,10,14,0,12,10,13,0), /* #<FACE 2867> and #<FACE 2622> */
  gsSP2Triangles(11,8,10,0,8,9,10,0), /* #<FACE 2620> and #<FACE 1049> */
  gsSPVertex(&cont_finish_v[296],11,5),
  gsSP2Triangles(13,11,5,0,13,5,6,0), /* #<FACE 557> and #<FACE 556> */
  gsSP2Triangles(13,6,7,0,13,7,8,0), /* #<FACE 555> and #<FACE 554> */
  gsSP2Triangles(13,8,9,0,13,9,10,0), /* #<FACE 553> and #<FACE 552> */
  gsSP2Triangles(13,10,12,0,13,12,11,0), /* #<FACE 551> and #<FACE 132> */
  gsSP2Triangles(2,0,14,0,2,14,15,0), /* #<FACE 564> and #<FACE 563> */
  gsSP1Triangle(2,15,1,0), /* #<FACE 562> */
  gsSPVertex(&cont_finish_v[307],16,0),
  gsSP2Triangles(4,3,5,0,4,5,6,0), /* #<FACE 540> and #<FACE 539> */
  gsSP2Triangles(4,6,0,0,4,0,1,0), /* #<FACE 538> and #<FACE 537> */
  gsSP2Triangles(4,1,2,0,15,13,7,0), /* #<FACE 446> and #<FACE 550> */
  gsSP2Triangles(15,7,8,0,15,8,9,0), /* #<FACE 549> and #<FACE 548> */
  gsSP2Triangles(15,9,10,0,15,10,11,0), /* #<FACE 547> and #<FACE 546> */
  gsSP2Triangles(15,11,12,0,15,12,14,0), /* #<FACE 545> and #<FACE 544> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 127> */
  gsSPVertex(&cont_finish_v[323],11,5),
  gsDPSetPrimColor(0,0,0,0,153,255),
  gsSP1Triangle(13,14,15,0), /* #<FACE 461> */
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP2Triangles(5,10,8,0,9,10,0,0), /* #<FACE 2596> and #<FACE 2595> */
  gsSP2Triangles(5,6,10,0,7,2,10,0), /* #<FACE 2594> and #<FACE 2593> */
  gsSP2Triangles(2,1,10,0,7,10,6,0), /* #<FACE 1076> and #<FACE 1075> */
  gsSP2Triangles(1,0,10,0,10,9,8,0), /* #<FACE 1074> and #<FACE 1073> */
  gsSP2Triangles(4,11,12,0,4,2,11,0), /* #<FACE 2860> and #<FACE 543> */
  gsSP1Triangle(4,12,3,0), /* #<FACE 541> */
  gsSPVertex(&cont_finish_v[334],13,0),
  gsDPSetPrimColor(0,0,0,153,0,255),
  gsSP2Triangles(5,4,6,0,5,6,7,0), /* #<FACE 487> and #<FACE 486> */
  gsSP2Triangles(5,7,8,0,5,8,9,0), /* #<FACE 485> and #<FACE 484> */
  gsSP1Triangle(5,9,3,0), /* #<FACE 460> */
  gsDPSetPrimColor(0,0,0,0,153,255),
  gsSP2Triangles(13,15,1,0,13,1,2,0), /* #<FACE 497> and #<FACE 496> */
  gsSP2Triangles(13,2,0,0,13,0,10,0), /* #<FACE 495> and #<FACE 494> */
  gsSP2Triangles(13,10,11,0,13,11,12,0), /* #<FACE 493> and #<FACE 492> */
  gsSP1Triangle(13,12,14,0), /* #<FACE 491> */
  gsSPVertex(&cont_finish_v[347],8,8),
  gsSP2Triangles(8,13,11,0,12,13,1,0), /* #<FACE 2548> and #<FACE 2547> */
  gsSP2Triangles(8,9,13,0,10,0,13,0), /* #<FACE 2546> and #<FACE 2545> */
  gsSP2Triangles(0,2,13,0,10,13,9,0), /* #<FACE 1124> and #<FACE 1123> */
  gsSP2Triangles(2,1,13,0,13,12,11,0), /* #<FACE 1122> and #<FACE 1121> */
  gsDPSetPrimColor(0,0,0,153,0,255),
  gsSP2Triangles(5,3,14,0,5,14,15,0), /* #<FACE 490> and #<FACE 489> */
  gsSP1Triangle(5,15,4,0), /* #<FACE 488> */
  gsSPVertex(&cont_finish_v[355],16,0),
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP2Triangles(2,6,5,0,3,5,1,0), /* #<FACE 1805> and #<FACE 520> */
  gsSP1Triangle(4,0,5,0), /* #<FACE 513> */
  gsDPSetPrimColor(0,0,0,153,0,255),
  gsSP2Triangles(7,15,13,0,14,15,11,0), /* #<FACE 2528> and #<FACE 2527> */
  gsSP2Triangles(7,9,15,0,10,8,15,0), /* #<FACE 2526> and #<FACE 2525> */
  gsSP2Triangles(8,12,15,0,10,15,9,0), /* #<FACE 1144> and #<FACE 1143> */
  gsSP2Triangles(12,11,15,0,15,14,13,0), /* #<FACE 1142> and #<FACE 1141> */
  gsSPVertex(&cont_finish_v[371],9,7),
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP2Triangles(7,11,12,0,1,12,3,0), /* #<FACE 2880> and #<FACE 2879> */
  gsSP2Triangles(7,12,10,0,1,8,12,0), /* #<FACE 1879> and #<FACE 1867> */
  gsSP2Triangles(9,12,8,0,10,12,9,0), /* #<FACE 1802> and #<FACE 1790> */
  gsSP2Triangles(13,3,12,0,13,12,11,0), /* #<FACE 519> and #<FACE 518> */
  gsSP2Triangles(14,5,0,0,14,1,5,0), /* #<FACE 2885> and #<FACE 2878> */
  gsSP2Triangles(3,2,5,0,4,5,15,0), /* #<FACE 1864> and #<FACE 1858> */
  gsSP1Triangle(6,15,5,0), /* #<FACE 1811> */
  gsSPVertex(&cont_finish_v[380],15,0),
  gsDPSetPrimColor(0,0,255,0,0,255),
  gsSP2Triangles(3,8,6,0,7,8,9,0), /* #<FACE 2412> and #<FACE 2411> */
  gsSP2Triangles(3,4,8,0,5,11,8,0), /* #<FACE 2410> and #<FACE 2409> */
  gsSP2Triangles(11,10,8,0,5,8,4,0), /* #<FACE 1260> and #<FACE 1259> */
  gsSP2Triangles(10,9,8,0,8,7,6,0), /* #<FACE 1258> and #<FACE 1257> */
  gsSP2Triangles(14,13,9,0,14,9,10,0), /* #<FACE 411> and #<FACE 410> */
  gsSP2Triangles(14,10,11,0,14,11,12,0), /* #<FACE 409> and #<FACE 408> */
  gsSP2Triangles(14,12,1,0,14,1,2,0), /* #<FACE 407> and #<FACE 406> */
  gsSP2Triangles(14,2,0,0,14,0,13,0), /* #<FACE 405> and #<FACE 179> */
  gsSPVertex(&cont_finish_v[395],13,3),
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP2Triangles(13,14,3,0,13,3,4,0), /* #<FACE 393> and #<FACE 392> */
  gsSP2Triangles(13,4,6,0,13,5,15,0), /* #<FACE 391> and #<FACE 390> */
  gsSP1Triangle(6,5,13,0), /* #<FACE 186> */
  gsDPSetPrimColor(0,0,255,0,0,255),
  gsSP2Triangles(7,12,10,0,11,12,1,0), /* #<FACE 2404> and #<FACE 2403> */
  gsSP2Triangles(7,8,12,0,9,0,12,0), /* #<FACE 2402> and #<FACE 2401> */
  gsSP2Triangles(0,2,12,0,9,12,8,0), /* #<FACE 1268> and #<FACE 1267> */
  gsSP2Triangles(2,1,12,0,12,11,10,0), /* #<FACE 1266> and #<FACE 1265> */
  gsSPVertex(&cont_finish_v[408],11,0),
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(0,6,7,0,0,7,4,0), /* #<FACE 3251> and #<FACE 3250> */
  gsSP2Triangles(5,7,2,0,8,1,7,0), /* #<FACE 2096> and #<FACE 2094> */
  gsSP2Triangles(1,3,7,0,3,2,7,0), /* #<FACE 1575> and #<FACE 1573> */
  gsSP2Triangles(5,4,7,0,8,7,6,0), /* #<FACE 135> and #<FACE 134> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP2Triangles(13,15,9,0,13,9,10,0), /* #<FACE 396> and #<FACE 395> */
  gsSP1Triangle(13,10,14,0), /* #<FACE 394> */
  gsSPVertex(&cont_finish_v[419],16,0),
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(13,10,9,0,10,12,14,0), /* #<FACE 1486> and #<FACE 259> */
  gsSP2Triangles(11,10,15,0,0,6,7,0), /* #<FACE 258> and #<FACE 3248> */
  gsSP2Triangles(0,4,6,0,5,3,6,0), /* #<FACE 3209> and #<FACE 2144> */
  gsSP2Triangles(8,6,1,0,1,6,2,0), /* #<FACE 2121> and #<FACE 1548> */
  gsSP2Triangles(3,2,6,0,5,6,4,0), /* #<FACE 1525> and #<FACE 175> */
  gsSP1Triangle(8,7,6,0), /* #<FACE 137> */
  gsSPVertex(&cont_finish_v[435],9,0),
  gsDPSetPrimColor(0,0,255,0,0,255),
  gsSP2Triangles(2,5,6,0,1,3,6,0), /* #<FACE 1483> and #<FACE 402> */
  gsSP1Triangle(0,6,4,0), /* #<FACE 401> */
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(7,15,10,0,7,10,14,0), /* #<FACE 3126> and #<FACE 3125> */
  gsSP2Triangles(12,10,13,0,11,8,10,0), /* #<FACE 2183> and #<FACE 2181> */
  gsSP1Triangle(9,10,8,0), /* #<FACE 1488> */
  gsSPVertex(&cont_finish_v[444],9,7),
  gsSP2Triangles(10,9,12,0,11,9,14,0), /* #<FACE 1349> and #<FACE 741> */
  gsSP1Triangle(9,15,13,0), /* #<FACE 737> */
  gsDPSetPrimColor(0,0,255,0,0,255),
  gsSP2Triangles(7,6,0,0,7,1,6,0), /* #<FACE 2982> and #<FACE 2981> */
  gsSP2Triangles(3,2,6,0,4,6,8,0), /* #<FACE 2186> and #<FACE 2091> */
  gsSP1Triangle(8,6,5,0), /* #<FACE 1578> */
  gsSPVertex(&cont_finish_v[453],9,0),
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(3,5,6,0,0,5,4,0), /* #<FACE 2762> and #<FACE 2761> */
  gsSP2Triangles(5,0,1,0,1,2,5,0), /* #<FACE 2326> and #<FACE 2325> */
  gsSP2Triangles(2,7,5,0,4,5,3,0), /* #<FACE 1344> and #<FACE 736> */
  gsSP2Triangles(7,6,5,0,8,15,9,0), /* #<FACE 733> and #<FACE 2760> */
  gsSP2Triangles(13,14,9,0,11,12,9,0), /* #<FACE 2759> and #<FACE 2320> */
  gsSP1Triangle(9,10,8,0), /* #<FACE 2319> */
  gsSPVertex(&cont_finish_v[462],14,2),
  gsSP2Triangles(6,3,2,0,2,5,6,0), /* #<FACE 2772> and #<FACE 2770> */
  gsSP2Triangles(7,15,6,0,3,6,4,0), /* #<FACE 2336> and #<FACE 2335> */
  gsSP2Triangles(13,4,6,0,13,6,15,0), /* #<FACE 1334> and #<FACE 1333> */
  gsSP2Triangles(7,6,5,0,11,13,14,0), /* #<FACE 720> and #<FACE 2769> */
  gsSP2Triangles(8,10,14,0,8,14,9,0), /* #<FACE 2332> and #<FACE 2331> */
  gsSP2Triangles(15,9,14,0,12,14,10,0), /* #<FACE 1338> and #<FACE 1337> */
  gsSP2Triangles(14,12,11,0,15,14,13,0), /* #<FACE 722> and #<FACE 721> */
  gsSPVertex(&cont_finish_v[476],16,0),
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP2Triangles(6,5,2,0,5,6,4,0), /* #<FACE 371> and #<FACE 370> */
  gsSP2Triangles(7,13,9,0,9,4,7,0), /* #<FACE 3008> and #<FACE 3007> */
  gsSP2Triangles(8,3,9,0,8,9,12,0), /* #<FACE 2358> and #<FACE 2356> */
  gsSP2Triangles(11,12,9,0,9,3,4,0), /* #<FACE 1313> and #<FACE 1311> */
  gsSP2Triangles(11,9,13,0,11,13,15,0), /* #<FACE 368> and #<FACE 3009> */
  gsSP2Triangles(10,12,15,0,10,15,1,0), /* #<FACE 2354> and #<FACE 2352> */
  gsSP2Triangles(0,1,15,0,15,12,11,0), /* #<FACE 1317> and #<FACE 1315> */
  gsSP2Triangles(14,15,13,0,15,14,0,0), /* #<FACE 367> and #<FACE 366> */
  gsSPVertex(&cont_finish_v[492],9,7),
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP1Triangle(13,14,15,0), /* #<FACE 425> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP2Triangles(7,2,9,0,9,0,7,0), /* #<FACE 3004> and #<FACE 3003> */
  gsSP2Triangles(8,9,12,0,8,1,9,0), /* #<FACE 2364> and #<FACE 2350> */
  gsSP2Triangles(9,1,0,0,11,12,9,0), /* #<FACE 1319> and #<FACE 1305> */
  gsSP2Triangles(11,9,2,0,11,2,5,0), /* #<FACE 372> and #<FACE 3005> */
  gsSP2Triangles(10,12,5,0,10,5,3,0), /* #<FACE 2362> and #<FACE 2360> */
  gsSP2Triangles(4,3,5,0,5,12,11,0), /* #<FACE 1309> and #<FACE 1307> */
  gsSPVertex(&cont_finish_v[501],13,0),
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(3,4,0,0,1,3,7,0), /* #<FACE 648> and #<FACE 2963> */
  gsSP2Triangles(7,2,5,0,7,5,6,0), /* #<FACE 2457> and #<FACE 665> */
  gsSP2Triangles(8,7,6,0,3,2,7,0), /* #<FACE 664> and #<FACE 426> */
  gsSP2Triangles(8,13,15,0,9,13,11,0), /* #<FACE 2964> and #<FACE 2788> */
  gsSP2Triangles(8,10,13,0,13,9,14,0), /* #<FACE 2453> and #<FACE 2447> */
  gsSP2Triangles(12,13,10,0,13,12,11,0), /* #<FACE 1216> and #<FACE 673> */
  gsSPVertex(&cont_finish_v[514],10,6),
  gsSP2Triangles(6,14,11,0,7,11,9,0), /* #<FACE 2962> and #<FACE 2809> */
  gsSP2Triangles(6,11,8,0,8,11,7,0), /* #<FACE 2469> and #<FACE 1200> */
  gsSP2Triangles(11,15,10,0,11,10,9,0), /* #<FACE 1198> and #<FACE 635> */
  gsSP2Triangles(15,11,14,0,12,3,0,0), /* #<FACE 427> and #<FACE 2802> */
  gsSP2Triangles(1,13,3,0,2,3,12,0), /* #<FACE 2465> and #<FACE 1210> */
  gsSP1Triangle(4,3,13,0), /* #<FACE 1204> */
  gsSPVertex(&cont_finish_v[524],14,0),
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP2Triangles(1,3,4,0,2,4,0,0), /* #<FACE 1804> and #<FACE 1803> */
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(9,7,5,0,5,8,9,0), /* #<FACE 2828> and #<FACE 2483> */
  gsSP2Triangles(7,9,6,0,10,9,8,0), /* #<FACE 1228> and #<FACE 1186> */
  gsSP2Triangles(10,11,9,0,14,13,15,0), /* #<FACE 416> and #<FACE 2477> */
  gsSP2Triangles(14,11,12,0,13,14,12,0), /* #<FACE 614> and #<FACE 613> */
  gsSPVertex(&cont_finish_v[538],10,6),
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP2Triangles(6,11,12,0,6,12,9,0), /* #<FACE 1876> and #<FACE 1875> */
  gsSP2Triangles(8,7,12,0,7,10,12,0), /* #<FACE 1869> and #<FACE 1800> */
  gsSP2Triangles(8,12,13,0,10,9,12,0), /* #<FACE 1799> and #<FACE 1794> */
  gsSP2Triangles(13,12,11,0,14,0,4,0), /* #<FACE 1793> and #<FACE 1866> */
  gsSP2Triangles(2,1,4,0,4,3,15,0), /* #<FACE 1865> and #<FACE 1863> */
  gsSPVertex(&cont_finish_v[548],15,0),
  gsSP2Triangles(4,5,6,0,5,1,7,0), /* #<FACE 2869> and #<FACE 2862> */
  gsSP2Triangles(0,5,3,0,0,2,5,0), /* #<FACE 2572> and #<FACE 2562> */
  gsSP2Triangles(5,2,1,0,4,3,5,0), /* #<FACE 1107> and #<FACE 1097> */
  gsSP2Triangles(7,6,5,0,8,12,13,0), /* #<FACE 529> and #<FACE 2889> */
  gsSP2Triangles(8,13,11,0,10,13,9,0), /* #<FACE 2555> and #<FACE 1116> */
  gsSP2Triangles(11,13,10,0,14,13,12,0), /* #<FACE 1114> and #<FACE 511> */
  gsSPVertex(&cont_finish_v[563],16,0),
  gsSP2Triangles(14,11,12,0,13,11,15,0), /* #<FACE 1096> and #<FACE 532> */
  gsSP2Triangles(6,7,2,0,0,1,7,0), /* #<FACE 2891> and #<FACE 2600> */
  gsSP2Triangles(0,7,5,0,7,1,3,0), /* #<FACE 2598> and #<FACE 1069> */
  gsSP2Triangles(3,2,7,0,4,8,9,0), /* #<FACE 509> and #<FACE 2893> */
  gsSP2Triangles(4,9,6,0,9,5,7,0), /* #<FACE 2892> and #<FACE 1071> */
  gsSP2Triangles(7,6,9,0,10,9,8,0), /* #<FACE 508> and #<FACE 507> */
  gsSPVertex(&cont_finish_v[579],11,0),
  gsSP2Triangles(4,5,6,0,6,7,0,0), /* #<FACE 2874> and #<FACE 2873> */
  gsSP2Triangles(2,3,6,0,0,1,6,0), /* #<FACE 2579> and #<FACE 2565> */
  gsSP2Triangles(2,6,1,0,4,6,3,0), /* #<FACE 1104> and #<FACE 1090> */
  gsSP2Triangles(7,6,5,0,14,15,11,0), /* #<FACE 524> and #<FACE 2866> */
  gsSP2Triangles(11,13,10,0,8,12,11,0), /* #<FACE 2865> and #<FACE 2573> */
  gsSP2Triangles(8,11,9,0,11,10,9,0), /* #<FACE 2563> and #<FACE 1106> */
  gsSPVertex(&cont_finish_v[590],14,2),
  gsSP2Triangles(8,2,5,0,3,7,8,0), /* #<FACE 2890> and #<FACE 2584> */
  gsSP2Triangles(3,8,4,0,8,5,4,0), /* #<FACE 2557> and #<FACE 1112> */
  gsSP2Triangles(8,7,6,0,9,14,15,0), /* #<FACE 1085> and #<FACE 2861> */
  gsSP2Triangles(12,10,15,0,11,15,10,0), /* #<FACE 2603> and #<FACE 1066> */
  gsSP2Triangles(15,13,12,0,15,14,13,0), /* #<FACE 592> and #<FACE 542> */
  gsSPVertex(&cont_finish_v[604],15,0),
  gsSP2Triangles(6,7,3,0,4,7,1,0), /* #<FACE 2875> and #<FACE 2610> */
  gsSP2Triangles(5,0,7,0,0,2,7,0), /* #<FACE 2608> and #<FACE 1061> */
  gsSP2Triangles(2,1,7,0,4,3,7,0), /* #<FACE 1059> and #<FACE 523> */
  gsSP2Triangles(7,6,5,0,11,12,13,0), /* #<FACE 522> and #<FACE 2894> */
  gsSP2Triangles(9,12,10,0,9,8,12,0), /* #<FACE 2588> and #<FACE 1109> */
  gsSP2Triangles(11,10,12,0,14,13,12,0), /* #<FACE 1081> and #<FACE 506> */
  gsSPVertex(&cont_finish_v[619],16,0),
  gsSP2Triangles(4,5,0,0,11,10,5,0), /* #<FACE 2625> and #<FACE 2623> */
  gsSP2Triangles(10,1,5,0,11,5,12,0), /* #<FACE 1046> and #<FACE 1045> */
  gsSP2Triangles(1,0,5,0,15,13,3,0), /* #<FACE 1044> and #<FACE 2632> */
  gsSP2Triangles(15,3,7,0,6,7,3,0), /* #<FACE 2630> and #<FACE 1039> */
  gsSP2Triangles(3,13,14,0,3,14,2,0), /* #<FACE 1037> and #<FACE 536> */
  gsSP2Triangles(4,7,9,0,4,9,5,0), /* #<FACE 2628> and #<FACE 2626> */
  gsSP2Triangles(12,5,9,0,9,7,6,0), /* #<FACE 1043> and #<FACE 1041> */
  gsSP1Triangle(9,8,12,0), /* #<FACE 533> */
  gsSPVertex(&cont_finish_v[635],10,0),
  gsSP2Triangles(2,1,0,0,15,6,13,0), /* #<FACE 1057> and #<FACE 2631> */
  gsSP2Triangles(4,13,3,0,5,3,13,0), /* #<FACE 2617> and #<FACE 1052> */
  gsSP2Triangles(13,4,14,0,6,5,13,0), /* #<FACE 1051> and #<FACE 1038> */
  gsSP2Triangles(7,11,12,0,8,9,11,0), /* #<FACE 2624> and #<FACE 2621> */
  gsSP1Triangle(9,10,11,0), /* #<FACE 1048> */
  gsSPVertex(&cont_finish_v[645],13,3),
  gsSP2Triangles(14,3,15,0,3,5,15,0), /* #<FACE 1070> and #<FACE 1068> */
  gsSP2Triangles(4,7,8,0,9,6,8,0), /* #<FACE 2604> and #<FACE 2602> */
  gsSP2Triangles(8,15,5,0,15,8,6,0), /* #<FACE 2601> and #<FACE 1067> */
  gsSP2Triangles(9,8,7,0,10,2,0,0), /* #<FACE 1065> and #<FACE 2612> */
  gsSP2Triangles(2,12,1,0,11,13,2,0), /* #<FACE 2611> and #<FACE 2609> */
  gsSP1Triangle(13,12,2,0), /* #<FACE 1060> */
  gsSPVertex(&cont_finish_v[658],14,0),
  gsDPSetPrimColor(0,0,0,153,0,255),
  gsSP2Triangles(2,1,4,0,3,4,0,0), /* #<FACE 1138> and #<FACE 1137> */
  gsDPSetPrimColor(0,0,0,0,153,255),
  gsSP2Triangles(11,12,8,0,5,12,7,0), /* #<FACE 2543> and #<FACE 2542> */
  gsSP2Triangles(5,6,12,0,6,9,12,0), /* #<FACE 2541> and #<FACE 1128> */
  gsSP2Triangles(7,12,10,0,9,8,12,0), /* #<FACE 1127> and #<FACE 1126> */
  gsSP1Triangle(12,11,10,0), /* #<FACE 1125> */
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP1Triangle(13,14,15,0), /* #<FACE 2599> */
  gsSPVertex(&cont_finish_v[672],11,5),
  gsDPSetPrimColor(0,0,0,153,0,255),
  gsSP2Triangles(11,12,8,0,5,12,7,0), /* #<FACE 2523> and #<FACE 2522> */
  gsSP2Triangles(5,6,12,0,6,9,12,0), /* #<FACE 2521> and #<FACE 1148> */
  gsSP2Triangles(7,12,10,0,9,8,12,0), /* #<FACE 1147> and #<FACE 1146> */
  gsSP2Triangles(12,11,10,0,13,0,4,0), /* #<FACE 1145> and #<FACE 2532> */
  gsSP2Triangles(13,4,1,0,15,14,4,0), /* #<FACE 2531> and #<FACE 2529> */
  gsSP2Triangles(14,2,4,0,15,4,3,0), /* #<FACE 1140> and #<FACE 1139> */
  gsSPVertex(&cont_finish_v[683],14,0),
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP2Triangles(2,3,4,0,4,5,9,0), /* #<FACE 3015> and #<FACE 3014> */
  gsSP2Triangles(0,1,4,0,0,4,10,0), /* #<FACE 1841> and #<FACE 1839> */
  gsSP2Triangles(4,9,10,0,2,4,1,0), /* #<FACE 1830> and #<FACE 1828> */
  gsSP2Triangles(5,4,3,0,11,9,13,0), /* #<FACE 361> and #<FACE 3013> */
  gsSP2Triangles(8,11,12,0,6,10,11,0), /* #<FACE 3012> and #<FACE 1838> */
  gsSP2Triangles(6,11,7,0,8,7,11,0), /* #<FACE 1836> and #<FACE 1833> */
  gsSP2Triangles(11,10,9,0,13,12,11,0), /* #<FACE 1831> and #<FACE 364> */
  gsSPVertex(&cont_finish_v[697],14,2),
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP2Triangles(2,6,7,0,2,7,4,0), /* #<FACE 2882> and #<FACE 2881> */
  gsSP2Triangles(8,3,7,0,5,4,7,0), /* #<FACE 1870> and #<FACE 517> */
  gsSP1Triangle(8,7,6,0), /* #<FACE 516> */
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(12,13,14,0,9,13,11,0), /* #<FACE 2780> and #<FACE 2779> */
  gsSP2Triangles(10,15,13,0,12,11,13,0), /* #<FACE 690> and #<FACE 687> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 686> */
  gsSPVertex(&cont_finish_v[711],16,0),
  gsSP2Triangles(4,1,7,0,4,7,5,0), /* #<FACE 2635> and #<FACE 2634> */
  gsSP2Triangles(2,7,3,0,0,3,7,0), /* #<FACE 1925> and #<FACE 1744> */
  gsSP2Triangles(7,2,6,0,6,5,7,0), /* #<FACE 1743> and #<FACE 1012> */
  gsSP1Triangle(0,7,1,0), /* #<FACE 1011> */
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP2Triangles(13,8,15,0,9,13,14,0), /* #<FACE 2872> and #<FACE 2871> */
  gsSP2Triangles(8,13,12,0,9,10,13,0), /* #<FACE 1885> and #<FACE 1884> */
  gsSP2Triangles(10,11,13,0,12,13,11,0), /* #<FACE 1785> and #<FACE 1784> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 527> */
  gsSPVertex(&cont_finish_v[727],12,4),
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(5,7,8,0,4,6,8,0), /* #<FACE 2640> and #<FACE 2639> */
  gsSP2Triangles(9,0,8,0,8,1,4,0), /* #<FACE 1930> and #<FACE 1929> */
  gsSP2Triangles(1,8,0,0,8,6,5,0), /* #<FACE 1739> and #<FACE 1007> */
  gsSP2Triangles(9,8,7,0,15,14,10,0), /* #<FACE 1006> and #<FACE 2638> */
  gsSP2Triangles(10,2,15,0,11,13,15,0), /* #<FACE 2636> and #<FACE 1942> */
  gsSP2Triangles(11,15,12,0,3,12,15,0), /* #<FACE 1941> and #<FACE 1728> */
  gsSP2Triangles(14,15,13,0,3,15,2,0), /* #<FACE 1727> and #<FACE 1010> */
  gsSPVertex(&cont_finish_v[739],16,0),
  gsSP2Triangles(7,5,2,0,0,6,7,0), /* #<FACE 2330> and #<FACE 2329> */
  gsSP2Triangles(0,7,3,0,4,7,1,0), /* #<FACE 2328> and #<FACE 2327> */
  gsSP2Triangles(2,1,7,0,7,4,3,0), /* #<FACE 1342> and #<FACE 1341> */
  gsSP2Triangles(7,6,5,0,11,13,14,0), /* #<FACE 1340> and #<FACE 2646> */
  gsSP2Triangles(8,12,14,0,9,14,10,0), /* #<FACE 2645> and #<FACE 1936> */
  gsSP2Triangles(14,9,8,0,10,14,15,0), /* #<FACE 1935> and #<FACE 1733> */
  gsSP2Triangles(14,12,11,0,15,14,13,0), /* #<FACE 993> and #<FACE 992> */
  gsSPVertex(&cont_finish_v[755],14,2),
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(5,7,8,0), /* #<FACE 2178> */
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(3,7,4,0,7,3,2,0), /* #<FACE 2177> and #<FACE 1834> */
  gsSP1Triangle(5,4,7,0), /* #<FACE 1492> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(8,7,6,0), /* #<FACE 1491> */
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(9,12,13,0,12,14,11,0), /* #<FACE 2182> and #<FACE 2180> */
  gsSP2Triangles(10,15,12,0,12,15,14,0), /* #<FACE 2179> and #<FACE 1490> */
  gsSP1Triangle(13,12,11,0), /* #<FACE 1487> */
  gsSPVertex(&cont_finish_v[769],14,0),
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(3,4,7,0), /* #<FACE 360> */
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(8,10,15,0,10,9,14,0), /* #<FACE 2098> and #<FACE 2097> */
  gsSP2Triangles(1,2,10,0,2,9,10,0), /* #<FACE 2095> and #<FACE 1574> */
  gsSP2Triangles(10,14,15,0,11,0,13,0), /* #<FACE 1571> and #<FACE 2100> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(6,7,13,0), /* #<FACE 2099> */
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP1Triangle(13,12,11,0), /* #<FACE 1827> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(7,5,13,0), /* #<FACE 1570> */
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP1Triangle(6,13,0,0), /* #<FACE 1569> */
  gsSPVertex(&cont_finish_v[783],6,10),
  gsSP2Triangles(13,0,2,0,10,12,13,0), /* #<FACE 3247> and #<FACE 2124> */
  gsSP2Triangles(10,13,1,0,13,2,1,0), /* #<FACE 2093> and #<FACE 1576> */
  gsSP1Triangle(13,12,11,0), /* #<FACE 1545> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP2Triangles(7,14,3,0,5,7,4,0), /* #<FACE 3017> and #<FACE 3016> */
  gsSP2Triangles(6,15,7,0,7,15,14,0), /* #<FACE 2122> and #<FACE 1547> */
  gsSPVertex(&cont_finish_v[789],16,0),
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(4,6,7,0,2,5,7,0), /* #<FACE 2696> and #<FACE 2695> */
  gsSP2Triangles(7,0,1,0,1,3,7,0), /* #<FACE 1970> and #<FACE 1969> */
  gsSP2Triangles(3,2,7,0,7,5,4,0), /* #<FACE 1700> and #<FACE 869> */
  gsSP2Triangles(0,7,6,0,8,13,14,0), /* #<FACE 868> and #<FACE 1934> */
  gsSP2Triangles(15,12,14,0,8,14,9,0), /* #<FACE 1933> and #<FACE 1932> */
  gsSP2Triangles(14,11,10,0,14,10,9,0), /* #<FACE 1931> and #<FACE 1737> */
  gsSP2Triangles(12,11,14,0,15,14,13,0), /* #<FACE 1736> and #<FACE 1735> */
  gsSPVertex(&cont_finish_v[805],13,3),
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP2Triangles(8,9,6,0,3,7,8,0), /* #<FACE 3018> and #<FACE 3011> */
  gsSP2Triangles(4,8,5,0,8,6,5,0), /* #<FACE 2143> and #<FACE 1526> */
  gsSP1Triangle(9,8,7,0), /* #<FACE 365> */
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(0,15,13,0,10,12,13,0), /* #<FACE 3187> and #<FACE 2697> */
  gsSP2Triangles(14,11,13,0,13,1,0,0), /* #<FACE 2136> and #<FACE 1699> */
  gsSP2Triangles(11,10,13,0,1,13,12,0), /* #<FACE 1533> and #<FACE 867> */
  gsSP1Triangle(14,13,15,0), /* #<FACE 199> */
  gsSPVertex(&cont_finish_v[818],14,0),
  gsSP2Triangles(3,5,6,0,1,5,4,0), /* #<FACE 2738> and #<FACE 2737> */
  gsSP2Triangles(0,5,2,0,5,0,15,0), /* #<FACE 2294> and #<FACE 2293> */
  gsSP2Triangles(2,5,1,0,4,5,3,0), /* #<FACE 1375> and #<FACE 770> */
  gsSP2Triangles(15,6,5,0,13,11,7,0), /* #<FACE 767> and #<FACE 3128> */
  gsSP2Triangles(8,12,13,0,8,13,9,0), /* #<FACE 2184> and #<FACE 2141> */
  gsSP2Triangles(13,10,9,0,13,12,11,0), /* #<FACE 1528> and #<FACE 1485> */
  gsSPVertex(&cont_finish_v[832],16,0),
  gsSP2Triangles(13,15,14,0,2,3,4,0), /* #<FACE 324> and #<FACE 3051> */
  gsSP2Triangles(0,1,4,0,2,4,1,0), /* #<FACE 1847> and #<FACE 1822> */
  gsSP2Triangles(5,4,3,0,8,11,9,0), /* #<FACE 331> and #<FACE 3053> */
  gsSP2Triangles(6,11,7,0,10,11,6,0), /* #<FACE 2315> and #<FACE 1358> */
  gsSP2Triangles(11,8,7,0,12,9,11,0), /* #<FACE 1354> and #<FACE 329> */
  gsSP1Triangle(12,11,10,0), /* #<FACE 328> */
  gsSPVertex(&cont_finish_v[848],13,0),
  gsSP2Triangles(5,15,13,0,15,5,0,0), /* #<FACE 3056> and #<FACE 3055> */
  gsSP2Triangles(13,7,5,0,7,2,5,0), /* #<FACE 2322> and #<FACE 1347> */
  gsSP2Triangles(5,2,1,0,1,0,5,0), /* #<FACE 1345> and #<FACE 325> */
  gsSP2Triangles(4,8,3,0,8,6,9,0), /* #<FACE 3057> and #<FACE 3050> */
  gsSP2Triangles(6,8,7,0,7,8,2,0), /* #<FACE 2321> and #<FACE 1348> */
  gsSP2Triangles(8,4,2,0,9,3,8,0), /* #<FACE 1346> and #<FACE 323> */
  gsSP2Triangles(10,14,11,0,14,15,11,0), /* #<FACE 2313> and #<FACE 1356> */
  gsSP1Triangle(13,14,12,0), /* #<FACE 1350> */
  gsSPVertex(&cont_finish_v[861],9,7),
  gsSP2Triangles(8,13,3,0,15,7,8,0), /* #<FACE 3054> and #<FACE 2309> */
  gsSP2Triangles(13,8,7,0,8,14,15,0), /* #<FACE 1360> and #<FACE 732> */
  gsSP2Triangles(9,10,11,0,0,11,10,0), /* #<FACE 2307> and #<FACE 1362> */
  gsSP2Triangles(1,12,11,0,1,11,0,0), /* #<FACE 1343> and #<FACE 326> */
  gsSP2Triangles(12,2,4,0,4,3,13,0), /* #<FACE 2323> and #<FACE 327> */
  gsSPVertex(&cont_finish_v[870],13,0),
  gsSP2Triangles(2,5,3,0,6,5,2,0), /* #<FACE 2766> and #<FACE 728> */
  gsSP2Triangles(4,3,5,0,8,5,6,0), /* #<FACE 724> and #<FACE 2765> */
  gsSP2Triangles(6,7,8,0,0,8,7,0), /* #<FACE 2317> and #<FACE 1352> */
  gsSP2Triangles(1,8,0,0,9,15,14,0), /* #<FACE 330> and #<FACE 2763> */
  gsSP2Triangles(14,12,9,0,11,12,14,0), /* #<FACE 729> and #<FACE 2764> */
  gsSP1Triangle(12,11,10,0), /* #<FACE 2311> */
  gsSPVertex(&cont_finish_v[883],13,3),
  gsSP2Triangles(13,3,5,0,3,4,5,0), /* #<FACE 718> and #<FACE 719> */
  gsSP2Triangles(6,5,4,0,11,7,1,0), /* #<FACE 2771> and #<FACE 2768> */
  gsSP2Triangles(7,11,8,0,12,15,11,0), /* #<FACE 2767> and #<FACE 1336> */
  gsSP2Triangles(15,8,11,0,10,1,0,0), /* #<FACE 723> and #<FACE 3052> */
  gsSP2Triangles(1,10,11,0,9,12,10,0), /* #<FACE 2334> and #<FACE 1339> */
  gsSP2Triangles(12,11,10,0,14,15,12,0), /* #<FACE 1335> and #<FACE 2333> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 332> */
  gsSPVertex(&cont_finish_v[896],14,0),
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP2Triangles(2,1,3,0,2,0,1,0), /* #<FACE 705> and #<FACE 373> */
  gsSP2Triangles(6,4,3,0,6,5,4,0), /* #<FACE 706> and #<FACE 358> */
  gsSP2Triangles(9,7,10,0,9,8,7,0), /* #<FACE 709> and #<FACE 362> */
  gsSP2Triangles(13,11,10,0,13,12,11,0), /* #<FACE 710> and #<FACE 369> */
  gsSPVertex(&cont_finish_v[910],14,2),
  gsSP2Triangles(12,13,4,0,7,4,2,0), /* #<FACE 2355> and #<FACE 1316> */
  gsSP2Triangles(2,4,13,0,14,12,4,0), /* #<FACE 1314> and #<FACE 379> */
  gsSP2Triangles(3,9,11,0,3,14,9,0), /* #<FACE 2999> and #<FACE 2998> */
  gsSP2Triangles(9,4,7,0,14,4,9,0), /* #<FACE 2353> and #<FACE 378> */
  gsSP2Triangles(1,0,5,0,9,7,10,0), /* #<FACE 3010> and #<FACE 2351> */
  gsSP2Triangles(15,10,6,0,6,10,8,0), /* #<FACE 2349> and #<FACE 1320> */
  gsSP2Triangles(8,10,7,0,11,9,10,0), /* #<FACE 1318> and #<FACE 377> */
  gsSP1Triangle(11,10,15,0), /* #<FACE 376> */
  gsSPVertex(&cont_finish_v[924],12,0),
  gsSP2Triangles(1,15,3,0,0,1,8,0), /* #<FACE 375> and #<FACE 3002> */
  gsSP2Triangles(0,8,11,0,8,3,2,0), /* #<FACE 2995> and #<FACE 2361> */
  gsSP2Triangles(1,3,8,0,6,5,4,0), /* #<FACE 374> and #<FACE 3006> */
  gsSP2Triangles(8,2,10,0,13,10,7,0), /* #<FACE 2359> and #<FACE 1312> */
  gsSP2Triangles(7,10,2,0,11,8,10,0), /* #<FACE 1310> and #<FACE 381> */
  gsSP2Triangles(9,12,14,0,9,11,12,0), /* #<FACE 2997> and #<FACE 2996> */
  gsSP2Triangles(12,10,13,0,11,10,12,0), /* #<FACE 2357> and #<FACE 380> */
  gsSPVertex(&cont_finish_v[936],12,4),
  gsSP2Triangles(5,13,8,0,5,8,15,0), /* #<FACE 2370> and #<FACE 2368> */
  gsSP2Triangles(8,13,12,0,0,15,1,0), /* #<FACE 1299> and #<FACE 1303> */
  gsSP2Triangles(1,15,8,0,15,7,5,0), /* #<FACE 1301> and #<FACE 2367> */
  gsSP2Triangles(15,14,7,0,4,7,14,0), /* #<FACE 2365> and #<FACE 1304> */
  gsSP2Triangles(7,6,5,0,8,9,1,0), /* #<FACE 1302> and #<FACE 3001> */
  gsSP2Triangles(8,12,9,0,9,10,3,0), /* #<FACE 3000> and #<FACE 2363> */
  gsSP2Triangles(2,3,11,0,11,3,10,0), /* #<FACE 1308> and #<FACE 1306> */
  gsSPVertex(&cont_finish_v[948],12,0),
  gsSP2Triangles(14,0,2,0,14,15,0,0), /* #<FACE 2380> and #<FACE 2366> */
  gsSP2Triangles(1,2,0,0,5,2,3,0), /* #<FACE 1289> and #<FACE 2378> */
  gsSP2Triangles(5,3,10,0,3,2,1,0), /* #<FACE 2376> and #<FACE 1291> */
  gsSP2Triangles(9,10,3,0,10,7,5,0), /* #<FACE 1293> and #<FACE 2375> */
  gsSP2Triangles(10,8,7,0,4,7,8,0), /* #<FACE 2373> and #<FACE 1296> */
  gsSP2Triangles(7,6,5,0,8,10,11,0), /* #<FACE 1294> and #<FACE 2374> */
  gsSP2Triangles(8,11,13,0,11,10,9,0), /* #<FACE 2372> and #<FACE 1295> */
  gsSP1Triangle(12,13,11,0), /* #<FACE 1297> */
  gsSPVertex(&cont_finish_v[960],16,0),
  gsSP2Triangles(0,11,10,0,11,13,12,0), /* #<FACE 1278> and #<FACE 1276> */
  gsSP2Triangles(5,1,10,0,5,7,1,0), /* #<FACE 2389> and #<FACE 2387> */
  gsSP2Triangles(8,1,7,0,1,0,10,0), /* #<FACE 1282> and #<FACE 1280> */
  gsSP2Triangles(2,4,5,0,3,6,4,0), /* #<FACE 2388> and #<FACE 2386> */
  gsSP2Triangles(6,7,4,0,7,5,4,0), /* #<FACE 1283> and #<FACE 1281> */
  gsSP2Triangles(6,9,7,0,6,15,9,0), /* #<FACE 2385> and #<FACE 2383> */
  gsSP2Triangles(14,9,15,0,9,8,7,0), /* #<FACE 1286> and #<FACE 1284> */
  gsSPVertex(&cont_finish_v[976],9,0),
  gsDPSetPrimColor(0,0,255,0,0,255),
  gsSP1Triangle(2,1,0,0), /* #<FACE 1269> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP2Triangles(7,12,3,0,7,3,15,0), /* #<FACE 2395> and #<FACE 2381> */
  gsSP2Triangles(3,14,15,0,13,3,12,0), /* #<FACE 1288> and #<FACE 1274> */
  gsSP2Triangles(4,6,7,0,5,8,6,0), /* #<FACE 2396> and #<FACE 2394> */
  gsSP2Triangles(8,12,6,0,12,7,6,0), /* #<FACE 1275> and #<FACE 1273> */
  gsSP2Triangles(8,11,12,0,8,10,11,0), /* #<FACE 2393> and #<FACE 2391> */
  gsSPVertex(&cont_finish_v[985],11,5),
  gsDPSetPrimColor(0,0,255,0,0,255),
  gsSP2Triangles(5,8,7,0,5,7,6,0), /* #<FACE 2984> and #<FACE 2983> */
  gsSP2Triangles(13,6,7,0,0,7,8,0), /* #<FACE 400> and #<FACE 399> */
  gsSP2Triangles(0,8,15,0,1,11,9,0), /* #<FACE 398> and #<FACE 2399> */
  gsSP2Triangles(10,9,11,0,2,15,14,0), /* #<FACE 1270> and #<FACE 2986> */
  gsSP2Triangles(2,0,15,0,11,1,2,0), /* #<FACE 2985> and #<FACE 2400> */
  gsSP2Triangles(11,2,12,0,14,12,2,0), /* #<FACE 2398> and #<FACE 1271> */
  gsSPVertex(&cont_finish_v[996],12,0),
  gsSP2Triangles(14,15,1,0,0,2,3,0), /* #<FACE 397> and #<FACE 2980> */
  gsSP2Triangles(0,3,1,0,14,1,3,0), /* #<FACE 2979> and #<FACE 404> */
  gsSP2Triangles(5,3,2,0,6,9,4,0), /* #<FACE 403> and #<FACE 2407> */
  gsSP2Triangles(10,4,9,0,9,6,7,0), /* #<FACE 1262> and #<FACE 2408> */
  gsSP2Triangles(9,7,8,0,7,6,5,0), /* #<FACE 2406> and #<FACE 1261> */
  gsSP2Triangles(13,8,7,0,8,11,9,0), /* #<FACE 1263> and #<FACE 2405> */
  gsSP1Triangle(11,10,9,0), /* #<FACE 1264> */
  gsSPVertex(&cont_finish_v[1008],16,0),
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(14,0,13,0,10,0,2,0), /* #<FACE 2961> and #<FACE 2774> */
  gsSP2Triangles(0,14,15,0,12,0,10,0), /* #<FACE 1244> and #<FACE 1232> */
  gsSP2Triangles(0,15,2,0,12,13,0,0), /* #<FACE 700> and #<FACE 428> */
  gsSP2Triangles(1,4,5,0,10,2,5,0), /* #<FACE 2950> and #<FACE 2773> */
  gsSP2Triangles(5,11,10,0,1,5,3,0), /* #<FACE 2438> and #<FACE 1242> */
  gsSP2Triangles(3,5,2,0,11,5,4,0), /* #<FACE 704> and #<FACE 439> */
  gsDPSetPrimColor(0,0,255,0,0,255),
  gsSP2Triangles(6,9,7,0,9,8,7,0), /* #<FACE 2397> and #<FACE 1272> */
  gsSPVertex(&cont_finish_v[1024],10,0),
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(3,6,4,0,7,6,3,0), /* #<FACE 2776> and #<FACE 699> */
  gsSP2Triangles(5,1,2,0,6,8,5,0), /* #<FACE 2960> and #<FACE 2439> */
  gsSP2Triangles(2,0,5,0,4,5,0,0), /* #<FACE 2429> and #<FACE 1240> */
  gsSP2Triangles(1,5,8,0,5,4,6,0), /* #<FACE 1230> and #<FACE 696> */
  gsSP2Triangles(14,6,7,0,6,14,8,0), /* #<FACE 2775> and #<FACE 2440> */
  gsSP2Triangles(7,15,14,0,13,8,14,0), /* #<FACE 2425> and #<FACE 1229> */
  gsSP2Triangles(9,12,10,0,9,10,11,0), /* #<FACE 2437> and #<FACE 1231> */
  gsSPVertex(&cont_finish_v[1034],12,4),
  gsSP2Triangles(4,12,10,0,13,9,11,0), /* #<FACE 2445> and #<FACE 2444> */
  gsSP2Triangles(9,5,8,0,7,14,15,0), /* #<FACE 691> and #<FACE 2959> */
  gsSP2Triangles(7,5,9,0,9,13,7,0), /* #<FACE 2778> and #<FACE 2443> */
  gsSP2Triangles(5,7,6,0,7,15,6,0), /* #<FACE 2413> and #<FACE 1256> */
  gsSP2Triangles(14,7,13,0,0,2,8,0), /* #<FACE 1226> and #<FACE 2777> */
  gsSP2Triangles(11,9,2,0,2,9,8,0), /* #<FACE 1225> and #<FACE 695> */
  gsSP1Triangle(11,2,1,0), /* #<FACE 429> */
  gsSPVertex(&cont_finish_v[1046],10,0),
  gsSP2Triangles(2,0,3,0,3,14,1,0), /* #<FACE 2448> and #<FACE 2783> */
  gsSP2Triangles(14,3,0,0,0,15,14,0), /* #<FACE 1221> and #<FACE 430> */
  gsSP2Triangles(14,13,1,0,11,4,13,0), /* #<FACE 683> and #<FACE 2782> */
  gsSP2Triangles(1,4,3,0,13,4,1,0), /* #<FACE 2781> and #<FACE 684> */
  gsSP2Triangles(3,12,2,0,12,3,4,0), /* #<FACE 1222> and #<FACE 685> */
  gsSP2Triangles(4,11,10,0,10,12,4,0), /* #<FACE 2446> and #<FACE 1224> */
  gsSP2Triangles(5,10,11,0,6,8,9,0), /* #<FACE 1223> and #<FACE 2442> */
  gsSP1Triangle(9,8,7,0), /* #<FACE 1227> */
  gsSPVertex(&cont_finish_v[1056],13,3),
  gsSP2Triangles(11,5,12,0,3,5,13,0), /* #<FACE 2452> and #<FACE 1254> */
  gsSP2Triangles(6,12,5,0,5,11,13,0), /* #<FACE 1217> and #<FACE 677> */
  gsSP2Triangles(14,9,0,0,14,15,9,0), /* #<FACE 2450> and #<FACE 2449> */
  gsSP2Triangles(15,6,4,0,3,8,4,0), /* #<FACE 2958> and #<FACE 2785> */
  gsSP2Triangles(4,5,3,0,15,4,9,0), /* #<FACE 2415> and #<FACE 1220> */
  gsSP2Triangles(9,4,8,0,6,5,4,0), /* #<FACE 678> and #<FACE 431> */
  gsSP2Triangles(7,10,8,0,0,9,10,0), /* #<FACE 2784> and #<FACE 1219> */
  gsSP1Triangle(10,9,8,0), /* #<FACE 682> */
  gsSPVertex(&cont_finish_v[1069],10,0),
  gsSP2Triangles(15,14,3,0,3,14,5,0), /* #<FACE 2790> and #<FACE 671> */
  gsSP2Triangles(4,6,14,0,14,6,5,0), /* #<FACE 2789> and #<FACE 672> */
  gsSP2Triangles(7,13,11,0,12,9,11,0), /* #<FACE 2786> and #<FACE 2451> */
  gsSP2Triangles(1,8,2,0,0,7,8,0), /* #<FACE 2957> and #<FACE 2787> */
  gsSP2Triangles(8,1,0,0,9,8,11,0), /* #<FACE 1238> and #<FACE 1218> */
  gsSP2Triangles(11,8,7,0,9,2,8,0), /* #<FACE 674> and #<FACE 432> */
  gsSPVertex(&cont_finish_v[1079],12,4),
  gsSP2Triangles(4,9,6,0,10,9,4,0), /* #<FACE 2792> and #<FACE 670> */
  gsSP2Triangles(15,7,14,0,8,5,7,0), /* #<FACE 2455> and #<FACE 2423> */
  gsSP2Triangles(6,7,5,0,14,7,9,0), /* #<FACE 1246> and #<FACE 1214> */
  gsSP2Triangles(7,6,9,0,15,8,7,0), /* #<FACE 666> and #<FACE 433> */
  gsSP2Triangles(1,9,10,0,9,1,14,0), /* #<FACE 2791> and #<FACE 2456> */
  gsSP2Triangles(10,0,1,0,2,14,1,0), /* #<FACE 2431> and #<FACE 1213> */
  gsSP2Triangles(3,12,11,0,11,12,13,0), /* #<FACE 2454> and #<FACE 1215> */
  gsSPVertex(&cont_finish_v[1091],12,0),
  gsSP2Triangles(15,0,2,0,0,15,3,0), /* #<FACE 1209> and #<FACE 658> */
  gsSP2Triangles(14,3,15,0,14,5,3,0), /* #<FACE 2797> and #<FACE 2796> */
  gsSP2Triangles(3,6,0,0,5,6,3,0), /* #<FACE 2795> and #<FACE 659> */
  gsSP2Triangles(0,4,1,0,4,0,6,0), /* #<FACE 2459> and #<FACE 660> */
  gsSP2Triangles(8,4,6,0,8,6,7,0), /* #<FACE 1212> and #<FACE 2458> */
  gsSP2Triangles(7,6,5,0,7,9,8,0), /* #<FACE 1211> and #<FACE 2794> */
  gsSP2Triangles(8,9,10,0,9,13,10,0), /* #<FACE 661> and #<FACE 662> */
  gsSP2Triangles(12,10,13,0,12,11,10,0), /* #<FACE 2793> and #<FACE 663> */
  gsSPVertex(&cont_finish_v[1103],13,3),
  gsSP2Triangles(4,12,13,0,11,12,15,0), /* #<FACE 652> and #<FACE 434> */
  gsSP2Triangles(14,6,2,0,6,3,7,0), /* #<FACE 1207> and #<FACE 653> */
  gsSP2Triangles(5,15,12,0,5,3,6,0), /* #<FACE 2955> and #<FACE 2799> */
  gsSP2Triangles(6,14,5,0,3,5,4,0), /* #<FACE 2461> and #<FACE 2421> */
  gsSP2Triangles(5,12,4,0,15,5,14,0), /* #<FACE 1248> and #<FACE 1208> */
  gsSP2Triangles(6,7,10,0,10,2,6,0), /* #<FACE 2798> and #<FACE 2462> */
  gsSP2Triangles(8,10,7,0,1,2,0,0), /* #<FACE 657> and #<FACE 2460> */
  gsSP1Triangle(2,10,9,0), /* #<FACE 2956> */
  gsSPVertex(&cont_finish_v[1116],11,0),
  gsSP2Triangles(15,3,11,0,3,15,4,0), /* #<FACE 1203> and #<FACE 646> */
  gsSP2Triangles(14,4,15,0,4,14,5,0), /* #<FACE 2804> and #<FACE 2803> */
  gsSP2Triangles(6,5,14,0,7,10,11,0), /* #<FACE 647> and #<FACE 2464> */
  gsSP2Triangles(7,9,10,0,2,8,1,0), /* #<FACE 1206> and #<FACE 2954> */
  gsSP2Triangles(10,8,13,0,8,10,9,0), /* #<FACE 2801> and #<FACE 2463> */
  gsSP2Triangles(8,2,0,0,8,0,13,0), /* #<FACE 1236> and #<FACE 649> */
  gsSP2Triangles(9,1,8,0,10,13,12,0), /* #<FACE 435> and #<FACE 2800> */
  gsSP1Triangle(11,10,12,0), /* #<FACE 1205> */
  gsSPVertex(&cont_finish_v[1127],13,3),
  gsSP2Triangles(3,12,11,0,10,3,11,0), /* #<FACE 640> and #<FACE 639> */
  gsSP2Triangles(5,8,9,0,13,14,9,0), /* #<FACE 2805> and #<FACE 2467> */
  gsSP2Triangles(4,7,15,0,9,7,5,0), /* #<FACE 2953> and #<FACE 2806> */
  gsSP2Triangles(7,4,6,0,14,7,9,0), /* #<FACE 1250> and #<FACE 1202> */
  gsSP2Triangles(7,6,5,0,14,15,7,0), /* #<FACE 641> and #<FACE 436> */
  gsSP2Triangles(9,2,13,0,8,0,2,0), /* #<FACE 2468> and #<FACE 2433> */
  gsSP2Triangles(1,13,2,0,2,9,8,0), /* #<FACE 1201> and #<FACE 645> */
  gsSPVertex(&cont_finish_v[1140],10,0),
  gsSP2Triangles(0,1,14,0,14,1,15,0), /* #<FACE 2472> and #<FACE 1197> */
  gsSP2Triangles(14,13,0,0,0,13,2,0), /* #<FACE 2811> and #<FACE 633> */
  gsSP2Triangles(4,2,3,0,13,3,2,0), /* #<FACE 2810> and #<FACE 634> */
  gsSP2Triangles(12,4,5,0,5,4,3,0), /* #<FACE 2470> and #<FACE 1199> */
  gsSP2Triangles(5,8,12,0,12,8,11,0), /* #<FACE 2808> and #<FACE 636> */
  gsSP2Triangles(7,9,8,0,9,7,6,0), /* #<FACE 2807> and #<FACE 2466> */
  gsSP2Triangles(8,9,11,0,9,10,11,0), /* #<FACE 637> and #<FACE 638> */
  gsSPVertex(&cont_finish_v[1150],14,2),
  gsSP2Triangles(14,13,5,0,8,6,2,0), /* #<FACE 437> and #<FACE 632> */
  gsSP2Triangles(6,3,2,0,4,5,13,0), /* #<FACE 628> and #<FACE 2952> */
  gsSP2Triangles(4,3,6,0,6,10,4,0), /* #<FACE 2813> and #<FACE 2473> */
  gsSP2Triangles(13,12,4,0,3,4,12,0), /* #<FACE 2417> and #<FACE 1252> */
  gsSP2Triangles(5,4,10,0,9,6,8,0), /* #<FACE 1196> and #<FACE 2812> */
  gsSP2Triangles(6,9,10,0,8,7,9,0), /* #<FACE 2474> and #<FACE 2419> */
  gsSP2Triangles(11,10,9,0,1,0,15,0), /* #<FACE 1195> and #<FACE 2471> */
  gsSPVertex(&cont_finish_v[1164],11,0),
  gsSP2Triangles(4,15,3,0,15,4,5,0), /* #<FACE 1192> and #<FACE 623> */
  gsSP2Triangles(7,9,14,0,9,6,10,0), /* #<FACE 1193> and #<FACE 624> */
  gsSP2Triangles(8,6,9,0,9,7,8,0), /* #<FACE 2815> and #<FACE 2475> */
  gsSP2Triangles(6,8,1,0,8,2,1,0), /* #<FACE 2435> and #<FACE 1234> */
  gsSP2Triangles(0,8,7,0,0,2,8,0), /* #<FACE 1194> and #<FACE 438> */
  gsSP2Triangles(9,10,13,0,13,14,9,0), /* #<FACE 2814> and #<FACE 2476> */
  gsSP1Triangle(12,13,10,0), /* #<FACE 627> */
  gsSPVertex(&cont_finish_v[1175],8,8),
  gsSP2Triangles(8,2,0,0,1,2,15,0), /* #<FACE 2951> and #<FACE 2819> */
  gsSP2Triangles(8,14,2,0,2,14,15,0), /* #<FACE 1189> and #<FACE 620> */
  gsSP2Triangles(3,9,4,0,10,4,9,0), /* #<FACE 2478> and #<FACE 1191> */
  gsSP2Triangles(4,10,12,0,11,12,10,0), /* #<FACE 621> and #<FACE 2818> */
  gsSP2Triangles(11,13,12,0,12,5,4,0), /* #<FACE 2817> and #<FACE 2816> */
  gsSP1Triangle(13,5,12,0), /* #<FACE 622> */
  gsSPVertex(&cont_finish_v[1183],14,0),
  gsSP2Triangles(14,3,15,0,4,3,14,0), /* #<FACE 616> and #<FACE 2820> */
  gsSP2Triangles(14,1,4,0,3,4,2,0), /* #<FACE 2479> and #<FACE 2427> */
  gsSP1Triangle(0,4,1,0), /* #<FACE 1190> */
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP2Triangles(13,5,10,0,6,13,11,0), /* #<FACE 1857> and #<FACE 1856> */
  gsSP2Triangles(13,10,12,0,8,12,7,0), /* #<FACE 1855> and #<FACE 1849> */
  gsSP2Triangles(9,7,12,0,12,8,11,0), /* #<FACE 1820> and #<FACE 1819> */
  gsSP2Triangles(10,9,12,0,13,12,11,0), /* #<FACE 1814> and #<FACE 1813> */
  gsSPVertex(&cont_finish_v[1197],14,2),
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(0,14,15,0,14,0,2,0), /* #<FACE 2484> and #<FACE 608> */
  gsSP2Triangles(1,2,0,0,1,3,2,0), /* #<FACE 2825> and #<FACE 609> */
  gsSP2Triangles(12,3,1,0,3,8,2,0), /* #<FACE 2824> and #<FACE 2823> */
  gsSP2Triangles(8,14,2,0,3,12,10,0), /* #<FACE 610> and #<FACE 2482> */
  gsSP2Triangles(10,8,3,0,10,4,9,0), /* #<FACE 1188> and #<FACE 2822> */
  gsSP2Triangles(4,10,12,0,4,7,9,0), /* #<FACE 1187> and #<FACE 611> */
  gsSP2Triangles(5,6,7,0,7,6,9,0), /* #<FACE 2821> and #<FACE 612> */
  gsSP2Triangles(11,8,10,0,11,10,9,0), /* #<FACE 2481> and #<FACE 615> */
  gsSP1Triangle(1,13,12,0), /* #<FACE 2480> */
  gsSPVertex(&cont_finish_v[1211],14,0),
  gsDPSetPrimColor(0,0,0,153,0,255),
  gsSP2Triangles(4,2,1,0,5,0,2,0), /* #<FACE 1182> and #<FACE 471> */
  gsSP2Triangles(3,6,7,0,2,7,5,0), /* #<FACE 2917> and #<FACE 2916> */
  gsSP2Triangles(7,2,4,0,7,4,3,0), /* #<FACE 2487> and #<FACE 2485> */
  gsSP2Triangles(8,5,7,0,8,7,6,0), /* #<FACE 470> and #<FACE 469> */
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(13,11,10,0,9,12,11,0), /* #<FACE 605> and #<FACE 2827> */
  gsSP2Triangles(10,12,13,0,11,12,10,0), /* #<FACE 2826> and #<FACE 606> */
  gsSP2Triangles(15,13,12,0,14,13,15,0), /* #<FACE 607> and #<FACE 1185> */
  gsSPVertex(&cont_finish_v[1225],13,3),
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP1Triangle(3,11,12,0), /* #<FACE 1806> */
  gsDPSetPrimColor(0,0,0,153,0,255),
  gsSP2Triangles(0,15,10,0,1,10,14,0), /* #<FACE 2914> and #<FACE 1180> */
  gsSP2Triangles(10,13,14,0,15,13,10,0), /* #<FACE 1178> and #<FACE 472> */
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP2Triangles(4,8,9,0,4,9,5,0), /* #<FACE 1862> and #<FACE 1859> */
  gsSP2Triangles(9,6,5,0,9,8,7,0), /* #<FACE 1810> and #<FACE 1807> */
  gsDPSetPrimColor(0,0,0,153,0,255),
  gsSP2Triangles(10,2,0,0,2,10,1,0), /* #<FACE 2915> and #<FACE 2489> */
  gsSPVertex(&cont_finish_v[1238],10,0),
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP1Triangle(5,4,3,0), /* #<FACE 1850> */
  gsDPSetPrimColor(0,0,0,153,0,255),
  gsSP2Triangles(6,8,1,0,6,7,8,0), /* #<FACE 2495> and #<FACE 2493> */
  gsSP2Triangles(8,2,1,0,0,2,8,0), /* #<FACE 1174> and #<FACE 474> */
  gsSP2Triangles(15,0,13,0,7,14,13,0), /* #<FACE 2913> and #<FACE 2491> */
  gsSP2Triangles(13,8,7,0,0,8,13,0), /* #<FACE 1176> and #<FACE 473> */
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP1Triangle(11,9,12,0), /* #<FACE 1812> */
  gsSPVertex(&cont_finish_v[1248],13,3),
  gsDPSetPrimColor(0,0,0,153,0,255),
  gsSP1Triangle(3,15,10,0), /* #<FACE 476> */
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP2Triangles(4,9,7,0,4,5,9,0), /* #<FACE 1873> and #<FACE 1871> */
  gsSP2Triangles(6,9,5,0,9,8,7,0), /* #<FACE 1798> and #<FACE 1796> */
  gsDPSetPrimColor(0,0,0,153,0,255),
  gsSP2Triangles(10,12,13,0,12,10,15,0), /* #<FACE 2911> and #<FACE 2910> */
  gsSP2Triangles(15,14,12,0,11,12,14,0), /* #<FACE 2499> and #<FACE 1170> */
  gsSP2Triangles(13,2,0,0,1,2,11,0), /* #<FACE 2912> and #<FACE 2497> */
  gsSP2Triangles(2,12,11,0,13,12,2,0), /* #<FACE 1172> and #<FACE 475> */
  gsSPVertex(&cont_finish_v[1261],14,0),
  gsDPSetPrimColor(0,0,0,0,153,255),
  gsSP2Triangles(4,1,2,0,2,0,4,0), /* #<FACE 2907> and #<FACE 2507> */
  gsSP2Triangles(5,4,0,0,1,4,3,0), /* #<FACE 1162> and #<FACE 478> */
  gsSP2Triangles(3,8,9,0,8,3,4,0), /* #<FACE 2909> and #<FACE 2908> */
  gsSP2Triangles(4,5,8,0,7,8,5,0), /* #<FACE 2505> and #<FACE 1164> */
  gsSP2Triangles(9,11,12,0,6,11,7,0), /* #<FACE 2918> and #<FACE 2503> */
  gsSP2Triangles(11,6,10,0,11,8,7,0), /* #<FACE 1167> and #<FACE 1166> */
  gsSP2Triangles(9,8,11,0,12,11,10,0), /* #<FACE 477> and #<FACE 468> */
  gsDPSetPrimColor(0,0,0,153,0,255),
  gsSP1Triangle(14,15,13,0), /* #<FACE 1184> */
  gsSPVertex(&cont_finish_v[1275],13,3),
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP2Triangles(11,10,3,0,3,10,12,0), /* #<FACE 1781> and #<FACE 1779> */
  gsDPSetPrimColor(0,0,0,0,153,255),
  gsSP2Triangles(6,15,4,0,5,4,14,0), /* #<FACE 2904> and #<FACE 1158> */
  gsSP2Triangles(4,13,14,0,15,13,4,0), /* #<FACE 1156> and #<FACE 482> */
  gsSP2Triangles(4,7,6,0,7,4,5,0), /* #<FACE 2905> and #<FACE 2511> */
  gsSP2Triangles(0,7,5,0,8,6,7,0), /* #<FACE 1160> and #<FACE 481> */
  gsSP2Triangles(7,2,8,0,2,7,0,0), /* #<FACE 2906> and #<FACE 2509> */
  gsSP2Triangles(9,8,2,0,9,2,1,0), /* #<FACE 480> and #<FACE 479> */
  gsSPVertex(&cont_finish_v[1288],9,0),
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP1Triangle(4,3,2,0), /* #<FACE 1701> */
  gsDPSetPrimColor(0,0,0,0,153,255),
  gsSP2Triangles(15,5,13,0,0,14,13,0), /* #<FACE 2903> and #<FACE 2513> */
  gsSP2Triangles(13,1,0,0,5,1,13,0), /* #<FACE 1154> and #<FACE 483> */
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP2Triangles(6,12,10,0,10,7,6,0), /* #<FACE 1890> and #<FACE 1889> */
  gsSP2Triangles(8,10,11,0,10,8,7,0), /* #<FACE 1888> and #<FACE 1887> */
  gsSPVertex(&cont_finish_v[1297],11,5),
  gsDPSetPrimColor(0,0,0,153,0,255),
  gsSP2Triangles(5,14,13,0,13,15,6,0), /* #<FACE 1151> and #<FACE 1149> */
  gsDPSetPrimColor(0,0,0,0,153,255),
  gsSP2Triangles(8,0,1,0,8,1,7,0), /* #<FACE 2515> and #<FACE 2502> */
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(4,10,9,0,4,9,12,0), /* #<FACE 2652> and #<FACE 2650> */
  gsSP2Triangles(10,4,2,0,3,4,11,0), /* #<FACE 1968> and #<FACE 1967> */
  gsSP1Triangle(12,11,4,0), /* #<FACE 1702> */
  gsSPVertex(&cont_finish_v[1308],13,0),
  gsDPSetPrimColor(0,0,0,0,153,255),
  gsSP1Triangle(1,2,0,0), /* #<FACE 1134> */
  gsDPSetPrimColor(0,0,0,153,0,255),
  gsSP2Triangles(14,3,12,0,3,11,12,0), /* #<FACE 2517> and #<FACE 1152> */
  gsSP2Triangles(6,5,4,0,9,8,7,0), /* #<FACE 2530> and #<FACE 2524> */
  gsSP2Triangles(15,12,10,0,11,10,12,0), /* #<FACE 2519> and #<FACE 1150> */
  gsSP2Triangles(12,15,13,0,12,13,14,0), /* #<FACE 2520> and #<FACE 2518> */
  gsSPVertex(&cont_finish_v[1321],12,4),
  gsDPSetPrimColor(0,0,0,0,153,255),
  gsSP2Triangles(8,6,4,0,5,4,6,0), /* #<FACE 2539> and #<FACE 1130> */
  gsSP2Triangles(6,8,9,0,6,9,0,0), /* #<FACE 2540> and #<FACE 2538> */
  gsSP2Triangles(9,8,7,0,0,11,13,0), /* #<FACE 1129> and #<FACE 2536> */
  gsSP2Triangles(11,0,9,0,10,14,12,0), /* #<FACE 1131> and #<FACE 2534> */
  gsSP2Triangles(14,13,12,0,12,13,11,0), /* #<FACE 1135> and #<FACE 1133> */
  gsSP2Triangles(13,1,0,0,13,14,1,0), /* #<FACE 2535> and #<FACE 2533> */
  gsSP1Triangle(15,1,14,0), /* #<FACE 1136> */
  gsSPVertex(&cont_finish_v[1333],16,0),
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP2Triangles(3,4,2,0,8,5,7,0), /* #<FACE 1100> and #<FACE 526> */
  gsSP2Triangles(8,7,6,0,1,12,10,0), /* #<FACE 525> and #<FACE 594> */
  gsSP2Triangles(10,11,0,0,11,9,0,0), /* #<FACE 2591> and #<FACE 1078> */
  gsSP1Triangle(12,11,10,0), /* #<FACE 595> */
  gsDPSetPrimColor(0,0,0,0,153,255),
  gsSP1Triangle(15,14,13,0), /* #<FACE 2544> */
  gsSPVertex(&cont_finish_v[1349],11,5),
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP2Triangles(13,15,14,0,5,6,7,0), /* #<FACE 1092> and #<FACE 2597> */
  gsSP2Triangles(8,7,6,0,9,1,0,0), /* #<FACE 1072> and #<FACE 2592> */
  gsSP2Triangles(10,3,2,0,10,12,3,0), /* #<FACE 2569> and #<FACE 2568> */
  gsSP1Triangle(3,12,11,0), /* #<FACE 1101> */
  gsSPVertex(&cont_finish_v[1360],13,0),
  gsSP2Triangles(1,2,3,0,12,0,1,0), /* #<FACE 1787> and #<FACE 1087> */
  gsSP2Triangles(6,5,4,0,9,8,7,0), /* #<FACE 593> and #<FACE 510> */
  gsSP2Triangles(10,14,15,0,12,11,13,0), /* #<FACE 2577> and #<FACE 2876> */
  gsSP2Triangles(14,0,13,0,13,0,12,0), /* #<FACE 2576> and #<FACE 1093> */
  gsSPVertex(&cont_finish_v[1373],13,3),
  gsSP2Triangles(3,6,7,0,3,5,6,0), /* #<FACE 2590> and #<FACE 2586> */
  gsSP2Triangles(6,5,4,0,8,7,6,0), /* #<FACE 1083> and #<FACE 1079> */
  gsSP2Triangles(9,14,12,0,9,10,14,0), /* #<FACE 2607> and #<FACE 2605> */
  gsSP2Triangles(11,14,10,0,14,13,12,0), /* #<FACE 1064> and #<FACE 1062> */
  gsSP2Triangles(15,1,0,0,15,2,1,0), /* #<FACE 2582> and #<FACE 1882> */
  gsSPVertex(&cont_finish_v[1386],16,0),
  gsSP2Triangles(3,2,10,0,4,9,7,0), /* #<FACE 2618> and #<FACE 2629> */
  gsSP2Triangles(4,5,9,0,6,9,5,0), /* #<FACE 2627> and #<FACE 1042> */
  gsSP2Triangles(9,8,7,0,10,13,11,0), /* #<FACE 1040> and #<FACE 2619> */
  gsSP2Triangles(13,12,11,0,1,14,0,0), /* #<FACE 1050> and #<FACE 2614> */
  gsSP1Triangle(15,0,14,0), /* #<FACE 1055> */
  gsSPVertex(&cont_finish_v[1402],14,2),
  gsDPSetPrimColor(0,0,0,0,153,255),
  gsSP1Triangle(14,2,15,0), /* #<FACE 1132> */
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP2Triangles(5,4,3,0,12,6,10,0), /* #<FACE 1077> and #<FACE 1054> */
  gsSP2Triangles(7,9,0,0,0,9,1,0), /* #<FACE 1058> and #<FACE 2613> */
  gsSP2Triangles(8,13,1,0,9,8,1,0), /* #<FACE 1782> and #<FACE 1056> */
  gsSP2Triangles(11,12,10,0,13,12,11,0), /* #<FACE 2615> and #<FACE 1780> */
  gsSPVertex(&cont_finish_v[1416],14,0),
  gsSP1Triangle(2,1,0,0), /* #<FACE 514> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(5,4,3,0), /* #<FACE 363> */
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(10,8,12,0,9,12,6,0), /* #<FACE 1840> and #<FACE 1837> */
  gsSP2Triangles(6,12,7,0,8,7,12,0), /* #<FACE 1832> and #<FACE 1829> */
  gsSP2Triangles(9,11,12,0,12,11,10,0), /* #<FACE 176> and #<FACE 140> */
  gsDPSetPrimColor(0,0,0,0,153,255),
  gsSP1Triangle(13,14,15,0), /* #<FACE 2537> */
  gsSPVertex(&cont_finish_v[1430],12,4),
  gsDPSetPrimColor(0,0,255,173,0,255),
  gsSP2Triangles(6,7,4,0,5,6,4,0), /* #<FACE 1791> and #<FACE 1878> */
  gsSP2Triangles(6,9,7,0,9,8,7,0), /* #<FACE 1877> and #<FACE 1792> */
  gsSP2Triangles(13,14,10,0,13,11,12,0), /* #<FACE 1818> and #<FACE 1815> */
  gsSP2Triangles(12,0,1,0,14,1,15,0), /* #<FACE 2884> and #<FACE 2883> */
  gsSP2Triangles(1,13,12,0,1,14,13,0), /* #<FACE 1854> and #<FACE 1851> */
  gsSP1Triangle(2,15,1,0), /* #<FACE 515> */
  gsSPVertex(&cont_finish_v[1442],16,0),
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(14,9,3,0,4,9,8,0), /* #<FACE 3356> and #<FACE 1738> */
  gsSP2Triangles(4,3,9,0,1,6,0,0), /* #<FACE 35> and #<FACE 3358> */
  gsSP2Triangles(6,1,2,0,6,8,7,0), /* #<FACE 40> and #<FACE 1928> */
  gsSP2Triangles(0,7,5,0,7,0,6,0), /* #<FACE 1926> and #<FACE 33> */
  gsSP2Triangles(8,9,12,0,8,12,7,0), /* #<FACE 1742> and #<FACE 1741> */
  gsSP2Triangles(9,14,15,0,10,11,15,0), /* #<FACE 3357> and #<FACE 2633> */
  gsSP2Triangles(15,12,9,0,15,13,10,0), /* #<FACE 1927> and #<FACE 1740> */
  gsSP2Triangles(12,15,11,0,15,14,13,0), /* #<FACE 1013> and #<FACE 34> */
  gsSPVertex(&cont_finish_v[1458],11,5),
  gsSP2Triangles(15,5,6,0,6,14,15,0), /* #<FACE 3355> and #<FACE 3354> */
  gsSP2Triangles(14,6,13,0,6,7,9,0), /* #<FACE 1939> and #<FACE 1732> */
  gsSP2Triangles(13,6,9,0,7,6,5,0), /* #<FACE 1730> and #<FACE 38> */
  gsSP2Triangles(0,13,1,0,13,9,1,0), /* #<FACE 1940> and #<FACE 1729> */
  gsSP2Triangles(8,12,9,0,1,9,12,0), /* #<FACE 1938> and #<FACE 1731> */
  gsSP2Triangles(10,2,11,0,12,2,1,0), /* #<FACE 3360> and #<FACE 3353> */
  gsSP1Triangle(12,11,2,0), /* #<FACE 31> */
  gsSPVertex(&cont_finish_v[1469],12,0),
  gsSP2Triangles(5,6,1,0,0,1,6,0), /* #<FACE 1944> and #<FACE 1725> */
  gsSP2Triangles(14,2,15,0,14,13,7,0), /* #<FACE 39> and #<FACE 1008> */
  gsSP2Triangles(13,9,7,0,9,8,7,0), /* #<FACE 2637> and #<FACE 1009> */
  gsSP2Triangles(10,4,11,0,3,11,4,0), /* #<FACE 3359> and #<FACE 32> */
  gsSPVertex(&cont_finish_v[1481],11,5),
  gsSP2Triangles(15,1,5,0,5,1,0,0), /* #<FACE 1946> and #<FACE 1723> */
  gsSP2Triangles(14,15,5,0,6,14,5,0), /* #<FACE 1719> and #<FACE 36> */
  gsSP2Triangles(11,8,2,0,2,8,7,0), /* #<FACE 1948> and #<FACE 1721> */
  gsSP2Triangles(2,10,11,0,9,10,12,0), /* #<FACE 2642> and #<FACE 2641> */
  gsSP2Triangles(3,12,10,0,11,10,9,0), /* #<FACE 999> and #<FACE 998> */
  gsSP2Triangles(3,4,12,0,13,12,4,0), /* #<FACE 1846> and #<FACE 1823> */
  gsSPVertex(&cont_finish_v[1492],14,0),
  gsSP2Triangles(4,6,0,0,7,6,5,0), /* #<FACE 1954> and #<FACE 1734> */
  gsSP2Triangles(1,0,6,0,7,1,6,0), /* #<FACE 1715> and #<FACE 37> */
  gsSP2Triangles(10,9,8,0,13,15,14,0), /* #<FACE 2324> and #<FACE 1950> */
  gsSP2Triangles(14,12,13,0,11,12,3,0), /* #<FACE 2644> and #<FACE 2643> */
  gsSP2Triangles(2,3,12,0,13,12,11,0), /* #<FACE 997> and #<FACE 996> */
  gsSPVertex(&cont_finish_v[1506],12,4),
  gsSP2Triangles(10,11,14,0,4,5,12,0), /* #<FACE 985> and #<FACE 1916> */
  gsSP2Triangles(11,12,5,0,11,5,13,0), /* #<FACE 1753> and #<FACE 986> */
  gsSP2Triangles(15,5,4,0,0,1,6,0), /* #<FACE 203> and #<FACE 1956> */
  gsSP2Triangles(7,6,1,0,2,8,3,0), /* #<FACE 1713> and #<FACE 1952> */
  gsSP1Triangle(9,3,8,0), /* #<FACE 1717> */
  gsSPVertex(&cont_finish_v[1518],10,0),
  gsSP2Triangles(15,2,3,0,4,5,0,0), /* #<FACE 1756> and #<FACE 3349> */
  gsSP2Triangles(5,1,0,0,6,14,4,0), /* #<FACE 45> and #<FACE 1965> */
  gsSP2Triangles(5,4,14,0,5,14,13,0), /* #<FACE 1704> and #<FACE 984> */
  gsSP2Triangles(9,10,7,0,8,10,6,0), /* #<FACE 3186> and #<FACE 1966> */
  gsSP2Triangles(14,6,10,0,10,8,7,0), /* #<FACE 1703> and #<FACE 200> */
  gsSP2Triangles(13,14,11,0,12,11,9,0), /* #<FACE 2651> and #<FACE 1915> */
  gsSP1Triangle(10,9,11,0), /* #<FACE 1754> */
  gsSPVertex(&cont_finish_v[1528],12,4),
  gsSP2Triangles(6,5,4,0,7,8,14,0), /* #<FACE 1835> and #<FACE 2142> */
  gsSP2Triangles(8,15,14,0,9,1,10,0), /* #<FACE 1527> and #<FACE 2654> */
  gsSP2Triangles(1,9,0,0,1,12,10,0), /* #<FACE 1972> and #<FACE 980> */
  gsSP2Triangles(2,13,11,0,13,3,12,0), /* #<FACE 3183> and #<FACE 2653> */
  gsSPVertex(&cont_finish_v[1540],14,0),
  gsSP2Triangles(2,3,13,0,5,13,3,0), /* #<FACE 139> and #<FACE 3246> */
  gsSP1Triangle(13,5,4,0), /* #<FACE 138> */
  gsDPSetPrimColor(0,0,255,0,0,255),
  gsSP2Triangles(6,9,7,0,6,1,9,0), /* #<FACE 2089> and #<FACE 2087> */
  gsSP2Triangles(0,9,1,0,9,8,7,0), /* #<FACE 1582> and #<FACE 1580> */
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(10,14,11,0,11,14,15,0), /* #<FACE 2123> and #<FACE 1546> */
  gsSP1Triangle(2,13,12,0), /* #<FACE 1842> */
  gsSPVertex(&cont_finish_v[1554],10,6),
  gsSP2Triangles(8,7,6,0,11,10,9,0), /* #<FACE 1697> and #<FACE 1937> */
  gsDPSetPrimColor(0,0,255,0,0,255),
  gsSP2Triangles(14,0,15,0,1,15,0,0), /* #<FACE 2126> and #<FACE 1543> */
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(3,2,13,0,3,13,12,0), /* #<FACE 3245> and #<FACE 1572> */
  gsSPVertex(&cont_finish_v[1564],13,0),
  gsDPSetPrimColor(0,0,255,0,0,255),
  gsSP1Triangle(14,15,0,0), /* #<FACE 2139> */
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(1,4,2,0,2,4,3,0), /* #<FACE 2135> and #<FACE 1534> */
  gsSP2Triangles(3,4,6,0,8,6,4,0), /* #<FACE 865> and #<FACE 2699> */
  gsSP2Triangles(5,7,8,0,8,7,6,0), /* #<FACE 2698> and #<FACE 866> */
  gsSP2Triangles(9,10,11,0,12,11,10,0), /* #<FACE 2133> and #<FACE 1536> */
  gsSPVertex(&cont_finish_v[1577],14,2),
  gsSP2Triangles(8,6,2,0,3,8,5,0), /* #<FACE 3208> and #<FACE 3129> */
  gsSP2Triangles(2,5,8,0,5,4,3,0), /* #<FACE 255> and #<FACE 256> */
  gsSP1Triangle(8,7,6,0), /* #<FACE 1489> */
  gsDPSetPrimColor(0,0,230,230,230,255),
  gsSP1Triangle(11,10,9,0), /* #<FACE 359> */
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(12,15,13,0,15,14,13,0), /* #<FACE 174> and #<FACE 133> */
  gsSPVertex(&cont_finish_v[1591],16,0),
  gsSP2Triangles(1,3,0,0,0,3,4,0), /* #<FACE 2291> and #<FACE 1378> */
  gsSP1Triangle(2,3,1,0), /* #<FACE 321> */
  gsDPSetPrimColor(0,0,255,0,0,255),
  gsSP2Triangles(5,9,10,0,5,7,9,0), /* #<FACE 2190> and #<FACE 2188> */
  gsSP2Triangles(9,7,6,0,10,11,8,0), /* #<FACE 1481> and #<FACE 1530> */
  gsSP1Triangle(11,10,9,0), /* #<FACE 1479> */
  gsDPSetPrimColor(0,0,12,12,12,255),
  gsSP2Triangles(15,12,14,0,15,14,13,0), /* #<FACE 257> and #<FACE 136> */
  gsSPVertex(&cont_finish_v[1607],11,5),
  gsSP2Triangles(0,5,1,0,2,1,12,0), /* #<FACE 2740> and #<FACE 2295> */
  gsSP2Triangles(12,1,6,0,6,1,5,0), /* #<FACE 1374> and #<FACE 763> */
  gsSP2Triangles(9,6,8,0,6,9,12,0), /* #<FACE 2739> and #<FACE 2296> */
  gsSP2Triangles(8,7,9,0,11,12,9,0), /* #<FACE 1913> and #<FACE 1373> */
  gsSP2Triangles(15,10,14,0,13,3,2,0), /* #<FACE 1376> and #<FACE 3058> */
  gsSPVertex(&cont_finish_v[1618],9,0),
  gsSP2Triangles(5,4,1,0,0,1,4,0), /* #<FACE 757> and #<FACE 2746> */
  gsSP2Triangles(0,15,1,0,14,5,1,0), /* #<FACE 2745> and #<FACE 759> */
  gsSP2Triangles(15,14,1,0,13,5,14,0), /* #<FACE 758> and #<FACE 2744> */
  gsSP2Triangles(5,13,7,0,2,7,13,0), /* #<FACE 2298> and #<FACE 1371> */
  gsSP2Triangles(7,2,3,0,12,3,2,0), /* #<FACE 760> and #<FACE 2743> */
  gsSP2Triangles(12,11,3,0,6,3,11,0), /* #<FACE 761> and #<FACE 2742> */
  gsSP2Triangles(3,6,7,0,4,5,8,0), /* #<FACE 2741> and #<FACE 2297> */
  gsSP2Triangles(7,8,5,0,8,7,6,0), /* #<FACE 1372> and #<FACE 762> */
  gsSPVertex(&cont_finish_v[1627],15,1),
  gsSP2Triangles(2,5,1,0,4,15,3,0), /* #<FACE 3127> and #<FACE 3252> */
  gsSP2Triangles(7,6,5,0,10,9,8,0), /* #<FACE 3249> and #<FACE 981> */
  gsSP2Triangles(11,12,13,0,14,13,12,0), /* #<FACE 2305> and #<FACE 1364> */
  gsSPVertex(&cont_finish_v[1642],5,0),
  gsSP2Triangles(2,1,0,0,4,3,15,0), /* #<FACE 766> and #<FACE 3210> */
  gsDPPipeSync(),
  gsDPSetCycleType(G_CYC_2CYCLE),
  gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
  gsSPClearGeometryMode((G_SHADE|G_SHADING_SMOOTH|G_LIGHTING|G_TEXTURE_GEN|G_TEXTURE_GEN_LINEAR|G_CULL_BOTH|G_FOG)),
  gsSPSetGeometryMode( G_ZBUFFER | G_CULL_BACK | G_SHADE | G_SHADING_SMOOTH | G_LIGHTING | G_TEXTURE_GEN | G_FOG),
  gsSPTexture (32<<6, 32<<6, 0, G_TX_RENDERTILE, G_ON),
  gsDPSetTexturePersp (G_TP_PERSP),
  gsDPSetTextureDetail (G_TD_CLAMP),
  gsDPSetTextureLOD (G_TL_TILE),
  gsDPSetTextureLUT (G_TT_NONE),
  gsDPLoadTextureTile   (Silver_Reflection, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32,32,
                       0,0,31,31,0,G_TX_WRAP,G_TX_WRAP,
                       5,5,G_TX_NOLOD, G_TX_NOLOD),
  gsDPSetTextureFilter (G_TF_BILERP),
  gsDPSetTextureConvert(G_TC_FILT),
/*  gsSPSetLights1(NWorldLight),*/
  gsSPFogPosition(990, 1000),

  gsDPSetFogColor(0, 0, 0, 255),
  gsDPSetColorDither(G_CD_DISABLE),
/*  gsDPSetColorDither(G_CD_BAYER),*/
  gsDPSetPrimColor(0,0,255,255,255,255),
  gsDPSetCombineMode(G_CC_MODULATERGBA, G_CC_PASS2), /* N64-SHADE-TYPE-E */
  gsSPVertex(&cont_finish_v[1647],15,0),
  gsSP2Triangles(2,3,1,0,2,0,3,0), /* #<FACE 604> and #<FACE 568> */
  gsSP2Triangles(13,9,10,0,4,12,13,0), /* #<FACE 3281> and #<FACE 3280> */
  gsSP2Triangles(4,13,11,0,5,8,13,0), /* #<FACE 3268> and #<FACE 2944> */
  gsSP2Triangles(13,7,6,0,6,5,13,0), /* #<FACE 2921> and #<FACE 501> */
  gsSP2Triangles(14,7,13,0,9,13,8,0), /* #<FACE 463> and #<FACE 445> */
  gsSP2Triangles(11,13,10,0,14,13,12,0), /* #<FACE 117> and #<FACE 105> */
  gsSPVertex(&cont_finish_v[1662],13,3),
  gsSP2Triangles(3,15,14,0,4,3,14,0), /* #<FACE 818> and #<FACE 817> */
  gsSP2Triangles(5,4,14,0,13,5,14,0), /* #<FACE 816> and #<FACE 815> */
  gsSP2Triangles(2,11,10,0,6,0,2,0), /* #<FACE 2923> and #<FACE 2854> */
  gsSP2Triangles(7,6,2,0,8,7,2,0), /* #<FACE 2853> and #<FACE 2852> */
  gsSP2Triangles(9,8,2,0,10,9,2,0), /* #<FACE 2851> and #<FACE 2850> */
  gsSP2Triangles(1,12,2,0,2,12,11,0), /* #<FACE 2496> and #<FACE 1173> */
  gsSPVertex(&cont_finish_v[1675],12,0),
  gsSP2Triangles(5,7,6,0,6,1,3,0), /* #<FACE 415> and #<FACE 274> */
  gsSP2Triangles(2,4,6,0,6,7,0,0), /* #<FACE 269> and #<FACE 264> */
  gsSP2Triangles(8,10,14,0,8,14,9,0), /* #<FACE 2008> and #<FACE 2007> */
  gsSP2Triangles(15,9,14,0,11,14,10,0), /* #<FACE 1662> and #<FACE 1661> */
  gsSP1Triangle(11,13,14,0), /* #<FACE 819> */
  gsSPVertex(&cont_finish_v[1687],8,8),
  gsSP2Triangles(11,15,8,0,8,12,11,0), /* #<FACE 165> and #<FACE 143> */
  gsSP2Triangles(13,14,11,0,1,6,0,0), /* #<FACE 121> and #<FACE 3121> */
  gsSP2Triangles(3,2,6,0,6,4,10,0), /* #<FACE 3115> and #<FACE 3111> */
  gsSP2Triangles(9,5,6,0,10,9,6,0), /* #<FACE 2975> and #<FACE 2974> */
  gsSPVertex(&cont_finish_v[1695],11,0),
  gsSP2Triangles(0,2,4,0,5,1,4,0), /* #<FACE 1620> and #<FACE 915> */
  gsSP2Triangles(6,5,4,0,3,6,4,0), /* #<FACE 914> and #<FACE 913> */
  gsSP2Triangles(11,14,15,0,7,8,11,0), /* #<FACE 3433> and #<FACE 3432> */
  gsSP2Triangles(11,9,7,0,13,11,8,0), /* #<FACE 3279> and #<FACE 3265> */
  gsSP2Triangles(11,12,10,0,11,10,9,0), /* #<FACE 3242> and #<FACE 2924> */
  gsSPVertex(&cont_finish_v[1706],11,5),
  gsSP2Triangles(5,12,11,0,14,15,11,0), /* #<FACE 3364> and #<FACE 3295> */
  gsSP2Triangles(5,11,6,0,11,10,7,0), /* #<FACE 1951> and #<FACE 1949> */
  gsSP2Triangles(6,11,9,0,11,7,8,0), /* #<FACE 1718> and #<FACE 995> */
  gsSP2Triangles(9,11,8,0,10,11,15,0), /* #<FACE 994> and #<FACE 89> */
  gsSP2Triangles(12,14,11,0,4,1,0,0), /* #<FACE 29> and #<FACE 2050> */
  gsSP2Triangles(15,4,2,0,4,13,3,0), /* #<FACE 2049> and #<FACE 2048> */
  gsSP2Triangles(13,4,14,0,15,14,4,0), /* #<FACE 2047> and #<FACE 1622> */
  gsSPVertex(&cont_finish_v[1717],16,0),
  gsSP2Triangles(2,4,5,0,2,1,3,0), /* #<FACE 1985> and #<FACE 968> */
  gsSP2Triangles(4,2,3,0,5,0,2,0), /* #<FACE 967> and #<FACE 76> */
  gsSP2Triangles(14,6,8,0,7,6,14,0), /* #<FACE 3348> and #<FACE 3347> */
  gsSP2Triangles(7,14,15,0,14,8,10,0), /* #<FACE 3203> and #<FACE 1971> */
  gsSP2Triangles(13,14,9,0,12,9,14,0), /* #<FACE 1914> and #<FACE 1755> */
  gsSP2Triangles(14,10,11,0,12,14,11,0), /* #<FACE 983> and #<FACE 982> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 182> */
  gsSPVertex(&cont_finish_v[1733],13,3),
  gsSP2Triangles(4,5,12,0,6,3,12,0), /* #<FACE 2065> and #<FACE 2063> */
  gsSP2Triangles(4,12,3,0,11,12,5,0), /* #<FACE 1606> and #<FACE 1604> */
  gsSP2Triangles(14,12,13,0,12,15,6,0), /* #<FACE 95> and #<FACE 85> */
  gsSP2Triangles(7,2,0,0,10,2,7,0), /* #<FACE 3310> and #<FACE 3309> */
  gsSP2Triangles(2,8,1,0,9,8,2,0), /* #<FACE 3197> and #<FACE 3196> */
  gsSP1Triangle(10,9,2,0), /* #<FACE 3195> */
  gsSPVertex(&cont_finish_v[1746],11,0),
  gsSP2Triangles(0,7,8,0,3,0,8,0), /* #<FACE 3362> and #<FACE 3361> */
  gsSP2Triangles(1,8,5,0,2,8,1,0), /* #<FACE 3328> and #<FACE 3327> */
  gsSP2Triangles(3,8,2,0,8,9,4,0), /* #<FACE 3326> and #<FACE 876> */
  gsSP2Triangles(6,8,4,0,6,5,8,0), /* #<FACE 875> and #<FACE 63> */
  gsSP2Triangles(9,8,7,0,14,15,12,0), /* #<FACE 30> and #<FACE 3299> */
  gsSP2Triangles(10,13,12,0,11,10,12,0), /* #<FACE 3289> and #<FACE 3288> */
  gsSPVertex(&cont_finish_v[1757],16,0),
  gsSP2Triangles(10,12,9,0,9,11,14,0), /* #<FACE 1463> and #<FACE 289> */
  gsSP2Triangles(13,15,9,0,5,1,7,0), /* #<FACE 284> and #<FACE 2134> */
  gsSP2Triangles(0,7,4,0,1,2,7,0), /* #<FACE 1698> and #<FACE 1535> */
  gsSP2Triangles(7,2,3,0,4,7,3,0), /* #<FACE 871> and #<FACE 870> */
  gsSP2Triangles(7,8,5,0,8,7,6,0), /* #<FACE 198> and #<FACE 46> */
  gsSPVertex(&cont_finish_v[1773],9,0),
  gsSP2Triangles(2,5,3,0,4,2,3,0), /* #<FACE 828> and #<FACE 827> */
  gsSP2Triangles(4,1,2,0,5,2,0,0), /* #<FACE 333> and #<FACE 305> */
  gsSP2Triangles(6,9,15,0,8,9,6,0), /* #<FACE 3104> and #<FACE 3103> */
  gsSP2Triangles(13,9,14,0,11,9,12,0), /* #<FACE 3098> and #<FACE 2206> */
  gsSP2Triangles(10,9,7,0,8,7,9,0), /* #<FACE 2204> and #<FACE 1465> */
  gsSPVertex(&cont_finish_v[1782],13,3),
  gsSP2Triangles(3,14,13,0,5,6,14,0), /* #<FACE 3316> and #<FACE 2731> */
  gsSP2Triangles(14,7,8,0,3,4,14,0), /* #<FACE 2275> and #<FACE 2131> */
  gsSP2Triangles(5,14,4,0,7,14,6,0), /* #<FACE 1538> and #<FACE 781> */
  gsSP2Triangles(8,15,14,0,9,0,2,0), /* #<FACE 70> and #<FACE 3081> */
  gsSP2Triangles(10,9,2,0,12,10,2,0), /* #<FACE 3080> and #<FACE 3079> */
  gsSP2Triangles(11,2,1,0,12,2,11,0), /* #<FACE 3049> and #<FACE 3048> */
  gsSPVertex(&cont_finish_v[1795],13,0),
  gsSP2Triangles(0,2,1,0,11,9,3,0), /* #<FACE 190> and #<FACE 3188> */
  gsSP2Triangles(4,9,10,0,4,5,9,0), /* #<FACE 3061> and #<FACE 2292> */
  gsSP2Triangles(5,6,9,0,9,6,7,0), /* #<FACE 1377> and #<FACE 769> */
  gsSP2Triangles(8,9,7,0,11,10,9,0), /* #<FACE 768> and #<FACE 320> */
  gsSP2Triangles(12,14,15,0,12,13,14,0), /* #<FACE 3319> and #<FACE 3317> */
  gsSPVertex(&cont_finish_v[1808],14,2),
  gsSP2Triangles(10,9,2,0,11,10,2,0), /* #<FACE 731> and #<FACE 730> */
  gsSP2Triangles(12,14,10,0,13,15,10,0), /* #<FACE 288> and #<FACE 218> */
  gsSP2Triangles(3,7,1,0,4,3,1,0), /* #<FACE 3093> and #<FACE 3092> */
  gsSP2Triangles(1,8,5,0,1,5,6,0), /* #<FACE 2278> and #<FACE 750> */
  gsSP2Triangles(0,1,6,0,8,1,7,0), /* #<FACE 749> and #<FACE 294> */
  gsSPVertex(&cont_finish_v[1822],9,0),
  gsSP2Triangles(0,5,2,0,6,1,5,0), /* #<FACE 2488> and #<FACE 2486> */
  gsSP2Triangles(1,3,5,0,3,2,5,0), /* #<FACE 1183> and #<FACE 1181> */
  gsSP2Triangles(6,5,4,0,7,10,15,0), /* #<FACE 462> and #<FACE 3167> */
  gsSP2Triangles(13,10,14,0,10,11,12,0), /* #<FACE 3099> and #<FACE 2312> */
  gsSP2Triangles(7,8,10,0,8,9,10,0), /* #<FACE 2310> and #<FACE 1359> */
  gsSPVertex(&cont_finish_v[1831],16,0),
  gsSP2Triangles(1,0,11,0,12,11,0,0), /* #<FACE 3063> and #<FACE 318> */
  gsSP2Triangles(13,14,11,0,11,15,1,0), /* #<FACE 204> and #<FACE 197> */
  gsSP2Triangles(2,8,9,0,3,2,9,0), /* #<FACE 3172> and #<FACE 3171> */
  gsSP2Triangles(9,5,7,0,3,9,4,0), /* #<FACE 3036> and #<FACE 3025> */
  gsSP2Triangles(9,6,4,0,10,5,9,0), /* #<FACE 1330> and #<FACE 354> */
  gsSP2Triangles(7,6,9,0,10,9,8,0), /* #<FACE 352> and #<FACE 214> */
  gsSPVertex(&cont_finish_v[1847],10,0),
  gsSP2Triangles(0,7,5,0,1,5,3,0), /* #<FACE 3096> and #<FACE 3074> */
  gsSP2Triangles(6,2,5,0,4,5,2,0), /* #<FACE 3073> and #<FACE 311> */
  gsSP2Triangles(4,3,5,0,7,6,5,0), /* #<FACE 310> and #<FACE 292> */
  gsSP2Triangles(14,15,11,0,9,13,11,0), /* #<FACE 3189> and #<FACE 3182> */
  gsSP2Triangles(11,12,8,0,9,11,8,0), /* #<FACE 3065> and #<FACE 3064> */
  gsSPVertex(&cont_finish_v[1857],16,0),
  gsSP2Triangles(11,10,13,0,12,10,15,0), /* #<FACE 1449> and #<FACE 1447> */
  gsSP2Triangles(9,14,10,0,0,8,6,0), /* #<FACE 840> and #<FACE 2268> */
  gsSP2Triangles(1,5,8,0,0,3,8,0), /* #<FACE 2267> and #<FACE 2262> */
  gsSP2Triangles(1,8,2,0,4,2,8,0), /* #<FACE 2261> and #<FACE 1408> */
  gsSP2Triangles(4,8,3,0,8,5,7,0), /* #<FACE 1407> and #<FACE 1402> */
  gsSP1Triangle(8,7,6,0), /* #<FACE 1401> */
  gsSPVertex(&cont_finish_v[1873],9,0),
  gsSP2Triangles(2,6,0,0,1,4,6,0), /* #<FACE 3112> and #<FACE 267> */
  gsSP2Triangles(6,3,5,0,7,15,10,0), /* #<FACE 209> and #<FACE 2222> */
  gsSP2Triangles(10,14,7,0,12,13,10,0), /* #<FACE 2221> and #<FACE 2220> */
  gsSP2Triangles(11,8,10,0,8,9,10,0), /* #<FACE 2219> and #<FACE 1450> */
  gsSPVertex(&cont_finish_v[1882],8,8),
  gsSP2Triangles(14,0,13,0,2,10,9,0), /* #<FACE 211> and #<FACE 3436> */
  gsSP2Triangles(2,0,15,0,8,1,2,0), /* #<FACE 3130> and #<FACE 3118> */
  gsSP2Triangles(9,8,2,0,11,2,15,0), /* #<FACE 3117> and #<FACE 253> */
  gsSP2Triangles(2,11,10,0,6,2,1,0), /* #<FACE 210> and #<FACE 3437> */
  gsSP2Triangles(6,4,3,0,12,6,5,0), /* #<FACE 3435> and #<FACE 3177> */
  gsSPVertex(&cont_finish_v[1890],12,0),
  gsSP2Triangles(0,7,8,0,0,5,7,0), /* #<FACE 2176> and #<FACE 2175> */
  gsSP2Triangles(1,3,7,0,4,2,7,0), /* #<FACE 2174> and #<FACE 2173> */
  gsSP2Triangles(7,2,1,0,6,7,3,0), /* #<FACE 1496> and #<FACE 1495> */
  gsSP2Triangles(5,4,7,0,8,7,6,0), /* #<FACE 1494> and #<FACE 1493> */
  gsSP2Triangles(9,14,15,0,9,10,13,0), /* #<FACE 3131> and #<FACE 3114> */
  gsSP1Triangle(11,10,9,0), /* #<FACE 3113> */
  gsSPVertex(&cont_finish_v[1902],15,1),
  gsSP2Triangles(13,6,15,0,13,7,6,0), /* #<FACE 2156> and #<FACE 2155> */
  gsSP2Triangles(2,1,6,0,6,1,5,0), /* #<FACE 2153> and #<FACE 1516> */
  gsSP2Triangles(3,6,4,0,7,2,6,0), /* #<FACE 1515> and #<FACE 1514> */
  gsSP2Triangles(15,6,3,0,6,5,4,0), /* #<FACE 1513> and #<FACE 860> */
  gsSP2Triangles(7,13,14,0,8,7,14,0), /* #<FACE 3447> and #<FACE 3159> */
  gsSP2Triangles(8,14,11,0,14,9,12,0), /* #<FACE 3157> and #<FACE 3044> */
  gsSP2Triangles(14,10,9,0,14,15,10,0), /* #<FACE 1331> and #<FACE 338> */
  gsSP2Triangles(14,12,11,0,15,14,13,0), /* #<FACE 226> and #<FACE 225> */
  gsSPVertex(&cont_finish_v[1917],16,0),
  gsSP2Triangles(3,5,6,0,5,4,1,0), /* #<FACE 300> and #<FACE 217> */
  gsSP2Triangles(2,0,5,0,12,14,15,0), /* #<FACE 177> and #<FACE 2148> */
  gsSP2Triangles(9,14,11,0,7,10,14,0), /* #<FACE 2147> and #<FACE 2146> */
  gsSP2Triangles(7,14,8,0,9,8,14,0), /* #<FACE 2145> and #<FACE 1524> */
  gsSP2Triangles(13,14,10,0,12,11,14,0), /* #<FACE 1523> and #<FACE 1522> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 1521> */
  gsSPVertex(&cont_finish_v[1933],9,7),
  gsSP2Triangles(7,8,13,0,13,8,9,0), /* #<FACE 3180> and #<FACE 206> */
  gsSP2Triangles(14,13,9,0,15,10,13,0), /* #<FACE 180> and #<FACE 172> */
  gsSP2Triangles(11,5,0,0,2,5,1,0), /* #<FACE 3207> and #<FACE 3168> */
  gsSP2Triangles(4,5,3,0,11,12,5,0), /* #<FACE 3086> and #<FACE 2858> */
  gsSP1Triangle(6,5,12,0), /* #<FACE 566> */
  gsSPVertex(&cont_finish_v[1942],12,0),
  gsSP2Triangles(1,2,4,0,0,3,4,0), /* #<FACE 87> and #<FACE 68> */
  gsSP2Triangles(5,10,8,0,6,8,7,0), /* #<FACE 3217> and #<FACE 3179> */
  gsSP2Triangles(7,8,9,0,10,9,8,0), /* #<FACE 207> and #<FACE 166> */
  gsSP2Triangles(15,13,11,0,11,13,14,0), /* #<FACE 3434> and #<FACE 3205> */
  gsSPVertex(&cont_finish_v[1954],11,5),
  gsSP2Triangles(11,10,13,0,9,14,10,0), /* #<FACE 1551> and #<FACE 1550> */
  gsSP2Triangles(12,10,15,0,7,0,4,0), /* #<FACE 1549> and #<FACE 3321> */
  gsSP2Triangles(5,1,4,0,2,8,4,0), /* #<FACE 3297> and #<FACE 3296> */
  gsSP2Triangles(5,4,6,0,3,6,4,0), /* #<FACE 2859> and #<FACE 565> */
  gsSP1Triangle(4,8,7,0), /* #<FACE 88> */
  gsSPVertex(&cont_finish_v[1965],9,0),
  gsSP2Triangles(2,3,1,0,5,4,2,0), /* #<FACE 2925> and #<FACE 2902> */
  gsSP2Triangles(0,6,2,0,2,4,3,0), /* #<FACE 581> and #<FACE 498> */
  gsSP2Triangles(6,5,2,0,7,15,10,0), /* #<FACE 130> and #<FACE 2120> */
  gsSP2Triangles(7,10,14,0,12,13,10,0), /* #<FACE 2119> and #<FACE 2118> */
  gsSP2Triangles(11,8,10,0,8,9,10,0), /* #<FACE 2117> and #<FACE 1552> */
  gsSPVertex(&cont_finish_v[1974],11,5),
  gsSP2Triangles(5,12,13,0,5,10,12,0), /* #<FACE 2086> and #<FACE 2085> */
  gsSP2Triangles(11,12,7,0,8,12,6,0), /* #<FACE 2084> and #<FACE 2083> */
  gsSP2Triangles(9,6,12,0,12,8,7,0), /* #<FACE 1586> and #<FACE 1585> */
  gsSP2Triangles(10,9,12,0,13,12,11,0), /* #<FACE 1584> and #<FACE 1583> */
  gsSP2Triangles(0,2,14,0,1,15,2,0), /* #<FACE 3256> and #<FACE 3255> */
  gsSP1Triangle(15,14,2,0), /* #<FACE 3254> */
  gsSPVertex(&cont_finish_v[1985],16,0),
  gsSP2Triangles(3,0,2,0,1,3,2,0), /* #<FACE 911> and #<FACE 910> */
  gsSP2Triangles(5,4,6,0,8,7,6,0), /* #<FACE 3264> and #<FACE 3212> */
  gsSP2Triangles(13,14,10,0,13,9,15,0), /* #<FACE 3269> and #<FACE 3243> */
  gsSP2Triangles(10,11,13,0,12,13,11,0), /* #<FACE 2857> and #<FACE 567> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 164> */
  gsSPVertex(&cont_finish_v[2001],13,3),
  gsSP2Triangles(13,3,11,0,11,3,4,0), /* #<FACE 1995> and #<FACE 1674> */
  gsSP2Triangles(11,4,12,0,5,14,11,0), /* #<FACE 1673> and #<FACE 1668> */
  gsSP2Triangles(5,11,15,0,2,0,6,0), /* #<FACE 1667> and #<FACE 2054> */
  gsSP2Triangles(6,10,2,0,7,8,2,0), /* #<FACE 2053> and #<FACE 2052> */
  gsSP2Triangles(2,9,7,0,1,2,8,0), /* #<FACE 2051> and #<FACE 1617> */
  gsSP1Triangle(10,9,2,0), /* #<FACE 1616> */
  gsSPVertex(&cont_finish_v[2014],9,0),
  gsSP2Triangles(2,5,6,0,0,4,5,0), /* #<FACE 3334> and #<FACE 3333> */
  gsSP2Triangles(1,5,3,0,3,5,2,0), /* #<FACE 3306> and #<FACE 78> */
  gsSP2Triangles(7,5,4,0,7,6,5,0), /* #<FACE 58> and #<FACE 57> */
  gsSP2Triangles(8,15,11,0,13,11,14,0), /* #<FACE 2002> and #<FACE 2001> */
  gsSP1Triangle(8,11,12,0), /* #<FACE 1996> */
  gsSPVertex(&cont_finish_v[2023],15,1),
  gsSP2Triangles(11,1,12,0,14,1,11,0), /* #<FACE 194> and #<FACE 54> */
  gsSP2Triangles(13,11,15,0,2,10,8,0), /* #<FACE 53> and #<FACE 1906> */
  gsSP2Triangles(9,10,6,0,3,5,10,0), /* #<FACE 1905> and #<FACE 1904> */
  gsSP2Triangles(2,4,10,0,10,4,3,0), /* #<FACE 1903> and #<FACE 1766> */
  gsSP2Triangles(7,10,5,0,7,6,10,0), /* #<FACE 1765> and #<FACE 1764> */
  gsSP1Triangle(10,9,8,0), /* #<FACE 1763> */
  gsSPVertex(&cont_finish_v[2038],11,0),
  gsSP2Triangles(5,3,7,0,6,2,3,0), /* #<FACE 1747> and #<FACE 161> */
  gsSP2Triangles(1,3,5,0,6,3,0,0), /* #<FACE 8> and #<FACE 3> */
  gsSP2Triangles(4,7,3,0,8,15,11,0), /* #<FACE 2> and #<FACE 3336> */
  gsSP2Triangles(13,14,11,0,9,11,12,0), /* #<FACE 3335> and #<FACE 3192> */
  gsSP2Triangles(8,11,10,0,10,11,9,0), /* #<FACE 3191> and #<FACE 195> */
  gsSPVertex(&cont_finish_v[2049],10,6),
  gsSP2Triangles(6,12,13,0,6,10,12,0), /* #<FACE 3278> and #<FACE 3241> */
  gsSP2Triangles(7,9,12,0,7,12,8,0), /* #<FACE 3201> and #<FACE 3134> */
  gsSP2Triangles(11,8,12,0,9,14,12,0), /* #<FACE 3133> and #<FACE 184> */
  gsSP2Triangles(11,12,10,0,14,13,12,0), /* #<FACE 142> and #<FACE 106> */
  gsSP2Triangles(3,1,0,0,15,3,2,0), /* #<FACE 3376> and #<FACE 3223> */
  gsSP1Triangle(4,3,15,0), /* #<FACE 3222> */
  gsSPVertex(&cont_finish_v[2059],16,0),
  gsSP2Triangles(4,6,3,0,2,3,5,0), /* #<FACE 3415> and #<FACE 3414> */
  gsSP2Triangles(1,5,3,0,0,3,6,0), /* #<FACE 3413> and #<FACE 3412> */
  gsSP2Triangles(7,14,15,0,7,12,14,0), /* #<FACE 2226> and #<FACE 2225> */
  gsSP2Triangles(8,14,10,0,8,9,14,0), /* #<FACE 2044> and #<FACE 2043> */
  gsSP2Triangles(9,13,14,0,10,14,11,0), /* #<FACE 1626> and #<FACE 1625> */
  gsSP2Triangles(12,11,14,0,15,14,13,0), /* #<FACE 1444> and #<FACE 1443> */
  gsSPVertex(&cont_finish_v[2075],11,5),
  gsSP2Triangles(5,12,13,0,5,10,12,0), /* #<FACE 3427> and #<FACE 3426> */
  gsSP2Triangles(6,12,8,0,6,7,12,0), /* #<FACE 3425> and #<FACE 3424> */
  gsSP2Triangles(7,11,12,0,8,12,9,0), /* #<FACE 3407> and #<FACE 3406> */
  gsSP2Triangles(10,9,12,0,13,12,11,0), /* #<FACE 3405> and #<FACE 3404> */
  gsSP2Triangles(14,3,0,0,14,1,3,0), /* #<FACE 3419> and #<FACE 3418> */
  gsSP2Triangles(15,3,2,0,15,4,3,0), /* #<FACE 3417> and #<FACE 3416> */
  gsSPVertex(&cont_finish_v[2086],15,0),
  gsSP2Triangles(2,5,3,0,4,2,3,0), /* #<FACE 1025> and #<FACE 1024> */
  gsSP2Triangles(2,4,1,0,5,2,0,0), /* #<FACE 51> and #<FACE 47> */
  gsSP2Triangles(12,8,9,0,6,12,13,0), /* #<FACE 2943> and #<FACE 2846> */
  gsSP2Triangles(7,12,6,0,8,12,7,0), /* #<FACE 2845> and #<FACE 2844> */
  gsSP2Triangles(9,10,12,0,10,11,12,0), /* #<FACE 1872> and #<FACE 1797> */
  gsSP2Triangles(14,12,11,0,14,13,12,0), /* #<FACE 1035> and #<FACE 570> */
  gsSPVertex(&cont_finish_v[2101],12,4),
  gsSP2Triangles(4,11,12,0,6,4,12,0), /* #<FACE 3069> and #<FACE 3068> */
  gsSP2Triangles(5,12,8,0,6,12,5,0), /* #<FACE 3067> and #<FACE 3066> */
  gsSP2Triangles(12,10,7,0,9,12,7,0), /* #<FACE 1021> and #<FACE 1020> */
  gsSP2Triangles(9,8,12,0,12,11,10,0), /* #<FACE 316> and #<FACE 315> */
  gsSP2Triangles(13,0,2,0,15,13,2,0), /* #<FACE 3345> and #<FACE 3344> */
  gsSP2Triangles(14,2,1,0,15,2,14,0), /* #<FACE 3339> and #<FACE 3338> */
  gsSPVertex(&cont_finish_v[2113],16,0),
  gsSP2Triangles(1,0,5,0,5,6,2,0), /* #<FACE 3363> and #<FACE 1953> */
  gsSP2Triangles(5,3,4,0,6,5,4,0), /* #<FACE 991> and #<FACE 990> */
  gsSP2Triangles(8,13,14,0,7,14,11,0), /* #<FACE 3228> and #<FACE 3220> */
  gsSP2Triangles(8,14,7,0,14,10,12,0), /* #<FACE 3219> and #<FACE 1845> */
  gsSP2Triangles(14,15,9,0,10,14,9,0), /* #<FACE 1001> and #<FACE 1000> */
  gsSP2Triangles(14,12,11,0,15,14,13,0), /* #<FACE 163> and #<FACE 156> */
  gsSPVertex(&cont_finish_v[2129],16,0),
  gsSP2Triangles(12,11,0,0,13,12,0,0), /* #<FACE 956> and #<FACE 955> */
  gsSP2Triangles(14,1,12,0,1,15,12,0), /* #<FACE 42> and #<FACE 41> */
  gsSP2Triangles(5,9,10,0,3,7,9,0), /* #<FACE 2658> and #<FACE 2657> */
  gsSP2Triangles(2,9,6,0,2,4,9,0), /* #<FACE 1964> and #<FACE 1963> */
  gsSP2Triangles(4,3,9,0,6,9,5,0), /* #<FACE 1706> and #<FACE 1705> */
  gsSP2Triangles(9,7,8,0,10,9,8,0), /* #<FACE 977> and #<FACE 976> */
  gsSPVertex(&cont_finish_v[2145],11,0),
  gsSP2Triangles(0,7,8,0,0,8,4,0), /* #<FACE 3323> and #<FACE 3313> */
  gsSP2Triangles(8,3,5,0,8,6,1,0), /* #<FACE 2011> and #<FACE 1993> */
  gsSP2Triangles(8,1,2,0,3,8,2,0), /* #<FACE 946> and #<FACE 945> */
  gsSP2Triangles(5,4,8,0,8,7,6,0), /* #<FACE 73> and #<FACE 66> */
  gsSP2Triangles(10,12,15,0,9,14,12,0), /* #<FACE 3352> and #<FACE 3351> */
  gsSP2Triangles(12,13,9,0,10,11,12,0), /* #<FACE 1984> and #<FACE 1665> */
  gsSPVertex(&cont_finish_v[2156],16,0),
  gsSP2Triangles(13,0,15,0,12,0,13,0), /* #<FACE 2830> and #<FACE 2829> */
  gsSP2Triangles(13,15,1,0,13,1,2,0), /* #<FACE 1973> and #<FACE 920> */
  gsSP2Triangles(3,13,2,0,13,3,14,0), /* #<FACE 919> and #<FACE 14> */
  gsSP2Triangles(8,11,9,0,4,11,7,0), /* #<FACE 2678> and #<FACE 2020> */
  gsSP2Triangles(5,6,11,0,10,11,6,0), /* #<FACE 1991> and #<FACE 1678> */
  gsSP2Triangles(8,7,11,0,11,10,9,0), /* #<FACE 1649> and #<FACE 932> */
  gsSPVertex(&cont_finish_v[2172],12,0),
  gsSP2Triangles(15,1,0,0,2,10,8,0), /* #<FACE 171> and #<FACE 3444> */
  gsSP2Triangles(2,8,6,0,7,8,4,0), /* #<FACE 3443> and #<FACE 2090> */
  gsSP2Triangles(9,3,8,0,3,5,8,0), /* #<FACE 2088> and #<FACE 1581> */
  gsSP2Triangles(5,4,8,0,8,7,6,0), /* #<FACE 1579> and #<FACE 124> */
  gsSP2Triangles(10,9,8,0,11,13,14,0), /* #<FACE 123> and #<FACE 3381> */
  gsSP1Triangle(12,13,11,0), /* #<FACE 3380> */
  gsSPVertex(&cont_finish_v[2184],14,2),
  gsSP2Triangles(5,13,4,0,6,2,13,0), /* #<FACE 2189> and #<FACE 2187> */
  gsSP2Triangles(3,13,2,0,4,13,3,0), /* #<FACE 1482> and #<FACE 1480> */
  gsSP2Triangles(13,5,14,0,15,6,13,0), /* #<FACE 266> and #<FACE 265> */
  gsSP2Triangles(10,12,0,0,7,12,9,0), /* #<FACE 3213> and #<FACE 2130> */
  gsSP2Triangles(8,12,7,0,10,9,12,0), /* #<FACE 1696> and #<FACE 1539> */
  gsSP1Triangle(1,12,11,0), /* #<FACE 591> */
  gsSPVertex(&cont_finish_v[2198],12,0),
  gsSP2Triangles(1,0,2,0,4,8,9,0), /* #<FACE 168> and #<FACE 2843> */
  gsSP2Triangles(3,5,9,0,4,9,7,0), /* #<FACE 1685> and #<FACE 1410> */
  gsSP2Triangles(9,5,6,0,7,9,6,0), /* #<FACE 812> and #<FACE 811> */
  gsSP2Triangles(10,9,8,0,11,15,13,0), /* #<FACE 572> and #<FACE 3439> */
  gsSP1Triangle(11,13,14,0), /* #<FACE 3438> */
  gsSPVertex(&cont_finish_v[2210],13,3),
  gsSP2Triangles(13,5,6,0,13,7,3,0), /* #<FACE 2270> and #<FACE 2254> */
  gsSP2Triangles(13,3,4,0,5,13,4,0), /* #<FACE 799> and #<FACE 798> */
  gsSP2Triangles(13,6,14,0,7,13,15,0), /* #<FACE 302> and #<FACE 297> */
  gsSP2Triangles(8,2,0,0,1,2,11,0), /* #<FACE 3216> and #<FACE 3087> */
  gsSP2Triangles(8,9,2,0,10,2,9,0), /* #<FACE 2127> and #<FACE 1542> */
  gsSP1Triangle(12,11,2,0), /* #<FACE 299> */
  gsSPVertex(&cont_finish_v[2223],12,0),
  gsSP2Triangles(1,2,0,0,6,8,9,0), /* #<FACE 590> and #<FACE 2722> */
  gsSP2Triangles(3,9,7,0,4,5,9,0), /* #<FACE 2272> and #<FACE 2245> */
  gsSP2Triangles(6,9,5,0,10,7,9,0), /* #<FACE 1424> and #<FACE 1397> */
  gsSP2Triangles(10,9,8,0,11,15,13,0), /* #<FACE 794> and #<FACE 3089> */
  gsSP1Triangle(11,13,14,0), /* #<FACE 3084> */
  gsSPVertex(&cont_finish_v[2235],12,4),
  gsSP2Triangles(13,11,12,0,11,14,4,0), /* #<FACE 1383> and #<FACE 754> */
  gsSP2Triangles(15,11,4,0,2,8,5,0), /* #<FACE 753> and #<FACE 3072> */
  gsSP2Triangles(7,2,5,0,6,0,2,0), /* #<FACE 3071> and #<FACE 2832> */
  gsSP2Triangles(7,6,2,0,2,10,8,0), /* #<FACE 2831> and #<FACE 2290> */
  gsSP2Triangles(2,1,9,0,10,2,9,0), /* #<FACE 772> and #<FACE 771> */
  gsSPVertex(&cont_finish_v[2247],11,0),
  gsSP2Triangles(2,1,5,0,1,7,3,0), /* #<FACE 3163> and #<FACE 1848> */
  gsSP2Triangles(1,3,4,0,6,1,4,0), /* #<FACE 740> and #<FACE 739> */
  gsSP2Triangles(6,5,1,0,7,1,0,0), /* #<FACE 221> and #<FACE 212> */
  gsSP2Triangles(10,11,15,0,13,14,11,0), /* #<FACE 2750> and #<FACE 2749> */
  gsSP2Triangles(8,12,11,0,8,11,9,0), /* #<FACE 2286> and #<FACE 2285> */
  gsSP1Triangle(10,9,11,0), /* #<FACE 1384> */
  gsSPVertex(&cont_finish_v[2258],13,3),
  gsSP2Triangles(15,3,13,0,3,4,13,0), /* #<FACE 2498> and #<FACE 1171> */
  gsSP2Triangles(4,14,13,0,6,10,5,0), /* #<FACE 1169> and #<FACE 3166> */
  gsSP2Triangles(10,7,8,0,10,8,9,0), /* #<FACE 2308> and #<FACE 735> */
  gsSP2Triangles(11,10,9,0,12,0,1,0), /* #<FACE 734> and #<FACE 3176> */
  gsSP1Triangle(2,12,1,0), /* #<FACE 3175> */
  gsSPVertex(&cont_finish_v[2271],13,0),
  gsSP2Triangles(3,4,2,0,1,4,0,0), /* #<FACE 1094> and #<FACE 580> */
  gsSP2Triangles(10,5,6,0,11,10,8,0), /* #<FACE 3270> and #<FACE 2510> */
  gsSP2Triangles(6,7,10,0,7,9,10,0), /* #<FACE 2508> and #<FACE 1161> */
  gsSP2Triangles(9,8,10,0,10,11,13,0), /* #<FACE 1159> and #<FACE 2856> */
  gsSP2Triangles(15,13,11,0,12,13,14,0), /* #<FACE 2855> and #<FACE 2500> */
  gsSPVertex(&cont_finish_v[2284],11,5),
  gsSP2Triangles(13,15,14,0,5,8,11,0), /* #<FACE 337> and #<FACE 2836> */
  gsSP2Triangles(6,5,11,0,1,6,11,0), /* #<FACE 2835> and #<FACE 2834> */
  gsSP2Triangles(2,11,7,0,11,9,7,0), /* #<FACE 1088> and #<FACE 598> */
  gsSP2Triangles(9,11,8,0,0,4,10,0), /* #<FACE 579> and #<FACE 2901> */
  gsSP2Triangles(11,4,1,0,4,11,2,0), /* #<FACE 2833> and #<FACE 2581> */
  gsSP1Triangle(12,4,3,0), /* #<FACE 2575> */
  gsSPVertex(&cont_finish_v[2295],12,0),
  gsSP2Triangles(4,2,1,0,6,4,0,0), /* #<FACE 1912> and #<FACE 1911> */
  gsSP2Triangles(1,0,4,0,3,2,4,0), /* #<FACE 1758> and #<FACE 1022> */
  gsSP2Triangles(6,5,4,0,7,11,13,0), /* #<FACE 1019> and #<FACE 3039> */
  gsSP2Triangles(13,8,7,0,9,8,13,0), /* #<FACE 3038> and #<FACE 3037> */
  gsSP2Triangles(10,15,13,0,13,14,9,0), /* #<FACE 3029> and #<FACE 351> */
  gsSP1Triangle(11,10,13,0), /* #<FACE 342> */
  gsSPVertex(&cont_finish_v[2307],16,0),
  gsSP2Triangles(15,14,0,0,4,2,6,0), /* #<FACE 1379> and #<FACE 1910> */
  gsSP2Triangles(3,1,6,0,1,8,6,0), /* #<FACE 1909> and #<FACE 1760> */
  gsSP2Triangles(3,6,2,0,5,4,6,0), /* #<FACE 1759> and #<FACE 1029> */
  gsSP2Triangles(7,5,6,0,8,7,6,0), /* #<FACE 1028> and #<FACE 1027> */
  gsSP2Triangles(9,11,10,0,10,11,12,0), /* #<FACE 3070> and #<FACE 314> */
  gsSP1Triangle(13,12,11,0), /* #<FACE 313> */
  gsSPVertex(&cont_finish_v[2323],14,0),
  gsSP2Triangles(1,2,3,0,0,3,2,0), /* #<FACE 1405> and #<FACE 809> */
  gsSP2Triangles(4,7,8,0,4,8,13,0), /* #<FACE 2288> and #<FACE 2287> */
  gsSP2Triangles(5,8,9,0,6,13,8,0), /* #<FACE 1423> and #<FACE 1382> */
  gsSP2Triangles(9,8,7,0,15,13,14,0), /* #<FACE 1381> and #<FACE 2289> */
  gsSP2Triangles(10,14,11,0,12,11,14,0), /* #<FACE 2129> and #<FACE 1540> */
  gsSP1Triangle(13,12,14,0), /* #<FACE 1380> */
  gsSPVertex(&cont_finish_v[2337],13,3),
  gsSP2Triangles(12,15,13,0,14,11,12,0), /* #<FACE 1433> and #<FACE 833> */
  gsSP2Triangles(3,5,8,0,3,8,4,0), /* #<FACE 1844> and #<FACE 1843> */
  gsSP2Triangles(6,4,8,0,10,8,5,0), /* #<FACE 1826> and #<FACE 1825> */
  gsSP2Triangles(1,6,8,0,2,7,0,0), /* #<FACE 810> and #<FACE 2266> */
  gsSP2Triangles(2,9,7,0,8,2,1,0), /* #<FACE 2265> and #<FACE 2264> */
  gsSP2Triangles(8,10,2,0,10,9,2,0), /* #<FACE 2263> and #<FACE 1406> */
  gsSPVertex(&cont_finish_v[2350],11,0),
  gsSP2Triangles(6,7,3,0,5,1,7,0), /* #<FACE 2231> and #<FACE 2228> */
  gsSP2Triangles(4,7,0,0,2,0,7,0), /* #<FACE 2227> and #<FACE 1442> */
  gsSP2Triangles(2,7,1,0,4,3,7,0), /* #<FACE 1441> and #<FACE 1438> */
  gsSP2Triangles(7,6,5,0,15,12,10,0), /* #<FACE 1437> and #<FACE 2235> */
  gsSP2Triangles(8,12,9,0,12,8,14,0), /* #<FACE 2234> and #<FACE 2233> */
  gsSP2Triangles(9,12,13,0,11,10,12,0), /* #<FACE 1435> and #<FACE 1434> */
  gsSPVertex(&cont_finish_v[2361],15,1),
  gsSP2Triangles(6,1,4,0,1,6,7,0), /* #<FACE 2214> and #<FACE 2213> */
  gsSP2Triangles(2,6,5,0,2,3,6,0), /* #<FACE 2212> and #<FACE 2211> */
  gsSP2Triangles(3,8,6,0,5,6,4,0), /* #<FACE 1458> and #<FACE 1457> */
  gsSP2Triangles(8,7,6,0,15,13,11,0), /* #<FACE 1456> and #<FACE 2224> */
  gsSP2Triangles(13,9,12,0,10,9,13,0), /* #<FACE 2223> and #<FACE 1448> */
  gsSP2Triangles(13,12,11,0,15,14,13,0), /* #<FACE 1445> and #<FACE 839> */
  gsSPVertex(&cont_finish_v[2376],15,0),
  gsSP2Triangles(2,6,3,0,5,6,4,0), /* #<FACE 2198> and #<FACE 2197> */
  gsSP2Triangles(2,4,6,0,3,6,1,0), /* #<FACE 1472> and #<FACE 1471> */
  gsSP2Triangles(5,7,6,0,0,11,10,0), /* #<FACE 1470> and #<FACE 2202> */
  gsSP2Triangles(0,1,11,0,11,6,7,0), /* #<FACE 2200> and #<FACE 2199> */
  gsSP2Triangles(6,11,1,0,7,9,11,0), /* #<FACE 1469> and #<FACE 843> */
  gsSP2Triangles(12,13,8,0,11,9,12,0), /* #<FACE 2203> and #<FACE 2201> */
  gsSP2Triangles(9,14,12,0,11,12,10,0), /* #<FACE 1468> and #<FACE 1467> */
  gsSP1Triangle(14,13,12,0), /* #<FACE 842> */
  gsSPVertex(&cont_finish_v[2391],12,4),
  gsSP2Triangles(5,7,8,0,13,7,4,0), /* #<FACE 3147> and #<FACE 3146> */
  gsSP2Triangles(0,7,1,0,7,0,6,0), /* #<FACE 3106> and #<FACE 3105> */
  gsSP2Triangles(1,7,13,0,5,4,7,0), /* #<FACE 282> and #<FACE 238> */
  gsSP2Triangles(8,7,6,0,2,3,12,0), /* #<FACE 237> and #<FACE 2196> */
  gsSP2Triangles(12,10,2,0,9,12,15,0), /* #<FACE 2195> and #<FACE 2194> */
  gsSP2Triangles(9,11,12,0,11,10,12,0), /* #<FACE 2193> and #<FACE 1476> */
  gsSP2Triangles(15,12,14,0,14,12,3,0), /* #<FACE 1475> and #<FACE 1473> */
  gsSPVertex(&cont_finish_v[2403],13,0),
  gsSP2Triangles(0,3,4,0,1,2,3,0), /* #<FACE 3408> and #<FACE 3398> */
  gsSP2Triangles(7,6,10,0,5,15,10,0), /* #<FACE 3144> and #<FACE 3109> */
  gsSP2Triangles(14,10,15,0,6,5,10,0), /* #<FACE 279> and #<FACE 241> */
  gsSP2Triangles(7,10,8,0,11,8,10,0), /* #<FACE 240> and #<FACE 3145> */
  gsSP2Triangles(9,11,14,0,9,13,11,0), /* #<FACE 3108> and #<FACE 281> */
  gsSP2Triangles(14,11,10,0,12,11,13,0), /* #<FACE 280> and #<FACE 239> */
  gsSPVertex(&cont_finish_v[2416],11,5),
  gsSP2Triangles(5,10,11,0,12,10,9,0), /* #<FACE 3140> and #<FACE 3139> */
  gsSP2Triangles(6,7,10,0,6,10,5,0), /* #<FACE 3137> and #<FACE 251> */
  gsSP2Triangles(7,8,10,0,9,10,8,0), /* #<FACE 250> and #<FACE 247> */
  gsSP2Triangles(12,11,10,0,13,3,0,0), /* #<FACE 246> and #<FACE 3423> */
  gsSP2Triangles(3,13,1,0,14,15,3,0), /* #<FACE 3422> and #<FACE 3421> */
  gsSP2Triangles(3,2,14,0,4,3,15,0), /* #<FACE 3420> and #<FACE 3410> */
  gsSPVertex(&cont_finish_v[2427],14,0),
  gsSP2Triangles(4,1,5,0,5,0,2,0), /* #<FACE 1902> and #<FACE 1901> */
  gsSP2Triangles(6,3,5,0,6,5,8,0), /* #<FACE 1900> and #<FACE 1899> */
  gsSP2Triangles(2,8,5,0,4,5,3,0), /* #<FACE 1770> and #<FACE 1769> */
  gsSP2Triangles(0,5,1,0,9,13,11,0), /* #<FACE 1767> and #<FACE 2168> */
  gsSP2Triangles(13,7,12,0,6,13,10,0), /* #<FACE 2167> and #<FACE 1898> */
  gsSP2Triangles(6,8,13,0,8,7,13,0), /* #<FACE 1897> and #<FACE 1772> */
  gsSP2Triangles(10,13,9,0,13,12,11,0), /* #<FACE 1771> and #<FACE 1501> */
  gsSPVertex(&cont_finish_v[2441],14,2),
  gsSP2Triangles(5,12,11,0,5,13,12,0), /* #<FACE 2164> and #<FACE 2163> */
  gsSP2Triangles(2,13,5,0,3,2,5,0), /* #<FACE 848> and #<FACE 847> */
  gsSP2Triangles(4,3,5,0,6,4,5,0), /* #<FACE 846> and #<FACE 845> */
  gsSP2Triangles(11,6,5,0,9,10,14,0), /* #<FACE 844> and #<FACE 2172> */
  gsSP2Triangles(0,1,10,0,0,10,7,0), /* #<FACE 2170> and #<FACE 2169> */
  gsSP2Triangles(8,7,10,0,15,10,1,0), /* #<FACE 1500> and #<FACE 1499> */
  gsSP2Triangles(9,8,10,0,14,10,15,0), /* #<FACE 1498> and #<FACE 1497> */
  gsSPVertex(&cont_finish_v[2455],9,0),
  gsSP2Triangles(0,4,15,0,14,4,2,0), /* #<FACE 3152> and #<FACE 3151> */
  gsSP2Triangles(3,4,1,0,3,2,4,0), /* #<FACE 343> and #<FACE 234> */
  gsSP2Triangles(4,14,15,0,6,12,5,0), /* #<FACE 233> and #<FACE 2159> */
  gsSP2Triangles(7,5,12,0,12,6,8,0), /* #<FACE 1510> and #<FACE 1509> */
  gsSP2Triangles(13,7,12,0,11,12,8,0), /* #<FACE 1506> and #<FACE 1505> */
  gsSPVertex(&cont_finish_v[2464],16,0),
  gsSP2Triangles(12,0,13,0,13,14,15,0), /* #<FACE 162> and #<FACE 75> */
  gsSP2Triangles(2,5,6,0,1,5,7,0), /* #<FACE 3173> and #<FACE 3091> */
  gsSP2Triangles(5,2,3,0,5,3,7,0), /* #<FACE 3035> and #<FACE 295> */
  gsSP2Triangles(6,5,4,0,9,8,7,0), /* #<FACE 213> and #<FACE 2240> */
  gsSP2Triangles(8,9,10,0,11,10,9,0), /* #<FACE 2239> and #<FACE 1430> */
  gsSPVertex(&cont_finish_v[2480],11,0),
  gsSP2Triangles(3,1,4,0,4,2,0,0), /* #<FACE 1558> and #<FACE 1557> */
  gsSP2Triangles(5,6,7,0,7,14,5,0), /* #<FACE 2026> and #<FACE 2025> */
  gsSP2Triangles(8,7,6,0,13,15,10,0), /* #<FACE 1643> and #<FACE 3378> */
  gsSP2Triangles(9,14,13,0,10,12,13,0), /* #<FACE 3311> and #<FACE 3221> */
  gsSPVertex(&cont_finish_v[2491],9,7),
  gsSP2Triangles(11,8,13,0,11,12,9,0), /* #<FACE 3370> and #<FACE 3367> */
  gsSP2Triangles(7,10,11,0,7,11,15,0), /* #<FACE 3304> and #<FACE 3303> */
  gsSP2Triangles(11,9,8,0,13,15,11,0), /* #<FACE 1746> and #<FACE 82> */
  gsSP2Triangles(12,11,10,0,14,4,0,0), /* #<FACE 81> and #<FACE 2112> */
  gsSP2Triangles(2,4,1,0,13,4,15,0), /* #<FACE 2111> and #<FACE 2110> */
  gsSP2Triangles(13,3,4,0,15,4,14,0), /* #<FACE 1560> and #<FACE 1559> */
  gsSPVertex(&cont_finish_v[2500],16,0),
  gsSP2Triangles(3,5,6,0,1,6,2,0), /* #<FACE 2104> and #<FACE 2102> */
  gsSP2Triangles(1,0,6,0,0,4,6,0), /* #<FACE 2101> and #<FACE 1568> */
  gsSP2Triangles(2,6,7,0,6,4,3,0), /* #<FACE 1567> and #<FACE 1566> */
  gsSP2Triangles(7,6,5,0,8,13,14,0), /* #<FACE 1565> and #<FACE 3375> */
  gsSP2Triangles(13,8,9,0,10,12,13,0), /* #<FACE 3373> and #<FACE 3275> */
  gsSP2Triangles(9,11,13,0,13,11,10,0), /* #<FACE 3273> and #<FACE 110> */
  gsSP2Triangles(15,13,12,0,15,14,13,0), /* #<FACE 109> and #<FACE 19> */
  gsSPVertex(&cont_finish_v[2516],12,4),
  gsSP2Triangles(11,9,8,0,5,6,9,0), /* #<FACE 3237> and #<FACE 3235> */
  gsSP2Triangles(10,4,9,0,5,9,4,0), /* #<FACE 3234> and #<FACE 149> */
  gsSP2Triangles(6,7,9,0,8,9,7,0), /* #<FACE 148> and #<FACE 145> */
  gsSP2Triangles(11,10,9,0,12,0,1,0), /* #<FACE 144> and #<FACE 899> */
  gsSP2Triangles(13,12,1,0,14,13,1,0), /* #<FACE 898> and #<FACE 897> */
  gsSP2Triangles(15,14,1,0,2,15,1,0), /* #<FACE 896> and #<FACE 895> */
  gsSPVertex(&cont_finish_v[2528],15,0),
  gsSP2Triangles(2,6,0,0,3,6,4,0), /* #<FACE 3337> and #<FACE 3283> */
  gsSP2Triangles(1,4,6,0,1,6,2,0), /* #<FACE 100> and #<FACE 99> */
  gsSP2Triangles(6,5,0,0,12,7,14,0), /* #<FACE 52> and #<FACE 3431> */
  gsSP2Triangles(7,12,10,0,12,13,8,0), /* #<FACE 3430> and #<FACE 3429> */
  gsSP2Triangles(8,9,12,0,9,11,12,0), /* #<FACE 3428> and #<FACE 3403> */
  gsSP2Triangles(11,10,12,0,14,13,12,0), /* #<FACE 3401> and #<FACE 3395> */
  gsSPVertex(&cont_finish_v[2543],11,5),
  gsSP2Triangles(10,6,5,0,5,11,10,0), /* #<FACE 3287> and #<FACE 3286> */
  gsSP2Triangles(12,8,10,0,9,7,10,0), /* #<FACE 3231> and #<FACE 3230> */
  gsSP2Triangles(7,6,10,0,9,10,8,0), /* #<FACE 154> and #<FACE 153> */
  gsSP2Triangles(11,12,10,0,0,15,2,0), /* #<FACE 97> and #<FACE 3342> */
  gsSP2Triangles(13,1,2,0,14,12,2,0), /* #<FACE 3284> and #<FACE 152> */
  gsSP2Triangles(13,2,12,0,15,14,2,0), /* #<FACE 98> and #<FACE 49> */
  gsSPVertex(&cont_finish_v[2554],14,0),
  gsSP2Triangles(5,4,6,0,3,5,7,0), /* #<FACE 1896> and #<FACE 1895> */
  gsSP2Triangles(0,1,5,0,0,5,2,0), /* #<FACE 1894> and #<FACE 1893> */
  gsSP2Triangles(3,2,5,0,4,5,1,0), /* #<FACE 1776> and #<FACE 1775> */
  gsSP2Triangles(6,7,5,0,13,10,12,0), /* #<FACE 1774> and #<FACE 2081> */
  gsSP2Triangles(6,8,12,0,6,12,7,0), /* #<FACE 2080> and #<FACE 2079> */
  gsSP2Triangles(9,7,12,0,11,12,8,0), /* #<FACE 1590> and #<FACE 1589> */
  gsSP2Triangles(10,9,12,0,13,12,11,0), /* #<FACE 1588> and #<FACE 1587> */
  gsSPVertex(&cont_finish_v[2568],14,2),
  gsSP2Triangles(2,6,7,0,2,4,6,0), /* #<FACE 2076> and #<FACE 2075> */
  gsSP2Triangles(6,14,5,0,14,6,15,0), /* #<FACE 2074> and #<FACE 2073> */
  gsSP2Triangles(3,15,6,0,4,3,6,0), /* #<FACE 1596> and #<FACE 1594> */
  gsSP2Triangles(7,6,5,0,13,11,10,0), /* #<FACE 1593> and #<FACE 2030> */
  gsSP2Triangles(8,12,13,0,0,13,1,0), /* #<FACE 2029> and #<FACE 1892> */
  gsSP2Triangles(0,9,13,0,9,8,13,0), /* #<FACE 1891> and #<FACE 1778> */
  gsSP2Triangles(1,13,10,0,13,12,11,0), /* #<FACE 1777> and #<FACE 1640> */
  gsSPVertex(&cont_finish_v[2582],13,0),
  gsSP2Triangles(4,0,2,0,6,4,1,0), /* #<FACE 2066> and #<FACE 1601> */
  gsSP2Triangles(2,1,4,0,3,7,10,0), /* #<FACE 901> and #<FACE 2069> */
  gsSP2Triangles(7,4,6,0,3,5,7,0), /* #<FACE 2068> and #<FACE 2067> */
  gsSP2Triangles(7,5,4,0,6,8,7,0), /* #<FACE 1602> and #<FACE 900> */
  gsSP2Triangles(9,11,12,0,14,15,12,0), /* #<FACE 2072> and #<FACE 2071> */
  gsSP2Triangles(7,8,12,0,12,10,7,0), /* #<FACE 2070> and #<FACE 1600> */
  gsSP2Triangles(9,12,8,0,15,10,12,0), /* #<FACE 1599> and #<FACE 1598> */
  gsSP1Triangle(14,12,11,0), /* #<FACE 1597> */
  gsSPVertex(&cont_finish_v[2595],14,2),
  gsSP2Triangles(2,15,13,0,3,2,13,0), /* #<FACE 906> and #<FACE 905> */
  gsSP2Triangles(14,3,13,0,14,8,4,0), /* #<FACE 904> and #<FACE 2059> */
  gsSP2Triangles(9,4,8,0,8,14,6,0), /* #<FACE 1610> and #<FACE 1609> */
  gsSP2Triangles(5,10,8,0,6,5,8,0), /* #<FACE 903> and #<FACE 902> */
  gsSP2Triangles(11,12,0,0,8,10,11,0), /* #<FACE 2064> and #<FACE 2062> */
  gsSP2Triangles(7,9,11,0,11,9,8,0), /* #<FACE 2061> and #<FACE 1608> */
  gsSP1Triangle(12,11,10,0), /* #<FACE 1607> */
  gsSPVertex(&cont_finish_v[2609],13,0),
  gsSP2Triangles(0,2,1,0,7,6,3,0), /* #<FACE 912> and #<FACE 1615> */
  gsSP2Triangles(8,5,6,0,5,4,6,0), /* #<FACE 1614> and #<FACE 909> */
  gsSP2Triangles(6,7,10,0,10,8,6,0), /* #<FACE 2056> and #<FACE 2055> */
  gsSP2Triangles(9,10,7,0,15,8,10,0), /* #<FACE 1613> and #<FACE 1612> */
  gsSP2Triangles(12,10,9,0,13,10,12,0), /* #<FACE 1611> and #<FACE 2058> */
  gsSP2Triangles(13,15,10,0,11,14,13,0), /* #<FACE 2057> and #<FACE 908> */
  gsSP1Triangle(12,11,13,0), /* #<FACE 907> */
  gsSPVertex(&cont_finish_v[2622],14,2),
  gsSP2Triangles(2,14,13,0,2,13,15,0), /* #<FACE 1628> and #<FACE 1627> */
  gsSP2Triangles(7,3,5,0,8,4,7,0), /* #<FACE 2046> and #<FACE 2045> */
  gsSP2Triangles(7,4,3,0,6,7,5,0), /* #<FACE 1624> and #<FACE 1621> */
  gsSP2Triangles(9,8,7,0,0,1,10,0), /* #<FACE 916> and #<FACE 1619> */
  gsSP1Triangle(12,11,1,0), /* #<FACE 1618> */
  gsSPVertex(&cont_finish_v[2636],12,0),
  gsSP2Triangles(5,6,0,0,0,4,5,0), /* #<FACE 2036> and #<FACE 2035> */
  gsSP2Triangles(1,2,5,0,5,3,1,0), /* #<FACE 2032> and #<FACE 1638> */
  gsSP2Triangles(7,5,2,0,4,3,5,0), /* #<FACE 1637> and #<FACE 1634> */
  gsSP2Triangles(7,6,5,0,11,15,13,0), /* #<FACE 917> and #<FACE 2042> */
  gsSP2Triangles(9,13,14,0,8,10,13,0), /* #<FACE 2041> and #<FACE 2038> */
  gsSP2Triangles(13,9,8,0,11,13,10,0), /* #<FACE 1632> and #<FACE 1631> */
  gsSPVertex(&cont_finish_v[2648],15,1),
  gsSP2Triangles(1,6,5,0,1,3,6,0), /* #<FACE 2000> and #<FACE 1999> */
  gsSP2Triangles(6,2,4,0,6,7,2,0), /* #<FACE 1998> and #<FACE 1997> */
  gsSP2Triangles(3,8,6,0,5,6,4,0), /* #<FACE 1670> and #<FACE 1669> */
  gsSP2Triangles(8,7,6,0,9,15,13,0), /* #<FACE 958> and #<FACE 953> */
  gsSP2Triangles(10,9,13,0,11,10,13,0), /* #<FACE 952> and #<FACE 951> */
  gsSP2Triangles(14,12,13,0,15,14,13,0), /* #<FACE 950> and #<FACE 949> */
  gsSPVertex(&cont_finish_v[2663],16,0),
  gsSP2Triangles(0,3,1,0,3,2,1,0), /* #<FACE 3190> and #<FACE 196> */
  gsSP2Triangles(4,6,7,0,5,7,8,0), /* #<FACE 3377> and #<FACE 55> */
  gsSP2Triangles(8,7,6,0,9,11,13,0), /* #<FACE 17> and #<FACE 1976> */
  gsSP2Triangles(9,13,10,0,14,10,13,0), /* #<FACE 1975> and #<FACE 1694> */
  gsSP2Triangles(12,13,11,0,15,14,13,0), /* #<FACE 1693> and #<FACE 1650> */
  gsSPVertex(&cont_finish_v[2679],16,0),
  gsSP2Triangles(0,11,12,0,1,6,4,0), /* #<FACE 1030> and #<FACE 3446> */
  gsSP2Triangles(2,6,1,0,5,6,2,0), /* #<FACE 3386> and #<FACE 3385> */
  gsSP2Triangles(4,6,3,0,7,6,5,0), /* #<FACE 3374> and #<FACE 20> */
  gsSP2Triangles(10,9,8,0,14,13,9,0), /* #<FACE 1757> and #<FACE 1026> */
  gsSP1Triangle(10,15,9,0), /* #<FACE 1023> */
  gsSPVertex(&cont_finish_v[2695],9,0),
  gsSP2Triangles(8,15,0,0,1,3,13,0), /* #<FACE 3346> and #<FACE 3181> */
  gsSP2Triangles(13,2,1,0,14,2,13,0), /* #<FACE 3020> and #<FACE 3019> */
  gsSP2Triangles(4,13,3,0,7,6,5,0), /* #<FACE 205> and #<FACE 312> */
  gsSP1Triangle(11,8,12,0), /* #<FACE 1031> */
  gsSPVertex(&cont_finish_v[2704],16,0),
  gsSP2Triangles(14,13,11,0,15,11,12,0), /* #<FACE 158> and #<FACE 157> */
  gsSP2Triangles(0,4,5,0,2,4,1,0), /* #<FACE 3292> and #<FACE 1824> */
  gsSP2Triangles(4,2,3,0,6,4,3,0), /* #<FACE 1005> and #<FACE 1004> */
  gsSP2Triangles(6,5,4,0,9,7,8,0), /* #<FACE 92> and #<FACE 3060> */
  gsSP1Triangle(10,9,8,0), /* #<FACE 3059> */
  gsSPVertex(&cont_finish_v[2720],10,0),
  gsSP2Triangles(3,4,1,0,0,4,2,0), /* #<FACE 989> and #<FACE 183> */
  gsSP2Triangles(5,13,14,0,8,13,6,0), /* #<FACE 3226> and #<FACE 1716> */
  gsSP2Triangles(14,11,15,0,13,8,11,0), /* #<FACE 3227> and #<FACE 1955> */
  gsSP2Triangles(12,11,7,0,7,11,9,0), /* #<FACE 1947> and #<FACE 1722> */
  gsSP1Triangle(9,11,8,0), /* #<FACE 1714> */
  gsSPVertex(&cont_finish_v[2730],11,5),
  gsSP2Triangles(6,10,15,0,9,10,7,0), /* #<FACE 2660> and #<FACE 1908> */
  gsSP2Triangles(8,10,5,0,6,5,10,0), /* #<FACE 1907> and #<FACE 1762> */
  gsSP2Triangles(10,8,7,0,10,9,14,0), /* #<FACE 1761> and #<FACE 975> */
  gsSP2Triangles(15,10,14,0,13,4,0,0), /* #<FACE 974> and #<FACE 3202> */
  gsSP2Triangles(12,1,4,0,2,4,11,0), /* #<FACE 2647> and #<FACE 1920> */
  gsSP2Triangles(3,11,4,0,4,13,12,0), /* #<FACE 1749> and #<FACE 1708> */
  gsSPVertex(&cont_finish_v[2741],13,0),
  gsSP2Triangles(8,9,4,0,6,10,8,0), /* #<FACE 2663> and #<FACE 1992> */
  gsSP2Triangles(4,1,8,0,8,1,7,0), /* #<FACE 1990> and #<FACE 1679> */
  gsSP2Triangles(10,9,8,0,5,12,3,0), /* #<FACE 1677> and #<FACE 3330> */
  gsSP2Triangles(12,11,0,0,12,2,15,0), /* #<FACE 1979> and #<FACE 1977> */
  gsSP2Triangles(12,15,14,0,11,12,14,0), /* #<FACE 973> and #<FACE 972> */
  gsSP2Triangles(12,5,2,0,0,3,12,0), /* #<FACE 62> and #<FACE 61> */
  gsSPVertex(&cont_finish_v[2754],8,8),
  gsSP2Triangles(10,9,8,0,3,12,1,0), /* #<FACE 3320> and #<FACE 2666> */
  gsSP2Triangles(0,11,3,0,12,3,11,0), /* #<FACE 1987> and #<FACE 1682> */
  gsSP2Triangles(4,3,1,0,13,2,5,0), /* #<FACE 1680> and #<FACE 3329> */
  gsSP2Triangles(5,3,4,0,5,15,13,0), /* #<FACE 1989> and #<FACE 1986> */
  gsSP2Triangles(5,4,14,0,15,5,14,0), /* #<FACE 965> and #<FACE 964> */
  gsSPVertex(&cont_finish_v[2762],16,0),
  gsSP2Triangles(5,3,6,0,3,7,6,0), /* #<FACE 2009> and #<FACE 1660> */
  gsSP2Triangles(3,8,7,0,8,3,4,0), /* #<FACE 72> and #<FACE 65> */
  gsSP2Triangles(15,13,9,0,1,2,15,0), /* #<FACE 2668> and #<FACE 2006> */
  gsSP2Triangles(9,10,15,0,15,10,1,0), /* #<FACE 2005> and #<FACE 1664> */
  gsSP2Triangles(0,15,2,0,0,14,15,0), /* #<FACE 1663> and #<FACE 2669> */
  gsSP2Triangles(11,14,12,0,0,12,14,0), /* #<FACE 1983> and #<FACE 1686> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 954> */
  gsSPVertex(&cont_finish_v[2778],10,6),
  gsSP2Triangles(0,2,6,0,7,6,2,0), /* #<FACE 3318> and #<FACE 3315> */
  gsSP2Triangles(2,1,9,0,7,2,8,0), /* #<FACE 2672> and #<FACE 1994> */
  gsSP2Triangles(9,8,2,0,12,11,10,0), /* #<FACE 1675> and #<FACE 2842> */
  gsSP2Triangles(14,13,12,0,15,4,3,0), /* #<FACE 2004> and #<FACE 3324> */
  gsSPVertex(&cont_finish_v[2788],16,0),
  gsSP2Triangles(3,4,2,0,0,9,7,0), /* #<FACE 67> and #<FACE 3312> */
  gsSP2Triangles(6,9,5,0,9,6,13,0), /* #<FACE 1658> and #<FACE 942> */
  gsSP2Triangles(8,9,13,0,9,8,7,0), /* #<FACE 941> and #<FACE 74> */
  gsSP2Triangles(1,12,14,0,10,14,11,0), /* #<FACE 2673> and #<FACE 2010> */
  gsSP2Triangles(11,14,15,0,14,12,13,0), /* #<FACE 1659> and #<FACE 944> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 943> */
  gsSPVertex(&cont_finish_v[2804],11,5),
  gsSP2Triangles(0,15,7,0,15,5,7,0), /* #<FACE 2018> and #<FACE 1651> */
  gsSP2Triangles(7,5,6,0,8,7,6,0), /* #<FACE 934> and #<FACE 933> */
  gsSP2Triangles(9,10,1,0,9,11,12,0), /* #<FACE 2014> and #<FACE 938> */
  gsSP2Triangles(10,9,12,0,11,3,12,0), /* #<FACE 937> and #<FACE 2675> */
  gsSP2Triangles(2,13,3,0,14,3,13,0), /* #<FACE 2015> and #<FACE 1654> */
  gsSP1Triangle(3,14,12,0), /* #<FACE 939> */
  gsSPVertex(&cont_finish_v[2815],13,0),
  gsSP2Triangles(1,2,0,0,15,1,0,0), /* #<FACE 927> and #<FACE 926> */
  gsSP2Triangles(1,7,2,0,5,2,7,0), /* #<FACE 1982> and #<FACE 1687> */
  gsSP2Triangles(8,7,3,0,12,11,4,0), /* #<FACE 64> and #<FACE 2679> */
  gsSP2Triangles(5,7,12,0,4,6,12,0), /* #<FACE 2022> and #<FACE 2021> */
  gsSP2Triangles(12,6,5,0,8,12,7,0), /* #<FACE 1648> and #<FACE 1647> */
  gsSP2Triangles(8,13,12,0,14,13,9,0), /* #<FACE 2681> and #<FACE 2016> */
  gsSP2Triangles(9,13,8,0,13,10,11,0), /* #<FACE 1653> and #<FACE 930> */
  gsSP1Triangle(12,13,11,0), /* #<FACE 929> */
  gsSPVertex(&cont_finish_v[2828],15,1),
  gsSP2Triangles(15,2,1,0,14,8,2,0), /* #<FACE 3302> and #<FACE 2027> */
  gsSP2Triangles(9,2,8,0,13,3,4,0), /* #<FACE 1642> and #<FACE 3238> */
  gsSP2Triangles(8,4,7,0,8,14,4,0), /* #<FACE 2034> and #<FACE 2028> */
  gsSP2Triangles(4,14,13,0,5,7,4,0), /* #<FACE 1641> and #<FACE 1635> */
  gsSP2Triangles(10,6,12,0,9,10,11,0), /* #<FACE 3305> and #<FACE 3298> */
  gsSP2Triangles(7,10,8,0,10,9,8,0), /* #<FACE 2033> and #<FACE 1636> */
  gsSP1Triangle(12,11,10,0), /* #<FACE 86> */
  gsSPVertex(&cont_finish_v[2843],13,0),
  gsSP2Triangles(1,15,0,0,2,5,6,0), /* #<FACE 83> and #<FACE 2837> */
  gsSP2Triangles(5,4,3,0,7,6,5,0), /* #<FACE 2060> and #<FACE 578> */
  gsSP2Triangles(8,10,11,0,9,10,8,0), /* #<FACE 3233> and #<FACE 3232> */
  gsSP2Triangles(10,9,13,0,13,14,10,0), /* #<FACE 3225> and #<FACE 1633> */
  gsSP1Triangle(12,11,10,0), /* #<FACE 150> */
  gsSPVertex(&cont_finish_v[2856],14,2),
  gsSP2Triangles(15,14,2,0,6,8,9,0), /* #<FACE 141> and #<FACE 3257> */
  gsSP2Triangles(3,7,8,0,8,5,4,0), /* #<FACE 3253> and #<FACE 1605> */
  gsSP2Triangles(6,5,8,0,9,8,7,0), /* #<FACE 1603> and #<FACE 131> */
  gsSP2Triangles(0,12,1,0,1,11,10,0), /* #<FACE 3290> and #<FACE 2031> */
  gsSP1Triangle(13,1,12,0), /* #<FACE 94> */
  gsSPVertex(&cont_finish_v[2870],14,0),
  gsSP2Triangles(2,1,0,0,1,2,3,0), /* #<FACE 3282> and #<FACE 2077> */
  gsSP2Triangles(3,2,5,0,4,8,9,0), /* #<FACE 1592> and #<FACE 3389> */
  gsSP2Triangles(3,5,9,0,9,5,4,0), /* #<FACE 2078> and #<FACE 1591> */
  gsSP2Triangles(9,7,6,0,9,8,7,0), /* #<FACE 120> and #<FACE 5> */
  gsSP2Triangles(12,11,10,0,13,14,15,0), /* #<FACE 3285> and #<FACE 3244> */
  gsSPVertex(&cont_finish_v[2884],16,0),
  gsSP2Triangles(0,11,12,0,13,11,1,0), /* #<FACE 893> and #<FACE 155> */
  gsSP2Triangles(4,3,2,0,15,14,5,0), /* #<FACE 357> and #<FACE 3274> */
  gsSP2Triangles(6,8,9,0,9,10,7,0), /* #<FACE 3272> and #<FACE 3271> */
  gsSP1Triangle(10,9,8,0), /* #<FACE 111> */
  gsSPVertex(&cont_finish_v[2900],11,0),
  gsSP2Triangles(14,15,1,0,2,14,0,0), /* #<FACE 3276> and #<FACE 2108> */
  gsSP2Triangles(1,0,14,0,3,14,2,0), /* #<FACE 1561> and #<FACE 884> */
  gsSP2Triangles(7,8,4,0,7,5,6,0), /* #<FACE 2103> and #<FACE 888> */
  gsSP2Triangles(8,7,6,0,9,11,13,0), /* #<FACE 887> and #<FACE 3229> */
  gsSP2Triangles(11,9,10,0,11,10,12,0), /* #<FACE 2106> and #<FACE 894> */
  gsSPVertex(&cont_finish_v[2911],16,0),
  gsSP2Triangles(14,0,13,0,1,13,12,0), /* #<FACE 1695> and #<FACE 185> */
  gsSP2Triangles(15,13,1,0,4,3,2,0), /* #<FACE 60> and #<FACE 1553> */
  gsSP2Triangles(11,7,5,0,5,10,11,0), /* #<FACE 3392> and #<FACE 3379> */
  gsSP2Triangles(7,11,6,0,9,8,11,0), /* #<FACE 3365> and #<FACE 878> */
  gsSP1Triangle(11,10,9,0), /* #<FACE 15> */
  gsSPVertex(&cont_finish_v[2927],12,0),
  gsSP2Triangles(1,2,0,0,3,5,8,0), /* #<FACE 191> and #<FACE 872> */
  gsSP2Triangles(10,8,5,0,4,9,10,0), /* #<FACE 2694> and #<FACE 2693> */
  gsSP2Triangles(5,6,10,0,7,10,6,0), /* #<FACE 1988> and #<FACE 1681> */
  gsSP2Triangles(10,9,8,0,14,13,15,0), /* #<FACE 873> and #<FACE 3331> */
  gsSP1Triangle(11,12,13,0), /* #<FACE 3200> */
  gsSPVertex(&cont_finish_v[2939],11,5),
  gsSP2Triangles(6,9,10,0,5,14,9,0), /* #<FACE 3185> and #<FACE 2702> */
  gsSP2Triangles(8,9,7,0,6,5,9,0), /* #<FACE 2137> and #<FACE 1711> */
  gsSP2Triangles(7,9,11,0,11,9,14,0), /* #<FACE 1532> and #<FACE 862> */
  gsSP2Triangles(10,9,8,0,12,0,2,0), /* #<FACE 201> and #<FACE 3194> */
  gsSP2Triangles(11,14,2,0,2,1,11,0), /* #<FACE 2700> and #<FACE 2138> */
  gsSP2Triangles(12,2,13,0,15,13,2,0), /* #<FACE 1960> and #<FACE 1709> */
  gsSP1Triangle(15,2,14,0), /* #<FACE 864> */
  gsSPVertex(&cont_finish_v[2950],16,0),
  gsSP2Triangles(1,2,0,0,9,5,3,0), /* #<FACE 854> and #<FACE 3047> */
  gsSP2Triangles(4,9,7,0,5,9,4,0), /* #<FACE 3034> and #<FACE 3023> */
  gsSP2Triangles(9,6,8,0,9,8,7,0), /* #<FACE 1517> and #<FACE 346> */
  gsSP2Triangles(10,15,13,0,12,15,11,0), /* #<FACE 3164> and #<FACE 1520> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 220> */
  gsSPVertex(&cont_finish_v[2966],14,2),
  gsSP2Triangles(2,4,5,0,3,2,5,0), /* #<FACE 3022> and #<FACE 3021> */
  gsSP2Triangles(6,5,4,0,10,7,8,0), /* #<FACE 356> and #<FACE 2160> */
  gsSP2Triangles(10,8,9,0,11,10,9,0), /* #<FACE 852> and #<FACE 851> */
  gsSP2Triangles(13,12,1,0,1,15,13,0), /* #<FACE 3150> and #<FACE 3149> */
  gsSP2Triangles(0,14,1,0,15,1,14,0), /* #<FACE 2157> and #<FACE 1512> */
  gsSPVertex(&cont_finish_v[2980],15,0),
  gsSP2Triangles(2,1,0,0,5,3,7,0), /* #<FACE 3441> and #<FACE 3154> */
  gsSP2Triangles(4,5,6,0,7,6,5,0), /* #<FACE 3153> and #<FACE 232> */
  gsSP2Triangles(10,12,13,0,13,14,8,0), /* #<FACE 3156> and #<FACE 3155> */
  gsSP2Triangles(9,11,13,0,13,11,10,0), /* #<FACE 2166> and #<FACE 1503> */
  gsSP1Triangle(14,13,12,0), /* #<FACE 227> */
  gsSPVertex(&cont_finish_v[2995],16,0),
  gsSP2Triangles(14,0,15,0,3,2,1,0), /* #<FACE 254> and #<FACE 3107> */
  gsSP2Triangles(6,4,5,0,5,7,6,0), /* #<FACE 3110> and #<FACE 2192> */
  gsSP2Triangles(7,8,6,0,9,13,11,0), /* #<FACE 1477> and #<FACE 3199> */
  gsSP2Triangles(7,13,8,0,13,9,8,0), /* #<FACE 2191> and #<FACE 1478> */
  gsSP2Triangles(13,10,12,0,13,12,11,0), /* #<FACE 188> and #<FACE 187> */
  gsSPVertex(&cont_finish_v[3011],14,0),
  gsSP2Triangles(1,3,2,0,0,4,3,0), /* #<FACE 836> and #<FACE 236> */
  gsSP2Triangles(7,12,10,0,5,10,8,0), /* #<FACE 3123> and #<FACE 2207> */
  gsSP2Triangles(5,6,10,0,7,10,6,0), /* #<FACE 2205> and #<FACE 1464> */
  gsSP2Triangles(10,9,8,0,11,9,10,0), /* #<FACE 1462> and #<FACE 262> */
  gsSP2Triangles(12,11,10,0,13,14,15,0), /* #<FACE 261> and #<FACE 3132> */
  gsSPVertex(&cont_finish_v[3025],12,4),
  gsSP2Triangles(4,13,12,0,8,14,6,0), /* #<FACE 223> and #<FACE 3169> */
  gsSP2Triangles(5,7,8,0,6,5,8,0), /* #<FACE 3142> and #<FACE 3141> */
  gsSP2Triangles(14,8,15,0,9,8,7,0), /* #<FACE 1436> and #<FACE 245> */
  gsSP2Triangles(10,0,3,0,3,1,10,0), /* #<FACE 3148> and #<FACE 2161> */
  gsSP1Triangle(3,11,2,0), /* #<FACE 837> */
  gsSPVertex(&cont_finish_v[3037],12,0),
  gsSP2Triangles(10,0,1,0,15,1,4,0), /* #<FACE 3158> and #<FACE 2238> */
  gsSP2Triangles(2,4,1,0,14,7,3,0), /* #<FACE 1431> and #<FACE 3136> */
  gsSP2Triangles(4,7,15,0,4,5,7,0), /* #<FACE 2237> and #<FACE 2165> */
  gsSP2Triangles(6,7,5,0,7,14,15,0), /* #<FACE 1504> and #<FACE 1432> */
  gsSP2Triangles(11,12,13,0,12,10,9,0), /* #<FACE 3161> and #<FACE 2236> */
  gsSP2Triangles(12,9,8,0,12,11,10,0), /* #<FACE 832> and #<FACE 224> */
  gsSPVertex(&cont_finish_v[3049],14,2),
  gsSP2Triangles(14,6,4,0,2,6,15,0), /* #<FACE 2709> and #<FACE 2256> */
  gsSP2Triangles(2,3,6,0,3,5,6,0), /* #<FACE 2255> and #<FACE 1414> */
  gsSP2Triangles(15,6,14,0,4,6,9,0), /* #<FACE 1413> and #<FACE 824> */
  gsSP2Triangles(6,5,9,0,7,12,13,0), /* #<FACE 820> and #<FACE 3090> */
  gsSP2Triangles(10,8,13,0,13,11,9,0), /* #<FACE 1415> and #<FACE 822> */
  gsSP2Triangles(10,13,9,0,13,12,11,0), /* #<FACE 821> and #<FACE 296> */
  gsSPVertex(&cont_finish_v[3063],13,0),
  gsSP2Triangles(4,3,2,0,0,4,1,0), /* #<FACE 1411> and #<FACE 813> */
  gsSP2Triangles(5,15,6,0,7,6,15,0), /* #<FACE 2838> and #<FACE 577> */
  gsSP2Triangles(14,7,15,0,9,10,8,0), /* #<FACE 571> and #<FACE 2259> */
  gsSP1Triangle(12,11,10,0), /* #<FACE 573> */
  gsSPVertex(&cont_finish_v[3076],11,5),
  gsSP2Triangles(13,9,12,0,8,9,6,0), /* #<FACE 2715> and #<FACE 2244> */
  gsSP2Triangles(7,9,5,0,13,5,9,0), /* #<FACE 2243> and #<FACE 1426> */
  gsSP2Triangles(9,7,6,0,9,8,14,0), /* #<FACE 1425> and #<FACE 808> */
  gsSP2Triangles(12,9,14,0,11,4,0,0), /* #<FACE 807> and #<FACE 2713> */
  gsSP2Triangles(4,10,1,0,10,4,2,0), /* #<FACE 2712> and #<FACE 2258> */
  gsSP2Triangles(3,4,15,0,11,15,4,0), /* #<FACE 2257> and #<FACE 1412> */
  gsSPVertex(&cont_finish_v[3087],12,0),
  gsSP2Triangles(0,15,1,0,4,2,3,0), /* #<FACE 1400> and #<FACE 2251> */
  gsSP2Triangles(4,3,5,0,6,4,5,0), /* #<FACE 803> and #<FACE 802> */
  gsSP2Triangles(11,10,5,0,10,6,5,0), /* #<FACE 2717> and #<FACE 805> */
  gsSP2Triangles(10,8,7,0,12,14,8,0), /* #<FACE 301> and #<FACE 2716> */
  gsSP2Triangles(8,10,12,0,11,9,12,0), /* #<FACE 2250> and #<FACE 2249> */
  gsSP2Triangles(9,13,12,0,11,12,10,0), /* #<FACE 1420> and #<FACE 1419> */
  gsSPVertex(&cont_finish_v[3099],13,3),
  gsSP2Triangles(12,13,11,0,6,14,15,0), /* #<FACE 307> and #<FACE 2271> */
  gsSP2Triangles(3,6,4,0,4,6,7,0), /* #<FACE 2247> and #<FACE 1422> */
  gsSP2Triangles(6,15,5,0,7,6,5,0), /* #<FACE 797> and #<FACE 796> */
  gsSP2Triangles(9,0,8,0,10,1,9,0), /* #<FACE 2719> and #<FACE 2269> */
  gsSPVertex(&cont_finish_v[3112],11,0),
  gsSP2Triangles(14,10,2,0,10,14,9,0), /* #<FACE 2726> and #<FACE 2273> */
  gsSP2Triangles(15,14,2,0,14,3,9,0), /* #<FACE 1398> and #<FACE 1396> */
  gsSP2Triangles(4,8,7,0,8,4,5,0), /* #<FACE 3083> and #<FACE 2277> */
  gsSP2Triangles(8,5,6,0,10,8,6,0), /* #<FACE 790> and #<FACE 789> */
  gsSP2Triangles(7,8,13,0,8,11,13,0), /* #<FACE 303> and #<FACE 3077> */
  gsSP2Triangles(11,9,0,0,12,11,1,0), /* #<FACE 2725> and #<FACE 2276> */
  gsSP2Triangles(8,10,11,0,10,9,11,0), /* #<FACE 2274> and #<FACE 1395> */
  gsSP1Triangle(0,1,11,0), /* #<FACE 1393> */
  gsSPVertex(&cont_finish_v[3123],12,4),
  gsSP2Triangles(15,1,6,0,0,4,1,0), /* #<FACE 2732> and #<FACE 1394> */
  gsSP2Triangles(6,1,5,0,9,7,8,0), /* #<FACE 780> and #<FACE 784> */
  gsSP2Triangles(10,9,8,0,9,10,12,0), /* #<FACE 783> and #<FACE 2279> */
  gsSP2Triangles(11,12,10,0,14,13,12,0), /* #<FACE 1390> and #<FACE 304> */
  gsSPVertex(&cont_finish_v[3135],14,0),
  gsSP2Triangles(1,6,0,0,4,2,3,0), /* #<FACE 3445> and #<FACE 765> */
  gsSP2Triangles(5,4,3,0,8,7,6,0), /* #<FACE 764> and #<FACE 3440> */
  gsSP2Triangles(12,13,9,0,12,15,10,0), /* #<FACE 2284> and #<FACE 2282> */
  gsSP2Triangles(12,10,11,0,13,12,11,0), /* #<FACE 777> and #<FACE 776> */
  gsSPVertex(&cont_finish_v[3149],16,0),
  gsSP2Triangles(15,0,13,0,1,13,0,0), /* #<FACE 1919> and #<FACE 1750> */
  gsSP2Triangles(13,2,14,0,7,3,4,0), /* #<FACE 1366> and #<FACE 2747> */
  gsSP2Triangles(3,7,8,0,4,5,7,0), /* #<FACE 2300> and #<FACE 1917> */
  gsSP2Triangles(6,7,5,0,9,8,7,0), /* #<FACE 1752> and #<FACE 1369> */
  gsSP1Triangle(12,11,10,0), /* #<FACE 3442> */
  gsSPVertex(&cont_finish_v[3165],13,0),
  gsSP2Triangles(2,1,0,0,0,4,2,0), /* #<FACE 2306> and #<FACE 1361> */
  gsSP2Triangles(3,1,2,0,7,11,9,0), /* #<FACE 219> and #<FACE 2757> */
  gsSP2Triangles(5,8,11,0,7,6,11,0), /* #<FACE 2301> and #<FACE 1531> */
  gsSP2Triangles(10,11,8,0,11,10,9,0), /* #<FACE 1368> and #<FACE 743> */
  gsSP2Triangles(13,12,15,0,12,13,14,0), /* #<FACE 2753> and #<FACE 2303> */
  gsSPVertex(&cont_finish_v[3178],11,5),
  gsSP2Triangles(5,9,10,0,8,6,10,0), /* #<FACE 3102> and #<FACE 1821> */
  gsSP2Triangles(10,11,7,0,8,10,7,0), /* #<FACE 726> and #<FACE 725> */
  gsSP2Triangles(11,10,9,0,12,14,1,0), /* #<FACE 285> and #<FACE 2314> */
  gsSP2Triangles(13,0,1,0,14,13,1,0), /* #<FACE 1363> and #<FACE 1355> */
  gsSP1Triangle(15,3,2,0), /* #<FACE 3165> */
  gsSPVertex(&cont_finish_v[3189],16,0),
  gsSP2Triangles(1,2,0,0,3,5,7,0), /* #<FACE 459> and #<FACE 2506> */
  gsSP2Triangles(5,4,7,0,8,7,6,0), /* #<FACE 1163> and #<FACE 115> */
  gsSP2Triangles(13,10,9,0,10,13,14,0), /* #<FACE 2922> and #<FACE 2492> */
  gsSP2Triangles(11,12,13,0,12,15,13,0), /* #<FACE 2490> and #<FACE 1179> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 1177> */
  gsSPVertex(&cont_finish_v[3205],12,4),
  gsSP2Triangles(12,5,13,0,10,11,5,0), /* #<FACE 3259> and #<FACE 2571> */
  gsSP2Triangles(14,5,15,0,15,5,4,0), /* #<FACE 2567> and #<FACE 1102> */
  gsSP2Triangles(4,5,11,0,10,5,12,0), /* #<FACE 1098> and #<FACE 504> */
  gsSP2Triangles(14,13,5,0,6,0,2,0), /* #<FACE 128> and #<FACE 2926> */
  gsSP2Triangles(9,8,2,0,2,8,7,0), /* #<FACE 2516> and #<FACE 1153> */
  gsSP1Triangle(2,1,9,0), /* #<FACE 602> */
  gsSPVertex(&cont_finish_v[3217],8,0),
  gsSP2Triangles(3,13,6,0,14,15,6,0), /* #<FACE 3258> and #<FACE 2570> */
  gsSP2Triangles(15,5,6,0,6,5,4,0), /* #<FACE 1099> and #<FACE 1095> */
  gsSP2Triangles(6,13,14,0,0,10,12,0), /* #<FACE 129> and #<FACE 2896> */
  gsSP2Triangles(1,2,10,0,0,1,10,0), /* #<FACE 2849> and #<FACE 2848> */
  gsSP2Triangles(10,7,11,0,7,10,2,0), /* #<FACE 601> and #<FACE 569> */
  gsSPVertex(&cont_finish_v[3225],11,5),
  gsSP2Triangles(1,5,7,0,5,1,0,0), /* #<FACE 2900> and #<FACE 2847> */
  gsSP2Triangles(8,1,6,0,1,7,6,0), /* #<FACE 2583> and #<FACE 1086> */
  gsSP2Triangles(2,1,8,0,9,14,15,0), /* #<FACE 597> and #<FACE 2919> */
  gsSP2Triangles(13,10,15,0,10,11,15,0), /* #<FACE 2580> and #<FACE 1089> */
  gsSP2Triangles(12,15,11,0,15,14,13,0), /* #<FACE 599> and #<FACE 467> */
  gsSPVertex(&cont_finish_v[3236],14,0),
  gsSP2Triangles(3,2,1,0,5,0,3,0), /* #<FACE 353> and #<FACE 2338> */
  gsSP2Triangles(6,5,4,0,7,13,11,0), /* #<FACE 344> and #<FACE 2898> */
  gsSP2Triangles(9,8,13,0,12,13,8,0), /* #<FACE 2587> and #<FACE 1082> */
  gsSP2Triangles(13,10,9,0,13,12,11,0), /* #<FACE 596> and #<FACE 502> */
  gsSPVertex(&cont_finish_v[3250],14,2),
  gsSP2Triangles(2,4,5,0,5,4,3,0), /* #<FACE 345> and #<FACE 322> */
  gsSP2Triangles(7,12,6,0,14,6,12,0), /* #<FACE 3033> and #<FACE 347> */
  gsSP2Triangles(9,12,7,0,8,11,9,0), /* #<FACE 335> and #<FACE 3046> */
  gsSP2Triangles(8,0,11,0,11,12,9,0), /* #<FACE 3045> and #<FACE 336> */
  gsSP2Triangles(10,15,13,0,14,12,13,0), /* #<FACE 3040> and #<FACE 3031> */
  gsSP2Triangles(13,12,11,0,15,14,13,0), /* #<FACE 349> and #<FACE 341> */
  gsSPVertex(&cont_finish_v[3264],16,0),
  gsSP2Triangles(2,1,0,0,5,4,3,0), /* #<FACE 823> and #<FACE 308> */
  gsSP2Triangles(6,7,8,0,9,8,7,0), /* #<FACE 3143> and #<FACE 242> */
  gsSP2Triangles(10,15,13,0,10,13,11,0), /* #<FACE 2758> and #<FACE 2756> */
  gsSP2Triangles(12,11,13,0,15,14,13,0), /* #<FACE 744> and #<FACE 742> */
  gsSPVertex(&cont_finish_v[3280],15,1),
  gsSP2Triangles(14,13,1,0,15,5,6,0), /* #<FACE 1451> and #<FACE 2230> */
  gsSP2Triangles(15,3,5,0,3,2,5,0), /* #<FACE 2229> and #<FACE 1440> */
  gsSP2Triangles(6,5,4,0,7,10,8,0), /* #<FACE 1439> and #<FACE 2721> */
  gsSP2Triangles(10,9,8,0,12,0,11,0), /* #<FACE 795> and #<FACE 2710> */
  gsSPVertex(&cont_finish_v[3295],12,0),
  gsSP2Triangles(0,1,2,0,15,0,2,0), /* #<FACE 831> and #<FACE 830> */
  gsSP2Triangles(3,15,2,0,4,6,5,0), /* #<FACE 829> and #<FACE 1455> */
  gsSP2Triangles(8,5,6,0,6,9,8,0), /* #<FACE 841> and #<FACE 2216> */
  gsSP2Triangles(6,7,9,0,7,11,9,0), /* #<FACE 2215> and #<FACE 1454> */
  gsSP2Triangles(8,9,14,0,9,13,14,0), /* #<FACE 1453> and #<FACE 2218> */
  gsSP2Triangles(9,11,13,0,11,10,13,0), /* #<FACE 2217> and #<FACE 1452> */
  gsSPVertex(&cont_finish_v[3307],14,2),
  gsSP2Triangles(3,6,4,0,6,3,2,0), /* #<FACE 3178> and #<FACE 3116> */
  gsSP2Triangles(6,5,4,0,9,8,7,0), /* #<FACE 208> and #<FACE 1466> */
  gsSP2Triangles(10,11,15,0,0,13,14,0), /* #<FACE 2208> and #<FACE 2210> */
  gsSP2Triangles(11,14,15,0,14,12,0,0), /* #<FACE 1461> and #<FACE 1460> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 1459> */
  gsSPVertex(&cont_finish_v[3321],15,0),
  gsSP2Triangles(1,5,3,0,1,0,5,0), /* #<FACE 3198> and #<FACE 278> */
  gsSP2Triangles(2,4,5,0,5,4,3,0), /* #<FACE 243> and #<FACE 189> */
  gsSP2Triangles(8,7,6,0,11,10,9,0), /* #<FACE 3062> and #<FACE 283> */
  gsSP1Triangle(14,13,12,0), /* #<FACE 1502> */
  gsSPVertex(&cont_finish_v[3336],16,0),
  gsSP2Triangles(0,15,14,0,3,1,2,0), /* #<FACE 215> and #<FACE 3409> */
  gsSP2Triangles(4,3,2,0,5,8,6,0), /* #<FACE 3397> and #<FACE 3411> */
  gsSP2Triangles(8,7,6,0,11,12,10,0), /* #<FACE 3399> and #<FACE 276> */
  gsSP2Triangles(10,9,11,0,13,12,11,0), /* #<FACE 275> and #<FACE 268> */
  gsSPVertex(&cont_finish_v[3352],12,0),
  gsSP2Triangles(3,1,0,0,3,2,1,0), /* #<FACE 319> and #<FACE 317> */
  gsSP2Triangles(8,4,6,0,8,5,7,0), /* #<FACE 291> and #<FACE 273> */
  gsSP2Triangles(8,6,5,0,9,8,7,0), /* #<FACE 272> and #<FACE 244> */
  gsSP2Triangles(11,14,15,0,14,11,10,0), /* #<FACE 3170> and #<FACE 216> */
  gsSPVertex(&cont_finish_v[3364],15,1),
  gsSP2Triangles(1,4,5,0,4,1,3,0), /* #<FACE 348> and #<FACE 229> */
  gsSP2Triangles(4,3,2,0,9,5,4,0), /* #<FACE 228> and #<FACE 3032> */
  gsSP2Triangles(9,7,5,0,8,9,6,0), /* #<FACE 230> and #<FACE 340> */
  gsSP2Triangles(9,8,7,0,10,15,13,0), /* #<FACE 231> and #<FACE 2704> */
  gsSP2Triangles(11,10,13,0,14,12,13,0), /* #<FACE 853> and #<FACE 850> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 849> */
  gsSPVertex(&cont_finish_v[3379],16,0),
  gsSP2Triangles(2,3,1,0,3,4,1,0), /* #<FACE 3184> and #<FACE 202> */
  gsSP2Triangles(4,0,1,0,7,6,5,0), /* #<FACE 107> and #<FACE 173> */
  gsSP2Triangles(13,10,11,0,13,8,10,0), /* #<FACE 2150> and #<FACE 2149> */
  gsSP2Triangles(11,10,9,0,13,11,15,0), /* #<FACE 1519> and #<FACE 2152> */
  gsSP2Triangles(12,14,15,0,15,14,13,0), /* #<FACE 2151> and #<FACE 1518> */
  gsSPVertex(&cont_finish_v[3395],13,3),
  gsSP2Triangles(4,5,3,0,6,5,4,0), /* #<FACE 3371> and #<FACE 4> */
  gsSP2Triangles(7,9,8,0,11,9,14,0), /* #<FACE 2114> and #<FACE 2113> */
  gsSP2Triangles(9,11,8,0,11,12,10,0), /* #<FACE 1555> and #<FACE 2116> */
  gsSP2Triangles(11,14,12,0,14,13,12,0), /* #<FACE 2115> and #<FACE 1554> */
  gsSP1Triangle(15,1,0,0), /* #<FACE 3277> */
  gsSPVertex(&cont_finish_v[3408],15,0),
  gsSP2Triangles(0,1,3,0,3,1,4,0), /* #<FACE 3372> and #<FACE 21> */
  gsSP2Triangles(5,6,2,0,5,2,3,0), /* #<FACE 113> and #<FACE 112> */
  gsSP2Triangles(3,4,5,0,4,10,5,0), /* #<FACE 22> and #<FACE 13> */
  gsSP2Triangles(5,9,6,0,9,5,10,0), /* #<FACE 3383> and #<FACE 3382> */
  gsSP2Triangles(9,8,7,0,7,6,9,0), /* #<FACE 114> and #<FACE 12> */
  gsSP2Triangles(9,13,8,0,13,9,10,0), /* #<FACE 80> and #<FACE 27> */
  gsSP2Triangles(12,13,10,0,14,12,11,0), /* #<FACE 26> and #<FACE 3368> */
  gsSP1Triangle(14,13,12,0), /* #<FACE 3366> */
  gsSPVertex(&cont_finish_v[3423],16,0),
  gsSP2Triangles(0,13,15,0,0,14,13,0), /* #<FACE 7> and #<FACE 6> */
  gsSP2Triangles(1,4,5,0,2,3,4,0), /* #<FACE 3343> and #<FACE 3341> */
  gsSP2Triangles(6,4,3,0,6,5,4,0), /* #<FACE 50> and #<FACE 48> */
  gsSP2Triangles(9,10,11,0,7,12,10,0), /* #<FACE 2691> and #<FACE 889> */
  gsSP2Triangles(9,8,10,0,12,11,10,0), /* #<FACE 886> and #<FACE 885> */
  gsSPVertex(&cont_finish_v[3439],11,0),
  gsSP2Triangles(3,1,0,0,3,2,1,0), /* #<FACE 3402> and #<FACE 3396> */
  gsSP2Triangles(6,4,8,0,5,8,4,0), /* #<FACE 3224> and #<FACE 160> */
  gsSP2Triangles(8,7,6,0,9,15,13,0), /* #<FACE 159> and #<FACE 151> */
  gsSP1Triangle(10,13,14,0), /* #<FACE 79> */
  gsSPVertex(&cont_finish_v[3450],16,0),
  gsSP2Triangles(2,0,5,0,5,0,4,0), /* #<FACE 3266> and #<FACE 119> */
  gsSP2Triangles(2,5,1,0,5,4,3,0), /* #<FACE 101> and #<FACE 56> */
  gsSP2Triangles(15,9,14,0,14,9,7,0), /* #<FACE 3267> and #<FACE 118> */
  gsSP2Triangles(8,9,6,0,9,8,7,0), /* #<FACE 104> and #<FACE 103> */
  gsSP2Triangles(10,11,12,0,13,12,11,0), /* #<FACE 3400> and #<FACE 3394> */
  gsSPVertex(&cont_finish_v[3466],13,0),
  gsSP2Triangles(2,1,0,0,3,7,8,0), /* #<FACE 1639> and #<FACE 2040> */
  gsSP2Triangles(3,5,7,0,5,4,7,0), /* #<FACE 2039> and #<FACE 1630> */
  gsSP2Triangles(8,7,6,0,14,9,15,0), /* #<FACE 1629> and #<FACE 3211> */
  gsSP1Triangle(12,11,10,0), /* #<FACE 96> */
  gsSPVertex(&cont_finish_v[3479],16,0),
  gsSP2Triangles(2,1,0,0,3,4,5,0), /* #<FACE 3340> and #<FACE 2128> */
  gsSP2Triangles(6,5,4,0,8,7,9,0), /* #<FACE 1541> and #<FACE 2674> */
  gsSP2Triangles(11,10,9,0,12,13,14,0), /* #<FACE 940> and #<FACE 2677> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 935> */
  gsSPVertex(&cont_finish_v[3495],15,0),
  gsSP2Triangles(0,2,1,0,3,6,4,0), /* #<FACE 1745> and #<FACE 3384> */
  gsSP2Triangles(6,5,4,0,9,7,8,0), /* #<FACE 11> and #<FACE 44> */
  gsSP2Triangles(10,9,8,0,11,14,12,0), /* #<FACE 16> and #<FACE 2701> */
  gsSP1Triangle(14,13,12,0), /* #<FACE 863> */
  gsSPVertex(&cont_finish_v[3510],15,1),
  gsSP2Triangles(1,3,4,0,4,2,1,0), /* #<FACE 1860> and #<FACE 1036> */
  gsSP2Triangles(4,6,5,0,14,6,3,0), /* #<FACE 2920> and #<FACE 1816> */
  gsSP2Triangles(4,3,6,0,6,7,5,0), /* #<FACE 1809> and #<FACE 466> */
  gsSP2Triangles(8,6,14,0,8,14,15,0), /* #<FACE 1853> and #<FACE 1852> */
  gsSP2Triangles(8,7,6,0,11,10,9,0), /* #<FACE 465> and #<FACE 3041> */
  gsSP1Triangle(13,12,0,0), /* #<FACE 18> */
  gsSPVertex(&cont_finish_v[3525],13,0),
  gsSP2Triangles(0,2,1,0,5,4,3,0), /* #<FACE 91> and #<FACE 1032> */
  gsSP2Triangles(8,7,6,0,11,10,9,0), /* #<FACE 1033> and #<FACE 1034> */
  gsSP1Triangle(14,12,15,0), /* #<FACE 1817> */
  gsSPVertex(&cont_finish_v[3538],12,4),
  gsSP2Triangles(4,5,14,0,14,5,15,0), /* #<FACE 2648> and #<FACE 988> */
  gsSP2Triangles(6,10,8,0,7,10,6,0), /* #<FACE 1945> and #<FACE 1720> */
  gsSP2Triangles(0,1,9,0,8,1,13,0), /* #<FACE 3294> and #<FACE 1726> */
  gsSP2Triangles(10,1,8,0,10,9,1,0), /* #<FACE 1724> and #<FACE 90> */
  gsSP2Triangles(11,2,0,0,2,13,1,0), /* #<FACE 3293> and #<FACE 1943> */
  gsSP1Triangle(13,2,12,0), /* #<FACE 1003> */
  gsSPVertex(&cont_finish_v[3550],14,0),
  gsSP2Triangles(9,7,10,0,10,7,6,0), /* #<FACE 2656> and #<FACE 978> */
  gsSP2Triangles(2,6,8,0,0,8,5,0), /* #<FACE 2655> and #<FACE 1918> */
  gsSP2Triangles(7,5,8,0,1,2,8,0), /* #<FACE 1751> and #<FACE 1691> */
  gsSP2Triangles(7,8,6,0,1,8,0,0), /* #<FACE 979> and #<FACE 170> */
  gsSP2Triangles(10,3,12,0,10,12,9,0), /* #<FACE 1962> and #<FACE 1961> */
  gsSP2Triangles(12,13,15,0,4,11,13,0), /* #<FACE 2649> and #<FACE 1957> */
  gsSP2Triangles(14,13,11,0,13,12,3,0), /* #<FACE 1712> and #<FACE 1707> */
  gsSP2Triangles(13,14,15,0,13,3,4,0), /* #<FACE 987> and #<FACE 43> */
  gsSPVertex(&cont_finish_v[3564],10,6),
  gsSP2Triangles(6,1,0,0,14,9,10,0), /* #<FACE 3214> and #<FACE 3193> */
  gsSP2Triangles(13,7,9,0,14,13,9,0), /* #<FACE 2659> and #<FACE 1689> */
  gsSP2Triangles(10,9,8,0,12,2,1,0), /* #<FACE 192> and #<FACE 1978> */
  gsSP2Triangles(12,11,2,0,15,4,3,0), /* #<FACE 1692> and #<FACE 3350> */
  gsSPVertex(&cont_finish_v[3574],12,0),
  gsSP2Triangles(0,1,9,0,2,6,5,0), /* #<FACE 1684> and #<FACE 969> */
  gsSP2Triangles(3,15,4,0,4,15,6,0), /* #<FACE 1959> and #<FACE 1710> */
  gsSP2Triangles(15,7,6,0,7,9,5,0), /* #<FACE 2662> and #<FACE 971> */
  gsSP2Triangles(6,7,5,0,8,9,7,0), /* #<FACE 970> and #<FACE 2661> */
  gsSP2Triangles(0,9,8,0,11,13,14,0), /* #<FACE 1683> and #<FACE 1980> */
  gsSP1Triangle(11,10,13,0), /* #<FACE 1690> */
  gsSPVertex(&cont_finish_v[3586],13,3),
  gsSP2Triangles(4,15,3,0,8,7,9,0), /* #<FACE 167> and #<FACE 2667> */
  gsSP2Triangles(5,7,8,0,6,11,9,0), /* #<FACE 1676> and #<FACE 961> */
  gsSP2Triangles(7,6,9,0,8,9,10,0), /* #<FACE 960> and #<FACE 2665> */
  gsSP2Triangles(11,10,9,0,0,12,1,0), /* #<FACE 962> and #<FACE 2664> */
  gsSP2Triangles(13,1,12,0,14,13,12,0), /* #<FACE 963> and #<FACE 966> */
  gsSPVertex(&cont_finish_v[3599],14,0),
  gsSP2Triangles(4,3,0,0,5,2,6,0), /* #<FACE 71> and #<FACE 2839> */
  gsSP2Triangles(1,6,2,0,9,7,11,0), /* #<FACE 576> and #<FACE 3322> */
  gsSP2Triangles(9,8,7,0,11,10,9,0), /* #<FACE 1671> and #<FACE 69> */
  gsSP1Triangle(15,13,12,0), /* #<FACE 959> */
  gsSPVertex(&cont_finish_v[3613],11,5),
  gsSP2Triangles(5,8,9,0,6,8,5,0), /* #<FACE 2671> and #<FACE 947> */
  gsSP2Triangles(9,0,7,0,3,7,0,0), /* #<FACE 2012> and #<FACE 1657> */
  gsSP2Triangles(0,9,8,0,13,12,10,0), /* #<FACE 948> and #<FACE 2670> */
  gsSP2Triangles(2,11,1,0,12,13,15,0), /* #<FACE 2840> and #<FACE 1666> */
  gsSP2Triangles(13,3,4,0,4,15,13,0), /* #<FACE 3314> and #<FACE 2003> */
  gsSP1Triangle(15,4,14,0), /* #<FACE 957> */
  gsSPVertex(&cont_finish_v[3624],16,0),
  gsSP2Triangles(5,7,2,0,3,5,2,0), /* #<FACE 3308> and #<FACE 3307> */
  gsSP2Triangles(5,6,7,0,6,5,4,0), /* #<FACE 3332> and #<FACE 2019> */
  gsSP2Triangles(6,0,1,0,1,7,6,0), /* #<FACE 2017> and #<FACE 59> */
  gsSP2Triangles(13,11,9,0,9,11,8,0), /* #<FACE 2676> and #<FACE 936> */
  gsSP2Triangles(11,13,14,0,11,14,10,0), /* #<FACE 1656> and #<FACE 1655> */
  gsSP2Triangles(13,12,15,0,15,14,13,0), /* #<FACE 3325> and #<FACE 2013> */
  gsSPVertex(&cont_finish_v[3640],14,2),
  gsSP2Triangles(14,13,1,0,1,2,14,0), /* #<FACE 1645> and #<FACE 2683> */
  gsSP2Triangles(2,1,0,0,14,2,3,0), /* #<FACE 1652> and #<FACE 925> */
  gsSP2Triangles(14,3,4,0,4,15,14,0), /* #<FACE 2682> and #<FACE 2023> */
  gsSP2Triangles(5,4,3,0,4,5,6,0), /* #<FACE 928> and #<FACE 1981> */
  gsSP2Triangles(7,6,5,0,12,10,8,0), /* #<FACE 1688> and #<FACE 2680> */
  gsSP2Triangles(9,11,12,0,12,11,10,0), /* #<FACE 1672> and #<FACE 931> */
  gsSPVertex(&cont_finish_v[3654],13,0),
  gsSP2Triangles(0,1,7,0,7,1,4,0), /* #<FACE 1644> and #<FACE 918> */
  gsSP2Triangles(5,2,3,0,3,6,5,0), /* #<FACE 1974> and #<FACE 2685> */
  gsSP2Triangles(6,7,4,0,5,6,4,0), /* #<FACE 922> and #<FACE 921> */
  gsSP2Triangles(8,7,6,0,15,10,13,0), /* #<FACE 2684> and #<FACE 923> */
  gsSP2Triangles(10,12,13,0,10,9,12,0), /* #<FACE 924> and #<FACE 77> */
  gsSP2Triangles(14,15,13,0,12,11,13,0), /* #<FACE 1646> and #<FACE 2024> */
  gsSPVertex(&cont_finish_v[3667],15,1),
  gsSP2Triangles(1,3,4,0,4,3,2,0), /* #<FACE 3236> and #<FACE 146> */
  gsSP2Triangles(7,6,5,0,15,10,11,0), /* #<FACE 147> and #<FACE 3300> */
  gsSP2Triangles(8,9,10,0,11,10,9,0), /* #<FACE 116> and #<FACE 84> */
  gsSP1Triangle(14,13,12,0), /* #<FACE 3301> */
  gsSPVertex(&cont_finish_v[3682],15,0),
  gsSP2Triangles(0,2,3,0,2,1,4,0), /* #<FACE 3291> and #<FACE 3218> */
  gsSP2Triangles(4,3,2,0,5,8,9,0), /* #<FACE 93> and #<FACE 2899> */
  gsSP2Triangles(9,7,6,0,9,8,7,0), /* #<FACE 2897> and #<FACE 500> */
  gsSP2Triangles(12,11,10,0,14,13,15,0), /* #<FACE 1623> and #<FACE 2037> */
  gsSPVertex(&cont_finish_v[3697],16,0),
  gsSP2Triangles(2,1,0,0,5,4,3,0), /* #<FACE 2082> and #<FACE 1773> */
  gsSP2Triangles(8,7,6,0,10,9,8,0), /* #<FACE 3240> and #<FACE 1595> */
  gsSP2Triangles(12,13,11,0,15,14,13,0), /* #<FACE 2687> and #<FACE 2686> */
  gsSPVertex(&cont_finish_v[3713],16,0),
  gsSP2Triangles(2,1,3,0,3,1,0,0), /* #<FACE 2107> and #<FACE 1562> */
  gsSP2Triangles(5,4,6,0,6,10,5,0), /* #<FACE 2690> and #<FACE 1563> */
  gsSP2Triangles(8,9,6,0,6,9,10,0), /* #<FACE 2105> and #<FACE 1564> */
  gsSP2Triangles(8,7,9,0,9,15,10,0), /* #<FACE 108> and #<FACE 2689> */
  gsSP2Triangles(10,15,13,0,11,14,15,0), /* #<FACE 890> and #<FACE 2688> */
  gsSP2Triangles(14,12,13,0,15,14,13,0), /* #<FACE 892> and #<FACE 891> */
  gsSPVertex(&cont_finish_v[3729],14,2),
  gsSP2Triangles(3,4,2,0,5,4,3,0), /* #<FACE 874> and #<FACE 2692> */
  gsSP2Triangles(4,5,6,0,7,6,5,0), /* #<FACE 2132> and #<FACE 1537> */
  gsSP2Triangles(10,9,8,0,13,12,11,0), /* #<FACE 575> and #<FACE 877> */
  gsSP2Triangles(15,14,0,0,1,15,0,0), /* #<FACE 883> and #<FACE 882> */
  gsSPVertex(&cont_finish_v[3743],16,0),
  gsSP2Triangles(0,1,5,0,2,5,1,0), /* #<FACE 3162> and #<FACE 222> */
  gsSP2Triangles(5,2,4,0,5,4,3,0), /* #<FACE 3174> and #<FACE 861> */
  gsSP2Triangles(9,8,11,0,10,11,6,0), /* #<FACE 2140> and #<FACE 2125> */
  gsSP2Triangles(6,11,7,0,8,7,11,0), /* #<FACE 1544> and #<FACE 1529> */
  gsSP2Triangles(9,11,12,0,12,11,10,0), /* #<FACE 181> and #<FACE 122> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 1958> */
  gsSPVertex(&cont_finish_v[3759],16,0),
  gsSP2Triangles(6,4,1,0,2,6,1,0), /* #<FACE 1507> and #<FACE 2162> */
  gsSP2Triangles(2,1,0,0,5,6,3,0), /* #<FACE 235> and #<FACE 2703> */
  gsSP2Triangles(6,5,4,0,7,12,10,0), /* #<FACE 1508> and #<FACE 855> */
  gsSP2Triangles(8,9,11,0,9,10,11,0), /* #<FACE 2158> and #<FACE 1511> */
  gsSP2Triangles(12,11,10,0,15,14,13,0), /* #<FACE 856> and #<FACE 3160> */
  gsSPVertex(&cont_finish_v[3775],16,0),
  gsSP2Triangles(0,3,4,0,1,5,3,0), /* #<FACE 1484> and #<FACE 178> */
  gsSP2Triangles(2,3,5,0,6,9,7,0), /* #<FACE 125> and #<FACE 3138> */
  gsSP2Triangles(9,8,7,0,12,11,10,0), /* #<FACE 249> and #<FACE 2171> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 1768> */
  gsSPVertex(&cont_finish_v[3791],11,5),
  gsSP2Triangles(6,7,8,0,6,5,7,0), /* #<FACE 3122> and #<FACE 2209> */
  gsSP2Triangles(9,8,7,0,13,11,10,0), /* #<FACE 263> and #<FACE 3135> */
  gsSP2Triangles(14,13,12,0,1,3,0,0), /* #<FACE 1474> and #<FACE 2185> */
  gsSP2Triangles(2,15,3,0,15,4,3,0), /* #<FACE 2092> and #<FACE 1577> */
  gsSPVertex(&cont_finish_v[3802],15,0),
  gsSP2Triangles(2,1,0,0,3,5,4,0), /* #<FACE 1446> and #<FACE 287> */
  gsSP2Triangles(7,4,5,0,7,5,6,0), /* #<FACE 3100> and #<FACE 286> */
  gsSP2Triangles(8,7,6,0,9,11,10,0), /* #<FACE 3101> and #<FACE 2978> */
  gsSP2Triangles(11,13,10,0,11,12,13,0), /* #<FACE 412> and #<FACE 3124> */
  gsSP1Triangle(14,13,12,0), /* #<FACE 260> */
  gsSPVertex(&cont_finish_v[3817],15,1),
  gsSP2Triangles(7,6,3,0,1,5,6,0), /* #<FACE 838> and #<FACE 2706> */
  gsSP2Triangles(6,5,3,0,2,4,5,0), /* #<FACE 834> and #<FACE 2705> */
  gsSP2Triangles(5,4,3,0,7,10,6,0), /* #<FACE 835> and #<FACE 2232> */
  gsSP2Triangles(15,9,8,0,10,11,13,0), /* #<FACE 248> and #<FACE 3097> */
  gsSP2Triangles(11,12,13,0,14,13,12,0), /* #<FACE 290> and #<FACE 270> */
  gsSPVertex(&cont_finish_v[3832],14,0),
  gsSP2Triangles(5,4,3,0,0,7,1,0), /* #<FACE 1429> and #<FACE 3095> */
  gsSP2Triangles(6,7,0,0,2,1,7,0), /* #<FACE 3094> and #<FACE 293> */
  gsSP2Triangles(15,9,8,0,14,10,9,0), /* #<FACE 2708> and #<FACE 2707> */
  gsSP1Triangle(13,12,11,0), /* #<FACE 413> */
  gsSPVertex(&cont_finish_v[3846],12,4),
  gsSP2Triangles(7,5,4,0,5,7,8,0), /* #<FACE 3082> and #<FACE 2252> */
  gsSP2Triangles(9,6,8,0,9,8,7,0), /* #<FACE 1418> and #<FACE 1417> */
  gsSP2Triangles(0,1,10,0,1,14,11,0), /* #<FACE 3075> and #<FACE 2241> */
  gsSP2Triangles(1,2,14,0,11,10,1,0), /* #<FACE 825> and #<FACE 309> */
  gsSP2Triangles(12,13,15,0,13,14,15,0), /* #<FACE 2242> and #<FACE 1427> */
  gsSP1Triangle(2,15,14,0), /* #<FACE 826> */
  gsSPVertex(&cont_finish_v[3858],16,0),
  gsSP2Triangles(3,2,1,0,15,6,4,0), /* #<FACE 1404> and #<FACE 814> */
  gsSP2Triangles(12,5,7,0,15,12,6,0), /* #<FACE 2711> and #<FACE 2260> */
  gsSP2Triangles(7,6,12,0,8,11,9,0), /* #<FACE 1409> and #<FACE 2841> */
  gsSP2Triangles(11,10,9,0,12,15,14,0), /* #<FACE 574> and #<FACE 3088> */
  gsSP2Triangles(14,0,13,0,15,0,14,0), /* #<FACE 1416> and #<FACE 298> */
  gsSPVertex(&cont_finish_v[3874],12,4),
  gsSP2Triangles(4,14,5,0,5,15,4,0), /* #<FACE 2720> and #<FACE 801> */
  gsSP2Triangles(0,5,14,0,7,8,6,0), /* #<FACE 800> and #<FACE 804> */
  gsSP2Triangles(9,8,7,0,1,2,10,0), /* #<FACE 2718> and #<FACE 806> */
  gsSP2Triangles(1,10,11,0,12,1,11,0), /* #<FACE 2714> and #<FACE 1403> */
  gsSP1Triangle(3,13,2,0), /* #<FACE 3085> */
  gsSPVertex(&cont_finish_v[3886],13,0),
  gsSP2Triangles(3,2,4,0,2,9,4,0), /* #<FACE 791> and #<FACE 2724> */
  gsSP2Triangles(9,10,4,0,3,4,5,0), /* #<FACE 792> and #<FACE 2723> */
  gsSP2Triangles(10,5,4,0,8,1,0,0), /* #<FACE 793> and #<FACE 2248> */
  gsSP2Triangles(8,7,6,0,15,10,9,0), /* #<FACE 2246> and #<FACE 1399> */
  gsSP1Triangle(14,12,11,0), /* #<FACE 2253> */
  gsSPVertex(&cont_finish_v[3899],14,2),
  gsSP2Triangles(3,0,1,0,1,4,3,0), /* #<FACE 1421> and #<FACE 2729> */
  gsSP2Triangles(4,1,2,0,3,4,5,0), /* #<FACE 1428> and #<FACE 785> */
  gsSP2Triangles(4,6,5,0,6,8,5,0), /* #<FACE 2728> and #<FACE 786> */
  gsSP2Triangles(6,7,8,0,9,8,7,0), /* #<FACE 2280> and #<FACE 1389> */
  gsSP2Triangles(11,12,13,0,11,10,12,0), /* #<FACE 2727> and #<FACE 1391> */
  gsSP2Triangles(12,15,13,0,15,14,13,0), /* #<FACE 787> and #<FACE 788> */
  gsSPVertex(&cont_finish_v[3913],14,0),
  gsSP2Triangles(5,0,2,0,4,5,3,0), /* #<FACE 1385> and #<FACE 779> */
  gsSP2Triangles(11,12,3,0,12,4,3,0), /* #<FACE 2734> and #<FACE 778> */
  gsSP2Triangles(6,5,4,0,5,6,0,0), /* #<FACE 2733> and #<FACE 2283> */
  gsSP2Triangles(6,1,0,0,10,11,7,0), /* #<FACE 1386> and #<FACE 1387> */
  gsSP2Triangles(9,10,8,0,13,10,9,0), /* #<FACE 782> and #<FACE 2730> */
  gsSP2Triangles(10,13,11,0,13,12,11,0), /* #<FACE 2281> and #<FACE 1388> */
  gsSPVertex(&cont_finish_v[3927],12,4),
  gsSP2Triangles(0,1,4,0,5,15,4,0), /* #<FACE 2748> and #<FACE 756> */
  gsSP2Triangles(1,5,4,0,6,9,8,0), /* #<FACE 755> and #<FACE 773> */
  gsSP2Triangles(7,10,9,0,10,11,8,0), /* #<FACE 2736> and #<FACE 775> */
  gsSP2Triangles(9,10,8,0,14,11,10,0), /* #<FACE 774> and #<FACE 2735> */
  gsSP1Triangle(14,13,12,0), /* #<FACE 3076> */
  gsSPVertex(&cont_finish_v[3939],13,0),
  gsSP2Triangles(9,3,6,0,4,5,7,0), /* #<FACE 748> and #<FACE 1392> */
  gsSP2Triangles(5,8,7,0,8,9,6,0), /* #<FACE 2752> and #<FACE 752> */
  gsSP2Triangles(7,8,6,0,11,9,8,0), /* #<FACE 751> and #<FACE 2751> */
  gsSP2Triangles(0,10,11,0,10,9,11,0), /* #<FACE 2302> and #<FACE 1367> */
  gsSP2Triangles(11,12,0,0,15,2,12,0), /* #<FACE 3078> and #<FACE 2299> */
  gsSP2Triangles(1,12,2,0,1,0,12,0), /* #<FACE 1370> and #<FACE 306> */
  gsSPVertex(&cont_finish_v[3952],13,3),
  gsSP2Triangles(4,5,3,0,4,8,5,0), /* #<FACE 727> and #<FACE 2318> */
  gsSP2Triangles(9,5,8,0,6,9,10,0), /* #<FACE 1351> and #<FACE 2316> */
  gsSP2Triangles(7,6,10,0,10,9,8,0), /* #<FACE 1357> and #<FACE 1353> */
  gsSP2Triangles(0,1,11,0,12,11,1,0), /* #<FACE 2304> and #<FACE 1365> */
  gsSP2Triangles(12,1,13,0,2,13,1,0), /* #<FACE 745> and #<FACE 2755> */
  gsSP2Triangles(2,15,13,0,14,12,13,0), /* #<FACE 2754> and #<FACE 747> */
  gsSP1Triangle(15,14,13,0), /* #<FACE 746> */
  gsSPVertex(&cont_finish_v[3965],15,0),
  gsSP2Triangles(5,2,4,0,2,0,4,0), /* #<FACE 2512> and #<FACE 1157> */
  gsSP2Triangles(1,4,0,0,5,4,3,0), /* #<FACE 603> and #<FACE 464> */
  gsSP2Triangles(6,8,9,0,8,7,10,0), /* #<FACE 2504> and #<FACE 2501> */
  gsSP2Triangles(10,9,8,0,11,14,12,0), /* #<FACE 1165> and #<FACE 2494> */
  gsSP1Triangle(14,13,12,0), /* #<FACE 1175> */
  gsSPVertex(&cont_finish_v[3980],14,2),
  gsSP2Triangles(14,3,15,0,15,3,2,0), /* #<FACE 2585> and #<FACE 1084> */
  gsSP2Triangles(4,7,5,0,7,6,5,0), /* #<FACE 2578> and #<FACE 1091> */
  gsSP2Triangles(9,10,8,0,11,10,9,0), /* #<FACE 2574> and #<FACE 600> */
  gsSP2Triangles(0,13,1,0,1,13,12,0), /* #<FACE 2514> and #<FACE 1155> */
  gsSPVertex(&cont_finish_v[3994],12,0),
  gsSP2Triangles(2,1,0,0,7,5,3,0), /* #<FACE 193> and #<FACE 2976> */
  gsSP2Triangles(4,3,5,0,7,6,5,0), /* #<FACE 414> and #<FACE 271> */
  gsSP2Triangles(14,15,10,0,8,10,15,0), /* #<FACE 2589> and #<FACE 1080> */
  gsSP2Triangles(14,10,11,0,11,10,9,0), /* #<FACE 503> and #<FACE 499> */
  gsSPVertex(&cont_finish_v[4006],14,2),
  gsSP2Triangles(4,3,2,0,5,7,8,0), /* #<FACE 2977> and #<FACE 277> */
  gsSP2Triangles(8,7,6,0,11,9,10,0), /* #<FACE 252> and #<FACE 3239> */
  gsSP2Triangles(12,11,10,0,15,14,13,0), /* #<FACE 102> and #<FACE 1002> */
  gsSPVertex(&cont_finish_v[4020],6,0),
  gsSP2Triangles(2,1,0,0,5,4,3,0), /* #<FACE 1168> and #<FACE 738> */
  gsSPPopMatrix(0),
  gsSPEndDisplayList(),
};
