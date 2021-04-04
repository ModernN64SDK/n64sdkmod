#
#NuSYSTEM Sample Program "SNAKE TAIL HACK"
#
#Makefile
#
#Copyright (C) 1997, NINTENDO Co,Ltd.

include /usr/include/n64/make/PRdefs

# The directory which has the include file and library of NuSYSTEM
#
NUSYSINCDIR  = /usr/include/n64/nusys
NUSYSLIBDIR  = /usr/lib/n64/nusys


ELF		= nusnake.elf
TARGETS	= nusnake.z64
MAP		= nusnake.map
LD_SCRIPT	= nusnake.ld
CP_LD_SCRIPT	= nusnake_cp.ld

OPTIMIZER =	-g

ASMFILES	= asm/entry.s asm/rom_header.s sound_data.s

ASMOBJECTS	= $(ASMFILES:.s=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

CODEFILES   = 	font.c \
		gamemain.c \
		gametitle.c \
		gameover.c \
		gamepakerr.c \
		gamepakrwerr.c \
		gfxinit.c \
		graphic.c \
		hitchk.c \
		main.c \
		map.c \
		math.c \
		view.c

CODEOBJECTS =	$(CODEFILES:.c=.o)  $(NUSYSLIBDIR)/nusys.o

DATAFILES   =	movieimg.c

DATAOBJECTS =	$(DATAFILES:.c=.o)

CODESEGMENT =	codesegment.o

OBJECTS		=	$(ASMOBJECTS) $(BOOT_OBJ) $(CODESEGMENT) $(DATAOBJECTS)

LCDEFS =	-DNDEBUG -DF3DEX_GBI_2
LCINCS =	-I.  -I$(NUSYSINCDIR)
LCOPTS =	-G 0
LDFLAGS =	$(MKDEPOPT) -L/usr/lib/n64 -L$(NUSYSLIBDIR) -lnualsgi_n -lnusys  -ln_audio -lultra_rom  -L$(N64_LIBGCCDIR) -lgcc

LDIRT  =  $(ELF) $(ASMOBJECTS) $(CP_LD_SCRIPT) $(MAP) $(TARGETS)

default:	$(TARGETS)

include $(COMMONRULES)

.s.o:
	$(AS) -I. -I asm -Wa,-Iasm -o $@ $<

$(CODESEGMENT):	$(CODEOBJECTS) Makefile
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -I$(NUSYSINCDIR) -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)
