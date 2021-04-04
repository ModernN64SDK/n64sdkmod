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

    . = 0x80000400;

   BEGIN_NOLOAD(cfb)
   {
      test_workspace.o(.bss*);
   }
   END_NOLOAD(cfb)

   BEGIN_SEG(code, 0x80060000) SUBALIGN(16)
   {
      asm/entry.o(.text);
      test_wave.o(.text);
      /usr/lib/n64/PR/rspboot.o(.text);
      /usr/lib/n64/PR/gspF3DEX.fifo.o(.text);

      /* data */
      test_wave.o(.data*);
      /usr/lib/n64/PR/rspboot.o(.data*);
      /usr/lib/n64/PR/gspF3DEX.fifo.o(.data*);

      /* rodata */
      test_wave.o(.rodata*);
   }
   END_SEG(code)
   BEGIN_NOLOAD(code)
   {
      test_wave.o(COMMON);
      test_wave.o(.scommon*);
      test_wave.o(.bss*);
   }
   END_NOLOAD(code)

   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
