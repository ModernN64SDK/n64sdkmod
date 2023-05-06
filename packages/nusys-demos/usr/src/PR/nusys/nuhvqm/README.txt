------------------------------------------------------------------
nuhvqm
Ver.0.1     Created by Yutaka Murakami 1999 (c) Nintendo Co.,LTD.
Ver.0.2-1.0 Modified by Yutaka Murakami 1999 (c) Nintendo Co.,LTD.
------------------------------------------------------------------

Explanation
===========

This sample program introduces one way to handle hvqm on 
NuSystem.  The RSP must beused with the current version.

The program will run as-is on NuSystem2.05 (released 6/15) and 
later versions, but it will not run on earlier versions.  In 
order to run the sample on older versions of NuSystem, you must 
change the microcode's yield size from OS_YIELD_DATA_SIZE to 
HVQ2_YIELD_DATA_SIZE and rebuild NuSystem.


Capabilities
============

It is assumed that hvqm will only be used in certain screens in a 
game.  By pushing the START button of Controller 1 you can switch 
between stage00, which does not use hvqm, and stage01, which 
does.  

It is also assumed that hvqm will be stopped in midstream and 
restarted sometimes. In stage01, hvqm playback will start from 
the beginning by pressing the A Button of Controller 1.  If you 
substitute data here, different data can be played back.


File
====

Concerning the location of the hvqm-related header file and 
library and the sample HVQM data, it is presumed that the hvqm 
package was copied over as-is with the same directory structure.  If the locations are different, you should appropriately rewrite Makefile and the spec file.


Points to Note
==============


(Regarding Video)

The video display position is set for the center in hvqmStart.  
If you want to adjust the position, you can do this by changing 
the value of the hvqmOffset variable.  However, you must not 
make any change if hvqmVideoMgr has already been executed, even 
once.  (Though it is possible to shift the timing of the drawing 
of key frames.)

Also, you must not overwrite with polygons etc. inside the video 
display area.  And you must not clear the preceding image until 
hvqmVideoMgr creates the new screen.  

For frames that do not require an image update, hvqmVideoMgr 
does nothing.  It does not even swap the frame buffer.  If 
HVQM_PERMIT_OTHER_GRAPHICS is TRUE, the HVQM image previously 
used gets copied to the current frame buffer.  If you wish to 
place other images around the HVQM image, 
HVQM_PERMIT_OTHER_GRAPHICS should be TRUE. A microcode used for 
copying can be changed by HVQM_USE_SPRITE_UCODE.  S2DEX2 
requires a smaller display list, which saves memory consumption. 
F3DEX2 saves S2DEX2-equivalent amount of memory unless S2DEX2 is 
used in other parts.  Aside from this, it is a matter of 
preference.


(Regarding Audio)

The dedicated hvqm audio manager is running during hvqm playback 
so you must not play other sounds.  If the playback of other 
sounds is necessary, get rid of the audio inside the hvqm data 
and comment out hvqmAudioInit, which is called inside 
hvqmVideoMgr in hvqmmain.c.  If you match the timing of the 
audio you want to play with hvqm the same as you would the 
normal audio there should be no problem.  Of course, you could 
also play the audio-containing hvqm file without using the audio 
manager (though this is a waste of the data area).  

The dedicated hvqm audio manager (thread) only exists during 
playback (it runs from hvqmStart until the first hvqmVideoMg is 
called), and it is automatically deleted at the time the movie 
ends.


Other Details
=============

For other detailed matters, see the comments in the source.
Be sure to read hvqm.h for various settings.



Revision History
================
v1.0
-- Fixed a problem where frame buffers would not get updated 
   regardless of HVQM_PERMIT_OTHER_GRAPHICS, when finished  
   playing images and while waiting to complete. 

-- Added volatile to hvqmStatus, just in case.
   Other slight modifications and general changes.

v0.9
-- hvqmCopyFrameBuffer is included in a separate file.  Also,  
   32bit RGBA is supported. Not only for S2DEX2 microcode, but  
   for F3DEX2 microcode as well. The process speed hasn't  
   changed.

v0.8
-- Sometimes unexpected noise would occur, depending upon the  
   timing of the switching of frame buffers and launching  
   hvqmVideoMgr. Now this can be checked by registering callback  
   functions using nuGfxSwapCfbFuncSet and by switching the  
   HVQM_STATUS_WAIT_SWAPBUFFER flag.  In general, it is  
   necessary to be aware of whether it is before or after  
   switching occurs, when the address of the frame buffer is  
   used in a display list.

v0.7
-- If HVQM_PERMIT_OTHER_GRAPHICS is TRUE, an image gets copied  
   for a frame that doesn't need an image update.  To be exact,  
   the job was completed by not switching (doing nothing) frame  
   buffers regardless of the arguments used for hvqmVideoMgr  
   when images don't need to get updated.  However, by using  
   this function, it can be used as if you were executing  
   nuGfxStartTask.  To utilize this function, initializing a  
   frame buffer is needed before hvqmVideoMgr.

-- Fixed so that the extra area for disabling cache is added  
   when HVQM_CFB_FORMAT is 2.

-- Fixed a problem in deciding key frames which occured when  
   fast forwarding images in v0.5.

-- Writing back the cache is performed inside nuGfxTaskStart.   
   So this has been removed.

-- Disabling cache could be located anywhere, however, it is  
   located just before the memory reference.

-- Unused nuGfxTaskAllEndWait() has been removed.

v0.6
-- Fixed a problem where a fatal error would occur if the last  
   frame is skipped. Also, a change has been made so that a  
   message gets displayed in the console when skipped. The last  
   frame can be decoded by setting HVQM_LAST_FRAME to TRUE.

v0.5
-- Released in ntsc-online.
32-bit RGBA output is now supported, which can be set  
   withHVQM_CFB_FORMAT. Please edit hvqmgfxinit.c to change the  
   settings for the frame buffer area. Fixed a problem where  
   forwarding would go to the non-key frame when the output of  
   an image is delayed.

v0.4

-- Changed it so the audio thread is not started when the data  
   does not contain audio. 


v0.3
	
-- Made it possible to enable/disable frame buffer swapping by  
   setting an argument in hvqmVideoMgr.  For the argument you  
   can specify either NU_SC_SWAPBUFFER or NU_SC_NOSWAPBUFFER.

Incidentally, NU_SC_NORDP etc. are set automatically.  At the  
   present time, even if NU_SC_SWAPBUFFER is specified, when the  
   HVQM data is HVQM2_VIDEO_HOLD the frame buffer will not be  
   swapped, so if you need buffer swapping you will have to make  
   modifications.


v0.2

-- In NuSystem Ver.2.05 the size of the yield buffer was  
   increased, so changes were made to make use of the original  
   NuSystem code (nuGfxTaskStart, etc.) for processes relating  
   to task activation.  To be more specific, HVQM microcode was  
   added to nuGfxInit and the name was simply changed to  
   hvqmGfxInit.


v0.1

-- First release.
With the HVQM microcode, the yield buffer needed to be larger  
   than the default value, so the processes relating to the  
   NuSystem task manager were included into hvqmmain.c and the  
   necessary settings were changed.