.SUFFIXES:
.SUFFIXES:	.o .c

include /usr/include/n64/make/PRdefs

########## Please customize the following macros as you see fit. ##########

# If N_AUDIO is set to 0, N_AUDIO will be disabled.
N_AUDIO		= 1

# If DEBUGSYM is set to an null text string, you will be unable to generate debug symbols.
DEBUGSYM	= -g

# If the following compiler flag is changed, the optimization level will also change.
OPTIMIZER	= -O0

# NuSystem related directories
NUSYSINC  = /usr/include/n64/nusys
NUSYSLIB  = /usr/lib/n64/nusys

# When an object is set up to be dependent on Makefile, 
# set DEPEND_ON_MAKE = Makefile.
# Comment this line out if you do not want it to be dependent.
DEPEND_ON_MAKE  = Makefile

########## You may customize the following. ##########

TARGET		= 64aud3

ASMFILES	= asm/entry.s asm/rom_header.s
ASMOBJECTS	= $(ASMFILES:.s=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ	= boot.6102.o

CODEFILES	= main.c menu.c sound.c rom.c \
		  menu0.c menu1.c
NUOBJ		= $(NUSYSLIB)/nusys.o
CODEOBJECTS	= $(CODEFILES:.c=.o) $(NUOBJ)

ifeq	($(N_AUDIO), 1)
CUSTFLAGS	= -DN_AUDIO
LDFLAGS		= -L/usr/lib/n64 -L/usr/lib/n64/PR -L$(NUSYSLIB) \
		  -ln_mus -ln_audio_sc -lnualstl -lnusys -lultra_rom \
		  -L$(N64_LIBGCCDIR) -lgcc
else
CUSTFLAGS	= 
LDFLAGS		= -L/usr/lib/n64 -L/usr/lib/n64/PR -L$(NUSYSLIB) \
		  -lmus -lnualstl -lnusys -lultra_rom \
		  -L$(N64_LIBGCCDIR) -lgcc
endif
LCINCS		= -I. -I$(NUSYSINC) -I/usr/include/n64 -I/usr/include/n64/PR
LCOPTS		= -G 0 $(DEBUGSYM) $(CUSTFLAGS)

CODESEGMENT	= codesegment.o

OBJECTS		= $(ASMOBJECTS) $(CODESEGMENT) $(BOOT_OBJ)

DATADIR		= ../data
DATAFILES	= $(DATADIR)/groove.ptr $(DATADIR)/groove.wbk $(DATADIR)/groove.bin \
		  $(DATADIR)/orch.ptr $(DATADIR)/orch.wbk $(DATADIR)/orch.bin \
		  $(DATADIR)/rock.ptr $(DATADIR)/rock.wbk $(DATADIR)/rock.bin \
		  $(DATADIR)/demo3_4.ptr $(DATADIR)/demo3_4.wbk $(DATADIR)/demo3_4.bfx \
		  $(DATADIR)/demo3_5.ptr $(DATADIR)/demo3_5.wbk $(DATADIR)/demo3_5.bfx

LDIRT =  $(ELF) $(ASMOBJECTS) $(CP_LD_SCRIPT) $(MAP) $(TARGETS)

ELF		= $(TARGET).elf
TARGETS	= $(TARGET).z64
MAP		= $(TARGET).map
LD_SCRIPT	= $(TARGET).ld
CP_LD_SCRIPT	= $(TARGET)_cp.ld

default:	$(TARGETS)

%.o: %.s
	$(AS) -I. -I asm -Wa,-Iasm -o $@ $<

$(CODESEGMENT):	$(CODEOBJECTS)
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -I$(NUSYSINC) $(CUSTFLAGS) -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)

main.o:			main.c $(DEPEND_ON_MAKE)
menu.o:			menu.c menu.h define.h $(DEPEND_ON_MAKE)
menu0.o:		menu0.c menu.h sound.h define.h $(DEPEND_ON_MAKE)
menu1.o:		menu1.c menu.h sound.h define.h $(DEPEND_ON_MAKE)
sound.o:		sound.c sound.h menu.h define.h rom.h segment.h $(DEPEND_ON_MAKE)
rom.o:			rom.c $(DEPEND_ON_MAKE)

include /usr/include/n64/make/commonrules
