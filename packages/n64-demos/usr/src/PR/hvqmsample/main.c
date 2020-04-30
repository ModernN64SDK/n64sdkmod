/* 
 *  N64-HVQM2 library   Sample program
 * 
 *  FILE : main.c
 * 
 *  Copyright (C) 1998,1999 NINTENDO Co.,Ltd.
 * 
 */

/* 1999-03-29 (for N64-HVQ2 library version 1.2) */

#include <ultra64.h>
#include <HVQM2File.h>
#include <hvqm2dec.h>
#include <adpcmdec.h>
#include "system.h"
#include "timekeeper.h"

/*************************************************************************
 *    Message queue for receiving message blocks and end of DMA 
 * notifications when requesting that audio records be read from 
 * the HVQM2 data (ROM).
 ***********************************************************************/
#define  AUDIO_DMA_MSG_SIZE  1
static OSIoMesg     audioDmaMesgBlock;
static OSMesgQueue  audioDmaMessageQ;
static OSMesg       audioDmaMessages[AUDIO_DMA_MSG_SIZE];

/***********************************************************************
 *    Message queue for receiving message blocks and end of DMA 
 * notifications when requesting that video records be read from 
 * the HVQM2 data (ROM).
 ***********************************************************************/
#define  VIDEO_DMA_MSG_SIZE  1
static OSIoMesg     videoDmaMesgBlock;
static OSMesgQueue  videoDmaMessageQ;
static OSMesg       videoDmaMessages[VIDEO_DMA_MSG_SIZE];

/***********************************************************************
 * SP event (SP task end) message queue
 ***********************************************************************/
/* Necessary when using the RSP version of the decoder */
#if USE_RSP
static OSMesgQueue  spMesgQ;
static OSMesg       spMesgBuf;
#endif

/***********************************************************************
 * RSP task data and parameter for the HVQM2 microcode
 ***********************************************************************/
/* Necessary when using the RSP version of the decoder */
#if USE_RSP
OSTask hvqtask;			/* RSP task data */
HVQM2Arg hvq_sparg;		/* Parameter for the HVQM2 microcode */
#endif

/***********************************************************************
 * Buffer for the HVQM2 header ***********************************************************************/
u8 hvqm_headerBuf[sizeof(HVQM2Header) + 16];

/***********************************************************************
 * Other data
 ***********************************************************************/
static u32 total_frames;	/* Total number of video records (frames) */
static u32 total_audio_records;	/* Total number of audio records */
static void *video_streamP;	/* Video record read-in pointer */
static void *audio_streamP;	/* Audio record read-in pointer */
static u32 audio_remain;	/* Counter for remaining number of audio records to read */
static u32 video_remain;	/* Counter for remaining number of video records to read */
static u64 disptime;		/* Counter for scheduled display time of next video frame */
static ADPCMstate adpcm_state;	/* Buffer for state information passed to the ADPCM decoder */


/***********************************************************************
 *
 * void print_hvqm_info(HVQM2Header *header)
 *
 * Explanation
 *     Prints to the debugger the main information included in the header.
 *     Also checks things like whether the various kinds of statically
 *  reserved buffers have sufficient capacity.
 *
 ***********************************************************************/
static void
print_hvqm_info( HVQM2Header *header )
{
  int err;

  osSyncPrintf( "\n" );
  osSyncPrintf( "File version        : %s\n", header->file_version );
  osSyncPrintf( "File size           : %d\n", load32( header->file_size ) );
  osSyncPrintf( "Image width         : %d\n", load16( header->width ) );
  osSyncPrintf( "Image height        : %d\n", load16( header->height ) );
  osSyncPrintf( "Compress type       : %s\n", header->v_sampling_rate == 1 ? "4:2:2" : "4:1:1" );
  osSyncPrintf( "Total frames        : %d\n", load32( header->total_frames ) );
  osSyncPrintf( "Video rate          : %f frame/sec\n", 1000000.0 / (double)load32( header->usec_per_frame ) );
  osSyncPrintf( "Total audio records : %d\n", load32( header->total_audio_records ) );
  osSyncPrintf( "Audio rate          : %d Hz\n", load32( header->samples_per_sec ) );

  osSyncPrintf( "\n" );
  osSyncPrintf( "Display mode        : %s\n", CFB_FORMAT == 1 ? "16-bit RGBA" : "32-bit RGBA" );
  osSyncPrintf( "\n" );

  err = 0;

  /*
   * Check whether the buffer for video records hvqbuf has sufficient capacity
   */
  if ( load32( header->max_frame_size ) > HVQ_DATASIZE_MAX ) {
    osSyncPrintf( "ERROR: hvqbuf insufficient\n" );
    ++err;
  }

  /*
   * Check whether the buffer for audio records adpcmbuf has sufficient capacity
   */
  if ( load32( header->max_audio_record_size ) > AUDIO_RECORD_SIZE_MAX ) {
    osSyncPrintf( "ERROR: adpcmbuf insufficient\n" );
    ++err;
  }

#if USE_RSP
  /*
   * Check whether the size of the data area for the RSP is large enough
   */
  if ( header->max_sp_packets > HVQ_SPFIFO_SIZE ) {
    osSyncPrintf( "ERROR: hvq_spfifo insufficient\n" );
    ++err;
  }
#endif

  if ( err ) for ( ; ; );	/* Stop execution */
}

