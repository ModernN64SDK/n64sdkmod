.include "macros.inc"

.section .data

glabel _limageSegmentRomStart
.incbin "picalow.551"
.balign 16
glabel _limageSegmentRomEnd

glabel _himageSegmentRomStart
.incbin "picahigh.551"
.balign 16
glabel _himageSegmentRomEnd