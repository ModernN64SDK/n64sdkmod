.include "macros.inc"

#ifdef NUALSGI
.section .data

glabel _miditableSegmentRomStart
.incbin "sound/Sgi_Audio_Data/wave.tbl"
.balign 16
glabel _miditableSegmentRomEnd

glabel _midibankSegmentRomStart
.incbin "sound/Sgi_Audio_Data/wave.ctl"
.balign 16
glabel _midibankSegmentRomEnd

glabel _sfxbankSegmentRomStart
.incbin "sound/Sgi_Audio_Data/se.ctl"
.balign 16
glabel _sfxbankSegmentRomEnd

glabel _sfxtableSegmentRomStart
.incbin "sound/Sgi_Audio_Data/se.tbl"
.balign 16
glabel _sfxtableSegmentRomEnd

glabel _seqSegmentRomStart
.incbin "sound/Sgi_Audio_Data/midi.sbk"
.balign 16
glabel _seqSegmentRomEnd
#endif