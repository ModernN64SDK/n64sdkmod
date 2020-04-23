/*
 * Color frame buffers for 'morphfaces'
 *
 * The declaration for the color frame buffers is in a separate file so that
 * their location in memory can be precisely controlled using the spec file.
 */

#include "morphfaces.h"

#ifdef FB32BIT
unsigned int cfb_A[SCREEN_WD*SCREEN_HT];
unsigned int cfb_B[SCREEN_WD*SCREEN_HT];
#else
unsigned short cfb_A[SCREEN_WD*SCREEN_HT];
unsigned short cfb_B[SCREEN_WD*SCREEN_HT];
#endif


