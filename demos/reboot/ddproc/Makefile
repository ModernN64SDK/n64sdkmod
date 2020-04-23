#
# Please refer to /usr/include/make/
# 

#!smake
include $(ROOT)/usr/include/make/PRdefs

#START_FROM_CASSETTE = YES
#FINAL = YES
#ERROR_DONT_STOP = YES

#
#  for the compiler    -->  (c)
#  for the linker      -->  (l)
#  for the ROM bugger(mild -->  (m)
#

ifeq ($(FINAL),YES)
        OPTIMIZER = -O2					 	# (c) Optimize for the maximum level
#        LCDEFS    =						  
else
        OPTIMIZER = -g						 	# (c) Create the debugging information		
        LCDEFS   += -D_RDB_	-DDEBUG			# (c) #define DEBUG 
	LCOPTS+=	-Wunused -Wswitch -Wreturn-type -Werror -W -Wstrict-prototypes
	LCOPTS+=	-Wimplicit -Wmissing-prototypes  -Wredundant-decls	
	LCOPTS+=    -Wcast-qual						# (c) Type of warnings
# LCOPTS+=    -Wconversion
endif

ifeq ($(START_FROM_CASSETTE),YES)
        LCDEFS += -DSTART_FROM_CASSETTE		# (c) define START_FROM_CASSETTE	
		CODESEGMENT =	ddproc.o
		DDOBJ  = dd.o 
#        DDDEFS =
else
#        LCDEFS +=
		CODESEGMENT =	ddproc2.o
        DDDEFS = -DD64 -t 0					# (m) create theImage file for the DD disc type 0 and gwrite
#											
		DDOBJ  = dd2.o
endif

#
# Combine source files into an object

CODEFILES   =	globals.c \
				try.c \
				si.c \
				reset.c \
				video_core.c video_str.c video_jstr.c video_draw.c \
				sprite.c heap.c				

OPTIONALCODES = sb.c btile.c	

#ifndef COMPACT  
CODEFILES += $(OPTIONALCODES)
#endif

CODEOBJECTS =	$(CODEFILES:.c=.o)

#
# Make an individual source file into an object separately

DATAFILES   =   
#  cfb.c rsp_cfb.c readwritebuf.c
#  str_dlb.c  sp_dlb.c
DATAOBJECTS =   $(DATAFILES:.c=.o)

OBJECTS =	$(CODESEGMENT) $(DATAOBJECTS)	# (c) name of the target object		

LCINCS =	-I.								# (c) include the current directory in the include path.
LCOPTS +=	-G 0 							# (c) Small data section is not used
LCDEFS  +=	-DPTN64 -DTEST					# (c) necessary for the host communication

#
# (l) Options for the linker

LDFLAGS =	$(MKDEPOPT)

#
# refer to /usr/include/make/commonrules
# file to be deleted by clobber

LDIRT  =	$(APP) *.so gwrite	

# 
# Create up to the targets file (finish at the compile if the file is .o)
# 

default: $(OBJECTS)

#
# Partner starts up from the disk if there is gwrite
#
#ifeq ($(START_FROM_CASSETTE),YES)
#	deltree /y gwrite
#endif

#
# compile
#

include $(COMMONRULES)

#
# link
#
#
$(CODESEGMENT):	$(CODEOBJECTS) $(DDOBJ)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(DDOBJ) $(LDFLAGS)

#
# makerom
#
#
#$(TARGETS) $(APP):	$(OBJECTS)
#	$(MAKEROM) $(DDDEFS) -r $(TARGETS) spec


# DD rule

ifeq ($(START_FROM_CASSETTE),YES)
$(DDOBJ): dd.c
	$(CC) $(CFLAGS) -DSTART_FROM_CASSETTE -c dd.c -o $(DDOBJ)
else
$(DDOBJ): dd.c
	$(CC) $(CFLAGS) -c dd.c -o $(DDOBJ)
endif
