/*---------------------------------------------------------------------
  File : pakmenu.s	version 1.5

  Assembled from pakmenu.c
  Original C source by Yoshitaka Yasumoto.  Feb 20, 1996.
  Modified          by Koji Mitsunari.      Oct 15, 1996.
  Copyright         by Nintendo, Co., Ltd.          1997,1998.
  ---------------------------------------------------------------------*/
#include	<PR/gbi.h>
#include	<PR/os.h>
#include	"pakmenu.h"
	
	.option	pic0

	.rdata	
	.align	2
	.align	0
$$7:					# ERASE / ERASE
	.ascii	STR_CMD_ERASE
	.rdata	
	.align	2
	.align	0
$$8:					# ERASE OK / ERASE OK
	.ascii	STR_CMD_OK
	.rdata	
	.align	2
	.align	0
$$9:					# CANSEL / CANCEL
	.ascii	STR_CMD_QUIT
	.rdata	
	.align	2
	.align	0
$$16:					# QUIT / QUIT
	.ascii	STR_CMD_END
	.data	
	.align	2
	.align	0
cmd_str:
	.word	$$7
	.word	$$8
	.word	$$9

	.data	
	.align	2
	.align	0
cmd_str_len:
#if LANG_JAPANESE
	.half	6 : 1			# sizeof(STR_CMD_OK) - 1
	.half	8 : 1			# sizeof(STR_CMD_QUIT) - 1
	.half	8 : 1			# sizeof(STR_CMD_END) - 1
#else
	.half	7 : 1			# sizeof(STR_CMD_OK) - 1
	.half	8 : 1			# sizeof(STR_CMD_QUIT) - 1
	.half	6 : 1			# sizeof(STR_CMD_END) - 1
#endif
	.data
	.align	2
	.align	0
cmd_str_yofs:
	.half	8 : 1
	.half	(MENU_IS_OK_YOFS*SCREEN_WD+8) : 1
	.half	(MENU_IS_NO_YOFS*SCREEN_WD+8) : 1

	.rdata	
	.align	2
	.align	0
$$34:					# GAME ...BLOCKS / GAME ~ TITLE
	.ascii	STR_MENU_ITEMS
	.rdata	
	.align	2
	.align	0
$$34a:
	.ascii	STR_MENU_TITLE
	.rdata	
	.align	2
	.align	0
$$34b:
	.ascii	STR_MENU_VER
	.rdata	
	.align	2
	.align	0
$$35:					# NOT USED = / Remainder =
	.ascii	STR_BLK_LEFT
	.rdata	
	.align	2
	.align	0
$$36:					# NO DATA TO ERASE /Nothing erased
	.ascii	STR_PAK_NODEL
	.rdata	
	.align	2
	.align	0
$$42:
	.ascii	STR_PAK_IDERR
	.rdata	
	.align	2
	.align	0
$$43:
	.ascii	STR_PAK_IDERR1
	.rdata	
	.align	2
	.align	0
$$44:
	.ascii	STR_PAK_IDERR2

	.data	
	.align	2
	.align	0
cursor_color:
	.word	-972691709 : 1
	.word	-142475327 : 1
	.word	-142481661 : 1
	.word	-972717045 : 1
	.word	1108444171 : 1
	.globl	pfs_stat
	.lcomm	pfs_stat 512
	.globl	pfs_stat_ret
	.lcomm	pfs_stat_ret 64
	.extern	__cursor 4
 #-------------------------------------------------------------------------
 # void	__display_term (int sel, int n, u16 fg1, u16 fg2, u16 bg)
 #-------------------------------------------------------------------------
	.text	
	.align	2
	.file	2 "pakmenu.c"
	.globl	__display_term
	.loc	2 23
 #  23	{
	.ent	__display_term 2
__display_term:
	.option	O2
	subu	$sp, 56
	sw	$31, 36($sp)
	sw	$19, 32($sp)
	sw	$18, 28($sp)
	sw	$17, 24($sp)
	sw	$16, 20($sp)
	.mask	0x800F0000, -20
	.frame	$sp, 56, $31
	sw	$4, 56($sp)		# sel
	move	$17, $5			# n
	move	$18, $6			# fg1
	sw	$7, 68($sp)		# fg2
	lhu	$19, 74($sp)		# bg
	.loc	2 23
	.loc	2 39
 #  39	  cy = n * FONT_HSKIP + MENU_FILE_Y;
	.loc	2 40
 #  40	  if ( n < 16 ){
	move	$6, $18
	move	$7, $19
	bge	$17, 16, $36
	.loc	2 40
	.loc	2 45
 #  41	    
 #  42	    //
 #  43	    // Check if file exists
 #  44	    // --------------------
 #  45	    if ( !pfs_stat_ret[n] ){
	mul	$14, $17, 4
	lw	$15, pfs_stat_ret($14)
	bne	$15, 0, $33
	.loc	2 45
	.loc	2 50
 #  46	      
 #  47	      //
 #  48	      // Draw filename and # of blocks
 #  49	      // -----------------------------
 #  50	      canvas_cursor( MENU_FILE_X, cy );
	mul	$24, $17, (FONT_HSKIP*1280)
	addu	$16, $24, (MENU_FILE_Y*1280)
	addu	$25, $16, (FBUF_TOP+MENU_FILE_X*2)
	sw	$25, __cursor
	.loc	2 51
 #  51	      canvas_puts( pfs_stat[n].game_name, 16, fg1, bg );
	mul	$8, $17, 32
	la	$9, pfs_stat
	addu	$2, $8, $9
	addu	$4, $2, 14
	li	$5, 16
	sw	$2, 44($sp)
	.livereg	0x2F00000E,0x00000000
	jal	canvas_puts
	.loc	2 52
 #  52	      if (pfs_stat[n].ext_name[0]){
	lw	$10, 44($sp)
	lbu	$11, 10($10)
	beq	$11, 0, $32
	.loc	2 52
	.loc	2 53
 #  53		canvas_putc( 0x3c, fg1, bg );
	li	$4, 60
	move	$5, $18
	move	$6, $19
	.livereg	0x0E00000E,0x00000000
	jal	canvas_putc
	la	$2, __cursor
	.loc	2 54
 #  54		__cursor -= MENU_EXT_X;
	.noalias	$2,$sp
	lw	$12, 0($2)
	addu	$13, $12, -16
	sw	$13, 0($2)
	.loc	2 55
 #  55		canvas_putc( pfs_stat[n].ext_name[0], fg1, bg );
	lw	$14, 44($sp)
	lbu	$4, 10($14)
	.livereg	0x0E00000E,0x00000000
	jal	canvas_putc
	.alias	$2,$sp
