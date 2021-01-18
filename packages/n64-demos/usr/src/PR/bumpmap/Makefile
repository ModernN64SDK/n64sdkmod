#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.12 $
#        $Date: 1999/04/02 10:08:50 $
# --------------------------------------------------------------------
#
# Makefile for bumpmap demo application
#
#

include /usr/include/n64/make/PRdefs

OPTIMIZER       = -O2
LCDEFS          = -D_FINALROM -DNDEBUG -DF3DEX_GBI_2
N64LIB          = -lultra_rom

ELF		= bumpmap.elf
TARGETS	= bumpmap.z64
MAP		= bumpmap.map

LD_SCRIPT	= bumpmap.ld
CP_LD_SCRIPT	= bumpmap_cp.ld

OBJHFILES =	./obj_dls.h ./obj_texs.h ./obj_verts.h

TEXHFILES =	obj_real.h

HFILES =	$(TEXHFILES) $(OBJHFILES) bumpmap.h

ASMFILES    =	asm/entry.s asm/rom_header.s

ASMOBJECTS  =	$(ASMFILES:.s=.o)

CODEFILES   =	bumpmap.c dram_stack.c

CODEOBJECTS =	$(CODEFILES:.c=.o)

DATAFILES   =	static.c cfb.c rsp_cfb.c zbuf.c

DATAOBJECTS =	$(DATAFILES:.c=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS) $(ASMOBJECTS) $(BOOT_OBJ)

LCINCS =	-I.
LCOPTS =	-G 0
LDFLAGS =	-L/usr/lib/n64 -L/usr/lib/n64/PR $(N64LIB) -L$(N64_LIBGCCDIR) -lgcc 

LDIRT  =	$(ELF) $(CP_LD_SCRIPT) $(MAP) $(ASMOBJECTS) $(TARGETS)

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
