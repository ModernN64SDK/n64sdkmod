/*---------------------------------------------------------------------
	Copyright (C) 1997, Nintendo.
	
	File		texture.c
	Coded    by	Yoshitaka Yasumoto.	Feb  7, 1997.
	Modified by	
	Comments	
	
	$Id: texture.c,v 1.2 1997/08/07 03:24:25 yasu Exp $
  ---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	"texture.h"

#define	BLACK	GPACK_RGBA5551(  0,  0,  0,1)
#define	RED	GPACK_RGBA5551(255,  0,  0,1)
#define	YELLOW	GPACK_RGBA5551(255,255,  0,1)
#define	GREEN	GPACK_RGBA5551(  0,255,  0,1)
#define	BLUE	GPACK_RGBA5551( 32, 32,255,1)
#define	GRAY	GPACK_RGBA5551(128,128,128,1)
#define	PACK16(a,b,c,d)	{ (((a)<<16)|(b)), (((c)<<16)|(d)) }
#define	PACK4(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)	\
{ (((a)<<28)|((b)<<24)|((c)<<20)|((d)<<16)| \
   ((e)<<12)|((f)<< 8)|((g)<< 4)|(h)), \
  (((i)<<28)|((j)<<24)|((k)<<20)|((l)<<16)| \
   ((m)<<12)|((n)<< 8)|((o)<< 4)|(p)) }

/*
 *	RGBA 16b texture
 */
#define	TXL(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) \
        PACK16(a,b,c,d), PACK16(e,f,g,h), PACK16(i,j,k,l), PACK16(m,n,o,p)
#define	X	BLACK
#define	A	RED
#define	B	YELLOW
#define	C	GREEN
#define	D	BLUE
TXTR	textureRGBA16[] = {
#include	"texture_pattern.h"
};
#undef	TXL
#undef	X
#undef	A
#undef	B
#undef	C
#undef	D

/*
 *	CI 4b texture
 */
#define	TXL(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) \
        PACK4(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)
#define	X	0
#define	A	1
#define	B	2
#define	C	3
#define	D	4
TXTR	textureCI4[] = {
#include	"texture_pattern.h"
};
TXTR	textureCI4pal[] = {
PACK16(BLACK, RED,   YELLOW, GREEN ), PACK16(BLUE,  GRAY,  GRAY,  GRAY  ),
PACK16(GRAY,  GRAY,  GRAY,   GRAY  ), PACK16(GRAY,  GRAY,  GRAY,  GRAY  ),
};
#undef	TXL
#undef	X
#undef	A
#undef	B
#undef	C
#undef	D

/*======== End of texture.c ========*/
