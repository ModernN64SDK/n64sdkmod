/* 
 *  HVQ2File.h: HVQ2(YUV 4:2:2/4:1:1) file format header file.
 * 
 *  Copyright (C) 1998,1999 NINTENDO Co.,Ltd.
 * 
 */

/*  revision 1.1  1999-02-12  */

#ifndef __HVQ2FILE_H__
#define __HVQ2FILE_H__

#include <HVQmd.h>

/* HVQ2 file header format */
typedef struct {
  CARD8  file_version[16];
  CARD32 file_size;		/* Size of file */
  CARD8  h_sampling_rate;	/* Horizontal sampling rate for color difference component */
  CARD8  v_sampling_rate;	/* Vertical sampling rate for color difference component */
  CARD16 width;			/* Width of image, in pixels */
  CARD16 height;		/* Height of image, in pixels */
  CARD16 nest_start_x;		/* Nest origin, X */
  CARD16 nest_start_y;		/* Nest origin, Y */
  CARD8  y_shiftnum;		/* Nest step in vertical direction (number of shifts) */
  CARD8  quantize_step;		/* DC quantized step */
  CARD32 sp_packets;		/* Number of packets needed for SP FIFO */
  CARD32 basisnum_offset[2];	/* Base number section offset */
  CARD32 basnum_run_offset[2];	/* Base number run section offset */
  CARD32 dc_offset[3];		/* DC section offset */
  CARD32 dc_run_offset[3];	/* DC run section offset */
  CARD32 scale_offset[3];	/* Base scale section offset */
  CARD32 fix_offset[3];		/* Fixed length code section offset */
} HVQ2Header;

/* Nest size */
#define HVQ2_NESTSIZE_L 70	/* Number of elements on long side */
#define HVQ2_NESTSIZE_S 38	/* Number of elements on long side */
#define HVQ2_NESTSIZE   (HVQ2_NESTSIZE_L * HVQ2_NESTSIZE_S)

#endif /* __HVQ2FILE_H__ */

