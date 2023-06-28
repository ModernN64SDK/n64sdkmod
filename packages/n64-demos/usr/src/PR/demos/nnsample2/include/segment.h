/*============================================================================

		NINTENDO64 TECHNICAL SUPPORT CENTER 
		
		    NINTENDO64 SAMPLE PROGRAM 2

		Copyright (C) 1997, NINTENDO Co,Ltd.

============================================================================*/

#ifndef SEGMENT_H
#define SEGMENT_H


/* Segment symbol */

extern char _codeSegmentStart[];         /* code segment start */
extern char _codeSegmentEnd[];           /* code segment end */
extern u8 _gfxdlistSegmentBssStart[];
extern u8 _gfxdlistSegmentBssEnd[];

extern u8 _seqSegmentRomStart[];
extern u8 _seqSegmentRomEnd[];
extern u8 _midibankSegmentRomStart[];
extern u8 _midibankSegmentRomEnd[];
extern u8 _miditableSegmentRomStart[];
extern u8 _miditableSegmentRomEnd[];
extern u8 _sfxbankSegmentRomStart[];
extern u8 _sfxbankSegmentRomEnd[];
extern u8 _sfxtableSegmentRomStart[];
extern u8 _sfxtableSegmentRomEnd[];
extern u8 _lodmodelSegmentRomStart[];
extern u8 _lodmodelSegmentRomEnd[];

#endif /* SEGMENT_H */


