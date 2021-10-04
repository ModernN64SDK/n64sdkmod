.include "macros.inc"

.section .data

glabel _miditableSegmentRomStart
.incbin "sound/sampleBank.tbl"
.balign 16
glabel _miditableSegmentRomEnd

glabel _midibankSegmentRomStart
.incbin "sound/sampleBank.ctl"
.balign 16
glabel _midibankSegmentRomEnd

glabel _seqSegmentRomStart
.incbin "sound/Midnight.sbk"
.balign 16
glabel _seqSegmentRomEnd