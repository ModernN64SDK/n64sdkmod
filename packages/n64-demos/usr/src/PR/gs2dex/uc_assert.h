/*---------------------------------------------------------------------
	Copyright (C) 1997, Nintendo.
	
	File		uc_assert.h
	Coded    by	Yoshitaka Yasumoto.	Mar 19, 1997.
	Modified by	
	Comments	
	
	$Id: uc_assert.h,v 1.2 1997/08/07 03:24:27 yasu Exp $
  ---------------------------------------------------------------------*/

extern	u32	ucCheckAssert(void);
extern	void	ucDebugGfxLogPrint(OSTask *);
extern	void	ucDebugRdpFifoPrint(OSTask *);
extern	void	ucDebugInfoPrint(void);
extern	void	ucDebugAssertPrint(void);
extern	void	ucDebugIMEMPrint(void);
extern	void	ucDebugDMEMPrint(void);

/*======== End of uc_assert.h ========*/
