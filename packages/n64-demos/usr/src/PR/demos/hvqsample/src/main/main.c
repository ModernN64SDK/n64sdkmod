/* 
 *  N64-HVQ2 library  Sample program
 * 
 *  FILE : main.c
 * 
 *  Copyright (C) 1998,1999 NINTENDO Co.,Ltd.
 * 
 */

/*  1999-03-29 (for N64-HVQ2 library version 1.2) */

#include <ultra64.h>
#include <HVQ2File.h>
#include <hvq2dec.h>
#include "system.h"


extern u8 HVQbuf[];		/* Buffer for compressed image data */
extern u16 hvqwork[];		/* Work area needed by HVQ2 library */

#if USE_RSP
OSTask hvqtask;			/* RSP task data */
HVQ2Arg hvq_sparg;		/* Parameters for the HVQ2 microcode */
extern u64 hvq_yieldbuf[];	/* Buffer for RSP task yield */
extern HVQ2Info hvq_spfifo[];	/* Data area for the HVQ2 microcode */
#endif

/*
 *  Main function
 */
void
Main(void *arg)
{
  int datasize;
  int h_offset, v_offset;
  void *screen_ptr;
  s32 cpudelta;
#if USE_RSP
  s32 rspdelta;
#endif
  int j;


  /*
   * Initialize the HVQ2 decoder (only needs to be done once at the start)
   */
#if USE_RSP
  hvq2InitSP(0xff);
#elif CFB_FORMAT == 1
  hvq2Init1(0xff);
#else
  hvq2Init2(0xff);
#endif

#if USE_RSP
  /*
   * Setup RSP task data
   */
  hvqtask.t.type = M_HVQ2TASK;
  hvqtask.t.flags = 0;
  hvqtask.t.ucode_boot = (u64 *)rspbootTextStart;
  hvqtask.t.ucode_boot_size = (int)rspbootTextEnd - (int)rspbootTextStart;
#if CFB_FORMAT == 1
  hvqtask.t.ucode = (u64 *)hvq2sp1TextStart;
  hvqtask.t.ucode_size = (int)hvq2sp1TextEnd - (int)hvq2sp1TextStart;
  hvqtask.t.ucode_data = (u64 *)hvq2sp1DataStart;
#else
  hvqtask.t.ucode = (u64 *)hvq2sp2TextStart;
  hvqtask.t.ucode_size = (int)hvq2sp2TextEnd - (int)hvq2sp2TextStart;
  hvqtask.t.ucode_data = (u64 *)hvq2sp2DataStart;
#endif
  hvqtask.t.ucode_data_size = HVQ2_UCODE_DATA_SIZE;
  hvqtask.t.data_ptr = (u64 *)&hvq_sparg;
  hvqtask.t.yield_data_ptr = (u64 *)hvq_yieldbuf;
  hvqtask.t.yield_data_size = HVQ2_YIELD_DATA_SIZE;
#endif

  /* 
   * Check capacity of the buffer for compressed data
   */
  datasize = _hvqdataSegmentRomEnd - _hvqdataSegmentRomStart;
  if ( datasize > HVQ_DATASIZE_MAX ) {
    osSyncPrintf( "ERROR: HVQbuf insufficient\n" );
    return;
  }

  /* 
   * Copy compressed data from ROM
   * (Note) Due to a cache line problem, the addres of HVQbuf must
   *        be at a 16byte boudary.
   */
  romcpy(HVQbuf, _hvqdataSegmentRomStart, datasize);

  /* 
   * Show information about the compressed data on the debugger
   */
  osSyncPrintf("\n" );
  osSyncPrintf("File version  : %s\n", ((HVQ2Header *)HVQbuf)->file_version);
  osSyncPrintf("File size     : %d\n", ((HVQ2Header *)HVQbuf)->file_size);
  osSyncPrintf("Image width   : %d\n", ((HVQ2Header *)HVQbuf)->width);
  osSyncPrintf("Image height  : %d\n", ((HVQ2Header *)HVQbuf)->height);
  osSyncPrintf("Compress type : %s\n", ((HVQ2Header *)HVQbuf)->v_sampling_rate == 1 ? "4:2:2" : "4:1:1");

  osSyncPrintf("\n");
  osSyncPrintf("Display mode  : %s\n", CFB_FORMAT == 1 ? "16-bit RGBA" : "32-bit RGBA");
  osSyncPrintf("\n");

  /*
   * Clear the frame buffer.
   * Adjust so small image is expanded in the center of the buffer.
   */
  for ( j = 0; j < SCREEN_WD * SCREEN_HT; j++ ) cfb[j] = 0;
  h_offset = (SCREEN_WD - ((HVQ2Header *)HVQbuf)->width) / 2;
  v_offset = (SCREEN_HT - ((HVQ2Header *)HVQbuf)->height) / 2;
  screen_ptr = &cfb[SCREEN_WD * v_offset + h_offset];

  /*
   * Decode the compressed data and expand image in the frame buffer
   */
#if USE_RSP			/* If using the RSP version decoder */

  /*
   * Check whether the data area for the RSP is big enough   */
  if ( ((HVQ2Header *)HVQbuf)->sp_packets > HVQ_SPFIFO_SIZE ) {
    osSyncPrintf( "ERROR: hvq_spfifo insufficient\n" );
    return;
  }

  cpudelta = osGetCount();
  /*
   * First half processed in the CPU
   */
  hvqtask.t.flags = 0;
  hvq2DecodeSP(HVQbuf, screen_ptr, SCREEN_WD, hvqwork, &hvq_sparg, hvq_spfifo);

  cpudelta = (rspdelta = osGetCount()) - cpudelta;
  /*
   * Last half processed in the RSP
   */
  osWritebackDCacheAll();
  osInvalDCache((void *)cfb, sizeof cfb);
  osSpTaskStart(&hvqtask);
  (void)osRecvMesg(&rspMessageQ, NULL, OS_MESG_BLOCK);

  rspdelta = osGetCount() - rspdelta;
  osSyncPrintf("RSP decoder ---> CPU %lldms + RSP %lldms\n", (OS_CYCLES_TO_USEC(cpudelta) + 500) / 1000, (OS_CYCLES_TO_USEC(rspdelta) + 500) / 1000);

#else				/* If using the CPU version decoder */

  cpudelta = osGetCount();

#if CFB_FORMAT == 1
  hvq2Decode1(HVQbuf, screen_ptr, SCREEN_WD, hvqwork);
#else
  hvq2Decode2(HVQbuf, screen_ptr, SCREEN_WD, hvqwork);
#endif

  osWritebackDCache(cfb, sizeof cfb);

  cpudelta = osGetCount() - cpudelta;
  osSyncPrintf("CPU decoder ---> %lldms\n", (OS_CYCLES_TO_USEC(cpudelta) + 500) / 1000);

#endif /* USE_RSP */

  /*
   * Display
   */
  osViSwapBuffer(cfb);
}

/* end */
