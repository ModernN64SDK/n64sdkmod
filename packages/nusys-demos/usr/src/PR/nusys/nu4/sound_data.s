.include "macros.inc"

.section .data

glabel _pbankSegmentRomStart
.incbin "drumtest.ptr"
.balign 16
glabel _pbankSegmentRomEnd

glabel _wbankSegmentRomStart
.incbin "drumtest.wbk"
.balign 16
glabel _wbankSegmentRomEnd

glabel _tune1SegmentRomStart
.incbin "dt_song1.bin"
.balign 16
glabel _tune1SegmentRomEnd

glabel _tune2SegmentRomStart
.incbin "dt_song2.bin"
.balign 16
glabel _tune2SegmentRomEnd

glabel _tune3SegmentRomStart
.incbin "dt_song3.bin"
.balign 16
glabel _tune3SegmentRomEnd

glabel _sfxSegmentRomStart
.incbin "dt_sfx1.bfx"
.balign 16
glabel _sfxSegmentRomEnd