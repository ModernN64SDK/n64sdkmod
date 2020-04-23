/*
 * makefat.c
 *
 * create fat data from file size
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* extern	int tab[][4292]; */

#ifndef null 
#define null 0
#endif

#define myClose(p) fclose(p)
#define myErrStop(str) 		myErrStopSSI(str,NULSTR,NULSTR,0,1)
#define myErrStopS(str,s1) 	myErrStopSSI(str,s1,NULSTR,0,1)
#define myErrStopI(str,i1)	myErrStopSSI(str,NULSTR,NULSTR,i1,1)

typedef unsigned char                   u8;     /* unsigned  8-bit */
typedef unsigned short                  u16;    /* unsigned 16-bit */
typedef unsigned long                   u32;    /* unsigned 32-bit */

typedef signed char                     s8;     /* signed  8-bit */
typedef short                           s16;    /* signed 16-bit */
typedef long                            s32;    /* signed 32-bit */


char 	NULSTR[1]={0};
FILE *fp;

char * buf[32];
extern int lba2Byte(int type, int startLBA, int nlbas);
extern int byte2LBA(int type, int startLBA, int nlbas);

/* local functions */
void 	myErrStopSSI(char * str ,char * s1 ,char * s2 ,int i ,int stop);
s32     myStr2Long(char * str);
s32     _getSizeOfFile(char * file);
FILE *  myOpen(char * filename , char * mode);
s32  rev32(u32 data);

/* implementation  */

int main(int argc, char **argv)
{
  s32 bytesize,lbasize,disktype,startlba,ret;
  
  if( argc != 4 ) myErrStop("makefat [disktype] [lba] [codesegment.o]");  
  
  disktype = myStr2Long(argv[1]);
  startlba = rev32( myStr2Long(argv[2]) );
  bytesize = _getSizeOfFile(argv[3]);
  lbasize  = rev32( byte2LBA( disktype ,startlba ,bytesize ) );

  fp = myOpen("fat","w");
  ret = fwrite( &startlba , sizeof(u8) , sizeof(s32) , fp );
  if( ret != sizeof(s32) ) myErrStop("write failed");
  
  ret = fwrite( &lbasize  , sizeof(u8) , sizeof(s32) , fp );
  if( ret != sizeof(s32) ) myErrStop("write failed");

  myClose(fp);

  return 0;
} /* main */

s32
myStr2Long(char * str)
{
  if (str == 0) return 0;
  if( *str == null ){
	return 0;
  }else{
	return strtol( str , (char **)null, 0);
  }
}


s32
_getSizeOfFile(char * file)
{
  s32 ret;
  FILE * p;
  
  p = myOpen(file,"r");
  ret = fseek(p, 0, SEEK_END);
  if(!ret) ret = ftell(p);
  myClose(p);
  if(ret == -1) myErrStopS( "can't seek ",file);

  return ret;
  
}



FILE *
myOpen(char * filename , char * mode)
{
  FILE * p;
  char str[64],mstr[16];

  sprintf(mstr,"%sb",mode);
  p = fopen( filename , mstr);
  if( p == NULL )
    {
	  sprintf(str,"can't open %s ",filename);
	  myErrStop(str);
	}
  return p;
}



void
myErrStopSSI(char * str ,char * s1 ,char * s2 ,int i ,int stop)
{
  char buf[256];

  sprintf( buf , "%s  %s  \n %c  %d \n" ,str,s1,*s2,i);
  fprintf( stderr , buf );

  if(stop) exit(0);
}

s32 
rev32(u32 data)
{
#ifdef __PC__
    u8 c0;
#endif

    union {
	u32 ld;
	u8  ch[4];
    } d;

    d.ld=data;

#ifdef __PC__  /* reverse endian */
    c0=d.ch[0];
    d.ch[0] = d.ch[3];
    d.ch[3] = c0;

    c0=d.ch[1];
    d.ch[1] = d.ch[2];
    d.ch[2] = c0;
#endif

    return d.ld;
}


