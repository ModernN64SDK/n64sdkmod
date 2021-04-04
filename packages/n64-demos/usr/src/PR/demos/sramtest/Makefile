# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo.
#        
#        $RCSfile: Makefile,v $
#        $Revision: 1.1.1.1 $
#        $Date: 2002/05/02 03:27:26 $
# --------------------------------------------------------------------
include /usr/include/n64/make/PRdefs

OPTIMIZER = -g -O0

HFILES		= nu64sys.h sram.h

ELF		= sramtest.elf
TARGETS	= sramtest.z64
MAP		= sramtest.map

LD_SCRIPT	= sramtest.ld
CP_LD_SCRIPT	= sramtest_cp.ld

ASMFILES    =	asm/entry.s asm/rom_header.s

ASMOBJECTS  =	$(ASMFILES:.s=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

CODEFILES	= main.c sraminit.c

CODEOBJECTS	= $(CODEFILES:.c=.o)

DATAFILES	= readbuf.c writebuf.c

DATAOBJECTS	= $(DATAFILES:.c=.o)

CODESEGMENT	= codesegment.o

OBJECTS		= $(ASMOBJECTS) $(BOOT_OBJ) $(CODESEGMENT) $(DATAOBJECTS)

LCDEFS		= -DDEBUG
LCINCS		= -I. -I$(ROOT)/usr/include/PR -I $(ROOT)/usr/include

LCOPTS		= -G 0

LDIRT  =	$(ELF) $(MAP) $(CP_LD_SCRIPT) $(ASMOBJECTS) $(TARGETS)

LDFLAGS		= $(MKDEPOPT) -L$(ROOT)/usr/lib -lultra_d -L$(N64_LIBGCCDIR) -lgcc

default: $(TARGETS)

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

