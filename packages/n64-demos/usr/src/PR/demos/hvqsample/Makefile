# 
#  N64-HVQ2 library  Sample program
#
#  FILE : Makefile
# 
#  Copyright (C) 1998,1999 NINTENDO Co.,Ltd.
# 

# 1999-02-19

include /usr/include/n64/make/PRdefs

######################################################################
#  Kind of HVQ2 decoder being used
#    0 : CPU version
#    1 : CPU+RSP version
USE_RSP = 1

#  Format of display-use frame buffer (Selected HVQ2 decoder also reflects on this)
#    1 : 16-bit
#    2 : 32-bit
CFB_FORMAT = 1

#  Maximum size of image [pixel]  (Reflects on the memory reserved for hvqwork.c)
MAXWIDTH  = 320
MAXHEIGHT = 240
######################################################################
#  Directory holding the HVQ2 library (libhvq2.a)
HVQLIBDIR = /usr/lib/n64/hvq

#  Directory holding the HVQ2 include file
HVQINCDIR = /usr/include/n64/hvq
######################################################################

NAME = sample

TARGETDIR = .

LCINCS = -I. -I$(HVQINCDIR)
LCOPTS = -O2 -G0 -g
LCDEFS = -DMAXWIDTH=$(MAXWIDTH) -DMAXHEIGHT=$(MAXHEIGHT) -DCFB_FORMAT=$(CFB_FORMAT) -DUSE_RSP=$(USE_RSP)

LDIRT  =	$(ELF) $(MAP) $(CP_LD_SCRIPT) $(ASMOBJECTS) $(ROM)

LIBS = -L$(HVQLIBDIR) -lhvq2 -L$(ROOT)/usr/lib -lultra_rom -L$(N64_LIBGCCDIR) -lgcc

ELF		= $(NAME).elf
ROM		= $(NAME).z64
MAP		= $(NAME).map

LD_SCRIPT	= $(NAME).ld
CP_LD_SCRIPT	= $(NAME)_cp.ld

SPECFILE = spec

ASMFILES    = asm/entry.s asm/rom_header.s
ASMOBJECTS  = $(ASMFILES:.s=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ	= boot.6102.o

CFILES      = system.c main.c
CODEOBJECTS = $(CFILES:.c=.o)
CODESEGMENT = codesegment.o

DATAFILES   = hvqwork.c hvqbuf.c cfb.c
DATAOBJECTS = $(DATAFILES:.c=.o)

HVQFILE	= sample.hvq
HVQ_OBJ	= samplehvq.o

default: $(ROM)

include $(COMMONRULES)

.s.o:
	$(AS) -Wa,-Iasm -o $@ $<

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(HVQ_OBJ): $(HVQFILE)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -DCFB_FORMAT=$(CFB_FORMAT) -o $@ $<

$(ROM): $(CODESEGMENT) $(ASMOBJECTS) $(DATAOBJECTS) $(BOOT_OBJ) $(HVQ_OBJ) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(ROM) -O binary
	makemask $(ROM)

$(CODESEGMENT): $(CODEOBJECTS)
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LIBS)

main.o system.o cfb.o hvqwork.o : Makefile
main.o system.o cfb.o hvqwork.o hvqbuf.o : system.h

######################################################################
