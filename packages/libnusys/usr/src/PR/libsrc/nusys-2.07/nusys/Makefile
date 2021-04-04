#
# 	NuSystem Makefile for NINTENDO64
#
#	Copyright (C) 1997-1999 NINTENDO Co.,Ltd.
#

NUSYSINCDIR  = /usr/include/n64/nusys
NUSYSLIBDIR  = /usr/lib/n64/nusys

include /usr/include/n64/make/PRdefs

LIB = /usr/lib/n64
LPR = $(LIB)/PR
INC = /usr/include/n64
MAKE		=	make
RM		=	rm -f
CP		=	cp
AR		=	mips-n64-ar
ARFLAGS	=	cr
RANLIB		=	mips-n64-ranlib

.c.o:
	$(CC) $(OPTIMIZER) $(MOPTDEF) -DUSE_EPI -G 0 -c -mabi=32 -march=vr4300 -mtune=vr4300 -mfix4300 -mno-shared -I.. -I$(INC)/PR -I$(INC) $<

#
# NUSYS LIBRARY
#
#

NUBOOTSRC	=	nuboot.c
NUBOOTROMSRC	=	nuboot_rom.c
NUBOOTPTNSRC	=	nuboot_ptn.c
NUBOOTISVSRC	=	nuboot_isv.c
NUBOOTEMUSRC	=	nuboot_emu.c
NUBOOTMSPSRC	=	nuboot_msp.c

NUBOOTOBJ	=	$(NUBOOTSRC:.c=.o)
NUBOOTROMOBJ	=	$(NUBOOTROMSRC:.c=.o)
NUBOOTPTNOBJ	=	$(NUBOOTPTNSRC:.c=.o)
NUBOOTISVOBJ	=	$(NUBOOTISVSRC:.c=.o)
NUBOOTEMUOBJ	=	$(NUBOOTEMUSRC:.c=.o)
NUBOOTMSPOBJ	=	$(NUBOOTMSPSRC:.c=.o)

NUBOOTLIB	= 	nusys.o
NUBOOTROMLIB	= 	nusys_rom.o
NUBOOTPTNLIB	= 	nusys_ptn.o
NUBOOTISVLIB	= 	nusys_isv.o
NUBOOTEMULIB	= 	nusys_emu.o
NUBOOTMSPLIB	= 	nusys_msp.o

NUBOOTDEBLIB	= 	nusys_d.o
NUBOOTPTNDEBLIB	= 	nusys_ptn_d.o
NUBOOTISVDEBLIB	= 	nusys_isv_d.o
NUBOOTEMUDEBLIB	= 	nusys_emu_d.o
NUBOOTMSPDEBLIB	= 	nusys_msp_d.o

NUBOOTOBJS	=	$(NUBOOTLIB) \
			$(NUBOOTROMLIB) \
			$(NUBOOTPTNLIB) \
			$(NUBOOTISVLIB) \
			$(NUBOOTEMULIB) \
			$(NUBOOTMSPLIB)

NUBOOTDEBOBJS	=	$(NUBOOTDEBLIB) \
			$(NUBOOTPTNDEBLIB) \
			$(NUBOOTISVDEBLIB) \
			$(NUBOOTEMUDEBLIB) \
			$(NUBOOTMSPDEBLIB)

ROMLIB		=	libnusys.a
DEBUGLIB	=	libnusys_d.a
ROMOBJDIR       =       libnusys
DEBUGOBJDIR     =       libnusys_d

HEADER          =       nusys.h

