OUTPUT_ARCH (mips)

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
      /usr/lib/n64/hvqm/hvqm2sp1.o(.text);

      /* data */
      codesegment.o(.data*);
      /usr/lib/n64/PR/rspboot.o(.data*);
      /usr/lib/n64/hvqm/hvqm2sp1.o(.data*);

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
   
   BEGIN_NOLOAD(hvqmwork)
   {
      hvqmwork.o(.bss*);
   }
   END_NOLOAD(hvqmwork)

   . = 0x80080000;
   
   BEGIN_NOLOAD(adpcmbuf)
   {
      adpcmbuf.o(.bss*);
   }
   END_NOLOAD(adpcmbuf)

   . = 0x80100000;
   
   BEGIN_NOLOAD(hvqbuf)
   {
      hvqbuf.o(.bss*);
   }
   END_NOLOAD(hvqbuf)

   . = 0x801c0000;
   
   BEGIN_NOLOAD(pcmbuf)
   {
      pcmbuf.o(.bss*);
   }
   END_NOLOAD(pcmbuf)

   . = 0x80200000;
   
   BEGIN_NOLOAD(cfb)
   {
      cfb.o(.bss*);
   }
   END_NOLOAD(cfb)

   BEGIN_SEG(hvqmdata, __romPos)
   {
      samplehvqm.o(.data);
   }
   END_SEG(hvqmdata)
   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
