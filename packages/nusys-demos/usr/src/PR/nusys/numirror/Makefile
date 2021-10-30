#=======================================================================#
#		NIFFprev						#
#		Makefile						#
#									#
#		Copyright (C) 1997, NINTENDO Co,Ltd.			#
#									#
#-----------------------------------------------------------------------# 
#

MAKE		=	make
RM		=	rm

default:
ifeq	($(SGISND), 1)
	$(MAKE) -C Mirror_House SGISND=1
else
	$(MAKE) -C Mirror_House SGISND=0
endif

clean:
ifeq	($(SGISND), 1)
	$(MAKE) -C Mirror_House SGISND=1 clean
else
	$(MAKE) -C Mirror_House SGISND=0 clean
endif
