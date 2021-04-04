#!smake -k
#
# Makefile for u64 dev board inventory program
#
#

include /usr/include/n64/make/PRdefs

OPTIMIZER =	-g

ELF		= ginv.elf
TARGETS	= ginv.z64
MAP		= ginv.map

LD_SCRIPT	= ginv.ld
CP_LD_SCRIPT	= ginv_cp.ld

ASMFILES    =	asm/entry.s asm/rom_header.s

ASMOBJECTS  =	$(ASMFILES:.s=.o)

CODEFILES   =	ginv.c

CODEOBJECTS =	$(CODEFILES:.c=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(ASMOBJECTS) $(BOOT_OBJ)

LCINCS =	-I.
LCOPTS =	-G 0
LDFLAGS =	-L/usr/lib/n64 -lultra_d -L$(N64_LIBGCCDIR) -lgcc

LDIRT  =	$(ELF) $(MAP) $(CP_LD_SCRIPT) $(ASMOBJECTS) $(TARGETS)

default:	$(TARGETS)

include $(COMMONRULES)

.s.o:
	$(AS) -Wa,-Iasm -o $@ $<

$(CODESEGMENT):	$(CODEOBJECTS)
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)
	@printf "Warning!\nThis demo requires a printf implementation. UNFLoader is recommended.\n"
	@printf "Therefore, NOTHING is displayed to the TV.\nThe demo prints its results to PC.\n"
	@echo; read -rsn1 -p "Press any key to continue . . ."; echo
