/*******************************************************************
 * bbcrl.h
 * use this for
 * any crl parsed by the BB player: CACRL, Eticket (ticket or license)
 * server crl, content publisher crl, or content crl (in content crl
 * the certificate serialNumber is replaced by contentID.
 ******************************************************************/
#ifndef __BB_CRL_H_
#define __BB_CRL_H_

#include <PR/bbtypes.h>

#define BB_CRL_TYPE_XS 0  /* ticket/transaction server */
#define BB_CRL_TYPE_CP 1  /* content publishing server */
#define BB_CRL_TYPE_CA 2  /* ca server */
#define BB_CRL_TYPE_MS 4  /* manufacturing server */

typedef enum {
    CRL_UNUSED0 = 0,
    CRL_UNUSED1,
    CRL_UNUSED2
}BbCrlUnusedEnumType;

typedef struct {
    /* fixed length header */
    BbGenericSig signature;  /* RSA 2048 or RSA 4096 signature, as
                              * defined by above sigType. 
                              */
    u32 type;            /* Crl type, BB_CRL_TYPE_* */
    u32 sigType;         /* signature type, BB_SIG_TYPE_* */
    BbCrlUnusedEnumType unusedPadding;
    u32 versionNumber;   /* version number of that type */
    u32 date;		 /* for server, unix style timestamp */
    BbServerName issuer; /* issuer for crl. indicates cert chain used
                          * to verify. MUST be consistent with rl type: 
                          *    Root                         
                          *          --> BB_CRL_TYPE_CA
                          *    Root-XSCAxxxxxxxx
                          *          --> BB_CRL_TYPE_XS
                          *    Root-CPCAxxxxxxxx 
                          *          --> BB_CRL_TYPE_CP
                          *    Root-MSCAxxxxxxxx
                          *          --> BB_CRL_TYPE_MS
                          */
  u32 numberRevoked;   /* number in this variable length list */
} BbCrlHead;

/* The CRL itself will have the form:
 *     BbRsaSig2048 sig; 
 *     BbRsaSig2048 zeros;
 * in case of Rsa4096 the last two fields are BbRsa4096
 * where the serialNumber[] entries refer to the BbCertId.name field.
 *     BbCrlHead header;
 *     BbCrlEntry serialNumber[header.numberRevoked];
 *     
 *
 */

#endif

