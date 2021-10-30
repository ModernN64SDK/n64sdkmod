.include "macros.inc"

.section .data

glabel _miditableSegmentRomStart
.incbin "sound/wave.tbl"
.balign 16
glabel _miditableSegmentRomEnd

glabel _midibankSegmentRomStart
.incbin "sound/wave.ctl"
.balign 16
glabel _midibankSegmentRomEnd

glabel _sfxbankSegmentRomStart
.incbin "sound/se.ctl"
.balign 16
glabel _sfxbankSegmentRomEnd

glabel _sfxtableSegmentRomStart
.incbin "sound/se.tbl"
.balign 16
glabel _sfxtableSegmentRomEnd

glabel _seq1SegmentRomStart
.incbin "sound/midi.sbk"
.balign 16
glabel _seq1SegmentRomEnd