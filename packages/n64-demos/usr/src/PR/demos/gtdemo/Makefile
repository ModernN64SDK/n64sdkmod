#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997,1998 Nintendo. (Originated by SGI)
#
#        $RCSfile: Makefile.src,v $
#        $Revision: 1.1 $
#        $Date: 1999/01/12 13:28:00 $
# --------------------------------------------------------------------
#
# Makefile for turbo demo application
#

include /usr/include/n64/make/PRdefs


OPTIMIZER       = -O2
LCDEFS          = -DNDEBUG -D_FINALROM
N64LIB          = -lultra_rom

ELF		= gtdemo.elf
TARGETS	= gtdemo.z64
MAP		= gtdemo.map

LD_SCRIPT	= gtdemo.ld
CP_LD_SCRIPT	= gtdemo_cp.ld

ASMFILES    =	asm/entry.s asm/rom_header.s

ASMOBJECTS  =	$(ASMFILES:.s=.o)

CODEFILES   =	gtdemo.c floor.c turbo.c character.c controller.c 	\
		timer.c walk.c punch.c rdp_output.c rdp_output_len.c

CODEOBJECTS =	$(CODEFILES:.c=.o) 

DATAFILES   =	static.c zbuffer.c cfb.c

DATAOBJECTS =	$(DATAFILES:.c=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

CODESEGMENT =	codesegment.o

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS) $(ASMOBJECTS) $(BOOT_OBJ)

LCINCS =	-I. -I/usr/include/n64/PR -I /usr/include/n64 -I../Texture
LDIRT  =	$(ELF) $(CP_LD_SCRIPT) $(MAP) $(ASMOBJECTS) walktable walktable.h punchtable punchtable.h $(TARGETS)

LDFLAGS =	-L/usr/lib/n64 -L/usr/lib/n64/PR $(N64LIB) -L$(N64_LIBGCCDIR) -lgcc

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

#
# Header file build rules
#
walk.o:	walktable.h

punch.o:	punchtable.h


walktable:	walktable.c
	$(HOST_CC) -o $@ walktable.c

walktable.h:	walktable
	./walktable > walktable.h	

punchtable:	punchtable.c
	$(HOST_CC) -o $@ punchtable.c

punchtable.h:	punchtable
	./punchtable > punchtable.h	






