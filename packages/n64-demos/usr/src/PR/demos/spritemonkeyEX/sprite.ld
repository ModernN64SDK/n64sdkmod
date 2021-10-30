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
   
   . = 0x02000000;

   BEGIN_NOLOAD(rsp_cfb)
   {
      BUILD_DIR/src/buffers/rsp_cfb.o(.bss*);
   }
   END_NOLOAD(rsp_cfb)

   BEGIN_SEG(boot, 0x04000000)
   {
      BUILD_DIR/asm/rom_header.o(.text); /* ROM Header */
      BUILD_DIR/boot.6102.o(.data); /* CIC 6102 bootcode */
   }
   END_SEG(boot)

   BEGIN_SEG(code, 0x80000400) SUBALIGN(16)
   {
		BUILD_DIR/asm/entry.o(.start);
      BUILD_DIR/src/main*.o(.text);

      */libultra_rom.a:*.o(.text);
      */libgcc.a:*.o(.text);
      /usr/lib/n64/PR/rspboot.o(.text);
      /usr/lib/n64/PR/gspS2DEX2.fifo.o(.text);

      /* data */      /* data */
      BUILD_DIR/src/main*.o(.data*);

      */libultra_rom.a:*.o(.data*);
      /usr/lib/n64/PR/rspboot.o(.data*);
      /usr/lib/n64/PR/gspS2DEX2.fifo.o(.data*);

      /* rodata */
      BUILD_DIR/src/main*.o(.*rodata*);

      */libultra_rom.a:*.o(.*rodata*);
      */libgcc.a:*.o(.*rodata*);
   }
   END_SEG(code)
   BEGIN_NOLOAD(code)
   {
      BUILD_DIR/src/main*.o(.*bss*);

      */libultra_rom.a:*.o(COMMON);
      */libultra_rom.a:*.o(.scommon);
      */libultra_rom.a:*.o(.*bss*);
   }
   END_NOLOAD(code)
   
   BEGIN_SEG(static, .)
   {
      BUILD_DIR/assets/static.o(.data);
   }
   END_SEG(static)

   . = 0x80300000;
   
   BEGIN_NOLOAD(cfb)
   {
      BUILD_DIR/src/buffers/cfb.o(.bss*);
   }
   END_NOLOAD(cfb)
   
   BEGIN_SEG(dk, _staticSegmentEnd)
   {
      BUILD_DIR/assets/dk.o(.data);
   }
   END_SEG(dk)

   BEGIN_SEG(dk7, .)
   {
      BUILD_DIR/assets/dk7.o(.data);
   }
   END_SEG(dk7)
   
   BEGIN_SEG(road, .)
   {
      BUILD_DIR/assets/road.o(.data);
   }
   END_SEG(road)
   
   BEGIN_SEG(l2_tv, .)
   {
      BUILD_DIR/assets/l2_tv.o(.data);
   }
   END_SEG(l2_tv)

   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
