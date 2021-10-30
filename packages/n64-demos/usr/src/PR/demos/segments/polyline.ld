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
      BUILD_DIR/asm/rom_header.o(.text); /* ROM Header */
      BUILD_DIR/boot.6102.o(.data); /* CIC 6102 bootcode */
   }
   END_SEG(boot)

   BEGIN_SEG(code, 0x80000400) SUBALIGN(16)
   {
      BUILD_DIR/asm/entry.o(.text);
      BUILD_DIR/codesegment.o(.text);
      /usr/lib/n64/PR/rspboot.o(.text);
      /usr/lib/n64/PR/gspL3DEX2.xbus.o(.text);
      /usr/lib/n64/PR/gspF3DEX2.xbus.o(.text);

      /* data */
      BUILD_DIR/codesegment.o(.data*);
      /usr/lib/n64/PR/rspboot.o(.data*);
      /usr/lib/n64/PR/gspL3DEX2.xbus.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.xbus.o(.data*);

      /* rodata */
      BUILD_DIR/codesegment.o(.rodata*);
   }
   END_SEG(code)
   BEGIN_NOLOAD(code)
   {
      BUILD_DIR/codesegment.o(COMMON);
      BUILD_DIR/codesegment.o(.scommon*);
      BUILD_DIR/codesegment.o(.bss*);
   }
   END_NOLOAD(code)

   . = 0x8018f800;
   
   BEGIN_NOLOAD(cfb)
   {
      BUILD_DIR/src/buffers/cfb.o(.bss*);
   }
   END_NOLOAD(cfb)

   . = 0x803da800;
   
   BEGIN_NOLOAD(zbuffer)
   {
      BUILD_DIR/src/buffers/zbuffer.o(.bss*);
   }
   END_NOLOAD(zbuffer)

   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
