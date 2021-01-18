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
      /usr/lib/n64/PR/gspFast3D.o(.text);
      /usr/lib/n64/PR/gspFast3D.fifo.o(.text);
      /usr/lib/n64/PR/gspTurbo3D.o(.text);
      /usr/lib/n64/PR/gspTurbo3D.fifo.o(.text);

      /* data */
      codesegment.o(.data*);
      /usr/lib/n64/PR/rspboot.o(.data*);
      /usr/lib/n64/PR/gspFast3D.o(.data*);
      /usr/lib/n64/PR/gspFast3D.fifo.o(.data*);
      /usr/lib/n64/PR/gspTurbo3D.o(.data*);
      /usr/lib/n64/PR/gspTurbo3D.fifo.o(.data*);

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
   
   BEGIN_SEG(static, 0x01000000)
   {
      static.o(.data);
   }
   END_SEG(static)
   
   . = 0x80118000;
   
   BEGIN_NOLOAD(zbuffer)
   {
      zbuffer.o(.bss*);
   }
   END_NOLOAD(zbuffer)

   . = 0x8013d800;
   
   BEGIN_NOLOAD(cfb)
   {
      cfb.o(.bss*);
   }
   END_NOLOAD(cfb)

   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
