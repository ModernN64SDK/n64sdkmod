.include "macros.inc"

.section .data

.balign 16
glabel _tableSegmentRomStart
.incbin "/usr/lib/n64/soundbanks/GenMidiBank.tbl"
glabel _tableSegmentRomEnd

.balign 16
glabel _bankSegmentRomStart
.incbin "/usr/lib/n64/soundbanks/GenMidiBank.ctl"
glabel _bankSegmentRomEnd

.balign 16
glabel _seqSegmentRomStart
.incbin "/usr/lib/n64/sequences/BassDrive.seq"
glabel _seqSegmentRomEnd