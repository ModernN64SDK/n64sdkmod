/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: cont.h,v $
        $Revision: 1.4 $
        $Date: 1998/11/13 12:23:41 $
 *---------------------------------------------------------------------*/
/*=====================================================================
  File  : cont.h

  Created   by Koji Mitsunari.   Jun,19 1997.
  Copyright by Nintendo, Co., Ltd.      1997.
  =====================================================================*/
#ifndef	_cont_h_
#define	_cont_h_

extern	void	initcont(OSMesgQueue *);
extern	void	readcont(void);

typedef struct{
  int	flag, stat;
  int	nowcon, oldcon, nowtrg;
  int	repeat, repcnt;
  int	sx, sy;
}Conts;

extern	Conts	conts[];

#endif /* _cont_h_ */

