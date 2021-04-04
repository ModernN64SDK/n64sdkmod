.SUFFIXES:
.SUFFIXES:	.o .c

include /usr/include/n64/make/PRdefs

########## Please customize the following macros as you see fit. ##########

# To allow a note be removed from the controller pak menu,
# set 1 to ENABLE_DELETION.
ENABLE_DELETION	= 0

# If DEBUGSYM is set to an null text string, you will be unable to generate debug symbols.
DEBUGSYM	= -g

# If the following compiler flag is changed, the optimization level will also change.
OPTIMIZER	= -O0

# NuSystem related directories
NUSYSINC	= /usr/include/n64/nusys
NUSYSLIB	= /usr/lib/n64/nusys

# When an object is set up to be dependent on Makefile, 
# set DEPEND_ON_MAKE = Makefile.
# Comment this line out if you do not want it to be dependent.
DEPEND_ON_MAKE  = Makefile

########## You may customize the following. ##########

TARGET		= 64devsi

ASMFILES	= asm/entry.s asm/rom_header.s
ASMOBJECTS	= $(ASMFILES:.s=.o)
CODEFILES	= main.c common.c mainmenu.c contmenu.c cpakmenu.c rmbmenu.c
NUOBJ		= $(NUSYSLIB)/nusys.o
CODEOBJECTS	= $(CODEFILES:.c=.o) $(NUOBJ)
BOOT		= /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ	= boot.6102.o

CUSTFLAGS	= -D__ENABLE_DELETION__=$(ENABLE_DELETION)
LCINCS		= -I$(NUSYSINC)
LCOPTS		=  $(DEBUGSYM) $(CUSTFLAGS)
LDFLAGS	= -L/usr/lib/n64 -L/usr/lib/n64/PR -L$(NUSYSLIB) \
		  -lnusys -lultra_rom -L$(N64_LIBGCCDIR) -lgcc
LDIRT		= $(ROM) $(MAP) $(ELF) $(CP_LD_SCRIPT) $(ASMOBJECTS)

CODESEGMENT	= codesegment.o

OBJECTS	= $(CODESEGMENT) $(ASMOBJECTS) $(BOOT_OBJ)

ELF		= $(TARGET).elf
ROM		= $(TARGET).z64
MAP		= $(TARGET).map
LD_SCRIPT	= $(TARGET).ld
CP_LD_SCRIPT	= $(TARGET)_cp.ld

default:	$(ROM)

%.o: %.s
	$(AS) -I. -I asm -Wa,-Iasm -o $@ $<

$(CODESEGMENT):	$(CODEOBJECTS) Makefile
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -I$(NUSYSINC) -o $@ $<

$(ROM): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(ROM) -O binary
	makemask $(ROM)

main.o:		main.c mainmenu.h rmbmenu.h $(DEPEND_ON_MAKE)
common.o:	common.c common.h $(DEPEND_ON_MAKE)
mainmenu.o:	mainmenu.c mainmenu.h common.h $(DEPEND_ON_MAKE)
contmenu.o:	contmenu.c contmenu.h common.h $(DEPEND_ON_MAKE)
cpakmenu.o:	cpakmenu.c cpakmenu.h common.h $(DEPEND_ON_MAKE)
rmbmenu.o:	rmbmenu.c rmbmenu.h common.h $(DEPEND_ON_MAKE)

include /usr/include/n64/make/commonrules
