
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo. (Originated by SGI)
        
        $RCSfile: playseq.c,v $
        $Revision: 1.11 $
        $Date: 1999/05/24 07:16:07 $
 *---------------------------------------------------------------------*/
/* sequence player test */

#define COMP_SEQ_PLAY

#define PROFILE

#ifdef DEBUG
#define PRINTF  osSyncPrintf
#endif

#include <ultra64.h>
#include <assert.h>
#include <PR/os.h>
#include <PR/ramrom.h>
#include "playseq.h"

/*##### n_audio : begin #####*/
#include <PR/n_libaudio.h>
#include <PR/n_libaudio_s_to_n.h>

#define SAMPLES        184
#define EXTRA_SAMPLES  0

/*###### n_audio : end ######*/

static  ALMicroTime     initOsc(void **oscState, f32 *initVal, u8 oscType,
                                u8 oscRate, u8 oscDepth, u8 oscDelay);
static  ALMicroTime     updateOsc(void *oscState, f32 *updateVal);
static  void            stopOsc(void *oscState);

#ifdef PROFILE
#define CLOCKS_SEC      50000000.0
#define FRAME_RATE      60

s32     CPUdeltaCount, CPUmaxCount = 0;
f32     CPUavgCount = 0;

s32     RSPdeltaCount, RSPmaxCount = 0;
f32     RSPavgCount = 0;
#endif

OSPiHandle	*handler;

/*
 * Stacks for the threads as well as message queues for synchronization
 */
u64	        bootStack[STACKSIZE/8];

static void	mainproc(u8 *);
static void	gameproc(u8 *);

static OSThread	mainThread;
static u64	mainThreadStack[STACKSIZE/8];

static OSThread	gameThread;
static u64	gameThreadStack[STACKSIZE/8];

/*
 * For PI manager
 */
static OSMesg PiMessages[DMA_QUEUE_SIZE];
static OSMesgQueue PiMessageQ;

static OSMesgQueue dmaMessageQ, taskMessageQ, retraceMessageQ;
static OSMesg dmaMessageBuf[DMA_QUEUE_SIZE], taskMessageBuf, retraceMessageBuf;
static OSIoMesg dmaIOMessageBuf;

/*
 * Globals for audio
 */
static ALHeap          hp;

static ALGlobals       g;
static ALSynConfig     c;
static ALSeqpConfig    seqc;
static s32             seqNo = 0;

static u8              audioHeap[AUDIO_HEAP_SIZE];

static OSIoMesg	       dmaIOMesgBuf[DMA_QUEUE_SIZE];
static s32             nextDMA = 0;
static s32             curBuf = 0;
static s32             curAudioBuf = 1;

/*
 * Double-buffered dynamic segments
 */
static OSTask   *tlist[2];               /* globaltask list      */
static Acmd	*cmdList[2];

/*
 * Triple-buffered
 */
static s16      audioSamples[3] = {0, 0, 0};
static s16      *audioBuffer[3];

/*
 * This can usually be reduced - it depends on the sequence
 */
#define         NBUFFERS       128

typedef struct 
{
    ALLink      node;
    int         startAddr;
    u32         lastFrame;
    char        *ptr;
} DMABuffer;

typedef struct 
{
    u8          initialized;
    DMABuffer   *firstUsed;
    DMABuffer   *firstFree;

} DMAState;

DMAState    dmaState;
DMABuffer   dmaBuffs[NBUFFERS];
u32         gFrameCt;

void CleanDMABuffs(void);

s32 dmaCallBack(s32 addr, s32 len, void *state)
{
    void        *freeBuffer;
    int         delta;
    DMABuffer   *dmaPtr,*lastDmaPtr;
    s32         addrEnd,buffEnd;


    lastDmaPtr = 0;
    dmaPtr = dmaState.firstUsed;
    addrEnd = addr+len;
 
    while(dmaPtr)  /* see if buffer is already set up */
    {

        buffEnd = dmaPtr->startAddr + MAX_BUFFER_LENGTH;
        if(dmaPtr->startAddr > addr) /* since buffers are ordered */
            break;                   /* abort if past possible */

        else if(addrEnd <= buffEnd) /* yes, found one */
        {
            dmaPtr->lastFrame = gFrameCt; /* mark it used */
            freeBuffer = dmaPtr->ptr + addr - dmaPtr->startAddr;
            return (int) osVirtualToPhysical(freeBuffer);
        }
        lastDmaPtr = dmaPtr;
        dmaPtr = (DMABuffer*)dmaPtr->node.next;
    }

    /* get here, and you didn't find a buffer, so dma a new one */

    /* get a buffer from the free list */
    dmaPtr = dmaState.firstFree;

#ifdef DEBUG
#ifndef __MWERKS__
    assert(dmaPtr);  /* be sure you have a buffer */
#endif
#endif

    dmaState.firstFree = (DMABuffer*)dmaPtr->node.next;
    alUnlink((ALLink*)dmaPtr);

    /* add it to the used list */
    if(lastDmaPtr) /* normal procedure */
    {
        alLink((ALLink*)dmaPtr,(ALLink*)lastDmaPtr);
    }
    else if(dmaState.firstUsed) /* jam at begining of list */
    {
        lastDmaPtr = dmaState.firstUsed;
        dmaState.firstUsed = dmaPtr;
        dmaPtr->node.next = (ALLink*)lastDmaPtr;
        dmaPtr->node.prev = 0;
        lastDmaPtr->node.prev = (ALLink*)dmaPtr;
    }
    else /* no buffers in list, this is the first one */
    {
        dmaState.firstUsed = dmaPtr;
        dmaPtr->node.next = 0;
        dmaPtr->node.prev = 0;
    }
    
    freeBuffer = dmaPtr->ptr;
    delta = addr & 0x1;
    addr -= delta;
    dmaPtr->startAddr = addr;
    dmaPtr->lastFrame = gFrameCt;  /* mark it */

    dmaIOMesgBuf[nextDMA].hdr.pri      = OS_MESG_PRI_NORMAL;
    dmaIOMesgBuf[nextDMA].hdr.retQueue = &dmaMessageQ;
    dmaIOMesgBuf[nextDMA].dramAddr     = freeBuffer;
    dmaIOMesgBuf[nextDMA].devAddr      = (u32)addr;
    dmaIOMesgBuf[nextDMA].size         = MAX_BUFFER_LENGTH;

    osEPiStartDma(handler, &dmaIOMesgBuf[nextDMA++], OS_READ);

    return (int) osVirtualToPhysical(freeBuffer) + delta;
}

