#ifndef __os_bb_sa_h__
#define __os_bb_sa_h__

#include <PR/R4300.h>
#include <PR/bbtypes.h>
#include <PR/bbcert.h>
#include <PR/bbcrl.h>
#include <PR/bbmetadata.h>
#include <PR/bbticket.h>
#include <PR/bbskapi.h>
#include "os_bb.h"

#define RECRYPT_CHUNK_SIZE (8*16*1024) /* NEVER CHANGE THIS AFTER FIRST
                                        * VIEWER RELEASE!!!
                                        */

#define BB_SYSAPP_PASS  0
#define BB_SYSAPP_FAIL -1

#define SACERT_ERR -1
#define SARL_ERR -1

/* informative success codes */
#define SALAUNCH_OK            0 /* may proceed with launch  */
#define SALAUNCH_PERSONALIZE   1 /* personalization required */

/* error codes */
#define SALAUNCH_ERR_FS             -1 /* error opening file on fs */
#define SALAUNCH_ERR_MEM            -2 /* not enough memory (heap) */
#define SALAUNCH_ERR_DATA           -3 /* data error               */
#define SALAUNCH_ERR_EXPIRED        -4 /* ticket expired           */
#define SALAUNCH_ERR_KEYLIST        -5 /* bad keylist              */
#define SALAUNCH_ERR_FILE_NOT_FOUND -6 /* bad keylist              */

/* max tickets in ticket.sys */
#define SA_META_MAX_TICKETS 48

/* max number of certs in cert.sys */
#define SA_MAX_NUM_CERTS 64

/* error codes */
#define SAMETA_ERR_FS          -1 /* error opening file on fs */
#define SAMETA_ERR_MEM         -2 /* not enough memory (heap) */
#define SAMETA_ERR_DATA        -3 /* data error               */
#define SAMETA_ERR_LP_ADVANCE  -4 /* skAdvanceWindow error    */

/* Game MetaData type flags */
#define  SA_META_TYPE_ONCARD  1  /* 1 if is on card */
#define  SA_META_TYPE_TRIAL   2  /* 1 if trial game */

#define SAMETA_TICKET_PERM        0
#define SAMETA_TICKET_LP_VALID    1
#define SAMETA_TICKET_LP_EXPIRED  2


#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

#if defined(_LANGUAGE_C) || defined(_LANGUAGE_C_PLUS_PLUS)

typedef struct {
    u16 thumb_len;      /* gzipped thumbnail length */
    u16 title_img_len;  /* gzipped title img length */
    u8 *thumb_data;     /* pointer to gzipped 56x56 RGBA16 thumbnail*/
    u8 *title_img;     /* pointer to gzipped 264x24 IA8 title img*/
    u8 *title;          /* game title string */
    u8 *isbn;          /* ISBN string */
    u8 *crid;          /* contract registration ID string */
    u8 *oadid;         /* official approval document ID string */
    u32 size;  /* game size in bytes              */
    u32 type;  /* SA_META_TYPE_* flags        */
    u16 code;  /* limit algorithm type for LP games */
    u16 limit; /* limit for LP games */
    u16 cc;    /* consumption count for LP games */
    OSBbLaunchMetaData launch; /* parameters for state saving and launching */
} OSBbSaGameMetaData;

typedef struct {
    u32 numTickets;
    BbTicket ticket[SA_META_MAX_TICKETS];
} OSBbSaTickets;


/* Cert Mgmt API.
 *   
 *   osBbSaCertInit() will occur automatically, but is exposed to
 *   allow re-initialization if necessary (also aids testing).
 */
int osBbSaCertInit();
int osBbSaCertCreateChain(const char *name,BbCertBase **chain);


/* Revocation list Mgmt API.
 */
int osBbSaRlInit();
int osBbSaRlBundle(BbAppLaunchCrls *rls);


/* Ticket and Meta-Data Mgmt API.
 */

/* as currently defined, the ticket.sys file will simply be read
 * into the tickets array since the formats are the same. if in the
 * future the tickey.sys layout changes, this api prevent the gui
 * from requiring change.
 */
int osBbSaMetaGetTickets(OSBbSaTickets *tickets, int verify);

/* reads ticket.sys file, deletes any "invalid" ticket, and groups 
 * the remaining tickets into permanant and LP group, both groups 
 * are sorted according to cids, writes out the cleaned up tickets 
 * back to ticket.sys file, returns the cleaned up tickets in 
 * "tickets" structure. 
 *
 * A ticket is "invalid" and is deleted if:
 * 1. it is a LP ticket and there exists a permanant ticket for
 *    the same cid, or
 * 2. it is an expired LP ticket and there exists >=1 unexpired
 *    LP ticket for the same cid, or
 * 3. it is an expired LP ticket and there exists an expired LP
 *    ticket for the same cid and with a greater tid
 */
int osBbSaMetaCleanTickets(OSBbSaTickets *tickets);

/* metaData:
 *    assumed large enough to hold tickets.numTickets SaGameMetaData 
 *    entries.
 */
int osBbSaMetaGetData(OSBbSaTickets *tickets, OSBbSaGameMetaData *metaData, int trial);


/* Launch API.
 *    (uses the Cert and Rl apis, above)
 */
int osBbSaBundleTicket(BbTicket *ticket,
                   BbTicketBundle *ticketBundle,
                   BbAppLaunchCrls *appRls);
int osBbSaGamePrelaunch(BbTicket *ticket);
int osBbSaGamePersonalize(u8 *chunkBuffer);
int osBbSaGameLaunch(OSBbLaunchMetaData *md);
int osBbSaGameLoadState(OSBbStateVector *sv, u32 bindings[4], BbTicketId tid);


#endif

#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif


#endif
