------------------------------------------------------------------
N64-HVQM2 library  version 1.2 (PC)
Explanation of the Sample Program
------------------------------------------------------------------


Summary
=======

This program is the N64-HVQM2 sample program.  It includes a 
timekeeper program that displays video records in synchronization 
with the audio while playing back audio records without 
interruption from the HVQM2 data.  See the comments in the source 
for details about the functions and use of the timekeeper.

Create Program
==============

This sample program assumes a development environment with N64OS 
2.0I Patch 5 as well as exeGCC N64 for PC (1998/10/21 release 
version).

1) Please set the following directories in the HVQMLIBDIR, 
   HVQMINCDIR and HVQMUCODEDIR macros in Makefile: the directory  
   holding the library (libhvqm2.a), the directory holding the  
   HVQM2 include files (hvqm2dec.h, adpcmdec.h, HVQM2File.h,  
   HVQmd.h), and the directory holding the HVQM2 microcode  
   (hvqm2sp*.o).

2) By setting the USE_RSP, SYNC_VIDEO, CFB_FORMAT, MAXWIDTH and  
   MAXHEIGHT macro definitions in Makefile, select whether to use 
   the RSP microcode (or to use only the CPU to decode video),  
   whether to force synchronization when video playback is  
   delayed relative to the audio playback, whether to expand and  
   display the images in 16bit RGBA or 32bit RGBA, and to  
   determine the upper-limit size of an image to decode.  Adjust  
   these settings to meet your needs.

3) Execute make. This creates sample.n64 (as well as sample.out).


How to Use
==========

By loading and executing the created program on the NINTENDO64,  
the HVQM2 data (video clip) that is linked to sample.n64 will be 
played repeatedly.


How to Link the HVQM2 Data
==========================

In this sample program, the HVQM2 file sample.hvq is included to 
the segment called hvqmdata in the spec file and linked by mild.  
Then with main.c the hvqmdata segment address is referenced and 
the data is sent to RDRAM.

This program can use HVQM2 files that have been created by 
hvqm2enc (version 1.2 and later).

If the error message "ERROR: hvqbuf insufficient" or "ERROR: 
adpcmbuf insufficient" or "ERROR: hvq_spfifo insufficient" is 
displayed when a newly created HVQM2 file is linked to this 
program and executed, adjust the definitions of HVQ_DATASIZE_MAX 
or AUDIO_RECORD_SIZE_MAX or HVQ_SPFIFO_SIZE in system.h to 
conform with the HVQM2 file.