ALDMAproc dmaNew(DMAState **state)
{
    int         i;

    if(!dmaState.initialized)  /* only do this once */
    {
        dmaState.firstFree = &dmaBuffs[0];
        for (i=0; i<NBUFFERS-1; i++)
        {
            alLink((ALLink*)&dmaBuffs[i+1],(ALLink*)&dmaBuffs[i]);
            dmaBuffs[i].ptr = alHeapAlloc(&hp, 1, MAX_BUFFER_LENGTH);
        }
	dmaBuffs[i].ptr = alHeapAlloc(&hp, 1, MAX_BUFFER_LENGTH);

        dmaState.initialized = 1;
    }

    *state = &dmaState;  /* state is never used in this case */

    return dmaCallBack;
}

void CleanDMABuffs(void)
{
    DMABuffer  *dmaPtr,*nextPtr;

    dmaPtr = dmaState.firstUsed;
    while(dmaPtr)
    {
        nextPtr = (DMABuffer*)dmaPtr->node.next;

        /* Can change this value.  Should be at least one.  */
        /* Larger values mean more buffers needed, but fewer DMA's */

        if(dmaPtr->lastFrame + 2  < gFrameCt) /* remove from used list */
        {
            if(dmaState.firstUsed == dmaPtr)
                dmaState.firstUsed = (DMABuffer*)dmaPtr->node.next;
            alUnlink((ALLink*)dmaPtr);
            if(dmaState.firstFree)
                alLink((ALLink*)dmaPtr,(ALLink*)dmaState.firstFree);
            else
            {
                dmaState.firstFree = dmaPtr;
                dmaPtr->node.next = 0;
                dmaPtr->node.prev = 0;
            }
        }
        dmaPtr = nextPtr;
    }
}


/***********************************************************************
 *    initOsc, updateOsc and stopOsc are callbacks used to implement
 *    LFO's for vibrato and tremelo. Type values are set in the bank.inst
 *    file, and should always be different between vibrato and tremelo.
 *    Your initOsc routine should then be able to distinguish based on this
 *    value whether the LFO is for tremelo or vibrato. The audio library
 *    does not use the type value to know, but uses a different method, so
 *    applications are free to use any values 1-255 for types. (A type of
 *    zero means no vibrato or no tremelo.) In the initOsc routine, the
 *    application initializes the oscillator, allocates memory for the
 *    oscState and stores a pointer to that memory in the oscState handle.
 *    This pointer will be handed back to the application at each updateOsc
 *    call. The initVal is a pointer to either a u8, when the osc is a tremelo
 *    oscillator, or a f32 when the oscillator is a vibrato oscillator.
 *    This value should always be set. In the case of tremelo oscillators,
 *    the u8 should be a volume value from 0-127.  In the case of the vibrato
 *    oscillator, the f32 is a ratio that the original pitch of the note should
 *    be multiplied by.  A ratio of 1.0 will produce unity pitch, while a value
 *    of 0.5 will produce one octave lower, and 2.0 will produce one octave higher.
 *    initOsc returns a deltaTime in microseconds until the first call to
 *    updateOsc. If a deltaTime of zero is returned the initVal will be used
 *    but no further calls to the oscillator will be made. When an updateOsc
 *    call occurs the application should do any necessary calculations to update
 *    the oscillator, set the value pointed to by updateVal, and return the
 *    time, in microseconds, until the next updateOsc call.  When a stopOsc
 *    call occurs, the application should do any necessary cleaning up, and
 *    freeing of memory. Given below are several examples of how to implement
 *    an LFO to control tremelo and vibrato.
 *****************************************************************************/

