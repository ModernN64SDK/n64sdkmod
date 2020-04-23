#
# Please refer to /usr/include/make/ 
# 

#!smake
include $(ROOT)/usr/include/make/PRdefs

TARGET = dwrite reboot.n64

ifeq ($(FINAL),YES)
	ARGS = FINAL=YES
endif

default:$(TARGET)

fat : .\makefat\makefat.exe onetri.n64
	.\makefat\makefat.exe 0 50 onetri.n64

# Select Makefile depending on used compiler
#.\makefat\makefat.exe :
#	echo cd .\makefat > temp.bat
#	echo make -f Makefile.borland  >> temp.bat
#	echo cd ..        >> temp.bat
#	temp.bat	

onetri.n64 : .\onetri\onetri.n64
	copy .\onetri\onetri.n64 .

.\onetri\onetri.n64 :
	echo cd .\onetri  > temp.bat
	echo make $(ARGS) >> temp.bat
	echo cd ..        >> temp.bat
	temp.bat	

hello.n64 : .\hello_world\hello.n64
	copy .\hello_world\hello.n64 .

.\hello_world\hello.n64 :
	echo cd .\hello_world  > temp.bat
	echo make $(ARGS) >> temp.bat
	echo cd ..        >> temp.bat
	temp.bat	

reboot.n64 : .\topgun\topgun.n64
	copy .\topgun\topgun.n64 reboot.n64
	copy .\topgun\topgun.out reboot.out

.\topgun\topgun.n64 :
	echo cd .\topgun  > temp.bat
	echo make $(ARGS) >> temp.bat
	echo cd ..        >> temp.bat
	temp.bat	

dwrite :  fat onetri.n64 hello.n64
	echo leowrite  -L 49 -t 0 -I id.dat fat > dwrite
	type .\onetri\gwrite  >> dwrite
	type .\hello_world\gwrite >> dwrite

clean clobber :
	echo cd ddproc > temp.bat
	echo make clobber >> temp.bat
	temp.bat
	echo cd topgun > temp.bat
	echo make clobber >> temp.bat
	temp.bat
	echo cd onetri > temp.bat
	echo make clobber >> temp.bat
	temp.bat
	echo cd hello_world > temp.bat
	echo make clobber >> temp.bat
	temp.bat
#	echo cd makefat > temp.bat
#	echo make -f Makefile.borland clean >> temp.bat
#	temp.bat	

	del temp.bat
	del *.n64
	del *.out
	del fat
	del dwrite
