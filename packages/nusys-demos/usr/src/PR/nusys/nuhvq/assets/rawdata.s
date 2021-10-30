.include "macros.inc"

.section .data

glabel _hvqdata1SegmentRomStart
.incbin "assets/sample1.hvq"
.balign 16
glabel _hvqdata1SegmentRomEnd

glabel _hvqdata2SegmentRomStart
.incbin "assets/sample2.hvq"
.balign 16
glabel _hvqdata2SegmentRomEnd

glabel _pbankSegmentRomStart
.incbin "sound/drumtest.ptr"
.balign 16
glabel _pbankSegmentRomEnd

glabel _wbankSegmentRomStart
.incbin "sound/drumtest.wbk"
.balign 16
glabel _wbankSegmentRomEnd

glabel _tuneSegmentRomStart
.incbin "sound/dt_song2.bin"
.balign 16
glabel _tuneSegmentRomEnd

glabel _sfxSegmentRomStart
.incbin "sound/dt_sfx1.bfx"
.balign 16
glabel _sfxSegmentRomEnd