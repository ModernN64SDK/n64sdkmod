/*
 * hvqmaudio.c
 */
#include "hvqm.h"

/* Local variables */
static OSThread hvqmAudioThread;
static u64 hvqmAudioStack[HVQM_AUDIO_THREAD_STACKSIZE];

/* Global function prototypes  */
static void hvqmAudioMgr(void *arg);
static void hvqmGetAudioRecord(HVQM2Record *hvqmHeader, void *hvqmBody);
static void hvqmPiReadRomHigh(u32 rom_addr, void *buf_ptr, u32 size);

/*
 * Start the audio manager
 */
void hvqmAudioInit(void)
{
    /* Process redudant as long as called by hvqmVideoMgr */
    if(hvqmStatus&HVQM_STATUS_THREAD_CREATED)
	hvqmAudioDelete();

    {
	OSIntMask mask;
	mask = osSetIntMask(OS_IM_NONE);
	hvqmStatus |= HVQM_STATUS_THREAD_CREATED;
	osSetIntMask(mask);
    }

    /* Create audio thread */
    osCreateThread(&hvqmAudioThread, HVQM_AUDIO_THREAD_ID,
		   hvqmAudioMgr, (void *)NULL,
		   (hvqmAudioStack + HVQM_AUDIO_THREAD_STACKSIZE/sizeof(u64)),
		   HVQM_AUDIO_THREAD_PRI);
    osStartThread(&hvqmAudioThread);
}

/*
 * Delete audio manager
 */
void hvqmAudioDelete(void)
{
    if(hvqmStatus&HVQM_STATUS_THREAD_CREATED)
    {
	OSIntMask mask;
	mask = osSetIntMask(OS_IM_NONE);
	osDestroyThread(&hvqmAudioThread);
	hvqmStatus &= ~HVQM_STATUS_THREAD_CREATED;
	osSetIntMask(mask);
    }
}

/*
 * The audio manager itself
 */
static void hvqmAudioMgr(void *arg)
{
    OSMesgQueue hvqmAudioMesgQ;
    OSMesg hvqmAudioMesg;
    NUScClient hvqmAudioClient;
    u8 hvqmAudioRecordHeaderBuf[sizeof(HVQM2Record)+16];
    HVQM2Record *hvqmAudioRecordHeader;
    HVQM2Audio *hvqmAudioHeader;

    /* Adjust alignment of buffer for header */
    hvqmAudioRecordHeader = OS_DCACHE_ROUNDUP_ADDR( hvqmAudioRecordHeaderBuf );

    /* Create a message for retrace */
    osCreateMesgQueue( &hvqmAudioMesgQ, &hvqmAudioMesg, 1);

    /* Removed from scheduler for the time being (so same thing not registered twice) */
    nuScRemoveClient(&hvqmAudioClient);
    /* Register in scheduler */
    nuScAddClient( &hvqmAudioClient, &hvqmAudioMesgQ, NU_SC_RETRACE_MSG);

    /* Set the playback frequency */
    osAiSetFrequency( hvqmAudioRate );

    while(hvqmAudioRemain)
    {
	u32 hvqmSamples;
	static u32 hvqmPcmBufNext=0;	/* Number of next buffer to use */
	static u32 hvqmPcmModSamples=0;	/* Surplus samples */
	ADPCMstate hvqmAudioState; /* Buffer for state information passed to the ADPCM decoder */

	/* Read the audio data */
	hvqmGetAudioRecord( hvqmAudioRecordHeader, hvqmAudioRecordBody );
	hvqmAudioRemain--;
	hvqmAudioHeader = (HVQM2Audio *)hvqmAudioRecordBody;
	hvqmSamples = hvqmAudioHeader->samples;

	/* ADPCM -> PCM */
	/* If some part is left over from previous time, continue to output */
	adpcmDecode( &hvqmAudioHeader[1], (u32)hvqmAudioRecordHeader->format, hvqmSamples, &hvqmPcmBuf[hvqmPcmBufNext][hvqmPcmModSamples<<1], 1, &hvqmAudioState );

	if( hvqmSamples > 0 )
	{
	    /* If some part is left over from previous time, increase numbero fsamples by that much */
	    if(hvqmPcmModSamples)
		hvqmSamples += hvqmPcmModSamples;

	    /* If data length is not multiple of 8, take out that part */
	    hvqmPcmModSamples = hvqmSamples & 1;

	    /* If there is left over part */
	    if(hvqmPcmModSamples)
	    {
		/* Copy into the front of the next buffer */
		bcopy(&hvqmPcmBuf[hvqmPcmBufNext][(hvqmSamples-hvqmPcmModSamples)<<1], &hvqmPcmBuf[(hvqmPcmBufNext+1)%HVQM_PCM_BUF_NUM], hvqmPcmModSamples<<2);
		/* And do not process that part this time */
		hvqmSamples -= hvqmPcmModSamples;
	    }

	}
	/* Is the audio over? */
	else
	    break;

	/* Wait for retrace message */
	do
	{
	    osRecvMesg( &hvqmAudioMesgQ, NULL, OS_MESG_BLOCK );
	}
	/* If the AI FIFO is full, do not do anything until the next retrace */
	while( osAiGetStatus() & AI_STATUS_FIFO_FULL );

	/* Put in sound data */
	osAiSetNextBuffer( &hvqmPcmBuf[hvqmPcmBufNext], hvqmSamples << HVQM_PCM_ALIGN_SHIFT );

	/* Switch to next buffer */
	hvqmPcmBufNext = (hvqmPcmBufNext + 1)%HVQM_PCM_BUF_NUM;
    }

    /* All processes are finished */

    /* Say goodbye to that thread too */
    hvqmStatus &= ~HVQM_STATUS_THREAD_CREATED;
}



