
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.(Originated by SGI)
        
        $RCSfile: onetri.h,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:20:05 $
 *---------------------------------------------------------------------*/



#define	STATIC_SEGMENT		1
#define	CFB_SEGMENT		2

#define	SCREEN_HT	240
#define	SCREEN_WD	320

#define	STACKSIZE	0x2000

#define	GLIST_LEN	2048
#ifdef _LANGUAGE_C

typedef struct {
	Mtx	projection;
	Mtx	modeling;
	Mtx	viewing;
        Mtx     identity;
	Gfx	glist[GLIST_LEN];
} Dynamic;

extern Dynamic	dynamic;

/* these are the static display lists */

extern Gfx 	rspinit_dl[];
extern Gfx 	rdpinit_dl[];
extern Gfx	clearcfb_dl[];
extern Gfx 	shadetri_dl[];
extern Gfx 	textri_dl[];

/* RSP task data that is modified by the RSP and read the the CPU */

extern u64 dram_stack[];	/* used for matrix stack */
extern u64 rdp_output_len;	/* RSP writes back size of RDP data */
extern u64 rdp_output[];	/* RSP writes back RDP data */

/* CPU addresses for the color frame buffer */

extern u16	cfb[][SCREEN_WD*SCREEN_HT];

/* RSP address for the color frame buffer */

extern u16	rsp_cfb[];

/* add */
extern void dd_proc(void);

#endif	/* _LANGUAGE_C */
