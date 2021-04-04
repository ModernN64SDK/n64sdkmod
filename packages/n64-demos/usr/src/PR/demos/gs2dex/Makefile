#! smake
#---------------------------------------------------------------------
#	Copyright (C) 1997, Nintendo.
#
#	File		Makefile
#	Coded    by	Yoshitaka Yasumoto.	Mar 19, 1997.
#	Modified by
#	Comments
#
#	$Id: Makefile,v 1.17 1999/04/02 10:10:55 yoshida Exp $
#---------------------------------------------------------------------

include /usr/include/n64/make/PRdefs

OPTIMIZER       = -O2
LCDEFS          = -DNDEBUG -D_FINALROM
N64LIB          = -lultra_rom

ELF		= gs2dex.elf
TARGETS	= gs2dex.z64
MAP		= gs2dex.map

LD_SCRIPT	= gs2dex.ld
CP_LD_SCRIPT	= gs2dex_cp.ld

ASMFILES    =	asm/entry.s asm/rom_header.s img_data.s
ASMOBJECTS  =	$(ASMFILES:.s=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ	= boot.6102.o

CODEFILES =	system.c main.c uc_assert.c action.c sprite.c
CODEOBJECTS =	$(CODEFILES:.c=.o)
CODESEGMENT =	codesegment.o

STATICFILES =	static.c texture.c sprite_ball.c
STATICOBJECTS =	$(STATICFILES:.c=.o)
STATICSEGMENT =	staticsegment.o

DATAFILES =	system_cfb.c system_rspbuf.c
DATAOBJECTS =	$(DATAFILES:.c=.o)

BGFILES =	L2_TV.c
BGOBJECTS =	$(BGFILES:.c=.o)

OBJECTS =	$(ASMOBJECTS) $(BOOT_OBJ) $(CODESEGMENT) $(STATICSEGMENT) $(DATAOBJECTS) $(BGOBJECTS)

LCINCS  =	-I. -I/usr/include/n64/PR
LCOPTS =	-G 0

LCDEFS  +=	-DF3DEX_GBI

LDIRT  =	$(ELF) $(CP_LD_SCRIPT) $(MAP) $(ASMOBJECTS) $(TARGETS)
		    
LDFLAGS =	-L/usr/lib/n64 $(N64LIB) -L$(N64_LIBGCCDIR) -lgcc

default:	$(TARGETS)

include $(COMMONRULES)

.s.o:
	$(AS) -Wa,-Iasm -o $@ $<

$(CODESEGMENT):	$(CODEOBJECTS)
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(STATICSEGMENT): $(STATICOBJECTS)
	$(LD) -o $(STATICSEGMENT) -r $(STATICOBJECTS) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)

