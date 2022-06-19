#
# 	NuSYS Makefile for NINTENDO64
#
#	Copyright (C) 1997, NINTENDO Co,Ltd.
#

CP		=	cp

default:
	$(MAKE) -C nusys    all
	$(MAKE) -C nualsgi  all
	$(MAKE) -C nualstl3 all

install: default
#	Install nusys binaries and headers
	mkdir -p build/inc
	$(CP) nusys/include/nusys.h build/inc
	$(CP) nusys/build/libnusys/*.o build
	$(CP) nusys/build/libnusys/*.a build
	$(CP) nusys/build/libnusys_d/*.o build
	$(CP) nusys/build/libnusys_d/*.a build
#	Install nualsgi binaries and headers
	$(CP) nualsgi/include/nualsgi.h build/inc
	$(CP) nualsgi/include/nualsgi_n.h build/inc
	$(CP) nualsgi/build/libnualsgi/*.a build
	$(CP) nualsgi/build/libnualsgi_d/*.a build
	$(CP) nualsgi/build/libnualsgi_n/*.a build
	$(CP) nualsgi/build/libnualsgi_n_d/*.a build
#	Install nualstl3 binaries and headers
	$(CP) nualstl3/include/nualstl.h build/inc
	$(CP) nualstl3/include/nualstl_n.h build/inc
	$(CP) nualstl3/build/libnualstl/*.a build
	$(CP) nualstl3/build/libnualstl_d/*.a build
	$(CP) nualstl3/build/libnualstl_n/*.a build
	$(CP) nualstl3/build/libnualstl_n_d/*.a build

clean:
	rm -rf build
	$(MAKE) -C nusys    clean 
	$(MAKE) -C nualsgi  clean
	$(MAKE) -C nualstl3 clean

