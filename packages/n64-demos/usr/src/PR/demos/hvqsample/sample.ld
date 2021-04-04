OUTPUT_ARCH (mips)

#include "system.h"

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

   BEGIN_SEG(code, 0x80000400) SUBALIGN(16)
   {
      asm/entry.o(.text);
      codesegment.o(.text);
      /usr/lib/n64/PR/rspboot.o(.text);
#if CFB_FORMAT == 2
      /usr/lib/n64/hvq/hvq2sp2.o(.text);
#else
      /usr/lib/n64/hvq/hvq2sp1.o(.text);
#endif

      /* data */
      codesegment.o(.data*);
      /usr/lib/n64/PR/rspboot.o(.data*);
#if CFB_FORMAT == 2
      /usr/lib/n64/hvq/hvq2sp2.o(.data*);
#else
      /usr/lib/n64/hvq/hvq2sp1.o(.data*);
#endif

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
   
   _codeSegmentBssEnd = .;

   . = 0x80040000;
   
   BEGIN_NOLOAD(hvqwork)
   {
      hvqwork.o(.bss*);
   }
   END_NOLOAD(hvqwork)

   . = 0x80100000;
   
   BEGIN_NOLOAD(hvqbuf)
   {
      hvqbuf.o(.bss*);
   }
   END_NOLOAD(hvqbuf)

   . = 0x80300000;
   
   BEGIN_NOLOAD(cfb)
   {
      cfb.o(.bss*);
   }
   END_NOLOAD(cfb)

   BEGIN_SEG(hvqdata, __romPos)
   {
      samplehvq.o(.data);
      . = ALIGN(0x10);
   }
   END_SEG(hvqdata)
   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
