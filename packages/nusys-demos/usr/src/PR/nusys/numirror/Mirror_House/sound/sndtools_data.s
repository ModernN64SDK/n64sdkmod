.include "macros.inc"

#ifdef NUALSTL
.section .data

glabel _pbankSegmentRomStart
.incbin "sound/Win_Audio_Data2/sample1.ptr"
.balign 16
glabel _pbankSegmentRomEnd

glabel _wbankSegmentRomStart
.incbin "sound/Win_Audio_Data2/sample1.wbk"
.balign 16
glabel _wbankSegmentRomEnd

glabel _game_main_musicSegmentRomStart
.incbin "sound/Win_Audio_Data2/sample1.bin"
.balign 16
glabel _game_main_musicSegmentRomEnd

glabel _select_musicSegmentRomStart
.incbin "sound/Win_Audio_Data2/select.bin"
.balign 16
glabel _select_musicSegmentRomEnd

glabel _title_musicSegmentRomStart
.incbin "sound/Win_Audio_Data2/title.bin"
.balign 16
glabel _title_musicSegmentRomEnd

glabel _sfxbankSegmentRomStart
.incbin "sound/Win_Audio_Data2/sample1.bfx"
.balign 16
glabel _sfxbankSegmentRomEnd
#endif