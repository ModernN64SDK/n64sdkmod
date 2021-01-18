.include "macros.inc"

.section .data

.balign 16
glabel _bg_rgbaSegmentRomStart
.incbin "L2_TV.rgba5551"
glabel _bg_rgbaSegmentRomEnd