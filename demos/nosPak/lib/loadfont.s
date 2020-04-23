/*---------------------------------------------------------------------
  File : loadfont.s	version 1.0

  Assembled from canvas.c
  Original C source by Yoshitaka Yasumoto.  Feb 20, 1996.
  Modified          by Koji Mitsunari.      Feb 28, 1996.
  Copyright         by Nintendo, Co., Ltd.          1997.
  ---------------------------------------------------------------------*/

#include	"pakmenu.h"

	.option	pic0

#if LANG_JAPANESE
	.globl	font_bitmaps
	.data	
	.align	2
	.align	0
font_bitmaps:
	.word	0 : 1
	.word	0 : 1
	.word	0 : 1
	.word	0 : 1
	.word	201363460 : 1
	.word	-2145910752 : 1
	.word	75501568 : 1
	.word	0 : 1
	.word	0 : 1
	.word	0 : 1
	.word	0 : 1
	.word	16389 : 1
	.word	1048576 : 1
	.word	0 : 1
	.word	0 : 1
	.word	0 : 1
	.word	0 : 1
	.word	0 : 1
	.word	0 : 1
	.word	4190208 : 1
	.word	-2147220448 : 1
	.word	570515472 : 1
	.word	8390656 : 1
	.word	0 : 1
	.word	2 : 1
	.word	1048832 : 1
	.word	268894664 : 1
	.word	4194816 : 1
	.word	268468224 : 1
	.word	0 : 1
	.word	67117087 : 1
	.word	-260013052 : 1
	.word	2097408 : 1
	.word	268468248 : 1
	.word	50331648 : 1
	.word	0 : 1
	.word	4064 : 1
	.word	134234114 : 1
	.word	1048704 : 1
	.word	67370496 : 1
	.word	0 : 1
	.word	0 : 1
	.word	134234367 : 1
	.word	-2146434688 : 1
	.word	335618097 : 1
	.word	101187648 : 1
	.word	0 : 1
	.word	64 : 1
	.word	554206274 : 1
	.word	571482113 : 1
	.word	1048832 : 1
	.word	805699584 : 1
	.word	0 : 1
	.word	32772 : 1
	.word	2589124 : 1
	.word	941629953 : 1
	.word	537133088 : 1
	.word	16777216 : 1
	.word	0 : 1
	.word	4064 : 1
	.word	16779264 : 1
	.word	1074004000 : 1
	.word	17301248 : 1
	.word	0 : 1
	.word	0 : 1
	.word	2093056 : 1
	.word	-2147213344 : 1
	.word	16779264 : 1
	.word	1140719616 : 1
	.word	262016 : 1
	.word	67117057 : 1
	.word	1073217600 : 1
	.word	33562625 : 1
	.word	1048832 : 1
	.word	806223872 : 1
	.word	50333184 : 1
	.word	135266432 : 1
	.word	67125250 : 1
	.word	2097664 : 1
	.word	537002080 : 1
	.word	1006632967 : 1
	.word	-4193792 : 1
	.word	272663048 : 1
	.word	272663560 : 1
	.word	-1069546496 : 1
	.word	537002016 : 1
	.word	2048 : 1
	.word	1074004032 : 1
	.word	67158076 : 1
	.word	236978432 : 1
	.word	134234114 : 1
	.word	1048704 : 1
	.word	33558559 : 1
	.word	-58711039 : 1
	.word	537395264 : 1
	.word	33558529 : 1
	.word	524416 : 1
	.word	403111936 : 1
	.word	67076224 : 1
	.word	67117057 : 1
	.word	524352 : 1
	.word	33558528 : 1
	.word	-2147217409 : 1
	.word	-2147483584 : 1
	.word	33558783 : 1
	.word	-268173216 : 1
	.word	50341890 : 1
	.word	1075970576 : 1
	.word	1619788800 : 1
	.word	537002000 : 1
	.word	8454112 : 1
	.word	1090652192 : 1
	.word	1090652176 : 1
	.word	-2130442170 : 1
	.word	1108090880 : 1
	.word	67117057 : 1
	.word	-252182464 : 1
	.word	33556480 : 1
	.word	1074001904 : 1
	.word	8389632 : 1
	.word	536936448 : 1
	.word	8356356 : 1
	.word	539033872 : 1
	.word	151027716 : 1
	.word	4194816 : 1
	.word	537002080 : 1
	.word	469766144 : 1
	.word	-2147221377 : 1
	.word	-234348478 : 1
	.word	33558528 : 1
	.word	-2147221440 : 1
	.word	33562638 : 1
	.word	0 : 1
	.word	536608800 : 1
	.word	16779264 : 1
	.word	1073872912 : 1
	.word	8389632 : 1
	.word	671023104 : 1
	.word	539033864 : 1
	.word	167768578 : 1
	.word	269516932 : 1
	.word	69214209 : 1
	.word	1048704 : 1
	.word	403112032 : 1
	.word	12583168 : 1
	.word	224 : 1
	.word	79700226 : 1
	.word	1048832 : 1
	.word	268632160 : 1
	.word	1006632963 : 1
	.word	-16775168 : 1
	.word	1074004000 : 1
	.word	33558529 : 1
	.word	1074331780 : 1
	.word	136413424 : 1
	.word	68157568 : 1
	.word	1073618961 : 1
	.word	8913984 : 1
	.word	1107562688 : 1
	.word	-2147221472 : 1
	.word	16744448 : 1
	.word	262288 : 1
	.word	75506177 : 1
	.word	269516928 : 1
	.word	134234116 : 1
	.word	4195328 : 1
	.word	-1072168960 : 1
	.word	8356356 : 1
	.word	539033878 : 1
	.word	151814164 : 1
	.word	4194816 : 1
	.word	537002080 : 1
	.word	469762096 : 1
	.word	505364482 : 1
	.word	1048707 : 1
	.word	-2088959 : 1
	.word	524416 : 1
	.word	67125276 : 1
	.word	529472 : 1
	.word	-1593538524 : 1
	.word	554766464 : 1
	.word	-2147221440 : 1
	.word	33562626 : 1
	.word	6298624 : 1
	.word	130816 : 1
	.word	0 : 1
	.word	2147222528 : 1
	.word	536936464 : 1
	.word	8390656 : 1
	.word	-2145910752 : 1
	.word	16779264 : 1
	.word	1073872924 : 1
	.word	9962544 : 1
	.word	541131016 : 1
	.word	4194816 : 1
	.word	268435712 : 1
	.word	134234623 : 1
	.word	-267386752 : 1
	.word	67117057 : 1
	.word	1048704 : 1
	.word	134250552 : 1
	.word	1023 : 1
	.word	-2147483648 : 1
	.word	0 : 1
	.word	0 : 1
	.word	0 : 1
	.word	268402688 : 1
	.word	262016 : 1
	.word	67117505 : 1
	.word	26215040 : 1
	.word	134234117 : 1
	.word	4457504 : 1
	.word	-1070071806 : 1
	.word	1064956 : 1
	.word	2097664 : 1
	.word	537002080 : 1
	.word	113299896 : 1
	.word	-2113667040 : 1
	.word	16777232 : 1
	.word	8389632 : 1
	.word	1073872912 : 1
	.word	16779264 : 1
	.word	-2146959232 : 1
	.word	134316080 : 1
	.word	66 : 1
	.word	34607168 : 1
	.word	-2113662912 : 1
	.word	1107431441 : 1
	.word	8913472 : 1
	.word	335585284 : 1
	.word	131088 : 1
	.word	75547672 : 1
	.word	587325448 : 1
	.word	4194816 : 1
	.word	268468227 : 1
	.word	-16777216 : 1
	.word	67076100 : 1
	.word	2097408 : 1
	.word	134250500 : 1
	.word	4194816 : 1
	.word	537002080 : 1
	.word	201326592 : 1
	.word	1610907682 : 1
	.word	34607169 : 1
	.word	33556480 : 1
	.word	536936452 : 1
	.word	2097280 : 1
	.word	67239952 : 1
	.word	8454128 : 1
	.word	536936521 : 1
	.word	38019617 : 1
	.word	277906564 : 1
	.word	404783360 : 1
	.word	262080 : 1
	.word	33558528 : 1
	.word	-2146959290 : 1
	.word	67649580 : 1
	.word	8389632 : 1
	.word	268468224 : 1
	.word	62915008 : 1
	.word	29360184 : 1
	.word	3932188 : 1
	.word	60 : 1
	.word	1966094 : 1
	.word	787456 : 1
	.word	536936456 : 1
	.word	8389632 : 1
	.word	537003024 : 1
	.word	545294364 : 1
	.word	1327464576 : 1
	.word	4096 : 1
	.word	-2147209152 : 1
	.word	1644220417 : 1
	.word	524448 : 1
	.word	75514372 : 1
	.word	281032704 : 1
	.word	262018 : 1
	.word	1048704 : 1
	.word	67370977 : 1
	.word	524352 : 1
	.word	33558528 : 1
	.word	2130706464 : 1
	.word	16779264 : 1
	.word	569503992 : 1
	.word	138428676 : 1
	.word	136331265 : 1
	.word	524352 : 1
	.word	33554433 : 1
	.word	-16775168 : 1
	.word	1073872912 : 1
	.word	8390656 : 1
	.word	1073872912 : 1
	.word	9437056 : 1
	.word	1023 : 1
	.word	-2147221472 : 1
	.word	16779519 : 1
	.word	-1073610736 : 1
	.word	8389632 : 1
	.word	671023104 : 1
	.word	261888 : 1
	.word	0 : 1
	.word	2146959424 : 1
	.word	33562625 : 1
	.word	1048832 : 1
	.word	1880883264 : 1
	.word	570494984 : 1
	.word	-2143026656 : 1
	.word	285247492 : 1
	.word	2097408 : 1
	.word	268468248 : 1
	.word	117444864 : 1
	.word	-2012987358 : 1
	.word	17827968 : 1
	.word	1140990241 : 1
	.word	151537732 : 1
	.word	-2111565248 : 1
	.word	-536870400 : 1
	.word	268468228 : 1
	.word	2097408 : 1
	.word	-2012987326 : 1
	.word	34611330 : 1
	.word	73415680 : 1
	.word	262096 : 1
	.word	41948160 : 1
	.word	-1610285016 : 1
	.word	20974080 : 1
	.word	1342341143 : 1
	.word	-8388608 : 1
	.word	67096578 : 1
	.word	-2146172768 : 1
	.word	83894273 : 1
	.word	1048704 : 1
	.word	134250520 : 1
	.half	1792 : 1
	.globl	canvas_ftbl
	.data	
	.align	2
	.align	0
