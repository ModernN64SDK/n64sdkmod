
/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo. (Originated by SGI)
        
        $RCSfile: main.c++,v $
        $Revision: 1.3 $
        $Date: 1998/09/25 21:47:56 $
 *---------------------------------------------------------------------*/
#define	STACKSIZEBYTES	0x2000
#define DMA_QUEUE_SIZE  200

#define INIT_PRIORITY		10
#define GAME_PRIORITY		10

#include <ultra64.h>
#include "main.h"

extern "C" {

void boot(void *argv);
void osInitialize_fakeisv();
void osInitializeCPP( u8 *base, s32 len );

};

/**** threads used by this file ****/
static OSThread gameThread;
static OSThread initThread;

/**** Stack for boot code.  Space can be reused after 1st thread starts ****/
u64    bootStack[STACKSIZEBYTES/sizeof(u64)];

/**** Stacks for the threads, divide by 8 which is the size of a u64 ****/
static u64      gameThreadStack[STACKSIZEBYTES/sizeof(u64)];
static u64      initThreadStack[STACKSIZEBYTES/sizeof(u64)];
static u64      cppArena[STACKSIZEBYTES/sizeof(u64)];

/**** function prototypes for private functions in this file ****/
static void     gameproc(void *);
static void     initproc(char *);

/**** message queues and message buffers used by this app ****/
static OSMesg           PiMessages[DMA_QUEUE_SIZE];
static OSMesgQueue      PiMessageQ;

void boot(void *arg)
{
    osInitialize();
#ifdef ISVPRINT
    osInitialize_isv();
#endif
    osInitializeCPP((u8 *)cppArena, sizeof(cppArena));
    
    osCreateThread(&initThread, 1, (void(*)(void *))initproc, arg,
                  (void *)(initThreadStack+(STACKSIZEBYTES/sizeof(u64))), 
		   (OSPri)INIT_PRIORITY);

    osStartThread(&initThread);
}

static void initproc(char *argv) 
{
    /**** Start PI Mgr for access to cartridge ****/
    osCreatePiManager((OSPri) OS_PRIORITY_PIMGR, &PiMessageQ, PiMessages,
                        DMA_QUEUE_SIZE);

    /**** Create the game thread and start it up ****/
    osCreateThread(&gameThread, 6, gameproc, argv, gameThreadStack + 
		   (STACKSIZEBYTES/sizeof(u64)), (OSPri)GAME_PRIORITY);

    osStartThread(&gameThread);

    /**** Set the thread to be the idle thread ****/
    osSetThreadPri(0, 0);
    for(;;);
}

void gameproc(void *) 
{    
    osSyncPrintf("adsf\n");     // flush printf buffer???
    
    Foo foo;
    foo.Proc1();                // stack object
    
    Bar *theBar = new Bar();    // heap object
    theBar->Proc1();
    delete theBar;
}

MyClass::MyClass() 
{
    i = 1;
    j = 2;
    
    osSyncPrintf("really\n");
    osSyncPrintf("MyClass::MyClass() - 0x%x\n", this);
}

MyClass::~MyClass() 
{
    osSyncPrintf("MyClass::~MyClass() - 0x%x\n", this);
}

void MyClass::NotImp() 
{}

//--------------------------------------------------
// Foo object
//--------------------------------------------------
Foo::Foo() 
{
    mc.i = 1;
    
    osSyncPrintf("Foo::Foo() - 0x%x\n", this);
}

Foo::~Foo() 
{
    osSyncPrintf("Foo::~Foo() - 0x%x\n", this);
}

void Foo::Proc1() 
{
    osSyncPrintf("Foo::Proc1() - 0x%x\n", this);
}

void Foo::Proc2()
{
    osSyncPrintf("Foo::Proc2() - 0x%x\n", this);
}

//--------------------------------------------------
// Bar object
//--------------------------------------------------
Bar::Bar() 
{
    osSyncPrintf("Bar::Bar() - 0x%x\n", this);
}

Bar::~Bar() 
{
    osSyncPrintf("Bar::~Bar() - 0x%x\n", this);
}

void Bar::Proc1() 
{
    osSyncPrintf("Bar::Proc1() - 0x%x\n", this);
}


