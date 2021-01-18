#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.13 $
#        $Date: 1999/04/02 10:13:36 $
# --------------------------------------------------------------------
#
# Makefile for spgame demo application
#
#

include /usr/include/n64/make/PRdefs

OPTIMIZER       = -O2
LCDEFS          = -DNDEBUG -D_FINALROM -DF3DEX_GBI_2
N64LIB          = -lultra_rom

ELF		= spgame.elf
TARGETS	= spgame.z64
MAP		= spgame.map

LD_SCRIPT	= spgame.ld
CP_LD_SCRIPT	= spgame_cp.ld

ASMFILES    =	asm/entry.s asm/rom_header.s

ASMOBJECTS  =	$(ASMFILES:.s=.o)

CODEFILES   =	main.c spgame.c font.c pics.c dram_stack.c

CODEOBJECTS =	$(CODEFILES:.c=.o)

DATAFILES   =	static.c cfb.c rsp_cfb.c

DATAOBJECTS =	$(DATAFILES:.c=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS) $(ASMOBJECTS) $(BOOT_OBJ)

LCINCS =	-I.
LDFLAGS =	-L/usr/lib/n64 $(N64LIB) -L$(N64_LIBGCCDIR) -lgcc

LDIRT  =	$(ELF) $(MAP) $(CP_LD_SCRIPT) $(ASMOBJECTS) $(BOOT_OBJ) $(TARGETS)

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
