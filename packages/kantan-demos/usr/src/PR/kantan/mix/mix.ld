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
      /usr/lib/n64/PR/aspMain.o(.text);
      /usr/lib/n64/PR/gspF3DEX.fifo.o(.text);
      /usr/lib/n64/PR/gspF3DLX.fifo.o(.text);
      /usr/lib/n64/PR/gspF3DLX.Rej.fifo.o(.text);
      /usr/lib/n64/PR/gspF3DLP.Rej.fifo.o(.text);
      /usr/lib/n64/PR/gspL3DEX.fifo.o(.text);
      /usr/lib/n64/PR/gspLine3D.fifo.o(.text);

      /* data */
      BUILD_DIR/src/main*.o(.data*);

      */libultra_rom.a:*.o(.data*);
      /usr/lib/n64/PR/rspboot.o(.data*);
      /usr/lib/n64/PR/aspMain.o(.data*);
      /usr/lib/n64/PR/gspF3DEX.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DLX.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DLX.Rej.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DLP.Rej.fifo.o(.data*);
      /usr/lib/n64/PR/gspL3DEX.fifo.o(.data*);
      /usr/lib/n64/PR/gspLine3D.fifo.o(.data*);

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
   
   . = 0x802DA800;
   
   BEGIN_NOLOAD(zbuf)
   {
      BUILD_DIR/src/buffers/zbuf.o(.bss*);
   }
   END_NOLOAD(zbuf)
   
   BEGIN_NOLOAD(gfxdlist)
   {
      BUILD_DIR/src/buffers/gfxdlistbuf.o(.bss*);
   }
   END_NOLOAD(gfxdlist)
   
   . = 0x8036A000;
   
   BEGIN_NOLOAD(cfb)
   {
      BUILD_DIR/src/buffers/cfb.o(.bss*);
   }
   END_NOLOAD(cfb)
   
   BEGIN_SEG(midibank, __romPos)
   {
      /usr/lib/n64/soundbanks/GenMidiBank.ctl.o(.data*);
      . = ALIGN(0x10);
   }
   END_SEG(midibank)
   BEGIN_SEG(miditable, __romPos)
   {
      /usr/lib/n64/soundbanks/GenMidiBank.tbl.o(.data*);
      . = ALIGN(0x10);
   }
   END_SEG(miditable)
   BEGIN_SEG(seq, __romPos)
   {
      /usr/lib/n64/sequences/ship.sbk.o(.data*);
      . = ALIGN(0x10);
   }
   END_SEG(seq)
   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
