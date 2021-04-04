------------------------------------------------------------------
N64-HVQ2 library  version 1.2 (PC)
Explanation of the sample program
------------------------------------------------------------------

Summary
=======
This program is the N64-HVQ2 (PC version) sample program.


Create Program
==============

This sample program can be compiled with exeGCC for the PC.

1) Please set the following directories in the HVQLIBDIR, 
HVQINCDIR and HVQUCODEDIR macros in Makefile: the directory 
holding the library (libhvq2.a), the directory holding the HVQ2 
include files (hvq2dec.h, HVQ2File.h, HVQmd.h), and the 
directory holding the HVQ2 microcode (hvq2sp*.o).

2) By setting the USE_RSP, CFB_FORMAT, MAXWIDTH and MAXHEIGHT 
macro definitions in Makefile, select whether to use the RSP 
microcode (or to use only the CPU to decode images), whether to 
expand and display the images in 16bit RGBA or 32bit RGBA, and 
to determine the upper-limit size of an image to decode.  Adjust 
these settings to meet your needs.

3) Execute make. This creates sample.n64 (as well as sample.out).


How to Use
==========
    
By loading and executing the created program on the Nintendo 64, 
the compressed image data that is linked to sample.n64 will be 
decoded and displayed, after which the system enters the idle 
state.
    

How to Link HVQ2 Compressed Image Data
======================================

In this sample program, the HVQ2 file sample.hvq is included to 
the segment called hvqdata in the spec file and linked by mild.  
Then with main.c the hvqdata segment address is referenced and 
the data sent to RDRAM.

This program can use HVQ2 files that have been created by 
hvq2enc (version 2.6 and later).

If the error message "ERROR: HVQbuf insufficient" or "ERROR: 
hvq_spfifo insufficient" is displayed when a newly created HVQ2 
file is linked to this program and executed, adjust the 
definitions of HVQ_DATASIZE_MAX or HVQ_SPFIFO_SIZE in system.h 
to conform with the HVQ2 file.