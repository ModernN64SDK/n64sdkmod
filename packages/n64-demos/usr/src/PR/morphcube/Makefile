#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#        
#        $RCSfile: GNUmakefile,v $
#        $Revision: 1.1 $
#        $Date: 2003/04/04 17:50:34 $
# --------------------------------------------------------------------
#
# This demo has the following features
#
#	CPU vertex morphing
#	mipmap texture tiling
#
#	Bill Mark
#

include /usr/include/n64/make/PRdefs

OPTIMIZER       = -O2
LCDEFS          = -DNDEBUG -D_FINALROM -DF3DEX_GBI_2
N64LIB          = -lultra_rom

ELF		= morphcube.elf
TARGETS	= morphcube.z64
MAP		= morphcube.map

LD_SCRIPT	= morphcube.ld
CP_LD_SCRIPT	= morphcube_cp.ld

ASMFILES    =	asm/entry.s asm/rom_header.s
ASMOBJECTS  =	$(ASMFILES:.s=.o)

CODEFILES =     morphdemo.c morph.c makeverts.c dram_stack.c
CODEOBJECTS =	$(CODEFILES:.c=.o)

DATAFILES =	static.c dynamic.c zbuf.c cfb.c
DATAOBJECTS =	$(DATAFILES:.c=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ	= boot.6102.o

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS) $(ASMOBJECTS) $(BOOT_OBJ)

LCINCS =	-I. -I/usr/include/n64/PR 

LDFLAGS =	-L/usr/lib/n64 -L/usr/lib/n64/PR $(N64LIB) -L$(N64_LIBGCCDIR) -lgcc 

LDIRT  =	$(ELF) $(CP_LD_SCRIPT) $(TARGETS) $(MAP) $(ASMOBJECTS) ppm2cTex mariotex.h load.map

default:	$(TARGETS)

include $(COMMONRULES)

.s.o:
	$(AS) -Wa,-Iasm -o $@ $<

$(CODESEGMENT):	mariotex.h $(CODEOBJECTS)
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)

mariotex.h: ppm2cTex mario.ppm
	./ppm2cTex -m -s < mario.ppm > mariotex.h

ppm2cTex: ppm2cTex.c
	$(HOST_CC) -o ppm2cTex ppm2cTex.c

static.o: mariotex.h
