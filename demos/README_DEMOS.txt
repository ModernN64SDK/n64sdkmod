------------------------------------------------------------------
This document explains the sample programs.  

Please note the following points when executing make for these 
sample programs:
 
Executing make for these programs without arguments creates a ROM 
image file that contains debugging information.  This ROM image 
file runs on the debugger and not on the Nintendo 64 Control Deck. 

To create the ROM image file in a format that runs on Nintendo 64, 
execute make with the argument FINAL=YES (i.e., "make FINAL=YES"). 
However, when this is done, the ROM image file does not contain 
debug information, so it will not run normally on some debuggers, 
including PARTNER-N64.  This argument is invalid for sample 
programs such as "hostio" which are meant to run on a debugger.  

The following sample programs depend on files located in the 
Texture/  directory.  Please be careful when moving these sample 
programs.

        blockmonkey
        ddspgame
        ground
        lights
        mipmap
        spgame
        texlight
	threadmonkey

The following is an explanation of the functions and features of 
each sample program:

autofill:
This is a simple sample program that measures the performance of 
painting out the screen.  It paints out the screen using a polygon 
with different aspect ratio with size, and measures the speed 
automatically.  It is possible to see the measurement data using 
gnuplot.  For details, refer to README in the autofill sample 
program.

blockmonkey:
This is an application that measures a simple performance.  A user 
can change the size of a display list or parameters, and confirm 
the effect when rendering.  Operation instructions for the 
Controller are displayed on the host screen when the application is
executed. 

The graph of performance measurement on the lower screen shows the 
time needed to render a polygon.  The CPU processing time
needed to create a display list is not calculated.  This is not to 
slow down the animation even when it cannot catch up with the next
frame. 

bumpmap:
This is a sample program of bump mapping.  For details, refer to 
the README in the bump map sample program.

chrome:
This is a sample program for the reflection mapping function.

ci8fb:
This sample program is similar to autofill.  Rendering is 
processed using 8bit color index.  For details, refer to the
README for the ci8fb sample program. 

This sample program changes the number and size of all triangles
displayed on screen at the same time.

If compiled with no arguments attached to make, the following five
values are shown with osSyncPrintf():

    * The area of the displayed triangle
    * The size of the triangle along the x-axis
      relative to its size in the y direction.
    * The number of triangles rendered in one frame
    * The time to render one triangle (usec)
    * The number of pixels that can be rendered in one second

When "#define PERFORM_CHECK" is defined with fill.c, the triangles 
are rendered without changing their number and size.
 
If compiled with no arguments attached to make, the following three
values are shown with osSyncPrintf():

    * The time to render 10 triangles
    * The time to render the background
    * The sum of the two above times
 
Since a large quantity of character strings are displayed in this
case, PARTNER-N64 may hang up.  

ddspgame:
An N64 Disk Drive sample program.  It calls Disk functions while 
displaying on screen, displaying the results of function errors and
the corresponding error messages using 64DD internal ROM fonts.  
       
ddspgame can be run by executing 1 (debugging information file 
name) in the Partner-N64 Command window after compiling.
       
The default setting is to start ddspgame from Disk.  To start it 
from a Game Pak, use make as follows: make START_FROM_CASSETTE=YES.
      	
ddspgame is created in conformance with the error handling 
sequences described in Chapter 10 of 64DD Programming Manual.  If 
an "abnormal termination" error occurs during a sequence, 
subsequent processing is not performed.

Numbers displayed in the lower portion of the screen during and 
after command execution indicate the error number.  Although these
numbers should not be displayed by the game program, they are 
displayed in the sample program to allow monitoring of the types of
errors issued in this program.
       
Please also see ddspgame/README for a description of this program.

detail:
This is a sample program which explains how to use the multi 
tiling texture. This demonstrates the detail texture and MIP map 
texture as well.  

eeptest:
A simple program to test EEPROM (4k or 16k).   
When "DEBUG_EEPROM=YES" is specified when executing make,
a read/write to 16k areas are performed even for 4k EEPROM.  

fault:
A sample program to process CPU interrupts with the application
using OS_EVENT_FAULT events.   
When PARTNER-N64 reads the debug information file fault.out, the 
interrupt is trapped and reading is suspended at that point.  In
order to run this sample program normally, please set PARTNER-N64
to only read the ROM image file fault.n64.  To do this, use the 
PARTNER-N64 rd command as follows: 
	
    >rd fault.n64,0xb0000000
    >reset
    >g

If the debug information file is read before the above is executed,
then the debug information will become valid and the program will 
not operate normally.  In this case, even executing the INIT 
command will not help, and you will have to exit PARTNER-N64, start 
it up again and then read the ROM image file.  

flash:
This program executes tests for 1M Flash ROM. The basic controls 
are as follows:
    Control Pad Up/Down:  Select command
    A Button:  Execute command
    C Button Unit: Change each parameter

