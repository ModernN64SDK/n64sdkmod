------------------------------------------------------------------
							  4/9/1999
------------------------------------------------------------------

About NuSystem for Sound Tools Ver3.0	


[1]  Installation
First, confirm that the libraries and headers in the lib 
directory of MUS Library Ver. 3 have been copied to the desired 
location. 

Then, move to the nualstl3 directory and enter the following 
command to install the NuSystem library.  

  make -f makefile install

This will install the NuSystem for libmus 3.0 in the location 
specified by N64KITDIR in makefile (default is 
c:\nintendo\n64kit). 

[2]  How to Compile 
When reconstructing the nualstl3 library, please use the 
following procedure. 

Normally, since the NuSystem source is installed at 

  X:\nintendo\n64kit\nusys\src

(X: is the drive on which NuSystem has been installed.)

as the compressed file nusys-120.lzh, decompress the file at 
that location copy nualstl3 to a directory under the 
"nusys-1.20" directory created by the decompression. 

This can then be compiled by entering 

  >make -f makefile

(Note) 
Libraries associated with N64 Sound Tools (libgmus.a, 
libn_gaudio_sc.a, etc.) are compatible with OS 2.01 Patch 5 and 
later.  When OS 2.01 Patch 4 or an earlier version  running, the 
libraries must be reconstructed.  Remove the comments from the 
following line in the respective source files and compile. 

    OLD_AUDIO_DEF = -D_OLD_AUDIO_LIBRARY


[3]  Changes to NuSystem

Most of the functions libmus 3.0 and their operation have been 
changed from the previous version. However, the differences have 
been absorbed on the NuSystem side as much as possible. Also, 
libraries for the audio microcode and the n_audio microcode can 
now be generated from the same source. 


Overview of Changes

An mus library for NuSystem used to be required, but the 
scheduler can now be changed from libmus 3.0.  Therefore,  the 
program has been changed so that libmus can be used as it is.  
However, until now, the mus library was changed so that the 
areas for the sound effects bank and/or binary songs, etc. were 
reserved from the audio heap area.  This can not be done when 
libmus is used unaltered.  The __MusInitMemMalloc() function of 
the libmus internal functions is used to resolve this. 

Since sound effects from libmus 3.0 are now handled as effect 
bank files and their functions have also been changed, changes 
were made on the NuSystem side. 

The functions which have been changed and the newly added 
functions are listed below. 


Changed Functions 

nuAuStlinit()
The members of the argument musConfig, which until now were 
virtually unused, are now all used.  Therefore, please set all 
members to their default settings.  However, the default value 
(auConfig) is used when NULL is specified. 

nuAuStlMgrInit()
The existing initialization for DMA call back and initialization 
of the sequencer have been deleted so that registration of the 
scheduler and initialization of mus is performed. In addition, 
the argument has been changed to a pointer for the musConfig 
structure. 


Added Functions 

nuAuStlSeqPlayerInit()
This has been added due to changes in the handling of sequences 
and sound effects. This maintains a sequence (binary song) area 
from the audio heap area.  Consequently, use size to specify the 
maximum number of sequence files to be used in the game. 

nuAuStlSndPlayerInit()
This has been added because sound effects have been stored in 
separate files.  This maintains a sound effect (binary song 
effects bank) area from the audio heap area.  Consequently, use 
size to specify the maximum number of sound effect files to be 
used in the game. 



Deleted Functions 

nuAuStlPlayerInit()
nuAuStlPlayerInit() has been changed to nuAuStlSeqPlayerInit().  
In addition, MusInitialize() used to be called by this function, 
but now it is called by nuAuStlMgrInit().  nuAuStlPlayerInit() 
is defined as nuAuStlSeqPlaerInit() using a macro. 

Other 
Volume, pan, and tempo specifications are defined by macros, but 
functions which do not exist in libmus 3.0 cannot be used.  In 
addition, please note that none have been newly created. 
