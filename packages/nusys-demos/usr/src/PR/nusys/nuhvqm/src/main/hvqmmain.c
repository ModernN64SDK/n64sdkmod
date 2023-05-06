/*
  hvqmmain.c
*/
#include "hvqm.h"

/* Global variables */
volatile u32 hvqmStatus=0;	/* status variable */
u32 hvqmOffset;		/* Position of image */

u32 hvqmTotalFrame;	/* HVQM data's total number of frames */
void *hvqmVideoStream;	/* HVQM data's video playback position */
void *hvqmAudioStream;	/* HVQM data's audio playback position */

u32 hvqmFrameRate;	/* HVQM data's frame rate per usec */
u32 hvqmAudioRate;	/* HVQM data's audio playback frequency */
u32 hvqmTotalAudio;	/* HVQM data's total number of audio records */
u32 hvqmVideoRemain;	/* HVQM data's number of remaining frames */
u32 hvqmAudioRemain;	/* HVQM data's number of remaining frames */

/* Local variables */

static u8 *hvqmRomAddress;	/* Starting address of HVQM data */
static u8 hvqmHeaderBuf[sizeof(HVQM2Header)+16]; /* HVQM data header */
static HVQM2Header *hvqmHeader;	/* Pointer to HVQM data header */

static u64 hvqmEndTime;		/* End time */
static u64 hvqmNowTime;		/* Present time */

static u8 hvqmRecordHeaderBuf[sizeof(HVQM2Record)+16];	/* Video header area */
static HVQM2Record *hvqmRecordHeader;	/* Pointer to video header */

/* Task-related */
static HVQM2Arg hvqmTaskArg;	/* Parameters for the HVQM2 microcode */
static HVQM2Info hvqmSPFifo[HVQM_SPFIFO_SIZE];

/* Internal function prototypes */
static void hvqmGetVideoRecord(HVQM2Record *hvqmRecordHeader, void *hvqmBody);
#if HVQM_SWAPBUFFER_CALLBACK == YES
static void hvqmGfxSwapCfb(void* hvqmTask);
#endif /* HVQM_SWAPBUFFER_CALLBACK */
#ifdef NU_DEBUG
static void hvqmPrintInfo(void);
#endif /* NU_DEBUG */

/*
 * Initialization function
 *         Needs to be called only once before playing HVQM
 */
void hvqmInit(void)
{
    /* Do nothing if already initialized */
    if( hvqmStatus&HVQM_STATUS_INITIALIZED )
	return;

    /* Set INITIALIZED flag */
    {
	OSIntMask mask;
	mask = osSetIntMask(OS_IM_NONE);
	hvqmStatus |= HVQM_STATUS_INITIALIZED;
	osSetIntMask(mask);
    }

    /* Adjust alignment of buffer for header */
    hvqmHeader = OS_DCACHE_ROUNDUP_ADDR( hvqmHeaderBuf );

#if HVQM_CFB_FORMAT == 1
    hvqm2InitSP1(0xff);
#else /* HVQM_CFB_FORMAT */
    hvqm2InitSP2(0xff);
#endif /* HVQM_CFB_FORMAT */
}

/*
 * Read data and start playback
 *     Activate dedicated audio manager internally
 *
 */
