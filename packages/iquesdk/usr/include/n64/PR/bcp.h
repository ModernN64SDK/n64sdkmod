#ifndef _BCP_H_
#define _BCP_H_

#include "PR/rcp.h"

/*************************************************************************
 * Additional VI registers. These exist in the RCP as well, but aren't 
 * defined in rcp.h, since they are only used for testing. BB uses them
 * for video reset functionality
 */
#define VI_SPANADDR_REG (VI_BASE_REG+0x38)
#define VI_SPANDATA_REG (VI_BASE_REG+0x3c)

/*************************************************************************
 * additional Peripheral Interface (PI) Registers
 */

/* PI buffer (R/W): [31:0]
 */
#define PI_BUFFER_BASE_REG	(PI_BASE_REG+0x10000)

/* for FLASH operations, the buffer appears as two distinct
 * buffer regions. Each has an associated region for out-of-band (OOB)
 * data storage.
 */
#define PI_BUFFER_DATA_SIZE     0x400
#define PI_BUFFER_OOB_SIZE      0x20
#define PI_BUFFER_0_START       (PI_BUFFER_BASE_REG)
#define PI_BUFFER_1_START       (PI_BUFFER_BASE_REG+(PI_BUFFER_DATA_SIZE>>1))
#define PI_BUFFER_0_OOB_START   (PI_BUFFER_BASE_REG+PI_BUFFER_DATA_SIZE)
#define PI_BUFFER_1_OOB_START   (PI_BUFFER_0_OOB_START+(PI_BUFFER_OOB_SIZE>>1))

/* for DMA, the buffer appears as a single 1024B section
 * of memory.
 */
#define PI_BUFFER_START         (PI_BUFFER_0_START)
#define PI_BUFFER_END           (PI_BUFFER_START+PI_BUFFER_DATA_SIZE)

/* PI AES control (R):   [31] AES busy
 *                       [30] AES interrupt status
 *                (W):   
 *                       [30] AES interrupt enable
 *                (R/W): [0] hardware CBC init vect chaining 
 *                           (1 = chain, 0 = use [7:1]) 
 *                       [7:1] 16-byte offset in PI buffer of CBC init vector
 *                       [15:9] 16-byte offset in PI buffer of data start
 *                       [21:16] data_size-1 in 16 byte-block units
 */
#define PI_AES_CTRL_REG		(PI_BASE_REG+0x50)

#define PI_AES_CTRL_START       0x80000000
#define PI_AES_CTRL_BUSY        PI_AES_CTRL_START
#define PI_AES_CTRL_INTR        0x40000000
#define PI_AES_CTRL_HC          0x00000001
#define PI_AES_CTRL_SIZE_SHIFT          16
#define PI_AES_CTRL_SIZE_MASK   0x003F0000
#define PI_AES_CTRL_DATA_SHIFT           9
#define PI_AES_CTRL_DATA_MASK   0x0000FE00
#define PI_AES_CTRL_IV_SHIFT             1
#define PI_AES_CTRL_IV_MASK     0x000000FE

/* PI AES expanded key (R/W): [31:0] x 44
 */
#define PI_AES_EKEY_REG		(PI_BUFFER_BASE_REG+0x420)

/* PI AES CBC init vector (R/W): [31:0] x 4
 */
#define PI_AES_INIT_REG		(PI_BUFFER_BASE_REG+0x4d0)

/* 16B index of init vector into pi buffer
 */
#define PI_AES_INIT_INDX16      77

/* PI Flash control (R): [31] busy, [30] interrupt status,
 *                       [11] single bit ecc error
 *			 [10] ecc double bit err
 *                  (W): [31] 0 = start, 1 = stop
 *                       [30] interrupt enable
 *			 [11] enable ECC
 *                       [10] enable multi-cycle command
 *                (R/W): [29] enable write data phase
 *                       [28] enable read data phase
 *                       [27:24] address phase selects, phase [3..0]
 *                       [23:16] flash command
 *                       [15] enable wait ready
 *                       [13:12] device id
 *                       [9:0] data phase size (bytes)
 */
#define PI_FLASH_CTRL_REG	(PI_BASE_REG+0x48)

#define PI_FLASH_CTRL_START	0x80000000
#define PI_FLASH_CTRL_BUSY	PI_FLASH_CTRL_START
#define PI_FLASH_CTRL_INTR	0x40000000
#define PI_FLASH_CTRL_WDPH	0x20000000
#define PI_FLASH_CTRL_RDPH	0x10000000
#define PI_FLASH_CTRL_ADPH_SHIFT	24
#define PI_FLASH_CTRL_ADPH_MASK	0x0F000000
#define PI_FLASH_CTRL_CMD_SHIFT		16
#define PI_FLASH_CTRL_CMD_MASK	0x00FF0000
#define PI_FLASH_CTRL_WRDY	0x00008000
#define PI_FLASH_CTRL_BUF0	0x00000000
#define PI_FLASH_CTRL_BUF1	0x00004000
#define PI_FLASH_CTRL_BUF_SHIFT		14
#define PI_FLASH_CTRL_DEV_SHIFT		12
#define PI_FLASH_CTRL_DEV_MASK	0x00003000
#define PI_FLASH_CTRL_ECC	0x00000800
#define PI_FLASH_CTRL_MCMD	0x00000400
#define PI_FLASH_CTRL_SIZE_SHIFT	0
#define PI_FLASH_CTRL_SIZE_MASK	0x000003FF
#define PI_FLASH_CTRL_SBERR     PI_FLASH_CTRL_ECC
#define PI_FLASH_CTRL_DBERR     PI_FLASH_CTRL_MCMD

/* PI flash config (R): [30:28] end_of_cycle_time - 1
 *               (R/W): [31] write protect, [26:24] read_data_sample_time - 1
 *                      [23:16] RE active time, [15:8] WE active time, 
 *                      [7:0] CLE/ALE active time
 */
#define PI_FLASH_CONFIG_REG	(PI_BASE_REG+0x4c)