/*
 * Read audio data's header and body
 */
static void hvqmGetAudioRecord(HVQM2Record *hvqmHeader, void *hvqmBody)
{
    u16 hvqmRecordType;
    u32 hvqmRecordSize;

    /* Read header */
    for(;;)
    {
	hvqmPiReadRomHigh((u32)hvqmAudioStream, hvqmHeader, sizeof(HVQM2Record));
	hvqmAudioStream = (u8 *)hvqmAudioStream + sizeof(HVQM2Record);

	/* Get parameters */
	hvqmRecordType = (u16)hvqmHeader->type;
	hvqmRecordSize = (u32)hvqmHeader->size;

	/* If the desired thing, leave */
	if( hvqmRecordType == HVQM2_AUDIO )
	    break;

	/* If the wrong thing, read next header */
	hvqmAudioStream = (u8 *)hvqmAudioStream + hvqmRecordSize;
    }

    /* Read body */
    if( hvqmRecordSize > 0 )
    {
	hvqmPiReadRomHigh((u32)hvqmAudioStream, hvqmBody, hvqmRecordSize);
	hvqmAudioStream = (u8 *)hvqmAudioStream + hvqmRecordSize;
    }
}

/*
 * High priority version of nuPiReadRom
 */
static void hvqmPiReadRomHigh(u32 rom_addr, void *buf_ptr, u32 size)
{
    OSIoMesg	dmaIoMesgBuf;
    OSMesgQueue dmaMesgQ;
    OSMesg	dmaMesgBuf;
    u32		readSize;

    
#ifdef	USE_EPI

    /* Create message queue */
    osCreateMesgQueue(&dmaMesgQ, &dmaMesgBuf, 1);
    dmaIoMesgBuf.hdr.pri      = OS_MESG_PRI_HIGH; /* Only this part is different from nuPiReadRom */
    dmaIoMesgBuf.hdr.retQueue = &dmaMesgQ;
    
    /* Invalidate CPU cache */
    osInvalDCache((void*)buf_ptr, (s32)size);

    while(size){
	if(size > NU_PI_CART_BLOCK_READ_SIZE){
	    readSize = NU_PI_CART_BLOCK_READ_SIZE;
	} else {
	    readSize = size;
	}
	dmaIoMesgBuf.dramAddr     = buf_ptr;
	dmaIoMesgBuf.devAddr      = (u32)rom_addr;
	dmaIoMesgBuf.size         = readSize;

	osEPiStartDma(nuPiCartHandle, &dmaIoMesgBuf, OS_READ);

	/* Wait for end */
	(void)osRecvMesg(&dmaMesgQ, &dmaMesgBuf, OS_MESG_BLOCK);
	
	rom_addr += readSize;
	buf_ptr = (void*)((u8*)buf_ptr + readSize);
	size -= readSize;
    }
#else /* USE_EPI */

     /* Invalidate CPU cache */
    osInvalDCache((void*)buf_ptr, (s32)size);

    /* Create message queue */
    osCreateMesgQueue(&dmaMesgQ, &dmaMesgBuf, 1);
    
    while(size){
	if(size > NU_PI_CART_BLOCK_READ_SIZE){
	} else {
	    readSize = size;
	}
	    
	/* DMA read */
	osPiStartDma(&dmaIoMesgBuf, OS_MESG_PRI_HIGH, OS_READ, rom_addr,
		     buf_ptr, readSize, &dmaMesgQ); /* only this part is different from nuPiReadRom */

	/* Wait for end  */
	(void)osRecvMesg(&dmaMesgQ, NULL, OS_MESG_BLOCK);
	
	rom_addr += readSize;
	buf_ptr = (void*)((u8*)buf_ptr + readSize);
	size -= readSize;
    }

#endif /* USE_EPI */
}
