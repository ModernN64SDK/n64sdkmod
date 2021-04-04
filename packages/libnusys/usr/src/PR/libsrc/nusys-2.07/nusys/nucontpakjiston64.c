/*======================================================================*/
/*		NuSYS										*/
/*		nucontpakjiston64.c							*/
/*												*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.				*/
/*												*/
/*----------------------------------------------------------------------*/    
/* Ver 1.0	97/10/9		Created by Kensaku Ohki(SLANP)		*/
/*======================================================================*/
#include <nusys.h>


/* The translation table 	*/
static u8 jis2n64[][2] = {
    {' ',  0x0f},
    {'!',  0x34},
    {'\"', 0x35},
    {'#',  0x36},
    {'\'', 0x37},
    {'*',  0x38},
    {'+',  0x39},
    {',',  0x3a},
    {'-',  0x3b},
    {'.',  0x3c},
    {'/',  0x3d},
    {':',  0x3e},
    {'=',  0x3f},
    {'?',  0x40},
    {'@',  0x41},
    {0xa1, 0x42},	/*  */
    {0xde, 0x43},	/* The sonant mark  */
    {0xdf, 0x44},	/* The p-sound mark  */
    {0xa7, 0x45},	/* a  */
    {0xa8, 0x46},	/* i  */
    {0xa9, 0x47},	/* u  */
    {0xaa, 0x48},	/* e  */
    {0xab, 0x49},	/* o  */
    {0xaf, 0x4a}, 	/* tsu  */    
    {0xac, 0x4b},	/* ya  */
    {0xad, 0x4c},	/* yu  */
    {0xae, 0x4d},	/* yo  */
    {0xa6, 0x4e},	/* wo  */    
    {0xdd, 0x4f},	/* n  */
    {0x00, 0x00},	/* end code */
};
    
    
/*----------------------------------------------------------------------------------*/
/*	nuContPakJisToN64 - The character code conversion (JIS->N64)			*/
/*	Convert from the JIS code to the N64 code 						*/
/*	IN:	*src	The pointer of the character string of the conversion source 	*/
/* 		*dest	The pointer of the character string of the conversion destination */
/*		len	The length of the character string of the conversion source 	*/
/*	RET:	None 												*/
/*----------------------------------------------------------------------------------*/
void nuContPakJisToN64(u8* src, u8* dest, u32 len)
{
    u32 	cnt;
    u32		cnt_len;
    for(cnt_len = 0; cnt_len < len; cnt_len++){
	if(*src == '\0') {
	    /* End with the NULL character  */
	    return;
	} else if((0x41 <= *src) && (*src <= 0x5a)){/* Capital letters of the alphabet  */
	    *dest = *src - 0x27;
	} else if((0x61 <= *src) &&  (*src <= 0x7a)){/* Small letters of the alphabet  */
	    *dest = *src - 0x47;
	} else if((0x30 <= *src) && (*src <= 0x39)){  /* Numerals */
	    *dest = *src -0x20;
	} else if((0xb1 <= *src) && (*src <= 0xdc)){ /* Katakana  */
	    if(*(src+1) == 0xde){		/* The sonant mark  */
		if( *src < 0xca){
		    *dest = *src - 0x3a;
		} else {	
		    *dest = *src - 0x3f;	/* Ha-line  */
		}
		src++;
	    } else if(*(src+1) == 0xdf){	/* The p-sound mark  */
		*dest = *src - 0x3a;
		src++;		
	    } else {
		*dest = *src - 0x61;		/* Normal  */
	    }
	} else {		/* Use the translation table for the others.  */
	    cnt = 0;
	    /* The table search  */
	    while(jis2n64[cnt][1]){
		if(jis2n64[cnt][0] == *src){
		    *dest = jis2n64[cnt][1];
		    break;
		}
		cnt++;
	    }
	    /* If it is not found, make it space.  */
	    if(jis2n64[cnt][1] == 0x00){
		*dest = 0x0f;
	    }
	}
	dest++;
	src++;
    }
}

/*----------------------------------------------------------------------------------*/
/*	nuContPakN64ToJis - The character code conversion (N64->jis)			*/
/*	Convert from the N64 code to the JIS code 						*/
/* 	Note that the sonant and p-sound marks are converted to two characters. 	*/
/*	IN:	*src	The pointer of the character string of the conversion source 	*/
/* 		*dest	The pointer of the character string of the conversion destination */
/*		len	The length of the character string of the conversion source 	*/
/*	RET:	None 												*/
/*----------------------------------------------------------------------------------*/
void nuContPakN64ToJis(u8* src, u8* dest, u32 len)
{
    u32	cnt_len;
    u32	cnt;
    
    for(cnt_len = 0; cnt_len < len; cnt_len++){
	if(*src == '\0') {
	    /* Leave the NULL character as it is  */
	    *dest = *src;
	} else if((0x1a <= *src) && (*src <= 0x33)){/* The alphabet  */
	    *dest = *src + 0x27;
	} else if((0x10 <= *src) && (*src <= 0x19)){  /* Numerals   */
	    *dest = *src + 0x20;
	} else if((0x50 <= *src) && (*src <= 0x7b)){ /* Katakana  */
	    *dest = *src + 0x61;
	} else if((0x7c <= *src) && (*src <= 0x8a)){ /* The sonant marks (Ga-Do)  */
	    *dest = *src + 0x3a;
	    *(dest + 1) = 0xde;
	    dest++;
	} else if((0x8b <= *src) && (*src <= 0x8f)){ /*The sonant marks (Ba-line)  */
	    *dest = *src + 0x3f;
	    *(dest + 1) = 0xde;
	} else if((0x90 <= *src) && (*src <= 0x94)){
	    *dest = *src + 0x3a;
	    *(dest + 1) = 0xdf;
	} else {		/* Use the translation table for the others  */
	    cnt = 0;
	    /* The table search  */
	    while(jis2n64[cnt][1]){
		if(jis2n64[cnt][1] == *src){
		    *dest = jis2n64[cnt][0];
		    break;
		}
		cnt++;
	    }
	    /* If it is not found, make it space.  */
	    if(jis2n64[cnt][1] == 0x00){
		*dest = 0x0f;
	    }
	}
	dest++;
	src++;
    }
}
