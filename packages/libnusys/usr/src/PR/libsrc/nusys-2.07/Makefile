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
	$(CP) nusys/include/nusys.h 	 /usr/include/n64/nusys
	$(CP) nusys/build/libnusys/*.o 	 /usr/lib/n64/nusys/
	$(CP) nusys/build/libnusys/*.a 	 /usr/lib/n64/nusys/
	$(CP) nusys/build/libnusys_d/*.o /usr/lib/n64/nusys/
	$(CP) nusys/build/libnusys_d/*.a /usr/lib/n64/nusys/
#	Install nualsgi binaries and headers
	$(CP) nualsgi/include/nualsgi.h /usr/include/n64/nusys
	$(CP) nualsgi/include/nualsgi_n.h /usr/include/n64/nusys
	$(CP) nualsgi/build/libnualsgi/*.a /usr/lib/n64/nusys/
	$(CP) nualsgi/build/libnualsgi_d/*.a /usr/lib/n64/nusys/
	$(CP) nualsgi/build/libnualsgi_n/*.a /usr/lib/n64/nusys/
	$(CP) nualsgi/build/libnualsgi_n_d/*.a /usr/lib/n64/nusys/
#	Install nualstl3 binaries and headers
	$(CP) nualstl3/include/nualstl.h /usr/include/n64/nusys
	$(CP) nualstl3/include/nualstl_n.h /usr/include/n64/nusys
	$(CP) nualstl3/build/libnualstl/*.a /usr/lib/n64/nusys/
	$(CP) nualstl3/build/libnualstl_d/*.a /usr/lib/n64/nusys/
	$(CP) nualstl3/build/libnualstl_n/*.a /usr/lib/n64/nusys/
	$(CP) nualstl3/build/libnualstl_n_d/*.a /usr/lib/n64/nusys/

pkginstall: default
#	Install nusys binaries and headers
	$(CP) nusys/include/nusys.h 	 /home/crash/n64dev/n64sdkmod/packages/libnusys/usr/include/n64/nusys
	$(CP) nusys/build/libnusys/*.o 	 /home/crash/n64dev/n64sdkmod/packages/libnusys/usr/lib/n64/nusys/
	$(CP) nusys/build/libnusys/*.a 	 /home/crash/n64dev/n64sdkmod/packages/libnusys/usr/lib/n64/nusys/
	$(CP) nusys/build/libnusys_d/*.o /home/crash/n64dev/n64sdkmod/packages/libnusys/usr/lib/n64/nusys/
	$(CP) nusys/build/libnusys_d/*.a /home/crash/n64dev/n64sdkmod/packages/libnusys/usr/lib/n64/nusys/
#	Install nualsgi binaries and headers
	$(CP) nualsgi/include/nualsgi.h /home/crash/n64dev/n64sdkmod/packages/libnusys/usr/include/n64/nusys
	$(CP) nualsgi/include/nualsgi_n.h /home/crash/n64dev/n64sdkmod/packages/libnusys/usr/include/n64/nusys
	$(CP) nualsgi/build/libnualsgi/*.a /home/crash/n64dev/n64sdkmod/packages/libnusys/usr/lib/n64/nusys/
	$(CP) nualsgi/build/libnualsgi_d/*.a /home/crash/n64dev/n64sdkmod/packages/libnusys/usr/lib/n64/nusys/
	$(CP) nualsgi/build/libnualsgi_n/*.a /home/crash/n64dev/n64sdkmod/packages/libnusys/usr/lib/n64/nusys/
	$(CP) nualsgi/build/libnualsgi_n_d/*.a /home/crash/n64dev/n64sdkmod/packages/libnusys/usr/lib/n64/nusys/
#	Install nualstl3 binaries and headers
	$(CP) nualstl3/include/nualstl.h /home/crash/n64dev/n64sdkmod/packages/libnusys/usr/include/n64/nusys
	$(CP) nualstl3/include/nualstl_n.h /home/crash/n64dev/n64sdkmod/packages/libnusys/usr/include/n64/nusys
	$(CP) nualstl3/build/libnualstl/*.a /home/crash/n64dev/n64sdkmod/packages/libnusys/usr/lib/n64/nusys/
	$(CP) nualstl3/build/libnualstl_d/*.a /home/crash/n64dev/n64sdkmod/packages/libnusys/usr/lib/n64/nusys/
	$(CP) nualstl3/build/libnualstl_n/*.a /home/crash/n64dev/n64sdkmod/packages/libnusys/usr/lib/n64/nusys/
	$(CP) nualstl3/build/libnualstl_n_d/*.a /home/crash/n64dev/n64sdkmod/packages/libnusys/usr/lib/n64/nusys/

clean:
	rm -rf build
	$(MAKE) -C nusys    clean 
	$(MAKE) -C nualsgi  clean
	$(MAKE) -C nualstl3 clean

