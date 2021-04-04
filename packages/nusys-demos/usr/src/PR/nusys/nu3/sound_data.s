.include "macros.inc"

.section .data

glabel _miditableSegmentRomStart
.incbin "wave.tbl"
.balign 16
glabel _miditableSegmentRomEnd

glabel _midibankSegmentRomStart
.incbin "wave.ctl"
.balign 16
glabel _midibankSegmentRomEnd

glabel _sfxbankSegmentRomStart
.incbin "se.ctl"
.balign 16
glabel _sfxbankSegmentRomEnd

glabel _sfxtableSegmentRomStart
.incbin "se.tbl"
.balign 16
glabel _sfxtableSegmentRomEnd

glabel _seqSegmentRomStart
.incbin "midi.sbk"
.balign 16
glabel _seqSegmentRomEnd