$32:
	.loc	2 57
 #  56	      }
 #  57	      canvas_cursor( MENU_BLOCK_X, cy );
	addu	$15, $16, (FBUF_TOP+MENU_BLOCK_X*2)
	sw	$15, __cursor
	.loc	2 58
 #  58	      canvas_u8_to_str( (u16)((pfs_stat[n].file_size+255) >> 8), ibuf );
	lw	$24, 44($sp)
	lw	$4, 0($24)
	addu	$4, $4, 255
	srl	$25, $4, 8
	move	$4, $25
	addu	$5, $sp, 52
	.livereg	0x0C00000E,0x00000000
	jal	canvas_u8_to_str
	.loc	2 59
 #  59	      canvas_puts( ibuf, 3, fg1, bg ); 
	addu	$4, $sp, 52
	li	$5, 3
	move	$6, $18
	move	$7, $19
	.livereg	0x0F00000E,0x00000000
	jal	canvas_puts
	lw	$3, 56($sp)
	.loc	2 64
 #  60	
 #  61	      //
 #  62	      // Draw "ERASE"
 #  63	      // ------------
 #  64	      __cursor += cmd_str_yofs[sel];
	mul	$2, $3, 2
	lw	$9, __cursor
	lh	$10, cmd_str_yofs($2)
	sll	$11, $10, 1
	addu	$12, $9, $11
	sw	$12, __cursor
	.loc	2 65
 #  65	      canvas_puts( cmd_str[sel], cmd_str_len[sel], fg2, bg );
	mul	$13, $3, 4
	lw	$4, cmd_str($13)
	lhu	$5, cmd_str_len($2)
	lhu	$6, 70($sp)
	and	$7, $19, 65535
	.livereg	0x0F00000E,0x00000000
	jal	canvas_puts
$33:
	.loc	2 71
 #  66	    }
 #  67	    
 #  68	    //
 #  69	    // Draw # (1-16):
 #  70	    // --------------
 #  71	    if (n < 9){
	move	$5, $18			# fg
	move	$6, $19			# bg
	mul	$16, $17, (FONT_HSKIP*1280)
	addu	$16, $16, (MENU_FILE_Y*1280)
	bge	$17, 9, $34
	.loc	2 71
	.loc	2 72
 #  72	      canvas_cursor( MENU_NUM_X + 3, cy );
	addu	$15, $16, (FBUF_TOP+(MENU_NUM_X + 3)*2)
	sw	$15, __cursor
	.loc	2 73
 #  73	      canvas_putc  ( n+0x11, fg1, bg );
	addu	$4, $17, 17
	.livereg	0x0E00000E,0x00000000
	jal	canvas_putc
	b	$35
$34:
	.loc	2 74
 #  74	    } else {
	.loc	2 75
 #  75	      canvas_cursor( MENU_NUM_X - 4, cy );
	addu	$24, $16, (FBUF_TOP+(MENU_NUM_X - 4)*2)
	sw	$24, __cursor
	.loc	2 76
 #  76	      canvas_putc( 0x11, fg1, bg );
	li	$4, 17
	.livereg	0x0E00000E,0x00000000
	jal	canvas_putc
	.loc	2 77
 #  77	      canvas_cursor( MENU_NUM_X + 4, cy );
	addu	$25, $16, (FBUF_TOP+(MENU_NUM_X + 4)*2)
	sw	$25, __cursor
	.loc	2 78
 #  78	      canvas_putc( n+7, fg1, bg );
	addu	$4, $17, 7
	.livereg	0x0E00000E,0x00000000
	jal	canvas_putc
$35:
	.loc	2 80
 #  79	    }
 #  80	    canvas_cursor( MENU_NUM_X + 15, cy );
	addu	$8, $16, (FBUF_TOP+(MENU_NUM_X + 15)*2)
	sw	$8, __cursor
	.loc	2 81
 #  81	    canvas_putc( 0x3e, fg1, bg );
	li	$4, 62
	.livereg	0x0E00000E,0x00000000
	jal	canvas_putc
	b	$37
$36:
	.loc	2 83
 #  82	    
 #  83	  } else {
	.loc	2 88
 #  84	      
 #  85	    //
 #  86	    // Draw "QUIT"
 #  87	    // -----------
 #  88	    canvas_cursor( MENU_NUM_X-2, cy+4 );
	mul	$9, $17, (FONT_HSKIP*1280)
	addu	$11, $9, FBUF_TOP+(MENU_NUM_X-2)*2+(MENU_FILE_Y+4)*1280
	sw	$11, __cursor
	.loc	2 89
 #  89	    canvas_putstr( STR_CMD_END, fg1, bg );
	la	$4, $$16

#if LANG_JAPANESE
	li	$5, 3
#else
	li	$5, 4
#endif
	.livereg	0x0F00000E,0x00000000
	jal	canvas_puts
	.loc	2 92
 #  90	  }
 #  91	    
 #  92	}
$37:
	.livereg	0x0000FF0E,0x00000FFF
	lw	$16, 20($sp)
	lw	$17, 24($sp)
	lw	$18, 28($sp)
	lw	$19, 32($sp)
	lw	$31, 36($sp)
	addu	$sp, 56
	j	$31
	.end	__display_term

 #-------------------------------------------------------------------------
 # void	__display_free_block(u8 free_blocks)
 #-------------------------------------------------------------------------
	.text	
	.align	2
	.file	2 "pakmenu.c"
	.globl	__display_free_block
	.loc	2 95
 #  93	
 #  94	void	__display_free_block(u8 free_blocks)
 #  95	{
	.ent	__display_free_block 2
