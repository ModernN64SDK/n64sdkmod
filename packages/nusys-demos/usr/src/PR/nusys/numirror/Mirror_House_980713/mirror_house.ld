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

   BEGIN_SEG(select_bg, _codeSegmentBssEnd)
   {
      Data/sp_select_bg.o(.data*);
   }
   END_SEG(select_bg)
   BEGIN_NOLOAD(select_bg)
   {
      _select_bgSegmentDataStart = .;
      Data/sp_select_bg.o(.bss*);
      _select_bgSegmentDataEnd = .;
   }
   END_NOLOAD(select_bg)

   BEGIN_SEG(opimgs, _select_bgSegmentBssEnd)
   {
      _opimgsSegmentDataStart = .;
      Data/opening_imgs.o(.data*);
      _opimgsSegmentDataEnd = .;
   }
   END_SEG(opimgs)

   BEGIN_SEG(logoimgs, _opimgsSegmentEnd)
   {
      _logoimgsSegmentDataStart = .;
      Data/logo_imgs.o(.data*);
      _logoimgsSegmentDataEnd = .;
   }
   END_SEG(logoimgs)

   BEGIN_SEG(opening, _logoimgsSegmentEnd)
   {
      _openingSegmentDataStart = .;
      Data/opening.o(.data*);
      _openingSegmentDataEnd = .;
   }
   END_SEG(opening)
   BEGIN_NOLOAD(opening)
   {
      Data/opening.o(.bss*);
   }
   END_NOLOAD(opening)

   BEGIN_NOLOAD(mem_heap)
   {
      Data/om_mem_heap.o(.bss*);
   }
   END_NOLOAD(mem_heap)

#ifdef NUALSGI
   BEGIN_SEG(sgi_data, __romPos)
   {
      sgi_data.o(.data);
   }
   END_SEG(sgi_data)
#else
   BEGIN_SEG(sndtools_data, __romPos)
   {
      sndtools_data.o(.data);
   }
   END_SEG(sndtools_data)
#endif

   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
