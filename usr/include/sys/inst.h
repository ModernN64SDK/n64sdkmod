#ifndef __SYS_INST_H__
#define __SYS_INST_H__
/*
 * |-----------------------------------------------------------|
 * | Copyright (c) 1991 MIPS Computer Systems, Inc.            |
 * | All Rights Reserved                                       |
 * |-----------------------------------------------------------|
 * |          Restricted Rights Legend                         |
 * | Use, duplication, or disclosure by the Government is      |
 * | subject to restrictions as set forth in                   |
 * | subparagraph (c)(1)(ii) of the Rights in Technical        |
 * | Data and Computer Software Clause of DFARS 52.227-7013.   |
 * |         MIPS Computer Systems, Inc.                       |
 * |         950 DeGuigne Drive                                |
 * |         Sunnyvale, CA 94086                               |
 * |-----------------------------------------------------------|
 */
#ident "$Revision: 1.1 $"

/*
 * inst.h -- instruction format defines
 */

#if (defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS))

/* for backward compatibility */
#define bb_ma_format ma_format

#ifdef MIPSEB
union mips_instruction {
	unsigned word;
	unsigned char byte[4];
	struct {
		unsigned opcode : 6;
		unsigned target : 26;
	} j_format;
	struct {
		unsigned opcode : 6;
		unsigned rs : 5;
		unsigned rt : 5;
		signed simmediate : 16;
	} i_format;
	struct {
		unsigned opcode : 6;
		unsigned rs : 5;
		unsigned rt : 5;
		unsigned uimmediate : 16;
	} u_format;
	struct {			/* new R4K cacheop format */
		unsigned opcode : 6;
		unsigned rs : 5;
		unsigned c_op : 3;
		unsigned cache : 2;
		unsigned simmediate : 16;
	} c_format;
	struct {
		unsigned opcode : 6;
		unsigned rs : 5;
		unsigned rt : 5;
		unsigned rd : 5;
		unsigned re : 5;
		unsigned func : 6;
	} r_format;
	struct {
		unsigned opcode : 6;
		unsigned : 1;
		unsigned fmt : 4;
		unsigned rt : 5;
		unsigned rd : 5;
		unsigned re : 5;
		unsigned func : 6;
	} f_format;
        struct {
                unsigned opcode : 6;
                unsigned fr : 5;
                unsigned ft : 5;
                unsigned fs : 5;
                unsigned fd : 5;
                unsigned func : 3;
                unsigned fmt : 3;
        } ma_format;
};
#endif

#ifdef MIPSEL
union mips_instruction {
	unsigned word;
	unsigned char byte[4];
	struct {
		unsigned target : 26;
		unsigned opcode : 6;
	} j_format;
	struct {
		signed simmediate : 16;
		unsigned rt : 5;
		unsigned rs : 5;
		unsigned opcode : 6;
	} i_format;
	struct {
		unsigned uimmediate : 16;
		unsigned rt : 5;
		unsigned rs : 5;
		unsigned opcode : 6;
	} u_format;
	struct {
		unsigned simmediate : 16;
		unsigned cache : 2;
		unsigned c_op : 3;
		unsigned rs : 5;
		unsigned opcode : 6;
	} c_format;
	struct {
		unsigned func : 6;
		unsigned re : 5;
		unsigned rd : 5;
		unsigned rt : 5;
		unsigned rs : 5;
		unsigned opcode : 6;
	} r_format;
	struct {
		unsigned func : 6;
		unsigned re : 5;
		unsigned rd : 5;
		unsigned rt : 5;
		unsigned fmt : 4;
		unsigned : 1;
		unsigned opcode : 6;
	} f_format;
        struct {
                unsigned fmt : 3;
                unsigned func : 3;
                unsigned fd : 5;
                unsigned fs : 5;
                unsigned ft : 5;
                unsigned fr : 5;
                unsigned opcode : 6;
        } ma_format;
};
#endif