canvas_ftbl:
	.word	186253578 : 1
	.word	621687610 : 1
	.globl	font_kana_dot
	.data	
	.align	2
	.align	0
font_kana_dot:
	.word	34145280 : 1
	.word	-68557581 : 1
	.word	117769984 : 1
	.word	-134875152 : 1
#else
	.globl	canvas_ftbl
	.data	
	.align	2
	.align	0
canvas_ftbl:
	.word	186253578 : 1
	.half	9486 : 1
#endif

	.globl	__font_buf
	.lcomm	__font_buf 4
	.globl	__cursor
	.lcomm	__cursor 4
 #-------------------------------------------------------------------------
 # void	nosLoadFont (u16 *font_buf)
 #-------------------------------------------------------------------------
	.text	
	.align	2
	.file	2 "canvas.c"
	.globl	nosLoadFont
	.loc	2 53
 #  53	{
	.ent	nosLoadFont 2
nosLoadFont:
	.option	O2
	subu	$sp, 32
	.frame	$sp, 32, $31
	.loc	2 53
	.loc	2 55
 #  54	  register u16	*fpix;
 #  55	  register u32	*farc = (u32 *)FONTARC_TOP+1;
	li	$6, FONTARC_TOP+4
 #	li	$6, -1342174348
	.loc	2 56
 #  56	  register u32	bitmap = *(farc-1);
	lw	$7, -4($6)
	.loc	2 57
 #  57	  register u32	position = 0;
	move	$8, $0
	.loc	2 62
 #  58	  register int	i;
 #  59	  register int	j;
 #  60	  int	k;
 #  61	
 #  62	  __font_buf = font_buf;
	sw	$4, __font_buf
	.loc	2 65
 #  63	

 #  64	  // Clear font buffer
 #  65	  for (i = 0; i < FONT_HT * (FONT_CODE_BTM - FONT_CODE_TOP + 2); i++){
	move	$9, $0
