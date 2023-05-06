.include "macros.inc"

.section .data

glabel _hvqmdataSegmentRomStart
.incbin "assets/sample.hvqm"
.balign 16
glabel _hvqmdataSegmentRomEnd