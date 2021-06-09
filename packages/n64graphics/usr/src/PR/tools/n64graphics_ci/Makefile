CC	=	gcc
ECHO	=	echo

CFLAGS	=	-I. -DN64GRAPHICS_STANDALONE -O2 -c

default: n64graphics_ci

n64graphics_ci: n64graphics_ci.o utils.o exoquant.o
	@$(ECHO) Linking $@
	@$(CC) n64graphics_ci.o utils.o exoquant.o -o $@ -lm
	

.SUFFIXES: .o .c

%.o: %.c
	@$(ECHO) Compiling $<
	@$(CC) $(CFLAGS) $<

clean:
	rm -f n64graphics_ci n64graphics_ci.o utils.o exoquant.o