#define PI_FLASH_CONFIG_WR_PROTECT	0x80000000
#define PI_FLASH_CONFIG_EO_CYC_T_MASK	0x70000000
#define PI_FLASH_CONFIG_EO_CYC_T_SHIFT	28
#define PI_FLASH_CONFIG_RD_SAM_T_MASK	0x07000000
#define PI_FLASH_CONFIG_RD_SAM_T_SHIFT	24
#define PI_FLASH_CONFIG_RE_ACT_T_MASK	0x00FF0000
#define PI_FLASH_CONFIG_RE_ACT_T_SHIFT	16
#define PI_FLASH_CONFIG_WE_ACT_T_MASK	0x0000FF00
#define PI_FLASH_CONFIG_WE_ACT_T_SHIFT	8
#define PI_FLASH_CONFIG_CA_ACT_T_MASK	0x000000FF
#define PI_FLASH_CONFIG_CA_ACT_T_SHIFT	0

/* PI flash address (R/W): [29:0] address bits sent in address phases
 */
#define PI_FLASH_ADDR_REG	(PI_BASE_REG+0x70)

/* PI flash device properties
 */
#define PI_FLASH_MAX_DEVS       4

/* PI ATB entry upper (W): [8:0] upper data bits of ATB entry
 */
#define PI_ATBU_REG		(PI_BASE_REG+0x40)

/* PI ATB entries (R/W): [31:0] x 192 lower 32 bits of ATB entries
*/
#define PI_ATB_BUFFER_OFFSET    0x500
#define PI_ATB_BUFFER_LO_REG	(PI_BUFFER_BASE_REG+PI_ATB_BUFFER_OFFSET)
#define PI_ATB_NUM_ENTRIES      192

/* PI ATB entries (R)  : [24:16] upper bits of entry 0, [8:0] upper bits of
 *                       entry 1
 */
#define PI_ATB_BUFFER_HI_REG	(PI_BUFFER_BASE_REG+0x800)

/* PI ATB entry is 41 bits, consisting of the 32-bit
 * entry in the PI_ATB_BUFFER region of the PI buffer (i.e.,
 * PI_ATB_NUM_ENTRIES 32-bit words starting at PI_ATB_BUFFER_OFFSET),
 * and the corresponding upper 9 bits (when a write is performed
 * to a PI_ATB_BUFFER entry, the value in the PI_ATBU_REG is written
 * to the upper 9 bits).
 *
 * The layout in terms of a 41-bit word:
 *    [40]: (IV) if 1 use this entry to indicate the aes init vector
 *          should come from the PI buffer instead of data in module flash.
 *          If 1, the SIZE must be 16K and the PERM bits must be 0x3.
 * [39:38]: (DEV) which device to address on the memory module. Valid
 *          entries are 0, 1 and 2.
 * [37:36]: (PERM) permission bits with binary meaning -
 *            00 = no access (cause access error)
 *            10 = allow pio read
 *            01 = allow dma read (from catridge space)
 *            11 = allow pio read and dma read
 * [35:32]: (SIZE) size encoding of this entry as (16KB*(1<<SIZE)).
 * [31:16]: (PADDR) physical block address (blocks are 16KB).
 *  [15:0]: (VADDR) virtual block address (actual bits 29:14 of virtual addr).
 */
#define PI_ATBU_IV              (1<<(40-32))
#define PI_ATBU_DEV_SHIFT       (38-32)
#define PI_ATBU_DEV_MASK        (3<<PI_ATBU_DEV_SHIFT)
#define PI_ATBU_PERM_PIO        (1<<(37-32))
#define PI_ATBU_PERM_DMA        (1<<(36-32))
#define PI_ATBU_PERM_IV         (PI_ATBU_PERM_PIO|PI_ATBU_PERM_DMA)
#define PI_ATBU_SIZE_SHIFT      (32-32)
#define PI_ATBU_SIZE_MASK       (0xf<<PI_ATBU_SIZE_SHIFT)
#define PI_ATBL_PADDR_SHIFT     (16)
#define PI_ATBL_PADDR_MASK      (0xffff<<PI_ATBL_PADDR_SHIFT)
#define PI_ATBL_VADDR_SHIFT     (0)
#define PI_ATBL_VADDR_MASK      (0xffff<<PI_ATBL_VADDR_SHIFT)

/* PI buffer dma read (R/W): [23:0] dma length and start read
 */
#define PI_DMA_BUFFER_RD_REG	(PI_BASE_REG+0x58)

/* PI buffer dma write (R/W): [23:0] dma length and start write
 */
#define PI_DMA_BUFFER_WR_REG	(PI_BASE_REG+0x5c)

/* PI error (R): [29:8] error address, 
 *        (R/W): [31] enable error generates secure trap
 *               [30] enable error generates error interrupt
 *                [4] enable write-to-cartridge generates trap/interrupt
 *                [3] correctable ecc
 *                [2] uncorrectable ecc,
 *                [1] ATB violation from DMA
 *                [0] ATB violation from PIO
 */
#define PI_ERROR_REG		(PI_BASE_REG+0x44)

#define PI_ERROR_KERNEL_INTR		0x80000000
#define PI_ERROR_SYS_INTR		0x40000000
#define PI_ERROR_ADDR_MASK		0x3FFFFF00
#define PI_ERROR_WRITE_TRAP		0x00000010
#define PI_ERROR_CORRECTABLE_ECC	0x00000008
#define PI_ERROR_UNCORRECTABLE_ECC	0x00000004
#define PI_ERROR_ATB_DMA		0x00000002
#define PI_ERROR_ATB_PIO		0x00000001

#define PI_EDATA_REG            (PI_BASE_REG+0x6c)

/* PI access control (R/W): [7] PI_ERROR
 *			    [6] PI_IOC_CONFIG, PI_IDE[0-3]
 *			    [5] PI_GPIO
 *			    [4] PI_DMA_BUFFER_RD, PI_DMA_BUFFER_WR
 *			    [3] PI_AES_EKEY, PI_AES_INIT, PI_AES_CTRL
 *			    [2] PI_ATBU, PI_ATB_BUFFER, PI_ATB_BUFFER_HI
 *			    [1] PI_FLASH_CTRL, PI_FLASH_CONFIG
 *			    [0] PI_BUFFER[0-1]
 */
#define PI_ACCESS_REG		(PI_BASE_REG+0x54)

#define PI_ACCESS_ERROR		0x80
#define PI_ACCESS_IO		0x40
#define PI_ACCESS_GPIO		0x20
#define PI_ACCESS_BDMA		0x10
#define PI_ACCESS_AES		0x08
#define PI_ACCESS_ATB		0x04
#define PI_ACCESS_FLASH		0x02
#define PI_ACCESS_BUFFER	0x01
#define PI_ACCESS_ALL      \
           (PI_ACCESS_ERROR | PI_ACCESS_IO     | PI_ACCESS_GPIO | \
            PI_ACCESS_BDMA  | PI_ACCESS_AES    | PI_ACCESS_ATB  | \
            PI_ACCESS_FLASH | PI_ACCESS_BUFFER )