__display_free_block:
	.option	O2
	subu	$sp, 48
	sw	$31, 20($sp)
	.mask	0x80000000, -28
	.frame	$sp, 48, $31
	sw	$4, 48($sp)
	and	$14, $4, 65535
	move	$4, $14
	.loc	2 95
	.loc	2 101
 #  96	  u8	ibuf[4];
 #  97	  int	i;
 #  98	  int	j;
 #  99	  int	k;
 # 100	  
 # 101	  canvas_u8_to_str(free_blocks, ibuf);
	addu	$5, $sp, 44

 #	bleu	$4, 255, $37_1
 #	li	$4, 255
 #$37_1:		

	.livereg	0x0C00000E,0x00000000
	jal	canvas_u8_to_str
	.loc	2 102
 # 102	  canvas_cursor( MENU_FREE_BLK_X2, MENU_FREE_BLK_Y2 );
	li	$15, FBUF_ADRS( MENU_FREE_BLK_X2, MENU_FREE_BLK_Y2 )
	sw	$15, __cursor
	.loc	2 104
 # 103	
 # 104	  for (i = 0; i < FONT_HT; i ++){
	move	$2, $0
	move	$5, $0
	li	$6, 10240
	li	$4, 48
$38:
	.loc	2 104
	.loc	2 105
 # 105	    for (j = 0; j < FONT_WSKIP * 3; j ++){
	move	$3, $0
	mul	$2, $5, 2
	addu	$2, $2, FBUF_ADRS(MENU_FREE_BLK_X2, MENU_FREE_BLK_Y2)
$39:
	.loc	2 105
	.loc	2 106
 # 106	      k = i * SCREEN_WD + j;
	.loc	2 107
 # 107	      ((u16 *)FBUF_ADRS(MENU_FREE_BLK_X2, MENU_FREE_BLK_Y2))[k] = (u16)BGCOLOR;
	sh	$0, 0($2)
	.loc	2 105
 # 105	    for (j = 0; j < FONT_WSKIP * 3; j ++){
	sh	$0, 2($2)
	sh	$0, 4($2)
	sh	$0, 6($2)
	addu	$3, $3, 4
	addu	$2, $2, 8
	bne	$3, $4, $39
	.loc	2 104
 # 104	  for (i = 0; i < FONT_HT; i ++){
	addu	$5, $5, 640
	bne	$5, $6, $38
	.loc	2 110
 # 105	    for (j = 0; j < FONT_WSKIP * 3; j ++){
 # 106	      k = i * SCREEN_WD + j;
 # 107	      ((u16 *)FBUF_ADRS(MENU_FREE_BLK_X2, MENU_FREE_BLK_Y2))[k] = (u16)BGCOLOR;
 # 108	    }
 # 109	  }
 # 110	  canvas_puts( ibuf, 3, MENU_COLOR_FILE, MENU_COLOR_SHADOW );
	addu	$4, $sp, 44
	li	$5, 3
	li	$6, 65535
	li	$7, 1
	.livereg	0x0F00000E,0x00000000
	jal	canvas_puts
	.loc	2 111
 # 111	}
	.livereg	0x0000FF0E,0x00000FFF
	lw	$31, 20($sp)
	addu	$sp, 48
	j	$31
	.end	__display_free_block

 #-------------------------------------------------------------------------
 # int  nosPakMenu( OSMesgQueue *siMessageQ, OSMesgQueue *retraceMessageQ )
 #-------------------------------------------------------------------------
	.text	
	.align	2
	.file	2 "pakmenu.c"
	.globl	nosPakMenu
	.loc	2 118
 # 113	
 # 114	//
 # 115	//  Returns by 0 if "PAK MENU" no used, 1 if "PAK MENU" used.
 # 116	//
 # 117	int	nosPakMenu( OSMesgQueue *siMessageQ, OSMesgQueue *retraceMessageQ )
 # 118	{
	.ent	nosPakMenu 2
nosPakMenu:
	.option	O2
	subu	$sp, 288
	sw	$31, 68($sp)
	sw	$30, 64($sp)
	sw	$23, 60($sp)
	sw	$22, 56($sp)
	sw	$21, 52($sp)
	sw	$20, 48($sp)
	sw	$19, 44($sp)
	sw	$18, 40($sp)
	sw	$17, 36($sp)
	sw	$16, 32($sp)
	.mask	0xC0FF0000, -220
	.frame	$sp, 288, $31
	sw	$4, 288($sp)
	sw	$5, 292($sp)
	.loc	2 118
	.loc	2 149
 # 149	  osContSetCh(PORT_ID+1);			// Check for PORT 1
	li	$4, PORT_ID+1
	.livereg	0x0800000E,0x00000000
	jal	osContSetCh
	.loc	2 150
 # 150	  osContInit(siMessageQ, &exist, stat);		// Initialize
	lw	$4, 288($sp)
	addu	$5, $sp, 243
	addu	$6, $sp, 268
	.livereg	0x0E00000E,0x00000000
	jal	osContInit
	.loc	2 151
 # 151	  if (!exist) return 0;		 		// Return if no connect
	lbu	$14, 243($sp)
	bne	$14, 0, $40
	.loc	2 151
	move	$2, $0
	b	$82
$40:
	.loc	2 156
 # 152	
 # 153	  //
 # 154	  // Check if any device plugged on
 # 155	  // ------------------------------
 # 156	  if (!(stat[PORT_ID].status & CONT_CARD_ON)) return 0;
	lbu	$15, (268+PORT_ID*4+2)($sp)
	and	$24, $15, 1
	bne	$24, 0, $41
	.loc	2 156
	move	$2, $0
	b	$82
$41:
	.loc	2 161
 # 157	  
 # 158	  //
 # 159	  // Check if START button pushed
 # 160	  // ----------------------------
 # 161	  osContStartReadData(siMessageQ);		// Scan controller data
	lw	$4, 288($sp)
	.livereg	0x0800000E,0x00000000
	jal	osContStartReadData
	.loc	2 162
 # 162	  nWaitMesg(si);
	lw	$4, 288($sp)
	addu	$5, $sp, 284
	li	$6, 1
	.livereg	0x0E00000E,0x00000000
	jal	osRecvMesg
	.loc	2 163
 # 163	  osContGetReadData(data);			// Get controller data
	addu	$4, $sp, 244
	.livereg	0x0800000E,0x00000000
	jal	osContGetReadData

 #-------------------------------------------------------------	
	.loc	2 171
 # 167	
 # 168	  //
 # 169	  // Check if CONTROLLER PAK plugged
 # 170	  // -------------------------------
 # 171	  if (data[PORT_ID].button & START_BUTTON) {
	lhu	$25, (244+PORT_ID*6)($sp)
	and	$8, $25, 4096
	beq	$8, 0, $47X
	.loc	2 171
	.loc	2 172
 # 172	    mode = osPfsInitPak(siMessageQ, &pfs, PORT_ID);
	lw	$4, 288($sp)
	addu	$5, $sp, 128
	li	$6, PORT_ID
	.livereg	0x0E00000E,0x00000000
	jal	osPfsInitPak
	move	$22, $2
	.loc	2 173
 # 173	    if (mode != 0 && mode != PFS_ERR_ID_FATAL) {
	beq	$2, 0, $42
	beq	$2, 10, $42
	.loc	2 173
	.loc	2 174
 # 174	      return 0; // Return if no Controller Pak
	move	$2, $0
	b	$82
