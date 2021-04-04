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
	$(MAKE) -C Mirror_House_980713/Nd
	$(MAKE) -C Mirror_House_980713/Nd install
	$(MAKE) -C Mirror_House_980713/Resource_Bg
	$(MAKE) -C Mirror_House_980713/Resource_Bg install
	$(MAKE) -C Mirror_House_980713/Resource_Char
	$(MAKE) -C Mirror_House_980713/Resource_Char install
	$(MAKE) -C Mirror_House_980713/Data
ifeq	($(SGISND), 1)
	$(MAKE) -C Mirror_House_980713 SGISND=1
else
	$(MAKE) -C Mirror_House_980713 SGISND=0
endif

clean:
	$(MAKE) -C Mirror_House_980713/Nd clean
	$(MAKE) -C Mirror_House_980713/Resource_Bg clean
	$(MAKE) -C Mirror_House_980713/Resource_Char clean
	$(MAKE) -C Mirror_House_980713/Data clean
ifeq	($(SGISND), 1)
	$(MAKE) -C Mirror_House_980713 SGISND=1 clean
else
	$(MAKE) -C Mirror_House_980713 SGISND=0 clean
endif
	$(RM) Mirror_House_980713/Lib/*.a
