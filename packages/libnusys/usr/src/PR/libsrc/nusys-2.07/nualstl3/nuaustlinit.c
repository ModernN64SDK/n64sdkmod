/*======================================================================*/
/*		NuSYS							*/
/*		nuaustlinit.c						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nuaustlinit.c,v 1.1 1999/01/07 12:16:57 ohki Exp $		*/
/*======================================================================*/
#include <nusys.h>
#include <nualstl.h>

musConfig nuAuStlConfig = {
    0,					/* control_flag		*/
    NU_AU_CHANNELS,			/* channels		*/
    NULL,				/* sched		*/
    NU_AU_MGR_THREAD_PRI,		/* thread_priority	*/
    (unsigned char*)NU_AU_HEAP_ADDR,	/* heap			*/
    NU_AU_HEAP_SIZE,			/* heap_length		*/
    NULL,				/* ptr			*/
    NULL,				/* wbk			*/
    NULL,				/* default_fxbank	*/
    NU_AU_FIFO_LENGTH,			/* fifo_length		*/
    NU_AU_SYN_UPDATE_MAX,		/* syn_updates		*/
    NU_AU_OUTPUT_RATE,			/* syn_output_rate	*/
    NU_AU_CLIST_LEN,			/* cmd buffer size 	*/
    1,					/* syn_retraceCount	*/
    NU_AU_DMA_BUFFER_NUM,		/* syn_num_dma_bufs	*/
    NU_AU_DMA_BUFFER_SIZE		/* syn_dma_buf_size	*/
};

/*----------------------------------------------------------------------*/
/*	nuAuInit - Initializes audio					*/
/*									*/
/*	IN:	nothing							*/
/*	RET:	size of the heap used					*/
/*----------------------------------------------------------------------*/
s32 nuAuStlInit(void)
{


    /* Initialize the Audio Manager. 	*/
    nuAuStlMgrInit(&nuAuStlConfig);

    /* Initialize the Sequence Player.	*/
    nuAuStlSeqPlayerInit(NU_AU_SEQ_PLAYER0, NU_AU_SONG_SIZE);
    nuAuStlSeqPlayerInit(NU_AU_SEQ_PLAYER1, NU_AU_SONG_SIZE);

    /* Initialize the Audio Player.	*/
    nuAuStlSndPlayerInit(NU_AU_SE_SIZE);

    /* Initialize the pointer bank.	*/
    nuAuStlPtrBankInit(NU_AU_SAMPLE_SIZE);
    
    /* Register the PRENMI function. */
    nuAuPreNMIFuncSet(nuAuPreNMIProc);
    
    return(nuAuStlHeapGetUsed());
}
