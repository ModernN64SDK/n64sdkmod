#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.15 $
#        $Date: 1999/04/02 08:26:20 $
# --------------------------------------------------------------------
#
# Makefile for performance demo application
#
#

include /usr/include/n64/make/PRdefs

OPTIMIZER       = -O2
LCDEFS          = -DNDEBUG -D_FINALROM -DF3DEX_GBI_2
N64LIB          = -lultra_rom

ELF		= fill.elf
TARGETS	= fill.z64
MAP		= fill.map

LD_SCRIPT	= fill.ld
CP_LD_SCRIPT	= fill_cp.ld

ASMFILES    =	asm/entry.s asm/rom_header.s

ASMOBJECTS  =	$(ASMFILES:.s=.o)

CODEFILES   =	fill.c static.c tris.c controller.c timer.c rdp_output.c \
		dram_stack.c

CODEOBJECTS =	$(CODEFILES:.c=.o)

DATAFILES   =	zbuffer.c cfb.c cfb_b.c

DATAOBJECTS =	$(DATAFILES:.c=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS) $(ASMOBJECTS) $(BOOT_OBJ)

LCDEFS +=	$(HW_FLAGS)
LCINCS =	-I. -I/usr/include/n64/PR
LCOPTS =	-G 0
LDFLAGS =	-L/usr/lib/n64 $(N64LIB)  -L$(N64_LIBGCCDIR) -lgcc

LDIRT  =	$(ELF) $(MAP) $(CP_LD_SCRIPT) $(ASMOBJECTS) $(TARGETS)

default:	$(TARGETS)

include $(COMMONRULES)

.s.o:
	$(AS) -Wa,-Iasm -o $@ $<

$(CODESEGMENT):	$(CODEOBJECTS)
	@printf "Warning!\nThis demo fills the screen with brightly colored triangles at a fast rate,\nwhich could cause issues for those with epilepsy.\nPlease use caution before running this demo.\n"
	@printf "SDK maintainer is not a medical professional, and assumes no responsibility.\nRun at your own risk.\n"
	@echo; read -rsn1 -p "Press any key to continue . . ."; echo
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)
