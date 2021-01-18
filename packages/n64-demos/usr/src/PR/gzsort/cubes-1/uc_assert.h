/*---------------------------------------------------------------------*
	Copyright (C) 1997, Nintendo.
	
	File		uc_assert.h
	Coded    by	Yoshitaka Yasumoto.	Sep 12, 1997.
	Modified by	
	
	$Id: uc_assert.h,v 1.1 1997/09/12 07:03:28 yasu Exp $
 *---------------------------------------------------------------------*/

extern	u32	ucCheckAssert(void);
extern	void	ucDebugGfxLogPrint(OSTask *);
extern	void	ucDebugRdpFifoPrint(OSTask *);
extern	void	ucDebugInfoPrint(void);
extern	void	ucDebugAssertPrint(void);
extern	void	ucDebugIMEMPrint(void);
extern	void	ucDebugDMEMPrint(void);

/*======== End of uc_assert.h ========*/
