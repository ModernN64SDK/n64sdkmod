/*---------------------------------------------------------------------
  File  : eeptest.h

  Created   by Koji Mitsunari.   Aug,27 1997.
  Copyright by Nintendo, Co., Ltd.      1997.
  ---------------------------------------------------------------------*/
#ifndef __eeptest_h__
#define __eeptest_h__

#include <ultra64.h>

#define InitializeMode		0
#define	TestLong4KMode		1
#define	TestLong16KMode		2
#define	TestShort4KMode		3
#define	TestShort16KMode	4
#define	MainMode		7
#define	FomtMode		8
#define	FileMode		9
#define EndMode		       20
#define NoControllerMode       30

#define TITLE_MES	"EEPROM Test Program"
#define	TITLE_X		10
#define	TITLE_Y		3

#define	MES_X		4
#define	MES_Y		5

#define	PUSH_X		32
#define PUSH_Y 		22
#define PUSH_A_MES	"Push A Button!"
#define ERASE_PUSH_A	"              "
#define PUSH_BLINK	64

#define	ContNo		0

#endif