/***********************************************************************
 *
 * u32 rewind(void *pcmbuf)
 *
 * Argument
 *     pcmbuf    Buffer holding the PCM data
 *
 * Explanation
 *     Reads the next audio record (ADPCM data) from the HVQM data
 *     in RAM and decodes it in pcmbuf.
 *
 * Returned value
 *     The number of samples in pcmbuf.  If there are no more audio 
 *     records, 0 is returned.
 *
 ***********************************************************************/
static u32
next_audio_record( void *pcmbuf )
{
  u8 header_buffer[sizeof(HVQM2Record)+16];
  HVQM2Record *record_header;
  HVQM2Audio *audio_headerP;
  u32 samples;

  if ( audio_remain == 0 ) return 0;

  record_header = OS_DCACHE_ROUNDUP_ADDR( header_buffer );
  audio_streamP = get_record( record_header, adpcmbuf, 
		       HVQM2_AUDIO, audio_streamP, 
		       &audioDmaMesgBlock, &audioDmaMessageQ );
  --audio_remain;

  audio_headerP = (HVQM2Audio *)adpcmbuf;
  samples = load32( audio_headerP->samples );
  adpcmDecode( &audio_headerP[1], (u32)load16( record_header->format ), samples, pcmbuf, 1, &adpcm_state );

  return samples;
}

/***********************************************************************
 *
 * tkAudioProc rewind(void)
 *
 * Explanation
 *
 *     The callback function called from the timekeeper before movie
 *   playback starts when tkStart() is executed.  Here the counter
 *   for read-out of audio records and video records from ROM is reset.
 *
 * Returned value
 *     The pointer (type tkAudioProc) to the callback function creating
 *  the next set of PCM data is returned from this callback function.  Here
 *  what is returned is next_audio_record(), which is the function for
 *  reading and decoding the next audio record (ADPCM data).
 *
 ***********************************************************************/
static tkAudioProc
rewind( void )
{
  video_streamP = audio_streamP = _hvqmdataSegmentRomStart + sizeof(HVQM2Header);
  audio_remain = total_audio_records;
  video_remain = total_frames;
  disptime = 0;
  return &next_audio_record;
}

/***********************************************************************
 *
 * Main - Main routine (runs inside the main thread)
 *
 *     Reads and plays HVQM2 motion picture data from ROM.
 *
 *     First activates the timekeeper thread and gives the timekeeper the 
 *   job of playing audio records and synchronizing the playback of
 *   video with audio.
 *
 *     Then, this main routine successively extracts just the video records
 *   from the HVQM2 stream, decoding them and sending completed frame
 *   buffers to the timekeeper as occasion demands.
 *
 ***********************************************************************/
