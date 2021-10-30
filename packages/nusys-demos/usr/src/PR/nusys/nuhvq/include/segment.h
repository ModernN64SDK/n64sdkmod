/*
   segment.h

   Declare external references for the segment symbols output by makerom 
      (Not used in this sample program)
*/


#ifndef SEGMENT_H
#define SEGMENT_H

extern char _codeSegmentStart[];         /* code segment start */
extern char _codeSegmentEnd[];           /* code segment end */

extern u8 _wbankSegmentRomStart[];
extern u8 _pbankSegmentRomStart[];
extern u8 _pbankSegmentRomEnd[];
extern u8 _tuneSegmentRomStart[];
extern u8 _tuneSegmentRomEnd[];
extern u8 _sfxSegmentRomStart[];
extern u8 _sfxSegmentRomEnd[];

#define WBANK_START	_wbankSegmentRomStart
#define PBANK_START	_pbankSegmentRomStart
#define PBANK_END	_pbankSegmentRomEnd
#define MUSIC_START	_tuneSegmentRomStart
#define MUSIC_END	_tuneSegmentRomEnd
#define SFX_START	_sfxSegmentRomStart
#define SFX_END		_sfxSegmentRomEnd

#endif /* SEGMENT_H */
