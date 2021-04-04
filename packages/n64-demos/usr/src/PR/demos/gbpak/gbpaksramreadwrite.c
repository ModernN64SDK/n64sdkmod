/*---------------------------------------------------------------------
        Copyright (C) 1998, Nintendo.
        
        File            gbpaksramreadwrite.c
        Coded    by     Koji Mitsunari. Oct  2, 1997.
        Modified by     Koji Mitsunari. Jul 28, 1998.
        Comments        64GB-PAK SRAM Read/Write for MBC1
   
        $Id: 
   ---------------------------------------------------------------------*/
/**************************************************************************
 *
 *  $Revision: 1.5 $
 *  $Date: 1998/07/29 02:46:06 $
 *  $Source: 
 *
 **************************************************************************/

#include <ultra64.h>

static	u8	srambank[MAXCONTROLLERS]; /* SRAM bank for GB */

/*------------------------------------------------------------------------*
 * 	64GB Pack MBC1 SRAM access initialization function GbpakSramInit()
 *
 * [Syntax]
 *  #include <ultra64.h>
 *  extern s32	GbpakSramInit(OSPfs *);
 *
 *  s32 GbpakSramInit(OSPfs *pfs);
 *
 * [Description]
 *	This is the initialization function which calls GbpakSramReadWrite()
 *	It sets the registers for reading the SRAM data on the MBC1 GameBoy 
 *	cartridge. 
 *	This function must be called before GbpakSramReadWrite(). 
 *
 *	If an error occurs, the following error codes will be returned.
 *	
 *
 *	PFS_ERR_NOPACK
 *		The 64GB Pak has not been inserted in the specified controller.
 *	PFS_ERR_DEVICE
 *		A device other than 64GB Pak has been inserted in the 
 *		controller
 *	PFS_ERR_CONTRFAIL
 *		Data transfer failure with controller. 
 *		Controller is not properly connected, or there might be a problem with 
 *		the 64GB Pak or controller connector. 
 *	PFS_ERR_NO_GBCART
 *		GB cartridge has not been inserted in the 64GB Pak. 
 *	PFS_ERR_NEW_GBCART
 *		GB cartridge has been removed and inserted. 
 *
 *------------------------------------------------------------------------*/
s32
GbpakSramInit(OSPfs *pfs)
{
  s32	i, ret;
  u8	status;
  u8	temp[32];
  
  /*--- Has GB cartridge been removed and inserted?  ---*/
  ret = osGbpakGetStatus(pfs, &status);
  if (ret != 0){
    return(ret);
  }

  /*--- Set MBC1 Register 3 to (0x6000) ---*/
  /*--- This sets Register 2 (0x4000) to SRAM bank switching  ---*/
  for (i = 0 ; i < BLOCKSIZE ; temp[i++] = 1 );
  if ( (ret = osGbpakReadWrite(pfs, OS_WRITE,
			       0x6000, temp, BLOCKSIZE)) != 0 ) {
    return(ret);
  }

  /*--- Set MBC1 Register 0 to (0x0000)  ---*/
  /*--- This makes it possible to access SRAM  ---*/
  for ( i = 0 ; i < BLOCKSIZE ; temp[i++] = 0 );
  temp[BLOCKSIZE-1] = 0x0a;
  
  if ( (ret = osGbpakReadWrite(pfs, OS_WRITE, 0x0000, temp, 32)) != 0 ) {
    return(ret);
  }

  /*--- Current SRAM bank.  Since this initially is uncertain, a dummy value is inserted  ---*/
  srambank[pfs->channel] = 0xff;

  /*--- Has GB cartridge been removed and inserted? ---*/
  ret = osGbpakGetStatus(pfs, &status);

  /*--- Has power to GB cartridge been turned OFF?  ---*/
  /*--- (In other words, has controller been removed during data read or write?) ---*/
  if (ret == 0){
    if (!(status & OS_GBPAK_POWER)) {
      ret = PFS_ERR_CONTRFAIL;
    }
  }

  return(ret);
} /* End of GbpakSramInit */

/*------------------------------------------------------------------------*
 * 	64GB Pak MBC1 SRAM bank switching function 
 *
 * [Syntax]
 *  #include <ultra64.h>
 *  s32 GbpakSramSetBank(OSPfs *pfs, u8 bank);
 *
 * [Description]
 *	This is a SRAM bank switching function which is called within the function
 *	GbpakSramReadWrite()
 *
 *------------------------------------------------------------------------*/
