#! smake
#---------------------------------------------------------------------
#  $Id: Makefile,v 1.1.1.1 2002/05/02 03:27:18 blythe Exp $
#  
#  File : Makefile
#  
#  Coded     by Yoshitaka Yasumoto.   Apr 11, 1997.
#  Copyright by Nintendo, Co., Ltd.           1997.
#---------------------------------------------------------------------
CC	= mips-n64-gcc -mabi=32 -ffreestanding -mfix4300 -G 0
AS	= mips-n64-as -mtune=vr4300 -march=vr4300 -mabi=32 -mips3 -G 0
LD	= mips-n64-ld --no-check-sections
OBJCOPY = mips-n64-objcopy

.c.o:
	$(CC) $(OPTIMIZER) -c -I. -I/usr/include/n64 -DNDEBUG -D_FINALROM -DF3DEX_GBI -o $@ $<
.s.o:
	$(AS) -I. -I asm -o $@ $<

OPTIMIZER =	-g

ELF		= minigfx.elf
TARGETS	= minigfx.z64
MAP		= minigfx.map

LD_SCRIPT	= minigfx.ld
CP_LD_SCRIPT	= minigfz_cp.ld

ASMFILES    =	asm/entry.s asm/rom_header.s
ASMOBJECTS  =	$(ASMFILES:.s=.o)
CODEFILES	=	system.c main.c static.c
CODEOBJECTS =	$(CODEFILES:.c=.o)
CODESEGMENT =	codesegment.o

DATAFILES =     system_cfb.c system_rspbuf.c
DATAOBJECTS =   $(DATAFILES:.c=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ	= boot.6102.o
OBJECTS		=	$(CODESEGMENT) $(DATAOBJECTS) $(ASMOBJECTS) $(BOOT_OBJ)

LDFLAGS 	=	-L/usr/lib/n64 -lultra_rom -L$(N64_LIBGCCDIR) -lgcc

DIRT		=	$(ELF) $(TARGETS) $(MAP) $(CP_LD_SCRIPT) $(OBJECTS)	$(CODEOBJECTS)

default:	$(TARGETS)

clean:		
	rm -rf $(DIRT)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CODESEGMENT):	$(CODEOBJECTS)
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)

#======== End of Makefile ========