The following are the definition of each command.
    FLASH CHANGE    :   Switches the Flash being used
			Can specify from 0 to 3
    READ ID         :   Reads Flash ID
    READ STATUS     :   Reads the status of Flash
    CLEAR STATUS    :   Clears the status of Flash
    ALL ERASE       :   Deletes all the data of Flash
    SECTOR ERASE    :   Deletes a certain sector of Flash.
			The deleted sector is a sector which 
			contains the page specified by page number.
    READ ARRAY(DMA) :   Reads a specific page of Flash with DMA.
			The page to be read is the page length from
			the page specified by page number.
    WRITE BUFFER    :   Writes Flash buffer.
    WRITE ARRAY     :   Writes each page  of Flash from Flash 
			buffer.  The page to be read is the page 
			specified by page number.
    FILL WRITE DATA :   Creates the write data pattern of a page 
			length (1 PAGE) for the page specified by 
			page number. Page numbers are inserted in 
			the last 4 bytes of the data.
    CLEAR ALL       :   Deletes the data in the read buffer of the 
			test READ DATA program.
    CLEAR ALL       :   Deletes the data in the write buffer of the 
    WRITE DATA		test program.
    ALL ERASE &     :   Deletes, creates write data and writes the 
    FILL & WRITE	entire area of Flash.
    ALL READ        :   Reads data from the entire area of Flash. 
			Data is read per page.
    ALL COMPARE     :   Compares the entire content of the read 
			buffer and the write buffer of the test
			program.
    REGION COMPARE  :   Compares the content of the specified area
			of the read buffer and the write buffer of
			the test program.  The area to be compared
			is the page length from the page specified
 			number.
    ALL TEST        :   Executes ALL ERASE -> ALL FILL -> ALL WRITE
			-> ALL COMPARE as many times as specified.
    PAGE LENGTH     :   Specifies page length. Other than motion 
			experiment, specify 1 in principle.
    PAGE NUMBER     :   Specifies page number.


fogworld:
This is a sample program which shows how to use the fog process 
and the volume culling process.  This program enables you to 
output information such as clock speed of RCP or CPU and total
available DRAM capacity. 

gbpak:
A simple sample program for N64 Transfer Pak.  Reads data from Game 
Boy Game Pak (compatible with MBC1 only), and if an error 
occurrs, a corresponding message is displayed on the screen. 
By default, a game name and a company code are not included.  
Please rewrite COMPANY_CODE that is defined by siproc.h with 
GAME_TITLE and use it.  Please note that by default, when 
COMPANY_CODE is not defined, game name and company code are
not checked.  And, when it is desired to change the size of 
data to be read, please rewrite the value of DATA_SIZE (by
default, 0x8000) which is defined by main.h.  Though there is a
place called "compatible game" in the message that is displayed
on the screen, please display the specific game name for the 
actual game application.
If make LANG_ENGLISH=YES is entered when executing make, the
messages displayed on the screen will appear in English.

ginv:
This is a simple sample program which outputs such information as 
clock speed of RCP and CPU, and the total amount of available DRAM.

gl:
This is a game loader for Partner-N64.  ROM image can be read from 
command lines and executed.  See README.txt for details.
  
ground:	
This is a sample program which performs the clipping of a large 
ground plane accurately.

gs2dex:	
This is a sample program which explains how to use the sprite 
micro code S2DEX.

lights:	
This expresses specular highlight effect which uses RSP texture, 
and shadows (shades) falsely.  By using inputs from the 
Controller, it controls the location and size.  The background is 
processed with MIP map.

mipmap:	
This demonstrates MIP mapping.  It is possible to verify by 
comparing with other texture processing or changing MIP map
parameters.   

morphcube:	
CPU calculates each vertex for morphing.  CPU and the graphics 
pipeline execute 2 frames consecutively in parallel.  Tiling of a 
large MIP map texture is processed with a capacity that is stored 
in a texture memory.

morphfaces:	
Morphing of two dimensional images. CPU processes the morphing of 
vertex, and blending is performed using 2 cycle mode. For details, 
refer to NOTES in the morphfaces sample program.

motortest:
A sample Rumble Pak program.  For more information, see the
accompanying README file.

nosPak:
A sample program for the Controller Pak menu using N64 font 
data.  To execute make, please run the batch file makenos.bat. 

onetri:	
A simple sample program for rendering 1 polygon without Z buffer.  
When executed using "gload -a -t", simple texture mapping is
possible.

onetri-fpal:
A PAL version of onetri.  FPAL mode is used for full-screen display
of video in PAL format.

overlay:
A sample program that overlays onetri code (2 different segments 
containing code or data share the same memory area). 

pfs:		
A sample program for the Controller Pak.  You can experiment with 
checking, creating and deleting game notes.  The control operations
are expained below:

    Control Pad L/R ... Select game note to create 
    C Up            ... Check the file system
    C Down          ... Delete game note
    C Left          ... Create game note
    C Right         ... Format the Controller Pak
    A               ... Game note read test
    B               ... Game note write test
    START           ... Reread & display

This sample program differs from the Controller Pak menu provided 
with the application.  For a sample Controller Pak menu, see the 
provided nosPak sample program.  Please keep the following points 
in mind when using this sample program with a Controller Pak that 
contains game notes created with an actual game:  1) Game notes may
be destroyed by a write test or deletion, and 2) katakana (Japanese
script) is not supported; thus any portion of a note name rendered 
in katakana will not be displayed.

