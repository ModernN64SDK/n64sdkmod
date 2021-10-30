/*
 *  NUSYSTEM -HVQ library  Sample program
   stage00.c
Copyright (C) 1999 NINTENDO Co.,Ltd.
Copyright (C) 1999 MONEGI CORPORATION
*/

#include <assert.h>
#include <nusys.h>
#include "localdef.h"
#include "main.h"
#include <hvq2dec.h>
#include <PR/gs2dex.h>
#include <nualstl_n.h>
#include "segment.h"
#define GFX_LIST_NUM		4
#define GFX_LIST_SIZE		2048

uObjBg	objBghvq = {
    0, 320<<2, 0, 320<<2,
    0, 240<<2, 0, 240<<2,
    (u64*)NULL,
    G_BGLT_LOADTILE,              /* imageLoad                      */
    G_IM_FMT_RGBA,                /* imageFmt                       */
    G_IM_SIZ_16b,                 /* imageSiz                       */
    0,                            /* imagePal                       */
    0                             /* imageFlip                      */
};

Gfx rdpInit_dl[] = {
    gsDPPipeSync(),
    gsDPPipelineMode(G_PM_1PRIMITIVE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPSetCombineKey(G_CK_NONE),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPSetColorDither(G_CD_DISABLE),
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0,640, 480),
    gsSPEndDisplayList(),
};

Gfx spriteInit_dl[] = {
    gsDPPipeSync(),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsSPEndDisplayList(),
};

