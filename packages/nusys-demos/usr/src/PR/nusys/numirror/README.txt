------------------------------------------------------------------
NuSystem Sample Program
------------------------------------------------------------------

1. Compiling

Currently provided as samples in the numirror directory are 2q
types of makefiles.  Executing these makefiles compiles all the 
samples. The following is a description of the makefiles. 

  Makefiles in the numirror directory

	Makefile.win.64s	(makefile for the N64 Sound 
				Tools, PC version)
	Makefile.win.nualsgi	(makefile for the SGI Sound  
				Tools, PC version) 


  Compiling

	From the MS-DOS prompt (MS-DOS or UNIX), move to the 
	numirror directory.  For each version, executing the 
	following commands from the command shell compiles all 
	programs in the numirror directory and below.

		make -f Makefile.win.64s
		make -f Makefile.win.nualsgi

2. Miscellaneous

Note that the N64 Sound Tools System must be purchased separately 
to create applications supported by the N64 Sound Tools.  It is 
not necessary to purchase a separate toolkit for SGI Sound 
Library support.

3. NOTE FROM CrashOveride

This demo port is NOT COMPLETE. In particular the 64s rom breaks. I don't know why this is. nualsgi works 100% however. 
If you can get 64s working PLEASE let me know.
