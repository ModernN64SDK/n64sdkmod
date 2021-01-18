#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1998 Nintendo.
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.11 $
#        $Date: 1999/07/13 08:33:27 $
# --------------------------------------------------------------------
#
# Makefile for gbpak test application
#
#

include /usr/include/n64/make/PRdefs

#LANG_ENGLISH = YES
OPTIMIZER	= -O2
N64LIB		= -lultra_rom
LCDEFS		= -DNDEBUG -D_FINALROM

ELF			= main.elf
TARGETS		= main.z64
MAP			= main.map

LD_SCRIPT	= main.ld
CP_LD_SCRIPT	= main_cp.ld

HFILES		= main.h nu64sys.h siproc.h graph.h thread.h mes_e.h

ASMFILES	= asm/entry.s asm/rom_header.s
ASMOBJECTS	= $(ASMFILES:.s=.o)

ifeq ($(LANG_ENGLISH), YES)
CODEFILES	= main.c siproc.c gbpaksramreadwrite.c \
		  nu64sys.c asci.c graph.c
LCDEFS		+= -DLANG_ENGLISH
else
CODEFILES	= main.c siproc.c gbpaksramreadwrite.c \
		  nu64sys.c asci.c graph.c \
		  putkanji.c printkanji.c

KCODEFILES 	= kfont.c mes.c
endif

CODEOBJECTS	= $(CODEFILES:.c=.o) $(KCODEFILES:.c=.o)

DATAFILES	= cfb.c
DATAOBJECTS	= $(DATAFILES:.c=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ	= boot.6102.o

CODESEGMENT	= codesegment.o

OBJECTS		= $(CODESEGMENT) $(DATAOBJECTS) $(ASMOBJECTS) $(BOOT_OBJ)

LCINCS		= -I. -I/usr/include/n64/PR -I/usr/include/n64

LCOPTS =	-G 0

LDIRT		= $(ELF) $(CP_LD_SCRIPT) $(MAP) $(ASMOBJECTS) $(TARGETS)

LDFLAGS 	=	-L/usr/lib/n64 -lultra_rom -L$(N64_LIBGCCDIR) -lgcc

default:	$(TARGETS)

include $(COMMONRULES)

.s.o:
	$(AS) -I. -Wa,-Iasm -o $@ $<

$(CODESEGMENT):	$(CODEOBJECTS)
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -L/usr/lib/n64 -lultra_rom -L$(N64_LIBGCCDIR) -lgcc -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)