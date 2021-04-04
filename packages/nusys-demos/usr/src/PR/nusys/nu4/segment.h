/*
   segment.h

   Declare external reference of the segment symbol that makerom outputs 
    ( Not used in the sample this time.)

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.	
*/


#ifndef SEGMENT_H
#define SEGMENT_H

extern char _codeSegmentStart[];         /* code segment start */
extern char _codeSegmentEnd[];           /* code segment end */

extern u8 _wbankSegmentRomStart[];
extern u8 _pbankSegmentRomStart[];
extern u8 _pbankSegmentRomEnd[];
extern u8 _tune1SegmentRomStart[];
extern u8 _tune1SegmentRomEnd[];
extern u8 _tune2SegmentRomStart[];
extern u8 _tune2SegmentRomEnd[];
extern u8 _tune3SegmentRomStart[];
extern u8 _tune3SegmentRomEnd[];
extern u8 _sfxSegmentRomStart[];
extern u8 _sfxSegmentRomEnd[];

#define WBANK_START	_wbankSegmentRomStart
#define PBANK_START	_pbankSegmentRomStart
#define PBANK_END	_pbankSegmentRomEnd
#define MUSIC1_START	_tune1SegmentRomStart
#define MUSIC1_END	_tune1SegmentRomEnd
#define MUSIC2_START	_tune2SegmentRomStart
#define MUSIC2_END	_tune2SegmentRomEnd
#define MUSIC3_START	_tune3SegmentRomStart
#define MUSIC3_END	_tune3SegmentRomEnd
#define SFX_START	_sfxSegmentRomStart
#define SFX_END		_sfxSegmentRomEnd

#endif /* SEGMENT_H */