/* PI general purpose io (R): [31:16] system id
 *                              [31:28] board config
 *                                      xx00, NTSC 14.31818 MHz
 *                                      xx01, PAL  17.734475 MHz
 *                                      xx10, MPAL 14.31818 MHz
 *                                      xx11, NTSC 14.31818 MHz
 *                              [27] memory size
 *                                        0, 16MB
 *                                        1, 32MB
 *                              [26:25] system clock rate
 *                                        00, 62.5 MHz
 *                                        01, 83.3 MHz
 *                                        10, 96.00 MHz, interal gen USB
 *                                        11, 96.00 MHz, external 48.0 USB Xtal
 *                              [24:22] clock multiplier
 *                                        same as MI_CTRL_REG clock divider mode
 *                     (R/W): [7:4] GPIO output enable
 *                            [3:0] GPIO data, depending on output enable
 */
#define PI_GPIO_REG		(PI_BASE_REG+0x60)	/*XXX correct?*/

#define PI_GPIO_ID_SHIFT		16
#define PI_GPIO_ID_MASK			0xffc00000

#define PI_ID_BOARD_CONFIG_MASK		(0x7<<(12+16))
#define PI_ID_BOARD_CONFIG_SHIFT	(12+16)

#define PI_ID_BOARD_REV_MASK		(0x3<<(14+16))
#define PI_ID_BOARD_REV_SHIFT		(14+16)
#define PI_ID_BOARD_REV_0		0
#define PI_ID_BOARD_REV_1		1

#define PI_ID_VIDEO_MODE_MASK		(3<<((12+16))
#define PI_ID_VIDEO_MODE_SHIFT		(12+16)
#define PI_ID_VIDEO_MODE_NTSC1		0
#define PI_ID_VIDEO_MODE_PAL		1
#define PI_ID_VIDEO_MODE_MPAL		2
#define PI_ID_VIDEO_MODE_NTSC2		3

#define PI_ID_MEM_SIZE_MASK		(1<<(11+16))
#define PI_ID_MEM_SIZE_SHIFT		(11+16)
#define PI_ID_MEM_SIZE_16		0
#define PI_ID_MEM_SIZE_32		1

#define PI_ID_SYS_CLOCK_MASK		(3<<(9+16))
#define PI_ID_SYS_CLOCK_SHIFT		(9+16)
#define PI_ID_SYS_CLOCK_62_5		0
#define PI_ID_SYS_CLOCK_80		1
#define PI_ID_SYS_CLOCK_96		2

#define PI_ID_CLOCK_MULT_MASK		(7<<(6+16))
#define PI_ID_CLOCK_MULT_SHIFT		(6+16)
#define PI_ID_CLOCK_MULT_X1		0
#define PI_ID_CLOCK_MULT_X1_5		1
#define PI_ID_CLOCK_MULT_X2		2
#define PI_ID_CLOCK_MULT_X3		3

#define PI_ID_MEM_CONFIG_MASK		(1<<(5+16))
#define PI_ID_MEM_CONFIG_SHIFT		(5+16)
#define PI_ID_MEM_CONFIG_X16		PI_ID_MEM_CONFIG_MASK

#define PI_GPIO_ENABLE_SHIFT		4
#define PI_GPIO_ENABLE_MASK		0x000000f0
#define PI_GPIO_DATA_SHIFT		0
#define PI_GPIO_DATA_MASK		0x0000000f

#define PI_GPIO_POWER_BIT		1	/* power control */
#define PI_GPIO_ERROR_BIT		2	/* error LED */
#define PI_GPIO_USER0_BIT		4	/* unassigned */
#define PI_GPIO_USER1_BIT		8	/* unassigned */


/* PI IO bus config (R/W): [31] reset
 *                             [30:26] dma cycle end time
 *                             [25:21] dma deassertion time
 *			       [20:16] dma assertion time
 *			       [15:10] pio cycle end time
 *			       [9:5] pio deassertion time
 *			       [4:0] pio assertion time
 */
#define PI_IDE_CONFIG_REG		(PI_BASE_REG+0x64)
#define PI_IDE_CONFIG_RESET		0x80000000

/* PI IO bus control (R/W): 
 *
 */
#define PI_IDE_CTRL_REG		(PI_BASE_REG+0x68)

#define PI_IDE0_BASE_REG	(PI_BASE_REG+0x80000)
#define PI_IDE1_BASE_REG	(PI_BASE_REG+0xa0000)
#define PI_IDE2_BASE_REG	(PI_BASE_REG+0xc0000)
#define PI_IDE3_BASE_REG	(PI_BASE_REG+0xe0000)
#define PI_IDE_FC_BASE_REG	(PI_BASE_REG+0x20000)

#define IDE_READ(addr)		(*(vu16 *)PHYS_TO_K1(addr))
#define IDE_WRITE_FLUSH()	((void)*(vu16 *)PHYS_TO_K1(PI_IDE_FC_BASE_REG))
#define IDE_READ_FLUSH()	(IO_READ(PI_IDE_FC_BASE_REG))
#define IDE_WRITE_NB(addr,data)	(*(vu16 *)PHYS_TO_K1(addr)=(u16)(data))
#define IDE_WRITE(addr,data)	(IDE_WRITE_NB(addr,data), IDE_READ_FLUSH())

/*************************************************************************
 * additional MIPS Interface (MI) Registers
 */

/* MI control (R/W): [18] enable secure trap on pif write
 *                   [17] enable write error intr on pif write
 *                   [16] enable bus error on pif access
 *                   [15] enable secure trap on non-mem block rqst
 *                   [14] enable write error intr on non-mem block rqst
 *                   [13] enable bus error on non-mem block rqst
 *                   [10:8] clock divider mode (000 = 1, 001 = 1.5, 011 = 2, 101 = 3)
 *              (W): [12] warm reset
 *                   [11] cold reset
 */
#define MI_CTRL_REG		(MI_BASE_REG+0x10)

