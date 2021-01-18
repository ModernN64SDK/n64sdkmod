#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.30 $
#        $Date: 1999/04/02 10:14:23 $
# --------------------------------------------------------------------
#
# Makefile for topgun demo application
#
#
#
include /usr/include/n64/make/PRdefs

OPTIMIZER       = -O2
LCDEFS          = -D_FINALROM -DNDEBUG -DF3DEX_GBI_2
N64LIB          = -lultra_rom

ELF		= topgun.elf
TARGETS	= topgun.z64
MAP		= topgun.map

LD_SCRIPT	= topgun.ld
CP_LD_SCRIPT	= topgun_cp.ld

ASMFILES    =	asm/entry.s asm/rom_header.s

ASMOBJECTS  =	$(ASMFILES:.s=.o)

CODEFILES   =	topgun.c dram_stack.c rdp_output.c

CODEOBJECTS =	$(CODEFILES:.c=.o)

DATAFILES   =	av8b.c		\
		cfb.c		\
		ground.c	\
		static.c	\
		zbuffer.c

DATAOBJECTS =	$(DATAFILES:.c=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS) $(ASMOBJECTS) $(BOOT_OBJ)

LCINCS =	-I. -I/usr/include/n64/PR -I/usr/include/n64
LDIRT  =	$(ELF) $(CP_LD_SCRIPT) $(MAP) $(ASMOBJECTS) $(TARGETS)

LDFLAGS =	-L/usr/lib/n64 -L/usr/lib/n64/PR $(N64LIB) -L$(N64_LIBGCCDIR) -lgcc

default:	$(TARGETS)

include $(COMMONRULES)

.s.o:
	$(AS) -Wa,-Iasm -o $@ $<

$(CODESEGMENT):	$(CODEOBJECTS)
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)
