#!smake
#
# Makefile for n_audio library subdirectory
#

include /usr/include/n64/make/PRdefs

FINALLIB = libn_audio.a
DEBUGLIB = libn_audio_d.a

ARCHIVES=$(FINALLIB)

AUDIOSRC = ../libultra/audio

LCINCS = -I. -I$(AUDIOSRC) -I$(ROOT)/usr/include -I$(ROOT)/usr/include/PR
LCDEFS = -G 0 -D_ULTRA64 -DN_MICRO
LCOPTS = -DNDEBUG -D_FINALROM
OPTIMIZER = -O3

CFILES  = \
	n_auxbus.c \
	n_drvrNew.c \
	n_env.c \
	n_load.c \
	n_mainbus.c \
	n_resample.c \
	n_reverb.c \
	n_save.c \
	n_sl.c \
	\
	n_synaddplayer.c \
	n_synallocfx.c \
	n_synallocvoice.c \
	n_syndelete.c \
	n_synfreefx.c \
	n_synfreevoice.c \
	n_syngetfxref.c \
	n_syngetpriority.c \
	n_synremoveplayer.c \
	n_synsetfxmix.c \
	n_synsetfxparam.c \
	n_synsetpan.c \
	n_synsetpitch.c \
	n_synsetpriority.c \
	n_synsetvol.c \
	n_synstartvoice.c \
	n_synstartvoiceparam.c \
	n_synstopvoice.c \
	n_synthesizer.c\
	\
	n_seqplayer.c\
	n_seqpdelete.c\
	n_seqpgetchlvol.c\
	n_seqpgetfxmix.c\
	n_seqpgetpan.c\
	n_seqpgetpriority.c\
	n_seqpgetprogram.c\
	n_seqpgetseq.c\
	n_seqpgetstate.c\
	n_seqpgettempo.c\
	n_seqpgetvol.c\
	n_seqpplay.c\
	n_seqpsendmidi.c\
	n_seqpsetbank.c\
	n_seqpsetchlvol.c\
	n_seqpsetfxmix.c\
	n_seqpsetpan.c\
	n_seqpsetpriority.c\
	n_seqpsetprogram.c\
	n_seqpsetseq.c\
	n_seqpsettempo.c\
	n_seqpsetvol.c\
	n_seqpstop.c\
	n_seqploop.c\
	\
	n_csplayer.c\
	n_cspdelete.c\
	n_cspgetchlvol.c\
	n_cspgetfxmix.c\
	n_cspgetpan.c\
	n_cspgetpriority.c\
	n_cspgetprogram.c\
	n_cspgetseq.c\
	n_cspgetstate.c\
	n_cspgettempo.c\
	n_cspgetvol.c\
	n_cspplay.c\
	n_cspsendmidi.c\
	n_cspsetbank.c\
	n_cspsetchlvol.c\
	n_cspsetfxmix.c\
	n_cspsetpan.c\
	n_cspsetpriority.c\
	n_cspsetprogram.c\
	n_cspsetseq.c\
	n_cspsettempo.c\
	n_cspsetvol.c\
	n_cspstop.c\
	\
	n_sndpallocate.c\
	n_sndpdeallocate.c\
	n_sndpdelete.c\
	n_sndpgetstate.c\
	n_sndplayer.c\
	n_sndpplay.c\
	n_sndpplayat.c\
	n_sndpsetfxmix.c\
	n_sndpsetpan.c\
	n_sndpsetpitch.c\
	n_sndpsetpriority.c\
	n_sndpsetvol.c\
	n_sndpstop.c\
	n_sndpgetsound.c\
	n_sndpsetsound.c\
	\
	n_event.c\
	n_seq.c\
	n_csq.c

COBJECTS = $(CFILES:.c=.o)

LDIRT = $(ARCHIVES)

$(ARCHIVES): $(COBJECTS)
	ar r $(ARCHIVES) $(COBJECTS)

deb:
	rm -f *.o
	make ARCHIVES=$(DEBUGLIB) OPTIMIZER='-g -O' LCOPTS='-D_DEBUG'

include $(COMMONRULES)

default: $(ARCHIVES)
