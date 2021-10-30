.include "macros.inc"

.section .data

glabel _himageSegmentRomStart
.incbin "assets/picahigh.551"
.balign 16
glabel _himageSegmentRomEnd

glabel _limageSegmentRomStart
.incbin "assets/picalow.551"
.balign 16
glabel _limageSegmentRomEnd