/*******************************************************************
 *
 *  segment.h
 *
 *******************************************************************/

#ifndef   __SEGMENT_H__
#define   __SEGMENT_H__

extern u8   _codeSegmentStart[];         /* code segment start */
extern u8   _codeSegmentEnd[];           /* code segment end */

extern u8   _wbank1SegmentRomStart[];
extern u8   _pbank1SegmentRomStart[];
extern u8   _pbank1SegmentRomEnd[];
extern u8   _wbank2SegmentRomStart[];
extern u8   _pbank2SegmentRomStart[];
extern u8   _pbank2SegmentRomEnd[];
extern u8   _wbank3SegmentRomStart[];
extern u8   _pbank3SegmentRomStart[];
extern u8   _pbank3SegmentRomEnd[];
extern u8   _wbank4SegmentRomStart[];
extern u8   _pbank4SegmentRomStart[];
extern u8   _pbank4SegmentRomEnd[];
extern u8   _wbank5SegmentRomStart[];
extern u8   _pbank5SegmentRomStart[];
extern u8   _pbank5SegmentRomEnd[];
extern u8   _song1SegmentRomStart[];
extern u8   _song1SegmentRomEnd[];
extern u8   _song2SegmentRomStart[];
extern u8   _song2SegmentRomEnd[];
extern u8   _song3SegmentRomStart[];
extern u8   _song3SegmentRomEnd[];
extern u8   _sebank1SegmentRomStart[];
extern u8   _sebank1SegmentRomEnd[];
extern u8   _sebank2SegmentRomStart[];
extern u8   _sebank2SegmentRomEnd[];

#define WBANK_A_START   _wbank1SegmentRomStart
#define PBANK_A_START   _pbank1SegmentRomStart
#define PBANK_A_END     _pbank1SegmentRomEnd
#define WBANK_B_START   _wbank2SegmentRomStart
#define PBANK_B_START   _pbank2SegmentRomStart
#define PBANK_B_END     _pbank2SegmentRomEnd
#define WBANK_C_START   _wbank3SegmentRomStart
#define PBANK_C_START   _pbank3SegmentRomStart
#define PBANK_C_END     _pbank3SegmentRomEnd
#define WBANK_D_START   _wbank4SegmentRomStart
#define PBANK_D_START   _pbank4SegmentRomStart
#define PBANK_D_END     _pbank4SegmentRomEnd
#define WBANK_E_START   _wbank5SegmentRomStart
#define PBANK_E_START   _pbank5SegmentRomStart
#define PBANK_E_END     _pbank5SegmentRomEnd
#define SONG_A_START    _song1SegmentRomStart
#define SONG_A_END      _song1SegmentRomEnd
#define SONG_B_START    _song2SegmentRomStart
#define SONG_B_END      _song2SegmentRomEnd
#define SONG_C_START    _song3SegmentRomStart
#define SONG_C_END      _song3SegmentRomEnd
#define SEBANK_D_START  _sebank1SegmentRomStart
#define SEBANK_D_END    _sebank1SegmentRomEnd
#define SEBANK_E_START  _sebank2SegmentRomStart
#define SEBANK_E_END    _sebank2SegmentRomEnd

#endif /* __SEGMENT_H__ */
