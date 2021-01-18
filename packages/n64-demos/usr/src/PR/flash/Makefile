# Makefile for 1M flash test application 
#
#
CC			= mips-n64-gcc -mabi=32 -ffreestanding -mfix4300 -G 0
AS			= mips-n64-as -mtune=vr4300 -march=vr4300 -mabi=32 -mips3 -G 0
LD			= mips-n64-ld --no-check-sections
OBJCOPY		= mips-n64-objcopy

OPTIMIZER       = -O2
LCDEFS          = -DNDEBUG -D_FINALROM -DF3DEX_GBI_2
N64LIB          = -lultra_rom

ELF			= flash.elf
TARGETS		= flash.z64
MAP			= flash.map

LD_SCRIPT	= flash.ld
CP_LD_SCRIPT	= flash_cp.ld

HFILES =	buffer.h display.h graph.h nu64sys.h thread.h

ASMFILES	= asm/entry.s asm/rom_header.s
ASMOBJECTS	= $(ASMFILES:.s=.o)

CODEFILES   =	nu64sys.c main.c graph.c asci.c
CODEOBJECTS =	$(CODEFILES:.c=.o)

DATAFILES   =	cfb.c buffer.c
DATAOBJECTS =	$(DATAFILES:.c=.o)

CODESEGMENT =	codesegment.o

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ	= boot.6102.o

OBJECTS		= $(CODESEGMENT) $(DATAOBJECTS) $(ASMOBJECTS) $(BOOT_OBJ)

LCINCS		= -I. -I/usr/include/n64/PR -I/usr/include/n64

DIRT		=	$(ELF) $(CP_LD_SCRIPT) $(TARGETS) $(MAP) $(OBJECTS) $(CODEOBJECTS)

LDFLAGS 	=	-L/usr/lib/n64 -lultra_rom -L$(N64_LIBGCCDIR) -lgcc

default:	$(TARGETS)

clean:
	rm -f $(DIRT)

.c.o:
	$(CC) $(OPTIMIZER) -c $(LCINCS) -DF3DEX_GBI_2 $(LCDEFS) $<
.s.o:
	$(AS) -I. -I asm -o $@ $<

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CODESEGMENT):	$(CODEOBJECTS)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -L/usr/lib/n64 -lultra_rom -L$(N64_LIBGCCDIR) -lgcc -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)
