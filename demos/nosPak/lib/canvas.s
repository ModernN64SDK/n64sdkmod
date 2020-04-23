/*---------------------------------------------------------------------
  File : canvas.s	version 1.3

  Assembled from canvas.c
  Modified  by Koji Mitsunari.      Apr 13, 1996.
  Copyright by Nintendo, Co., Ltd.          1997.
  ---------------------------------------------------------------------*/

#include	"pakmenu.h"

	.option	pic0

 #-------------------------------------------------------------------------
 # void	canvas_u8_to_str(u8 n, u8 *str)
 #-------------------------------------------------------------------------
	.text	
	.align	2
	.file	2 "canvas.c"
	.globl	canvas_u8_to_str
	.loc	2 150
 # 148	
 # 149	void	canvas_u8_to_str(u16 n, u8 *str)
 # 150	{
	.ent	canvas_u8_to_str 2
canvas_u8_to_str:
	.option	O2
	.frame	$sp, 0, $31
	and	$4, $4, 65535
	.loc	2 150
	.loc	2 152
	bleu	$4, 999, $00_0
	li	$4, 999
$00_0:	
 # 151	#if 1
 # 152	  str[0] = n / 100 + 0x0f + (n >= 100);
	div	$14, $4, 100
	sge	$15, $4, 100
	addu	$24, $14, $15
	addu	$25, $24, 15
	sb	$25, 0($5)
	.loc	2 153
 # 153	  str[1] = (n % 100) / 10  + 0x0f + (n >= 10);
	mfhi	$8
	div	$9, $8, 10
	sge	$10, $4, 10
	addu	$11, $9, $10
	addu	$12, $11, 15
	sb	$12, 1($5)
	.loc	2 154
 # 154	  str[2] = n % 10  + 0x10;
	mfhi	$13
	addu	$14, $13, 16
	sb	$14, 2($5)
	.loc	2 168
 # 168	}  
	.livereg	0x0000FF0E,0x00000FFF
	j	$31
	.end	canvas_u8_to_str

 #-------------------------------------------------------------------------
 # void	canvas_putc(int c, u16 color_fg, u16 color_bg)
 #-------------------------------------------------------------------------
	.text	
	.align	2
	.file	2 "canvas.c"
	.globl	canvas_putc
	.loc	2 173
 # 169	
 # 170	u16	*__cursor;
 # 171	
 # 172	void	canvas_putc(int c, u16 color_fg, u16 color_bg)
 # 173	{
	.ent	canvas_putc 2
canvas_putc:
	.option	O2
	subu	$sp, 16
	.frame	$sp, 16, $31
	.loc	2 173
	.loc	2 179
 # 174	  register int	i;
 # 175	  register int	j;
 # 176	  register u16	*fpix;
 # 177	  register u16	*curp;
 # 178	
 # 179	  if (c == FONT_CODE_QSKIP) {
	lw	$14, __cursor

	bne	$4, FONT_CODE_QSKIP, $51
	.loc	2 179
	.loc	2 180
 # 180	    __cursor += FONT_WSKIP/3;
	addu	$14, $14, FONT_WSKIP*2/3
	b	$59
$51:
	.loc	2 181
 # 181	  } else {
	.loc	2 182
 # 182	    if (c < FONT_CODE_TOP || FONT_CODE_BTM < c) {
	blt	$4, FONT_CODE_TOP, $52
	ble	$4, FONT_CODE_BTM, $53
$52:
	.loc	2 182
	.loc	2 183
 # 183	      c = FONT_CODE_NON;
	li	$4, FONT_CODE_NON
$53:
	.loc	2 185
 # 184	    }
 # 185	    fpix = __font_buf + (c - FONT_CODE_TOP)*FONT_HT;
	sll	$24, $4, 5
	addu	$11, $24, -FONT_CODE_TOP*FONT_HT*2
	lw	$12, __font_buf
	addu	$9, $11, $12
	.loc	2 186
 # 186	    curp = __cursor;
	move	$10, $14
	.loc	2 188
 # 187	
 # 188	    for (i = 0; i < FONT_HT; i ++) {
	move	$7, $0
$54:
	.loc	2 188
	.loc	2 189
 # 189	      for (j = FONT_WD - 1; j >= 0; j --) {
	li	$8, 0x8000
$55:
	.loc	2 189
	.loc	2 190
 # 190		if ((*fpix >> j) & 1){
	lhu	$13, 0($9)
	and	$15, $13, $8
	beq	$15, 0, $56
	.loc	2 190
	.loc	2 191
 # 191	          *(curp)             = color_fg;
	sh	$5, 0($10)
	.loc	2 192
 # 192		  *(curp+1+SCREEN_WD) = color_bg;
	sh	$6, ((1+SCREEN_WD)*2)($10)
$56:
	.loc	2 194
 # 193	        }
 # 194		curp ++;
	addu	$10, $10, 2
	.loc	2 189
 # 189	      for (j = FONT_WD - 1; j >= 0; j --) {
	srl	$8, $8, 1
	bne	$8, 0, $55
$57:
	.loc	2 196
 # 196	      curp += (SCREEN_WD - FONT_WD);
	addu	$10, $10, (SCREEN_WD - FONT_WD)*2
	.loc	2 197
 # 197	      fpix ++;
	addu	$9, $9, 2
	.loc	2 188
 # 188	    for (i = 0; i < FONT_HT; i ++) {
	addu	$7, $7, 1
	blt	$7, 16, $54
$58:
	.loc	2 199
 # 199	    __cursor += FONT_WSKIP;
	addu	$14, $14, FONT_WSKIP*2
	.loc	2 201
 # 200	  }
 # 201	}
$59:
	sw	$14, __cursor
	.livereg	0x0000FF0E,0x00000FFF
	addu	$sp, 16
	j	$31
	.end	canvas_putc
 #-------------------------------------------------------------------------
 # void	canvas_puts(u8 *str, int n, u16 color_fg, u16 color_bg)
 #-------------------------------------------------------------------------
	.text	
	.align	2
	.file	2 "canvas.c"
	.globl	canvas_puts
	.loc	2 204
 # 202	
 # 203	void	canvas_puts(u8 *str, int n, u16 color_fg, u16 color_bg)
 # 204	{
	.ent	canvas_puts 2
canvas_puts:
	.option	O2
	subu	$sp, 40
	sw	$31, 28($sp)
	sw	$4, 40($sp)
	sw	$5, 44($sp)
	sw	$6, 48($sp)
	sw	$7, 52($sp)
	sw	$16, 24($sp)
	.mask	0x80010000, -12
	.frame	$sp, 40, $31
	.loc	2 204
	.loc	2 205
 # 205	  while (n -- && *str) canvas_putc(*str ++, color_fg, color_bg);
	b	$61_1
$60:
	.loc	2 205
	lhu	$5, 50($sp)
	lhu	$6, 54($sp)
	addu	$9, $8, 1
	sw	$9, 40($sp)

	.livereg	0x0E00000E,0x00000000
	jal	canvas_putc
$61_1:	
	lw	$10, 44($sp)
	beq	$10, 0, $61
	addu	$11, $10, -1
	sw	$11, 44($sp)

	lw	$8, 40($sp)
	lbu	$4, 0($8)
	bne	$4, 0, $60
	.loc	2 206
 # 206	}
$61:
	lhu	$6, 48($sp)
	lhu	$7, 52($sp)
	.livereg	0x0000FF0E,0x00000FFF
	lw	$16, 24($sp)
	lw	$31, 28($sp)
	addu	$sp, 40
	j	$31
	.end	canvas_puts
