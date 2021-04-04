#!smake -k
# --------------------------------------------------------------------
#        Copyright (C) 1997 Nintendo. (Originated by SGI)
#        
#        $RCSfile: Makefile,v $
#        $Revision: 1.6 $
#        $Date: 1998/09/25 21:47:27 $
# --------------------------------------------------------------------
#
# Makefile for textures shared among applications
#

include /usr/include/n64/make/PRdefs

DATAFILES =				\
		brick.rgb		\
		chainlink.rgb		\
		checker.rgba		\
		letters.rgb		\
		mario.rgb		\
		molecule.rgba

TEXHFILES =				\
		RGBA16brick.h		\
		RGBA16brickMM.h		\
		RGBA16chainlink.h 	\
		RGBA16chainlinkMM.h 	\
		RGBA16checker.h		\
		RGBA16checkerMM.h	\
		RGBA16mario.h		\
		RGBA16marioMM.h		\
		RGBA16molecule.h	\
		RGBA16moleculeMM.h	\
		letters_img.h

LDIRT  =	$(TEXHFILES)

default:	$(TEXHFILES)

include $(COMMONRULES)

#
# these are all 'exports' because we need them for the apps
#
exports install:        default
	$(INSTALL) -m 444 -F /usr/src/PR/demos/Texture \
			README $(DATAFILES) Makefile

##########################################################################
#
# Texture .h files, built from RGB images:
#

RGBA16brick.h: brick.rgb
	$(RGB2C) -F -s 16 -m RGBA16brick brick.rgb > $@

RGBA16brickMM.h: brick.rgb
	$(RGB2C) -F -s 16 -m RGBA16brickMM -o MIP brick.rgb > $@

RGBA16chainlink.h: chainlink.rgb
	$(RGB2C) -F -s 16 -m RGBA16chainlink chainlink.rgb > $@

RGBA16chainlinkMM.h: chainlink.rgb
	$(RGB2C) -F -s 16 -m RGBA16chainlinkMM -o MIP chainlink.rgb > $@

RGBA16checker.h: checker.rgba
	$(RGB2C) -F -s 16 -m RGBA16checker checker.rgba > $@

RGBA16checkerMM.h: checker.rgba
	$(RGB2C) -F -s 16 -m RGBA16checkerMM -o MIP checker.rgba > $@

RGBA16mario.h: mario.rgb
	$(RGB2C) -s 16 -m RGBA16mario mario.rgb > $@

RGBA16marioMM.h: mario.rgb
	$(RGB2C) -s 16 -m RGBA16marioMM -o MIP mario.rgb > $@

RGBA16molecule.h: molecule.rgba
	$(RGB2C) -F -s 16 -m RGBA16molecule molecule.rgba > $@

RGBA16moleculeMM.h: molecule.rgba
	$(RGB2C) -F -s 16 -m RGBA16moleculeMM -o MIP molecule.rgba > $@

letters_img.h:	letters.rgb
	$(RGB2C) -F -m letters_img -f I -s 4 letters.rgb > $@

#
##########################################################################
