.include "macros.inc"

.section .data

glabel _sfxbankSegmentRomStart
.incbin "/usr/lib/n64/soundbanks/sfx.ctl"
.balign 16
glabel _sfxbankSegmentRomEnd

glabel _sfxtableSegmentRomStart
.incbin "/usr/lib/n64/soundbanks/sfx.tbl"
.balign 16
glabel _sfxtableSegmentRomEnd
