# n64graphics made by queueRAM, makefile by CrashOveride

CC	=	gcc
ECHO	=	echo

CFLAGS	=	-I. -DN64GRAPHICS_STANDALONE -O2 -c

default: n64graphics

n64graphics: n64graphics.o utils.o
	@$(ECHO) Linking $@
	@$(CC) n64graphics.o utils.o -o $@
	

.SUFFIXES: .o .c

%.o: %.c
	@$(ECHO) Compiling $<
	@$(CC) $(CFLAGS) $<

clean:
	rm -f n64graphics n64graphics.o utils.o