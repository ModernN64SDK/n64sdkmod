/*
 * Z-buffer for 'morphfaces'
 *
 * The declaration for the Z-buffer is in a separate file so that its
 * location in memory can be precisely controlled using the spec file.
 *
 */
#include "morphfaces.h"

#ifdef FB32BIT
unsigned int zbuffer[SCREEN_WD*SCREEN_HT];
#else
unsigned short zbuffer[SCREEN_WD*SCREEN_HT];
#endif