/**********************************************************************
 *    OscType may be assigned in any way suited to the application. In
 *    this example, oscType's are assigned as follows:
 *    oscType = 1     A tremelo that uses a sin function
 *    oscType = 2     A tremelo that uses a square wave
 *    oscType = 3     A tremelo that uses a descending sawtooth
 *    oscType = 4     A tremelo that uses an ascending sawtooth
 *    oscType = 128   A vibrato that uses a sin function
 *    oscType = 129   A vibrato that uses a square wave
 *    oscType = 130   A vibrato that uses a descending sawtooth
 *    oscType = 131   A vibrato that uses an ascending sawtooth
 **********************************************************************/

#define  TREMELO_SIN        1
#define  TREMELO_SQR        2
#define  TREMELO_DSC_SAW    3
#define  TREMELO_ASC_SAW    4
#define  VIBRATO_SIN        128
#define  VIBRATO_SQR        129
#define  VIBRATO_DSC_SAW    130
#define  VIBRATO_ASC_SAW    131

#define  OSC_HIGH   0
#define  OSC_LOW    1
#define  TWO_PI     6.2831853

/*****************************************************************************
 *   oscDepth, oscRate and oscDelay may be used in anyway deemed appropriate
 *   by the programmer. These values are not used by the sequence player, but
 *   merely passed to the init routine. In this example, oscDelay is converted
 *   to microseconds by multiplying by 0x4000. But many other methods could have
 *   been chosen. In the vibrato examples, oscDepth is converted by the routine
 *   _depth2Cents.  Cents can then easily be converted to ratio. For the tremelo
 *   examples, oscDepth is the max depth on the volume scale of 0-127. Sin
 *   oscillators are implemented using the sinf() library call.
 *****************************************************************************/

typedef struct {  
    u8      rate;
    u8      depth;
    u8      oscCount;
} defData;

typedef struct {
    u8      halfdepth;
    u8      baseVol;
} tremSinData;
    
typedef struct {
    u8      curVal;
    u8      hiVal;
    u8      loVal;
} tremSqrData;

typedef struct {
    u8      baseVol;
    u8      depth;
} tremSawData;
    
typedef struct {
    f32     depthcents;
} vibSinData;

typedef struct {
    f32     loRatio;
    f32     hiRatio;
} vibSqrData;

typedef struct {
    s32     hicents;
    s32     centsrange;
} vibDSawData;

typedef struct {
    s32     locents;
    s32     centsrange;
} vibASawData;

typedef struct oscData_s {
    struct oscData_s  *next;
    u8      type;
    u8      stateFlags;
    u16     maxCount;
    u16     curCount;
    union {
        defData         def;
        tremSinData     tsin;
        tremSqrData     tsqr;
        tremSawData     tsaw;
        vibSinData      vsin;
        vibSqrData      vsqr;
        vibDSawData     vdsaw;
        vibASawData     vasaw;
    } data;        
} oscData;

/*
 * Number of osc states needed. In worst case will need two for each
 * voice. But if tremelo and vibrato not used on all instruments will
 * need less.
 */
#define  OSC_STATE_COUNT    2*MAX_VOICES 
oscData  *freeOscStateList;
oscData  oscStates[OSC_STATE_COUNT];

/************************************************************************
 *   _depth2Cents()  convert a u8 (0-255) to a cents value. Convert using
 *   1.03099303^(depth). This gives an exponential range of values from
 *   1 to 2400.  (2400 cents is 2 octaves). Lots of small values for
 *   good control of depth in musical applications, and a couple of
 *   really broad ranges for special effects.
 ************************************************************************/
f32 _depth2Cents(u8 depth)
{
    f32     x = 1.03099303;
    f32     cents = 1.0;

    while(depth)
    {
        if(depth & 1)
            cents *= x;
        x *= x;
        depth >>= 1;
    }

    return(cents);
}


