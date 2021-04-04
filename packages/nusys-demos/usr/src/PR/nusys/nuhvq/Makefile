#!smake -k
# 	NuSYSTEM samples nuhvq Makefile

include /usr/include/n64/make/PRdefs

# Directory holding NuSYSTEM include file and library
#
NUSYSINCDIR  = /usr/include/n64/nusys
NUSYSLIBDIR  = /usr/lib/n64/nusys
LIB = /usr/lib/n64

#  Directory holding the HVQ2 library  (libhvq.a)
HVQLIBDIR = /usr/lib/n64/hvq
#  Directory holding the HVQ2 include file
HVQINCDIR = /usr/include/n64/hvq

# If you are using the old audio microcode please
# comment this out
N_AUDIO = yes

ifeq  ($(N_AUDIO),yes) 
NAUDIODEF = -DNAUDIO
NUAUDIOLIB = -lnualstl_n -ln_mus -ln_audio_sc
else
NUAUDIOLIB = -lnualstl -lmus
endif

#  Maximum size of image [pixel]  (Reflects on the memory reserved for hvqwork.c)
MAXWIDTH  = 320
MAXHEIGHT = 240

ELF		= nuhvq.elf
TARGETS	= nuhvq.z64
MAP		= nuhvq.map
LD_SCRIPT	= nuhvq.ld
CP_LD_SCRIPT	= nuhvq_cp.ld

HFILES =	main.h

ASMFILES	= asm/entry.s asm/rom_header.s rawdata.s

ASMOBJECTS	= $(ASMFILES:.s=.o)

CODEFILES   = 	main.c stage00.c appgfxinit.c

CODEOBJECTS =	$(CODEFILES:.c=.o)  $(NUSYSLIBDIR)/nusys.o

DATAFILES   =	hvqwork.c hvqbuf.c hvqimg.c

DATAOBJECTS =	$(DATAFILES:.c=.o)

CODESEGMENT =	codesegment.o

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

OBJECTS =	$(ASMOBJECTS) $(BOOT_OBJ) $(CODESEGMENT) $(DATAOBJECTS)

LCDEFS =  -DMAXWIDTH=$(MAXWIDTH) -DMAXHEIGHT=$(MAXHEIGHT) 

LCINCS =	-I. -I$(HVQINCDIR) -I$(NUSYSINCDIR) -I/usr/include/n64/PR
LCOPTS =	-G 0
LDFLAGS =	$(MKDEPOPT) -L$(LIB) -L$(NUSYSLIBDIR) $(NUAUDIOLIB) -lnusys -lultra_rom -L$(N64_LIBGCCDIR) -lgcc -L$(HVQLIBDIR) -lhvq2
LDIRT =  $(ELF) $(ASMOBJECTS) $(CP_LD_SCRIPT) $(MAP) $(TARGETS)

default:	$(TARGETS)

include $(COMMONRULES)

.s.o:
	$(AS) -I. -I asm -Wa,-Iasm -o $@ $<

$(CODESEGMENT):	$(CODEOBJECTS) Makefile
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -I$(NUSYSINCDIR) -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)