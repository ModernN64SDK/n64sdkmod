/**********************************************
 *
 * BbTicket.h
 *
 * Definition of eTicket (or content license) for client and server
 * store and forward each field in network order (big endian) to client
 *
 **********************************************/

#ifndef __BB_TICKET_H_
#define __BB_TICKET_H_

#include <PR/bbtypes.h>
#include <PR/bbmetadata.h>

#define BB_TICKET_ID_PERMANENT	0x0	/* base id for permanent ids */
#define BB_TICKET_ID_LIMITED	0x8000	/* base id for limited ids */
#define BB_TICKET_ID_INVAL	0xffff	/* internal marker */

#define BB_LIMIT_CODE_TIME2	0	/* time limited play, uses watchdog */
#define BB_LIMIT_CODE_COUNT	1	/* count limited play */
#define BB_LIMIT_CODE_TIME	2	/* time limited play */
#define BB_LIMIT_CODE_EXTERN	3	/* external limit */
#define BB_LIMIT_CODE_INVAL     0xffff	/* internal marker */

/************************************************
 * BbTicketHead:
 *     broken out this way so it may be separately
 *     allocated from the ticket, since the ticket
 *     requires the large content description field.
 ***********************************************/ 
typedef struct {
    BbId bbId;
    BbTicketId tid;
    u16 code;		/* limit algorithm */
    u16 limit;		/* limit value */
    u16 reserved;	/* future expansion */
  
    /*
     * CRL information  - appended by eTicket server (unencrypted).
     *       as many possible revocations as BBs.
     */
  
    u32 tsCrlVersion;
  
    /*
     * Decryption/verification related information - 
     * appended by eTicket server(unencrypted)
     */
    BbAesIv cmdIv;                /* iv used when decrypting cmd field */
    BbEccPublicKey serverKey;     /* ephemeral public key */
    BbServerName issuer;          /* name of issuing server */
    BbRsaSig2048 ticketSign;      /* RSA 2048 bit sign on entire ticket*/
    
} BbTicketHead;

/************************************************
 * BbTicket: 
 *    complete ticket.
 ***********************************************/ 
typedef struct {
    BbContentMetaData cmd; /* portions will be encrypted. */
    BbTicketHead head;     /* "header" for ticket. */
} BbTicket;


#endif 