ALMicroTime initOsc(void **oscState, f32 *initVal,u8 oscType,
                    u8 oscRate,u8 oscDepth,u8 oscDelay)
{
    oscData         *statePtr;
    ALMicroTime     deltaTime = 0;


    if(freeOscStateList)  /* yes there are oscStates available */
    {
        statePtr = freeOscStateList;
        freeOscStateList = freeOscStateList->next;
        statePtr->type = oscType;
        *oscState = statePtr;

        /*
         * Convert delay into usec's, In this example, multiply by
         * 0x4000, but could easily use another conversion method.
         */
        deltaTime = oscDelay * 0x4000;

        switch(oscType) /* set the initVal */
        {
            case TREMELO_SIN:
                statePtr->curCount = 0;
                statePtr->maxCount = 259-oscRate; /* gives values 4-259 */
                statePtr->data.tsin.halfdepth = oscDepth >> 1;
                statePtr->data.tsin.baseVol = AL_VOL_FULL - statePtr->data.tsin.halfdepth;
                *initVal = (f32)statePtr->data.tsin.baseVol;
                break;

            case TREMELO_SQR:
                statePtr->maxCount = 256-oscRate; /* values from 1-256 */
                statePtr->curCount = statePtr->maxCount;
                statePtr->stateFlags = OSC_HIGH;
                statePtr->data.tsqr.loVal = AL_VOL_FULL-oscDepth;
                statePtr->data.tsqr.hiVal = AL_VOL_FULL;
                statePtr->data.tsqr.curVal = AL_VOL_FULL;
                *initVal = (f32)AL_VOL_FULL;
                break;

            case TREMELO_DSC_SAW:
                statePtr->maxCount = 256-oscRate;
                statePtr->curCount = 0;
                statePtr->data.tsaw.depth = oscDepth;
                statePtr->data.tsaw.baseVol = AL_VOL_FULL;
                *initVal = (f32)statePtr->data.tsaw.baseVol;
                break;

            case TREMELO_ASC_SAW: 
                statePtr->maxCount = 256-oscRate;
                statePtr->curCount = 0;
                statePtr->data.tsaw.depth = oscDepth;
                statePtr->data.tsaw.baseVol = AL_VOL_FULL - oscDepth;
                *initVal = (f32)statePtr->data.tsaw.baseVol;
                break;           

            case VIBRATO_SIN:
                statePtr->data.vsin.depthcents = _depth2Cents(oscDepth);
                statePtr->curCount = 0;
                statePtr->maxCount = 259-oscRate; /* gives values 4-259 */
                *initVal = 1.0f; /* start at unity pitch */
                break;

            case VIBRATO_SQR:
                {
                    s32     cents;
                    statePtr->maxCount = 256-oscRate; /* values from 1-256 */
                    statePtr->curCount = statePtr->maxCount;
                    statePtr->stateFlags = OSC_HIGH;
                    cents = _depth2Cents(oscDepth);
                    statePtr->data.vsqr.loRatio = alCents2Ratio(-cents);
                    statePtr->data.vsqr.hiRatio = alCents2Ratio(cents);
                    *initVal = statePtr->data.vsqr.hiRatio;
                }
                break;
                    
            case VIBRATO_DSC_SAW:
                {
                    s32     cents;
                    statePtr->maxCount = 256-oscRate; /* values from 1-256 */
                    statePtr->curCount = statePtr->maxCount;
                    cents = _depth2Cents(oscDepth);
                    statePtr->data.vdsaw.hicents = cents;
                    statePtr->data.vdsaw.centsrange = 2 * cents;
                    *initVal = alCents2Ratio(statePtr->data.vdsaw.hicents);
                }
                break;
                
            case VIBRATO_ASC_SAW:
                {
                    s32     cents;
                    statePtr->maxCount = 256-oscRate; /* values from 1-256 */
                    statePtr->curCount = statePtr->maxCount;
                    cents = _depth2Cents(oscDepth);
                    statePtr->data.vasaw.locents = -cents;
                    statePtr->data.vasaw.centsrange = 2 * cents;
                    *initVal = alCents2Ratio(statePtr->data.vasaw.locents);
                }
                break;

        }
    }
    return(deltaTime);  /* if there are no oscStates, return zero, but if
                           oscState was available, return delay in usecs */
}

ALMicroTime updateOsc(void *oscState, f32 *updateVal)
{
    f32             tmpFlt;
    oscData         *statePtr = (oscData*)oscState;
    ALMicroTime     deltaTime = AL_USEC_PER_FRAME; /* in this example callback every */
                                              /* frame, but could be at any interval */

    switch(statePtr->type)   /* perform update calculations */
    {
        case TREMELO_SIN:
            statePtr->curCount++;
            if(statePtr->curCount >= statePtr->maxCount)
                statePtr->curCount = 0;
            tmpFlt = (f32)statePtr->curCount / (f32)statePtr->maxCount;
            tmpFlt = sinf(tmpFlt*TWO_PI);
            tmpFlt = tmpFlt * (f32)statePtr->data.tsin.halfdepth;
            *updateVal = (f32)statePtr->data.tsin.baseVol + tmpFlt;
            break;

        case TREMELO_SQR:
            if(statePtr->stateFlags == OSC_HIGH)
            {
                *updateVal = (f32)statePtr->data.tsqr.loVal;
                statePtr->stateFlags = OSC_LOW;
            }
            else
            {
                *updateVal = (f32)statePtr->data.tsqr.hiVal;
                statePtr->stateFlags = OSC_HIGH;
            }
            deltaTime *= statePtr->maxCount;
            break;
            
        case TREMELO_DSC_SAW:
            statePtr->curCount++;
            if(statePtr->curCount > statePtr->maxCount)
                statePtr->curCount = 0;
            
            tmpFlt = (f32)statePtr->curCount / (f32)statePtr->maxCount;
            tmpFlt *= (f32)statePtr->data.tsaw.depth;
            *updateVal = (f32)statePtr->data.tsaw.baseVol - tmpFlt;
            break;
            
        case TREMELO_ASC_SAW: 
            statePtr->curCount++;
            if(statePtr->curCount > statePtr->maxCount)
                statePtr->curCount = 0;
            tmpFlt = (f32)statePtr->curCount / (f32)statePtr->maxCount;
            tmpFlt *= (f32)statePtr->data.tsaw.depth;
            *updateVal = (f32)statePtr->data.tsaw.baseVol + tmpFlt;
            break;

        case VIBRATO_SIN:
            /* calculate a sin value (from -1 to 1) and multiply it by depthcents.
               Then convert cents to ratio. */

            statePtr->curCount++;
            if(statePtr->curCount >= statePtr->maxCount)
                statePtr->curCount = 0;
            tmpFlt = (f32)statePtr->curCount / (f32)statePtr->maxCount;
            tmpFlt = sinf(tmpFlt*TWO_PI) * statePtr->data.vsin.depthcents;
            *updateVal = alCents2Ratio((s32)tmpFlt);
            break;
            
        case VIBRATO_SQR:
            if(statePtr->stateFlags == OSC_HIGH)
            {
                statePtr->stateFlags = OSC_LOW;
                *updateVal = statePtr->data.vsqr.loRatio;
            }
            else
            {
                statePtr->stateFlags = OSC_HIGH;
                *updateVal = statePtr->data.vsqr.hiRatio;
            }
            deltaTime *= statePtr->maxCount;
            break;

        case VIBRATO_DSC_SAW:
            statePtr->curCount++;
            if(statePtr->curCount > statePtr->maxCount)
                statePtr->curCount = 0;
            tmpFlt = (f32)statePtr->curCount / (f32)statePtr->maxCount;
            tmpFlt *= (f32)statePtr->data.vdsaw.centsrange;
            tmpFlt = (f32)statePtr->data.vdsaw.hicents - tmpFlt;
            *updateVal = alCents2Ratio((s32)tmpFlt);
            break;
            
        case VIBRATO_ASC_SAW:
            statePtr->curCount++;
            if(statePtr->curCount > statePtr->maxCount)
                statePtr->curCount = 0;
            tmpFlt = (f32)statePtr->curCount / (f32)statePtr->maxCount;
            tmpFlt *= (f32)statePtr->data.vasaw.centsrange;
            tmpFlt += (f32)statePtr->data.vasaw.locents;
            *updateVal = alCents2Ratio((s32)tmpFlt);
            break;
    }
    
    return(deltaTime);
}

