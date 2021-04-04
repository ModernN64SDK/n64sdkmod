# use naudio
NAUDIO := 0

# command names #
LIB		= /usr/lib/n64
LPR		= $(LIB)/PR
INC		= /usr/include/n64
RM		= rm -f
CC		= mips-n64-gcc -mabi=32 -ffreestanding -mfix4300 -G 0
AS		= mips-n64-as -mtune=vr4300 -march=vr4300 -mabi=32 -mips3 -G 0
LD		= mips-n64-ld --no-check-sections
OBJCOPY = mips-n64-objcopy

# compile C files to object files #
.c.o:
	@echo "Compiling C code: $@"
	@$(CC) $(DEFINES) -c -O0 -g -I. -I$(INC) -I$(INC)/PR $<
.s.o:
	@echo "Assembling: $@"
	@$(AS) -I. -I asm -o $@ $<

# addition files used by make #
FILE_DEPENDS	= Makefile.depend

# files to be made..
CODESEGMENT	= test_wave.o
BOOT		= /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ	= boot.6102.o

ELF		= fxchange.elf
TARGETS	= fxchange.z64
MAP		= fxchange.map

LD_SCRIPT	= fxchange.ld
CP_LD_SCRIPT	= fxchange_cp.ld

ASMFILES	= asm/entry.s asm/rom_header.s
ASMOBJECTS	= $(ASMFILES:.s=.o)

CODEFILES	= test_main.c test_music.c
CODEOBJECTS	= $(CODEFILES:.c=.o)

DATAFILES	= test_workspace.c
DATAOBJECTS	= $(DATAFILES:.c=.o)

OBJECTS		= $(ASMOBJECTS) $(BOOT_OBJ) $(CODESEGMENT) $(DATAOBJECTS)

# default settings for compilation and linking #
ifeq  ($(NAUDIO),1) 
NAUDIODEF = -DNAUDIO
LDFLAGS = $(MKDEPOPT) -L$(LIB) -ln_mus -ln_audio_sc -lultra_rom -L$(N64_LIBGCCDIR) -lgcc
else
LDFLAGS = $(MKDEPOPT) -L$(LIB) -lmus -lultra_rom -L$(N64_LIBGCCDIR) -lgcc
endif

DEFINES = -DNDEBUG -D_FINALROM $(NAUDIODEF)

default:	$(TARGETS)

include $(FILE_DEPENDS)

$(BOOT_OBJ): $(BOOT)
	@echo "Linking bootcode"
	@$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	@echo "Preprocessing linkerscript"
	@cpp -P -Wno-trigraphs $(NAUDIODEF) -o $@ $<

$(CODESEGMENT):	$(CODEOBJECTS) Makefile
	@echo "Linking codesegment"
	@$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

# generate stand alone test rom #
$(TARGETS): $(CP_LD_SCRIPT)	$(FILE_MAKEROM) $(OBJECTS)
	@echo "Making rom"
	@$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	@$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	@makemask $(TARGETS)

# remove generated output files #
clean:
	$(RM) $(OBJECTS) $(CODEOBJECTS)
	$(RM) $(ELF) $(TARGETS) $(CP_LD_SCRIPT) $(MAP)