LIBSYSSRC	= 	nusched.c 			\
			nuprenmifuncset.c		\
			nurdpoutput.c 			\
			nuyieldbuf.c 			\
			nudramstack.c 			\
			nuidlefuncset.c 		\
			nugfxthread.c 			\
			nugfxtaskmgr.c 			\
			nugfxinit.c  			\
			nugfxfuncset.c 			\
			nugfxprenmifuncset.c 		\
			nugfxtaskendfuncset.c 		\
			nugfxswapcfbfuncset.c		\
			nugfxswapcfb.c			\
			nugfxtaskallendwait.c		\
			nugfxretracewait.c		\
			nugfxsetcfb.c			\
			nugfxdisplayoff.c 		\
			nugfxdisplayon.c 		\
			nugfxsetucodefifo.c		\
			nudebtaskperfbar0.c		\
			nudebtaskperfbar1.c		\
			nudebload.c			\
			nudebconsole.c			\
			nudebconputc.c			\
			nudebconcputs.c			\
			nudebconwindowset.c		\
			nudebconwindowpos.c		\
			nudebconwindowsize.c		\
			nudebconwindowshow.c		\
			nudebcontextcolor.c		\
			nudebcontextattr.c		\
			nudebcontextpos.c		\
			nudebconscroll.c		\
			nudebconclear.c			\
			nudebconprintf.c		\
			nusimgr.c			\
			nusicallbackadd.c		\
			nusicallbackremove.c		\
			nusisendmesgnw.c		\
			nupiinit.c			\
			nupireadrom.c			\
			nupiinitsram.c			\
			nupiinitddrom.c			\
			nupireadwritesram.c		\
			nudebconbuf.c			\
			nudebcondisp.c			\
			nudebcondisp_ex2.c		\
			nugfxinit_ex2.c			\
			nudebtaskperfbar0_ex2.c		\
			nudebtaskperfbar1_ex2.c		\
			nucontmgr.c 			\
			nucontinit.c 			\
			nucontdatalock.c		\
			nucontdataread.c		\
			nucontdatareadwait.c 		\
			nucontdatareadstart.c 		\
			nucontdataget.c			\
			nucontdatagetall.c		\
			nucontdatagetex.c		\
			nucontdatagetexall.c		\
			nucontqueryread.c		\
			nucontreadfuncset.c		\
			nucontpakmgr.c			\
			nucontpakjiston64.c		\
			nucontpakopen.c			\
			nucontpakgetfree.c		\
			nucontpakrepairid.c		\
			nucontpakfileopen.c		\
			nucontpakfileopenjis.c		\
			nucontpakcodeset.c		\
			nucontpakfilereadwrite.c	\
			nucontpakfiledelete.c		\
			nucontpakfiledeletejis.c	\
			nucontpakfilestate.c		\
			nucontpakfilenum.c		\
			nucontrmbmgr.c			\
			nucontrmbstart.c		\
			nucontrmbstop.c			\
			nucontrmbcheck.c		\
			nucontrmbmodeset.c		\
			nucontrmbforcestop.c		\
			nucontrmbforcestopend.c		\
			nucontrmbsearchtimeset.c	\
			nueeprommgr.c			\
			nucontgbpakmgr.c		\
			nucontgbpakopen.c		\
			nucontgbpakgetstatus.c		\
			nucontgbpakpower.c		\
			nucontgbpakreadid.c		\
			nucontgbpakreadwrite.c		\
			nucontgbpakcheck.c		\
			nucontgbpakfread.c		\
			nucontgbpakfwrite.c		\
			nucontgbpakregwrite.c		\
			nucontpakfilefread.c		\
			nucontpakfilefwrite.c		\
			nuvrsmgr.c			\
			nuvrsopen.c			\
			nuvrscleardictionary.c		\
			nuvrssetword.c			\
			nuvrsmaskdictionary.c		\
			nuvrsstartreaddata.c		\
			nuvrsgetreaddata.c		\
			nuvrsstopreaddata.c		\
			nuvrscontrolgain.c		\
			numark.c			\
			nudebperfmarkset.c		\
			nudebtaskperfintervalset.c	\
			nufont.c			\
			nupireadromoverlay.c


LIBSYSOBJ	=	$(LIBSYSSRC:.c=.o)

VPATH           = ..

#
# compile
#
default:
	-mkdir $(ROMOBJDIR)
	$(MAKE) -C $(ROMOBJDIR) -f ../Makefile OPTIMIZER=-O3 $(ROMLIB)
	-mkdir $(DEBUGOBJDIR)
	$(MAKE) -C $(DEBUGOBJDIR) -f ../Makefile OPTIMIZER=-g MOPTDEF=-DNU_DEBUG $(DEBUGLIB)


$(ROMLIB): $(LIBSYSOBJ) $(NUBOOTOBJS) ../nusys.h
	$(AR) $(ARFLAGS) $(ROMLIB) $(LIBSYSOBJ)
	$(RANLIB) $(ROMLIB)

$(DEBUGLIB):    $(LIBSYSOBJ) $(NUBOOTDEBOBJS) ../nusys.h
	$(AR) $(ARFLAGS) $(DEBUGLIB) $(LIBSYSOBJ)
	$(RANLIB) $(DEBUGLIB)

#
# boot code
#
#nusys_boot:	$(NUBOOTOBJS)
#nusys_boot_d:	$(NUBOOTDEBOBJS)

$(NUBOOTLIB):	$(NUBOOTOBJ) nusys.h
	$(LD) -o $(NUBOOTLIB) -r $(NUBOOTOBJ)