void stopOsc(void *oscState)
{
    ((oscData*)oscState)->next = freeOscStateList;
    freeOscStateList = (oscData*)oscState;
}

/*
 * private routine used to parse args.
 */
static s32
myatoi(u8 *str)
{
    s32		log10[5], logn = 0, val = 0, i, pow10 = 1;

    if (str == NULL || *str == '\0')
	return(-1);

    while (*str != '\0') {
	if (!(*str == '0' ||
	      *str == '1' ||
	      *str == '2' ||
	      *str == '3' ||
	      *str == '4' ||
	      *str == '5' ||
	      *str == '6' ||
	      *str == '7' ||
	      *str == '8' ||
	      *str == '9')) {
	    logn = 0;
	    break;
	}

	log10[logn++] = *str - '0';
	str++;
    }

    if (logn == 0)
	return(-1);

    for (i=logn-1; i>= 0; i--) {
	val += log10[i] * pow10;
	pow10 *= 10;
    }
    return (val);
}


#ifdef DEBUG
/*
 * private routine used to parse args - stolen from rdpatt.
 */
static void
parse_args(u8 *argstring)
{
    s32 argc = 1;
    u8	*arglist[32], **argv = arglist;	/* max 32 args */
    u8	*ptr;

    if (argstring == NULL || argstring[0] == '\0')
	return;

    /* re-organize argstring to be like main(argv,argc) */
    ptr = argstring;
    while (*ptr != '\0') {
	while (*ptr != '\0' && (*ptr == ' ')) {
	    *ptr = '\0';
	    ptr++;
	}
	if (*ptr != '\0')
	    arglist[argc++] = ptr;
	while (*ptr != '\0' && (*ptr != ' ')) {
	    ptr++;
	}
    }

    /* process the arguments: */
    while ((argc > 1) && (argv[1][0] == '-')) {
	switch(argv[1][1]) {

	  case 's':
	    seqNo = myatoi(argv[2]);	/* Sequence to run */
	    if (seqNo < 0)
		seqNo = 0;
	    argc--;
	    argv++;
	    break;	    
	  default:
	    break;
	}
	argc--;
	argv++;
    }
}
#endif


void boot(void *arg)
{

#ifdef DEBUG
    s32 i;
    u32 *argp;
    u32 argbuf[16];
#endif

    osInitialize();

    handler = osCartRomInit();

#ifdef DEBUG
    argp = (u32 *)RAMROM_APP_WRITE_ADDR;
    for (i=0; i<sizeof(argbuf)/4; i++, argp++) {
	osEPiReadIo(handler, (u32)argp, &argbuf[i]);
    }
    parse_args((u8 *)argbuf);
#endif

    osCreateThread(&mainThread, 1, (void(*)(void *))mainproc, arg,
		  ((u8 *) mainThreadStack) + STACKSIZE, 10);

    osStartThread(&mainThread);
}

static void mainproc(u8 *argv) 
{
    osCreateThread(&gameThread, 3, (void(*)(void *))gameproc, argv,
                                 ((u8 *) gameThreadStack) + STACKSIZE, 10);

    /* Initialize video */

    osCreateViManager(OS_PRIORITY_VIMGR);
    osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);

    /*
     * Start PI Mgr for access to cartridge - start before debugger?
     */
    osCreatePiManager((OSPri) 150, &PiMessageQ, PiMessages, DMA_QUEUE_SIZE);

    osStartThread(&gameThread);

    /*
     * Become the idle thread
     */
    osSetIntMask( OS_IM_ALL );
    osSetThreadPri( 0, 0 );
    for (;;);

}

