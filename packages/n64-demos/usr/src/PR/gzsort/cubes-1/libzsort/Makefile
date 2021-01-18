#! smake -f
#---------------------------------------------------------------------#
#	Copyright (C) 1997, Nintendo.
#	
#	File		Makefile
#	Coded    by	Yoshitaka Yasumoto.	Sep 12, 1997.
#	Modified by	
#	
#	$Id: Makefile,v 1.10 1997/11/26 09:10:38 mitu Exp $
#---------------------------------------------------------------------#

CC			= mips-n64-gcc -mabi=32 -ffreestanding -mfix4300 -G 0
AS			= mips-n64-gcc -c -x assembler-with-cpp -mabi=32 -ffreestanding -mfix4300 -G 0
LD			= mips-n64-ld --no-check-sections
OBJCOPY		= mips-n64-objcopy
AR			= mips-n64-ar

.c.o:
	$(CC) $(OPTIMIZER) -c $(LCINCS) $(LCDEFS) $<
.s.o:
	$(AS) -I. -I asm -o $@ $<

TARGETS   =	libzsort.a
OPTIMIZER =	-g -O0

HFILES    =	gzpoly.h dirtydcache.h
CFILES    =	zarray.c zsetmmtx.c zcalcpoly.c zdrawpoly.c zdrawpoly_sh.c \
		zdrawpoly_tx.c
ASFILES   =	dirtydcache.s dirtydcache16.s
OBJECTS   =	$(CFILES:.c=.o) $(ASFILES:.s=.o)

LCINCS +=	-I. -I/usr/include/n64/PR -I/usr/include/n64
LCDEFS +=	-DNDEBUG -D_FINALROM -DF3DEX_GBI -DF3DEX_GBI_2
LCDEFS +=	-DFOG
DIRT   =	$(OBJECTS) $(TARGETS)

default:	$(TARGETS)

clean:
	rm -rf $(DIRT)

libzsort.a:	$(OBJECTS)
	$(AR) cur $@ $(OBJECTS)

#======== End of Makefile ========
