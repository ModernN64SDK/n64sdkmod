#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.17 $
#        $Date: 1999/04/02 10:13:00 $
# --------------------------------------------------------------------
#
# Makefile for lines demo application
#
#

include /usr/include/n64/make/PRdefs

OPTIMIZER       = -O2
LCDEFS          = -D_FINALROM -DNDEBUG -DF3DEX_GBI_2
N64LIB          = -lultra_rom

ELF		= polyline.elf
TARGETS	= polyline.z64
MAP		= polyline.map

LD_SCRIPT	= polyline.ld
CP_LD_SCRIPT	= polyline_cp.ld

ASMFILES    =	asm/entry.s asm/rom_header.s

ASMOBJECTS  =	$(ASMFILES:.s=.o)

CODEFILES   =	lines.c static.c dram_stack.c

CODEOBJECTS =	$(CODEFILES:.c=.o)

DATAFILES   =	zbuffer.c cfb.c

DATAOBJECTS =	$(DATAFILES:.c=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS) $(ASMOBJECTS) $(BOOT_OBJ)

LCINCS =	-I. -I/usr/include/n64/PR
LDFLAGS =	-L/usr/lib/n64 $(N64LIB) -L$(N64_LIBGCCDIR) -lgcc

LDIRT  =	$(ELF) $(MAP) $(CP_LD_SCRIPT) $(ASMOBJECTS) $(BOOT_OBJ) $(TARGETS) makesphere sphere.dat sphere.dl

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

static.c:	sphere.dat sphere.dl

sphere.dat sphere.dl:	makesphere
	./makesphere -t > sphere.dl
	./makesphere > sphere.dat
	touch static.c

makesphere:	makesphere.c
	$(HOST_CC) -o makesphere makesphere.c -lm
