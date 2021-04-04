INC = /usr/include/n64
RM		= rm -f
CC	= mips-n64-gcc -mabi=32 -ffreestanding -mfix4300 -G 0
LD	= mips-n64-ld --no-check-sections

# If you are using older N64OS than OS2.0i with patch5, uncomment next line.
# OLD_AUDIO_DEF = -D_OLD_AUDIO_LIBRARY

ARCHIVES = libn_audio_sc.a

.c.o:
	$(CC) -O3 -G 0 -c -I. -I$(INC) -I$(INC)/PR -DN_MICRO $(OLD_AUDIO_DEF) $<

LIBFILES = \
	n_auxbus.c \
	n_drvrNew.c \
	n_env.c \
	n_load.c \
	n_mainbus.c \
	n_resample.c \
	n_reverb.c \
	n_save.c \
	n_sl.c \
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
	n_synsetfxtype.c \
	n_synsetpan.c \
	n_synsetpitch.c \
	n_synsetpriority.c \
	n_synsetvol.c \
	n_synstartvoice.c \
	n_synstartvoiceparam.c \
	n_synstopvoice.c \
	n_synthesizer.c

LIBOBJECTS = $(LIBFILES:.c=.o)

default: $(ARCHIVES)

clean:
	$(RM) $(ARCHIVES)
	$(RM) *.o

$(ARCHIVES): $(LIBOBJECTS)
	mips-n64-ar rc $(ARCHIVES) $(LIBOBJECTS)
