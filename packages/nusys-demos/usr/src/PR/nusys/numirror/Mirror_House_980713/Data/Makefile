#=======================================================================#
#		NIFFprev						#
#		Makefile.win						#
#									#
#		Copyright (C) 1997, NINTENDO Co,Ltd.			#
#									#
#-----------------------------------------------------------------------# 
#

# The directory which has include files and library of NuSYSTEM
#
include /usr/include/n64/make/PRdefs

NUSYSINCDIR  = /usr/include/n64/nusys
NUSYSLIBDIR  = /usr/lib/n64/nusys

N64GCC_INC =	/usr/include/n64
USR_INC    =	../Include

GBI = -DF3DEX_GBI_2

#default: sp_select_bg.c sp_lit.c test_data2.c
default: opening_imgs.c logo_imgs.c
	$(CC) -g -G 0 -c $(CC_INC) $(CC_DEF) opening_imgs.c
	$(CC) -g -G 0 -c $(CC_INC) $(CC_DEF) logo_imgs.c
	$(CC) -g -G 0 -c $(CC_INC) $(CC_DEF) sp_select_bg.c
	$(CC) -g -G 0 -c $(CC_INC) $(CC_DEF) opening.c
	$(CC) -g -G 0 -c $(CC_INC) $(CC_DEF) om_mem_heap.c

CC_INC =	-I. -I$(N64GCC_INC)/PR -I$(N64GCC_INC) -I$(NUSYSINCDIR) -I$(USR_INC)

clean:
	rm -rf *.o

install:
	cp *.o ../.

# DO NOT DELETE
