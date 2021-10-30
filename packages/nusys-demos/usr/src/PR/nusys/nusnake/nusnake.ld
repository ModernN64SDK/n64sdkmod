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
    _##name##SegmentEnd = ADDR(.name) + SIZEOF(.name); \
    _##name##SegmentRomEnd = __romPos + SIZEOF(.name); \
    __romPos += SIZEOF(.name);

/* Starts a noload segment which is intended to contain variables which do not have data yet. */
#define BEGIN_NOLOAD(name) \
    _##name##SegmentBssStart = ADDR(.name.noload); \
    .name.noload (NOLOAD) :

/* Ends a noload segment */
#define END_NOLOAD(name) \
    _##name##SegmentBssEnd = ADDR(.name.noload) + SIZEOF(.name.noload); \
    _##name##SegmentBssSize = SIZEOF(.name.noload);

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
      /usr/lib/n64/nusys/nusys_rom.o(.text);
      BUILD_DIR/src/main*.o(.text);

      */libultra_rom.a:*.o(.text);
      */libnusys.a:*.o(.text);
      */libnualsgi_n.a:*.o(.text);
      */libn_audio.a:*.o(.text);
      */libgcc.a:*.o(.text);
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
      /usr/lib/n64/nusys/nusys_rom.o(.data*);
      BUILD_DIR/src/main*.o(.data*);

      */libultra_rom.a:*.o(.data*);
      */libnusys.a:*.o(.data*);
      */libnualsgi_n.a:*.o(.data*);
      */libn_audio.a:*.o(.data*);
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
      /usr/lib/n64/nusys/nusys_rom.o(.*rodata*);
      BUILD_DIR/src/main*.o(.*rodata*);

      */libultra_rom.a:*.o(.*rodata*);
      */libnusys.a:*.o(.*rodata*);
      */libnualsgi_n.a:*.o(.*rodata*);
      */libn_audio.a:*.o(.*rodata*);
      */libgcc.a:*.o(.*rodata*);
   }
   END_SEG(code)
   BEGIN_NOLOAD(code)
   {
      /usr/lib/n64/nusys/nusys_rom.o(COMMON);
      /usr/lib/n64/nusys/nusys_rom.o(.scommon);
      /usr/lib/n64/nusys/nusys_rom.o(.*bss*);
      BUILD_DIR/src/main*.o(.*bss*);

      */libultra_rom.a:*.o(COMMON);
      */libultra_rom.a:*.o(.scommon);
      */libultra_rom.a:*.o(.*bss*);
      */libnusys.a:*.o(COMMON);
      */libnusys.a:*.o(.scommon);
      */libnusys.a:*.o(.*bss*);
      */libnualsgi_n.a:*.o(COMMON);
      */libnualsgi_n.a:*.o(.scommon);
      */libnualsgi_n.a:*.o(.*bss*);
      */libn_audio.a:*.o(COMMON);
      */libn_audio.a:*.o(.scommon);
      */libn_audio.a:*.o(.*bss*);
   }
   END_NOLOAD(code)

   BEGIN_SEG(moviedata, .)
   {
      BUILD_DIR/assets/movieimg.o(.data);
   }
   END_SEG(moviedata)

   BEGIN_SEG(sound_data, __romPos)
   {
      BUILD_DIR/sound/sound_data.o(.data);
   }
   END_SEG(sound_data)

   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
