#ifndef _BBDEBUG_H_
#define _BBDEBUG_H_

#include "PR/bcp.h"

/*
 * Header to include register defines for IDE debug port.
 */

#define IDE_DEBUG_BASE       (PI_IDE0_BASE_REG) 

/*
 * CMD register.
 *
 *     (R):  [0]  incoming interrupt status (1 = on, 0 = off)
 *     (W):  [7]  clear incoming interrupt (writing 1)
 *           [3]  reset debug interface (not ide)
 *           [2]  set negative edge interrupt
 *           [1]  clear data input counter
 *           [0]  set outgoing (not IDE) interrupt
 */
#define IDE_DEBUG_CMD_REG        (IDE_DEBUG_BASE)

/* CMD register bit defs */
#define IDE_DEBUG_CMD_IN_INT_CHK    0x01
#define IDE_DEBUG_CMD_IN_INT_CLR    0x80
#define IDE_DEBUG_CMD_RESET         0x08
#define IDE_DEBUG_CMD_NEG_INT       0x04
#define IDE_DEBUG_CMD_COUNT_CLR     0x02
#define IDE_DEBUG_CMD_OUT_INT       0x01

/*
 * Access to 32-bit data register.
 */
#define IDE_DEBUG_DATA_LO_REG    (IDE_DEBUG_BASE+2)
#define IDE_DEBUG_DATA_HI_REG    (IDE_DEBUG_BASE+4)

/*
 * Definitions for parallel port debug board
 */
#define IDE_RDB_BASE_REG		PI_IDE3_BASE_REG
#define IDE_RDB_DATA_REG		(IDE_RDB_BASE_REG+0)
#define IDE_RDB_RDATA_REG		(IDE_RDB_BASE_REG+0x400)
#define IDE_RDB_CTRL_REG		(IDE_RDB_BASE_REG+0x8000)

#define IDE_RDB_CTRL_RESET		0x40
#define IDE_RDB_CTRL_SET_BB_REQ		0x01
#define IDE_RDB_CTRL_SET_BB_ACK		0x02
#define IDE_RDB_CTRL_CLR_HOST_REQ	0x04
#define IDE_RDB_CTRL_CLR_HOST_ACK	0x08

#define IDE_RDB_STATUS_REG		IDE_RDB_CTRL_REG
#define IDE_RDB_STATUS_BB_REQ		0x1
#define IDE_RDB_STATUS_BB_ACK		0x2
#define IDE_RDB_STATUS_HOST_REQ		0x4
#define IDE_RDB_STATUS_HOST_ACK		0x8

#endif /* _BBDEBUG_H_ */
