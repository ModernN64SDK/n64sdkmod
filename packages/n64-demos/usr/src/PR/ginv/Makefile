#!smake -k
#
# Makefile for u64 dev board inventory program
#
#

include $(ROOT)/usr/include/make/PRdefs

APP =		ginv.out

OPTIMIZER =	-g

TARGETS =	ginv.n64

TEXHFILES =	brick.h

HFILES =	ginv.h

CODEFILES   =	ginv.c

CODEOBJECTS =	$(CODEFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT)

LCINCS =	-I.
LCOPTS =	-G 0
LDFLAGS =	$(MKDEPOPT)  -L$(ROOT)/usr/lib -lultra_d -L$(N64_LIBGCCDIR) -lgcc

LDIRT  =	$(APP)

default:	$(TARGETS)

include $(COMMONRULES)

$(CODESEGMENT):	$(CODEOBJECTS)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(TARGETS) $(APP):	spec $(OBJECTS) 
		$(MAKEROM) -s 9 -r $(TARGETS) -e $(APP) spec
		makemask $(TARGETS)
		$(warning This demo requires a printf implementation. UNFLoader or ED64Log is recommended.)