#define MI_CTRL_UNMASK_IDE      (1<<19)
#define MI_CTRL_SK_TRAP_PIF	(1<<18)
#define MI_CTRL_WR_INTR_PIF	(1<<17)
#define MI_CTRL_BUS_ERROR_PIF	(1<<16)
#define MI_CTRL_SK_TRAP_BNM	(1<<15)
#define MI_CTRL_WR_INTR_BNM	(1<<14)
#define MI_CTRL_BUS_ERROR_BNM	(1<<13)
#define MI_CTRL_SOFT_RESET	(1<<12)
#define MI_CTRL_HARD_RESET	(1<<11)
#define MI_CTRL_DIV_MODE_1	(0<<8)
#define MI_CTRL_DIV_MODE_1_5	(1<<8)
#define MI_CTRL_DIV_MODE_2	(2<<8)
#define MI_CTRL_DIV_MODE_3	(3<<8)

#define MI_CTRL_DIV_MODE_SHIFT  (8)
#define MI_CTRL_DIV_MODE_MASK   (7<<MI_CTRL_DIV_MODE_SHIFT)

/* MI secure mode  (R/W): [8] enable iram access in non-secure mode
 *                        [7] enable button trap 
 *                        [6] button trap triggered (clear to re-arm)
 *                        [5] emulation trap triggered (clear to re-arm)
 *                        [4] fatal error triggered (clear to re-arm)
 *                        [3] secure timer trigged (clear to re-arm)
 *                        [2] app triggered (clear to re-arm)
 *                        [1] brom/bram address space control
 *                               1: brom 0x1fc00000 bram 0x1fc2000
 *                               0: bram 0x1fc00000 brom 0x1fc2000
 *                        [0] secure exit (writing 0 exits)
 */
#define MI_SEC_MODE_REG		(MI_BASE_REG+0x14)

#define MI_SEC_MODE_MD_TRAP_EN	(1<<26)
#define MI_SEC_MODE_BUT_TRAP_EN	(1<<25)
#define MI_SEC_MODE_IRAM_ACCESS	(1<<24)
#define MI_SEC_MODE_MD_TRAP	(1<<7)
#define MI_SEC_MODE_BUT_TRAP	(1<<6)
#define MI_SEC_MODE_TRAP	(1<<5)
#define MI_SEC_MODE_FATAL	(1<<4)
#define MI_SEC_MODE_TIMER	(1<<3)
#define MI_SEC_MODE_APP		(1<<2)
#define MI_SEC_MODE_BROM_LO 	(1<<1)
#define MI_SEC_MODE_BRAM_LO 	(0<<1)
#define MI_SEC_MODE_SECURE	(1<<0)
#define MI_SEC_MODE_NON_SECURE	(0<<0)

/* MI secure timer (W):   [31:16] prescale
 *                 (R):   [31:16] current count
 *                 (R/W): [15:0] start value
 */
#define MI_SEC_TIMER_REG	(MI_BASE_REG+0x18)
#define MI_SEC_TIMER_PRESCALE_MASK	0xffff0000
#define MI_SEC_TIMER_PRESCALE_SHIFT	16
#define MI_SEC_TIMER_START_MASK		0x0000ffff
#define MI_SEC_TIMER_START_SHIFT	0


/* MI secure timer (W):   [31:16] prescale
 *                 (R):   [31:16] current count
 *                 (R/W): [15:0] start value
 */
#define MI_SEC_VTIMER_REG	(MI_BASE_REG+0x1c)
#define MI_SEC_VTIMER_TIME_MASK	        0x00ff0000
#define MI_SEC_VTIMER_TIME_SHIFT        8
#define MI_SEC_VTIMER_TBASE_MASK	0x000000ff
#define MI_SEC_VTIMER_TBASE_SHIFT	0

/* MI error address (R): [31:0] address
 */
#define MI_ERROR_ADDR_REG	(MI_BASE_REG+0x20)

/* MI error data
 */
#define MI_ERROR_DATA_REG	(MI_BASE_REG+0x24)

/* MI error info    (R):   [5] read response timeout on bus error
 *                         [3] multiple error
 *                         [2] write request type (0 = single, 1 = block)
 *                         [1:0] write error request size
 *                                     single: 00 = 1, 01 = 2, 10 = 3, 11 = 4
 *                                     block: 00=8, 01 = 16, 10 = 32, 11 = illegal
 *                  (R/W): [4] enable capture logic
 */
#define MI_ERROR_INFO_REG	(MI_BASE_REG+0x28)

#define MI_ERROR_INFO_ENABLE	(1<<4)
#define MI_ERROR_INFO_MULTIPLE	(1<<3)
#define MI_ERROR_INFO_TYPE_BLOCK	(1<<2)
#define MI_ERROR_INFO_SIZE_MASK	(3)
#define MI_ERROR_INFO_SIZE_SHIFT (0)

/*
 * MI Random number generator register (R): [0] valid bits
 */
#define MI_RANDOM_REG (MI_BASE_REG+0x2C)

/*
 * MI AV Control register
 */
#define MI_AVCTRL_REG (MI_BASE_REG+0x30)
#define MI_AVCTRL_AV_RESET (1<<25)
#define MI_AVCTRL_PLL_BYPASS (1<<24)
#define MI_AVCTRL_VENC_TEST (1<<23)
#define MI_AVCTRL_VENC_VTRAP (1<<22)
#define MI_AVCTRL_VENC_VMPAL (1<<21)
#define MI_AVCTRL_VENC_VNTPL (1<<20)
#define MI_AVCTRL_DAC_POWER (1<<19)
#define MI_AVCTRL_VPLL_PDIV_SHIFT (16)
#define MI_AVCTRL_VPLL_PDIV_MASK (0x00070000)
#define MI_AVCTRL_VPLL_NDIV_SHIFT (9)
#define MI_AVCTRL_VPLL_NDIV_MASK (0x0000fe00)
#define MI_AVCTRL_VPLL_MDIV_SHIFT (4)
#define MI_AVCTRL_VPLL_MDIV_MASK (0x000001f0)
#define MI_AVCTRL_VPLL_FRANGE_SHIFT (2)
#define MI_AVCTRL_VPLL_FRANGE_MASK (0x0000000c)
#define MI_AVCTRL_VPLL_DIVRESET (1<<1)
#define MI_AVCTRL_VPLL_STANDBY (1<<0)

/*
 * MI extended interrupt (R): [25:24,13:0] valid bits - see below for bit patterns 
 */