void hvqmStart(void *hvqmData)
{
    u32 hvqmOffsetWidth, hvqmOffsetHeight;

    /* Delete all if currently playing */
    if( hvqmStatus&HVQM_STATUS_PLAYING )
	hvqmDelete();

    /* Raise PLAYING flag */
    {
	OSIntMask mask;
	mask = osSetIntMask(OS_IM_NONE);
	hvqmStatus |= HVQM_STATUS_PLAYING;
	osSetIntMask(mask);
    }

    /* Get address of HVQM data */
    hvqmRomAddress = hvqmData;

    /* Read HVQM data header */
    nuPiReadRom( (u32)hvqmRomAddress, hvqmHeader, sizeof(HVQM2Header) );

#ifdef NU_DEBUG
    hvqmPrintInfo();
#endif /* NU_DEBUG */

    /* Get the various parameters */
    hvqmTotalFrame = (u32)hvqmHeader->total_frames;
    hvqmFrameRate  = (u32)hvqmHeader->usec_per_frame;
    hvqmTotalAudio = (u32)hvqmHeader->total_audio_records;
    hvqmAudioRate  = (u32)hvqmHeader->samples_per_sec;
    hvqmEndTime    = 0;

    /* Determine location of image display (Fixed in the center for now) */
    hvqmOffsetHeight = ( HVQM_CFB_HEIGHT - hvqmHeader->height ) / 2;
    hvqmOffsetWidth  = ( HVQM_CFB_WIDTH  - hvqmHeader->width ) / 2;
    hvqmOffset       = HVQM_CFB_WIDTH * hvqmOffsetHeight + hvqmOffsetWidth;

    /* Initialization required of every data set to use HVQM */
#if HVQM_CFB_FORMAT == 1
    hvqm2SetupSP1( hvqmHeader, HVQM_CFB_WIDTH );
#else /* HVQM_CFB_FORMAT */
    hvqm2SetupSP2( hvqmHeader, HVQM_CFB_WIDTH );
#endif /* HVQM_CFB_FORMAT */

    /* Set the condition at the start of the data */
    hvqmVideoRemain = hvqmTotalFrame;
    hvqmAudioRemain = hvqmTotalAudio;
    hvqmVideoStream = hvqmAudioStream = hvqmRomAddress+sizeof(HVQM2Header);

    /* Adjust the alignment of buffer for video header */
    hvqmRecordHeader = OS_DCACHE_ROUNDUP_ADDR( hvqmRecordHeaderBuf );

#if HVQM_SWAPBUFFER_CALLBACK == YES
    /* Make changes to the callback functions used for switching buffers. */
    nuGfxSwapCfbFuncSet(hvqmGfxSwapCfb);
    /* Turn off the flag to get ready for the forcible resume when playing HVQM. */
    hvqmStatus &= ~HVQM_STATUS_WAIT_SWAPBUFFER;
#endif /* HVQM_SWAPBUFFER_CALLBACK */
}

/*
 * Output the video
 *     Must call for every frame
 *     Of course, if another thread is started, only needs to be called once
 *     The argument specifies NU_SC_SWAPBUFFER or NU_SC_NOSWAPBUFFER
 */
