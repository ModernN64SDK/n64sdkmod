/*
 * Copyright (c) 1995, Silicon Graphics, Inc.  All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics, Inc.;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior written
 * permission of Silicon Graphics, Inc.
 *
 * RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to restrictions
 * as set forth in subdivision (c)(1)(ii) of the Rights in Technical Data
 * and Computer Software clause at DFARS 252.227-7013, and/or in similar or
 * successor clauses in the FAR, DOD or NASA FAR Supplement.  Unpublished
 * rights reserved under the Copyright Laws of the United States.
 *
 * Module: dbg_comm.h: typedefs & constants for use with libstand.a
 */

#define DEFAULT_MASTER_PORT	5777
#define BUFSIZE			1236992 /* 302x4096 bytes */
#define ST_NO_ALARM     	1

/*  
 * Default timeouts (in seconds)
 */
#define	TTY_TIME_OUT	5
#define	TCP_TIME_OUT	2

#define METHOD_CPU	0
#define METHOD_RCP	1

/*
 * dbg_comm_struct is based on the C++ class "Settop", borrowed from the
 * Settop OS.
 */
typedef struct dbg_comm_struct_t {
    short        	_masterClient; /* 1 => this is the master client */
    int          	fildes;        /* UDP socket file descriptor */
    char        	*buf;           /* Buffer to store packet */
    unsigned short	_port;
    unsigned int	bufsize;
    int			how;            /*  TCP/TTY */
    int			flags;
    int			lastRequest;    /* last request made */
    int			logfile;
    int      		method;		/* METHOD_CPU or METHOD_RCP */
    struct sockaddr_in	serv_addr;	/* Server address */
    struct sockaddr_in	cli_addr;	/* Client address */
} dbg_comm_struct;

extern int dgReadWord(unsigned int addr, unsigned int *word);
extern int dgWriteWord(unsigned int addr, unsigned int word);
extern int dgReadMem(unsigned int addr, unsigned int nbytes, char *buffer);
extern int dgWriteMem(unsigned int addr, unsigned int nbytes, char *buffer);
extern int dgInitComm(void);

extern int dgListSubTests(TEST_REF *test_refs);
extern int dgInitRdram(void);
extern int dgTestReg(unsigned int address, unsigned int write_data,
                   unsigned int expect_data);
extern int dgTestRegMsk(unsigned int address, unsigned int write_data, 
                        unsigned int expect_data, unsigned int mask);
extern int dgRdTestWordMsk(unsigned int address, unsigned int expect_data, 
			   unsigned int mask);
extern void dgWait(char *fmt, ...);
extern void dgTrigger(void);
extern int dgCompareWord(unsigned int address, unsigned int expect_data);
extern int dgEnableVideo(unsigned int start_address, int mode);

extern int dgDmaReadMem(unsigned int ramromAddr, 
			unsigned int rdramAddr, unsigned int nbytes);
extern int dgDmaWriteMem(unsigned int ramromAddr, 
			unsigned int rdramAddr, unsigned int nbytes);

/*
 * Communication types supported by libdg:
 */
#define DG_VERILOG		1
#define DG_TINYMON_PIF		2
#define DG_TINYMON_IMEM		3
#define DG_TINYMON_DMEM		4
#define DG_TINYMON_RDRAM	5
#define DG_RMON			6
#define DG_RMON_RESET		7

/*
 * Global variables to be maintained by dgInitComm():
 *
 * dgMemFree:	pointer to first word of memory that's available after the 
 *		monitor (monitor will be loaded at fixed locations selected
 *		by the commtype variable).
 *
 * dgRdramFree:	pointer to first word of rdram that's freely available
 *		(e.g. after the exception vector table, or after the exception
 *		vector table AND monitor).  We can assume that tinymon will be
 *		loaded in rdram immediately following the exception vector
 *		table.  For the standard rmon case, this model breaks down due
 *		to discontiguous rdram usage (e.g. exception vectors, 
 *		multiple program segments, and stack).  My suggestion: don't
 * 		run tests that depend on these variables when running with 
 *		rmon.
 * dgInitialize: Initicate if the target system should be initialized.
 *
 */
extern int dgMemFree;
extern int dgRdramFree;
extern int dgInitialize;
extern int dgIORevision;
extern int dgRACRevision;
extern int dgRDPRevision;
extern int dgRSPRevision;

