#! smake -f
#---------------------------------------------------------------------#
#	Copyright (C) 1997, Nintendo.
#	
#	File		Makefile
#	Coded    by	Yoshitaka Yasumoto.	Sep 12, 1997.
#	Modified by	
#	
#	$Id: Makefile,v 1.3 1997/11/26 09:10:39 mitu Exp $
#---------------------------------------------------------------------#
LIB	= /usr/lib/n64
INC	= /usr/include/n64
CC	= mips-n64-gcc -mabi=32 -ffreestanding -mfix4300 -G 0
AS	= mips-n64-as -mtune=vr4300 -march=vr4300 -mabi=32 -mips3 -G 0
LD	= mips-n64-ld --no-check-sections
OBJCOPY = mips-n64-objcopy

.c.o:
	$(CC) $(OPTIMIZER) -c -I. -I$(INC) -DNDEBUG -D_FINALROM -DF3DEX_GBI -DF3DEX_GBI_2 -o $@ $<
.s.o:
	$(AS) -I. -I asm -o $@ $<

ELF		= zonetri.elf
TARGETS	= zonetri.z64
MAP		= zonetri.map

LD_SCRIPT	= zonetri.ld
CP_LD_SCRIPT	= zonetri_cp.ld

OPTIMIZER =	-g

ASMFILES    =	asm/entry.s asm/rom_header.s
ASMOBJECTS  =	$(ASMFILES:.s=.o)
CODEFILES   =	main.c static.c system.c
CODEOBJECTS =	$(CODEFILES:.c=.o)
CODESEGMENT =	codesegment.o
DATAFILES   =	system_cfb.c system_rspbuf.c
DATAOBJECTS =	$(DATAFILES:.c=.o)
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