static s32
GbpakSramSetBank(OSPfs *pfs, u8 bank)
{
  s32	i , ret = 0;
  u8	temp[32];

  /*--- Since writing is done in 32 byte units, the value of the SRAM bank ---*/
  /*--- to which you wish to switch is set in all 32 bytes ---*/
  for (i = 0 ; i < BLOCKSIZE ; temp[i++] = bank );

  /*--- Set MBC1 Register 2 and switch SRAM bank  ---*/
  if ( (ret = osGbpakReadWrite(pfs, OS_WRITE,
			       0x4000, temp, BLOCKSIZE)) == 0 ) {
    /*--- When bank switching is successful, set new bank value  ---*/
    srambank[pfs->channel] = bank;
  }
  return(ret);
} /* End of GbpakSramSetBank */

/*------------------------------------------------------------------------*
 * 	64GB Pak MBC1 SRAM access function GbpakSramReadWrite()
 *
 * [Syntax]
 *  #include <ultra64.h>
 *  extern s32	GbpakSramReadWrite(OSPfs *, u16, u32, u8 *, u32);
 *
 *  s32 GbpakSramReadWrite(OSPfs *pfs, u16 flag, u16 address, u8 *buffer,
 *                            u16 size);
 *
 * [Description]
 *	GbpakSramReadWrite() is the function for reading and writing
 *	SRAM data to the GameBoy cartridge. 
 *	
 *	OS_READ or OS_WRITE is specified in the flag. 
 *	When OS_READ is specified, data are read from the GameBoy cartridge. When 
 *	OS_WRITE is specified data are written to the GameBoy cartridge. 
 *	Address specifies the GB cartridge RAM address value (0x0000~0x7fff).
 *	Size specifies the number of bytes to be read or written. 
 *	Address and size must be a multiple of 32. 
 *	Buffer is the buffer in RDRAM that will perform reading or writing. 
 *	pfs in the OSPfs structure is the handle which is initialized by the function osGbpakInit.
 *
 *	If an error occurs, the following error codes will be returned.
 *	
 *
 *	PFS_ERR_NOPACK
 *		The 64GB Pak has not been inserted in the specified controller.
 *	PFS_ERR_DEVICE
 *		A device other than 64GB Pak has been inserted in the 
 *		controller
 *	PFS_ERR_CONTRFAIL
 *		Data transfer failure with controller. 
 *		Controller is not properly connected, or there might be a problem with 
 *		the 64GB Pak or the controller connector. 
 *	PFS_ERR_NO_GBCART
 *		GB cartridge has not been inserted in the 64GB Pak. 
 *	PFS_ERR_NEW_GBCART
 *		GB cartridge has been removed and inserted. 
 *
 *------------------------------------------------------------------------*/
s32
GbpakSramReadWrite(OSPfs *pfs, u16 flag, u32 address, u8 *buffer, u32 size)
{
  s32	ret, addr_end;
  u8	status;
  u8	bank;

  /*--- Has GB cartridge been removed and inserted? ---*/
  ret = osGbpakGetStatus(pfs, &status);
  if (ret != 0){
    return(ret);
  }

  /*--- Determine SRAM bank from 3 most significant bits of start address ---*/
  bank = (u8)(address >> 13);
  if ( bank != srambank[pfs->channel] ) { /* Check SRAM active bank  */
    if ((ret = GbpakSramSetBank(pfs, bank)) != 0 ) {
      return(ret);
    }
  }

  /* Because the accessed address is in the 0x2000 region from 0xa000~0xbfff */
  address &= 0x1fff;
  addr_end = address + size;

  while (1) {
    if ( addr_end > 0x2000 ) {	/* Spans over next bank */
      addr_end -= 0x2000;	/* Address from head of next bank */

      /*--- Regardless, read/write until end (0xbfff) of current bank ---*/
      ret = osGbpakReadWrite(pfs, flag, (address+0xa000),
			     buffer, 0x2000-address);
      if (ret != 0 ) break;

      /*--- Advance pointer on RDRAM side only by read/written amount ---*/
      buffer += (0x2000-address);

      /*--- Switch to next bank ---*/
      if ( (ret = GbpakSramSetBank(pfs, ++bank)) != 0 ) break;
      size = addr_end;
      address = 0;
    } else {	      /*--- When completed in this bank only ---*/
      ret = osGbpakReadWrite(pfs, flag, (address+0xa000), buffer, size);
      break;
    }
  }

  if (ret == 0) {
    /*--- Has GB cartridge been removed and inserted? ---*/
    ret = osGbpakGetStatus(pfs, &status);
  }

  if (ret == 0){
    /*--- Has power to GB cartridge been turned OFF?  ---*/
  /*--- (In other words, has controller been removed during data read or write?) ---*/
    if (!(status & OS_GBPAK_POWER)) {
      ret = PFS_ERR_CONTRFAIL;
    } else if (status & OS_GBPAK_RSTB_DETECTION) {
      ret = PFS_ERR_CONTRFAIL;
    }
  }

  return(ret);
} /* End of GbpakSramReadWrite */