#define MI_EINTR_REG		(MI_BASE_REG+0x38)
#define MI_EINTR_BUT_STATUS	(1<<24)
#define MI_EINTR_BUTTON_PRESSED	MI_EINTR_BUT_STATUS
#define MI_EINTR_MD_STATUS	(1<<25)
#define MI_EINTR_MODULE_REMOVED	MI_EINTR_MD_STATUS

/* 
 * MI extended interrupt mask (W): [23:0] valid bits - see below for bit patterns 
 *                            (R): [11:0] valid bits - see below for bit patterns 
 */
#define MI_INTR_EMASK_REG	(MI_BASE_REG+0x3C)

/*
 * Additional BCP interrupts - Use the following to check for the interrupt
 * setting (MI_EINTR_REG)
 */
#define MI_INTR_FLASH		0x40		/* Bit 6: PI Flash intr */
#define MI_INTR_AES		0x80		/* Bit 7: PI AES intr */
#define MI_INTR_IDE		0x100		/* Bit 8: PI IDE intr */
#define MI_INTR_PI_ERR	        0x200		/* Bit 9: PI Error intr */
#define MI_INTR_USB0		0x400		/* Bit 10: USB0 intr */
#define MI_INTR_USB1		0x800		/* Bit 11: USB1 intr */
#define MI_INTR_BUT		0x1000		/* Bit 12: Button intr */
#define MI_INTR_MD		0x2000		/* Bit 13: Module intr */

/*
 * The following are values to clear/set various interrupt bit mask
 * They can be ORed together to manipulate multiple bits 
 * (MI_EINTR_MASK_REG - write)
 */
#define MI_INTR_MASK_CLR_FLASH  0x1000          /* Bit 12: clear Flash mask */
#define MI_INTR_MASK_SET_FLASH  0x2000          /* Bit 13: set Flash mask */
#define MI_INTR_MASK_CLR_AES    0x4000          /* Bit 14: clear AES mask */
#define MI_INTR_MASK_SET_AES    0x8000          /* Bit 15: set AES mask */
#define MI_INTR_MASK_CLR_IDE    0x10000         /* Bit 16: clear IDE mask */
#define MI_INTR_MASK_SET_IDE    0x20000         /* Bit 17: set IDE mask */
#define MI_INTR_MASK_CLR_PI_ERR 0x40000         /* Bit 18: clear PI Error mask */
#define MI_INTR_MASK_SET_PI_ERR 0x80000         /* Bit 19: set PI Error mask */
#define MI_INTR_MASK_CLR_USB0   0x100000 	/* Bit 20: clear USB0 mask */
#define MI_INTR_MASK_SET_USB0   0x200000	/* Bit 21: set USB0 mask */
#define MI_INTR_MASK_CLR_USB1   0x400000 	/* Bit 22: clear USB1 mask */
#define MI_INTR_MASK_SET_USB1   0x800000 	/* Bit 23: set USB1 mask */
#define MI_INTR_MASK_CLR_BUT	0x1000000 	/* Bit 24: clear Button mask */
#define MI_INTR_MASK_SET_BUT	0x2000000 	/* Bit 25: set Button mask */
#define MI_INTR_MASK_CLR_MD	0x4000000 	/* Bit 26: clear Module mask */
#define MI_INTR_MASK_SET_MD	0x8000000 	/* Bit 27: set Module mask */

/*
 * The following are values to check for interrupt mask setting 
 * (MI_EINTR_MASK_REG - read)
 */
#define MI_INTR_MASK_FLASH	0x40		/* Bit 6: PI Flash intr */
#define MI_INTR_MASK_AES	0x80		/* Bit 7: PI AES intr */
#define MI_INTR_MASK_IDE	0x100		/* Bit 8: PI IDE intr */
#define MI_INTR_MASK_PI_ERR	0x200		/* Bit 9: PI Error intr */
#define MI_INTR_MASK_USB0   	0x400		/* Bit 10: USB0 intr mask */  
#define MI_INTR_MASK_USB1   	0x800 		/* Bit 11: USB1 intr mask */
#define MI_INTR_MASK_BUT   	0x1000 		/* Bit 11: Button intr mask */
#define MI_INTR_MASK_MD   	0x2000 		/* Bit 11: Module intr mask */

/* 8Kx32 internal sram */
#define INTERNAL_RAM_START	0x1fc40000
#define INTERNAL_RAM_END	0x1fc48000

/*
 * Virage control registers - Common bit definitions
 */
#define VIRAGE0_CTRL_REG        0x1fc8c000
#define VIRAGE1_CTRL_REG        0x1fc9c000
#define VIRAGE2_CTRL_REG        0x1fcac000

/*
 * Virage NMS register - same bit definitions as control, however
 * writing to this register will send the command and other bits
 * directly to the Store controller
 */
#define VIRAGE0_NMS_REG         0x1fc8e000
#define VIRAGE1_NMS_REG         0x1fc9e000
#define VIRAGE2_NMS_REG         0x1fcae000
/*
 * Virage CP register - same bit definitions as control, however
 * writing to this register will cause the Charge Pump Clock
 * enable to be pulsed so values can be loaded. These registers
 * should be used to store values in the CP. Eg, data, vppselect.
 */

#define VIRAGE0_CP_REG          0x1fc8d000
#define VIRAGE1_CP_REG          0x1fc9d000
#define VIRAGE2_CP_REG          0x1fcad000

#define VIRAGE_CTRL_NMS_BYPASS  (1<<31)
#define VIRAGE_CTRL_NMS_READY   (1<<30)
#define VIRAGE_CTRL_NMS_PASS    (1<<29)
#define VIRAGE_CTRL_NMS_KEEP    (1<<28)
#define VIRAGE_CTRL_NMS_CMD_SHIFT (24)
#define VIRAGE_CTRL_NMS_CMD_MASK (0x07000000)
#define VIRAGE_CTRL_NV_RCREADY  (1<<23)
#define VIRAGE_CTRL_NV_MATCH    (1<<22)
#define VIRAGE_CTRL_NV_RECALL   (1<<21)
#define VIRAGE_CTRL_NV_STORE    (1<<20)
#define VIRAGE_CTRL_NV_COMP     (1<<18)
#define VIRAGE_CTRL_NV_MRCL_SHIFT (16)
#define VIRAGE_CTRL_NV_MRCL_MASK (0x00030000)
#define VIRAGE_CTRL_NV_TECC_SHIFT (14)
#define VIRAGE_CTRL_NV_TECC_MASK (0x0000c000)
#define VIRAGE_CTRL_NV_BIAS_SHIFT (12)
#define VIRAGE_CTRL_NV_BIAS_MASK (0x00003000)
#define VIRAGE_CTRL_CP_RESET    (1<<11)
#define VIRAGE_CTRL_CP_PE       (1<<10)
#define VIRAGE_CTRL_CP_DATA     (1<<9)
#define VIRAGE_CTRL_CP_VRANGE   (1<<8)
#define VIRAGE_CTRL_CP_VPPSEL_SHIFT (4)
#define VIRAGE_CTRL_CP_VPPSEL_MASK (0x000000f0)
#define VIRAGE_CTRL_CP_UNLOCK   (1<<1)
#define VIRAGE_CTRL_CP_PORST    (1<<0)

