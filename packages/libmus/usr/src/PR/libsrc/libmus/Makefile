
#############################################################
#
#  Makefile : Nintendo 64 Music Tools Programmers Library
#  (c) Copyright 1997/1998, Software Creations (Holdings) Ltd.
#
#  Version 3.14
#
#  Make file for library.
#
#############################################################

# special option #
# If you are using older N64OS than OS2.0i with patch5, uncomment next line.
# USE_OLD_LIBRARY = -D_OLD_AUDIO_LIBRARY

# command names #
LIB		= $(ROOT)/usr/lib
LPR		= $(LIB)/PR
INC		= $(ROOT)/usr/include
AR		= mips-n64-ar rc
RM		= rm -f
CC	= mips-n64-gcc -mabi=32 -ffreestanding -mfix4300 -G 0
LD	= mips-n64-ld --no-check-sections
CP		= cp
VPATH		= .

# addition files used by make #
FILE_DEPENDS	= $(VPATH)/Makefile.depend

# compile C files to object files #
.c.o:
	$(CC) $(OPTIMIZER) $(DEFINES) $(USE_OLD_LIBRARY) -G 0 -c -I.. -I$(INC) -I$(INC)/PR $<

# files to be made #
LIBFILES	= player.c player_fx.c aud_dma.c aud_samples.c aud_sched.c aud_thread.c lib_memory.c

LIBOBJECTS	= $(LIBFILES:.c=.o)

# default builds all libraries...
default:	libmus.a libn_mus.a libmus_d.a libn_mus_d.a

include $(FILE_DEPENDS)

# generate music library file #
thislib:	$(LIBOBJECTS)
	$(AR) $(LIBNAME) $(LIBOBJECTS)
	$(CP) $(LIBNAME) ..

# remove generated files #
clean:
	$(RM) libmus/*.o
	$(RM) libmus/libmus.a
	$(RM) libmus.a
	$(RM) libn_mus/*.o
	$(RM) libn_mus/libn_mus.a
	$(RM) libn_mus.a
	$(RM) libmus_d/*.o
	$(RM) libmus_d/libmus_d.a
	$(RM) libmus_d.a
	$(RM) libn_mus_d/*.o
	$(RM) libn_mus_d/libn_mus_d.a
	$(RM) libn_mus_d.a

##############################################################
## build libraries
##############################################################

# standard library
libmus.a:	$(LIBFILES)
	-mkdir -p libmus
	make -C libmus -f ../Makefile thislib LIBNAME='libmus.a' DEFINES='' OPTIMIZER='-O3' VPATH='..'


# n_audio library
libn_mus.a:	$(LIBFILES)
	-mkdir -p libn_mus
	make -C libn_mus -f ../Makefile thislib LIBNAME='libn_mus.a' DEFINES='-DSUPPORT_NAUDIO' OPTIMIZER='-O3' VPATH='..'

# debug library
libmus_d.a:	$(LIBFILES)
	-mkdir -p libmus_d
	make -C libmus_d -f ../Makefile thislib LIBNAME='libmus_d.a' DEFINES='-D_AUDIODEBUG' OPTIMIZER='-O0 -g' VPATH='..'

# debug n_audio library
libn_mus_d.a:	$(LIBFILES)
	-mkdir -p libn_mus_d
	make -C libn_mus_d -f ../Makefile thislib LIBNAME='libn_mus_d.a' DEFINES='-D_AUDIODEBUG -DSUPPORT_NAUDIO' OPTIMIZER='-O0 -g' VPATH='..'


# end of file #
