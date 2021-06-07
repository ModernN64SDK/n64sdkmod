#ifndef __BB_CERTS_H__
#define __BB_CERTS_H__

/*
 * BB Certificates.
 * All big endian representation when stored or transmitted.
 */

#include <PR/bbtypes.h>

#define BB_CERT_TYPE_BB      0   /* BB player cert, holds ECC pub key */
#define BB_CERT_TYPE_SERVER  1   /* BB server cert, holds RSA2048 pub key.
                                  * BB_SIG_TYPE_* is used to determine 
                                  * signing method. For CA servers, sig
                                  * will be RSA4096. For all other servers
                                  * sig will be RSA2048.
                                  */

#define BB_RSA_CERT_SIGNED_BYTES (sizeof(BbRsaCert) - sizeof(BbGenericSig))
#define BB_ECC_CERT_SIGNED_BYTES (sizeof(BbEccCert) - sizeof(BbGenericSig))

/* 
 * fixed string defs used for BbServerName as issuer.
 * NOTE: strings may change, but absolute position is assumed to be
 * fixed so string lengths must remain fixed. 
 */
#define BB_CERT_STR_DELIM        "-"
#define BB_CERT_STR_ROOT         "Root"
#define BB_CERT_STR_XS_PREFIX    "XS"
#define BB_CERT_STR_CP_PREFIX    "CP"
#define BB_CERT_STR_XSCA_PREFIX  "XSCA"
#define BB_CERT_STR_CPCA_PREFIX  "CPCA"
#define BB_CERT_STR_XSCA_CHAIN   BB_CERT_STR_ROOT BB_CERT_STR_DELIM \
                                 BB_CERT_STR_XSCA_PREFIX 
#define BB_CERT_STR_CPCA_CHAIN   BB_CERT_STR_ROOT BB_CERT_STR_DELIM \
                                 BB_CERT_STR_CPCA_PREFIX 

/*
 * fixed maximum length for cert-chain
 */
#define BB_CERT_CHAIN_MAXLEN 5

/*
 * Common identification for all certs.
 * Will be first field in all cert typedefs.
 */
typedef struct {
    /* fixed length fields */
    u32 certType;     /* key type held by cert, BB_CERT_TYPE_* */
    u32 sigType;      /* signature used for cert, BB_SIG_TYPE_* */

    u32 date;		/* for server, unix style timestamp */
  
    BbServerName issuer;  /* name used to id issuing server */

    /* the name type a cert is bound to depends on the certType field. */
    union {
        BbServerSuffix  server; /* BB_CERT_TYPE_SERVER. Holds server suffix
                                 * name, containing serial number (i.e.,
                                 * TSxxxxxxxx, where xxxxxxxx is s/n).  */
        BbName bbid;        /* BB_CERT_TYPE_BB, where the serial number
                            * is the id BBxxxxxxxx.*/
    } name;
} BbCertId;

typedef BbCertId BbCertBase;

/*
 * use this for ECC certificates for BB 
 * ECC public key, signed RSA 2048 
 */
typedef struct {
    BbCertId certId;    /* this cert id info */
    u32 publicKey[16];	/* 512 bits for two co-ordinates for ECC*/
    BbGenericSig signature; /* may only be RSA2048 for this cert type */
} BbEccCert;


/*
 * use this for RSA certificates, fixed length accomodates upto 4096 RSA
 * signature
 */
typedef struct {
    BbCertId certId;    /* this cert id info */
    BbRsaPublicKey2048 publicKey;
    BbRsaExponent  exponent;
    BbGenericSig signature;	
                        /* could be RSA4096 or RSA2048 depending on 
                         * certId.sigType.
			 * signature is over all contents upto here
			 * padded as specified by SHA-1 and
			 * padded again for RSA
			 */
} BbRsaCert;


#endif