$42:
	.loc	2 179
 # 175	    }
 # 176	  //
 # 177	  // Load font
 # 178	  // --------- 
 # 179	    nosLoadFont((u16 *)FONT_BUF);
	li	$4, FONT_BUF
	.livereg	0x0800000E,0x00000000
	jal	nosLoadFont
	.loc	2 183
 # 180	  //
 # 181	  // Clear screen
 # 182	  // -----------------
 # 183	    for (i = 0; i < FBUF_SIZE / sizeof(u32); i ++){
	move	$16, $0
	li	$2, FBUF_TOP
	li	$3, (FBUF_TOP+SCREEN_WD*SCREEN_HT*2)
$43X:
	.loc	2 183
	.loc	2 184
 # 184	      ((u32 *)FBUF_TOP)[i] = (BGCOLOR << 16) | BGCOLOR;
	sw	$0, 0($2)
	.loc	2 183
 # 183	    for (i = 0; i < FBUF_SIZE / sizeof(u32); i ++){
	sw	$0, 4($2)
	sw	$0, 8($2)
	sw	$0, 12($2)
	addu	$2, $2, 16
	bne	$2, $3, $43X
	.loc	2 186
 # 184	      ((u32 *)FBUF_TOP)[i] = (BGCOLOR << 16) | BGCOLOR;
 # 185	    }
 # 186	    osViSwapBuffer((u32 *)FBUF_TOP);
	li	$4, FBUF_TOP
	.livereg	0x0800000E,0x00000000
	jal	osViSwapBuffer

	jal	osWritebackDCacheAll
	lw	$4, 292($sp)
	addu	$5, $sp, 284
	li	$6, 1
	jal	osRecvMesg
	
	.loc	2 190
 # 187	#if	SCREEN_LOW
 # 188	    osViSetMode(&osViModeTable[OS_VI_NTSC_LPN1]);
 # 189	#else
 # 190	    osViSetMode(&osViModeTable[OS_VI_NTSC_HPF1]);
 #	la	$4, osViModeTable+720
 #	.livereg	0x0800000E,0x00000000
 #	jal	osViSetMode
	.loc	2 192
 # 191	#endif
 # 192	    osViBlack(0);
	move	$4, $0
	.livereg	0x0800000E,0x00000000
	jal	osViBlack
	.loc	2 194
 # 193	  
 # 194	    if ( mode == PFS_ERR_ID_FATAL ) {
	bne	$22, 10, $43
	.loc	2 194
	.loc	2 195
 # 195	      canvas_cursor( MENU_IDERR_X, MENU_IDERR_Y );
	li	$9, FBUF_ADRS(MENU_IDERR_X, MENU_IDERR_Y)
	sw	$9, __cursor
	.loc	2 196
 # 196	      canvas_putstr( STR_PAK_IDERR, MENU_COLOR_IDERR, MENU_COLOR_TITLE );
	la	$4, $$42
#if	LANG_JAPANESE
	li	$5, 15
#else
	li	$5, 20
#endif
	li	$6, MENU_COLOR_IDERR
	li	$7, MENU_COLOR_TITLE
	.livereg	0x0F00000E,0x00000000
	jal	canvas_puts
	.loc	2 197
 # 197	      canvas_cursor( MENU_IDERR_X1, MENU_IDERR_Y1 );
	li	$10, FBUF_ADRS(MENU_IDERR_X1, MENU_IDERR_Y1)
	sw	$10, __cursor
	.loc	2 198
 # 198	      canvas_putstr( STR_PAK_IDERR1, MENU_COLOR_TITLE, MENU_COLOR_TITLE );
	la	$4, $$43
#if	LANG_JAPANESE
	li	$5, 13
#else
	li	$5, 28
#endif
	li	$6, 65535
	li	$7, 65535
	.livereg	0x0F00000E,0x00000000
	jal	canvas_puts
	.loc	2 199
 # 199	      canvas_cursor( MENU_IDERR_X2, MENU_IDERR_Y2 );
	li	$11, FBUF_ADRS(MENU_IDERR_X2, MENU_IDERR_Y2)
	sw	$11, __cursor
	.loc	2 200
 # 200	      canvas_putstr( STR_PAK_IDERR2, MENU_COLOR_TITLE, MENU_COLOR_TITLE );
	la	$4, $$44
#if	LANG_JAPANESE
	li	$5, 14
#else
	li	$5, 24
#endif
	li	$6, 65535
	li	$7, 65535
	.livereg	0x0F00000E,0x00000000
	jal	canvas_puts
	.loc	2 201
 # 201	      osWritebackDCacheAll();
	.livereg	0x0000000E,0x00000000
	jal	osWritebackDCacheAll
	.loc	2 202
 # 202	      nWaitMesg(retrace);
	lw	$4, 292($sp)
	addu	$5, $sp, 284
	li	$6, 1
	.livereg	0x0E00000E,0x00000000
	jal	osRecvMesg
	.loc	2 203
 # 203	      do {
	li	$16, 48
$44X:
	.loc	2 203
	.loc	2 204
 # 204		osContStartReadData(siMessageQ);	// Scan controller data
	lw	$4, 288($sp)
	.livereg	0x0800000E,0x00000000
	jal	osContStartReadData
	.loc	2 205
 # 205		nWaitMesg(si);
	lw	$4, 288($sp)
	addu	$5, $sp, 284
	li	$6, 1
	.livereg	0x0E00000E,0x00000000
	jal	osRecvMesg
	.loc	2 206
 # 206	        osContGetReadData(data);		// Get controller data
	addu	$4, $sp, 244
	.livereg	0x0800000E,0x00000000
	jal	osContGetReadData
	.loc	2 207
 # 207	        if ( data[PORT_ID].button == 0 ) {
	lhu	$4, 244($sp)
	beq	$4, 0, $46X
	.loc	2 207
	.loc	2 208
 # 208		  continue;
	.loc	2 209
 # 209		} else if ( data[PORT_ID].button == (L_TRIG|R_TRIG) ) {
	bne	$16, $4, $45X
	.loc	2 209
	.loc	2 210
 # 210		  osPfsRepairId(&pfs);
	addu	$4, $sp, 128
	.livereg	0x0800000E,0x00000000
	jal	__osGetId
	.loc	2 210
