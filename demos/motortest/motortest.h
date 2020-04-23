/*=====================================================================
  File  : motortest.h

  Created   by Koji Mitsunari.   Feb,12 1997.
  Copyright by Nintendo, Co., Ltd.      1997.
  =====================================================================*/
#ifndef	_motortest_h_
#define	_motortest_h_

extern	void	MotorSiGetAccess(void);
extern	void	MotorSiRelAccess(void);

#define MOTOR_PRIORITY	        15
#define	STACKSIZEBYTES		0x2000
#define	MOTOR_MSG_NUM		1
#define	MOTOR_STOP_FR		(3-1)

typedef	struct {
  u16	cmd;
  int	contno;
} MotorMsg;

#define TITLE_MES	"Motor Test"
#define	TITLE_MES_X	14
#define	TITLE_MES_Y	2

#define InitializeMode		0
#define MainMode		1

#define	MESB_X		7
#define	MESD_X		MESB_X + 2
#define	MES_X		MESB_X + 6
#define	MES_Y		6
#define	MES_ON		"ON"
#define	MES_OFF		"OFF"
#define	MES_ON_OFF	"Joy"

#define	JOY_X		MES_X + 2
#define	JOY_Y		MES_Y + 4

#define	DIS_X		6

#define WHITE		GPACK_RGBA5551(255, 255, 255, 1)
#define BLUE		GPACK_RGBA5551(  0,   0, 255, 1)
#define BLUE2		GPACK_RGBA5551( 30,  60, 255, 1)
#define GREEN		GPACK_RGBA5551(  0, 255,   0, 1)
#define RED		GPACK_RGBA5551(255,   0,   0, 1)
#define YELLOW		GPACK_RGBA5551(255, 255,   0, 1)
#define GRAY		GPACK_RGBA5551( 90,  90,  90, 1)
#define BGCOLOR		0x0000

#define CHAR_WD		8
#define CHAR_HT		16

#define	MOTOR_DUMMY	0
#define	MOTOR_ON	1
#define	MOTOR_OFF	2

#define	NORMAL		0
#define	REVERSE		1

#define max(x,y)		(((x) >= (y)) ? (x) : (y))
#define min(x,y)		(((x) <  (y)) ? (x) : (y))
#define abs(x)			(((x) < 0)    ? -(x): (x))

#endif /* _motortest_h_ */
