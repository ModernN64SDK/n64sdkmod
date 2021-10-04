/*---------------------------------------------------------------------
        Copyright (C) 1998 Nintendo.
        
        File            cont.h
        Coded    by     Koji Mitsunari. Jun 19, 1997.
        Modified by	Koji Mitsunari. May 18, 1998.
        Comments        Header file for SI Procedure
   
        $Id: 
   ---------------------------------------------------------------------*/
#ifndef	_siproc_h_
#define	_siproc_h_

/*--------------------------------------------------------------------
                              Macro Definition
  --------------------------------------------------------------------*/

/* Company code and game title for corresponding game */
#define	COMPANY_CODE	0			 /* <-- Please change */
#define	GAME_TITLE	"GAME TITLE\0\0\0\0\0\0" /* <-- Please change */

#define CONTNO		0		/* Controller number */

/* Commands corresponding to siproc */
#define	SI_CONT_READ		0
#define	SI_GBPAK_READ		2
#define	SI_GBPAK_WRITE		3

/* Error message numbers */
#define	GBPAK_ERR_MES_NOPACK		1
#define	GBPAK_ERR_MES_ERR_DEVICE	2
#define	GBPAK_ERR_MES_FAIL		3
#define	GBPAK_ERR_MES_NOCART		4
#define	GBPAK_ERR_MES_ANOTHER_GAME	5
#define	GBPAK_ERR_MES_NOW_LOADING	6
#define	GBPAK_ERR_MES_FAIL_IN_READ_ID	7

/* Error codes when another game (dummy code for this sample only) */
#define	GBPAK_ERR_ANOTHER_GAME		100

#define	SI_MSG_NUM	1
#define	RET_MSG_NUM	1

/*--------------------------------------------------------------------
                              Structures
  --------------------------------------------------------------------*/
typedef struct{		/*--- For controller read data ---*/
  int	flag;
  int	stat;
  int	nowcon;
  int	oldcon;
  int	nowtrg;
  int	repeat;
  int	repcnt;
  int	sx;
  int	sy;
} Conts;

typedef struct {	/*--- For message transfer to siproc ---*/
  s32	cmd;			/* Command of SI */
  u16	address;		/* Gameboy Address */
  u16	size;			/* Data size (bytes) */
  u8	*buffer;		/* RDRAM buffer pointer */
} SiMsg;

typedef struct {	/*--- For message return from siproc ---*/
  s32	proc_status;
  s32	errmes;			/* Error message */
  s32	ret;			/* ret of 64GB-PAK function */
  u8	status;			/* Status of 64GB-PAK */
} RetMsg;

/*--------------------------------------------------------------------
                              External Statements
  --------------------------------------------------------------------*/

extern Conts	conts[];	/* Data of controller  */

extern OSMesgQueue	siMsgQ;	/* For message transfer to siproc */
extern OSMesgQueue	retMsgQ;/* For message return from siproc */

extern	void	siproc(void);

#endif /* _siproc_h_ */