/* major opcodes */
#define spec_op		0x00
#define bcond_op	0x01
#define j_op		0x02
#define jal_op		0x03

#define beq_op		0x04
#define bne_op		0x05
#define blez_op		0x06
#define bgtz_op		0x07

#define addi_op		0x08
#define addiu_op	0x09
#define slti_op		0x0A
#define sltiu_op	0x0B

#define andi_op		0x0C
#define ori_op		0x0D
#define xori_op		0x0E
#define lui_op		0x0F

#define cop0_op		0x10
#define cop1_op		0x11
#define cop2_op		0x12
#define cop1x_op        0x13

#define beql_op		0x14
#define bnel_op		0x15
#define blezl_op	0x16
#define bgtzl_op	0x17

#define daddi_op        0x18
#define daddiu_op       0x19
#define ldl_op          0x1A
#define ldr_op          0x1B

#define lb_op		0x20
#define lh_op		0x21
#define lwl_op		0x22
#define lw_op		0x23

#define lbu_op		0x24
#define lhu_op		0x25
#define lwr_op		0x26
#define lwu_op		0x27

#define sb_op		0x28
#define sh_op		0x29
#define swl_op		0x2A
#define sw_op		0x2B

#define sdl_op		0x2C
#define sdr_op		0x2D
#define swr_op		0x2E
#define cache_op	0x2F

#define ll_op		0x30
#define lwc1_op		0x31
#define lwc2_op		0x32
#define pref_op		0x33

#define lld_op		0x34
#define ldc1_op		0x35
#define ldc2_op		0x36
#define	ld_op		0x37

#define sc_op		0x38
#define swc1_op		0x39
#define swc2_op		0x3A

#define scd_op		0x3C
#define sdc1_op		0x3D
#define sdc2_op		0x3E
#define	sd_op		0x3F

/* func field of spec opcode */
#define sll_op		0x00
#define movc_op         0x01
#define srl_op		0x02
#define sra_op		0x03

#define sllv_op		0x04
#define srlv_op		0x06
#define srav_op		0x07

#define jr_op		0x08
#define jalr_op		0x09
#define movz_op         0x0A
#define movn_op         0x0B

#define syscall_op	0x0C
#define break_op	0x0D
#define spim_op		0x0E
#define sync_op		0x0F

#define mfhi_op		0x10
#define mthi_op		0x11
#define mflo_op		0x12
#define mtlo_op		0x13

#define dsllv_op	0x14
#define dsrlv_op	0x16
#define dsrav_op	0x17

#define mult_op		0x18
#define multu_op	0x19
#define div_op		0x1A
#define divu_op		0x1B

#define dmult_op	0x1C
#define dmultu_op	0x1D
#define ddiv_op		0x1E
#define ddivu_op	0x1F

#define add_op		0x20
#define addu_op		0x21
#define sub_op		0x22
#define subu_op		0x23

#define and_op		0x24
#define or_op		0x25
#define xor_op		0x26
#define nor_op		0x27

#define slt_op		0x2A
#define sltu_op		0x2B

#define dadd_op		0x2C
#define daddu_op	0x2D
#define dsub_op		0x2E
#define dsubu_op	0x2F

#define tge_op		0x30
#define tgeu_op		0x31
#define tlt_op		0x32
#define tltu_op		0x33

#define teq_op		0x34
#define tne_op		0x36

#define dsll_op         0x38
#define dsrl_op         0x3A
#define dsra_op         0x3B

#define dsll32_op       0x3C
#define dsrl32_op       0x3E
#define dsra32_op       0x3F

/* rt field of bcond opcodes */
#define bltz_op		0x00
#define bgez_op		0x01
#define bltzl_op	0x02
#define bgezl_op	0x03

#define spimi_op	0x04

#define tgei_op		0x08
#define tgeiu_op	0x09
#define tlti_op		0x0A
#define tltiu_op	0x0B

#define teqi_op		0x0C
#define tnei_op		0x0E

