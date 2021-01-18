#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#        
#        $RCSfile: GNUmakefile,v $
#        $Revision: 1.1 $
#        $Date: 2003/03/26 23:24:33 $
# --------------------------------------------------------------------
#
# Makefile for onetri demo application 
#
#

include /usr/include/n64/make/PRdefs

OPTIMIZER       = -O2
LCDEFS          = -DNDEBUG -D_FINALROM -DF3DEX_GBI_2
N64LIB          = -lultra_rom

ELF		= onetri.elf
TARGETS	= onetri.z64
MAP		= onetri.map

LD_SCRIPT	= onetri.ld
CP_LD_SCRIPT	= onetri_cp.ld

ASMFILES    =	asm/entry.s asm/rom_header.s

ASMOBJECTS  =	$(ASMFILES:.s=.o)

CODEFILES   =	onetri.c dram_stack.c rdp_output.c

CODEOBJECTS =	$(CODEFILES:.c=.o)

DATAFILES   =	static.c cfb.c rsp_cfb.c

DATAOBJECTS =	$(DATAFILES:.c=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS) $(ASMOBJECTS) $(BOOT_OBJ)

LCINCS =	-I.

LDFLAGS =	-L/usr/lib/n64 -lultra_rom -L$(N64_LIBGCCDIR) -lgcc

LDIRT  =	$(ELF) $(CP_LD_SCRIPT) $(TARGETS) $(MAP) $(ASMOBJECTS)

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

