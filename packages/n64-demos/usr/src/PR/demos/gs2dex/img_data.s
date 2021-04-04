.include "macros.inc"

.section .data

glabel _bg_rgbaSegmentRomStart
.incbin "L2_TV.rgba5551"
.balign 16
glabel _bg_rgbaSegmentRomEnd