playseq:	
A sample program using the sequence player.  In the header 
file playseq.h, the following two types of MAX_VOICES are 
defined (inserted with #ifdef VOICES_16 - #else - #endif):

    #define MAX_VOICES 16; 
    #define MAX_VOICES 32;

MAX_VOICES indicates the number of physical voices of the 
synthesizer and the number of virtual voices of the sequence 
player.  The greater the number of voices, the greater the 
number of sounds that can be played simultaneously.  But this
also increases the amount of processing by the CPU and the RSP.
Please test both definitions to check the effect of the number 
of voices.  If compiled without any changes, the number of
virtual voices becomes 32.  

playseq.naudio:
The sample program playseq modified to support n_audio.

polyline:	
This renders by switching polygon and line microcodes.

reboot:  
A sample restart sequence program.  The program topgun, which is 
loaded on Game Pak, starts onetri, which has been written to Disk.  
       
Executing make creates the files dwrite, topgun.n64, reboot.out,
onetri.n64, hello.n64, and fat in the directory reboot/.  fat 
contains information on the size and storage location of the 
started game, dwrite is a batch file for writing Disk data, and 
reboot.out is a debugging information file for topgun.n64.  After 
writing data to Disk by entering <dwrite in the Command window, 
enter l reboot and g.

After these commands are executed, the topgun screen will be 
displayed. Press the A button.  A menu for confirming whether to 
perform a restart will then be displayed.  Select OK and press the 
A button to perform the restart.

To start the game directly from Disk, enter resetdd and g in the 
Command window.  In this case, the sample will display a simple 
message.   

The directory structure of reboot/ is as follows.

ddproc/
Modules for Disk Drive processing sequences, Disk processing and 
message display.  These are shared by the three applications 
described below. If an abnormal termination error is issued during 
a sequence, subsequent processing will not be performed.

topgun/
A Game Pak game that starts onetri.

onetri/
A game started by topgun.  It is written to Disk by dwrite starting
at LBA 50.
	 
hello_world/
A game written to the starting position of the Disk.

makefat/
A program that outputs the size of a specified file and its storage
location to a file.  This sample program outputs the size and disk 
storage location of onetri to the file fat.  fat is written to Disk
LBA 49 by dwrite and is referenced by topgun.
 
soundmonkey:
A sample program using both the sequence player and the sound 
player.  Only four virtual voices have been prepared for the
sequence player in this sample.  Thus, when MIDI data is played,  
virtual voice allocation fails, and the audio is not performed
normally. If the libgultra_d.a library is linked during make
(i.e., if the ERROR=YES argument is attached to execute 
"make ERROR=YES"), then numerous errors are output to the console.  
Since PARTNER-N64 does not support the routine used for outputting
these errors, a program created with the ERROR=YES argument 
attached  does not operate on PARTNER-N64 at all.  If using 
PARTNER-N64, please do not execute make with the ERROR=YES 
argument.
 
This program was created like this on purpose.  It is not a bug.  

spgame:	
A sample program which renders objects in the foreground and the 
background using a sprite function.

spritemonkey:
A sample program which demonstrates the use of sprite microcode 
Sprite2D.
  
spritemonkeyEX:
A sample program which uses sprite microcode S2DEX2.  
Since the current S2DEX2 microcode is not displayed correctly when 
32bit texture is used, the emulation function guS2DEmuBgRect1Cyc() 
is used for this texture mode to display. 

sramtest:	
Tests for the operation of 256K SRAM.  The execution result 
is displayed with osSyncPrintf().  The 256K SRAM board is required
for execution of this sample program. 

terrain:	
This is a sample program for the creation of geographical 
features.  This renders the geometry using N64 hardware, and 
operates the interface using the Controller. 

texlight:	
A sample program which expresses a diffuse lighting process, a 
specular highlight process, and objects which are texture mapped 
at the same time.   

threadmonkey:
A sample program which shows how to use the thread profiler.   
The routine to call functions for the thread profiler and to 
display results is included in the part of block.c of blockmonkey.
It is changed to display the number of operations and operation 
time for each thread after normal operating mode and debug print of
key operation are displayed.

tile_rect2d:
This demonstrates in a simple way how to divide a large texture 
into small texture tiles so it can be stored in texture memory.    
It creates two dimensional texture rectangle primitives.

tile_rect3d:
This explains how to divide a large texture into tiles so it can 
be stored in texture memory.  As filtering is processed 
properly, there will be no joints.  A triangle will be created.

topgun:
The fighter plane that appears in this sample program is data in 
.flt format modeled by MultiGen.  The display list is created by 
the geometry converter flt2c, which is included as a sample 
program.

tron:		
A wall is a transparent object which is not sort processed. It 
demonstrates how to use the BOWTIE_VAL parameter, which avoids RDP 
hanging. 

voice:
A sample program for speech recognition that uses library functions
for speech recognition systems.  Speech can be displayed to the 
screen by connecting the speech recognition system to the 
Controller port and speaking into the microphone. For more 
information, see the accompanying README file.

**Note: Currently this demo is made only for the Japanese language.