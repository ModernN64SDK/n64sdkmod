/* 
 *  N64-HVQM2 library  Sample program
 * 
 *  FILE : timekeeper.h
 * 
 *  Copyright (C) 1998,1999 NINTENDO Co.,Ltd.
 * 
 */

/* 1999-02-16 */

#include <ultra64.h>
#include "system.h"

#ifndef __TIMEKEEPER_H__
#define __TIMEKEEPER_H__

#include <ultra64.h>
#include <HVQM2File.h>

typedef u32 (*tkAudioProc)(void *pcmbuf);
typedef tkAudioProc (*tkRewindProc)(void);

void createTimekeeper();
void tkStart(tkRewindProc rewind, u32 samples_per_sec);
void tkPushVideoframe(void *vaddr, u32 *statP, u64 disptime);
u64 tkGetTime(void);
void tkStop(void);

#endif /* __TIMEKEEPER_H__ */

/* end */
