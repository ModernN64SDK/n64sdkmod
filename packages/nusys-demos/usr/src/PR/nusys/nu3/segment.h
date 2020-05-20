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

#endif /* SEGMENT_H */
