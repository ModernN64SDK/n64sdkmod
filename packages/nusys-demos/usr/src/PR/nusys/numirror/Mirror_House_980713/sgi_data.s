.include "macros.inc"

.section .data

glabel _miditableSegmentRomStart
.incbin "Sgi_Audio_Data/wave.tbl"
.balign 16
glabel _miditableSegmentRomEnd

glabel _midibankSegmentRomStart
.incbin "Sgi_Audio_Data/wave.ctl"
.balign 16
glabel _midibankSegmentRomEnd

glabel _sfxbankSegmentRomStart
.incbin "Sgi_Audio_Data/se.ctl"
.balign 16
glabel _sfxbankSegmentRomEnd

glabel _sfxtableSegmentRomStart
.incbin "Sgi_Audio_Data/se.tbl"
.balign 16
glabel _sfxtableSegmentRomEnd

glabel _seqSegmentRomStart
.incbin "Sgi_Audio_Data/midi.sbk"
.balign 16
glabel _seqSegmentRomEnd
