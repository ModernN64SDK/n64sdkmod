
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: topgun.h,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:20:14 $
 *---------------------------------------------------------------------*/


#define	STATIC_SEGMENT		1
#define	DYNAMIC_SEGMENT		2

#define	SCREEN_HT	240
#define	SCREEN_WD	320

#define	STACKSIZE	0x2000

#define TOPGUN_ZBUFFER 	0x3d0000

#ifdef _LANGUAGE_C

typedef struct {
	Mtx	projectionBG;
	Mtx	projection;
	Mtx	viewingBG;
	Mtx	viewing;
	Mtx	translate;
	Mtx	rotate0;
	Mtx	rotate1;
	Mtx	rotate2;
	Gfx	glist[2048];
} Dynamic;

extern u16      zbuffer[];

extern unsigned short cfb_16_a[];
extern unsigned short cfb_16_b[];

extern Dynamic	dynamic;

extern Gfx 	rspinit_dl[];
extern Gfx 	rdpinit_dl[];
extern Gfx 	clear_zbuffer[];
extern Gfx 	av8b[];
extern Gfx	ground[];
extern Gfx 	sky[];
extern Gfx 	onetri_dl[];
extern Gfx 	subtri_dl[];

extern u64	dram_stack[];
extern u64	rdp_output[];
extern u64	rdp_output_len;

/* add */
extern void dd_proc(void);

#endif	/* _LANGUAGE_C */