$32:
	.loc	2 65
	.loc	2 66
 #  66	     *font_buf++ = 0x0000;
	sh	$0, 0($4)
	addu	$4, $4, 2
	.loc	2 65
 #  65	  for (i = 0; i < FONT_HT * (FONT_CODE_BTM - FONT_CODE_TOP + 2); i++){
	addu	$9, $9, 1
	blt	$9, FONT_HT*(FONT_CODE_BTM-FONT_CODE_TOP+2), $32
$33:
	.loc	2 70
 #  66	     *font_buf++ = 0x0000;
 #  67	  }
 #  68	  
 #  69	  // Extract font
 #  70	  for (i = 0; i < sizeof(canvas_ftbl)/sizeof(FTBL); i ++){
	move	$9, $0
$34:
	.loc	2 70
	.loc	2 72

#if LANG_JAPANESE
 #  71	#if	LANG_JAPANESE
 #  72	    if (i == 3){
	bne	$9, 3, $35
	.loc	2 72
	.loc	2 73
 #  73	      farc = (u32 *)font_bitmaps;
	la	$6, font_bitmaps+4
	.loc	2 74
 #  74	      position  = 0;
	move	$8, $0
	.loc	2 75
 #  75	      bitmap = *farc ++;
	lw	$7, -4($6)
