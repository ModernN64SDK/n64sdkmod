OUTPUT_ARCH (mips)

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
#ifdef NAUDIO
      /usr/lib/n64/PR/n_aspMain.o(.text);
#else
      /usr/lib/n64/PR/aspMain.o(.text);
#endif

      /* data */
      test_wave.o(.data*);
      /usr/lib/n64/PR/rspboot.o(.data*);
      /usr/lib/n64/PR/gspF3DEX.fifo.o(.data*);
#ifdef NAUDIO
      /usr/lib/n64/PR/n_aspMain.o(.data*);
#else
      /usr/lib/n64/PR/aspMain.o(.data*);
#endif

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
   
   BEGIN_SEG(pbank, __romPos)
   {
      ../../SoundData/drumtest.ptro(.data*);
      . = ALIGN(0x10);
   }
   END_SEG(pbank)
   
   BEGIN_SEG(wbank, __romPos)
   {
      ../../SoundData/drumtest.wbko(.data*);
      . = ALIGN(0x10);
   }
   END_SEG(wbank)
   
   BEGIN_SEG(tune, __romPos)
   {
      ../../SoundData/dt_song1.bino(.data*);
      . = ALIGN(0x10);
   }
   END_SEG(tune)

   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
