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
      boot.o(.text);
      audio.o(.text);
      dram_stack.o(.text);
      main.o(.text);
      sequence.o(.text);
      message.o(.text);
      nnsched.o(.text);
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
      boot.o(.data*);
      audio.o(.data*);
      dram_stack.o(.data*);
      main.o(.data*);
      sequence.o(.data*);
      message.o(.data*);
      nnsched.o(.data*);
      */libultra_rom.a:*.o(.data*);
      */libultra_rom.a:*.o(COMMON);
      */libultra_rom.a:*.o(.scommon*);
      /usr/lib/n64/PR/rspboot.o(.data*);
      /usr/lib/n64/PR/aspMain.o(.data*);
      /usr/lib/n64/PR/gspF3DEX.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DLX.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DLX.Rej.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DLP.Rej.fifo.o(.data*);
      /usr/lib/n64/PR/gspL3DEX.fifo.o(.data*);
      /usr/lib/n64/PR/gspLine3D.fifo.o(.data*);

      /* rodata */
      boot.o(.rodata*);
      audio.o(.rodata*);
      dram_stack.o(.rodata*);
      main.o(.rodata*);
      sequence.o(.rodata*);
      message.o(.rodata*);
      nnsched.o(.rodata*);
      */libultra_rom.a:*.o(.rodata*);
      */libgcc.a:*.o(.rodata*);
   }
   END_SEG(code)
   BEGIN_NOLOAD(code)
   {
      boot.o(.bss*);
      audio.o(.bss*);
      dram_stack.o(.bss*);
      main.o(.bss*);
      sequence.o(.bss*);
      message.o(.bss*);
      nnsched.o(.bss*);
      */libultra_rom.a:*.o(.bss*);
      */libultra_rom.a:*.o(.sbss*);
      . = ALIGN(0x8);
   }
   END_NOLOAD(code)
   
   . = 0x802DA800;
   
   BEGIN_NOLOAD(zbuf)
   {
      zbuf.o(.bss*);
   }
   END_NOLOAD(zbuf)
   
   . = 0x8036A000;
   
   BEGIN_NOLOAD(cfb)
   {
      cfb.o(.bss*);
   }
   END_NOLOAD(cfb)
   
   BEGIN_SEG(midibank, __romPos)
   {
      /usr/lib/n64/soundbanks/GenMidiBank.ctl.o(.data*);
   }
   END_SEG(midibank)
   BEGIN_SEG(miditable, __romPos)
   {
      /usr/lib/n64/soundbanks/GenMidiBank.tbl.o(.data*);
   }
   END_SEG(miditable)
   BEGIN_SEG(seq, __romPos)
   {
      /usr/lib/n64/sequences/ship.sbk.o(.data*);
   }
   END_SEG(seq)
   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
