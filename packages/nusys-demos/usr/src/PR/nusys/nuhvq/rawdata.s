.include "macros.inc"

.section .data

glabel _hvqdata1SegmentRomStart
.incbin "sample1.hvq"
.balign 16
glabel _hvqdata1SegmentRomEnd

glabel _hvqdata2SegmentRomStart
.incbin "sample2.hvq"
.balign 16
glabel _hvqdata2SegmentRomEnd

glabel _pbankSegmentRomStart
.incbin "drumtest.ptr"
.balign 16
glabel _pbankSegmentRomEnd

glabel _wbankSegmentRomStart
.incbin "drumtest.wbk"
.balign 16
glabel _wbankSegmentRomEnd

glabel _tuneSegmentRomStart
.incbin "dt_song2.bin"
.balign 16
glabel _tuneSegmentRomEnd

glabel _sfxSegmentRomStart
.incbin "dt_sfx1.bfx"
.balign 16
glabel _sfxSegmentRomEnd