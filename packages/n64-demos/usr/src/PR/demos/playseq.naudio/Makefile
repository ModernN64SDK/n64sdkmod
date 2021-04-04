#!smake
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#
#        $RCSfile: Makefile,v $
#        $Revision: 1.14 $
#        $Date: 1999/07/13 05:38:45 $
# --------------------------------------------------------------------

include /usr/include/n64/make/PRdefs

OPTIMIZER       = -O2
LCDEFS          = -DNDEBUG -D_FINALROM

ELF		= playseq.elf
TARGETS	= playseq.z64
MAP		= playseq.map

LD_SCRIPT	= playseq.ld
CP_LD_SCRIPT	= playseq_cp.ld

HFILES =	playseq.h

ASMFILES    =	asm/entry.s asm/rom_header.s sound_data.s

ASMOBJECTS  =	$(ASMFILES:.s=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

CODEFILES =	playseq.c
CODEOBJECTS =	$(CODEFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS =	$(ASMOBJECTS) $(BOOT_OBJ) $(CODESEGMENT)

LCINCS =	-I.\
		-I/usr/n64/include\
		-I/usr/n64/include/PR
LCOPTS =	-G 0
LDFLAGS =	-L/usr/lib/n64 -lultra_rom -ln_audio -L$(N64_LIBGCCDIR) -lgcc -u alSeqSetLoc -u __alCSeqNextDelta

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
