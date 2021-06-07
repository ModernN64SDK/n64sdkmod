/**********************************************
 *
 * BbContentMetaData.h
 *
 * Definition of signed content meta data (like a "content certificate")
 * issued for each content.
 *
 **********************************************/

#ifndef __BB_METADATA_H_
#define __BB_METADATA_H_

#include <PR/bbtypes.h>

/* 
 * content meta-data defines (CMD) and type flags 
 */

/* bytes of opaque content description data. must be a multiple of 512B */
#define BB_CMD_DESC_SIZE (1024*10)

/* CMD content description and type flags */
#define BB_CMD_DESC_COMMON_KEY 1   /* encrypted with common key */

/* CMD content execution flags */
#define BB_CMD_EXEC_COMPRESSED  1  /* boot section of content compressed */
#define BB_CMD_EXEC_RECRYPT     2  /* re-encryption required */

/* CMD hwAccessRights defines */
#define BB_CMD_HWAR_PI_MASK    0x000000ff /* same as PI_ACCESS bit meanings */
#define BB_CMD_HWAR_USB_MASK   0x00000100
#define BB_CMD_HWAR_IRAM_MASK  0x00000200

/* constant that must be the number of bytes to be encrypted in the
 * BbContentMetaDataHead struct.
 */
#define BB_CMD_ENCRYPTED_CHUNK_BYTES (sizeof(BbAesKey))

#define BB_CMD_HEAD_SIGNED_BYTES  ( sizeof(BbContentMetaDataHead) - \
                                    sizeof(BbRsaSig2048) )


/************************************************
 * "header" fields of content meta-data for any published 
 * content for the BB system. The indicated portions 
 * will be encrypted when they appear in the bbTicket. 
 ***********************************************/ 
typedef struct {
    /* 
     * CRL info
     */
    u32 unusedPadding;
    u32 caCrlVersion;
    u32 cpCrlVersion;

    /*
     * High level content information that is the same in all tickets.
     */
    u32         size; 
    u32         descFlags;   /* content description flags, BB_CMD_DESC_* */
    BbAesIv     commonCmdIv; /* the common key is used to encrypt the "key"
                              * field, further down in the struct. this iv
                              * is used in conjunction with that key.
                              */
    /*
     * Content security items that are the same in all tickets.
     */
    BbShaHash hash; /* SHA hash of content   */
    BbAesIv   iv;   /* initialization vector associated with "key" field
                     * appearing further down in this struct.
                     */
    
    /*
     * Content execution information.
     */
    u32 execFlags;      /* content execution flags, BB_CMD_EXEC_* */
    u32 hwAccessRights; /* see BB_CMD_HWAR_*_MASK  */
    u32 secureKernelRights; /* define flags defining alloweable skapi
                             * usage. a flag for each ordinal position
                             * in the skapi call table.
                             */

    u32 bbid;          /* if this field is non zero, this is the only
                        * bb on which this content executes. if 
                        * it is zero, this field is ignored 
                        */

    /* signer who supplies last field, contentMetaDataSign. */
    BbServerName issuer;

    /* server design caches portion of this struct above here (information
     * that is common for all players). the server design insists the
     * content id also appears below, even though it is common for all
     * players.
     */

    BbContentId id;   
    BbAesKey  key;  /* published content key. THE ONLY PORTION OF THE
                     * cmdh AND TICKET TO BE ENCRYPTED. during content
                     * publishing "key" is encrypted using the common
                     * key (virage), and during ticketing "key" is
                     * encrypted by the ticket server.
                     */

    /*
     * RSA 2048 bit sign on all data upto this point. 
     */
    BbRsaSig2048 contentMetaDataSign; 
} BbContentMetaDataHead;


/************************************************
 * BbContentMetaData: 
 *    complete metadata.
 ***********************************************/ 
typedef struct {
    u8 contentDesc[BB_CMD_DESC_SIZE]; /* large, opaque portion of CMD */
    BbContentMetaDataHead head;
} BbContentMetaData;


#endif 
