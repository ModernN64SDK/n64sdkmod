/*
 * Color frame buffers for 'morphdemo'
 *
 * The declaration for the color frame buffers is in a separate file so that
 * their location in memory can be precisely controlled using the spec file.
 */

#include "morphdemo.h"

unsigned short cfb_A[SCREEN_WD*SCREEN_HT];
unsigned short cfb_B[SCREEN_WD*SCREEN_HT];


