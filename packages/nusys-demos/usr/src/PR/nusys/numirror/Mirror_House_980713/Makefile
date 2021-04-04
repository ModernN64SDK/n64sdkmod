#=======================================================================#
#		NIFFprev						#
#		Makefile.win						#
#									#
#		Copyright (C) 1997, NINTENDO Co,Ltd.			#
#									#
#-----------------------------------------------------------------------# 
#
# If SGISND is set to 0, SoundTools will be used
SGISND		= 1

# The directory which has the include file and library of NuSYSTEM
#
include /usr/include/n64/make/PRdefs

NUSYSINCDIR  = /usr/include/n64/nusys
NUSYSLIBDIR  = /usr/lib/n64/nusys

N64GCC_INC =	/usr/include/n64
USR_INC    =	./Include
LIBDIR     =	./Lib

ELF		= mirror_house.elf
TARGETS	= mirror_house.z64
MAP		= mirror_house.map
LD_SCRIPT	= mirror_house.ld
CP_LD_SCRIPT	= mirror_house_cp.ld

ifeq	($(SGISND), 1)
AUDIO_CDEFS = -DNUALSGI -DN_AUDIO      # Compile options for the SGI Sound Library version
NUAUDIOLIB = -lnualsgi -ln_audio      # The SGI Sound Library
else
AUDIO_CDEFS = -DNUALSTL -DN_AUDIO   #Compile options for the N64 Sound Tools version
NUAUDIOLIB = -lnualstl_n -ln_mus -ln_audio_sc # N64 Sound Tools 
endif
GBI = -DF3DEX_GBI_2

.c.o:
		$(CC) -g -G 0 -c $(CC_INC) $(CC_DEF) $(AUDIO_CDEFS) $<

CC_INC =	-I. -I$(N64GCC_INC)/PR -I$(N64GCC_INC)/nustd -I$(N64GCC_INC) -I$(NUSYSINCDIR) -I$(USR_INC)
CC_DEF =	$(GBI) -D_WIN32

OPTIMIZER =	-g

ASMFILES	:= asm/entry.s asm/rom_header.s sgi_data.s sndtools_data.s

ASMOBJECTS	= $(ASMFILES:.s=.o)

BOOT		= /usr/lib/n64/PR/bootcode/boot.6102

BOOT_OBJ	= boot.6102.o

CODEFILES   =	main.c \
		expad.c \
		gfx.c \
		om_camera.c \
		om_eval_draw.c \
		om_char_entry.c \
		om_common.c \
		om_func.c \
		om_bg_entry.c \
		om_cornfrake.c \
		om_game_main.c \
		om_mychar.c \
		om_goal.c \
		om_game_over.c \
		om_audio.c \
		om_select_chara.c \
		om_title.c \
		om_title_param.c \
		om_rom_data_read.c

DATAOBJECTS =	$(CODEFILES:.c=.o)
CODEOBJECTS =	$(DATAOBJECTS) $(NUSYSLIBDIR)/nusys.o

CODESEGMENT =	codesegment.o

OBJECTS =	$(ASMOBJECTS) $(BOOT_OBJ) $(CODESEGMENT) $(DATAOBJECTS)

USR_LIB =	-lreschar -lresbg -lnd

LDFLAGS =	$(MKDEPOPT) -L/usr/lib/n64 -L$(NUSYSLIBDIR) -L$(LIBDIR) $(USR_LIB) $(NUAUDIOLIB) -lnusys -lultra_rom -lnustd -L$(N64_LIBGCCDIR) -lgcc
DEPENDFILE =    dependfile

default:	$(TARGETS)

.s.o:
	$(AS) -I. -I asm -Wa,-Iasm -o $@ $<

$(CODESEGMENT) :	$(DATAOBJECTS)
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -I$(NUSYSINCDIR) $(AUDIO_CDEFS) -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)

$(CODESEGMENT) : ./Lib/libreschar.a ./Lib/libnd.a ./Lib/libresbg.a

depend:
	makedepnd -f$(DEPENDFILE) *.c

clean:
	rm -rf *.o
	rm -rf $(ELF) $(ASMOBJECTS) $(CP_LD_SCRIPT) $(MAP)
	rm -rf $(TARGETS)

# DO NOT DELETE