#define bltzal_op	0x10
#define bgezal_op	0x11
#define bltzall_op	0x12
#define bgezall_op	0x13

/* rs field of cop opcodes */
#define mfc_op		0x00
#define dmfc_op		0x01
#define cfc_op		0x02
#define mtc_op		0x04
#define dmtc_op		0x05
#define ctc_op		0x06
#define bc_op		0x08
#define cop_op		0x10
#define copm_op         0x18

/* func field of cop0 COI opcodes */
#define tlbr_op		0x01
#define tlbwi_op	0x02
#define tlbwr_op	0x06
#define tlbp_op		0x08
#define rfe_op		0x10
#define eret_op		0x18

/* func field of cop0 COM opcodes */
#define tlbr1_op	0x01
#define tlbw_op         0x02
#define tlbp1_op	0x08
#define dctr_op         0x09
#define dctw_op         0x0A

/* fmt field of cop1 opcodes */
#define	s_fmt		0
#define	d_fmt		1
#define	e_fmt		2
#define q_fmt		3
#define	w_fmt		4
#define l_fmt		5

/* func field of cop1/{s,d,w} opcodes */
#define fadd_op		0x00
#define fsub_op		0x01
#define fmul_op		0x02
#define fdiv_op		0x03

#define fsqrt_op	0x04
#define fabs_op		0x05
#define fmov_op		0x06
#define fneg_op		0x07

#define froundl_op	0x08
#define ftruncl_op	0x09
#define fceill_op	0x0a
#define ffloorl_op	0x0b

#define fround_op	0x0c
#define ftrunc_op	0x0d
#define fceil_op	0x0e
#define ffloor_op	0x0f

#define fmovc_op        0x11
#define fmovz_op        0x12
#define fmovn_op        0x13

#define frecip_op       0x15
#define frsqrt_op       0x16

#define fcvts_op	0x20
#define fcvtd_op	0x21
#define fcvte_op	0x22

#define fcvtw_op	0x24
#define fcvtl_op	0x25

#define fcmp_op		0x30

/* func field of cop1x opcodes */

#define lwxc1_op        0x00
#define ldxc1_op        0x01
#define swxc1_op        0x08
#define sdxc1_op        0x09
#define pfetch_op       0x0f
#define madd_s_op       0x20
#define madd_d_op       0x21
#define madd_e_op       0x22
#define msub_s_op       0x28
#define msub_d_op       0x29
#define msub_e_op       0x2a
#define nmadd_s_op      0x30
#define nmadd_d_op      0x31
#define nmadd_e_op      0x32
#define nmsub_s_op      0x38
#define nmsub_d_op      0x39
#define nmsub_e_op      0x3a

/* func field for mad opcodes (ma_format.func) */
#define madd_op         0x04
#define msub_op         0x05
#define nmadd_op        0x06
#define nmsub_op        0x07

/* obsolete opcodes */
#define cop3_op		0x13

#endif /* _LANGUAGE_C */

#ifdef _LANGUAGE_PASCAL

#ifdef MIPSEB
type
    mips_instruction =
      packed record
        case cardinal of
	  0: (
	    word: cardinal;
	    );
	  1: (
	    byte: packed array[0..3] of 0..255;
	    );
	  2: (
	    opcode: 0..63;
	    target: 0..67108863;
	    );
	  3: (
	    opcode3: 0..63;
	    rs: 0..31;
	    rt: 0..31;
	    simmediate: -32768..32767;
	    );
	  4: (
	    opcode4: 0..63;
	    rs4: 0..63;
	    rt4: 0..63;
	    uimmediate: 0..65535;
	    );
	  5: (
	    opcode5: 0..63;
	    rs5: 0..63;
	    rt5: 0..63;
	    rd5: 0..63;
	    re5: 0..63;
	    func: 0..63;
	    );
      end {record};
#endif

