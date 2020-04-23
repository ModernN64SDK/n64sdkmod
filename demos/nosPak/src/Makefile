#!smake
# Nintendo64 nosPakMenu Library

include $(ROOT)/usr/include/make/PRdefs

LIBS =	libnos.a libnos_jpn.a

SRCFILES =	Makefile Makefile_jpn Makefile_eng canvas.c pakmenu.c fontbmp.h

LDIRT =	$(LIBS)

default:

install:
	$(INSTALL) -m 444 -F /usr/src/PR/demos/nosPak/src $(SRCFILES)

include $(COMMONRULES)