$35:
	.loc	2 78
 #  76	    }
 #  77	#endif
#endif

 #  78	    fpix = __font_buf +
 #  79	      (canvas_ftbl[i].code * FONT_HT + FONT_HOFS);
	mul	$14, $9, 2
	la	$15, canvas_ftbl
	addu	$24, $14, $15
	lbu	$11, 0($24)
	mul	$12, $11, FONT_HT*2
	addu	$13, $12, FONT_HOFS*2
	lw	$14, __font_buf
	addu	$5, $13, $14
	.loc	2 80
 #  80	    for (j = 0; j < canvas_ftbl[i].number; j ++){
	move	$10, $0

$36:
	.loc	2 80
	.loc	2 81
 #  81	      for (k = 0; k < FONTARC_HT; k ++){
	sw	$0, 4($sp)
$37:
	.loc	2 81
	.loc	2 82
 #  82		position += FONTARC_WD;
	addu	$8, $8, FONTARC_WD
	.loc	2 83
 #  83		if (position & ~0x1f) {
	move	$15, $0
	and	$12, $8, -32
	beq	$12, 0, $38
	.loc	2 83
	.loc	2 84
 #  84		  position &= 0x1f;
	and	$8, $8, 31
	.loc	2 85
 #  85		  *fpix = (u16)(bitmap << position)  & 0x1fff;
	sll	$14, $7, $8
	and	$15, $14, 0x1fff
	.loc	2 86
 #  86		  bitmap = *farc ++;
	lw	$7, 0($6)
	addu	$6, $6, 4
	.loc	2 87
 #  87		  if (!position) {
	beq	$8, 0, $39
	.loc	2 87
	.loc	2 88
 #  88		    goto nosLoadFont_00;
$38:
	.loc	2 91
 #  89		  }
 #  90		}
 #  91		*fpix |= (u16)(bitmap >> (32 - position)) & 0x1fff;
	negu	$11, $8
	srl	$12, $7, $11
	and	$14, $12, 0x1fff
	or	$15, $15, $14
	.loc	2 92
 #  92	      nosLoadFont_00:
$39:
	.loc	2 93
 #  93		fpix ++;
	sh	$15, 0($5)
	addu	$5, $5, 2
	.loc	2 81
 #  81	      for (k = 0; k < FONTARC_HT; k ++){
	lw	$15, 4($sp)
	addu	$11, $15, 1
	sw	$11, 4($sp)
	blt	$11, FONTARC_HT, $37
	.loc	2 108
 # 108	      fpix += (FONT_HT-FONTARC_HT);
	addu	$5, $5, (FONT_HT-FONTARC_HT)*2
	.loc	2 109
 # 109	      position += 2;
	addu	$8, $8, 2
	.loc	2 80
 #  80	    for (j = 0; j < canvas_ftbl[i].number; j ++){
	addu	$10, $10, 1
	mul	$12, $9, 2
	lbu	$13, canvas_ftbl+1($12)
	blt	$10, $13, $36
	.loc	2 70
 #  70	  for (i = 0; i < sizeof(canvas_ftbl)/sizeof(FTBL); i ++){
