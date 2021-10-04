.include "macros.inc"

.section .data

glabel _tableSegmentRomStart
.incbin "/usr/lib/n64/soundbanks/GenMidiBank.tbl"
.balign 16
glabel _tableSegmentRomEnd

glabel _bankSegmentRomStart
.incbin "/usr/lib/n64/soundbanks/GenMidiBank.ctl"
.balign 16
glabel _bankSegmentRomEnd

glabel _seqSegmentRomStart
.incbin "/usr/lib/n64/sequences/BassDrive.seq"
.balign 16
glabel _seqSegmentRomEnd