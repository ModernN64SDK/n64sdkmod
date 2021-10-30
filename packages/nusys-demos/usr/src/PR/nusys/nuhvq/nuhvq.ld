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
	KEEP(BUILD_DIR/asm/entry.o(.start));
      /usr/lib/n64/nusys/nusys_rom.o(.text);
      BUILD_DIR/src/main*.o(.text);

      */libultra_rom.a:*.o(.text);
      */libnusys.a:*.o(.text);
      */libhvq2.a:*.o(.text);
#ifdef NAUDIO
      */libnualstl_n.a:*.o(.text);
      */libn_mus.a:*.o(.text);
      */libn_audio_sc.a:*.o(.text);
#else
      */libnualstl.a:*.o(.text);
      */libmus.a:*.o(.text);
#endif
      */libgcc.a:*.o(.text);
      /usr/lib/n64/PR/rspboot.o(.text);
      /usr/lib/n64/PR/aspMain.o(.text);
      /usr/lib/n64/PR/n_aspMain.o(.text);
      /usr/lib/n64/PR/gspF3DEX2.fifo.o(.text);
      /usr/lib/n64/PR/gspL3DEX2.fifo.o(.text);
      /usr/lib/n64/PR/gspF3DEX2.Rej.fifo.o(.text);
      /usr/lib/n64/PR/gspF3DEX2.NoN.fifo.o(.text);
      /usr/lib/n64/PR/gspF3DLX2.Rej.fifo.o(.text);
      /usr/lib/n64/PR/gspS2DEX2.fifo.o(.text);
      /usr/lib/n64/hvq/hvq2sp1.o(.text);

      /* data */
      /usr/lib/n64/nusys/nusys_rom.o(.data*);
      BUILD_DIR/src/main*.o(.data*);

      */libultra_rom.a:*.o(.data*);
      */libnusys.a:*.o(.data*);
      */libhvq2.a:*.o(.data*);
#ifdef NAUDIO
      */libnualstl_n.a:*.o(.data*);
      */libn_mus.a:*.o(.data*);
      */libn_audio_sc.a:*.o(.data*);
#else
      */libnualstl.a:*.o(.data*);
      */libmus.a:*.o(.data*);
#endif
      /usr/lib/n64/PR/rspboot.o(.data*);
      /usr/lib/n64/PR/aspMain.o(.data*);
      /usr/lib/n64/PR/n_aspMain.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.fifo.o(.data*);
      /usr/lib/n64/PR/gspL3DEX2.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.Rej.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.NoN.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DLX2.Rej.fifo.o(.data*);
      /usr/lib/n64/PR/gspS2DEX2.fifo.o(.data*);
      /usr/lib/n64/hvq/hvq2sp1.o(.data*);

      /* rodata */
      /usr/lib/n64/nusys/nusys_rom.o(.*rodata*);
      BUILD_DIR/src/main*.o(.*rodata*);

      */libultra_rom.a:*.o(.*rodata*);
      */libnusys.a:*.o(.*rodata*);
#ifdef NAUDIO
      */libnualstl_n.a:*.o(.*rodata*);
      */libn_mus.a:*.o(.*rodata*);
      */libn_audio_sc.a:*.o(.*rodata*);
#else
      */libnualstl.a:*.o(.*rodata*);
      */libmus.a:*.o(.*rodata*);
#endif
      */libgcc.a:*.o(.*rodata*);
   }
   END_SEG(code)
   BEGIN_NOLOAD(code)
   {
      /usr/lib/n64/nusys/nusys_rom.o(COMMON);
      /usr/lib/n64/nusys/nusys_rom.o(.scommon);
      /usr/lib/n64/nusys/nusys_rom.o(.*bss*);
      BUILD_DIR/src/main*.o(.*bss*);

      */libultra_rom.a:*.o(COMMON);
      */libultra_rom.a:*.o(.scommon);
      */libultra_rom.a:*.o(.*bss*);
      */libnusys.a:*.o(COMMON);
      */libnusys.a:*.o(.scommon);
      */libnusys.a:*.o(.*bss*);
      */libhvq2.a:*.o(.bss*);
#ifdef NAUDIO
      */libnualstl_n.a:*.o(COMMON);
      */libnualstl_n.a:*.o(.scommon);
      */libnualstl_n.a:*.o(.*bss*);
      */libn_mus.a:*.o(COMMON);
      */libn_mus.a:*.o(.scommon);
      */libn_mus.a:*.o(.*bss*);
      */libn_audio_sc.a:*.o(COMMON);
      */libn_audio_sc.a:*.o(.scommon);
      */libn_audio_sc.a:*.o(.*bss*);
#else
      */libnualstl.a:*.o(COMMON);
      */libnualstl.a:*.o(.scommon);
      */libnualstl.a:*.o(.*bss*);
      */libmus.a:*.o(COMMON);
      */libmus.a:*.o(.scommon);
      */libmus.a:*.o(.*bss*);
#endif
   }
   END_NOLOAD(code)
   
   BEGIN_NOLOAD(hvqwork)
   {
      BUILD_DIR/src/buffers/hvqwork.o(.bss*);
      . = ALIGN(0x8);
   }
   END_NOLOAD(hvqwork)
   
   BEGIN_NOLOAD(hvqbuf)
   {
      BUILD_DIR/src/buffers/hvqbuf.o(.bss*);
      . = ALIGN(0x10);
   }
   END_NOLOAD(hvqbuf)
   
   BEGIN_NOLOAD(hvqimg)
   {
      BUILD_DIR/src/buffers/hvqimg.o(.bss*);
      . = ALIGN(0x10);
   }
   END_NOLOAD(hvqimg)

   BEGIN_SEG(rawdata, __romPos)
   {
      BUILD_DIR/assets/rawdata.o(.data*);
   }
   END_SEG(rawdata)
   
   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
