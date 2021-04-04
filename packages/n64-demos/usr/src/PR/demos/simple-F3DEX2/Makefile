#!smake
# --------------------------------------------------------------------
#        Copyright (C) 1998 Nintendo. (Originated by SGI)
#        
#        $RCSfile: Makefile,v $
#        $Revision: 1.1.1.1 $
#        $Date: 2002/05/02 03:27:21 $
# --------------------------------------------------------------------
include /usr/include/n64/make/PRdefs

OPTIMIZER		:= -O2
LCDEFS			:= -DNDEBUG -D_FINALROM
N64LIB			:= -lultra_rom


ELF		= simple.elf
TARGETS	= simple.z64
MAP		= simple.map

LD_SCRIPT	= simple.ld
CP_LD_SCRIPT	= simple_cp.ld

ASMFILES    =	asm/entry.s asm/rom_header.s sound_data.s

ASMOBJECTS  =	$(ASMFILES:.s=.o)

CODEFILES = audio.c audiomgr.c controller.c gfx.c gfxstack.c gfxyield.c gfxrdpfifo.c misc.c simple.c

CODEOBJECTS =	$(CODEFILES:.c=.o)

CODESEGMENT =	codesegment.o

DATAFILES = gfxdynamic.c gfxdlists.c gfxzbuffer.c gfxinit.c gfxstatic.c gfxcfb.c

DATAOBJECTS =	$(DATAFILES:.c=.o)

BOOT		=	/usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ	=	boot.6102.o

OBJECTS		=	$(CODESEGMENT) $(DATAOBJECTS) $(ASMOBJECTS) $(BOOT_OBJ)

LCINCS =	-I/usr/include/n64/PR 
LCDEFS +=	-DF3DEX_GBI_2
#LCDEFS +=	-DF3DEX_GBI_2 -DFOG
#LCDEFS +=	-DF3DEX_GBI_2 -DFOG -DXBUS
#LCDEFS +=	-DF3DEX_GBI_2 -DFOG -DXBUS -DSTOP_AUDIO

LDIRT  =	$(ELF) $(CP_LD_SCRIPT) $(TARGETS) $(MAP) $(ASMOBJECTS)

LDFLAGS =	-L/usr/lib/n64 $(N64LIB)  -L$(N64_LIBGCCDIR) -lgcc

default:	$(TARGETS)

include $(COMMONRULES)

.s.o:
	$(AS) -Wa,-Iasm -o $@ $<

$(CODESEGMENT):	$(CODEOBJECTS)
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs $(LCDEFS) -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)
