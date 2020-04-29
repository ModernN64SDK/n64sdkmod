/*
 * diag.h:
 *
 * Common structure definitions and global variables for BOSTON diags.
 *
 * $Revision: 1.2 $
 */

#define diagComm diagcomm
struct diagcomm {
    int init;	   /* Init flag; TRUE -> execute user's init function. */
    int entryNum;  /* First test number to be executed by the diag module. */
    int *skipNums; /* An array of test numbers to be skipped by the diag. */
    int errorCount;/* Total number of errors produced by the diag module. */
    int *errNums;  /* An array of test numbers in which errors occurred.*/
    int argc;	   /* 'C' conventions apply; use getopt() to parse. */
    char *argv[33];/* 'C' conventions apply; use getopt() to parse. */
};

/*
 * Define a global pointer to the communication structure which will be used
 * by errlog() for maintaining error status information as each diagnostic
 * module executes.
 */
extern struct diagComm *pGlobalComm;
extern struct diagComm myComm;
extern int errIndex;

typedef struct {
    char *name;
    int num;
    int (*fnc) ();
} TEST_REF;

/*
 * ide_run.c, ide_init.c will maintain these globals, so that we can access the
 * TEST_REF information at any function call depth of a diagnostic (without 
 * having to pass this info down via parameters).  This makes errlog reporting
 * MUCH easier, since errlog will now use global variables instead of 
 * parameters to get at the subtest info stored in the TEST_REF struct.
 */
extern char *ideTestName;
extern int ideSubTestNum;

/*
 * Declare an external reference for the common diagnostic initialization 
 * function diagInit() that all diagnostic modules must call:
 *
 * diagInit(pComm, testNumbers, initFcn, dispatchFcn)
 *
 * pComm: 	Pointer to the server<->module communication structure.
 * testNumbers:	An array of test numbers (provided by the diagnostic module).
 * initFcn:	Pointer to the diagnostic module's initialization function.
 * dispatchFcn:	Pointer to the diagnostic module's dispatch function.
 */
extern int diagInit();

/*
 * The TEST_NULL value is used to mark the end of valid test numbers in the
 * (unsized) arrays of test numbers.
 */
#define TEST_NULL 0

/*
 * Error reporting levels:
 *
 * INFO_MESSAGE:Informative messages that are printed as each test executes.
 * ERR_SIMPLE:	Used to print individual error messages (e.g. readback errors).
 * ERR_SEVERE:	Used to print a message indicating that a functional block has 
 *		failed.
 * DEBUG_MSG:   Used to print a message for HW or SW debug.
 * DEFAULT_MSGS:Prints all of the non-debug messages by default.
 */

#define INFO    	0x1 
#define ERR_SIMPLE	0x2 
#define ERR_SEVERE	0x4
#define DEBUG           0x8
#define DEFAULT_PRINT_MSGS 	0x7
#define DEFAULT_LOG_MSGS 	0x6
#define DEFAULT_EXIT_MSGS 	0x6
#define INFO_START	0x11
#define INFO_END	0x21
#define INFO_MIDDLE	0x31

#ifdef __sgi__
extern void errlog(...);
#else
extern void errlog(int,...);
#endif
extern int diaginit(TEST_REF test_refs[], int (*initFcn)(),
		    int (*dispatchFcn)());


/*
 * Global Variables
 *
 * Declare external references for the global variables which may be set
 * with commands given to the IDE interpreter.
 *
 * Traplevel: Error Trap Level. Integer value set to one of three levels.
 *            These levels are the following:
 *            1 => Continue on ALL errors
 *            2 => Return on SEVERE errors
 *            3 => Return on ALL errors
 * Scopeflag: When TRUE, it forces the diagnostic module into a scope loop
 * Traceflg:  When TRUE, the diagdr peek/poke functions will print the
 *            address and data that they are writing/reading to/from.
 *
 * NOTE: these variables are defined in diags/ide/ide_cmds.c
 */

extern int printmask;
extern int logmask;
extern int commtype;
extern int exitmask;
extern int Traplevel;
extern int Scopeflag;
extern int Traceflag;
/*
 * Error trap levels:
 *
 * TRAP_NONE:	1 => Continue on ALL errors
 * TRAP_SEVERE:	2 => Return on SEVERE errors
 * TRAP_ALL:	3 => Return on ALL errors
 */

#define TRAP_NONE    	1 
#define TRAP_SEVERE	2
#define TRAP_ALL	3

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

extern void ide_set_clean(void (* user_clean)(void));
