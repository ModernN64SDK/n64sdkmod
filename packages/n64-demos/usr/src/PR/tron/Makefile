#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.29 $
#        $Date: 1999/04/02 10:14:28 $
# --------------------------------------------------------------------
#
# Makefile for application test case 'tron'
#

include /usr/include/n64/make/PRdefs

OPTIMIZER       = -O2
LCDEFS          = -DNDEBUG -D_FINALROM -DF3DEX_GBI_2
N64LIB          = -lultra_rom

ELF		= tron.elf
TARGETS	= tron.z64
MAP		= tron.map

LD_SCRIPT	= tron.ld
CP_LD_SCRIPT	= tron_cp.ld

HFILES =	tron.h

ASMFILES    =	asm/entry.s asm/rom_header.s
ASMOBJECTS  =	$(ASMFILES:.s=.o)

CODEFILES =	tron.c dram_stack.c dram_yield.c
CODEOBJECTS =	$(CODEFILES:.c=.o)

DATAFILES =	dynamic.c zbuffer.c init.c cfb.c grid.c
DATAOBJECTS =	$(DATAFILES:.c=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ	= boot.6102.o

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS) $(ASMOBJECTS) $(BOOT_OBJ)

LCINCS =	-I. -I/usr/include/n64 -I/usr/include/n64/PR
LDIRT  =	$(ELF) $(CP_LD_SCRIPT) $(TARGETS) $(MAP) $(ASMOBJECTS) grid.c makegrid

LDFLAGS =	-L/usr/lib/n64 -L/usr/lib/n64/PR $(N64LIB) -L$(N64_LIBGCCDIR) -lgcc

default:	$(TARGETS)

include $(COMMONRULES)

.s.o:
	$(AS) -Wa,-Iasm -o $@ $<

$(CODESEGMENT):	$(TEXHFILES) $(CODEOBJECTS)
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)

grid.c:		makegrid
		./makegrid >grid.c

makegrid:	makegrid.c
		$(HOST_CC) -g makegrid.c -o makegrid