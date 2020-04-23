
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: hcontroller.h,v $
        $Revision: 1.1 $
        $Date: 1999/07/05 02:19:20 $
 *---------------------------------------------------------------------*/
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

/**************************************************************************
 *									  *
 *		 Copyright (C) 1995, Silicon Graphics, Inc.		  *
 *									  *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright law.  They  may  not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *									  *
 **************************************************************************/

#include <PR/ultratypes.h>

#if	0			
#define _PFS_1M_EXTENSION	1	/* for New 1M-PAK lib */
#endif

#define PFS_ONE_PAGE		8
#define PFS_PAGE_SIZE		(BLOCKSIZE*PFS_ONE_PAGE)

#define PIFRAMSIZE		16
#define PFS_INODE_SIZE_PER_PAGE	128
#define PFS_SIZE_LOC  		24
#define DEF_DIR_PAGES		2

#define PFS_ID_0AREA		1
#define PFS_ID_1AREA		3
#define PFS_ID_2AREA		4
#define PFS_ID_3AREA		6
#define PFS_LABEL_AREA		7
#define PFS_ID_PAGE     	PFS_ONE_PAGE * 0

#define PFS_WRITTEN		2

/* definition for page usage */
#define PFS_EOF			1
#define PFS_PAGE_NOT_EXIST	2
#define PFS_PAGE_NOT_USED	3

/* definition for memorypack size */
#define PFS_32K_RAM		1
#define PFS_64K_RAM		2

#define PFS_FORCE		1
#define PFS_DELETE		1

/* definition for bank */
#define	PFS_ID_BANK_256K	0
#define	PFS_ID_BANK_1M		4
#define	PFS_BANKS_256K		1

/* definition for EEPROM */
#define EEPROM_WAIT		12000	/* micro sec */

/*
#define	__osGetId	osPfsRepairId
*/
typedef struct {
	u32	ramarray[PIFRAMSIZE-1];		/* RAM */
	u32	pifstatus;		
} OSPifRam;

/* command type */

#define CONT_REQUEST	0
#define CONT_READ	1
#define CONT_RAM_READ	2
#define CONT_RAM_WRITE	3
#define CONT_EEPROM_READ	4
#define CONT_EEPROM_WRITE	5
#define CONT_SETCH	254
#define CONT_RESET	255
#define	CONT_ETC	CONT_SETCH

#define CON_ERR_MASK		0xc0

#ifndef _HW_VERSION_1
#define CONT_FORMAT		1	
#define CHANNEL_RESET		0xfd	 
#define FORMAT_END		0xfe	 
#endif

typedef struct {
	u8	dummy;			/* for word aliening */
	u8	txsize;			/* Tx size */
	u8	rxsize;			/* Rx size */
	u8	cmd;			/* commmand */
	u16     button;
	s8      stick_x;
	s8      stick_y;
} __OSContReadFormat; 

/* request/reset format */
typedef struct {
	u8	dummy;                  /* for word aliening */
	u8	txsize;			/* Tx size */
	u8	rxsize;			/* Rx size */
	u8	cmd;			/* commmand */
	u8      typeh;
	u8      typel;
	u8      status;
	u8	dummy1;                  /* for word aliening */
} __OSContRequesFormat; 

/* request/reset format */
typedef struct {
	u8	txsize;			/* Tx size */
	u8	rxsize;			/* Rx size */
	u8	cmd;			/* commmand */
	u8      typeh;
	u8      typel;
	u8      status;
} __OSContRequesFormatShort; 

/* ram read/write format */
typedef struct {
	u8	dummy;                  /* for word aliening */
	u8	txsize;			/* Tx size */
	u8	rxsize;			/* Rx size */
	u8	cmd;			/* commmand */
	u8	addrh;
	u8	addrl;
	u8      data[BLOCKSIZE];
	u8      datacrc;
} __OSContRamReadFormat; 


typedef union {
	struct {
		u8	bank;
		u8	page;
	}inode_t;
	u16	ipage;
} __OSInodeUnit;

