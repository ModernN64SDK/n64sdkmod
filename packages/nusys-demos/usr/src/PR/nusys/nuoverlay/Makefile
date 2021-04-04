# 	NuSYSTEM samples nuoverlay Makefile
#       Copyright (C) 1997-1999, NINTENDO Co,Ltd.

include /usr/include/n64/make/PRdefs

# The directory which has the include file and library of NuSYSTEM
#
NUSYSINCDIR  = /usr/include/n64/nusys
NUSYSLIBDIR  = /usr/lib/n64/nusys

LIB = /usr/lib/n64
LPR = $(LIB)/PR
INC = /usr/include/n64

NUAUDIOLIB = -lnualsgi_n -ln_audio

LCDEFS =	-DNDEBUG -DF3DEX_GBI_2
LDIRT  =  $(ELF) $(ASMOBJECTS) $(CP_LD_SCRIPT) $(MAP) $(TARGETS)
LCINCS =	-I. -I$(NUSYSINCDIR) -I/usr/include/n64/PR
LCOPTS =	-G 0
LDFLAGS =	$(MKDEPOPT) -L$(LIB) -L$(NUSYSLIBDIR) $(NUAUDIOLIB) -lnusys -lultra_rom -L$(N64_LIBGCCDIR) -lgcc

OPTIMIZER =	-g

ELF		= nuoverlay.elf
TARGETS	= nuoverlay.z64
MAP		= nuoverlay.map
LD_SCRIPT	= nuoverlay.ld
CP_LD_SCRIPT	= nuoverlay_cp.ld

HFILES =	main.h graphic.h segment.h

ASMFILES	= asm/entry.s asm/rom_header.s sound_data.s

ASMOBJECTS	= $(ASMFILES:.s=.o)

CODEFILES   = 	main.c graphic.c gfxinit.c

CODEOBJECTS =	$(CODEFILES:.c=.o)  $(NUSYSLIBDIR)/nusys.o

STAGE00FILES	= stage00.c

STAGE00OBJECTS	= $(STAGE00FILES:.c=.o)

STAGE01FILES	= stage01.c

STAGE01OBJECTS	= $(STAGE01FILES:.c=.o)

DATAFILES   =	

DATAOBJECTS =	$(DATAFILES:.c=.o)

CODESEGMENT =	codesegment.o

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS) $(ASMOBJECTS) $(BOOT_OBJ) 

FORCELINK =	-u guOrtho \
		-u guRotate \
		-u guTranslate \
		-u nuContDataGetEx \
		-u nuDebConCPuts \
		-u nuDebConDispEX2 \
		-u nuDebConTextPos \
		-u nuDebTaskPerfBar0EX2 \
		-u sprintf

default:        $(TARGETS)

include $(COMMONRULES)

.s.o:
	$(AS) -I. -I asm -Wa,-Iasm -o $@ $<

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -I$(NUSYSINCDIR) -o $@ $<

$(CODESEGMENT):	$(CODEOBJECTS) Makefile
	$(LD) $(FORCELINK) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(TARGETS): $(OBJECTS) $(STAGE00OBJECTS) $(STAGE01OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)

