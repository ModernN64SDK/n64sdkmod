/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo. (Originated by SGI)
        
        $RCSfile: main.c,v $
        $Revision: 1.15 $
        $Date: 1999/01/19 03:25:20 $
 *---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	"thread.h"
#include        "textlib.h"
#include	"ncode.h"

#define NUM_MESSAGE 	1

#define NUM_OF_FILE	16

#define	CONT_VALID	1
#define	CONT_INVALID	0

#define WAIT_CNT	5

#define LABEL1		"Nintendo64 CONTROLLER PAK       "

extern OSMesgQueue retraceMessageQ;

extern u16      *bitmap_buf;
extern u16      bitmap_buf1[];
extern u16      bitmap_buf2[];
extern u8	backup_buffer[];
extern void	eep_test(void);
extern s32	__osDumpInode(OSPfs *);
extern s32	__osDumpId(OSPfs *);

static int		err_handlaer(int);
OSMesgQueue  		pifMesgQueue;
OSMesg       		dummyMessage;
OSMesg       		pifMesgBuf[NUM_MESSAGE];

static OSContStatus     contstat[MAXCONTROLLERS];
static OSContPad        contdata[MAXCONTROLLERS];
static int		controller[MAXCONTROLLERS];
OSPfs			pfs0;

u16 c_code[NUM_OF_FILE]={
  0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008,
  0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f, 0x0010,
};

u32 gamecode[NUM_OF_FILE]={
  0x11111111,	0x22222222,	0x33333333,	0x44444444,
  0x55555555,	0x66666666,	0x77777777,	0x88888888,
  0x99999999,	0xaaaaaaaa,	0xbbbbbbbb,	0xcccccccc,
  0xdddddddd,	0xeeeeeeee,	0xffffffff,	0x12341234,
};

char gname[NUM_OF_FILE][16]= {  
{ __A, __U, __T, __O, __F, __I, __L, __L,   0,   0,   0, 0, 0, 0, 0, 0,},
{ __C, __U, __R, __V, __E,   0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0,},
{ __B, __L, __O, __C, __K, __M, __O, __N, __K, __E, __Y, 0, 0, 0, 0, 0,},
{ __D, __R, __I, __V, __E,   0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0,},
{ __B, __U, __M, __P, __M, __A, __P,   0,   0,   0,   0, 0, 0, 0, 0, 0,},
{ __T, __R, __A, __F, __F, __I, __C,   0,   0,   0,   0, 0, 0, 0, 0, 0,},
{ __C, __H, __R, __O, __M, __E,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0,},
{ __S, __O, __L, __I, __D, 0xf, __V, __I, __E, __W,   0, 0, 0, 0, 0, 0,},
{ __C, __I, __8, __F, __B,   0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0,},
{ __S, __K, __Y, 0xf, __F, __L, __Y,   0,   0,   0,   0, 0, 0, 0, 0, 0,},
{ __D, __E, __T, __A, __I, __L,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0,},
{ __R, __O, __L, __O, 0xf, __3, __D,   0,   0,   0,   0, 0, 0, 0, 0, 0,},
{ __G, __I, __N, __V,   0,   0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0,},
{ __S, __N, __O, __O, __P,   0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0,},
{ __G, __R, __O, __U, __N, __D,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0,},
{ __Q, __U, __A, __K, __E,   0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0,},
};

u8 extname[4] = {
  0, 0, 0, 0
};

u8 dsize[NUM_OF_FILE] = {
  16, 58, 32, 5,  7,121,  1,  8, 10,  1,  6,  3,  4, 10,  2,123,
};

u8	err_mes[512];

enum	{
  AllInitMode, EepromCheckMode, PakInitMode, MainMode, ErrorMode,
  WaitMode, DispStateMode, DispLabelMode, FatalMode,
  PakRepairMode, PakRepairMode2
} mode;