$(NUBOOTROMLIB):	$(NUBOOTOBJ) nusys.h
	$(LD) -o $(NUBOOTROMLIB) -r $(NUBOOTOBJ)
$(NUBOOTPTNLIB):	$(NUBOOTROMOBJ) nusys.h
	$(LD) -o $(NUBOOTPTNLIB) -r $(NUBOOTROMOBJ)
$(NUBOOTISVLIB):	$(NUBOOTISVOBJ) nusys.h
	$(LD) -o $(NUBOOTISVLIB) -r $(NUBOOTISVOBJ)
$(NUBOOTEMULIB):	$(NUBOOTEMUOBJ) nusys.h
	$(LD) -o $(NUBOOTEMULIB) -r $(NUBOOTEMUOBJ)
$(NUBOOTMSPLIB):	$(NUBOOTMSPOBJ) nusys.h
	$(LD) -o $(NUBOOTMSPLIB) -r $(NUBOOTMSPOBJ)
$(NUBOOTDEBLIB):	$(NUBOOTOBJ) nusys.h
	$(LD) -o $(NUBOOTDEBLIB) -r $(NUBOOTOBJ)
$(NUBOOTPTNDEBLIB):	$(NUBOOTPTNOBJ) nusys.h
	$(LD) -o $(NUBOOTPTNDEBLIB) -r $(NUBOOTPTNOBJ)
$(NUBOOTISVDEBLIB):	$(NUBOOTISVOBJ) nusys.h
	$(LD) -o $(NUBOOTISVDEBLIB) -r $(NUBOOTISVOBJ)
$(NUBOOTEMUDEBLIB):	$(NUBOOTEMUOBJ) nusys.h
	$(LD) -o $(NUBOOTEMUDEBLIB) -r $(NUBOOTEMUOBJ)
$(NUBOOTMSPDEBLIB):	$(NUBOOTMSPOBJ) nusys.h
	$(LD) -o $(NUBOOTMSPDEBLIB) -r $(NUBOOTMSPOBJ)

$(NUBOOTOBJ):	../$(NUBOOTSRC) ../nusys.h
$(NUBOOTROMOBJ):	../$(NUBOOTROMSRC) ../nusys.h
$(NUBOOTPTNOBJ):	../$(NUBOOTPTNSRC) ../nusys.h
$(NUBOOTISVOBJ):	../$(NUBOOTISVSRC) ../nusys.h
$(NUBOOTEMUOBJ):	../$(NUBOOTEMUSRC) ../nusys.h
$(NUBOOTMSPOBJ):	../$(NUBOOTMSPSRC) ../nusys.h

install:
	$(CP) $(HEADER) $(NUSYSINCDIR)
	$(CP) $(ROMOBJDIR)\$(ROMLIB) $(NUSYSLIBDIR)
	$(CP) $(ROMOBJDIR)\$(NUBOOTLIB) $(NUSYSLIBDIR)
	$(CP) $(ROMOBJDIR)\$(NUBOOTROMLIB) $(NUSYSLIBDIR)
	$(CP) $(ROMOBJDIR)\$(NUBOOTPTNLIB) $(NUSYSLIBDIR)
	$(CP) $(ROMOBJDIR)\$(NUBOOTISVLIB) $(NUSYSLIBDIR)
	$(CP) $(ROMOBJDIR)\$(NUBOOTEMULIB) $(NUSYSLIBDIR)
	$(CP) $(ROMOBJDIR)\$(NUBOOTMSPLIB) $(NUSYSLIBDIR)
	$(CP) $(DEBUGOBJDIR)\$(DEBUGLIB) $(NUSYSLIBDIR)
	$(CP) $(DEBUGOBJDIR)\$(NUBOOTDEBLIB) $(NUSYSLIBDIR)
	$(CP) $(DEBUGOBJDIR)\$(NUBOOTPTNDEBLIB) $(NUSYSLIBDIR)
	$(CP) $(DEBUGOBJDIR)\$(NUBOOTISVDEBLIB) $(NUSYSLIBDIR)
	$(CP) $(DEBUGOBJDIR)\$(NUBOOTEMUDEBLIB) $(NUSYSLIBDIR)
	$(CP) $(DEBUGOBJDIR)\$(NUBOOTMSPDEBLIB) $(NUSYSLIBDIR)

clean:
	$(RM) $(ROMOBJDIR)/*.o
	$(RM) $(ROMOBJDIR)/*.a
	$(RM) $(DEBUGOBJDIR)/*.o
	$(RM) $(DEBUGOBJDIR)/*.a
