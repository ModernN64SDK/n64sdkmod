OUTPUT_ARCH (mips)

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
      asm/rom_header.o(.text);
      boot.6102.o(.data);
   }
   END_SEG(boot)

   BEGIN_SEG(code, 0x80000400) SUBALIGN(16)
   {
      asm/entry.o(.text);
      codesegment.o(.text);
      /usr/lib/n64/PR/rspboot.o(.text);
#ifdef EX
      /usr/lib/n64/PR/gspF3DEX.fifo.o(.text);
#elif LX
      /usr/lib/n64/PR/gspF3DLX.fifo.o(.text);
#elif LX_Rej
      /usr/lib/n64/PR/gspF3DLX.Rej.fifo.o(.text);
#elif LP_Rej
      /usr/lib/n64/PR/gspF3DLP.Rej.fifo.o(.text);
#endif
      /usr/lib/n64/PR/aspMain.o(.text);

      /* data */
      codesegment.o(.data*);
      /usr/lib/n64/PR/rspboot.o(.data*);
#ifdef EX
      /usr/lib/n64/PR/gspF3DEX.fifo.o(.data*);
#elif LX
      /usr/lib/n64/PR/gspF3DLX.fifo.o(.data*);
#elif LX_Rej
      /usr/lib/n64/PR/gspF3DLX.Rej.fifo.o(.data*);
#elif LP_Rej
      /usr/lib/n64/PR/gspF3DLP.Rej.fifo.o(.data*);
#endif
      /usr/lib/n64/PR/aspMain.o(.data*);

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
   
   BEGIN_SEG(gfxdlists, .)
   {
      gfxdlists.o(.data);
   }
   END_SEG(gfxdlists)

   _gfxdlistsSegmentEnd = .;
   
   BEGIN_SEG(static, 0x01000000)
   {
      gfxinit.o(.data);
      gfxstatic.o(.data);
   }
   END_SEG(static)
   
   BEGIN_SEG(dynamic, 0x02000000)
   {
      gfxdynamic.o(.data);
   }
   END_SEG(dynamic)
   
   . = 0x801da800;
   
   BEGIN_NOLOAD(zbuffer)
   {
      gfxzbuffer.o(.bss*);
   }
   END_NOLOAD(zbuffer)

   . = 0x80200000;
   
   BEGIN_NOLOAD(cfb)
   {
      gfxcfb.o(.bss*);
   }
   END_NOLOAD(cfb)

   BEGIN_SEG(sound_data, __romPos)
   {
      sound_data.o(.data);
   }
   END_SEG(sound_data)

   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