void	
mainproc(void *arg)
{	
  int	i, j;
  int	cont_no = 0;
  s32	no;
  u16	number;
  u32	total_size;
  int	error;
  char	*s;
  u32	data_no = 0;
  int	cursor = 0;
  u32	ret;
  u8	bitpattern;
  u16	button = 0, oldbutton = 0, newbutton = 0; 
  OSPfsState	pfsstat[NUM_OF_FILE], state1;
  u8	label[BLOCKSIZE];
  u8	extbuf[__Z - __A];
  int	waitcnt = WAIT_CNT;
  int	len = BLOCKSIZE;

  mode = AllInitMode;

  osCreateMesgQueue(&pifMesgQueue, pifMesgBuf, NUM_MESSAGE);
  osSetEventMesg(OS_EVENT_SI, &pifMesgQueue, dummyMessage);

  osContInit(&pifMesgQueue, &bitpattern, &contstat[0]);
  for (i = 0; i < MAXCONTROLLERS; i++) {
    if ((bitpattern & (1<<i)) &&
       ((contstat[i].type & CONT_TYPE_MASK) == CONT_TYPE_NORMAL)) {
      controller[i] = CONT_VALID;
    } else {
      controller[i] = CONT_INVALID;
    }
  }

  init_draw();

  while(1) {
    osContStartReadData(&pifMesgQueue);
    osRecvMesg(&pifMesgQueue, NULL, OS_MESG_BLOCK);
    osContGetReadData(&contdata[0]);
    if (contdata[cont_no].errno & CONT_NO_RESPONSE_ERROR) {
      button = oldbutton;
    } else {
      oldbutton = button;
      button = contdata[cont_no].button;
    }
    newbutton = ~oldbutton & button;
    
    switch (mode) {
    case AllInitMode:
      mes_init(err_mes);
      mes_add("EEPROM Checking...");
      setcolor(0x70, 0xf0, 0x90);
      draw_puts(err_mes);
      v_sync_double_buffer(&retraceMessageQ);		
      mode = EepromCheckMode;
      break;
    case EepromCheckMode:
      if (osEepromProbe(&pifMesgQueue)) {
	eep_test();
      }
      mode = PakInitMode;
      break;
    case PakInitMode:
      clear_draw();
      osPfsIsPlug(&pifMesgQueue, &bitpattern);
      for (i = 0, cont_no = -1; i < MAXCONTROLLERS; i++) {
	if ((bitpattern & (1<<i)) != 0  && controller[i] != CONT_INVALID) {
	  cont_no = i;
	  break;
	}
      }
      if (cont_no < 0) {
	ret = PFS_ERR_NOPACK;
      } else {
	ret = 0;
      }
      if (ret == 0) {
	ret = osPfsInitPak(&pifMesgQueue, &pfs0, cont_no);
      }
      if (ret != 0) {
	err_handlaer(ret); 
      }
      setcolor(0x20, 0xa0, 0x90);
      draw_puts(err_mes);
      v_sync_double_buffer(&retraceMessageQ);		
      if (ret == 0) {
	osPfsSetLabel(&pfs0, LABEL1);
	mes_init(err_mes);
	mes_add(" ");
	mode = DispLabelMode;
      }
      break;
    case MainMode:
      clear_draw();
      if (newbutton & U_JPAD) {
	cursor--;
      } else if (newbutton & D_JPAD) {
	cursor++;
      } else if (newbutton & L_JPAD) {
	data_no--;
      } else if (newbutton & R_JPAD) {
	data_no++;
      } else if (newbutton & A_BUTTON) { /* DATA READ */
	ret = osPfsFindFile(&pfs0, state1.company_code, state1.game_code,
			    state1.game_name, state1.ext_name, &no);
	if (ret == 0) {
	  error = 0;
	  ret = osPfsReadWriteFile(&pfs0, no, PFS_READ, 0,
				   state1.file_size, backup_buffer);
	  if (ret == 0) {
	    for(j = 0 ; state1.game_code != gamecode[j]
		&& j < sizeof(gamecode)/sizeof(u32) ; j ++);
	    for(i = 0 ; i < state1.file_size; i++) {
	      if( backup_buffer[i] !=  j ){
		error++;
	      }
	    }
	    mes_init(err_mes);
	    mes_add(n64c2asc(state1.game_name, PFS_FILE_NAME_LEN));
	    if(error == 0) {
	      mes_add(" read success");
	    } else {
	      mes_add(" read error");
	    }
	  } else {		/* ERR in osPfsReadWriteFile */
	    err_handlaer(ret);
	    mes_add("\t read failure");
	    mes_add(uint2str(ret));
	  }	    
	} else {		/* ERR in osPfsFindFile */
	  err_handlaer(ret);
	}
	waitcnt = WAIT_CNT;
	mode = DispLabelMode;
      } else if  (newbutton & B_BUTTON) {	/* DATA WRITE */
	ret = osPfsFindFile(&pfs0, state1.company_code, state1.game_code,
			    state1.game_name, state1.ext_name, &no);
	if (ret == 0) {
	  for(j = 0 ; state1.game_code != gamecode[j]
	      && j < sizeof(gamecode)/sizeof(u32) ; j ++);
	  for(i = 0 ; i < state1.file_size ;backup_buffer[i++] = j);
	  ret = osPfsReadWriteFile(&pfs0, no, PFS_WRITE,
				   0, state1.file_size, backup_buffer);
	  mes_init(err_mes);
	  mes_add(n64c2asc(state1.game_name, PFS_FILE_NAME_LEN));
	  if (ret == 0) {
	    mes_add("write file success");
	  } else {		/* ERR in osPfsReadWriteFile */
	    err_handlaer(ret);
	    mes_add("write file failure");
	    mes_add(uint2str(ret));
	  }
	} else {		/* ERR in osPfsFindFile */
	  err_handlaer(ret);
	}
	waitcnt = WAIT_CNT;
	mode = DispLabelMode;
      } else if (newbutton & L_CBUTTONS) { /* ALLOCATE FILE */
	mes_init(err_mes);
	for(i = 0 ; i < __Z-__A ; extbuf[i++] = 0);
	for(i = 0 ; i < NUM_OF_FILE ; i ++) { 
	  if (pfsstat[i].game_code == gamecode[data_no]) {
	    if (pfsstat[i].ext_name[0] >= __A &&
		pfsstat[i].ext_name[0] <= __Z) {
	      extbuf[pfsstat[i].ext_name[0] - __A] = 1;
	    }
	  }
	}
	for(i = 0 ; i < NUM_OF_FILE && extbuf[i] != 0; i ++);
	extname[0] = __A + i;
	ret = osPfsAllocateFile(&pfs0, c_code[data_no], gamecode[data_no],
				gname[data_no], extname,
				256*dsize[data_no], &no);
	if (ret == 0) {
	  for(i = 0 ; i < (int)(256*dsize[data_no]); i++) {
	    backup_buffer[i] = data_no;
	  }
	  ret = osPfsReadWriteFile(&pfs0, no, PFS_WRITE, 0,
				   256*dsize[data_no], backup_buffer);
	  mes_init(err_mes);
	  if (ret != 0) {
	    err_handlaer(ret);
	    mes_add("\tCreate success, but write file failure");
	    mes_add(uint2str(ret));
	  } else {
	    mes_add("Create and write file success");
	  }
	} else {		/* ERR in osPfsAllocateFile */
	  if ((ret == PFS_DIR_FULL) || (ret == PFS_DATA_FULL)) {
	    mes_add("file system full");
	  } else {
	    err_handlaer(ret);
	    mes_add("\tcreate file failure");
	  }
	}
	waitcnt = WAIT_CNT;
	mode = DispLabelMode;
      } else if (newbutton & D_CBUTTONS) { /* DELETE FILE */
	mes_init(err_mes);
	ret = osPfsDeleteFile(&pfs0, state1.company_code, state1.game_code,
			      state1.game_name, state1.ext_name);
	if (ret != 0) {
	  err_handlaer(ret);
	  mes_add("\tdelete file failure");
	  mes_add(uint2str(ret));
	} else {
	  mes_add("delete file success");
	}
	waitcnt = WAIT_CNT;
	mode = DispLabelMode;
      } else if (newbutton & U_CBUTTONS) { /* CHECK FILE SYSTEM */
	mes_init(err_mes);
	ret = osPfsChecker(&pfs0);
	if (ret != 0) {
	  err_handlaer(ret);
	  mes_add("\tcheck file system failure");
	  mes_add(uint2str(ret));
	} else {
	  mes_add("\tcheck file system success");
	}
	waitcnt = WAIT_CNT;
	mode = DispLabelMode;
      } else if (newbutton & R_CBUTTONS) { /* FORMAT */
	mes_init(err_mes);
	setcolor(0xa0, 0x20, 0x80);
	ret = osPfsReFormat(&pfs0, &pifMesgQueue, cont_no);
	if (ret != 0) {
	  err_handlaer(ret);
	  mes_add(uint2str(ret));
	  waitcnt = WAIT_CNT;
	  mode = DispLabelMode;
	} else {
	  mes_add("Format file system success");
	  mode = PakInitMode;
	}
      } else if (newbutton & START_BUTTON) {
	mes_init(err_mes);
	mes_add("");
	waitcnt = WAIT_CNT;
	mode = DispLabelMode;
      }
      data_no &= 0x0f;

      setcolor(0xa0, 0xf0, 0x80);
      draw_puts("\f\n\n	\t");
      draw_puts("LABEL: ");
      s = label;
      for(j = 0; j < len ; j++) {
	draw_char(*s++);
      }
      draw_puts("\n\n");

      total_size = 0;
      if (waitcnt > 0) {
	waitcnt--;
      }
      for(i = 0, number = 0 ; i < NUM_OF_FILE ; i ++) { 
	if (pfsstat[i].file_size != 0 && waitcnt == 0) {
	  setcolor(0xa0, 0x20, 0x80);
	  draw_puts("	  ");
	  draw_ushort(number+1);
	  if( number == cursor ) {
	    draw_puts("    * ");
	    state1 = pfsstat[i];
	  } else {
	    draw_puts("      ");
	  }
	  s = n64c2asc(pfsstat[i].game_name, PFS_FILE_NAME_LEN);
	  draw_char(' ');
	  setcolor(0xf0,0xf0,0);
	  for(j = 0; j < PFS_FILE_NAME_LEN ; j++) {
	    draw_char( *s++ );
	  }
	  draw_char(' ');
	  setcolor(0x80, 0x60, 0x80);
	  s = n64c2asc(pfsstat[i].ext_name, PFS_FILE_EXT_LEN);
	  for(j = 0; j < PFS_FILE_EXT_LEN ; j++) {
	    draw_char( *s++ );
	  }
	  setcolor(0x20,0x70,0xf0);
	  draw_puts("  page = ");
	  draw_dec(pfsstat[i].file_size >> 8);
	  draw_char('\n');

	  total_size += (pfsstat[i].file_size >> 8);
	  number++;
	}
      }
      
      if( cursor < 0 ) {
	cursor = number - 1;
      } else if( cursor >= number ) {
	cursor = 0;
      }      
      draw_puts(CR25);
      setcolor(0x30, 0xf0, 0x20);
      draw_puts("		" "total_page = ");
      if (total_size != 0) {
	draw_dec( total_size );
      }
      draw_puts(" \n\n");
      setcolor(0xa0, 0x20, 0x80);
      draw_char('\t');
      draw_hex(data_no);
      draw_char(' ');
      draw_puts(n64c2asc(gname[data_no], PFS_FILE_NAME_LEN)); 
      draw_puts("page = ");
      draw_dec( dsize[data_no]);

      setcolor(0x20, 0x90, 0x90);
      draw_puts(err_mes);
      v_sync_double_buffer(&retraceMessageQ);		
      break;
    case ErrorMode:
      break;
    case WaitMode:
      (void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);
      if (--waitcnt == 0 ) {
	mode = DispLabelMode;
      }
      break;
    case DispLabelMode:
      if (osPfsGetLabel(&pfs0, label, &len)) {
	err_handlaer(ret);
      }
      mode = DispStateMode;
      break;
    case DispStateMode:
      for(i = 0 ; i < NUM_OF_FILE ; i ++) { 
	ret = osPfsFileState(&pfs0, i, &pfsstat[i]);
	if (ret != 0) {
	  err_handlaer(ret);
	  pfsstat[i].file_size = 0;
	  pfsstat[i].game_code = 0;
	  pfsstat[i].company_code = 0;
	}
      }
      mode = MainMode;
      break;
    case FatalMode:
      clear_draw();
      mes_add(" ");
      setcolor(0xff, 0xff, 0xff);
      draw_puts("\f\n\n\n\n");
      draw_puts("\tERROR in CONTROLLER and CONTROLLER PAK!\n");
      draw_puts("\tPlease check CONTROLLER and CONTROLLER PAK.\n");
      draw_puts("\f\n\n\n\n\n\n\n\n");
      draw_puts("\t\tConnect CONTROLLER and CONTROLLER PAK again\n");
      draw_puts("\t\tRepair system area of CONTROLLER PAK\n");
      if (newbutton & U_JPAD) {
	cursor--;
      } else if (newbutton & D_JPAD) {
	cursor++;
      }
      cursor &= 1;
      draw_puts("\f\n\n\n\n\n\n\n\n");
      if (cursor != 0) {
	draw_puts("\n");
      }
      draw_puts("\t   *");

      if (newbutton & (A_BUTTON | START_BUTTON)) {
	if (cursor == 0){
	  mode = PakInitMode;
	} else {
	  mode = PakRepairMode;
	}
      }
      v_sync_double_buffer(&retraceMessageQ);		
      break;
    case PakRepairMode:
      ret = osPfsRepairId(&pfs0);
      waitcnt = 30;
      mode = PakRepairMode2;
      break;
    case PakRepairMode2:
      if (--waitcnt > 0 ) {
	setcolor(0xff, 0xff, 0xff);
	draw_puts(CR21);
	if (ret == 0) {
	  draw_puts("Repair ID success!\n");
	} else {
	  draw_puts("Repair ID failure!\n");
	}
	v_sync_double_buffer(&retraceMessageQ);		
      } else {
	if (ret == 0) {
	  mode = PakInitMode;
	} else {
	  mode = FatalMode;
	}
      }
      break;
    }
  }
}

