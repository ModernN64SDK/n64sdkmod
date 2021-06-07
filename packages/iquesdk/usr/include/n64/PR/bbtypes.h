#ifndef __BB_TYPES_H__
#define __BB_TYPES_H__

#include <PR/ultratypes.h>

#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)

/*
 * Common types for BB Player software
 */

#define BB_SIG_TYPE_RSA2048  0 /* RSA 2048 bit signature */
#define BB_SIG_TYPE_RSA4096  1 /* RSA 4096 bit signature */
#define BB_SIG_TYPE_ECC      2 /* ECC signature */

typedef u32  BbId;                 /* 32-bit Player identity */
typedef u32  BbContentId;          /* 32-bit content identity */

typedef u32  BbAesKey[4];          /* 128-bit AES key */
typedef u32  BbAesIv[4];           /* 128-bit AES key */
typedef u32  BbEccPrivateKey[8];   /* 256-bit Elliptic key */
typedef u32  BbEccPublicKey[16];   /* X coord, Y coord of elliptic public key */
typedef u32  BbRsaPublicKey2048[64];   /* 2048-bit RSA public key */
typedef u32  BbRsaPublicKey4096[128];  /* 4096-bit RSA public key */
typedef u32  BbRsaExponent;

typedef u32  BbRsaSig2048[64];    /* 2048-bit RSA signature */
typedef u32  BbRsaSig4096[128];   /* 4096-bit RSA signature */
typedef u32  BbEccSig[16];        /* Signature from ECCSign() (X, Y) */
typedef u32  BbOwnerId;
typedef u32  BbRandomMix[8];      /* needed for signing along 
                                     with private key */

typedef u16  BbTicketId;	  /* unique per-player ticket id */
/* union for signature field. contents determined by BB_SIG_TYPE_*.  */
typedef union {                   
    BbRsaSig2048 rsa2048;  /* BB_SIG_TYPE_RSA2048 */
    BbRsaSig4096 rsa4096;  /* BB_SIG_TYPE_RSA4096 */
    BbEccSig     ecc;      /* BB_SIG_TYPE_ECC     */
} BbGenericSig;

typedef u32  BbShaHash[5];         /* 160-bit SHA1 hash */

typedef u8   BbServerName[64];     /* server name, used to describe 
                                    * ascii list heirarchy as: 
                                    *   string with xxxxxxxx representing 
                                    *   serial number in hex. (eg XS0000000f 
                                    *   is ticket server 15). pad with nulls.
                                    *   Root-XSCAxxxxxxxx-XSxxxxxxxx 
                                    *   Root-CPCAxxxxxxxx-CPxxxxxxxx
                                    *   Root-MSCAxxxxxxxx-MSxxxxxxxx
                                    */
typedef u8   BbName[64];
typedef u8   BbServerSuffix[64];   /* holds only suffix name for BbServerName
                                    * (i.e., XSxxxxxxxx, where xxxxxxxx is
                                    * the serial number).
                                    */
typedef BbServerSuffix   BbCrlEntry;

#endif

#endif /*__BB_TYPES_H__*/
