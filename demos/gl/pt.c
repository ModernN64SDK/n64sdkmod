#include <stdlib.h>
#ifdef WIN32
#define DEV_U64_DATA	"PARTNER-N64"
#include <direct.h>
#else
#include <sys/u64gio.h>
#endif
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <ultrahost.h>

static void usage(void)
{
  fprintf(stderr, "PARTNER Command Ver.0.10 1998 (C) Nintendo Co.,Ltd.\n");
  fprintf(stderr, "usage: pt <partner command>\n");
}

int  fd;

int main(int argc, char **argv)
{
  unsigned char cmd[256];
  unsigned char buf[256];
  unsigned char path[128];
  unsigned char tmpfile[128];
  unsigned char tmpcmd[128];
  int	i, j;
  unsigned char	*p;

  getcwd(path, sizeof(path));
  if (path[strlen(path)-1] != '/') {
    strcat(path, "/");
  }

  strcpy(cmd, " ");
  
  if (argc < 2) {
    usage();
    return (-1);
  }

  if ((fd = uhOpenGame(DEV_U64_DATA)) == -1) {
    fprintf(stderr, "pt: Unable to open %s : %s\n",
	    DEV_U64_DATA, sys_errlist[errno]);
    return(1);
  }

  if (argv[1][0] != '<' && argv[1][0] != '>' ) {
    for(i = 0 ; i < strlen(argv[1]) ; i++) {
      tmpcmd[i] = tolower(argv[1][i]);
    }
  } else {
    strcpy(tmpcmd, argv[1]);
  }

  if (((p = strstr(tmpcmd,"l")) != NULL && p == tmpcmd) ||
      ((p = strstr(tmpcmd,"rd")) != NULL && p == tmpcmd) ||
      ((p = strstr(tmpcmd,"rd")) != NULL && p == tmpcmd) ) {

    for (i = 1, j = 0 ; i < argc ; i ++) {
      switch(i) {
      case 1:
	if ( (p = strstr(tmpcmd,"ls")) != NULL) {
	  p = &argv[1][2];
	  strcat(cmd, "ls ");
	} else if ( (p = strstr(tmpcmd,"ls")) != NULL) {
	  p = &argv[1][2];
	  strcat(cmd, "lp ");
	} else if ( (p = strstr(tmpcmd,"l")) != NULL) {
	  p = &argv[1][1];
	  strcat(cmd, "l ");
	} else if ( (p = strstr(tmpcmd,"rd")) != NULL) {
	  p = &argv[1][2];
	  strcat(cmd, "rd ");
	} else if ( (p = strstr(tmpcmd,"wr")) != NULL) {
	  p = &argv[1][2];
	  strcat(cmd, "wr ");
	}
	if (*p != '\0') {
	  while ( *p == ' ' || *p == '\0') p++;
	  if (*p != '/') strcat(cmd, path);
	  strcat(cmd, p);
	  strcat(cmd, " ");
	} else {
	  j = 1;
	}
	break;
      case 2:
	if (argv[i][0] != '/' && j == 1) {
	  strcat(cmd, path);
	  strcat(cmd, argv[i]);
	  strcat(cmd, " ");
	  j = 0;
	} else {
	  strcat(cmd, argv[i]);
	  strcat(cmd, " ");
	}
	break;
      default:
	strcat(cmd, argv[i]);
	strcat(cmd, " ");
      }
    }
    strcat(cmd, "\n");
	
  } else if ((argv[1][0] == '<') ||
	     (argv[1][0] == '>') ) {

    p = strstr(tmpcmd,"gwrite");
    
    printf("---%s %s\n", argv[1], p);

    if ((argv[1][0] == '<') &&  (p = strstr(tmpcmd,"gwrite")) != NULL
	&& (strcmp(p, "gwrite")==0) ) {
      FILE		*fp1, *fp2;

      if (argv[1][1] == '/') {
	strcpy(path, &argv[1][1]);
	p = strrchr(path, '/');
	*(p+1) = '\0';
      } else if (strcmp(&argv[1][1], "gwrite")!=0) {
	strcpy(buf, &argv[1][1]);
	p = strstr(buf, "gwrite");
	if (*(p-1) == '/') {
	  *p = '\0';
	  strcat(path, buf);
	}
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

      if ((fp1 = fopen(&argv[1][1], "r")) == NULL) {
	fprintf(stderr, "gl: cannot open gwrite!\n");
	return(-1);
      }
      if ((fp2 = fopen(tmpfile, "w")) == NULL) {
	fprintf(stderr, "gl: cannot open temporary gwrite!\n");
	return(-1);
      }
      while (fgets(cmd, sizeof(buf), fp1)) {
	p = strrchr(cmd, ' ');
	if (*(p+1) != '/') {
	  strcpy(buf, p+1);
	  *(p+1) = '\0';
	  strcat(cmd, path);
	  strcat(cmd, buf);
	}
	fputs(cmd, fp2);
      }
      fclose(fp1);
      fclose(fp2);
      strcpy(cmd, "<");
      strcat(cmd, tmpfile);
      strcat(cmd, "\n");
    } else {
      for (i = 0, j = 0 ; i < strlen(argv[1]) ; i ++) {
	if (argv[1][i] != '<' && argv[1][i] != '>' ) break;
	cmd[j++] = argv[1][i];
      }
      cmd[j] = '\0';
      if (argv[1][i] != '/' && argv[1][i] != '\0') {
	strcat(cmd, path);
      }
      strcat(cmd, &argv[1][i]);
      strcat(cmd, " ");
    
      for (i = 2 ; i < argc ; i ++) {
	strcat(cmd, argv[i]);
	strcat(cmd, " ");
      }
      strcat(cmd, "\n");
    }
  } else {
    for (i = 1 ; i < argc ; i ++) {
      strcat(cmd, argv[i]);
      strcat(cmd, " ");
    }
    strcat(cmd, "\n");
  }

  while((p = strchr(cmd, ';')) != NULL) {
    *p = '\n';
  }

  if (uhPartnerCmd(fd, cmd) < 0){
    fprintf(stderr, "pt: partner command failed\n");
    return(-1);
  }
  return(0);
}
