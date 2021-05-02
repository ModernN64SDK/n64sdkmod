/*
	ROM spec file

	Main memory map

  	0x80000000  exception vectors, ...
  	0x80000400  zbuffer (size 320*240*2)
  	0x80025c00  codesegment
	      :  
  	0x8038F800  cfb 16b 3buffer (size 320*240*2*3)

	Copyright (C) 1997-1999, NINTENDO Co,Ltd.
*/

OUTPUT_ARCH (mips)

#include <nusys.h>

#define BEGIN_SEG(name, addr) \
    _##name##SegmentStart = ADDR(.name); \
    _##name##SegmentRomStart = __romPos; \
    .name addr : AT(__romPos)

#define END_SEG(name) \
    _##name##SegmentEnd = ADDR(.name) + SIZEOF(.name); \
    _##name##SegmentRomEnd = __romPos + SIZEOF(.name); \
    __romPos += SIZEOF(.name);

#define BEGIN_NOLOAD(name) \
    _##name##SegmentBssStart = ADDR(.name.noload); \
    .name.noload (NOLOAD) :

#define END_NOLOAD(name) \
    _##name##SegmentBssEnd = ADDR(.name.noload) + SIZEOF(.name.noload); \
    _##name##SegmentBssSize = SIZEOF(.name.noload);

SECTIONS
{
   __romPos = 0;

   BEGIN_SEG(boot, 0x04000000)
   {
      asm/rom_header.o(.text);
      boot.6102.o(.data);
   }
   END_SEG(boot)

   BEGIN_SEG(code, NU_SPEC_BOOT_ADDR) SUBALIGN(16)
   {
      /* text */
      asm/entry.o(.text);
      codesegment.o(.text);
      /usr/lib/n64/PR/rspboot.o(.text);
      /usr/lib/n64/PR/aspMain.o(.text);
      /usr/lib/n64/PR/n_aspMain.o(.text);
      /usr/lib/n64/PR/gspF3DEX2.fifo.o(.text);
      /usr/lib/n64/PR/gspL3DEX2.fifo.o(.text);
      /usr/lib/n64/PR/gspF3DEX2.Rej.fifo.o(.text);
      /usr/lib/n64/PR/gspF3DEX2.NoN.fifo.o(.text);
      /usr/lib/n64/PR/gspF3DLX2.Rej.fifo.o(.text);
      /usr/lib/n64/PR/gspS2DEX2.fifo.o(.text);

      /* data */
      codesegment.o(.data*);
      /usr/lib/n64/PR/rspboot.o(.data*);
      /usr/lib/n64/PR/aspMain.o(.data*);
      /usr/lib/n64/PR/n_aspMain.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.fifo.o(.data*);
      /usr/lib/n64/PR/gspL3DEX2.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.Rej.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.NoN.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DLX2.Rej.fifo.o(.data*);
      /usr/lib/n64/PR/gspS2DEX2.fifo.o(.data*);

      /* rodata */
      codesegment.o(.rodata*);
   }
   END_SEG(code)
   BEGIN_NOLOAD(code)
   {
      codesegment.o(COMMON);
      codesegment.o(.scommon*);
      codesegment.o(.bss*);
      . = ALIGN(0x8);
   }
   END_NOLOAD(code)

   BEGIN_SEG(stage00, _codeSegmentBssEnd) SUBALIGN(32)
   {
      _stage00SegmentTextStart = .;
      stage00.o(.text);
      _stage00SegmentTextEnd = .;
      _stage00SegmentDataStart = .;
      stage00.o(.data);
      _stage00SegmentDataEnd = .;
      stage00.o(.rodata*);
   }
   END_SEG(stage00)
   BEGIN_NOLOAD(stage00)
   {
      stage00.o(COMMON);
      stage00.o(.scommon*);
      stage00.o(.bss*);
      . = ALIGN(0x8);
   }
   END_NOLOAD(stage00)

   BEGIN_SEG(stage01, _codeSegmentBssEnd) SUBALIGN(32)
   {
      _stage01SegmentTextStart = .;
      stage01.o(.text);
      _stage01SegmentTextEnd = .;
      _stage01SegmentDataStart = .;
      stage01.o(.data);
      _stage01SegmentDataEnd = .;
      stage01.o(.rodata*);
   }
   END_SEG(stage01)
   BEGIN_NOLOAD(stage01)
   {
      stage01.o(COMMON);
      stage01.o(.scommon*);
      stage01.o(.bss*);
      . = ALIGN(0x8);
   }
   END_NOLOAD(stage01)

   BEGIN_SEG(sound_data, __romPos)
   {
      sound_data.o(.data);
   }
   END_SEG(sound_data)

   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
