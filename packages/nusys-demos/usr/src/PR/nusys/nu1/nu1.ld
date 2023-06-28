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

/* Starts a segment */
#define BEGIN_SEG(name, addr) \
    _##name##SegmentStart = ADDR(.name); \
    _##name##SegmentRomStart = __romPos; \
    .name addr : AT(__romPos)

/* Ends a segment */
#define END_SEG(name) \
    _##name##SegmentEnd = ALIGN(ADDR(.name) + SIZEOF(.name),0x10); \
    _##name##SegmentRomEnd = _##name##SegmentEnd - _##name##SegmentStart + _##name##SegmentRomStart; \
    __romPos = _##name##SegmentRomEnd; \
    . = _##name##SegmentEnd;

/* Starts a noload segment which is intended to contain variables which do not have data yet. */
#define BEGIN_NOLOAD(name) \
    _##name##SegmentBssStart = ADDR(.name.noload); \
    .name.noload (NOLOAD) :

/* Ends a noload segment */
#define END_NOLOAD(name) \
    _##name##SegmentBssEnd = ALIGN(ADDR(.name.noload) + SIZEOF(.name.noload),0x10); \
    _##name##SegmentBssSize = _##name##SegmentBssEnd - _##name##SegmentBssStart; \
    . = _##name##SegmentBssEnd;

SECTIONS
{
   __romPos = 0;

   BEGIN_SEG(boot, 0x04000000)
   {
      BUILD_DIR/asm/rom_header.o(.text); /* ROM Header */
      BUILD_DIR/boot.6102.o(.data); /* CIC 6102 bootcode */
   }
   END_SEG(boot)

   BEGIN_SEG(code, NU_SPEC_BOOT_ADDR) SUBALIGN(16)
   {
      /* text */
		BUILD_DIR/asm/entry.o(.start);
      /usr/lib/n64/nusys/nusys_isv.o(.text);
      BUILD_DIR/src/main*.o(.text);

      */libultra_d.a:*.o(.text);
      */libnusys_d.a:*.o(.text);
      */libgcc.a:*.o(.text);
      /usr/lib/n64/PR/rspboot.o(.text);
      /usr/lib/n64/PR/gspF3DEX2.fifo.o(.text);
      /usr/lib/n64/PR/gspL3DEX2.fifo.o(.text);
      /usr/lib/n64/PR/gspF3DEX2.Rej.fifo.o(.text);
      /usr/lib/n64/PR/gspF3DEX2.NoN.fifo.o(.text);
      /usr/lib/n64/PR/gspF3DLX2.Rej.fifo.o(.text);
      /usr/lib/n64/PR/gspS2DEX2.fifo.o(.text);

      /* data */
      /usr/lib/n64/nusys/nusys_isv.o(.data*);
      BUILD_DIR/src/main*.o(.data*);

      */libultra_d.a:*.o(.data*);
      */libnusys_d.a:*.o(.data*);
      /usr/lib/n64/PR/rspboot.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.fifo.o(.data*);
      /usr/lib/n64/PR/gspL3DEX2.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.Rej.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.NoN.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DLX2.Rej.fifo.o(.data*);
      /usr/lib/n64/PR/gspS2DEX2.fifo.o(.data*);

      /* rodata */
      /usr/lib/n64/nusys/nusys_isv.o(.*rodata*);
      BUILD_DIR/src/main*.o(.*rodata*);

      */libultra_d.a:*.o(.*rodata*);
      */libnusys_d.a:*.o(.*rodata*);
      */libgcc.a:*.o(.*rodata*);
   }
   END_SEG(code)
   BEGIN_NOLOAD(code)
   {
      /usr/lib/n64/nusys/nusys_isv.o(COMMON);
      /usr/lib/n64/nusys/nusys_isv.o(.scommon);
      /usr/lib/n64/nusys/nusys_isv.o(.*bss*);
      BUILD_DIR/src/main*.o(.*bss*);

      */libultra_d.a:*.o(COMMON);
      */libultra_d.a:*.o(.scommon);
      */libultra_d.a:*.o(.*bss*);
      */libnusys_d.a:*.o(COMMON);
      */libnusys_d.a:*.o(.scommon);
      */libnusys_d.a:*.o(.*bss*);
   }
   END_NOLOAD(code)
   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
