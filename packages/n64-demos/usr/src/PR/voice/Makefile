#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1998 Nintendo.
#        
#        $RCSfile: Makefile,v $
#        $Revision: 1.1.1.2 $
#        $Date: 2002/10/29 08:06:04 $
# --------------------------------------------------------------------
#
# Makefile for voice test application 
#
#

include /usr/include/n64/make/PRdefs

.NOTPARALLEL:

OPTIMIZER	= -O2
N64LIB		= -lultra_rom
LCDEFS		= -DNDEBUG -D_FINALROM

ELF			= main.elf
TARGETS		= main.z64
MAP			= main.map

LD_SCRIPT	= main.ld
CP_LD_SCRIPT	= main_cp.ld

HFILES		= nu64sys.h graph.h thread.h action.h siproc.h mes.h

ASMFILES	= asm/entry.s asm/rom_header.s
ASMOBJECTS	= $(ASMFILES:.s=.o)

CODEFILES	= main.c nu64sys.c asci.c graph.c action.c siproc.c kfont.c mes.c

CODEOBJECTS	= $(CODEFILES:.c=.o)

DATAFILES	= cfb.c

DATAOBJECTS	= $(DATAFILES:.c=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ	= boot.6102.o

CODESEGMENT	= codesegment.o

OBJECTS		= $(CODESEGMENT) $(DATAOBJECTS) $(ASMOBJECTS) $(BOOT_OBJ)

LCINCS		= -I. -I/usr/include/n64/PR -I/usr/include/n64

LDIRT		= $(ELF) $(CP_LD_SCRIPT) $(MAP) $(ASMOBJECTS) mkkanji mes.c mes.h kfont.c $(TARGETS)

LDFLAGS		= -L/usr/lib/n64 -L/usr/lib/n64/PR $(N64LIB) -L$(N64_LIBGCCDIR) -lgcc

default:	$(TARGETS)

include $(COMMONRULES)

.s.o:
	$(AS) -I. -I asm -Wa,-Iasm -o $@ $<

$(CODESEGMENT):	$(CODEOBJECTS) $(GBPAKLIB)
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -L/usr/lib/n64 -lultra_rom -L$(N64_LIBGCCDIR) -lgcc -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)

# making kanji data

main.o: main.c mes.h

mes.c mes.h kfont.c:            mes.txt message.c mkkanji
	./mkkanji mes.txt message.c
	nkf -e mes.c > mes.tmp
	mv mes.tmp mes.c

mkkanji:        mkkanji.o kanrom.o
	$(HOST_CC) -o mkkanji mkkanji.o kanrom.o

mkkanji.o:      mkkanji.c
	$(HOST_CC) -c -fextended-identifiers mkkanji.c

kanrom.o:       kanrom.c
	$(HOST_CC) -c kanrom.c

