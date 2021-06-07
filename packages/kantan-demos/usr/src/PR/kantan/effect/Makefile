#
# Makefile for  NINTENDO64 AUDIO SAMPLE PROGRAM(effect) (PC-GCC)
#

LIB	= /usr/lib/n64
LPR	= $(LIB)/PR
INC	= /usr/include/n64
CC	= mips-n64-gcc -mabi=32 -ffreestanding -mfix4300 -G 0
AS	= mips-n64-as -mtune=vr4300 -march=vr4300 -mabi=32 -mips3 -G 0
LD	= mips-n64-ld --no-check-sections
OBJCOPY = mips-n64-objcopy

.c.o:
	$(CC) $(OPTIMIZER) -c -I. -I$(INC) -I$(INC)/nustd -DNDEBUG -D_FINALROM $<
.s.o:
	$(AS) -I. -I asm -o $@ $<

ELF		= effect.elf
TARGETS	= effect.z64
MAP		= effect.map

OPTIMIZER	= -g

ASMFILES	= asm/entry.s asm/rom_header.s sound_data.s

ASMOBJECTS	= $(ASMFILES:.s=.o)

CODEFILES	= boot.c \
		  audio.c \
		  dram_stack.c \
		  effect.c \
		  main.c \
		  message.c \
		  nnsched.c

CODEOBJECTS	= $(CODEFILES:.c=.o)

CODESEGMENT	= codesegment.o

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

OBJECTS	= $(ASMOBJECTS) $(CODESEGMENT) $(BOOT_OBJ)

LD_SCRIPT	= effect.ld
CP_LD_SCRIPT	= effect_cp.ld

LDFLAGS = -L/usr/lib/n64 -lultra_rom -lnustd -L$(N64_LIBGCCDIR) -lgcc

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

