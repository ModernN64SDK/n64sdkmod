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
      /usr/lib/n64/PR/gspS2DEX.fifo.o(.text);

      /* data */
      codesegment.o(.data*);
      /usr/lib/n64/PR/rspboot.o(.data*);
      /usr/lib/n64/PR/gspS2DEX.fifo.o(.data*);

      /* rodata */
      codesegment.o(.rodata*);
   }
   END_SEG(code)
   BEGIN_NOLOAD(code)
   {
      codesegment.o(COMMON);
      codesegment.o(.scommon*);
      codesegment.o(.bss*);
   }
   END_NOLOAD(code)
   
   _codeSegmentBssEnd = .;
   
   BEGIN_SEG(static, 0x01000000)
   {
      staticsegment.o(.data);
   }
   END_SEG(static)
   
   BEGIN_SEG(bg_ci, 0x02000000)
   {
      L2_TV.o(.data);
   }
   END_SEG(bg_ci)

   . = 0x80300000;
   
   BEGIN_NOLOAD(system_cfb)
   {
      system_cfb.o(.bss*);
   }
   END_NOLOAD(system_cfb)

   . = 0x803a0000;
   
   BEGIN_NOLOAD(system_rspbuf)
   {
      system_rspbuf.o(.bss*);
   }
   END_NOLOAD(system_rspbuf)

   BEGIN_SEG(img_data, __romPos)
   {
      img_data.o(.data);
   }
   END_SEG(img_data)

   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
