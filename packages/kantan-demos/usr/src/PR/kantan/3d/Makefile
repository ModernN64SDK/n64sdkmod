#
#
# Makefile for NINTENDO64 Kantan manual sample (PC-GCC)
#
#       ---- Step 2 ----
#

LIB	= /usr/lib/n64
LPR	= $(LIB)/PR
INC	= /usr/include/n64
CC	= mips-n64-gcc -mabi=32 -ffreestanding -mfix4300 -G 0
AS	= mips-n64-as -mtune=vr4300 -march=vr4300 -mabi=32 -mips3 -G 0
LD	= mips-n64-ld --no-check-sections
OBJCOPY = mips-n64-objcopy

.c.o:
	$(CC) $(OPTIMIZER) -c -I. -I$(INC) -DF3DEX_GBI_2 -DNDEBUG -D_FINALROM $<
.s.o:
	$(AS) -I. -I asm -o $@ $<

ELF		= 3d.elf
TARGETS	= 3d.z64
MAP		= 3d.map

OPTIMIZER	= -g

ASMFILES	= asm/entry.s asm/rom_header.s
ASMOBJECTS	= $(ASMFILES:.s=.o)
CODEFILES	= boot.c malloc.c gfx.c data.c expad.c exgu.c 3d.c
CODEOBJECTS	= $(CODEFILES:.c=.o)
CODESEGMENT	= codesegment.o
BOOT		= /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ	= boot.6102.o
OBJECTS	= $(ASMOBJECTS) $(BOOT_OBJ) $(CODESEGMENT)

LD_SCRIPT	= 3d.ld
CP_LD_SCRIPT	= 3d_cp.ld

LDFLAGS = -L/usr/lib/n64 -lultra_rom -L$(N64_LIBGCCDIR) -lgcc

default: $(TARGETS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CODESEGMENT): $(CODEOBJECTS)
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)

clean:
	rm -f $(OBJECTS) $(CODEOBJECTS) $(ELF) $(TARGETS) $(MAP) a.out $(CP_LD_SCRIPT)

