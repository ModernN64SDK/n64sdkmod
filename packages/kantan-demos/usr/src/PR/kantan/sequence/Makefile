#
# Makefile for  NINTENDO64 AUDIO SAMPLE PROGRAM(sequence) (PC-GCC)
#

LIB	= /usr/lib/n64
LPR	= $(LIB)/PR
INC	= /usr/include/n64
CC	= mips-n64-gcc -mabi=32 -ffreestanding -mfix4300 -G 0
AS	= mips-n64-as -mtune=vr4300 -march=vr4300 -mabi=32 -mips3 -G 0
LD	= mips-n64-ld --no-check-sections
OBJCOPY = mips-n64-objcopy

.c.o:
	$(CC) $(OPTIMIZER) -c -I. -I$(INC) -DNDEBUG -D_FINALROM $<
.s.o:
	$(AS) -I. -I asm -o $@ $<

ELF	= sequence.elf
TARGET	= sequence.z64
MAP	= sequence.map

OPTIMIZER	= -g

ASMFILES	= asm/entry.s asm/rom_header.s

ASMOBJECTS	= $(ASMFILES:.s=.o)

CODEFILES	= audio.c \
		  boot.c \
		  dram_stack.c \
		  sequence.c \
		  main.c \
		  message.c \
		  nnsched.c

CODEOBJECTS	= $(CODEFILES:.c=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

DATAFILES	= cfb.c \
		  zbuf.c

DATAOBJECTS	= $(DATAFILES:.c=.o)

OBJECTS	= $(CODEOBJECTS) $(ASMOBJECTS) $(DATAOBJECTS) $(BOOT_OBJ)

LD_SCRIPT	= sequence.ld
CP_LD_SCRIPT	= sequence_cp.ld

default: $(TARGET)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -o $@ $<

$(TARGET): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -L$(LIB) -lultra_rom -L$(N64_LIBGCCDIR) -lgcc -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGET) -O binary
	makemask $(TARGET)

clean:
	rm -f $(OBJECTS) $(ELF) $(TARGET) $(MAP) a.out $(CP_LD_SCRIPT)

