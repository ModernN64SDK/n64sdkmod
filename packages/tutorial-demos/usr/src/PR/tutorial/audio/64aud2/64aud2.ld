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
      /usr/lib/n64/PR/gspF3DEX2.fifo.o(.text);
      /usr/lib/n64/PR/gspL3DEX2.fifo.o(.text);
      /usr/lib/n64/PR/gspF3DEX2.Rej.fifo.o(.text);
      /usr/lib/n64/PR/gspF3DEX2.NoN.fifo.o(.text);
      /usr/lib/n64/PR/gspF3DLX2.Rej.fifo.o(.text);
      /usr/lib/n64/PR/gspS2DEX2.fifo.o(.text);
#ifdef N_AUDIO
      /usr/lib/n64/PR/n_aspMain.o(.text);
#else
      /usr/lib/n64/PR/aspMain.o(.text);
#endif

      /* data */
      codesegment.o(.data*);
      /usr/lib/n64/PR/rspboot.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.fifo.o(.data*);
      /usr/lib/n64/PR/gspL3DEX2.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.Rej.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.NoN.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DLX2.Rej.fifo.o(.data*);
      /usr/lib/n64/PR/gspS2DEX2.fifo.o(.data*);
#ifdef N_AUDIO
      /usr/lib/n64/PR/n_aspMain.o(.data*);
#else
      /usr/lib/n64/PR/aspMain.o(.data*);
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
   
   BEGIN_SEG(pbank1, __romPos)
   {
      ../data/demo2_1.ptro(.data*);
      . = ALIGN(0x10);
   }
   END_SEG(pbank1)
   
   BEGIN_SEG(wbank1, __romPos)
   {
      ../data/demo2_1.wbko(.data*);
      . = ALIGN(0x10);
   }
   END_SEG(wbank1)
   
   BEGIN_SEG(song1, __romPos)
   {
      ../data/demo2_1.bino(.data*);
      . = ALIGN(0x10);
   }
   END_SEG(song1)
   
   BEGIN_SEG(song2, __romPos)
   {
      ../data/demo2_2.bino(.data*);
      . = ALIGN(0x10);
   }
   END_SEG(song2)
   
   BEGIN_SEG(sebank1, __romPos)
   {
      ../data/demo2_1.bfxo(.data*);
      . = ALIGN(0x10);
   }
   END_SEG(sebank1)
   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
