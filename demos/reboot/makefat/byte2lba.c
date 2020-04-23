/*
 * lba <-> byte conversion program
 *
 *     Author: has@rd3
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern	int tab[][4292];

#define	BYTE2LBA	0
#define LBA2BYTE	1

int byte2LBA(int type, int startLBA, int nbytes)
{
  int totalbytes = 0;
  int lba = startLBA;
  
  while(totalbytes < nbytes)
  {
    totalbytes += tab[type][lba];
    lba ++;
  }

  return(lba - startLBA);
  
}

int lba2Byte(int type, int startLBA, int nlbas)
{
  int result = 0;
  
  nlbas += startLBA;
  while(startLBA < nlbas)
    result += tab[type][startLBA++];

  return result;
  
}
