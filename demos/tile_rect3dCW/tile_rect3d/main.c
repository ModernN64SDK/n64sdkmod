/* main.c */

/* system includes
 */
#include <ultra64.h>

/* project includes
 */
#include "main.h"

/* functions prototype
 */
void boot( void ) ;
static void idle ( void * ) ;
void main( void * ) ;

/* threads
 */
static OSThread	idleThread ;
static OSThread	mainThread ;
static u64 idleThreadStack[STACK_SIZE_IDLE / sizeof(u64)] ;
static u64 mainThreadStack[STACK_SIZE_IDLE / sizeof(u64)] ;

/* message queue
 */
static OSMesg PiMessages[NUM_PI_MESSAGES] ;
static OSMesgQueue PiMessageQueue ;

/* global variables
 */


/* application entry point
 */
void boot( void )
{
	osInitialize() ;
	
	osCreateThread(	&idleThread, 
					1,
					idle, 
					NULL,
					idleThreadStack + STACK_SIZE_IDLE / sizeof(u64),
					10 ) ;
	osStartThread( &idleThread ) ;
	/* do no execute following line */
}

/* function of idle thread
 */
static void idle ( void * )
{
	/* First, Initialize the ViManager
	 */
	osCreateViManager( OS_PRIORITY_VIMGR );
	osViSetMode( &osViModeTable[OS_VI_NTSC_LPN1] ) ;
 	osViSetSpecialFeatures( OS_VI_DITHER_FILTER_ON | OS_VI_DIVOT_OFF ) ;	
	osViBlack( TRUE ) ;
	
	/* Next, Initialize the PI Manager
	 */
	osCreatePiManager( (OSPri)OS_PRIORITY_PIMGR,
					   &PiMessageQueue,
					   PiMessages,
					   NUM_PI_MESSAGES ) ;
	
	/* Then, Create some Threads
	 */
	osCreateThread( &mainThread,
					3,
					main,
					NULL,
					mainThreadStack + STACK_SIZE_MAIN / sizeof(u64),
					10 ) ;
	osStartThread( &mainThread ) ;

	/* Finally, Become the Idle Thread
	 */
	osSetThreadPri( 0, 0 ) ;
	
	/* do nothing
	 */
	while( 1 ){}
}

/* function of main thread
 */
void main( void * )
{
	osSyncPrintf( "Hello World\n" ) ;

	while( 1 ){
	}
}