typedef struct {
	u32 		game_code;
	u16     	company_code;
	__OSInodeUnit   start_page;
	u8      	status;
	s8		reserved;
	u16     	data_sum;
	u8	    	ext_name[PFS_FILE_EXT_LEN];
	u8	    	game_name[PFS_FILE_NAME_LEN];
} __OSDir;

typedef struct {
	__OSInodeUnit	inode_page[PFS_INODE_SIZE_PER_PAGE];
} __OSInode;


typedef struct {
	u32	repaired;
	u32	random;
	u64	serial_mid;
	u64	serial_low;
	u16	deviceid;
	u8	banks;
	u8	version;
	u16	checksum;
	u16	inverted_checksum;
} __OSPackId;

/* eeprom read/write format */
typedef struct {
	u8	txsize;			/* Tx size */
	u8	rxsize;			/* Rx size */
	u8	cmd;			/* commmand */
	u8      address;
	u8      data[EEPROM_BLOCK_SIZE];
} __OSContEepromFormat; 

/***** BY YASU */
#define	PFS_BANK_LAPPED_BY	8	/* => u8 */
#define	PFS_SECTOR_PER_BANK	32
#define	PFS_INODE_DIST_MAP	(PFS_BANK_LAPPED_BY * PFS_SECTOR_PER_BANK)
#define	PFS_SECTOR_SIZE		(PFS_INODE_SIZE_PER_PAGE/PFS_SECTOR_PER_BANK)

typedef	struct {
  __OSInode	inode;
  u8		bank;
  u8		map[PFS_INODE_DIST_MAP];
} __OSInodeCache;

extern OSPifRam		__osContPifRam;		/* PIF RAM buffer */
extern OSPifRam		__osPfsPifRam;		/* Memory Pack buffer */
extern OSPifRam		__osEepPifRam;		/* EEPROM RAM buffer */
extern u8	 	__osContLastCmd;	/* last pifmacro command */
extern u8		__osMaxControllers;	/* # of connected controllers */
extern OSTimer		__osEepromTimer;
extern OSMesgQueue	__osEepromTimerQ;
extern OSMesg		__osEepromTimerMsg;

extern u8		__osPfsInodeCacheBank;
extern __OSInode	__osPfsInodeCache;
extern int		__osPfsInodeCacheChannel;

extern void __osPackRequestData(u8 );
extern void __osPackResetData(void );
extern void __osContGetInitData(u8 *, OSContStatus *);
extern s32 __osContRamRead(OSMesgQueue *, int, u16, u8 *);
extern s32 __osContRamWrite(OSMesgQueue *, int, u16, u8 *, int);
extern u8 __osContAddressCrc(u16);
extern u8 __osContDataCrc(u8 *);
extern u16 __osSumcalc(u8 *,int);
extern s32 __osIdCheckSum(u16 *, u16 *, u16 *);
extern s32 __osCheckPackId(OSPfs *, __OSPackId *);
extern s32 __osGetId(OSPfs *);
extern s32 __osCheckId(OSPfs *);
extern s32 __osPfsRWInode(OSPfs *, __OSInode *, u8, u8);
extern s32 __osPfsGetStatus(OSMesgQueue *, int);
extern s32 __osPfsDeclearPage(OSPfs *, __OSInode *, int, int *, u8,int *,int *);
extern s32 __osPfsReleasePages(OSPfs *, __OSInode *,u8,u8, __OSInodeUnit *);
extern void __osPfsRequestData(u8 );
extern void __osPfsGetInitData(u8 *, OSContStatus *);
extern s32 __osEepStatus(OSMesgQueue *, OSContStatus *);
extern s32 __osPfsSelectBank(OSPfs *, u8);
extern s32 __osPfsSelectIdBank(OSPfs *);
extern void __osPfsRequestOneChannel(int);
extern void __osPfsGetOneChannelData(int, OSContStatus *);
extern s32 __osRepairPackId(OSPfs *, __OSPackId *, __OSPackId *);

#endif /* _CONTROLLER_H_ */