$45X:
	.loc	2 212
 # 211		}
 # 212		mode = osPfsInitPak(siMessageQ, &pfs, PORT_ID);	/* Any button */
	lw	$4, 288($sp)
	addu	$5, $sp, 128
	move	$6, $0
	.livereg	0x0E00000E,0x00000000
	jal	osPfsInitPak
	move	$22, $2
$46X:
	bne	$22, 0, $44X
	b	$43
$47X:
	.loc	2 215
 # 213	      } while (mode != 0);
 # 214	    }
 # 215	  } else {
	.loc	2 216
 # 216	    return 2;// Return if not START
	li	$2, 2
	b	$82
	
 #-------------------------------------------------------------	
	
$43:
	.loc	2 178
 # 174	  
 # 175	  //
 # 176	  // Get number of files
 # 177	  // -------------------
 # 178	  if (osPfsNumFiles(&pfs, &file_num_max, &file_num)) return 0;
	addu	$4, $sp, 128
	addu	$5, $sp, 120
	addu	$6, $sp, 124
	.livereg	0x0E00000E,0x00000000
	jal	osPfsNumFiles
	beq	$2, 0, $44
	.loc	2 178
	move	$2, $0
	b	$82
$44:
	.loc	2 183
 # 179	  
 # 180	  //
 # 181	  // Get file status
 # 182	  // ---------------
 # 183	  for (i = 0; i < 16; i ++){
	move	$16, $0
	la	$17, pfs_stat_ret
	li	$18, 16
$45:
	.loc	2 183
	.loc	2 184
 # 184	    pfs_stat_ret[i] = osPfsFileState(&pfs, i, pfs_stat+i);
	addu	$4, $sp, 128
	move	$5, $16
	sll	$9, $16, 5
	la	$10, pfs_stat
	addu	$6, $9, $10
	.livereg	0x0E00000E,0x00000000
	jal	osPfsFileState
	.noalias	$17,$sp
	sw	$2, 0($17)
	.loc	2 183
 # 183	  for (i = 0; i < 16; i ++){
	addu	$16, $16, 1
	addu	$17, $17, 4
	bne	$16, $18, $45
	.alias	$17,$sp
	.loc	2 190
 # 190	  if (osPfsFreeBlocks(&pfs, &free_blocks)) return 0;
	addu	$4, $sp, 128
	addu	$5, $sp, 116
	.livereg	0x0C00000E,0x00000000
	jal	osPfsFreeBlocks
	beq	$2, 0, $46
	.loc	2 190
	move	$2, $0
	b	$82
$46:
	lw	$2, 116($sp)
	.loc	2 191
 # 191	  free_blocks >>= 8;
	sra	$11, $2, 8
	sw	$11, 116($sp)
 # 197	  
 # 198	  //
 # 199	  // Initialize screen
 # 200	  // -----------------
 # 201	  screen_type = STYPE_MENU;
	sw	$0, 88($sp)
	.loc	2 202
 # 202	 display_screen:
	sh	$0, 234($sp)
	move	$22, $0
	li	$17, 16
	li	$18, 1024
	move	$23, $0
	move	$21, $0
	li	$20, 17
	la	$19, pfs_stat_ret
$47:
	.loc	2 203
 # 203	  for (i = 0; i < FBUF_SIZE / sizeof(u32); i ++){
	move	$16, $0
	li	$2, FBUF_TOP
	li	$3, (SCREEN_WD*SCREEN_HT/2)
$48:
	.loc	2 203
	.loc	2 204
 # 204	    ((u32 *)FBUF_TOP)[i] = (BGCOLOR << 16) | BGCOLOR;
	sw	$0, 0($2)
	.loc	2 203
 # 203	  for (i = 0; i < FBUF_SIZE / sizeof(u32); i ++){
	addu	$16, $16, 1
	addu	$2, $2, 4
	bltu	$16, $3, $48
	.loc	2 207
 # 204	    ((u32 *)FBUF_TOP)[i] = (BGCOLOR << 16) | BGCOLOR;
 # 205	  }
 # 206	  
 # 207	  switch(screen_type){
	lw	$12, 88($sp)
	beq	$12, 0, $49
	beq	$12, 1, $51
	b	$52
$49:
	li	$13, FBUF_ADRS( MENU_TITLE_X, MENU_TITLE_Y )
	sw	$13, __cursor
	la	$4, $$34a
#if	LANG_JAPANESE
	li	$5, 16
#else
	li	$5, 20
#endif
	li	$6, 0x843f
	li	$7, 1
	jal	canvas_puts
#ifdef	OS_PFS_VERSION
	li	$13, FBUF_ADRS( MENU_TITLE_X+19*16, MENU_TITLE_Y )
	sw	$13, __cursor
	la	$4, $$34b
	li	$5, 4
	li	$6, 0x843f
	li	$7, 1
	jal	canvas_puts
	li	$4, (OS_PFS_VERSION_HI+0x10)
	li	$5, 0x843f
	li	$6, 1
	jal	canvas_putc
	li	$4, 0x3c
	li	$5, 0x843f
	li	$6, 1
	jal	canvas_putc
	li	$4, (OS_PFS_VERSION_LO+0x10)
	li	$5, 0x843f
	li	$6, 1
	jal	canvas_putc
#endif	
	.loc	2 212
 # 208	  case	STYPE_MENU:
 # 209	    //
 # 210	    // Display table of game data
 # 211	    // --------------------------
 # 212	    canvas_cursor( MENU_NUM_X, MENU_FILE_Y-FONT_HSKIP );
	li	$13, FBUF_ADRS( MENU_NUM_X, MENU_FILE_Y-FONT_HSKIP )
	sw	$13, __cursor
	.loc	2 213
 # 213	    canvas_putstr( STR_MENU_ITEMS, MENU_COLOR_ITEM, MENU_COLOR_SHADOW );
	la	$4, $$34
	li	$5, 46
	li	$6, 65535
	li	$7, 1
	.livereg	0x0F00000E,0x00000000
	jal	canvas_puts
	.loc	2 214
 # 214	    canvas_cursor( MENU_FREE_BLK_X1, MENU_FREE_BLK_Y1 );
	li	$14, FBUF_TOP+(MENU_FREE_BLK_X1+MENU_FREE_BLK_Y1*SCREEN_WD)*2
	sw	$14, __cursor
	.loc	2 215
 # 215	    canvas_putstr( STR_BLK_LEFT, MENU_COLOR_ITEM, MENU_COLOR_SHADOW );
	la	$4, $$35