static int
err_handlaer(int error)
{
  mes_init(err_mes);
  switch(error) {

  case PFS_ERR_NOPACK:
    mes_add("CONTROLLER PAK is not connected!\n");
    mes_add("\tPlease set CONTROLLER PAK.\n");
    break;

  case PFS_ERR_NEW_PACK:
    mes_add("CONTROLLER PAK has been changed!\n");
    mes_add("\tPlease set the CONTROLLER PAK you have used before.\n");
    break;
	
  case PFS_ERR_INCONSISTENT:
    mes_add("The file system may be corrupted!\n");
    mes_add("\tPlease connect CONTROLLER PAK again!\n");
    break;

  case PFS_DATA_FULL:
    mes_add("DATA full!\n");
    break;

  case PFS_DIR_FULL:
    mes_add("File full!\n");
    break;

  case PFS_ERR_CONTRFAIL:
    mes_add("Please connect CONTROLLER again!\n");
    break;

  case PFS_ERR_DEVICE:
    mes_add("This is not CONTROLLER PAK!\n");
    break;

  case PFS_ERR_ID_FATAL:
    mode = FatalMode;
    break;

  case PFS_ERR_EXIST:
    mes_add("The file has already existed!\n");
    break;
    
  default:
    break;
  }
  return(0);
}
