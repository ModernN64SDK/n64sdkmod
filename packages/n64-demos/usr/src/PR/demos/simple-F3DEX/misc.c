/*====================================================================
 * misc.c
 *
 * Handles the command line argument parsing in debugging versions
 * Shouldn't be included in the final game.
 *
 * Copyright 1995, Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics,
 * Inc.; the contents of this file may not be disclosed to third
 * parties, copied or duplicated in any form, in whole or in part,
 * without the prior written permission of Silicon Graphics, Inc.
 *
 * RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to
 * restrictions as set forth in subdivision (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS
 * 252.227-7013, and/or in similar or successor clauses in the FAR,
 * DOD or NASA FAR Supplement. Unpublished - rights reserved under the
 * Copyright Laws of the United States.
 *====================================================================*/

#include <ultra64.h>

/*
 * globals set by the boot arguments
 */
s32 verbose     = 0;
s32 debugger    = 0;
s32 silent	= 0;
s32 logging	= 0;

void parse_args(char *argstring)
{
    s32		argc = 1;
    char	*arglist[32], **argv = arglist;	/* max 32 args */
    char	*ptr;

    if (argstring == NULL || argstring[0] == '\0')
	return;

    /* re-organize argstring to be like main(argv,argc) */
    ptr = argstring;
    while (*ptr != '\0') {
	while (*ptr != '\0' && (*ptr == ' ')) {
	    *ptr = '\0';
	    ptr++;
	}
	if (*ptr != '\0')
	    arglist[argc++] = ptr;
	while (*ptr != '\0' && (*ptr != ' ')) {
	    ptr++;
	}
    }

    /* process the arguments: */
    while ((argc > 1) && (argv[1][0] == '-')) {
	switch(argv[1][1]) {

            case 'd':
                debugger = 1;
                break;
	    
            case 'v':
                verbose = 1;
                break;
            
            case 's':
                silent = 1;
                break;
            
            case 'l':
                logging = 1;
                break;
            
            default:
                break;
	}
	argc--;
	argv++;
    }
}