void hvqmVideoMgr(u32 hvqmFlag)
{
    u16 hvqmFrameFormat;	/* Kind of video frame */
    s32 hvqmSPStatus;		/* Value returned from HVQM2's CPU process */
    u32 hvqmCfbPrev;		/* The number of the preceeding buffer */
#if HVQM_CFB_FORMAT == 2
    u32 **hvqmGfxCfb;		/* 32-bit version of nuGfxCfb */
#endif /* HVQM_CFB_FORMAT */

    /* Preparation OK? */
    if( !(hvqmStatus&HVQM_STATUS_PLAYING) )
	return;

    /* Start audio manager (if not already) */
    if( (hvqmTotalAudio != 0) && !(hvqmStatus&HVQM_STATUS_THREAD_CREATED) )
	hvqmAudioInit();

    /* Get the present time */
    hvqmNowTime = OS_CYCLES_TO_USEC(osGetTime());

    /* Calculate time of end of playback when first called */
    if( !(hvqmStatus&HVQM_STATUS_VIDEO_STARTED) )
    {
	OSIntMask mask;
	mask = osSetIntMask(OS_IM_NONE);
	hvqmStatus |= HVQM_STATUS_VIDEO_STARTED;
	osSetIntMask(mask);
	hvqmEndTime = hvqmNowTime + hvqmTotalFrame * hvqmFrameRate;
    }

    /* If there are uncreated images */
    if( hvqmVideoRemain >0 )
    {
	/*
	 * When image processing is too fast --> do nothing
	 * If you wish frame buffers be switched while drawing another 
	 * image outside the area, HVQM_PERMIT_OTHER_GRAPHICS needs to be YES.
	 */
	if( hvqmEndTime-hvqmNowTime > hvqmVideoRemain * hvqmFrameRate )
	{
#if HVQM_PERMIT_OTHER_GRAPHICS == YES
	    hvqmCopyFrameBuffer(hvqmHeader, hvqmFlag);
#endif /* HVQM_PERMIT_OTHER_GRAPHICS */
	    return;
	}
	/* So now let's read the header */
	/* Read header and body */
	hvqmGetVideoRecord(hvqmRecordHeader, hvqmRecordBody);
	/* Reduce remaining video by 1 */
	hvqmVideoRemain--;
	/* What's the format? */
	hvqmFrameFormat = (u16)hvqmRecordHeader->format;

	/* If video processing is too slow --> Read the next header */
	while( hvqmEndTime-hvqmNowTime < hvqmVideoRemain * hvqmFrameRate ){
	    /* Go to the key frame when skipping to read image!! */
	    do
	    {
		/*
		 * Do nothing if no image is left.
		 * If you wish frame buffers be switched while drawing another
		 * image outside the area, modification is needed.
		 */
		if( hvqmVideoRemain == 0 )
		{
#if HVQM_LAST_FRAME == YES
		    /*
		     * Time over, however, needs decoding for the last time.
		     * If the last image stays to show up a little while after completing
		     * decoding, it may be necessary to decode over again.
		     */
		    break;
#else /* HVQM_LAST_FRAME */
		    /* Note: If playback flag turns off, it means it proceeds to the next frame. */
#if HVQM_PERMIT_OTHER_GRAPHICS == YES
		    hvqmCopyFrameBuffer(hvqmHeader, hvqmFlag);
#endif /* HVQM_PERMIT_OTHER_GRAPHICS */
		    return;
#endif /* HVQM_LAST_FRAME */
		}
#ifdef NU_DEBUG
		/*
		 * Skipping here means that extra (unused) data is
		 * contained.
		 * If skipping happens every time, data should be rebuilt.
		 */
		osSyncPrintf("WARNING: Skip to decode a frame (Remain:%d)\n", hvqmVideoRemain);
#endif /* NU_DEBUG */
		/* Read the header and body */
		hvqmGetVideoRecord(hvqmRecordHeader, hvqmRecordBody);
		/* decrement the number of images left */
		hvqmVideoRemain--;
		/* Which format of an image? */
		hvqmFrameFormat = (u16)hvqmRecordHeader->format;
	    }
	    while( hvqmFrameFormat != HVQM2_VIDEO_KEYFRAME );
	}

	/* Video decoding process */

	/*
	 * Do nothing if the image doesn't need to get updated.
	 * If you wish frame buffers be swithced while drawing another
	 * image outside the area, HVQM_PERMIT_OTHER_GRAPHICS needs to be set YES.
	 */
	if( hvqmFrameFormat == HVQM2_VIDEO_HOLD )
	{
#if HVQM_PERMIT_OTHER_GRAPHICS == YES
	    hvqmCopyFrameBuffer(hvqmHeader, hvqmFlag);
#endif /* HVQM_PERMIT_OTHER_GRAPHICS */
	    return;
	}

	/* Initialize frame buffer (find previous buffer) */
	hvqmCfbPrev = (nuGfxCfbCounter+nuGfxCfbNum-1)%nuGfxCfbNum;

	/* CPU part */
	/*
	 * Disable the cache because the RSP changes the previous frame.
	 */
	osInvalDCache( nuGfxCfb[hvqmCfbPrev], HVQM_CFB_WIDTH*HVQM_CFB_HEIGHT*HVQM_CFB_FORMAT*2 );

#if HVQM_CFB_FORMAT == 1
	hvqmSPStatus = hvqm2DecodeSP1( hvqmRecordBody, hvqmFrameFormat,
			       &nuGfxCfb_ptr[hvqmOffset],
			       &nuGfxCfb[hvqmCfbPrev][hvqmOffset],
			       hvqmWorkspace, &hvqmTaskArg, hvqmSPFifo );
#else /* HVQM_CFB_FORMAT */
	hvqmGfxCfb = (u32**)nuGfxCfb;
	hvqmSPStatus = hvqm2DecodeSP2( hvqmRecordBody, hvqmFrameFormat,
			       &hvqmGfxCfb[nuGfxCfbCounter][hvqmOffset],
			       &hvqmGfxCfb[hvqmCfbPrev][hvqmOffset],
			       hvqmWorkspace, &hvqmTaskArg, hvqmSPFifo );
#endif /* HVQM_CFB_FORMAT */

	/* RSP part */
	if( hvqmSPStatus > 0)
	{
	    /* Execute as a graphics task */
	    nuGfxTaskStart( (Gfx*)&hvqmTaskArg, sizeof(hvqmTaskArg), HVQM_UCODE_HVQM2SP, hvqmFlag|NU_SC_NORDP|NU_SC_TASK_SP_ONLY );
	    /*
	     * Note: At the present time, NU_SC_TASK_SP_ONLY is only used
             * with the Z sort microcode, so it can be here or not
	     */
#if HVQM_SWAPBUFFER_CALLBACK == YES
	    /*
	     * Turn on the flag to check if the buffer is switched
	     * after this task is processed.
	     */
	    hvqmStatus |= HVQM_STATUS_WAIT_SWAPBUFFER;
#endif /* HVQM_SWAPBUFFER_CALLBACK */
	}
    }
    else
    {
	/* End video display and just wait for end time */

	/* It is end time yet? */
	if(hvqmEndTime < hvqmNowTime)
	{
	    OSIntMask mask;
	    mask = osSetIntMask(OS_IM_NONE);
	    hvqmStatus &= ~HVQM_STATUS_VIDEO_STARTED;
	    hvqmStatus &= ~HVQM_STATUS_PLAYING;
	    osSetIntMask(mask);
	}

#if HVQM_PERMIT_OTHER_GRAPHICS == YES
	/* Only copy here */
	hvqmCopyFrameBuffer(hvqmHeader, hvqmFlag);
#endif /* HVQM_PERMIT_OTHER_GRAPHICS */
    }
}

