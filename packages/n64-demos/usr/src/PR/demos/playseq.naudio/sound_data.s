.include "macros.inc"

.section .data

glabel _miditableSegmentRomStart
.incbin "sampleBank.tbl"
.balign 16
glabel _miditableSegmentRomEnd

glabel _midibankSegmentRomStart
.incbin "sampleBank.ctl"
.balign 16
glabel _midibankSegmentRomEnd

glabel _seqSegmentRomStart
.incbin "Midnight.sbk"
.balign 16
glabel _seqSegmentRomEnd