static
void gameproc(u8 *argv)
{
#ifdef COMP_SEQ_PLAY
    ALCSPlayer
        sequencePlayer,
        *seqp = &sequencePlayer;

    ALCSeq
        sequence,
        *seq = &sequence;
#else

    ALSeqPlayer
        sequencePlayer,
        *seqp = &sequencePlayer;

    ALSeq
        sequence,
        *seq = &sequence;
#endif
    
    ALSeqFile
        *sfile;
    
    ALBank
        *midiBank=0;

    Acmd
        *cmdlp;

    OSTask
        *tlistp;

    f32
        fsize;

    s32
        i,
        samplesLeft = 0,
        clcount,
        len,
        bankLen,
        buf,
        frame = 0,
        frameSize,
        minFrameSize;

    s16
        *audioOp;

    u8
/*##### n_audio : begin #####*/
        min_only_one = 1,
/*###### n_audio : end ######*/
        *ptr,
        *seqPtr,
        *midiBankPtr;

    OSMesgQueue
        seqMessageQ;

    OSMesg
        seqMessageBuf;
    
    OSIoMesg
        seqIOMessageBuf;

    oscData
        *oscStatePtr;
    
    /*
     * Message queue for PI manager return messages
     */
    osCreateMesgQueue(&dmaMessageQ, dmaMessageBuf, DMA_QUEUE_SIZE);
    osCreateMesgQueue(&seqMessageQ, &seqMessageBuf, 1);

    /*
     * Set up message queue for TASK and video interrupts
     */
    osCreateMesgQueue(&taskMessageQ, &taskMessageBuf, 1);
    osSetEventMesg(OS_EVENT_SP, &taskMessageQ, NULL);
    
    /*
     * Prime the task queue
     */
    osSendMesg(&taskMessageQ, NULL, OS_MESG_BLOCK);

    osCreateMesgQueue(&retraceMessageQ, &retraceMessageBuf, 1);

    osViSetEvent(&retraceMessageQ, NULL, NUM_FIELDS);
    
    /*
     * Audio heap
     */
    alHeapInit(&hp, (u8 *) audioHeap, AUDIO_HEAP_SIZE);

    /*
     * Allocate storage for command list and task headers
     */
    cmdList[0] = alHeapAlloc(&hp, 1, MAX_CLIST_SIZE*sizeof(Acmd));
    cmdList[1] = alHeapAlloc(&hp, 1, MAX_CLIST_SIZE*sizeof(Acmd));
    tlist[0] = alHeapAlloc(&hp, 1, sizeof(OSTask));
    tlist[1] = alHeapAlloc(&hp, 1, sizeof(OSTask));
    audioBuffer[0] = alHeapAlloc(&hp, 1, sizeof(s32)*MAX_AUDIO_LENGTH);
    audioBuffer[1] = alHeapAlloc(&hp, 1, sizeof(s32)*MAX_AUDIO_LENGTH);
    audioBuffer[2] = alHeapAlloc(&hp, 1, sizeof(s32)*MAX_AUDIO_LENGTH);

    bankLen = _midibankSegmentRomEnd - _midibankSegmentRomStart;
    midiBankPtr = alHeapAlloc(&hp, 1, bankLen);
    osWritebackDCacheAll();

    dmaIOMessageBuf.hdr.pri      = OS_MESG_PRI_NORMAL;
    dmaIOMessageBuf.hdr.retQueue = &dmaMessageQ;
    dmaIOMessageBuf.dramAddr     = midiBankPtr;
    dmaIOMessageBuf.devAddr      = (u32)_midibankSegmentRomStart;
    dmaIOMessageBuf.size         = bankLen;

    osEPiStartDma(handler, &dmaIOMessageBuf, OS_READ);
    (void) osRecvMesg(&dmaMessageQ, NULL, OS_MESG_BLOCK);

    /*
     * Copy over the sequence file header
     */
    sfile = alHeapAlloc(&hp, 1, 4);
    osWritebackDCacheAll();

    dmaIOMessageBuf.hdr.pri      = OS_MESG_PRI_NORMAL;
    dmaIOMessageBuf.hdr.retQueue = &dmaMessageQ;
    dmaIOMessageBuf.dramAddr     = sfile;
    dmaIOMessageBuf.devAddr      = (u32)_seqSegmentRomStart;
    dmaIOMessageBuf.size         = 4;

    osEPiStartDma(handler, &dmaIOMessageBuf, OS_READ);
    (void) osRecvMesg(&dmaMessageQ, NULL, OS_MESG_BLOCK);

    len = 4 + sfile->seqCount*sizeof(ALSeqData);
    sfile = alHeapAlloc(&hp, 1, 4 + sfile->seqCount*sizeof(ALSeqData));
    osWritebackDCacheAll();

    dmaIOMessageBuf.hdr.pri      = OS_MESG_PRI_NORMAL;
    dmaIOMessageBuf.hdr.retQueue = &dmaMessageQ;
    dmaIOMessageBuf.dramAddr     = sfile;
    dmaIOMessageBuf.devAddr      = (u32)_seqSegmentRomStart;
    dmaIOMessageBuf.size         = len;

    osEPiStartDma(handler, &dmaIOMessageBuf, OS_READ);
    (void) osRecvMesg(&dmaMessageQ, NULL, OS_MESG_BLOCK);
    
    alSeqFileNew(sfile, (u8 *) _seqSegmentRomStart);
    if (seqNo > sfile->seqCount)
        seqNo = 0;

    /*
     * Initialize DAC output rate
     */
    c.outputRate = osAiSetFrequency(OUTPUT_RATE);

    fsize = (f32) NUM_FIELDS * c.outputRate / (f32) 60;
    frameSize = (s32) fsize;
    if (frameSize < fsize)
        frameSize++;

    /*##### n_audio : begin #####*/
    frameSize = ((frameSize / SAMPLES) + 1) * SAMPLES;
    minFrameSize = frameSize - SAMPLES;
    /*###### n_audio : end ######*/

    /*
     * Audio synthesizer initialization
     */
    c.maxVVoices = MAX_VOICES;
    c.maxPVoices = MAX_VOICES;
    c.maxUpdates = MAX_UPDATES;
    c.dmaproc    = &dmaNew;
    c.heap       = &hp;
    c.fxType	 = AL_FX_SMALLROOM;

    if (c.fxType == AL_FX_CUSTOM) {
	s32	delay_size = 0;
	/*
	 * if you wanted to build an effect, you would specify c.fxType
	 * to be AL_FX_CUSTOM, and then allocate and fill in the effect
	 * parameters. Some examples follows:
	 */
#define ms *(((s32) ((f32) OUTPUT_RATE/1000))&~0x7)
#define SECTION_COUNT 8
#define SECTION_SIZE  8

	s32	params[SECTION_COUNT*SECTION_SIZE+2] = {

	    /* sections	   total length */
	    SECTION_COUNT,    325 ms,
                                           /*       chorus  chorus   filter
            input  output  fbcoef  ffcoef   gain     rate   depth     coef      */
	        0,   8 ms,  9830,  -9830,      0,      0,      0,      0,
	     8 ms,  12 ms,  9830,  -9830, 0x2b84,      0,      0, 0x5000,
	    41 ms, 128 ms, 16384, -16384, 0x11eb,      0,      0,      0,
	    45 ms, 103 ms,  8192,  -8192,      0,      0,      0,      0,
	   162 ms, 282 ms, 16384, -16384, 0x11eb,      0,      0, 0x6000,
	   166 ms, 238 ms,  8192,  -8192,      0,      0,      0,      0,
	   238 ms, 268 ms,  8192,  -8192,      0,      0,      0,      0,
	        0, 299 ms, 18000,      0,      0,    380,   2000, 0x7000
	};

	c.params = params;

	/*
	 * since fx params are only needed in alInit, call from here
	 * so stack storage is freed up
	 */

	alInit(&g, &c);
    } else {
	alInit(&g, &c);

    }

    freeOscStateList = &oscStates[0];
    oscStatePtr = &oscStates[0];
    for(i=0;i<(OSC_STATE_COUNT-1);i++)
    {
        oscStatePtr->next = &oscStates[i+1];
        oscStatePtr = oscStatePtr->next;
    }
    oscStatePtr->next = 0;  /* set last pointer to zero */
    
    
    seqc.maxVoices      = MAX_VOICES;
    seqc.maxEvents      = EVT_COUNT;
    seqc.maxChannels    = 16;
    seqc.heap           = &hp;
    seqc.initOsc        = &initOsc;
    seqc.updateOsc      = &updateOsc;
    seqc.stopOsc        = &stopOsc;
#ifdef DEBUG
    seqc.debugFlags     = NO_VOICE_ERR_MASK |NOTE_OFF_ERR_MASK | NO_SOUND_ERR_MASK;
#endif

#ifdef COMP_SEQ_PLAY
    alCSPNew(seqp, &seqc);
#else
    alSeqpNew(seqp, &seqc);
#endif    
    alBnkfNew((ALBankFile *)midiBankPtr, (u8 *) _miditableSegmentRomStart);
    midiBank = ((ALBankFile *)midiBankPtr)->bankArray[0];
#ifdef COMP_SEQ_PLAY    
    alCSPSetBank(seqp, midiBank);
#else
    alSeqpSetBank(seqp, midiBank);
#endif    

    /*
     * Allocate storage for sequence
     */
    seqPtr = alHeapAlloc(&hp, 1, MAX_SEQ_LENGTH);

/*
 * INITIALIZE GLOABAL TIME
 */
    while (1){
        if (seqNo >= sfile->seqCount)
            seqNo = 0;
        
        ptr = sfile->seqArray[seqNo].offset;
        len = sfile->seqArray[seqNo].len;
        if (len & 0x1)
            len++;

        osInvalDCache(seqPtr,len); 

        seqIOMessageBuf.hdr.pri      = OS_MESG_PRI_NORMAL;
        seqIOMessageBuf.hdr.retQueue = &seqMessageQ;
        seqIOMessageBuf.dramAddr     = seqPtr;
        seqIOMessageBuf.devAddr      = (u32)ptr;
        seqIOMessageBuf.size         = len;

        osEPiStartDma(handler, &seqIOMessageBuf, OS_READ);
        (void) osRecvMesg(&seqMessageQ, NULL, OS_MESG_BLOCK);

#ifdef COMP_SEQ_PLAY
        alCSeqNew(seq, seqPtr);
        alCSPSetSeq(seqp, seq);
        alCSPPlay(seqp);
#else
        /* alSeqpSetBank(seqp, midiBank); */
        alSeqNew(seq, seqPtr, len);
        alSeqpSetSeq(seqp, seq);
        alSeqpPlay(seqp);
#endif        

        /*
         * Sync up on vertical retrace - read more than 1 to be sure!
         */
        (void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);
        (void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);

	/*
	 * Note that this must be a do-while in order for seqp's state to
	 * get updated during the alAudioFrame processing.
	 */
       do {
            frame++; 

            /*
             * Where the task list goes in DRAM
             */
            tlistp = tlist[curBuf];
            cmdlp = cmdList[curBuf];       

            /*
             * Where the audio goes in DRAM
             */
            buf = curAudioBuf % 3;
            audioOp = (s16 *) osVirtualToPhysical(audioBuffer[buf]);

	    /*##### n_audio : begin #####*/
	    /*
	     * Samples in this frame
	     */
	    if((samplesLeft > (SAMPLES + EXTRA_SAMPLES)) && min_only_one)
	    {	      
	      audioSamples[buf] = minFrameSize;
	      min_only_one = 0;
	    }
	    else if(samplesLeft > (SAMPLES + EXTRA_SAMPLES))
	      audioSamples[buf] = frameSize;
	    else
	    {
	      audioSamples[buf] = frameSize;
	      min_only_one = 1;
	    }
	    /*###### n_audio : end ######*/

            /*
             * Call the frame handler
             */
#ifdef PROFILE            
            osInvalICache((void*)0x80000000, 0x100000);
            osInvalDCache((void*)0x80000000, 0x100000); 
            CPUdeltaCount = osGetCount();            
#endif

            cmdlp = alAudioFrame(cmdlp, &clcount, audioOp, audioSamples[buf]);

#ifdef PROFILE            
            CPUdeltaCount = osGetCount() - CPUdeltaCount;
            if (CPUdeltaCount > CPUmaxCount)
                CPUmaxCount = CPUdeltaCount;
            CPUavgCount += CPUdeltaCount;
#endif
            
            /*
             * Build the audio task
             */       
            tlistp->t.type = M_AUDTASK;
            tlistp->t.flags = 0x0;
            tlistp->t.ucode_boot = (u64 *) rspbootTextStart;
            tlistp->t.ucode_boot_size = ((s32)rspbootTextEnd -
                                         (s32)rspbootTextStart);

	    /*##### n_audio : begin #####*/
            tlistp->t.ucode = (u64 *) n_aspMainTextStart;
            tlistp->t.ucode_data = (u64 *) n_aspMainDataStart;
	    /*###### n_audio : end ######*/

            tlistp->t.ucode_size = 4096;
            tlistp->t.ucode_data_size = SP_UCODE_DATA_SIZE; 
            tlistp->t.data_ptr = (u64 *) cmdList[curBuf];
            tlistp->t.data_size = (cmdlp - cmdList[curBuf]) * sizeof(Acmd);

            /*
             * Video does nothing - just syncs up on the frame boundary.
             */
            (void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);

            /*
             * Find out how many samples left in the currently running
             * audio buffer
             */
            samplesLeft = IO_READ(AI_LEN_REG)>>2;

	    /*
	     * The last task should have finished before the frame message
	     * so this just clears the message queue
	     */
	    (void)osRecvMesg(&taskMessageQ, NULL, OS_MESG_BLOCK);

	    /*
	     * Point the DAC at the next buffer
	     */
	    buf = (curAudioBuf-1) % 3;

            osAiSetNextBuffer(audioBuffer[buf], audioSamples[buf]<<2);

            /*
             * Empty the dma queue to make sure all DMAs have completed.
             * If the app blocks here it is a bug since resources must
             * be managed so that all DMAs complete.
             */
            for (i=0; i<nextDMA; i++)
	      if (osRecvMesg(&dmaMessageQ, NULL, OS_MESG_NOBLOCK) == -1)
#ifdef DEBUG
		PRINTF("Dma not done\n");
#else
	        ;	    
#endif

            /*
             * Flush the cache and start task on RSP
             */
            osWritebackDCacheAll();
            osSpTaskStart(tlistp);
            CleanDMABuffs();

            /*
             * Swap buffers for wavetable storage and output
             */
            gFrameCt++;
            curBuf ^= 1; 
            curAudioBuf++;
            nextDMA = 0;

	} while (seqp->state != AL_STOPPED);

#ifdef PROFILE

#ifdef DEBUG        
        PRINTF("SeqNo %2d: CPU Max: %7d (%f %) Average: %7f (%f %)\n",seqNo,
               CPUmaxCount, CPUmaxCount*FRAME_RATE/(NUM_FIELDS*CLOCKS_SEC),
               CPUavgCount/frame, CPUavgCount*FRAME_RATE/(NUM_FIELDS*CLOCKS_SEC*frame));
#endif

        CPUmaxCount = 0;
        CPUavgCount = 0;
#endif     

	seqNo++;
        frame = 0;
    }

#ifdef COMP_SEQ_PLAY    
    alCSPStop(seqp);
#else
    alSeqpStop(seqp);
#endif
    
    alClose(&g);
    
    for(;;);
}