/*
 * Virage Store controller configuration registers
 */
#define VIRAGE0_NMS_CRSTO_0_REG 0x1fc88000
#define VIRAGE0_NMS_CRSTO_1_REG 0x1fc88004
#define VIRAGE0_NMS_CRM_0_REG   0x1fc88008
#define VIRAGE0_NMS_CRM_1_REG   0x1fc8800C
#define VIRAGE0_NMS_CRM_2_REG   0x1fc88010
#define VIRAGE0_NMS_CRM_3_REG   0x1fc88014

#define VIRAGE1_NMS_CRSTO_0_REG 0x1fc98000
#define VIRAGE1_NMS_CRSTO_1_REG 0x1fc98004
#define VIRAGE1_NMS_CRM_0_REG   0x1fc98008
#define VIRAGE1_NMS_CRM_1_REG   0x1fc9800C
#define VIRAGE1_NMS_CRM_2_REG   0x1fc98010
#define VIRAGE1_NMS_CRM_3_REG   0x1fc98014

#define VIRAGE2_NMS_CRSTO_0_REG 0x1fca8000
#define VIRAGE2_NMS_CRSTO_1_REG 0x1fca8004
#define VIRAGE2_NMS_CRM_0_REG   0x1fca8008
#define VIRAGE2_NMS_CRM_1_REG   0x1fca800C
#define VIRAGE2_NMS_CRM_2_REG   0x1fca8010
#define VIRAGE2_NMS_CRM_3_REG   0x1fca8014


/* 16x32 internal sram */
#define VIRAGE0_RAM_START	0x1fc80000
#define VIRAGE0_RAM_END		0x1fc80040

/* 16x32 internal sram */
#define VIRAGE1_RAM_START	0x1fc90000
#define VIRAGE1_RAM_END		0x1fc90040

/* 64x32 internal sram */
#define VIRAGE2_RAM_START	0x1fca0000
#define VIRAGE2_RAM_END		0x1fca0100

/* byte layout of VIRAGE2
 *    0-19:    hash value used to check SK
 *    20-27:   first boot code patch entry (if no patch
 *             uses 8 bytes so the jump and nop may be
 *             placed here)
 *    28-35:   second boot code patch entry
 *    ...
 *    248-251: checksum word used to insure the sum
 *             over the entire VIRAGE2 equals the
 *             value defined as VIRAGE2_CHECKSUM
 *             (which is 0x00bbc0de) in lib\bbboot\boot.h.
 *    252-255: JTAG enable code
 */
#define VIRAGE2_SK_HASH_START      (VIRAGE2_RAM_START)
#define VIRAGE2_BOOT_PATCH1_START  (VIRAGE2_RAM_START+20)
#define VIRAGE2_BOOT_PATCH2_START  (VIRAGE2_RAM_START+28)
#define VIRAGE2_CHECKSUM_START     (VIRAGE2_RAM_START+248)
#define VIRAGE2_JTAG_ENABLE_START  (VIRAGE2_RAM_START+252)

/* 16Kx32 internal sram, mapped at either lo or hi address */
#define BOOT_RAM_LO_START	0x1fc00000
#define BOOT_RAM_LO_END		0x1fc10000
#define BOOT_RAM_HI_START	0x1fc20000
#define BOOT_RAM_HI_END		0x1fc30000


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     BCP UI (USB Interface)
    
   Memory Map
   USB0   Reg from 0x04900000 to 0x049000FC
	  SRAM     0x04908000 to 0x049081FF
	  Clock select           0x04940000
	  Secure Access Mode     0x04940010

   USB1   Reg from 0x04A00000 to 0x04A000FC
	  SRAM     0x04A08000 to 0x04A081FF
	  Clock select           0x04A40000
	  Secure Access Mode     0x04A40010

   SRAM is used for BDT(Buffer Descriptor table).
   SRAM can only be access as 4 bytes aligned
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define USB0_BASE_REG	   		0x04900000
#define USB0_BDT_SRAM 			0x04980000
#define USB1_BASE_REG 			0x04A00000
#define USB1_BDT_SRAM 			0x04A80000

