#!smake -k -J 4
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.2 $
#        $Date: 1999/07/13 08:34:46 $
# --------------------------------------------------------------------
#
# Makefile for sprite demo application
#
#

include /usr/include/n64/make/PRdefs

OPTIMIZER       = -O2
LCDEFS          = -DNDEBUG -D_FINALROM
N64LIB          = -lultra_rom

ELF		= sprite.elf
TARGETS	= sprite.z64
MAP		= sprite.map

LD_SCRIPT	= sprite.ld
CP_LD_SCRIPT	= sprite_cp.ld

ASMFILES    =	asm/entry.s asm/rom_header.s

ASMOBJECTS  =	$(ASMFILES:.s=.o)

CODEFILES   =   main.c dram_stack.c \
                tile_rect.c task.c rcprdp.c framebuffer.c dma.c \
                cont.c game.c

CODEOBJECTS =   $(CODEFILES:.c=.o)

DATAFILES   =   static.c cfb.c rsp_cfb.c dk.c dk7.c road.c l2_tv.c

DATAOBJECTS =   $(DATAFILES:.c=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS) $(ASMOBJECTS) $(BOOT_OBJ)

LCINCS =	-I. -I/usr/include/n64/PR
LCDEFS +=   -DF3DEX_GBI_2 -DUSE_CFB32
LCOPTS =	-G 0
LDFLAGS =	-L/usr/lib/n64 $(N64LIB) -L$(N64_LIBGCCDIR) -lgcc

LDIRT  =	$(ELF) $(CP_LD_SCRIPT) $(MAP) $(ASMOBJECTS)

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
