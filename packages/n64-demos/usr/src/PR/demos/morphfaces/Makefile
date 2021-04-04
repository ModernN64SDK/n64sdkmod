#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#        
#        $RCSfile: GNUmakefile,v $
#        $Revision: 1.1 $
#        $Date: 2003/04/04 17:00:12 $
# --------------------------------------------------------------------
#
# This demo has the following features
#
#	CPU vertex morphing
#       Multi-pass rendering using transparency
#
#	Bill Mark
#

include /usr/include/n64/make/PRdefs

OPTIMIZER       = -O2
LCDEFS          = -DNDEBUG -D_FINALROM -DF3DEX_GBI_2
N64LIB          = -lultra_rom

ELF		= morphfaces.elf
TARGETS	= morphfaces.z64
MAP		= morphfaces.map

LD_SCRIPT	= morphfaces.ld
CP_LD_SCRIPT	= morphfaces_cp.ld

ASMFILES    =	asm/entry.s asm/rom_header.s
ASMOBJECTS  =	$(ASMFILES:.s=.o)

CODEFILES =     morphfaces.c morph.c data.c dram_stack.c
CODEOBJECTS =	$(CODEFILES:.c=.o)

CODESEGMENT =	codesegment.o

DATAFILES =	static.c dynamic.c zbuf.c cfb.c
DATAOBJECTS =	$(DATAFILES:.c=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ	= boot.6102.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS) $(ASMOBJECTS) $(BOOT_OBJ)

LCINCS =	-I. -I/usr/include/n64/PR

LDFLAGS =	-L/usr/lib/n64 $(N64LIB) -L$(N64_LIBGCCDIR) -lgcc 

LDIRT  =	$(ELF) $(CP_LD_SCRIPT) $(MAP) $(ASMOBJECTS) $(TARGETS) medit medit2c load.map mstatic.h mcode.h

default:	$(TARGETS)

include $(COMMONRULES)

.s.o:
	$(CC) -c -x assembler-with-cpp -I. -I asm -Wa,-Iasm -o $@ $<

$(CODESEGMENT): $(CODEOBJECTS)
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)

static.o: static.c ./mstatic.h

morphfaces.o: morphfaces.c morphfaces.h

data.o: ./mcode.h data.c

./mstatic.h: ./mcode.h

# program is not actually dependent on medit, but put it here to force it to
# be kept up to date
# (I've now commented out this dependency because it requires X11 header files
#  which some people don't have)
#
# Note: run medit as follows:
# medit -p ed.ppm -p tj.ppm -v ed_tj.v -t ed_tj.t [-m 6]

#./mcode.h: medit medit2c ed_tj.v ed_tj.t
./mcode.h: medit2c ed_tj.v ed_tj.t
	./medit2c -v ed_tj.v -t ed_tj.t -i ed.ppm -j tj.ppm \
	        -s mstatic.h -c mcode.h -m 16
medit: medit.c
	$(HOST_CC) -o medit medit.c -lX11 -lGL -lGLU -lm

medit2c: medit2c.c
	$(HOST_CC) -o medit2c medit2c.c