Gfx	gfxListBuf[GFX_LIST_NUM][GFX_LIST_SIZE];
u32	gfxListCnt = 0;
Gfx*	gfxListPtr;
Gfx*	gfxListStartPtr;
static u32	seqHandle = 0;
static u32	sndHandle = 0;
int ptr_buf[NU_AU_SAMPLE_SIZE] __attribute__((aligned (16)));
int tune_buf[NU_AU_SONG_SIZE] __attribute__((aligned (16)));
int sfx_buf[NU_AU_SE_SIZE] __attribute__((aligned (16)));
/* Declare prototypes  */
void soundCheck(void);
void Rom2Ram(void *, void *, s32);
void decodeHVQ(void);
char conbuf[40];
extern u16 hvqimg[SCREEN_WD*SCREEN_HT];
s32 cpudelta;
s32 rspdelta;
/* Initialize stage 0 */
void initStage00(void)
{

  decodeHVQ();
  /* Load and register sample bank */
  Rom2Ram((void *)PBANK_START, (void *)ptr_buf, PBANK_END-PBANK_START);
  MusPtrBankInitialize(ptr_buf, WBANK_START);

  /* Load song */
  Rom2Ram((void *)MUSIC_START, (void *)tune_buf, MUSIC_END-MUSIC_START);

  /* Load and register sound effect */
  Rom2Ram((void *)SFX_START, (void *)sfx_buf, SFX_END-SFX_START);
  MusFxBankInitialize(sfx_buf);

  /* Playback sequence */
  seqHandle = MusStartSong(tune_buf);
}
void decodeHVQ(void);
/*----------------------------------------------------------------------*/
/*	Update display list buffer                 			*/
/*	IN:	None							*/
/*	RET:	Pointer to display list buffer  			*/
/*----------------------------------------------------------------------*/
void gfxListBufferChange(void)
{
    gfxListStartPtr = &gfxListBuf[gfxListCnt][0];
    gfxListPtr = gfxListStartPtr;
    gfxListCnt++;
    gfxListCnt %= GFX_LIST_NUM;
    return;
}
/*----------------------------------------------------------------------------
  gfxClearCfb

  Set address and claer frame buffer/Z buffer

  Uses the NuSYSTEM global variables nuGfxZBuffer (Z buffer address) and
  nuGfxCfb_ptr (frame buffer address).
----------------------------------------------------------------------------*/
void gfxClearCfb(void)
{
  /* Clear the Z buffer */
  gDPSetDepthImage(gfxListPtr++, OS_K0_TO_PHYSICAL(nuGfxZBuffer));
  gDPSetCycleType(gfxListPtr++, G_CYC_FILL);
  gDPSetColorImage(gfxListPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b,SCREEN_WD,
		   OS_K0_TO_PHYSICAL(nuGfxZBuffer));
  gDPSetFillColor(gfxListPtr++,(GPACK_ZDZ(G_MAXFBZ,0) << 16 |
			       GPACK_ZDZ(G_MAXFBZ,0)));
  gDPFillRectangle(gfxListPtr++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
  gDPPipeSync(gfxListPtr++);

    /* Clear the frame buffer  */
  gDPSetColorImage(gfxListPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
		   osVirtualToPhysical(nuGfxCfb_ptr));
  gDPSetFillColor(gfxListPtr++, (GPACK_RGBA5551(0, 0, 0, 1) << 16 |
				GPACK_RGBA5551(0, 0, 0, 1)));
  gDPFillRectangle(gfxListPtr++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
  gDPPipeSync(gfxListPtr++);
}
/* Make display list and start task */
void makeDL00(void)
{
    uObjBg*	objBgPtr;
    /* Do not process if task not finished */
    if(nuGfxTaskSpool) return;
    /* Set the display list buffer */
    gfxListBufferChange();
    gSPDisplayList(gfxListPtr++, rdpInit_dl);
    /* Clear the screen */
    gfxClearCfb();

    /* end top-level display list */
    gDPFullSync(gfxListPtr++);
    gSPEndDisplayList(gfxListPtr++);

    nuGfxTaskStart(gfxListStartPtr, gfxListPtr - gfxListStartPtr,
		   NU_GFX_UCODE_S2DEX,NU_SC_NOSWAPBUFFER);

    gfxListBufferChange();

    gSPDisplayList(gfxListPtr++, rdpInit_dl);

    gSPDisplayList(gfxListPtr++, spriteInit_dl);

    gDPSetRenderMode(gfxListPtr++, G_RM_SPRITE, G_RM_SPRITE2);
    gDPSetCycleType(gfxListPtr++, G_CYC_COPY);
    gDPSetTextureFilter(gfxListPtr++, G_TF_POINT);
	objBgPtr = &objBghvq;

	objBgPtr->b.imagePtr = (u64*)hvqimg;
    guS2DInitBg(objBgPtr);

    objBgPtr->s.frameX = 1<<2;
    objBgPtr->s.frameY = 1<<2;

	gSPBgRectCopy(gfxListPtr++, objBgPtr);

    /* end top-level display list */
    gDPFullSync(gfxListPtr++);
    gSPEndDisplayList(gfxListPtr++);

    nuGfxTaskStart(gfxListStartPtr, gfxListPtr - gfxListStartPtr,
		   NU_GFX_UCODE_S2DEX,NU_SC_NOSWAPBUFFER);
    gfxListBufferChange();
//    nuDebTaskPerfBar0( 6,200,NU_SC_NOSWAPBUFFER );
    nuDebConDisp(NU_SC_SWAPBUFFER);
}


/* Game advance process for stage 0 */
void updateGame00(void)
{
  /* Read data from Controller 1 */

	nuContDataGetEx(contdata,0);
	if(contdata[0].errno != 0){
		osSyncPrintf("contdata= %d\n",contdata[0].errno);
    }
	soundCheck();
  /* Use A button to switch images */
	if(contdata[0].trigger & A_BUTTON){
		if(hvqno==1)
			hvqno=2;
		else
			hvqno=1;
		decodeHVQ();
   }

  /* Use B button to switch decode method  bug == ?*/
  if(contdata[0].trigger == B_BUTTON)
  {
	  if(useRSP==0)
		  useRSP=1;
	  else
		  useRSP=0;
	  cpudelta=0;
	  rspdelta=0;
	  decodeHVQ();
  }
}
extern u32 HVQbuf[];		/* Buffer for compressed image data */
extern u16 hvqwork[];		/* Work area needed for HVQ library */
HVQ2Arg hvq_sparg;		/* Parameters for the HVQ microcode */
extern HVQ2Info hvq_spfifo[];	/* Data area for the HVQ microcode */

void decodeHVQ(void)
{
	int datasize;
	int h_offset, v_offset;
	void *screen_ptr;
	int j;

/* Initialize the HVQ decoder (only needs to be done once at start) */
	if (useRSP)
		hvq2InitSP(0x0);
	else
		hvq2Init1(0x0);

/* Check capacity of buffer for compressed data */
	if(hvqno==1)
		datasize = _hvqdata1SegmentRomEnd - _hvqdata1SegmentRomStart;
	else
		datasize = _hvqdata2SegmentRomEnd - _hvqdata2SegmentRomStart;
	if ( datasize > HVQ_DATASIZE_MAX ) {
		osSyncPrintf( "ERROR: HVQbuf insufficient\n" );
    return;
	}
/* Copy the compressed data from ROM */
	if(hvqno==1)
		nuPiReadRom((u32)_hvqdata1SegmentRomStart,HVQbuf,datasize);
	else
		nuPiReadRom((u32)_hvqdata2SegmentRomStart,HVQbuf,datasize);

/* Show compressed data information in debugger */

	osSyncPrintf("\n" );
	osSyncPrintf("File version  : %s\n", ((HVQ2Header *)HVQbuf)->file_version);
	osSyncPrintf("File size     : %d\n", ((HVQ2Header *)HVQbuf)->file_size);
	osSyncPrintf("Image width   : %d\n", ((HVQ2Header *)HVQbuf)->width);
	osSyncPrintf("Image height  : %d\n", ((HVQ2Header *)HVQbuf)->height);
	osSyncPrintf("Compress type : %s\n", ((HVQ2Header *)HVQbuf)->v_sampling_rate == 1 ? "4:2:2" : "4:1:1");
	osSyncPrintf("\n");
	nuDebConClear(0);
	nuDebConTextPos(0,2,2);
	sprintf(conbuf,"File version  : %s\n", ((HVQ2Header *)HVQbuf)->file_version);
	nuDebConCPuts(0, conbuf);
	nuDebConTextPos(0,2,3);
	sprintf(conbuf,"File size     : %d\n", ((HVQ2Header *)HVQbuf)->file_size);
	nuDebConCPuts(0, conbuf);
	nuDebConTextPos(0,2,4);
	sprintf(conbuf,"Image width   : %d\n", ((HVQ2Header *)HVQbuf)->width);
	nuDebConCPuts(0, conbuf);
	nuDebConTextPos(0,2,5);
	sprintf(conbuf,"Image height  : %d\n", ((HVQ2Header *)HVQbuf)->height);
	nuDebConCPuts(0, conbuf);
	nuDebConTextPos(0,2,6);
	sprintf(conbuf,"Compress type : %s\n", ((HVQ2Header *)HVQbuf)->v_sampling_rate == 1 ? "4:2:2" : "4:1:1");
	nuDebConCPuts(0, conbuf);
/*   Clear the expanded image area.
     Adjust so a small image is expanded in the middle of this area.
*/
	for ( j = 0; j < SCREEN_WD * SCREEN_HT; j++ ) hvqimg[j] = 0;
	h_offset = (SCREEN_WD - ((HVQ2Header *)HVQbuf)->width) / 2;
	v_offset = (SCREEN_HT - ((HVQ2Header *)HVQbuf)->height) / 2;
	screen_ptr = &hvqimg[SCREEN_WD * v_offset + h_offset];
/* Decode the compressed data and expand the image in the frame buffer */
	if (useRSP){
/* If using the RSP version decoder, check to see whether 
   the RSP data area is big enough */
		if ( ((HVQ2Header *)HVQbuf)->sp_packets > HVQ_SPFIFO_SIZE ) {
			osSyncPrintf( "ERROR: hvq_spfifo insufficient\n" );
			return;
		}
		cpudelta = osGetCount();
/* First half of process done in CPU */
		hvq2DecodeSP(HVQbuf, screen_ptr, SCREEN_WD, hvqwork, &hvq_sparg, hvq_spfifo);
		cpudelta = (rspdelta = osGetCount()) - cpudelta;
/* Last half of process done in RSP  */
   		nuGfxTaskStart((Gfx *)&hvq_sparg, sizeof (hvq_sparg),APP_GFX_UCODE_HVQ,NU_SC_NOSWAPBUFFER | NU_SC_NORDP);
		nuGfxTaskAllEndWait();
		rspdelta = osGetCount() - rspdelta;// Not precise, because of time to interrupt audio.
		osSyncPrintf("RSP decoder ---> CPU %lldms + RSP %lldms\n", (OS_CYCLES_TO_USEC(cpudelta) + 500) / 1000, (OS_CYCLES_TO_USEC(rspdelta) + 500) / 1000);
        nuDebConTextPos(0,2,22);
        sprintf(conbuf,"RSP decoder ---> CPU %lldms + RSP %lldms\n", (OS_CYCLES_TO_USEC(cpudelta) + 500) / 1000, (OS_CYCLES_TO_USEC(rspdelta) + 500) / 1000);
	    nuDebConCPuts(0, conbuf);
	}else{
/* If using the CPU version decoder */
		cpudelta = osGetCount();
		hvq2Decode1(HVQbuf, screen_ptr, SCREEN_WD, hvqwork);
		cpudelta = osGetCount() - cpudelta;
		osSyncPrintf("CPU decoder ---> %lldms\n", (OS_CYCLES_TO_USEC(cpudelta) + 500) / 1000);
        nuDebConTextPos(0,2,22);
        sprintf(conbuf,"CPU decoder ---> CPU %lldms             \n", (OS_CYCLES_TO_USEC(cpudelta) + 500) / 1000);
	    nuDebConCPuts(0, conbuf);
    }
    nuDebConTextPos(0,2,21);
    sprintf(conbuf,"USE_RSP= %d\n", useRSP);
    nuDebConCPuts(0, conbuf);
}
/* Use Controller buttons to  play and control audio */
void soundCheck(void)
{
  static int snd_no = 0;
  /* static int seq_no = 0; */

  /* Play the audio in order using left/right on Control Pad */
  if((contdata[0].trigger & L_JPAD) || (contdata[0].trigger & R_JPAD))
    {
      if(snd_no)
	MusHandleStop(sndHandle,0);

      if(contdata[0].trigger & L_JPAD)
	{
	  snd_no--;
	  if(snd_no < 1) snd_no = 5;
	}
      else
	{
	  snd_no++;
	  if(snd_no > 5) snd_no = 1;
	}

      sndHandle = MusStartEffect(snd_no);

    }

  /* Use L & R buttons to change tempo of sequence playback */
  if((contdata[0].trigger & L_TRIG) || (contdata[0].trigger & R_TRIG))
    {
      if(contdata[0].trigger & L_TRIG)
	MusHandleSetTempo(seqHandle,0x100);
      else
	MusHandleSetTempo(seqHandle,0x40);
    }

  /* Use Z button to play & stop sequence playback */
  if( contdata[0].trigger & Z_TRIG )
    MusHandleStop(seqHandle,0);
}


/*----------------------------------------------------------------------*/
/*	Rom2Ram - Read data from ROM into RAM    			*/
/*	IN:	from_addr	Data origin ROM address 		*/
/*		to_addr		Data destination RAM address		*/
/*		seq_size	Size of data to read    		*/
/*	RET:	None    						*/
/*----------------------------------------------------------------------*/
void Rom2Ram(void *from_addr, void *to_addr, s32 seq_size)
{
    /* If size is odd-numbered, cannot send over PI, so make it even */
    if(seq_size & 0x00000001) seq_size++;

    nuPiReadRom((u32)from_addr, to_addr, seq_size);
}