#if LANG_JAPANESE
	li	$5, 4
#else
	li	$5, 9
#endif
	li	$6, 65535
	li	$7, 1
	.livereg	0x0F00000E,0x00000000
	jal	canvas_puts
	lw	$2, 116($sp)
	.loc	2 216
 # 216	    __display_free_block( (u16)(free_blocks) );
	and	$4, $2, 65535
	.livereg	0x0800000E,0x00000000
	jal	__display_free_block
	.loc	2 218
 # 217	    
 # 218	    for (i = 0; i < 17; i ++){
	move	$16, $0
$50:
	.loc	2 218
	.loc	2 219
 # 219	      __display_term( 0, i,
	move	$4, $0
	move	$5, $16
	li	$6, 65535
	li	$7, 1
	li	$15, 1
	sw	$15, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	__display_term
	.loc	2 218
 # 218	    for (i = 0; i < 17; i ++){
	addu	$16, $16, 1
	bne	$16, $20, $50
	.loc	2 222
 # 219	      __display_term( 0, i,
 # 220			      MENU_COLOR_FILE, MENU_COLOR_SHADOW, MENU_COLOR_SHADOW );
 # 221	    }
 # 222	    break;
	b	$52
$51:
	.loc	2 228
 # 223	    
 # 224	  case	STYPE_NODEL:
 # 225	    //
 # 226	    // Display message "Cannot delete file"
 # 227	    // ------------------------------------
 # 228	    canvas_cursor( MENU_WARN_X, MENU_WARN_Y );
	li	$24, FBUF_ADRS( MENU_WARN_X, MENU_WARN_Y )
	sw	$24, __cursor
	.loc	2 229
 # 229	    canvas_putstr( STR_PAK_NODEL, MENU_COLOR_TITLE, MENU_COLOR_TITLE );
	la	$4, $$36
#if LANG_JAPANESE
	li	$5, 8
#else
	li	$5, 16
#endif
	li	$6, 65535
	li	$7, 65535
	.livereg	0x0F00000E,0x00000000
	jal	canvas_puts
	.loc	2 230
 # 230	    file_num = 0;
	sw	$0, 124($sp)
	.loc	2 231
 # 231	    break;
$52:
	.loc	2 234
 # 232	  }
 # 233	  
 # 234	  osWritebackDCacheAll();
	.livereg	0x0000000E,0x00000000
	jal	osWritebackDCacheAll
	.loc	2 235
 # 235	  nWaitMesg(retrace);
	lw	$4, 292($sp)
	addu	$5, $sp, 284
	li	$6, 1
	.livereg	0x0E00000E,0x00000000
	jal	osRecvMesg
 # 246	  
 # 247	  repeat_cnt  = 0;
	.loc	2 248
 # 248	  repeat_trig = 0;
	.loc	2 249
 # 249	  target    = 16;
	.loc	2 250
 # 250	  trig      = D_JPAD;
	.loc	2 251
 # 251	  flash_cnt = 0;
	.loc	2 252
 # 252	  mode      = 0;
	.loc	2 253
 # 253	  while(1){
	li	$30, 16
	sw	$0, 108($sp)
$53:
	.loc	2 253
	.loc	2 254
 # 254	    save_target = target;
	.loc	2 255
 # 255	    save_mode   = mode;
	.loc	2 256
 # 256	    switch(mode){
	beq	$21, 0, $54
	beq	$21, 1, $66
	beq	$21, 2, $70
	b	$72
$54:
	.loc	2 258
 # 257	    case	0:
 # 258	      if (trig & A_BUTTON){
	move	$2, $18
	and	$25, $2, 32768
	beq	$25, 0, $58
	.loc	2 258
	.loc	2 261
 # 259		
 # 260		// Return if "QUIT" selected
 # 261		if (target == 16) {
	bne	$17, 16, $56
	.loc	2 261
	.loc	2 262
 # 262		  if (screen_type == STYPE_NODEL) {
	lw	$8, 88($sp)
	bne	$8, 1, $55
	.loc	2 262
	.loc	2 263
 # 263		    return 1;
	li	$2, 1
	b	$82
$55:
	.loc	2 264
 # 264		  } else {
	.loc	2 265
 # 265		    return 3;
	li	$2, 3
	b	$82
$56:
	.loc	2 270
 # 266		  }
 # 267		}
 # 268		
 # 269		// Block out other selections
 # 270		for (i = 0; i < 17; i ++){
	move	$16, $0
	li	$21, 2
$57:
	.loc	2 270
	.loc	2 271
 # 271		  __display_term( 0, i, MENU_COLOR_NOTSEL,
	move	$4, $0
	move	$5, $16
	li	$6, 2115
	li	$7, 1
	li	$9, 1
	sw	$9, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	__display_term
	.loc	2 270
 # 270		for (i = 0; i < 17; i ++){
	addu	$16, $16, 1
	bne	$16, $20, $57
	.loc	2 274
 # 271		  __display_term( 0, i, MENU_COLOR_NOTSEL,
 # 272				        MENU_COLOR_SHADOW, MENU_COLOR_SHADOW );
 # 273		}
 # 274		mode = 2;
	b	$72
$58:
	.loc	2 276
 # 275		
 # 276	      } else if ( (trig | repeat_trig) & U_JPAD) {
	or	$3, $2, $22
	and	$10, $3, 2048
	beq	$10, 0, $61
	.loc	2 276
	.loc	2 278
 # 277		// Go previous file
 # 278		while (pfs_stat_ret[--target] && target >= 0);
$59:
	.loc	2 278
	addu	$3, $17, -1
	move	$2, $3
	move	$17, $3
	mul	$14, $2, 4
	addu	$15, $19, $14
	.noalias	$15,$sp
	lw	$24, 0($15)
	.alias	$15,$sp
	beq	$24, 0, $60
	bge	$17, 0, $59
