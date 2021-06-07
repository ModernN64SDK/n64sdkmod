#
# Makefile for  NINTENDO64 GRAPHICS & AUDIO SAMPLE PROGRAM(mix) (PC-GCC)
#

LIB	= /usr/lib/n64
LPR	= $(LIB)/PR
INC	= /usr/include/n64
CC	= mips-n64-gcc -mabi=32 -ffreestanding -mfix4300 -G 0
AS	= mips-n64-gcc -x assembler-with-cpp -mabi=32 -ffreestanding -mfix4300 -G 0
LD	= mips-n64-ld --no-check-sections
OBJCOPY = mips-n64-objcopy

.c.o:
	$(CC) $(OPTIMIZER) -c -I. -I$(INC) -I$(INC)/nustd -DNDEBUG -D_FINALROM $<
.s.o:
	$(CC) -c -I. -I asm -Wa,-Iasm -Wa,-I/usr/lib/n64/PR -o $@ $<

ELF	= mix.elf
TARGETS	= mix.z64
MAP	= mix.map

OPTIMIZER	= -g

ASMFILES	= asm/entry.s asm/rom_header.s

ASMOBJECTS	= $(ASMFILES:.s=.o)

CODEFILES	= boot.c \
		  audio.c \
		  dram_stack.c \
		  mix.c \
		  mix_cg.c \
		  graphic.c \
		  gfxyield.c \
		  gfxinit.c \
		  main.c \
		  message.c \
		  nnsched.c \
		  rdp_output.c \
		  rdp_output_len.c

CODEOBJECTS	= $(CODEFILES:.c=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

DATAFILES	= zbuf.c \
		  cfb.c \
		  gfxdlistbuf.c

DATAOBJECTS	= $(DATAFILES:.c=.o)

CODESEGMENT	= codesegment.o

OBJECTS	= $(CODESEGMENT) $(ASMOBJECTS) $(DATAOBJECTS) $(BOOT_OBJ)

LD_SCRIPT	= mix.ld
CP_LD_SCRIPT	= mix_cp.ld

LDFLAGS = -L/usr/lib/n64 -lultra_rom -L$(N64_LIBGCCDIR) -lgcc

default: $(TARGETS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CODESEGMENT): $(CODEOBJECTS)
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -L$(LIB) -lultra_rom -L$(N64_LIBGCCDIR) -lgcc -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)

clean:
	rm -f $(OBJECTS) $(CODEOBJECTS) $(ELF) $(TARGETS) $(MAP) a.out $(CP_LD_SCRIPT)

