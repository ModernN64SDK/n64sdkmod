#ifndef _BBNAND_H_
#define _BBNAND_H_

/*
 *  This header contains definitions for external nand flash.
 */

/* byte oriented layout description */
#define PI_FLASH_PAGE_DATA_SIZE           512
#define PI_FLASH_OOB_SIZE                  16
#define PI_FLASH_PAGE_FULL_SIZE    (PI_FLASH_PAGE_DATA_SIZE+PI_FLASH_OOB_SIZE)
#define PI_FLASH_PAGES_PER_BLOCK           32
#define PI_FLASH_PAGE_ADDR_SHIFT            9
#define PI_FLASH_PAGE_PER_BLOCK_POW2        5

/* smart-media specific values */
#define PI_FLASH_PAGE_ECC1_OFFSET         525
#define PI_FLASH_PAGE_ECC2_OFFSET         520
#define PI_FLASH_OOB_ECC1_OFFSET \
                 (PI_FLASH_PAGE_ECC1_OFFSET-PI_FLASH_PAGE_DATA_SIZE)
#define PI_FLASH_OOB_ECC2_OFFSET \
                 (PI_FLASH_PAGE_ECC2_OFFSET-PI_FLASH_PAGE_DATA_SIZE)
#define PI_FLASH_OOB_BLOCK_STATUS_OFFSET      5

/* ecc bit locations per smart-media spec:
 *   These are big-endian specific, and are relative to a 4-byte (word)
 *   aligned read of the ecc byte locations. The START is from
 *   the least significant bit in the word, and likewise for the END.
 *
 *   For example, the word aligned read of the ECC1 bytes would
 *   occur at byte address 524 (PI_FLASH_PAGE_ECC1_OFFSET & ~3).
 *   The ecc bit positions within the word begin at bit location
 *   2 and end at 23 (inclusive, for a total of 22 bits).
 */
#define PI_FLASH_ECC1_BIT_START       2
#define PI_FLASH_ECC2_BIT_START       10
#define PI_FLASH_NUM_ECC_BITS         22
#define PI_FLASH_ECC1_BIT_END  \
           (PI_FLASH_ECC1_BIT_START + PI_FLASH_NUM_ECC_BITS - 1)
#define PI_FLASH_ECC2_BIT_END  \
           (PI_FLASH_ECC2_BIT_START + PI_FLASH_NUM_ECC_BITS - 1)

#define NAND_CMD_READ_0         0x00
#define NAND_CMD_READ_1         0x01
#define NAND_CMD_PROGRAM        0x10
#define NAND_CMD_DUMMY_PROGRAM  0x11
#define NAND_CMD_READ_2         0x50
#define NAND_CMD_ERASE_0        0x60
#define NAND_CMD_ERASE_1        0xD0
#define NAND_CMD_STATUS         0x70
#define NAND_CMD_MP_STATUS      0x71
#define NAND_CMD_DATA_INPUT     0x80
#define NAND_CMD_READ_ID        0x90
#define NAND_CMD_RESET          0xff

#define NAND_STATUS_CMD_PASS  0x00
#define NAND_STATUS_CMD_FAIL  0x01
#define NAND_STATUS_READY       0x40
#define NAND_STATUS_BUSY        0x00
#define NAND_STATUS_WRITE_OK    0x80
#define NAND_STATUS_WRITE_NOK   0x00

#endif
