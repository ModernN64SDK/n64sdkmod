#!smake -k
################################################################
#
# 	NuSystem Makefile for NINTENDO64			
#
#	Copyright (C) 1998, NINTENDO Co,Ltd.
#
################################################################
include $(ROOT)/usr/include/make/PRdefs


################################################################
#	DEFINE							
################################################################
TARGETS	=	all
MAKE		=	make
OPTIMIZER	=	$(OPTMZ)

RM		=	rm
CP		=	cp
AR		=	mips-n64-ar
ARFLAGS	=	cr

################################################################
#	Target	Library 
################################################################
# for audio micro code
ROMLIB		=	libnualsgi.a
DEBUGLIB	=	libnualsgi_d.a

# for naudio micro code
NROMLIB	=	libnualsgi_n.a
NDEBUGLIB	=	libnualsgi_n_d.a


################################################################
#	Install Directory
################################################################
NUSYSINCDIR	= 	/usr/include/n64/nusys
NUSYSLIBDIR	= 	/usr/lib/n64/nusys

################################################################
#	object temporary						
################################################################
#	for audio micro-code
ROMOBJDIR	=	libnualsgi
DEBUGOBJDIR	=	libnualsgi_d

#	for n_audio micro-code
NROMOBJDIR	=	libnualsgi_n
NDEBUGOBJDIR	=	libnualsgi_n_d

################################################################
# Library Source
################################################################
LIBALSGISRC	=	nuauinit.c			\
			nuauinitex.c			\
			nuauseqplayerinit.c		\
			nuauseqplayerbankset.c		\
			nuauseqplayerseqset.c		\
			nuauseqplayersetno.c		\
			nuauseqplayerplay.c		\
			nuauseqplayerfadeout.c		\
			nuausndplayerinit.c		\
			nuausndplayerbankset.c		\
			nuausndplayerplay.c		\
			nuauprenmifuncset.c             \
			nuauprenmiproc.c		\
			nuaumgrfuncset.c		\
			nuauseqplayercontrol.c		\
			nuaumgrex.c			\
			nuaumgr.c			\
			nuaudma.c			\
			nuauconfig.c			\
			nuaumgrparams.c			\
			nuauheap.c

LIBALSGIOBJ	=	$(LIBALSGISRC:.c=.o)

LCDEFS		=	-mabi=32 -march=vr4300 -mtune=vr4300 -mfix4300 -mno-shared $(MOPTDEF) -DNU_SYSTEM  -DUSE_EPI $(MUCODE) 
LCINCS 	= 	-I.. -I$(NUSYSINCDIR) -I$(ROOT)/usr/include -I$(ROOT)/usr/include/PR 

LCOPTS		= 	


.c.o:
	$(CC) $(OPTIMIZER) $(DEFINES) $(LCDEFS) -G 0 -c $(LCINCS) $(LCOPTS) $(LDFLAGS) $<

.PATH:	..


################################################################
#	Compile 						
################################################################
default:	$(TARGETS)

all:		makedir nualsgi nualsgi_n

makedir:
	-mkdir $(ROMOBJDIR)
	-mkdir $(DEBUGOBJDIR)
	-mkdir $(NROMOBJDIR)
	-mkdir $(NDEBUGOBJDIR)

nualsgi:
	$(MAKE) -C $(ROMOBJDIR) -f ../Makefile OPTMZ=-O3 $(ROMLIB) VPATH='..'
	$(MAKE) -C $(DEBUGOBJDIR) -f ../Makefile OPTMZ=-g MOPTDEF=-DNU_DEBUG $(DEBUGLIB) VPATH='..'

nualsgi_n:
	$(MAKE) -C $(NROMOBJDIR) -f ../Makefile OPTMZ=-O3 MUCODE=-DN_AUDIO $(NROMLIB) VPATH='..'
	$(MAKE) -C $(NDEBUGOBJDIR) -f ../Makefile OPTMZ=-g MOPTDEF=-DNU_DEBUG MUCODE=-DN_AUDIO $(NDEBUGLIB) VPATH='..'

$(ROMLIB):	$(LIBALSGIOBJ) ../nualsgi.h
	$(AR) $(ARFLAGS) ../$(ROMLIB) $(LIBALSGIOBJ)

$(DEBUGLIB):	$(LIBALSGIOBJ) ../nualsgi.h
	$(AR) $(ARFLAGS) ../$(DEBUGLIB) $(LIBALSGIOBJ)

$(NROMLIB):	$(LIBALSGIOBJ) ../nualsgi.h ../nualsgi_n.h
	$(AR) $(ARFLAGS) ../$(NROMLIB) $(LIBALSGIOBJ)

$(NDEBUGLIB):	$(LIBALSGIOBJ) ../nualsgi.h ../nualsgi_n.h
	$(AR) $(ARFLAGS) ../$(NDEBUGLIB) $(LIBALSGIOBJ)

install:
	$(CP) nualsgi.h $(NUSYSINCDIR)
	$(CP) nualsgi_n.h $(NUSYSINCDIR)
	$(CP) $(ROMLIB) $(NUSYSLIBDIR)
	$(CP) $(DEBUGLIB) $(NUSYSLIBDIR)
	$(CP) $(NROMLIB) $(NUSYSLIBDIR)
	$(CP) $(NDEBUGLIB) $(NUSYSLIBDIR)

clean:
	$(RM) $(ROMOBJDIR)/*.o
	$(RM) $(DEBUGOBJDIR)/*.o
	$(RM) $(NROMOBJDIR)/*.o
	$(RM) $(NDEBUGOBJDIR)/*.o
	$(RM) *.a