#define USB0_PER_ID_REG		        (USB0_BASE_REG+0x00)
#define USB0_ID_COMP_REG      		(USB0_BASE_REG+0x04)
#define USB0_REV_REG          		(USB0_BASE_REG+0x08)
#define USB0_ADD_INFO_REG	     	(USB0_BASE_REG+0x0C)
#define USB0_OTG_INT_STAT_REG 		(USB0_BASE_REG+0x10)
#define USB0_OTG_INT_EN_REG   		(USB0_BASE_REG+0x14)
#define USB0_OTG_STATUS_REG   		(USB0_BASE_REG+0x18)
#define USB0_OTG_CTRL_REG     		(USB0_BASE_REG+0x1C)
#define USB0_INT_STAT_REG     		(USB0_BASE_REG+0x80)
#define USB0_INT_ENB_REG      		(USB0_BASE_REG+0x84)
#define USB0_ERR_STAT_REG     		(USB0_BASE_REG+0x88)
#define USB0_ERR_ENB_REG      		(USB0_BASE_REG+0x8C)
#define USB0_STAT_REG         		(USB0_BASE_REG+0x90)
#define USB0_CTL_REG          		(USB0_BASE_REG+0x94)
#define USB0_ADDR_REG         		(USB0_BASE_REG+0x98)
#define USB0_BDT_PAGE_01_REG  		(USB0_BASE_REG+0x9C)
#define USB0_FRM_NUML_REG     		(USB0_BASE_REG+0xA0)
#define USB0_FRM_NUMH_REG     		(USB0_BASE_REG+0xA4)
#define USB0_TOKEN_REG        		(USB0_BASE_REG+0xA8)
#define USB0_SOFTHLDL_REG     		(USB0_BASE_REG+0xAC)
#define USB0_BDT_PAGE_02_REG  		(USB0_BASE_REG+0xB0)
#define USB0_BDT_PAGE_03_REG  		(USB0_BASE_REG+0xB4)
#define USB0_ENDPT0_REG       		(USB0_BASE_REG+0xC0)
#define USB0_ENDPT1_REG       		(USB0_BASE_REG+0xC4)
#define USB0_ENDPT2_REG       		(USB0_BASE_REG+0xC8)
#define USB0_ENDPT3_REG       		(USB0_BASE_REG+0xCC)
#define USB0_ENDPT4_REG       		(USB0_BASE_REG+0xD0)
#define USB0_ENDPT5_REG       		(USB0_BASE_REG+0xD4)
#define USB0_ENDPT6_REG       		(USB0_BASE_REG+0xD8)
#define USB0_ENDPT7_REG       		(USB0_BASE_REG+0xDC)
#define USB0_ENDPT8_REG       		(USB0_BASE_REG+0xE0)
#define USB0_ENDPT9_REG       		(USB0_BASE_REG+0xE4)
#define USB0_ENDPT10_REG      		(USB0_BASE_REG+0xE8)
#define USB0_ENDPT11_REG      		(USB0_BASE_REG+0xEC)
#define USB0_ENDPT12_REG      		(USB0_BASE_REG+0xF0)
#define USB0_ENDPT13_REG      		(USB0_BASE_REG+0xF4)
#define USB0_ENDPT14_REG      		(USB0_BASE_REG+0xF8)
#define USB0_ENDPT15_REG      		(USB0_BASE_REG+0xFC)

#define USB1_PER_ID_REG 		(USB1_BASE_REG+0x00)
#define USB1_ID_COMP_REG 		(USB1_BASE_REG+0x04)
#define USB1_REV_REG	          	(USB1_BASE_REG+0x08)
#define USB1_ADD_INFO_REG	     	(USB1_BASE_REG+0x0C)
#define USB1_OTG_INT_STAT_REG 		(USB1_BASE_REG+0x10)
#define USB1_OTG_INT_EN_REG   		(USB1_BASE_REG+0x14)
#define USB1_OTG_STATUS_REG   		(USB1_BASE_REG+0x18)
#define USB1_OTG_CTRL_REG     		(USB1_BASE_REG+0x1C)
#define USB1_INT_STAT_REG	     	(USB1_BASE_REG+0x80)
#define USB1_INT_ENB_REG	     	(USB1_BASE_REG+0x84)
#define USB1_ERR_STAT_REG	    	(USB1_BASE_REG+0x88)
#define USB1_ERR_ENB_REG	      	(USB1_BASE_REG+0x8C)
#define USB1_STAT_REG	 	       	(USB1_BASE_REG+0x90)
#define USB1_CTL_REG	          	(USB1_BASE_REG+0x94)
#define USB1_ADDR_REG	         	(USB1_BASE_REG+0x98)
#define USB1_BDT_PAGE_01_REG	  	(USB1_BASE_REG+0x9C)
#define USB1_FRM_NUML_REG	     	(USB1_BASE_REG+0xA0)
#define USB1_FRM_NUMH_REG	     	(USB1_BASE_REG+0xA4)
#define USB1_TOKEN_REG	        	(USB1_BASE_REG+0xA8)
#define USB1_SOFTHLDL_REG	     	(USB1_BASE_REG+0xAC)
#define USB1_BDT_PAGE_02_REG	  	(USB1_BASE_REG+0xB0)
#define USB1_BDT_PAGE_03_REG	  	(USB1_BASE_REG+0xB4)
#define USB1_ENDPT0_REG		       	(USB1_BASE_REG+0xC0)
#define USB1_ENDPT1_REG		       	(USB1_BASE_REG+0xC4)
#define USB1_ENDPT2_REG		       	(USB1_BASE_REG+0xC8)
#define USB1_ENDPT3_REG		       	(USB1_BASE_REG+0xCC)
#define USB1_ENDPT4_REG		       	(USB1_BASE_REG+0xD0)
#define USB1_ENDPT5_REG		       	(USB1_BASE_REG+0xD4)
#define USB1_ENDPT6_REG		       	(USB1_BASE_REG+0xD8)
#define USB1_ENDPT7_REG		       	(USB1_BASE_REG+0xDC)
#define USB1_ENDPT8_REG		       	(USB1_BASE_REG+0xE0)
#define USB1_ENDPT9_REG		       	(USB1_BASE_REG+0xE4)
#define USB1_ENDPT10_REG	      	(USB1_BASE_REG+0xE8)
#define USB1_ENDPT11_REG	      	(USB1_BASE_REG+0xEC)
#define USB1_ENDPT12_REG	      	(USB1_BASE_REG+0xF0)
#define USB1_ENDPT13_REG	      	(USB1_BASE_REG+0xF4)
#define USB1_ENDPT14_REG	      	(USB1_BASE_REG+0xF8)
#define USB1_ENDPT15_REG	      	(USB1_BASE_REG+0xFC)

#define USB_ENDPT_WO_HUB			0x80
#define USB_ENDPT_RETRY_DIS			0x40

#define USB_ENDPT_DIR_MASK			0xE3		/* Dir mask */
#define USB_ENDPT_DIS				0x10		/* Disale bit */
#define USB_ENDPT_TX_EN				0x04		/* Tx */
#define USB_ENDPT_RX_EN				0x08		/* Rx */
#define USB_ENDPT_STALL				0x02		/* Stall */
#define USB_ENDPT_HSHK				0x01		/* Hand shake */
#define USB_LS_EN   				0x80		/* Low speed enable */

/* Token PID for TOKEN_REG */
#define USB_OUT_TOKEN_PID				0x10	/* OUT */
#define USB_IN_TOKEN_PID				0x90	/* IN */
#define USB_SETUP_TOKEN_PID				0xD0	/* SETUP */

#define USB0_BDT_ADDR				(USB0_BDT_SRAM)
#define USB1_BDT_ADDR   	  		(USB1_BDT_SRAM)