void Main(void *argument)
{
  HVQM2Header *hvqm_header;
  int h_offset, v_offset;	/* Position of image display */
  int screen_offset;		/* Number of pixels from start of frame buffer to display position */
  u32 usec_per_frame;		/* Video frame display interval (usec) */


  hvqm_header = OS_DCACHE_ROUNDUP_ADDR( hvqm_headerBuf );

#if USE_RSP
  /*
   * Acquire an SP event (if using the RSP version of the decoder)  */
  osCreateMesgQueue( &spMesgQ, &spMesgBuf, 1 );
  osSetEventMesg( OS_EVENT_SP, &spMesgQ, NULL );
#endif

  /*
   * Create DMA message queue for the reading in of audio records
   */
  osCreateMesgQueue( &audioDmaMessageQ, audioDmaMessages, AUDIO_DMA_MSG_SIZE );

  /*
   * Create DMA message queue for the reading in of video records  
   */
  osCreateMesgQueue( &videoDmaMessageQ, videoDmaMessages, VIDEO_DMA_MSG_SIZE );

  /*
   * Create the timekeeper thread
   */
  createTimekeeper();

  /*
   * Initialize the HVQM2 decoder
   */
#if USE_RSP
  /* If using the RSP version of the decoder */
  /* also setup the RSP task data next */
#if CFB_FORMAT == 1
  hvqm2InitSP1(0xff);
  hvqtask.t.ucode = (u64 *)hvqm2sp1TextStart;
  hvqtask.t.ucode_size = (int)hvqm2sp1TextEnd - (int)hvqm2sp1TextStart;
  hvqtask.t.ucode_data = (u64 *)hvqm2sp1DataStart;
#else
  hvqm2InitSP2(0xff);
  hvqtask.t.ucode = (u64 *)hvqm2sp2TextStart;
  hvqtask.t.ucode_size = (int)hvqm2sp2TextEnd - (int)hvqm2sp2TextStart;
  hvqtask.t.ucode_data = (u64 *)hvqm2sp2DataStart;
#endif
  hvqtask.t.type = M_HVQM2TASK;
  hvqtask.t.flags = 0;
  hvqtask.t.ucode_boot = (u64 *)rspbootTextStart;
  hvqtask.t.ucode_boot_size = (int)rspbootTextEnd - (int)rspbootTextStart;
  hvqtask.t.ucode_data_size = HVQM2_UCODE_DATA_SIZE;
  hvqtask.t.data_ptr = (u64 *)&hvq_sparg;
  hvqtask.t.yield_data_ptr = (u64 *)hvq_yieldbuf;
  hvqtask.t.yield_data_size = HVQM2_YIELD_DATA_SIZE;

#else
  /* If using the CPU version of the decoder */
#if CFB_FORMAT == 1
  hvqm2Init1(0xff);
#else
  hvqm2Init2(0xff);
#endif

#endif

  /*
   * Initialize the frame buffer (clear buffer contents and status flag)
   */
  init_cfb();
  osViSwapBuffer( cfb[NUM_CFBs-1] );

  /*
   * Fetch the HVQM2 header
   */
  romcpy( hvqm_header, _hvqmdataSegmentRomStart, sizeof(HVQM2Header), 
	 OS_MESG_PRI_NORMAL, &videoDmaMesgBlock, &videoDmaMessageQ );

  /* 
   * Print the HVQM2 header information to the debugger
   */
  print_hvqm_info( hvqm_header );

  total_frames = load32( hvqm_header->total_frames );
  usec_per_frame = load32( hvqm_header->usec_per_frame );
  total_audio_records = load32( hvqm_header->total_audio_records );

  /*
   * Determine video display position
   * (adjust offset so a small image is expanded in the center of the
   *  frame buffer)
   */
  h_offset = (SCREEN_WD - hvqm_header->width) / 2;
  v_offset = (SCREEN_HT - hvqm_header->height) / 2;
  screen_offset = SCREEN_WD * v_offset + h_offset;

  /*
   * Setup the HVQM2 image decoder
   */
#if USE_RSP
#if CFB_FORMAT == 1
  hvqm2SetupSP1( hvqm_header, SCREEN_WD );
#else
  hvqm2SetupSP2( hvqm_header, SCREEN_WD );
#endif
#else
#if CFB_FORMAT == 1
  hvqm2Setup1( hvqm_header, SCREEN_WD );
#else
  hvqm2Setup2( hvqm_header, SCREEN_WD );
#endif
#endif

  /*
   * Repetitive playback loop
   */
  for ( ; ; ) {
    int prev_bufno = -1;

    /*
     * Release all frame buffers
     */
    release_all_cfb();

    /*
     * Indicate start of movie playback to timekeeper
     */
    tkStart( &rewind, load32( hvqm_header->samples_per_sec ) );

    /*
     * HVQM2 movie playback loop
     */
    while ( video_remain > 0 )
      {
	u8 header_buffer[sizeof(HVQM2Record)+16];
	HVQM2Record *record_header;
	u16 frame_format;
	int bufno;

	/*
	 * Fetch video record
	 */
	record_header = OS_DCACHE_ROUNDUP_ADDR( header_buffer );
	video_streamP = get_record( record_header, hvqbuf, 
			    HVQM2_VIDEO, video_streamP, 
			    &videoDmaMesgBlock, &videoDmaMessageQ );

#if SYNC_VIDEO
	{
	  /*
	   *   This block is an example of how to force the video to play
           * in sync with the audio.
           *
	   *   The video and audio is synchronized and played back by the
           * timekeeper thread, but this assumes the video (frame buffer)
           * is always completed and sent to the timekeeper thread before
           * its scheduled display time.
           *
           *   But with mixed I/O the compressed data can be read late and
           * decoding can take more time due to the increased burden on the
           * CPU, leading to a situation where the video can become delayed
           * relative to the audio.
           *
           *   One way to counter this is to skip to the next keyframe 
           * whenever the frame to be decoded is late by an amount of
           * time equal to 2 or more frames.
           *
	   */
	  if ( disptime > 0 ) {	/* Excluding the first frame */
	    if ( tkGetTime() > (disptime + (usec_per_frame * 2)) ) {
	      release_all_cfb();
	      do {
		disptime += usec_per_frame;
		if ( --video_remain == 0 ) break;
		video_streamP = get_record( record_header, hvqbuf, 
				    HVQM2_VIDEO, video_streamP, 
				    &videoDmaMesgBlock, &videoDmaMessageQ );
	      } while ( 
		       load16( record_header->format ) != HVQM2_VIDEO_KEYFRAME
		       || tkGetTime() > disptime );
	      if ( video_remain == 0 ) break;
	    }
	  }
	}
#endif

	/*
	 * Decode the compressed image data and expand it in the frame buffer
	 */
	frame_format = load16( record_header->format );

	if ( frame_format == HVQM2_VIDEO_HOLD )
	  {
	    /*
	     *   Just like when frame_format != HVQM2_VIDEO_HOLD you 
             * could call hvqm2Decode*() and decode in a new frame
             * buffer (in this case, just copying from the buffer of
             * the preceding frame).  But here we make use of the
             * preceding frame's buffer for the next frame in order
             * to speed up the process.
	     */
	    bufno = prev_bufno;
	  }
	else
	  {
	    bufno = get_cfb(); /* Get the frame buffer */

#if USE_RSP
	    /* If using the RSP version of the decoder */
	    {
	      int status;

	      /*
	       * Process first half in the CPU
	       */
	      hvqtask.t.flags = 0;
#if CFB_FORMAT == 1
	      status = hvqm2DecodeSP1( hvqbuf, frame_format, 
				     &cfb[bufno][screen_offset], 
				     &cfb[prev_bufno][screen_offset], 
				     hvqwork, &hvq_sparg, hvq_spfifo );
#else
	      status = hvqm2DecodeSP2( hvqbuf, frame_format, 
				     &cfb[bufno][screen_offset], 
				     &cfb[prev_bufno][screen_offset], 
				     hvqwork, &hvq_sparg, hvq_spfifo );
#endif
	      osWritebackDCacheAll();

	      /*
	       * Process last half in the RSP
	       */
	      if ( status > 0 ) {
		osInvalDCache( (void *)cfb[bufno], sizeof cfb[bufno] );
		osSpTaskStart( &hvqtask );
		osRecvMesg( &spMesgQ, NULL, OS_MESG_BLOCK );
	      }
	    }
#else
	    /* If using the CPU version of the decoder */
	    {
#if CFB_FORMAT == 1
	      hvqm2Decode1( hvqbuf, frame_format, 
			  &cfb[bufno][screen_offset], 
			  &cfb[prev_bufno][screen_offset], 
			  hvqwork );
#else
	      hvqm2Decode2( hvqbuf, frame_format, 
			  &cfb[bufno][screen_offset], 
			  &cfb[prev_bufno][screen_offset], 
			  hvqwork );
#endif
	      osWritebackDCache( cfb[bufno], sizeof cfb[bufno] );
	    }
#endif /* USE_RSP */
	    }

	/*
	 * This completes decoding of the image
	 * This frame is needed for decoding of the next frame, so keep it
	 */
	keep_cfb( bufno );

	/*
	 * Don't need the previous frame anymore, so release it
	 */
	if ( prev_bufno >= 0 && prev_bufno != bufno ) 
	  release_cfb( prev_bufno );

	/*
	 * Entrust the completed frame buffer to the timekeeper
	 */
	tkPushVideoframe( cfb[bufno], &cfb_status[bufno], disptime );

	/*
	 * Go to the process for next frame
	 */
	prev_bufno = bufno;
	disptime += usec_per_frame;
	--video_remain;
      }

    /*
     * At this point the processing of the last frame of the movie is complete.
     */

    /*
     * Dummy so the last frame will continue to be displayed until disptime
     */
    if ( prev_bufno >= 0 )
      tkPushVideoframe( cfb[prev_bufno], &cfb_status[prev_bufno], disptime );
  }

  /* NOT REACHED */
}

/* end */
