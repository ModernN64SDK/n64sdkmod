/*======================================================================*/
/*	NIFFprev							*/
/*	om_title_param.c						*/
/*									*/
/*	Copyright (C) 1998, NINTENDO Co,Ltd.				*/
/*									*/
/*======================================================================*/
#include	<ultra64.h>
#include	<PR/gs2dex.h>

#define		DEF_OPENING_WIDTH		64		/*Width of 1 sprite */
#define		DEF_OPENING_HEIGHT		32		/* Height of 1 sprite */
#define		DEF_OPENING_IMAGESIZ	G_IM_SIZ_16b	/* Sprite image size */
#define		DEF_OPENING_IMAGEFMT	G_IM_FMT_RGBA	/* Sprite image format */

/* ===========================================================================
	Sprite Definitions 
 =========================================================================== */
	/* Main title screen background */
uObjTxSprite title_sp[] = {
{	/* 0-0 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address for the loading destination */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		0<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		0*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 0-1 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		1*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		0*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 0-2 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		2*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		0*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 0-3 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		3*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		0*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 0-4 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		4*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		0*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 1-0 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		0<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		1*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 1-1 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		1*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		1*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 1-2 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		2*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		1*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 1-3 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		3*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		1*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 1-4 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		4*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		1*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 2-0 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		0<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		2*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 2-1 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		1*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		2*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 2-2 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		2*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		2*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 2-3 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		3*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		2*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 2-4 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		4*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		2*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 3-0 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		0<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		3*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 3-1 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		1*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		3*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 3-2 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		2*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		3*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 3-3 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		3*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		3*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 3-4 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		4*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		3*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 4-0 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		0<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		4*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 4-1 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		1*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		4*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 4-2 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		2*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		4*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 4-3 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		3*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		4*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 4-4 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		4*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		4*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 5-0 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		0<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		5*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 5-1 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		1*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		5*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 5-2 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		2*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		5*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 5-3 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		3*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		5*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 5-4 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		4*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		5*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 6-0 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		0<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		6*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 6-1 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		1*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		6*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 6-2 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		2*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		6*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 6-3 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		3*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		6*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 6-4 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		4*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		6*DEF_OPENING_HEIGHT<<2 , 1<<10 , DEF_OPENING_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 7-0 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		0<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		7*DEF_OPENING_HEIGHT<<2 , 1<<10 , 16<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 7-1 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		1*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		7*DEF_OPENING_HEIGHT<<2 , 1<<10 , 16<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 7-2 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		2*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		7*DEF_OPENING_HEIGHT<<2 , 1<<10 , 16<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 7-3 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		3*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		7*DEF_OPENING_HEIGHT<<2 , 1<<10 , 16<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 7-4 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_OPENING_WIDTH*DEF_OPENING_HEIGHT , DEF_OPENING_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		4*DEF_OPENING_WIDTH<<2 , 1<<10 , DEF_OPENING_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		7*DEF_OPENING_HEIGHT<<2 , 1<<10 , 16<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_OPENING_WIDTH , DEF_OPENING_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_OPENING_IMAGESIZ) ,	/* imageAdrs */
		DEF_OPENING_IMAGEFMT ,		/* imageFmt */
		DEF_OPENING_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
};

	/* Sprite parameters for the Monego logo */
#define		DEF_LOGO_WIDTH			64		/* Width of 1 sprite  */
#define		DEF_LOGO_HEIGHT			32		/* Height of 1 sprite */
#define		DEF_LOGO_IMAGESIZ	G_IM_SIZ_16b	/* Sprite image size */
#define		DEF_LOGO_IMAGEFMT	G_IM_FMT_RGBA	/* Sprite image format */

uObjTxSprite logo_sp[] = {
{	/* 0-0 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_LOGO_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_LOGO_WIDTH*DEF_LOGO_HEIGHT , DEF_LOGO_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_LOGO_WIDTH , DEF_LOGO_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		96<<2 , 1<<10 , DEF_LOGO_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		104<<2 , 1<<10 , DEF_LOGO_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_LOGO_WIDTH , DEF_LOGO_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_LOGO_IMAGESIZ) ,	/* imageAdrs */
		DEF_LOGO_IMAGEFMT ,		/* imageFmt */
		DEF_LOGO_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
{	/* 0-1 */
	{
		G_OBJLT_TXTRBLOCK ,		/* Type class*/
		0 ,	/* Texture source address in DRAM */
		GS_PIX2TMEM( 0 , DEF_LOGO_IMAGESIZ ) ,	/* TMEM word address of the loading destination  */
		GS_TB_TSIZE( DEF_LOGO_WIDTH*DEF_LOGO_HEIGHT , DEF_LOGO_IMAGESIZ ) ,	/* Texture width  */
		GS_TB_TLINE( DEF_LOGO_WIDTH , DEF_LOGO_IMAGESIZ ) ,	/* Texture height */
		0 ,			/* Status ID */
		-1 ,			/* Status Flag */
		0			/* Status Mask */
	} ,
	{
		96+1*DEF_LOGO_WIDTH<<2 , 1<<10 , DEF_LOGO_WIDTH<<5 , 0 ,	/* objX , scaleW , imageW , paddingX */
		104<<2 , 1<<10 , DEF_LOGO_HEIGHT<<5 , 0 ,	/* objY , scaleH , imageH , paddingY */
		GS_PIX2TMEM(DEF_LOGO_WIDTH , DEF_LOGO_IMAGESIZ) ,	/* ImageStride */
		GS_PIX2TMEM( 0 , DEF_LOGO_IMAGESIZ) ,	/* imageAdrs */
		DEF_LOGO_IMAGEFMT ,		/* imageFmt */
		DEF_LOGO_IMAGESIZ ,		/* imageSiz */
		0 ,					/* imagePal */
		0					/* imageFlags */
	}
},
};

	/* Matrix for sprite display (identity transformation) */
uObjMtx	mt = {
	0x10000 , 0 , 0 , 0x10000 , 0 , 0 , 1<<10 , 1<<10
};



