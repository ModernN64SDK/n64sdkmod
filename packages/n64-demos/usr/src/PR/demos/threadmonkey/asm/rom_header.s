/*
 * ROM header
 */

.byte  0x80, 0x37, 0x12, 0x40   /* PI BSD Domain 1 Configuration */
.word  0x0000000F               /* Clockrate setting*/
.word  __start                  /* Entrypoint */

.byte  0x00, 0x00               /* Unused */
.byte  20                       /* SDK Revision (Hardware, "2.0")*/
.ascii "L"                      /* SDK Revision (Software, "L")*/

.word  0x00000000, 0x00000000   /* Checksums (Overwritten by makemask) */
.word  0x00000000, 0x00000000   /* Unused */
.ascii "                    "   /* Internal ROM name (Max 20 characters, and must be EXACTLY 20 characters.) */
                                /* If your ROM name is shorter than 20 characters, 
                                pad the string with spaces to 20. */

/* Advanced Homebrew ROM Header starts here */
/* For documentation regarding the format read */
/* https://n64brew.dev/wiki/ROM_Header#Advanced_Homebrew_ROM_Header */

.byte  0x00                     /* Advanced Homebrew ROM Header: Expected controller hardware for port 1 */
.byte  0x00                     /* Advanced Homebrew ROM Header: Expected controller hardware for port 2 */
.byte  0x00                     /* Advanced Homebrew ROM Header: Expected controller hardware for port 3 */
.byte  0x00                     /* Advanced Homebrew ROM Header: Expected controller hardware for port 4 */
/* Game ID (EXAMPLE: "NSME") Begins here */
.byte  0x00, 0x00, 0x00         /* Unused */
.ascii "N"                      /* Cartridge Type ("N")*/
.ascii "  "                     /* Cartridge ID ("SM") (If using Advanced Homebrew ROM Header, set to "ED")*/
.ascii " "                      /* Region ("E")*/
.byte  0x00                     /* Version/Advanced Homebrew ROM Header: Savetype */