#define	USB0_SECURE_MODE_REG		0x04940010
#define	USB0_CLOCK_SELECT_REG		0x04940000
#define	USB1_SECURE_MODE_REG		0x04A40010
#define	USB1_CLOCK_SELECT_REG		0x04A40000

/*
 * Bit masks for Secure Mode and Clock Select registers
 */
#define	USB_CLOCK_SELECT_EXTCLK		0x0	/* external 48MHz USB xtal */
#define	USB_CLOCK_SELECT_SYSCLK		0x1	/* use Sysclk/2 */
#define	USB_SECURE_MODE_ON		0x0	/* only SK can access USB */
#define	USB_SECURE_MODE_OFF		0x1	/* any app can access USB */

/*************************************************************************
 * additional Serial Interface (SI) Registers
 */

// BCP versions of SI_PIF_ADDR_RD64B_REG and  SI_PIF_ADDR_WR64B_REG
#define SI_DMA_RD_REG           (SI_BASE_REG+0x04)
#define SI_DMA_WR_REG           (SI_BASE_REG+0x10) 


/* SI CONFIG      (R/W): [31]    JoyChannel 0=master, 1=slave
 *                       [30:24] JoyChannel clock divider
 *                       [23]    cause single PIF commands to return error
 *                       [22]    1 = enable button sampling
 *                       [21:16] Button sample interval
 *                (R):   [7:0]   same as SI CTRL
 */

#define SI_CONFIG_REG           (SI_BASE_REG+0x1c)

#define SI_CONFIG_JC_SLAVE          (1<<31)
#define SI_CONFIG_SGL_ERR           (1<<23)
#define SI_CONFIG_BUT_ENA_SHIFT     (22)
#define SI_CONFIG_BUT_ENA           (1<<SI_CONFIG_BUT_ENA_SHIFT)

#define SI_CONFIG_JC_DIV_SHIFT      (24)
#define SI_CONFIG_JC_DIV_MASK       (0x7F000000)

#define SI_CONFIG_BUT_RATE_SHIFT    (16)
#define SI_CONFIG_BUT_RATE_MASK     (0x003F0000)


/* SI CTRL      (R/W): [0]    1 = drives JoyChannel L for bus reset
 *                     [2]    in slave mode:
 *                               write 1 = start transmission of response
 *                               read    = transmitter busy
 *                (R): [1]    in slave mode: received request
 *                     [31:8] same as SI CONFIG
 */

#define SI_CTRL_REG             (SI_BASE_REG+0x0c)

#define SI_CTRL_SM_XMIT             (1<<2)
#define SI_CTRL_SM_XMIT_BUSY        (1<<2)
#define SI_CTRL_SM_REQ              (1<<1)
#define SI_CTRL_JCRST               (1<<0)

/*************************************************************************
 * additional RDRAM Interface (er, now DDRRAM) Registers
 */
/*
 * Undefine the existing interface since it doesn't exist
 */
#if 0 /*XXXblythe not yet, iosim still needs them*/
#undef RI_MODE_REG
#undef RI_CONFIG_REG
#undef RI_CURRENT_LOAD_REG
#undef RI_SELECT_REG
#undef RI_REFRESH_REG
#undef RI_COUNT_REG
#undef RI_LATENCY_REG
#undef RI_RERROR_REG
#undef RI_WERROR_REG
#endif

/* RI mode register (W): [31:30] command
 *			 [13:12] extended support
 *			 [11:0] data
 */
#define RI_NMODE_REG			(RI_BASE_REG+0x20)

#define RI_NMODE_CMD_MODE		(0<<30)
#define RI_NMODE_CMD_AUTO_REFRESH	(1<<30)
#define RI_NMODE_CMD_PRECHARGE_ALL	(2<<30)
#define RI_NMODE_EX			(2<<12)

#define RI_NMODE_MX_NORMAL		(0<<7)
#define RI_NMODE_MX_RESET_DLL		(2<<7)
#define RI_NMODE_MX_CL(x)		((x)<<4)
#define RI_NMODE_MX_BURST_SEQ		(0<<3)
#define RI_NMODE_MX_BURST_ILV		(1<<3)
#define RI_NMODE_MX_BURST_LEN_2		1
#define RI_NMODE_MX_BURST_LEN_4		2
#define RI_NMODE_MX_BURST_LEN_8		3
#define RI_NMODE_MX_BURST_LEN_PAGE	7

#define RI_NMODE_EX_DRIVE_STR_HALF	2
#define RI_NMODE_EX_DRIVE_STR_NORMAL	0x42
#define RI_NMODE_EX_DLL_DISABLE		1

/* RI refresh register (R/W): [12] enable, [11:4] count */
#define RI_NREFRESH_REG			(RI_BASE_REG+0x30)

#define RI_NREFRESH_ENABLE		(1<<12)
#define RI_NREFRESH_COUNT_SHIFT		4

#define RI_XMEM_REG			(RI_BASE_REG+0x50)
#define RI_XMEM_X16			1
#define RI_XMEM_X32			0


#define RI_STROBE_REV_REG	    (RI_BASE_REG+0x60)
#define RI_AUTO_PRE_CHG_REG	    (RI_BASE_REG+0x80)

/* RI memory configuration   (R): [31:28] auto precharge
 *				  [27:24] cas latency - tcl
 *			  (R/W):  [23:20] write delay - twdel
 *				  [19:16] ras cycle - tras
 *				  [15:12] row precharge - trp
 *				  [11:8] row column delay - trcd
 *				  [7:4] refresh cycle - trfc
 *				  [3:0] read delay - trdel
 */
#define RI_DDR_CONFIG_REG		(RI_BASE_REG+0x40)

#define RI_DDR_CONFIG_TRDEL_SHIFT	24
#define RI_DDR_CONFIG_TWDEL_SHIFT	20
#define RI_DDR_CONFIG_TRAS_SHIFT	16
#define RI_DDR_CONFIG_TRP_SHIFT		12
#define RI_DDR_CONFIG_TRCD_SHIFT	8
#define RI_DDR_CONFIG_TRFC_SHIFT	4
#define RI_DDR_CONFIG_TCL_SHIFT		0

/*************************************************************************
 * DDRRAM Memory (Assumes that maximum size is 8MB in x36 mode, 16MB in x64)
 */
#define DDRRAM36_START		RDRAM_START
#define DDRRAM36_END		RDRAM_END
#define DDRRAM64_START		0x01000000
#define DDRRAM64_END		(DDRRAM64_START+0x1000000)


#endif
