#ifndef __BBCARD_H__
#define __BBCARD_H__

#ifdef __cplusplus
extern "C" {
#endif

#define BBC_OK 0	/* Successful return value */
#define BBC_NOFORMAT -1	/* Card has errors and requires reformat */
#define BBC_NOID -2	/* BBID is not present on Card */
#define BBC_NOSPACE -3	/* No space left on Card */
#define BBC_NOCARD -4	/* Card has been removed */
#define BBC_CARDCHANGE -5	/* Init() has not been called */
#define BBC_TOOMANY -6	/* Too many active handles */
#define BBC_BADHANDLE -7	/* Invalid handle */
#define BBC_BADLENGTH -8	/* Invalid object length */
#define BBC_CARDFAILED -9	/* Card has too many bad blks or is unusable */
#define BBC_ERROR -10	/* Generic error */
#define BBC_EIO -11	/* IO error (USB unplugged) */
#define BBC_DATAERR -12	/* block read with DB ecc error */
#define BBC_NODEV -13	/* device not connected */
#define BBC_NOFILE -14  /* file does not exist on Card */
#define BBC_BADBLK -15	/* block marked bad (status != 0xff) */
#define BBC_SA1DIFFERS -16	/* BBCVerifySKSA2 finds SA1 different */
#define BBC_MULTIPLE_BB -17     /* BBCInit() found multiple BB connection */
#define BBC_MULTIPLE_FILE -18   /* cid mapped to both .app and .rec files */
#define BBC_SYNCLOST -19 /* Protocol synchronization lost */
#define BBC_NOMEM -20 /* Unable to allocate memory */
#define BBC_BADARG -21 /* Invalid argument */
#define BBC_DEVBUSY -22 /* Device already opened */
#define BBC_DEVERR -23 /* Device error */
#define BBC_TIMEOUT -24 /* Timeout */
#define BBC_SKWRITEFAIL -25 /* Write error during update of SK */
#define BBC_VERIFYFAIL -26  /* Data not written correctly to card */

/* Force flags for BBCVerifySKSA2 */
#define BBC_SKSA_FORCE_ALL      3 /* Forces the replacement of SKSA */
#define BBC_SKSA_FORCE_NOSKIP   2 /* Checks SKSA, if diff, replaces entire SKSA */
#define BBC_SKSA_FORCE_SKIP     1 /* Checks SKSA, if diff, replaces SKSA 
                                     starting at first differing block */
#define BBC_SKSA_FORCE_SA2      0 /* Checks SKSA, if SA2 diff, replaces SA2,
                                     if SK or SA1 diff, returns BBC_SA1DIFFERS */

/*
 * Typically all functions return one of the above
 * error codes in case of errors. Success is indicated by
 * a return value of zero or a positive integer in case of
 * writes which indicates the number of bytes written.
 */

/*
 * BBCInit() must be called before any operations can
 * be performed on a card. If Init has not been called on
 * the card that is currently inserted, then BBC_CARDCHANGE
 * is returned.
 * 
 * Success is indicated by a return of zero, which indicates
 * that the card is good and all necessary files are present.
 * Depot may proceed with normal operations. In case of
 * error, Depot has to proceed with recovery steps.
 * Recovery consists of reformatting the entire
 * flash card.
 *
 * Once all operations on the card have been completed,
 * call BBCClose();
 */
#define BBC_NDELAY	0x0001	/* non blocking call */
#define BBC_SYNC	0x0002	/* synchronous call */

typedef int BBCHandle;

/*
 * device is the USB reader device, e.g, /proc/bus/usb/001/002
 */
#ifndef WIN32
BBCHandle BBCInit(const char *device, int callType);
#else
BBCHandle BBCInit(int callType);
#endif
int BBCClose(BBCHandle h);

/*
 * Determine whether a card is present (boolean)
 */
int BBCCardPresent(BBCHandle h);

/*
 * Manipulate the LED on the BBCard Reader
 *
 * There is one LED that has two independently settable
 * colors, giving 4 possibilities.
 */
int BBCSetLed(BBCHandle h, int ledmask);
int BBCGetLed(BBCHandle h, int *ledmask);

#define BBC_LED_OFF	0x0000
#define BBC_LED_GREEN	0x0001
#define BBC_LED_RED	0x0002
#define BBC_LED_ORANGE	(BBC_LED_RED|BBC_LED_GREEN)

/*
 * Set the time on the player (does nothing on BBCard Reader)
 *
 * Time parameter is the return value from time(2) (seconds
 * since Epoch in UTC).  The time on the player will be set
 * to the corresponding local time.
 */
int BBCSetTime(BBCHandle h, time_t curTime);


/*
 * cardSize is in kbytes
 * reservedBlks, freeBlks and badBlks are in 16KB
 * In case of error, Depot will initiate recovery procedures
 */
int BBCStats(BBCHandle h, int *cardSize, int *reservedBlks, 
             int *freeBlks, int *badBlks);

/*
 * cardSize is in kbytes
 * sysSpace is in kbytes, is the total size of SKSA area, the  
 *     budgeted bad blocks, various *.sys files and state files 
 * contentSpace is in kbytes and is the total size of all the  
 *     .app and *.rec files
 * freeSpace is in kbytes and is the maximum space available 
 *     on card for content to be written and launched
 * In case of error, Depot will initiate recovery procedures
 */
int BBCStats2(BBCHandle h, int *cardSize, int *sysSpace, 
             int* contentSpace, int* freeSpace);

/* maximum state+ticket size per game is 256KB plus one ticket */

#define BBC_MAX_GAME_STATE_SIZE  (262144+16384)

/*
 * Returns the identity of the BB Card. May be called even if the
 * Init() or Stats() fails, in which case this will be recovered from any
 * backup copies etc. Returns the number of bytes written into
 * the buffer. BBC_NOID is returned if the 
 * the bbID cannot be recovered. The first 4 bytes of the buf
 * if present, will contain the bbHwRev. The remaining bytes
 * are for the BB Certificate, if present.
 */
int BBCGetIdentity(BBCHandle h, int *bbID, void* buf, int len);
int BBCStoreIdentity(BBCHandle h, int bbID, const void* buf, int len);

/*
 * Returns the identity of the actual BB player, as opposed to the
 * BB Card.  May be called even when no card is present.
 * The iQueAtHome version of the depot may allow games to
 * be downloaded onto a card that does not match the player being
 * used for the download. 
 *
 * Returns BBC_NODEV when called on a card reader.  
 */
int BBCGetPlayerIdentity(BBCHandle h, int *bbID);


/*
 * Formats the card, including bad block mapping etc.
 * Upon success, the Depot may proceed with copying all
 * of the necessary files on to the Card, which include
 * id.sys, crl.sys, ticket.sys, system area, cert.sys
 * These files may be of size zero if there is no data.
 * The sequence that will be followed is
 *	BBCFormatCard();
 *	BBCStoreIdentity();
 *	BBCVerifySKSA();
 *	BBCStoreCrls();
 *	BBCStoreCert();
 *	BBCStoreTickets();
 *	BBCStorePrivateData();
 *	BBCClose();
 */
int BBCFormatCard(BBCHandle h);

/*
 * BB Depot private data is stored in this file. Initially
 * we're expecting to store just the timestamp of last update.
 */
int BBCPrivateDataSize(BBCHandle h);
int BBCGetPrivateData(BBCHandle h, void* buf, int len);
int BBCStorePrivateData(BBCHandle h, const void* buf, int len);

/*
 * BB Depot user registration data
 */
int BBCUserDataSize(BBCHandle h);
int BBCGetUserData(BBCHandle h, void* buf, int len);
int BBCStoreUserData(BBCHandle h, const void* buf, int len);

/*
 * eTicket operations to get size of the "file", read eTickets
 * and update them. len is the size in bytes.
 */
int BBCTicketsSize(BBCHandle h);
int BBCGetTickets(BBCHandle h, void* buf, int len);
int BBCStoreTickets(BBCHandle h, const void* buf, int len);

/*
 * crl operations to get size of the "file", read crl
 * and update them. len is the size in bytes.
 */
int BBCCrlsSize(BBCHandle h);
int BBCGetCrls(BBCHandle h, void* buf, int len);
int BBCStoreCrls(BBCHandle h, const void* buf, int len);

/*
 * Certificate operations to get size of the "file", read certs
 * and update them. len is the size in bytes.
 */
int BBCCertSize(BBCHandle h);
int BBCGetCert(BBCHandle h, void* buf, int len);
int BBCStoreCert(BBCHandle h, const void* buf, int len);

/*
 * BBCContentListSize returns the number of content in the BB Card.
 * Size is in bytes.
 * BBCRemoveContent does not remove signed game data.
 */
typedef unsigned int ContentId;
typedef unsigned int EccSig[16];
#define BBC_CONTENT_APP 1
#define BBC_CONTENT_REC 2
int BBCListContentFiles(BBCHandle h, ContentId id);
int BBCContentListSize(BBCHandle h);
int BBCGetContentList(BBCHandle h, ContentId list[], int size[], int nentries);
int BBCStateListSize(BBCHandle h);
int BBCGetStateList(BBCHandle h, ContentId list[], int nentries);
int BBCContentSize(BBCHandle h, ContentId cid, int* type);
int BBCGetContent(BBCHandle h, ContentId cid , void* buf, int len, int* type);
int BBCStoreContent(BBCHandle h, ContentId id, const void* buf, int size);
int BBCRemoveContent(BBCHandle h, ContentId id);
int BBCRenameState(BBCHandle h, ContentId from, ContentId to);
int BBCStateSize(BBCHandle h, ContentId id);
int BBCGetState(BBCHandle h, ContentId id, void *buf, int len, EccSig sig, int* dirty);


/*
 * Update the SK and SA on the Card if it does not match the
 * content that is passed in. Len is in bytes. Returns 0 if
 * contents were not updated, else returns the number of bytes
 * written. The actual layout is SK + SA license + SA. 
 * SK is fixed in size at 64KB
 * SA License is fixed in size at 16KB
 */
int BBCVerifySKSA(BBCHandle h, const void* buf, int len);

/*
 * To support iQueAtHome, the SA has been split into two pieces:
 *   SA1 which supports update via USB and should not change
 *   SA2 the current viewer code which can be updated
 * The layout is
 *   SK + T1 + SA1 + T2 + SA2
 * where T1 and T2 are the 16KB ticket blocks for SA1 and SA2
 *
 * Under normal circumstances, only T2 and SA2 should change,
 * so that the user in not exposed to the risk of a disfunctional
 * system if the update is interrupted.  Depending on the value
 * of the "force" parameter, portions of the SKSA will be updated.
 *
 * "force" parameter       Action
 * BBC_SKSA_FORCE_ALL    - Everything will be updated (without checking)
 * BBC_SKSA_FORCE_NOSKIP - SKSA will be checked against the buffer passed in.
 *                      .  If it is different, then the entire SKSA will 
 *                         be updated.
 * BBC_SKSA_FORCE_SKIP   - SKSA will be checked against the buffer passed in.
 *                      .  If it is different, the the part of the SKSA 
 *                         starting from the first different block will 
 *                         be updated.
 * BBC_SKSA_FORCE_SA2    - SKSA will be checked but only SA2 will be updated.
 *                         The error code BBC_SA1DIFFERS will be returned if 
 *                         there is anything different in SK, T1 or SA1 
 *                         versus the buffer passed in.
 *
 * Return values:
 *   0 = the current contents agree, nothing was done
 *   > 0 => the length of data that was written (error if != "len")
 *   < 0 => error occurred
 */
int BBCVerifySKSA2(BBCHandle h, const void* buf, int len, int force);

/*
 * Challenge response authentication for BB
 *   Challenge string is normally a 160-bit SHA1 hash
 *   BB returns ECC signature of the challenge, using its private key
 *
 *   chlen is the length of the challenge in bytes
 *   sig points to the buffer to receive the signature
 *   siglen is the length of the buffer to be checked by 
 *
 * The parameters are typed in a generic way, but in the BB case,
 * the challenge should be of type BbShaHash and the signature
 * returned will be of type BbEccSig.
 *
 * Returns 0 if successful (caller must still validate the returned signature)
 * Returns BBC_BADLENGTH if sig buffer length too short
 * Returns BBC_NODEV on card reader
 */
int BBCAuthChallenge(BBCHandle h, const void *chal, int chlen, void *sig, int siglen);

/* 
 * Return number of bytes read by FRead
 */
int BBCGetBytesRead();

/* 
 * Return number of bytes written by FWrite
 */
int BBCGetBytesWritten();

/* LibBBC commands for diagnostics */

/*
 * Scans the BB card for blocks marked as bad by manufacturer
 * and blocks with DBE.  When used with USB, this operation
 * can take up to 3 minutes and is to be used for diagnostics only.
 *
 * If rewrite_dbe is set, will also attempt to rewrite blocks
 *  that have double bit errors.
 */
int BBCDiagScanBadBlocks(BBCHandle h, int rewrite_dbe);
/* Reads the SKSA area (all 64 blocks) and prints the hash for each block */
int BBCDiagSKSABlocksHash(BBCHandle h);
/* Same as BBCFormatCard except with option to clear bad blocks */
int BBCFormatCard2(BBCHandle h, int clear_bad_blocks);

#ifdef BBC_DIAG
#ifdef WIN32
VOID BBCDiagPrintInfo();
#endif
#endif

/* Set BBC diagnostics mod and log variables */
int BBCSetDiagMode(int mode);
int BBCSetLogLevel(int level);
int BBCSetLogFile(const char* filename);

#ifdef  __cplusplus
}
#endif

#endif /* __BBCARD_H__ */
