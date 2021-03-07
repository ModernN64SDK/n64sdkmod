/* 
 *  hvq2dec.h - N64-HVQ2 library header file
 * 
 *  Copyright (C) 1998,1999 NINTENDO Co.,Ltd.
 * 
 */

/*  revision 1.2  1999-03-25  */

#ifndef __HVQ2DEC_H__
#define __HVQ2DEC_H__

#include <ultra64.h>
#include <HVQ2File.h>

typedef struct {
  u8 nbase;
  u8 dc;
  u8 dc_l;
  u8 dc_r;
  u8 dc_u;
  u8 dc_d;
} HVQ2Block;

typedef struct {
  u8 sx;
  u8 sy;
  s16 scale;
  u16 offset;
  u16 lineskip;
} HVQ2Basis;

typedef union {
  HVQ2Block blockinfo;
  HVQ2Basis basisinfo;
  long long int	force_structure_alignment;
} HVQ2Info;

typedef struct {
  u64 *info;
  u64 *buf;
  u16 buf_width;
  u8  chroma_step_h;
  u8  chroma_step_v;
  u16 hmcus;
  u16 vmcus;
  u8 alpha;
  u8 nest[HVQ2_NESTSIZE];
} HVQ2Arg_t;

typedef union {
  HVQ2Arg_t t;
  long long int	force_structure_alignment;
} HVQ2Arg;

/*
 *  Functions supporting 16-bit color
 */
void hvq2Init1(u8 alpha);
void hvq2Decode1(void *code, u16 *outbuf, u32 outbufWidth, u16 *workbuf);

/*
 *  Functions supporting 32-bit color
 */
void hvq2Init2(u8 alpha);
void hvq2Decode2(void *code, u32 *outbuf, u32 outbufWidth, u16 *workbuf);

/*
 *  Functions supporting the RSP
 */
void hvq2InitSP(u8 alpha);
void hvq2DecodeSP(void *code, void *outbuf, u32 outbufWidth, u16 *workbuf, HVQ2Arg *arg, HVQ2Info *infobuf);

/*
 *  RSP microcode-related definitions
 */
#define  M_HVQ2TASK  6			/* Task type */
#define  HVQ2_UCODE_DATA_SIZE  0x70	/* Size of microcode data */
#define  HVQ2_YIELD_DATA_SIZE  0xc10	/* Size of yield buffer */

/*
 *  Definitions for RSP microcode supporting 16-bit color */
extern  u8  hvq2sp1TextStart[], hvq2sp1TextEnd[];
extern  u8  hvq2sp1DataStart[], hvq2sp1DataEnd[];

/*
 *   Definitions for RSP microcode supporting 32-bit color */
extern  u8  hvq2sp2TextStart[], hvq2sp2TextEnd[];
extern  u8  hvq2sp2DataStart[], hvq2sp2DataEnd[];

#endif /* __HVQ2DEC_H__ */