$40:
	addu	$9, $9, 1
#if LANG_JAPANESE
	bltu	$9, 4, $34
#else
	bltu	$9, 3, $34
#endif

#if LANG_JAPANESE
$41:
	.loc	2 115
 # 115	  for (i = F_ADRS(KA); i < F_ADRS(MA); i ++) {
	li	$9, 1120*2
$42:
	.loc	2 115
	.loc	2 116
 # 116	    (__font_buf+(F_ADRS(GA)-F_ADRS(KA)))[i] = __font_buf[i];
	lw	$25, __font_buf
	addu	$24, $25, $9
	lhu	$15, 0($24)
	sh	$15, 1248($24)
	.loc	2 115
 # 115	  for (i = F_ADRS(KA); i < F_ADRS(MA); i ++) {
	addu	$9, $9, 2
	blt	$9, 1520*2, $42
$43:
	.loc	2 118
 # 116	    (__font_buf+(F_ADRS(GA)-F_ADRS(KA)))[i] = __font_buf[i];
 # 117	  }
 # 118	  for (i = F_ADRS(HA); i < F_ADRS(MA); i ++) {
	li	$9, 1440*2
$44:
	.loc	2 118
	.loc	2 119
 # 119	    (__font_buf+(F_ADRS(BA)-F_ADRS(HA)))[i] = __font_buf[i];
	addu	$24, $25, $9
	lhu	$15, 0($24)
	sh	$15, 1088($24)
	.loc	2 118
 # 118	  for (i = F_ADRS(HA); i < F_ADRS(MA); i ++) {
	addu	$9, $9, 2
	blt	$9, 1520*2, $44
$45:
	.loc	2 122
 # 119	    (__font_buf+(F_ADRS(BA)-F_ADRS(HA)))[i] = __font_buf[i];
 # 120	  }  
 # 121	
 # 122	  k = 0;
	sw	$0, 4($sp)
	.loc	2 123
 # 123	  fpix = __font_buf + F_ADRS(GA);
	addu	$5, $25, 3488
	.loc	2 124
 # 124	  for (i = FONT_CODE_GA; i < FONT_CODE_PA+5; i ++){
	li	$9, 124
$46:
	.loc	2 124
	.loc	2 125
 # 125	    if (i == FONT_CODE_PA) k = 8;
	bne	$9, 144, $47
	.loc	2 125
	li	$12, 8
	sw	$12, 4($sp)
$47:
	.loc	2 126
 # 126	    for (j = 0; j < 4; j ++){
	move	$10, $0
$48:
	.loc	2 126
	.loc	2 127
 # 127	      fpix[j] = (fpix[j] | font_kana_dot[j+k]) & font_kana_dot[j+k+4];
	mul	$14, $10, 2
	addu	$13, $5, $14
	lhu	$24, 0($13)
	lw	$11, 4($sp)
	addu	$25, $10, $11
	la	$15, font_kana_dot
	addu	$12, $25, $15
	lb	$14, 0($12)
	or	$24, $24, $14
	lb	$25, 4($12)
	and	$14, $25, $24
	sh	$14, 0($13)
	.loc	2 126
 # 126	    for (j = 0; j < 4; j ++){
	addu	$10, $10, 1
	blt	$10, 4, $48
$49:
	.loc	2 144
 # 144	    fpix += FONT_HT;
	addu	$5, $5, 32
	.loc	2 124
 # 124	  for (i = FONT_CODE_GA; i < FONT_CODE_PA+5; i ++){
	addu	$9, $9, 1
	blt	$9, 149, $46
#endif
	.loc	2 147
 # 147	}
$50:
	.livereg	0x0000FF0E,0x00000FFF
	addu	$sp, 32
	j	$31
	.end	nosLoadFont
