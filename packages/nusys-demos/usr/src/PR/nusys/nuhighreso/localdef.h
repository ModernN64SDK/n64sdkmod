/*======================================================================*/
/*		NuSYS high resolution sample				*/
/*		localdef.h						*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/* 97/12/12	Created by Kensaku Ohki(SLANP)				*/
/*======================================================================*/  
#ifndef _LOCALDEF_H_
#define _LOCALDEF_H_

#define	MAIN_HIGH	0x0000
#define	MAIN_LOW	0x0001
#define MAIN_DUMMY	0xFFFF
#define AUDIO_HEAP_SIZE	0x80000

#define	ZBUFFER_ADDR	(0x80400000-AUDIO_HEAP_SIZE-640*480*2*3)
#define CFB_HIGH_ADDR0	(0x80400000-AUDIO_HEAP_SIZE-640*480*2*2)
#define CFB_HIGH_ADDR1	(0x80400000-AUDIO_HEAP_SIZE-640*480*2*1)

#define CFB_LOW_ADDR0	(0x80400000-AUDIO_HEAP_SIZE-320*240*2*3)
#define CFB_LOW_ADDR1	(0x80400000-AUDIO_HEAP_SIZE-320*240*2*2)
#define CFB_LOW_ADDR2	(0x80400000-AUDIO_HEAP_SIZE-320*240*2*1)

#define	AUDIO_HEAP_ADDR	(0x80400000-AUDIO_HEAP_SIZE)

/*
 *  Macro for extern declaration of WAVE data segment
 */
#define EXTERN_SEGMENT(name)    \
extern  char    _##name##SegmentStart[], _##name##SegmentEnd[], \
                _##name##SegmentBssStart[], _##name##SegmentBssEnd[], \
                _##name##SegmentRomStart[], _##name##SegmentRomEnd[]

EXTERN_SEGMENT(code);
extern u8 _himageSegmentRomStart[], _himageSegmentRomEnd[];
extern u8 _limageSegmentRomStart[], _limageSegmentRomEnd[];

#endif /* _LOCALDEF_H_*/
