#ifndef _BBSIM_H_
#define _BBSIM_H_

#include "PR/bcp.h"

/*
 * Header to include register defines for simulation-specific "hardware".
 */

/* IDE3 simulation hardware access */

/* IDE4 module presence */
#define PI_IDE3_MD_PRESENCE        (PI_IDE3_BASE_REG+0x1ffe4)

/* IDE3 module insertion/removal controller */
#define PI_IDE3_MD_ZERO_LIST_PTR   (PI_IDE3_BASE_REG+0x1ffe8)
#define PI_IDE3_MD_SET_LIST_DATA   (PI_IDE3_BASE_REG+0x1ffe6)
#define PI_IDE3_MD_TRIGGER         (PI_IDE3_BASE_REG+0x1ffea)

/* IDE3 GPIO loopback */
#define PI_IDE3_GPIO_LOOPBACK      (PI_IDE3_BASE_REG+0x1fff8)

/* IDE3 simulator power down */
#define PI_IDE3_SIM_HALT           (PI_IDE3_BASE_REG+0x1fffa)

/* IDE3 pin reset */
#define PI_IDE3_PIN_RESET          (PI_IDE3_BASE_REG+0x1fffe)

/* IDE3 set board id */
#define PI_IDE3_SET_BOARDID        (PI_IDE3_BASE_REG+0x1ffec)


#endif /* _BBSIM_H_ */
