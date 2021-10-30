------------------------------------------------------------------
NINTENDO64 NuSytem Sample nuhvq 1.02
Copyright (C) 1999 NINTENDO Co., Ltd.
Copyright (C) 1999 MONEGI CORPORATION
------------------------------------------------------------------

This directory contains a sample program that displays HVQ files 
with NuSystem.

The 32bit format does not work well with the GBI for S2DEX, so 
this version of the sample uses only the 16bit format.  To use 
the 32bit format, decode the data directly in the frame buffer, 
or divide the image data into strips and render by repeatedly 
using gSPTextureRectangle.
   
Also, note that this sample program will run as-is on 
NuSystem 2.05 (included in DevKit 4.2) and later versions, but 
it will not run on earlier versions. In order to run the sample 
on older versions of NuSystem, you must change the microcode's 
yield size from OS_YIELD_DATA_SIZE to HVQ2_YIELD_DATA_SIZE and 
rebuild NuSystem.


Controls:

Use the A Button to switch the picture being displayed
Use the B Button to switch the decode method


Audio is the same as sample nu4.