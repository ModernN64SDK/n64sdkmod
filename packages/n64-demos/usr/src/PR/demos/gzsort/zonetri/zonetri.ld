OUTPUT_ARCH (mips)

#include "system.h"

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
   
   BEGIN_SEG(code, 0x80000400) SUBALIGN(16)
   {
      BUILD_DIR/asm/entry.o(.start);
      BUILD_DIR/src/main*.o(.text);

      */libultra_rom.a:*.o(.text);
      */libgcc.a:*.o(.text);

      /usr/lib/n64/PR/rspboot.o(.text);
      /usr/lib/n64/PR/gspZSort.fifo.o(.text);

      /* data */
      BUILD_DIR/src/main*.o(.data*);

      */libultra_rom.a:*.o(.data*);

      /usr/lib/n64/PR/rspboot.o(.data*);
      /usr/lib/n64/PR/gspZSort.fifo.o(.data*);
      /* rodata */
      BUILD_DIR/src/main*.o(.*rodata*);

      */libultra_rom.a:*.o(.*rodata*);
      */libgcc.a:*.o(.*rodata*);
   }
   END_SEG(code)
   BEGIN_NOLOAD(code)
   {
      BUILD_DIR/src/main*.o(.*bss*);

      */libultra_rom.a:*.o(COMMON);
      */libultra_rom.a:*.o(.scommon);
      */libultra_rom.a:*.o(.*bss*);
   }
   END_NOLOAD(code)

   . = CFB_ADDRESS;

   BEGIN_NOLOAD(system_cfb)
   {
      BUILD_DIR/src/buffers/system_cfb.o(.bss*);
   }
   END_NOLOAD(system_cfb)
#ifdef RSPBUF_ADDRESS
   . = RSPBUF_ADDRESS;
#endif
   BEGIN_NOLOAD(system_rspbuf)
   {
      BUILD_DIR/src/buffers/system_rspbuf.o(.bss*);
   }
   END_NOLOAD(system_rspbuf)

   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
