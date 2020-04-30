/*******************************************************************
 *
 *  segment.h
 *
 *******************************************************************/

#ifndef   __SEGMENT_H__
#define   __SEGMENT_H__

extern u8   _codeSegmentStart[];         /* code segment start */
extern u8   _codeSegmentEnd[];           /* code segment end */

extern u8 _wbank1SegmentRomStart[];
extern u8 _pbank1SegmentRomStart[];
extern u8 _pbank1SegmentRomEnd[];
extern u8 _song1SegmentRomStart[];
extern u8 _song1SegmentRomEnd[];
extern u8 _song2SegmentRomStart[];
extern u8 _song2SegmentRomEnd[];
extern u8 _sebank1SegmentRomStart[];
extern u8 _sebank1SegmentRomEnd[];

#define WBANK1_START    _wbank1SegmentRomStart
#define PBANK1_START    _pbank1SegmentRomStart
#define PBANK1_END      _pbank1SegmentRomEnd
#define SONG1_START     _song1SegmentRomStart
#define SONG1_END       _song1SegmentRomEnd
#define SONG2_START     _song2SegmentRomStart
#define SONG2_END       _song2SegmentRomEnd
#define SEBANK1_START   _sebank1SegmentRomStart
#define SEBANK1_END     _sebank1SegmentRomEnd

#endif /* __SEGMENT_H__ */