/*
 * Deletion process
 */
void hvqmDelete(void)
{
#if HVQM_SWAPBUFFER_CALLBACK == YES
    /* Return the callback function for switching buffer to its default state */
    nuGfxSwapCfbFuncSet(nuGfxSwapCfb);
    if( hvqmStatus&HVQM_STATUS_WAIT_SWAPBUFFER )
    {
	OSIntMask mask;
	mask = osSetIntMask(OS_IM_NONE);
	hvqmStatus &= ~HVQM_STATUS_WAIT_SWAPBUFFER;
	osSetIntMask(mask);
    }
#endif /* HVQM_SWAPBUFFER_CALLBACK */

    if( hvqmStatus&HVQM_STATUS_THREAD_CREATED )
	hvqmAudioDelete();

    if( hvqmStatus&HVQM_STATUS_VIDEO_STARTED )
    {
	OSIntMask mask;
	mask = osSetIntMask(OS_IM_NONE);
	hvqmStatus &= ~HVQM_STATUS_VIDEO_STARTED;
	osSetIntMask(mask);
    }

    if( hvqmStatus&HVQM_STATUS_PLAYING )
    {
	OSIntMask mask;
	mask = osSetIntMask(OS_IM_NONE);
	hvqmStatus &= ~HVQM_STATUS_PLAYING;
	osSetIntMask(mask);
    }
}

/*
 * Read video data header and body
 */
