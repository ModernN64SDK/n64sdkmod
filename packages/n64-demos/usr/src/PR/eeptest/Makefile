#DEBUG_EEPROM = YES
CC			= mips-n64-gcc -mabi=32 -ffreestanding -mfix4300 -G 0
AS			= mips-n64-as -mtune=vr4300 -march=vr4300 -mabi=32 -mips3 -G 0
LD			= mips-n64-ld --no-check-sections
OBJCOPY		= mips-n64-objcopy

OPTIMIZER :=	-O2
LCDEFS := -DNDEBUG -D_FINALROM

ifeq ($(DEBUG_EEPROM), YES)
LCDEFS +=	-DDEBUG_EEPROM
endif

ELF			= main.elf
TARGETS		= main.z64
MAP			= main.map

LD_SCRIPT	= main.ld
CP_LD_SCRIPT	= main_cp.ld

HFILES		= nu64sys.h thread.h eeptest.h graph.h

ASMFILES	= asm/entry.s asm/rom_header.s
ASMOBJECTS	= $(ASMFILES:.s=.o)

CODEFILES	= eeptest.c nu64sys.c graph.c  asci.c
CODEOBJECTS	= $(CODEFILES:.c=.o)
CODESEGMENT =	codesegment.o

DATAFILES	=  cfb1.c
DATAOBJECTS	= $(DATAFILES:.c=.o)

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
