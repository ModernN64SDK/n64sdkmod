OUTPUT_ARCH (mips)

#include "system.h"

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

   BEGIN_SEG(code, 0x80000400) SUBALIGN(16)
   {
		BUILD_DIR/asm/entry.o(.start);
      BUILD_DIR/src/main*.o(.text);

      */libultra_rom.a:*.o(.text);
      */libhvq2.a:*.o(.text);
      */libgcc.a:*.o(.text);
      /usr/lib/n64/PR/rspboot.o(.text);
#if USE_RSP == 1
#if CFB_FORMAT == 2
      /usr/lib/n64/hvq/hvq2sp2.o(.text);
#else
      /usr/lib/n64/hvq/hvq2sp1.o(.text);
#endif
#endif

      /* data */
      BUILD_DIR/src/main*.o(.data*);

      */libultra_rom.a:*.o(.data*);
      */libhvq2.a:*.o(.data*);
      /usr/lib/n64/PR/rspboot.o(.data*);
#if USE_RSP == 1
#if CFB_FORMAT == 2
      /usr/lib/n64/hvq/hvq2sp2.o(.data*);
#else
      /usr/lib/n64/hvq/hvq2sp1.o(.data*);
#endif
#endif

      /* rodata */
      BUILD_DIR/src/main*.o(.*rodata*);

      */libultra_rom.a:*.o(.*rodata*);
      */libhvq2.a:*.o(.*rodata*);
      */libgcc.a:*.o(.*rodata*);
   }
   END_SEG(code)
   BEGIN_NOLOAD(code)
   {
      BUILD_DIR/src/main*.o(.*bss*);

      */libultra_rom.a:*.o(COMMON);
      */libultra_rom.a:*.o(.scommon);
      */libultra_rom.a:*.o(.*bss*);
      */libhvq2.a:*.o(.*bss*);
   }
   END_NOLOAD(code)

   . = 0x80040000;
   
   BEGIN_NOLOAD(hvqwork)
   {
      BUILD_DIR/src/buffers/hvqwork.o(.bss*);
   }
   END_NOLOAD(hvqwork)

   . = 0x80100000;
   
   BEGIN_NOLOAD(hvqbuf)
   {
      BUILD_DIR/src/buffers/hvqbuf.o(.bss*);
   }
   END_NOLOAD(hvqbuf)

   . = 0x80300000;
   
   BEGIN_NOLOAD(cfb)
   {
      BUILD_DIR/src/buffers/cfb.o(.bss*);
   }
   END_NOLOAD(cfb)

   BEGIN_SEG(hvqdata, __romPos)
   {
      BUILD_DIR/assets/sample.hvq.o(.data);
      . = ALIGN(0x10);
   }
   END_SEG(hvqdata)
   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
