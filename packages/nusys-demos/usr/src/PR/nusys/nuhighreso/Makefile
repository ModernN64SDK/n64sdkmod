#
# 	NuSystem highreso Program 
#
#	Copyright (C) 1997, NINTENDO Co,Ltd.
#

include /usr/include/n64/make/PRdefs

LIB = /usr/lib/n64
LPR = $(LIB)/PR
INC = /usr/include/n64

#
# NUSYS LIBRARY
#
#
NUSYSINCDIR  = /usr/include/n64/nusys
NUSYSLIBDIR  = /usr/lib/n64/nusys

ELF				= highreso.elf
TARGETS			= highreso.z64
MAP				= highreso.map
LD_SCRIPT		= highreso.ld
CP_LD_SCRIPT	= highreso_cp.ld

OPTIMIZER		=	-g

ASMFILES	= asm/entry.s asm/rom_header.s imgdata.s

ASMOBJECTS	= $(ASMFILES:.s=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

CODEFILES   	= 	main.c graphics.c

CODEOBJECTS		=	$(CODEFILES:.c=.o) $(NUSYSLIBDIR)/nusys.o

DATAFILES   	=

DATAOBJECTS		=	$(DATAFILES:.c=.o)

CODESEGMENT		=	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS) $(ASMOBJECTS) $(BOOT_OBJ)


LCDEFS	= 
LCINCS =	-I. -I$(NUSYSINCDIR)
LCOPTS =	-G 0
LDFLAGS = $(MKDEPOPT) -L$(LIB) -L$(NUSYSLIBDIR) -lnusys_d -lultra_rom -L$(N64_LIBGCCDIR) -lgcc

LDIRT  =  $(ELF) $(ASMOBJECTS) $(CP_LD_SCRIPT) $(MAP) $(TARGETS)

default:	$(TARGETS)

include $(COMMONRULES)

.s.o:
	$(AS) -I. -I asm -Wa,-Iasm -o $@ $<

$(CODESEGMENT):	$(CODEOBJECTS) Makefile
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -I$(NUSYSINCDIR) -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)