#ifdef MIPSEL
type
    mips_instruction =
      packed record
        case cardinal of
	  0: (
	    word: cardinal;
	    );
	  1: (
	    byte: packed array[0..3] of 0..255;
	    );
	  2: (
	    target: 0..67108863;
	    opcode: 0..63;
	    );
	  3: (
	    simmediate: -32768..32767;
	    rt: 0..31;
	    rs: 0..31;
	    opcode3: 0..63;
	    );
	  4: (
	    uimmediate: 0..65535;
	    rt4: 0..63;
	    rs4: 0..63;
	    opcode4: 0..63;
	    );
	  5: (
	    func: 0..63;
	    re5: 0..63;
	    rd5: 0..63;
	    rt5: 0..63;
	    rs5: 0..63;
	    opcode5: 0..63;
	    );
      end {record};
#endif

const
  /* major opcodes */
  spec_op =	16#00;
  bcond_op =	16#01;
  j_op =	16#02;
  jal_op =	16#03;

  beq_op =	16#04;
  bne_op =	16#05;
  blez_op =	16#06;
  bgtz_op =	16#07;

  addi_op =	16#08;
  addiu_op =	16#09;
  slti_op =	16#0A;
  sltiu_op =	16#0B;

  andi_op =	16#0C;
  ori_op =	16#0D;
  xori_op =	16#0E;
  lui_op =	16#0F;

  cop0_op =	16#10;
  cop1_op =	16#11;
  cop2_op =	16#12;
  cop1x_op =	16#13;

  beql_op =	16#14;
  bnel_op =	16#15;
  blezl_op =	16#16;
  bgtzl_op =	16#17;

  daddi_op =	16#18;
  daddiu_op = 	16#19;
  ldl_op =	16#1A;
  ldr_op =	16#1B;

  lb_op =	16#20;
  lh_op =	16#21;
  lwl_op =	16#22;
  lw_op =	16#23;

  lbu_op =	16#24;
  lhu_op =	16#25;
  lwr_op =	16#26;
  lwu_op =	16#27;

  sb_op =	16#28;
  sh_op =	16#29;
  swl_op =	16#2A;
  sw_op =	16#2B;

  sdl_op =	16#2C;
  sdr_op =	16#2D;
  swr_op =	16#2E;
  cache_op =	16#2F;

  ll_op =	16#30;
  lwc1_op =	16#31;
  lwc2_op =	16#32;
  pref_op =	16#33;

  lld_op =	16#34;
  ldc1_op =	16#35;
  ldc2_op =	16#36;
  ld_op =	16#37;

  sc_op =	16#38;
  swc1_op =	16#39;
  swc2_op =	16#3A;

  scd_op =	16#3C;
  sdc1_op =	16#3D;
  sdc2_op =	16#3E;
  sd_op =	16#3F;

  /* func field of spec opcode */
  sll_op =	16#00;
  movc_op =	16#01;
  srl_op =	16#02;
  sra_op =	16#03;

  sllv_op =	16#04;
  srlv_op =	16#06;
  srav_op =	16#07;

  jr_op =	16#08;
  jalr_op =	16#09;
  movz_op =	16#0A;
  movn_op =	16#0B;

  syscall_op =	16#0C;
  break_op =	16#0D;
  spim_op =	16#0E;
  sync_op =	16#0F;

  mfhi_op =	16#10;
  mthi_op =	16#11;
  mflo_op =	16#12;
  mtlo_op =	16#13;

  dsllv_op =	16#14;
  dsrlv_op =	16#16;
  dsrav_op =	16#17;

  mult_op =	16#18;
  multu_op =	16#19;
  div_op =	16#1A;
  divu_op =	16#1B;

  dmult_op =	16#1C;
  dmultu_op =	16#1D;
  ddiv_op =	16#1E;
  ddivu_op =	16#1F;

  add_op =	16#20;
  addu_op =	16#21;
  sub_op =	16#22;
  subu_op =	16#23;

  and_op =	16#24;
  or_op =	16#25;
  xor_op =	16#26;
  nor_op =	16#27;

  slt_op =	16#2A;
  sltu_op =	16#2B;

  dadd_op =	16#2C;
  daddu_op =	16#2D;
  dsub_op =	16#2E;
  dsubu_op =	16#2F;

  tge_op =	16#30;
  tgeu_op =	16#31;
  tlt_op =	16#32;
  tltu_op =	16#33;

  teq_op =	16#34;
  tne_op =	16#36;

  dsll_op =	16#38;
  dsrl_op =	16#3A;
  dsra_op =	16#3B;

  dsll32_op =	16#3C;
  dsrl32_op =	16#3E;
  dsra32_op =	16#3F;

  /* rt field of bcond opcodes */
  bltz_op =	16#00;
  bgez_op =	16#01;
  bltzl_op =	16#02;
  bgezl_op =	16#03;

  spimi_op =	16#04;

  tgei_op =	16#08;
  tgeiu_op =	16#09;
  tlti_op =	16#0A;
  tltiu_op =	16#0B;

  teqi_op =	16#0C;
  tnei_op =	16#0E;

  bltzal_op =	16#10;
  bgezal_op =	16#11;
  bltzall_op =	16#12;
  bgezall_op =	16#13;

  /* rs field of cop opcodes */
  mfc_op =	16#00;
  dmfc_op =	16#01;
  cfc_op =	16#02;
  mtc_op =	16#04;
  dmtc_op =	16#05;
  ctc_op =	16#06;
  bc_op =	16#08;
  cop_op =	16#10;
  copm_op =	16#18;

  /* func field of cop0 COI opcodes */
  tlbr_op =	16#1;
  tlbwi_op =	16#2;
  tlbwr_op =	16#6;
  tlbp_op =	16#8;
  rfe_op =	16#10;
  eret_op =	16#18;

  /* func field of cop0 COM opcodes */
  tlbr1_op =	16#1;
  tlbw_op =     16#2;
  tlbp1_op =	16#8;
  dctr_op =     16#9;
  dctw_op =     16#a;

  /* fmt field of cop1 opcodes */
  s_fmt =	0;
  d_fmt =	1;
  e_fmt =	2;
  q_fmt =	3;
  w_fmt =	4;
  l_fmt =	5;

  /* func field of cop1/{s,d,w} opcodes */
  fadd_op =	16#00;
  fsub_op =	16#01;
  fmul_op =	16#02;
  fdiv_op =	16#03;

  fsqrt_op =	16#04;
  fabs_op =	16#05;
  fmov_op =	16#06;
  fneg_op =	16#07;

  froundl_op =	16#08;
  ftruncl_op =	16#09;
  fceill_op =	16#0a;
  ffloorl_op =	16#0b;

  fround_op =	16#0c;
  ftrunc_op =	16#0d;
  fceil_op =	16#0e;
  ffloor_op =	16#0f;

  fmovc_op =    16#11;
  fmovz_op =    16#12;
  fmovn_op =    16#13;

  frecip_op =   16#15;
  frsqrt_op =   16#16;

  fcvts_op =	16#20;
  fcvtd_op =	16#21;
  fcvte_op =	16#22;

  fcvtw_op =	16#24;
  fcvtl_op =	16#25;

  fcmp_op =	16#30;

  /* func field of cop1x opcodes */
  lwxc1_op =    16#00;
  ldxc1_op =    16#01;
  swxc1_op =    16#08;
  sdxc1_op =    16#09;
  pfetch_op =   16#0F;
  madd_s_op =   16#20;
  madd_d_op =   16#21;
  msub_s_op =   16#28;
  msub_d_op =   16#29;
  nmadd_s_op =  16#30;
  nmadd_d_op =  16#31;
  nmsub_s_op =  16#38;
  nmsub_d_op =  16#39;

  /* func field of mad opcodes */
  madd_op =	16#04;
  msub_op =	16#05;
  nmadd_op =	16#06;
  nmsub_op =	16#07;

#endif /* _LANGUAGE_PASCAL */

#endif	/* __SYS_INST_H__ */
