.include "macros.inc"

.section .data

.balign 16
glabel _miditableSegmentRomStart
.incbin "sound/wave.tbl"
glabel _miditableSegmentRomEnd

.balign 16
glabel _midibankSegmentRomStart
.incbin "sound/wave.ctl"
glabel _midibankSegmentRomEnd

.balign 16
glabel _sfxbankSegmentRomStart
.incbin "sound/se.ctl"
glabel _sfxbankSegmentRomEnd

.balign 16
glabel _sfxtableSegmentRomStart
.incbin "sound/se.tbl"
glabel _sfxtableSegmentRomEnd

.balign 16
glabel _seqSegmentRomStart
.incbin "sound/midi.sbk"
glabel _seqSegmentRomEnd