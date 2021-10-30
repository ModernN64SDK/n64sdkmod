/*
   segment.h

   Declare external reference of the segment symbol that makerom outputs 

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.			
*/


#ifndef SEGMENT_H
#define SEGMENT_H

extern char _codeSegmentStart[];         /* code segment start */
extern char _codeSegmentEnd[];           /* code segment end */

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

extern u8 _stage00SegmentStart[];
extern u8 _stage00SegmentEnd[];
extern u8 _stage00SegmentRomStart[];
extern u8 _stage00SegmentRomEnd[];
extern u8 _stage00SegmentTextStart[];
extern u8 _stage00SegmentTextEnd[];
extern u8 _stage00SegmentDataStart[];
extern u8 _stage00SegmentDataEnd[];
extern u8 _stage00SegmentBssStart[];
extern u8 _stage00SegmentBssEnd[];

extern u8 _stage01SegmentStart[];
extern u8 _stage01SegmentEnd[];
extern u8 _stage01SegmentRomStart[];
extern u8 _stage01SegmentRomEnd[];
extern u8 _stage01SegmentTextStart[];
extern u8 _stage01SegmentTextEnd[];
extern u8 _stage01SegmentDataStart[];
extern u8 _stage01SegmentDataEnd[];
extern u8 _stage01SegmentBssStart[];
extern u8 _stage01SegmentBssEnd[];

#endif /* SEGMENT_H */
