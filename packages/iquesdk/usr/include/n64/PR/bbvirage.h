#ifndef __BB_VIRAGE_H__
#define __BB_VIRAGE_H__

#include <PR/bcp.h>
#include <PR/bbtypes.h>

/*
 * Structure containing Virage0/Virage1 embedded flash data.
 * Data is ping-ponged between two copies using a version number
 * and checksum to indicate which one is current.
 */
typedef struct {
    u8 tsCrlVersion;	/* largest seen ticket server CRL version number */
    u8 caCrlVersion;	/* largest seen CA  CRL version number */
    u8 cpCrlVersion;        /* content publishing CRL version number */
    u8 contentRlVersion;    /* content revocation list */
    u16 ticketRlVersion;
    u16 tidWindow;	/* base of ticket id window */
#define BB_MAX_CC		26
    u16 cc[BB_MAX_CC];	/* individual consumption counters */
    u16 seq;
    u16 sum;
#define BB_VIRAGE01_CKSUM       0x7adc	/* value cksum should produce */
} BbVirage01;


/*
 * define to allow computing space remaining in BbVirage2 struct
 * (below) for romPatch code area. Must be changed whenever
 * members are added or removed from BbVirage2 typedef.
 */
#define ROM_PATCH_AREA_BYTES \
         ( (VIRAGE2_RAM_END-VIRAGE2_RAM_START) - \
           (sizeof(BbShaHash) + sizeof(BbId) + sizeof(BbEccPublicKey) + \
            sizeof(BbEccPrivateKey) + sizeof(BbAesKey)*4 + \
            sizeof(u32)*2 ) )

/*
 * Structure containing Virage2 embedded flash data.
 * Should never be allocated, since resides at fixed memory
 * position VIRAGE2_RAM_START. Data is written at manufacturing 
 * time and is otherwise read-only.
 * 
 * The romPatch size must be adjusted such that this structure
 * is 256 bytes (VIRAGE2_RAM_END-VIRAGE_RAM_START). Also, members
 * added to this struct must be placed after romPatch due to
 * boot-rom restrictions.
 */
typedef struct {
    BbShaHash skHash;		/* sha1 hash of secure kernel: fixed 
				 0xf0f0f0f0 for now
				*/
    u32 romPatch[ROM_PATCH_AREA_BYTES/4]; /* patch code area for bootrom 
					  64 bytes of 0x55555555 repeated
					*/
    BbEccPublicKey publicKey;	/* player's public key */
    BbId bbId;			/* player's identity: unique serial number */
    BbEccPrivateKey privateKey;	/* player's private key: from ECC key pair */
   
    BbAesKey bootAppKey;	/* decryption key for boot application 
				   license: fixed
				   0x84983e36 0x4706816a 0xba3e2571 0x7850c26c
				 */

    /*
     * The following is 16*3 bytes of random data for various keys.
     * Pulled from RNG.
     */

    BbAesKey recryptListKey;	/* key for recryption list */
    BbAesKey appStateKey;	/* key for state saving */
    BbAesKey selfMsgKey;	/* key for messages to self */
    u32      csumAdjust;        /* checksum adjustment. used to insure checksum
                                   over BbVirage2 complies with defn in bcp.h 
				   for now make it 0x66666666
				*/
    u32      jtagEnable;        /* controls jtag for now =0x00000000
				   enable =0xbb2002fb */
} BbVirage2;

#endif /*__BB_VIRAGE_H__*/


