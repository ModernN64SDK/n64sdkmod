#! smake -f
#---------------------------------------------------------------------#
#	Copyright (C) 1997, Nintendo.
#	
#	File		Makefile
#	Coded    by	Yoshitaka Yasumoto.	Sep 12, 1997.
#	Modified by	
#	
#	$Id: Makefile,v 1.8 1997/11/26 09:10:37 mitu Exp $
#---------------------------------------------------------------------#

CC			= mips-n64-gcc -mabi=32 -ffreestanding -mfix4300 -G 0
AS			= mips-n64-as -mtune=vr4300 -march=vr4300 -mabi=32 -mips3 -G 0
LD			= mips-n64-ld --no-check-sections
OBJCOPY		= mips-n64-objcopy
AR			= mips-n64-ar

.c.o:
	$(CC) $(OPTIMIZER) -c $(LCINCS) $(LCDEFS) $<
.s.o:
	$(AS) -I. -I asm -o $@ $<

##### APP/OPTIMIZER
ELF		= cube.elf
TARGETS	= cube.z64
MAP		= cube.map
LD_SCRIPT	= cube.ld
CP_LD_SCRIPT	= cube_cp.ld
OPTIMIZER =	-g -O0

ASMFILES    =	asm/entry.s asm/rom_header.s
ASMOBJECTS  =	$(ASMFILES:.s=.o)
CODEFILES =	action.c cube.c cube_geo.c main.c static.c system.c uc_assert.c
CODEOBJECTS =	$(CODEFILES:.c=.o)
CODESEGMENT =	codesegment.o
DATAFILES =	system_cfb.c system_rspbuf.c
DATAOBJECTS =	$(DATAFILES:.c=.o)
BOOT		= /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ	= boot.6102.o
OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS) $(ASMOBJECTS) $(BOOT_OBJ)

LIBDIR =	libzsort
LIBZSORT =	libzsort.a
LIBZSORTFLAG =	-L$(LIBDIR) -lzsort

SUBDIRS =	$(LIBDIR)

LCINCS +=	-I. -I$(LIBDIR) -I/usr/include/n64/PR -I/usr/include/n64
LCDEFS +=	-DNDEBUG -D_FINALROM -DF3DEX_GBI -DF3DEX_GBI_2
LCDEFS +=	-DFOG
DIRT    =	$(ELF) $(TARGETS) $(MAP) $(CP_LD_SCRIPT) $(OBJECTS) $(CODEOBJECTS)

LDFLAGS 	=	-L/usr/lib/n64 -lultra_rom -L$(N64_LIBGCCDIR) -lgcc $(LIBZSORTFLAG)

default:	$(TARGETS)

clean:
	rm -rf $(DIRT)
	make -C $(LIBDIR) clean

$(LIBDIR)/$(LIBZSORT):	$(LIBDIR)
	make -C $(LIBDIR) $(LIBZSORT)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CODESEGMENT):	$(CODEOBJECTS) $(LIBDIR)/$(LIBZSORT)
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)

cube.o:		cube.h

cube_geo.o:	reflect.h brick.h cube.h

#======== End of Makefile ========