$60:
	.loc	2 279
 # 279		if (target == -1) target = 16;
	bne	$17, -1, $72
	.loc	2 279
	li	$17, 16
	b	$72
$61:
	.loc	2 281
 # 280			
 # 281	      } else if ( (trig | repeat_trig) & D_JPAD) {
	and	$25, $3, 1024
	beq	$25, 0, $65
	.loc	2 281
	.loc	2 284
 # 282		
 # 283		// Go next file
 # 284	      next_target:
$62:
	.loc	2 285
 # 285		if (target == 16) target = -1;
	bne	$17, 16, $63
	.loc	2 285
	li	$17, -1
$63:
	.loc	2 286
 # 286		while (pfs_stat_ret[++target] && target <= 15);
$64:
	.loc	2 286
	addu	$3, $17, 1
	move	$2, $3
	move	$17, $3
	mul	$11, $2, 4
	addu	$12, $19, $11
	.noalias	$12,$sp
	lw	$13, 0($12)
	.alias	$12,$sp
	beq	$13, 0, $72
	blt	$17, 16, $64
	b	$72
$65:
	.loc	2 288
 # 287		
 # 288	      } else if (trig & B_BUTTON){
	and	$14, $2, 16384
	beq	$14, 0, $72
	.loc	2 288
	.loc	2 291
 # 289		
 # 290		// Go onto "QUIT"
 # 291		target = 16;
	li	$17, 16
	.loc	2 294
 # 292	
 # 293	      }
 # 294	      break;
	b	$72
$66:
	.loc	2 297
 # 295	      
 # 296	    case	1:
 # 297	      if (trig & D_JPAD)   mode = 2;
	move	$2, $18
	and	$15, $2, 1024
	beq	$15, 0, $67
	.loc	2 297
	li	$21, 2
$67:
	.loc	2 298
 # 298	      if (trig & B_BUTTON) mode = 0;
	and	$24, $2, 16384
	beq	$24, 0, $68
	.loc	2 298
	move	$21, $0
$68:
	.loc	2 299
 # 299	      if (trig & A_BUTTON){	
	and	$25, $2, 32768
	beq	$25, 0, $72
	.loc	2 299
	.loc	2 300
 # 300		if (osPfsDeleteFile(&pfs,
	addu	$4, $sp, 128
	.noalias	$16,$19
	.noalias	$16,$sp
	mul	$8, $17, 32
	la	$9, pfs_stat
	addu	$16, $8, $9
	lhu	$5, 8($16)
	lw	$6, 4($16)
	addu	$7, $16, 14
	addu	$10, $16, 10
	sw	$10, 16($sp)
	move	$21, $0
	.livereg	0x0F00840E,0x00000000
	jal	osPfsDeleteFile
	beq	$2, 0, $69
	.alias	$16,$19
	.alias	$16,$sp
	.loc	2 304
 # 301				     pfs_stat[target].company_code,
 # 302				     pfs_stat[target].game_code,
 # 303				     pfs_stat[target].game_name,
 # 304				     pfs_stat[target].ext_name)){
	.loc	2 305
 # 305		  screen_type = STYPE_NODEL;
	li	$11, 1
	sw	$11, 88($sp)
	.loc	2 306
 # 306		  goto display_screen;
	sh	$0, 234($sp)
	move	$22, $0
	li	$17, 16
	li	$18, 1024
	move	$23, $0
	b	$47
$69:
	.loc	2 308
 # 307		}
 # 308		__display_term(1, target, MENU_COLOR_EMPTY,
	li	$4, 1
	move	$5, $17
	li	$6, 1
	li	$7, 1
	li	$12, 1
	sw	$12, 16($sp)
	mul	$13, $17, 4
	addu	$18, $19, $13
	.livereg	0x0F00200E,0x00000000
	jal	__display_term
	.loc	2 310
 # 309			                  MENU_COLOR_SHADOW, MENU_COLOR_SHADOW );
 # 310		__display_term(2, target, MENU_COLOR_EMPTY,
	li	$4, 2
	move	$5, $17
	li	$6, 1
	li	$7, 1
	li	$14, 1
	sw	$14, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	__display_term
	lw	$2, 116($sp)
	.loc	2 312
 # 311			                  MENU_COLOR_SHADOW, MENU_COLOR_SHADOW );
 # 312		mode = 0;
	.loc	2 313
 # 313		pfs_stat_ret[target] = -1;
	.noalias	$18,$sp
	li	$15, -1
	sw	$15, 0($18)
	.loc	2 314
 # 314		free_blocks += (u8)((pfs_stat[target].file_size+255) >> 8);
	.noalias	$16,$18
	.noalias	$16,$19
	.noalias	$16,$sp
	lw	$24, 0($16)
	addu	$25, $24, 255
	srl	$8, $25, 8
	and	$9, $8, 255
	addu	$2, $2, $9
	.loc	2 315
 # 315		__display_free_block( (u16)(free_blocks) );
	and	$4, $2, 65535
	sw	$2, 116($sp)
	.livereg	0x0800000E,0x00000000
	jal	__display_free_block
	.alias	$16,$18
	.alias	$16,$19
	.alias	$16,$sp
	.alias	$18,$sp
	.loc	2 316
 # 316		goto next_target;
	b	$62
$70:
	.loc	2 321
 # 317	      }      
 # 318	      break;
 # 319	      
 # 320	    case	2:
 # 321	      if (trig & U_JPAD)             mode = 1;
	move	$2, $18
	and	$10, $2, 2048
	beq	$10, 0, $71
	.loc	2 321
	li	$21, 1
$71:
	.loc	2 322
 # 322	      if (trig &(A_BUTTON|B_BUTTON)) mode = 0;
	and	$11, $2, 49152
	beq	$11, 0, $72
	.loc	2 322
	move	$21, $0
	.loc	2 323
 # 323	      break;      
$72:
	.loc	2 326
 # 324	    }
 # 325	    
 # 326	    if (!file_num) target = 16;
	lw	$12, 124($sp)
	bne	$12, 0, $73
	.loc	2 326
	li	$17, 16
$73:
	.loc	2 328
 # 327	    
 # 328	    if (mode != save_mode){
	lw	$13, 108($sp)
	sne	$2, $21, $13
	sw	$21, 108($sp)
	beq	$2, 0, $76
	.loc	2 328
	.loc	2 329
 # 329	      fg = (u16)MENU_COLOR_FILE;
	li	$16, 65535
	.loc	2 330
 # 330	      if (mode == 0){
	move	$23, $0
	bne	$21, 0, $75
	.loc	2 330
	.loc	2 332
 # 331	
 # 332		for (i = 0; i < 17; i ++){
	move	$16, $0
