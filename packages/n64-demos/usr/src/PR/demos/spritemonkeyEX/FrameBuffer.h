/*
  FrameBuffer.h
    Copyright (C) 1999, NINTENDO Co,Ltd., MONEGI CORPORATION.
    Jan. 28, 1999.
*/

#ifndef _FrameBuffer_h_
#define _FrameBuffer_h_

#ifdef __cplusplus
extern "C" {
#endif

#define	CFB_SEGMENT	2
#define	SCREEN_HT	240
#define	SCREEN_WD	320

#ifdef _LANGUAGE_C

#ifndef _SGI_MAKEROM_
#ifdef	USE_CFB32
extern u32 cfb[][SCREEN_WD*SCREEN_HT];
#else	/* USE_CFB32 */
extern u16 cfb[][SCREEN_WD*SCREEN_HT];
#endif	/* USE_CFB32 */

extern void init_framebuffer( void );
extern Gfx *clear_cfb( Gfx *glistp );
extern void swap_framebuffer( int number_of_buffer );
#endif

#endif /* _LANGUAGE_C */

#ifdef __cplusplus
}
#endif

#endif
