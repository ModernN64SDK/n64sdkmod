.include "macros.inc"

.section .data

.balign 16
glabel _miditableSegmentRomStart
.incbin "wave.tbl"
glabel _miditableSegmentRomEnd

.balign 16
glabel _midibankSegmentRomStart
.incbin "wave.ctl"
glabel _midibankSegmentRomEnd

.balign 16
glabel _sfxbankSegmentRomStart
.incbin "se.ctl"
glabel _sfxbankSegmentRomEnd

.balign 16
glabel _sfxtableSegmentRomStart
.incbin "se.tbl"
glabel _sfxtableSegmentRomEnd

.balign 16
glabel _seqSegmentRomStart
.incbin "midi.sbk"
glabel _seqSegmentRomEnd