#include <stdlib.h>
#ifdef WIN32
#define DEV_U64_DATA	"PARTNER-N64"
#include <direct.h>
#else
#include <sys/u64gio.h>
#include <unistd.h>
#endif
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <ultrahost.h>
#include <dirent.h>

static void usage(void)
{
  fprintf(stderr, "gload for PARTNER Ver.0.10 1998 (C) Nintendo Co.,Ltd.\n");
  fprintf(stderr, "usage: gl <romfile>\n");
}

int  fd;

int main(int argc, char **argv)
{
  unsigned char fname[128];
  unsigned char cmd[256];
  unsigned char buf[256];
  unsigned char path[128];
  unsigned char tmpfile[128];
  unsigned char *p;
  int flag = 0;
  FILE	*fp1, *fp2;
  DIR	*dp;
  struct dirent *entry;

  if (argc == 1) {
    dp = opendir(".");
    while ((entry = readdir(dp))!=NULL) {
      if (strcmp(entry->d_name, "gwrite")==0 ||
	  strcmp(entry->d_name, "GWRITE")==0) {
	flag = 2;
	break;
      }
    }
    if (flag == 0 ){
      rewinddir(dp);
      while ((entry = readdir(dp))!=NULL) {
	if (strcmp(entry->d_name, "rom")==0) {
	  flag = 1;
	  strcpy(fname, "rom");
	  break;
	}
      }
      if (flag == 0 ){
	rewinddir(dp);
	while ((entry = readdir(dp))!=NULL) {
	  if (strstr(entry->d_name, ".n64")!=NULL ||
	      strstr(entry->d_name, ".N64")!=NULL){
	    flag = 1;
	    strcpy(fname, entry->d_name);
	    break;
	  }
	}
      }
    }
    closedir(dp);

    if (flag == 0) {
      usage();
      return(1);
    }
  } else if (argc != 2) {
    usage();
    return (2);
  } else {
    strcpy(fname, argv[1]);
  }

  strcpy(cmd, "esc\n");

#ifdef	PARTNER_OLD_VERSION
  getcwd(path, sizeof(path));
  strcat(cmd, "cd ");
  strcat(cmd, path);
  strcat(cmd, "\n");
#endif

  if ((fd = uhOpenGame(DEV_U64_DATA)) == -1) {
    fprintf(stderr, "gl: Unable to open %s : %s\n",
	    DEV_U64_DATA, sys_errlist[errno]);
    return(3);
  }

  if (flag == 2) {
    getcwd(path, sizeof(path));
    if (path[0]!=NULL && path[strlen(path)-1] != '/') {
      strcat(path, "/");
    }
    tmpfile[0]=NULL;
    if (getenv("TEMP") != NULL ) {
      strcpy(tmpfile, getenv("TEMP"));
    } else if (getenv("TMP") != NULL ) {
      strcpy(tmpfile, getenv("TMP"));
    } else {
      strcpy(tmpfile, path);
    }
    if (tmpfile[0]!=NULL && tmpfile[strlen(tmpfile)-1] != '/') {
      strcat(tmpfile, "/");
    }
    strcat(tmpfile, "tmpgw");

    if ((fp1 = fopen("gwrite", "r")) == NULL) {
      fprintf(stderr, "gl: cannot open gwrite!\n");
      return(-1);
    }
    if ((fp2 = fopen(tmpfile, "w")) == NULL) {
      fprintf(stderr, "gl: cannot open temporary gwrite!\n");
      return(-1);
    }
    while (fgets(buf, sizeof(buf), fp1)) {
      p = strrchr(buf, ' ');
      if (*(p+1) != '/') {
	strcpy(fname, p+1);
	*(p+1) = '\0';
	strcat(buf, path);
	strcat(buf, fname);
      }
      fputs(buf, fp2);
    }
    fclose(fp1);
    fclose(fp2);
    strcat(cmd,"<");
    strcat(cmd, tmpfile);
    strcat(cmd,"\nresetdd\ng\n");
    uhPartnerCmd(fd, cmd);
  } else {
    if (uhGload(fd, fname) != 0) {
      fprintf(stderr, "gl: gload failed\n");
      return(4);
    }
  } 
  return(0);
}
