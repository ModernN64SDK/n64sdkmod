#include "simple.h"

/*
 * ROM spec file
 */

/* 
   Main Memory map for simple:

  0x000000	exception vectors, ...

  0x000400	code segment
		code, data, bss (includes audio heap, and
		gfx dynamic area)


  0x00????	gfxdlists segment
(after code	space for static gfx display lists
  segment)


		empty


  0x1da800	zbuffer segment
		The Z buffer

  0x200000	cfb segment
		The two color framebuffers

  0x24b000	empty


*/
beginseg
	name "code"
	flags BOOT OBJECT
	entry boot
	stack bootStack + STACKSIZEBYTES
	include "codesegment.o"
	include "/usr/lib/n64/PR/rspboot.o"
	include "/usr/lib/n64/PR/gspF3DEX.fifo.o"
	include "/usr/lib/n64/PR/gspF3DEX2.fifo.o"
	include "/usr/lib/n64/PR/gspF3DEX2.xbus.o"
	include "/usr/lib/n64/PR/gspF3DEX2.NoN.fifo.o"
	include "/usr/lib/n64/PR/gspF3DEX2.NoN.xbus.o"
	include "/usr/lib/n64/PR/gspF3DEX2.Rej.fifo.o"
	include "/usr/lib/n64/PR/gspF3DEX2.Rej.xbus.o"
	include "/usr/lib/n64/PR/gspF3DLX2.Rej.fifo.o"
	include "/usr/lib/n64/PR/gspF3DLX2.Rej.xbus.o"
	include "/usr/lib/n64/PR/aspMain.o"
endseg

/* area for static gfx display lists */
beginseg
	name "gfxdlists"
	flags OBJECT
	after code
	include "gfxdlists.o"
endseg

beginseg
	name "zbuffer"
	flags OBJECT
	address 0x801da800
	include "gfxzbuffer.o"
endseg

beginseg
	name "cfb"
	flags OBJECT
	address 0x80200000
	include "gfxcfb.o"
endseg

beginseg
	name "static"
	flags OBJECT
	number STATIC_SEGMENT
	include "gfxinit.o"
	include "gfxstatic.o"
endseg

beginseg
	name "dynamic"
	flags OBJECT
	number DYNAMIC_SEGMENT
	include "gfxdynamic.o"
endseg

beginseg
	name "bank"
	flags RAW
	include "/usr/lib/n64/PR/soundbanks/GenMidiBank.ctl"
endseg

beginseg
	name "table"
	flags RAW
	include "/usr/lib/n64/PR/soundbanks/GenMidiBank.tbl"
endseg

beginseg
	name "seq"
	flags RAW
	include "/usr/lib/n64/PR/sequences/BassDrive.seq"
endseg

beginwave
	name "simple"
	include "code"
	include "gfxdlists"
	include "static"
	include "dynamic"
	include "cfb"
	include "zbuffer"
	include "table"
	include "bank"
	include "seq"
endwave
