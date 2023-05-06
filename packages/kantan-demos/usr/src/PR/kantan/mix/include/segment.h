/*============================================================================

  segment.h

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

extern u8 _fiter1SegmentRomStart[];

extern u8 _fiter1SegmentRomEnd[];

extern u8 _fiter2SegmentRomStart[];

extern u8 _fiter2SegmentRomEnd[];

extern u8 _contSegmentRomStart[];

extern u8 _contSegmentRomEnd[];



#endif /* SEGMENT_H */

