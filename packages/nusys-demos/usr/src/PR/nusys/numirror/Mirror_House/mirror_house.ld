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
      KEEP(BUILD_DIR/asm/rom_header.o(.text)); /* ROM Header */
      KEEP(BUILD_DIR/boot.6102.o(.data)); /* CIC 6102 bootcode */
   }
   END_SEG(boot)

   BEGIN_SEG(code, NU_SPEC_BOOT_ADDR) SUBALIGN(16)
   {
      /* text */
      BUILD_DIR/asm/entry.o(.start);
      BUILD_DIR/codesegment.o(.text);
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
      BUILD_DIR/codesegment.o(.data*);
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
      BUILD_DIR/codesegment.o(.rodata*);
   }
   END_SEG(code)
   BEGIN_NOLOAD(code)
   {
      BUILD_DIR/codesegment.o(COMMON);
      BUILD_DIR/codesegment.o(.scommon*);
      BUILD_DIR/codesegment.o(.bss*);
      . = ALIGN(0x8);
   }
   END_NOLOAD(code)

   BEGIN_SEG(select_bg, _codeSegmentBssEnd)
   {
      _select_bgSegmentDataStart = .;
      BUILD_DIR/assets/sp_select_bg.o(.data*);
      _select_bgSegmentDataEnd = .;
   }
   END_SEG(select_bg)
   BEGIN_NOLOAD(select_bg)
   {
      BUILD_DIR/assets/sp_select_bg.o(.bss*);
   }
   END_NOLOAD(select_bg)

   BEGIN_SEG(opimgs, _select_bgSegmentBssEnd)
   {
      _opimgsSegmentDataStart = .;
      BUILD_DIR/assets/opening_imgs.o(.data*);
      _opimgsSegmentDataEnd = .;
   }
   END_SEG(opimgs)

   BEGIN_SEG(logoimgs, _opimgsSegmentEnd)
   {
      _logoimgsSegmentDataStart = .;
      BUILD_DIR/assets/logo_imgs.o(.data*);
      _logoimgsSegmentDataEnd = .;
   }
   END_SEG(logoimgs)

   BEGIN_SEG(opening, _logoimgsSegmentEnd)
   {
      _openingSegmentDataStart = .;
      BUILD_DIR/assets/opening.o(.data*);
      _openingSegmentDataEnd = .;
   }
   END_SEG(opening)
   BEGIN_NOLOAD(opening)
   {
      BUILD_DIR/assets/opening.o(.bss*);
   }
   END_NOLOAD(opening)

   BEGIN_NOLOAD(mem_heap)
   {
      BUILD_DIR/assets/om_mem_heap.o(.bss*);
   }
   END_NOLOAD(mem_heap)

#ifdef NUALSGI
   BEGIN_SEG(sgi_data, __romPos)
   {
      BUILD_DIR/sound/sgi_data.o(.data);
   }
   END_SEG(sgi_data)
#else
   BEGIN_SEG(sndtools_data, __romPos)
   {
      BUILD_DIR/sound/sndtools_data.o(.data);
   }
   END_SEG(sndtools_data)
#endif

   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