$74:
	.loc	2 332
	.loc	2 333
 # 333		  __display_term(0, i, MENU_COLOR_FILE,
	move	$4, $0
	move	$5, $16
	li	$6, 65535
	li	$7, 1
	li	$14, 1
	sw	$14, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	__display_term
	.loc	2 332
 # 332		for (i = 0; i < 17; i ++){
	addu	$16, $16, 1
	bne	$16, $20, $74
	.loc	2 336
 # 333		  __display_term(0, i, MENU_COLOR_FILE,
 # 334				       MENU_COLOR_SHADOW, MENU_COLOR_SHADOW);
 # 335		}
 # 336		fg = (u16)MENU_COLOR_SHADOW;
	li	$16, 1
$75:
	.loc	2 338
 # 337	      }
 # 338	      __display_term(1, save_target, MENU_COLOR_FILE, fg, MENU_COLOR_SHADOW);
	li	$4, 1
	move	$5, $30
	li	$6, 65535
	and	$7, $16, 65535
	li	$15, 1
	sw	$15, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	__display_term
	.loc	2 339
 # 339	      __display_term(2, save_target, MENU_COLOR_FILE, fg, MENU_COLOR_SHADOW);
	li	$4, 2
	move	$5, $30
	li	$6, 65535
	and	$7, $16, 65535
	li	$24, 1
	sw	$24, 16($sp)
	.livereg	0x0F00000E,0x00000000
	jal	__display_term
	.loc	2 340
 # 340	      flash_cnt = 0;
	b	$77
$76:
	.loc	2 342
 # 341	    
 # 342	    } else if ( target != save_target ){
	beq	$17, $30, $77
	.loc	2 342
	.loc	2 344
 # 343	      
 # 344	      __display_term(0, save_target,
	move	$4, $0
	move	$5, $30
	li	$6, 65535
	li	$7, 1
	li	$25, 1
	sw	$25, 16($sp)
	move	$23, $0
	.livereg	0x0F00010E,0x00000000
	jal	__display_term
	.loc	2 346
 # 345			     MENU_COLOR_FILE, MENU_COLOR_SHADOW, MENU_COLOR_SHADOW );
 # 346	      flash_cnt = 0;
$77:
	.loc	2 350
 # 347	      
 # 348	    }
 # 349	    
 # 350	    __display_term(mode, target,
	move	$4, $21
	move	$5, $17
	div	$8, $23, 3
	mul	$9, $8, 2
	lhu	$6, cursor_color($9)
	and	$7, $6, 65535
	sw	$6, 16($sp)
	move	$30, $17
	.livereg	0x0F00000E,0x00000000
	jal	__display_term
	.loc	2 353
 # 351			   cursor_color[flash_cnt/3],
 # 352			   cursor_color[flash_cnt/3], cursor_color[flash_cnt/3]);
 # 353	    if ((++ flash_cnt) == 30) flash_cnt = 0;
	addu	$23, $23, 1
	bne	$23, 30, $78
	.loc	2 353
	move	$23, $0
$78:
	.loc	2 355
 # 354	    
 # 355	    osContStartReadData(siMessageQ);		// Scan controller data
	lw	$4, 288($sp)
	.livereg	0x0800000E,0x00000000
	jal	osContStartReadData
 # ---	  osWritebackDCacheAll();
	.livereg	0x0000000E,0x00000000
	jal	osWritebackDCacheAll
	.loc	2 356
 # 356	    nWaitMesg(retrace);
	lw	$4, 292($sp)
	addu	$5, $sp, 284
	li	$6, 1
	.livereg	0x0E00000E,0x00000000
	jal	osRecvMesg
	.loc	2 357
 # 357	    nWaitMesg(si);
	lw	$4, 288($sp)
	addu	$5, $sp, 284
	li	$6, 1
	.livereg	0x0E00000E,0x00000000
	jal	osRecvMesg
	.loc	2 358
 # 358	    trig = data[PORT_ID].button;
	lhu	$22, (244+PORT_ID*6)($sp)
	and	$18, $22, 65535
	.loc	2 359
 # 359	    osContGetReadData(data);			// Get controller data
	addu	$4, $sp, 244
	.livereg	0x0800000E,0x00000000
	jal	osContGetReadData
	.loc	2 361
 # 360	
 # 361	    repeat_trig = data[PORT_ID].button & trig;
	lhu	$4, (244+PORT_ID*6)($sp)
	move	$2, $18
	and	$22, $4, $2
	and	$10, $22, 65535
	move	$22, $10
	.loc	2 362
 # 362	    if (repeat_trig) {
	beq	$22, 0, $80
	lhu	$5, 234($sp)
	.loc	2 362
	.loc	2 363
 # 363	      repeat_cnt ++;
	addu	$5, $5, 1
	and	$11, $5, 65535
	move	$5, $11
	.loc	2 364
 # 364	      if ( (repeat_cnt>>5)==0 || (repeat_cnt&7)!=0 ) repeat_trig = 0;
	move	$3, $5
	sh	$5, 234($sp)
	sra	$12, $3, 5
	beq	$12, 0, $79
	and	$13, $3, 7
	beq	$13, 0, $81
$79:
	.loc	2 364
	move	$22, $0
	b	$81
$80:
	.loc	2 365
 # 365	    } else {
	.loc	2 366
 # 366	      repeat_cnt = 0;
	sh	$0, 234($sp)
$81:
	.loc	2 369
 # 367	    }
 # 368	
 # 369	    trig = data[PORT_ID].button & (data[PORT_ID].button ^ trig);
	xor	$14, $4, $2
	and	$18, $14, $4
	and	$15, $18, 65535
	move	$18, $15
	b	$53
$82:
	.livereg	0x2000FF0E,0x00000FFF
	lw	$16, 32($sp)
	lw	$17, 36($sp)
	lw	$18, 40($sp)
	lw	$19, 44($sp)
	lw	$20, 48($sp)
	lw	$21, 52($sp)
	lw	$22, 56($sp)
	lw	$23, 60($sp)
	lw	$31, 68($sp)
	lw	$30, 64($sp)
	addu	$sp, 288
	j	$31
	.end	nosPakMenu
