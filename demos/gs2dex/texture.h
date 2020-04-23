/*---------------------------------------------------------------------
	Copyright (C) 1997, Nintendo.
	
	File		texture.h
	Coded    by	Yoshitaka Yasumoto.	Feb  7, 1997.
	Modified by	
	Comments	
	
	$Id: texture.h,v 1.3 1997/08/07 03:24:25 yasu Exp $
  ---------------------------------------------------------------------*/

typedef	union	{
  u32	texel[2];
  u64	force_aligned_64b;
} TXTR;

extern	TXTR	textureRGBA16[];
extern	TXTR	textureCI4[];
extern	TXTR	textureCI4pal[];

extern	u16	L2_TV[];
extern	u8	L2_TV_pal[];

/*======== End of texture.h ========*/
