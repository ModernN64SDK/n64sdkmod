------------------------------------------------------------------
12/7/98

In this directory, sample source is used to explain the 
procedure for customizing graphic initialization.  As an 
example, nu1 is changed so that only F3DEX2 of the F3DEX2 
microcode and S2DEX2 are used. 

The areas which have changed are listed below.  For details, 
please refer "Customizing Graphics," in the NuSystem Manual.

1. makefile
Because F3DEX2 microcode is being used, change makefile so 
F3DEX_GBI_2 is defined when the source is compiled. 

LCDEFS = -DF3DEX_GBI_2

Add appgfxinit.c, which is created in step 3 below, to the 
CODEFILES definition. 


2. spec
The "include" of the code segment is changed to only the 
microcode that is being used.  (In actual fact, S2DEX2 is not 
used in nu1, but for this explanation we will presume that it is 
used.)

	:
	include "codesegment.o"
	include "/usr/lib/n64/PR/rspboot.o"
	include "/usr/lib/n64/PR/gspF3DEX2.fifo.o"
	include "/usr/lib/n64/PR/gspS2DEX2.fifo.o"
endseg
	:

3. appgfxinit.c
Create appGfxInit(), the customized version of the nuGfxInit() 
function.
Copy nugfxinit.c from the NuSystem src directory, rename it 
appgfxinit.c, and then make the following changes: 

#include "graphic.h"

	:

static NUUcode nugfx_ucode[] = {
    {(u64*)gspF3DEX2_fifoTextStart, (u64*)gspF3DEX2_fifoDataStart}, /*0*/
    {(u64*)gspS2DEX2_fifoTextStart, (u64*)gspS2DEX2_fifoDataStart}, /*1*/
};

	:

    nuGfxTaskStart(gfxList,
		   (s32)(gfxList_ptr - gfxList) * sizeof (Gfx),
		   APP_GFX_UCODE_F3DEX2 , 0);
	:

(Caution) If the NuSystem debug function nuDebXXXX() is being 
used, make sure that the microcode at the head of nugfx_ucode[] 
is F3DEX (or F3DEX2) microcode.  Since the debug function 
assumes that the microcode at the start of the nugfx_ucode array 
is F3DEX microcode, it does not operate correctly when the head 
has S2DEX microcode. 

4. Define the microcode names. 

Define the microcode names in graphic.h in order corresponding 
to nugfx_ucode[] in appgfxinit.c.  This is used for the third 
argument of the nuGfxTaskStart function.

The names are arbitrary.  But since they are self-defined, 
APP_ is appended to the start of the names to distinguish them 
from the ones defined in nusys.h. 

#define	APP_GFX_UCODE_F3DEX2	0	/* F3DEX2 microcode */
#define	APP_GFX_UCODE_S2DEX2	1	/* S2DEX2 microcode */


5. Other source changes. 

Change the third argument of the nuGfxTaskStart function to the 
microcode name defined in step 3. 

Change the graphic initializer nuGfxInit() to appGfxInit().
Add the prototype declaration:
                  void  appGfxInit(void);