static void hvqmGetVideoRecord(HVQM2Record *hvqmRecord, void *hvqmBody)
{
    u16 hvqmRecordType;
    u32 hvqmRecordSize;

    /* Read header */
    for(;;)
    {
	nuPiReadRom((u32)hvqmVideoStream, hvqmRecord, sizeof(HVQM2Record));
	hvqmVideoStream = (u8 *)hvqmVideoStream + sizeof(HVQM2Record);

	/* Get parameters */
	hvqmRecordType = (u16)hvqmRecord->type;
	hvqmRecordSize = (u32)hvqmRecord->size;

	/* If this is the desired thing, leave */
	if( hvqmRecordType == HVQM2_VIDEO )
	    break;

    /* Read the header of the display list buffer */
	hvqmVideoStream = (u8 *)hvqmVideoStream + hvqmRecordSize;
    }

    /* Read body */
    if( hvqmRecordSize > 0 )
    {
	nuPiReadRom((u32)hvqmVideoStream, hvqmBody, hvqmRecordSize);
	hvqmVideoStream = (u8 *)hvqmVideoStream + hvqmRecordSize;
    }
}

#if HVQM_SWAPBUFFER_CALLBACK == YES
/*
 * Callback function to be called when switching buffers.
 */
static void hvqmGfxSwapCfb(void* hvqmTask)
{
    NUScTask*	hvqmTaskPtr;
    hvqmTaskPtr = (NUScTask*)hvqmTask;

    osViSwapBuffer(hvqmTaskPtr->framebuffer);

    /* The addition starts here */
    hvqmStatus &= ~HVQM_STATUS_WAIT_SWAPBUFFER;
}
#endif /* HVQM_SWAPBUFFER_CALLBACK */

#ifdef NU_DEBUG
/*
 * Display information about HVQM data on console
 */
static void hvqmPrintInfo(void)
{
    osSyncPrintf( "\n" );
    osSyncPrintf( "File version        : %s\n", hvqmHeader->file_version );
    osSyncPrintf( "File size           : %d\n", (u32)hvqmHeader->file_size );
    osSyncPrintf( "Image width         : %d\n", (u16)hvqmHeader->width );
    osSyncPrintf( "Image height        : %d\n", (u16)hvqmHeader->height );
    osSyncPrintf( "Compress type       : %s\n", hvqmHeader->v_sampling_rate == 1 ? "4:2:2" : "4:1:1" );
    osSyncPrintf( "Total frames        : %d\n", (u32)hvqmHeader->total_frames );
    osSyncPrintf( "Video rate          : %f frame/sec\n", 1000000.0 / (double)(u32) hvqmHeader->usec_per_frame );
    osSyncPrintf( "Total audio records : %d\n", (u32)hvqmHeader->total_audio_records );
    osSyncPrintf( "Audio rate          : %d Hz\n", (u32)hvqmHeader->samples_per_sec );

    osSyncPrintf( "\n" );
    osSyncPrintf( "Display mode        : %s\n", HVQM_CFB_FORMAT == 1 ? "16-bit RGBA" : "32-bit RGBA" );
    osSyncPrintf( "\n" );

    /* Check parameters */
    if( hvqmHeader->max_frame_size > HVQM_DATASIZE_MAX )
	osSyncPrintf( "ERROR: HVQM_DATASIZE_MAX is too small!!\n\tmax_frame_size is %ld\n", hvqmHeader->max_frame_size);

    if( hvqmHeader->max_audio_record_size > HVQM_AUDIO_DATASIZE_MAX )
	osSyncPrintf( "ERROR: HVQM_AUDIO_DATASIZE_MAX is too small!!\n\tmax_audio_record_size is %ld\n", hvqmHeader->max_audio_record_size);

    if( hvqmHeader->max_sp_packets > HVQM_SPFIFO_SIZE )
	osSyncPrintf( "ERROR: HVQM_SPFIFO_SIZE is too small!!\n\tmax_sp_packets is %ld\n", hvqmHeader->max_sp_packets );
}
#endif /* NU_DEBUG */
