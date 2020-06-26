/*---------------------------------------------------------------------
  Copyright (C) 1999 Nintendo.

  File   		main.c
  Comment	   	1M FLASH TEST(ver 1.25 for flash library 7.6)
  ---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	"nu64sys.h"
#include	"thread.h"
#include	"graph.h"
#include	"display.h"

extern u16	cfb_16_a[];
extern u16	cfb_16_b[];
extern u32	status_buf[];
extern u32	write_buf[];
extern u32	read_buf[];
extern u32             __osFlashPageInc;
extern OSPiHandle      __osFlashHandler;

u16	*cfb_tbl[2] = {
  cfb_16_a, cfb_16_b
};

/*
 *  Controller globals
 */
static OSMesgQueue contMessageQ;
static OSMesg   dummyMessage;
static OSContStatus statusdata[MAXCONTROLLERS];
static OSContPad controllerdata[MAXCONTROLLERS];
static int      controller;
static int      lastx;
static int      lasty;
static int      lastbutton = 0;
static u32 	seed =1;

/*
 *  Handler
 */
OSIoMesg        dmaIOMessageBuf;
OSPiHandle      *handler;

/*---------------------------------------------------------------------
  乱数作成
  ---------------------------------------------------------------------*/
u32 irnd(void)
{
  seed = seed*156083941+1;
  return seed;  
}

/*---------------------------------------------------------------------
  16進データの表示
  ---------------------------------------------------------------------*/
void putint_h32(u16 color, int curs_x, int curs_y, u32 num, char c) {
  int    i, k;
    
  for (i = 0 ; i < c ; i ++) {
    k = num % 16;
    if (k > 9) {
      putchar(color, curs_x - i, curs_y, k + 'A' - 10);
    } else {
      putchar(color, curs_x - i, curs_y, k + '0');
    }
    num = num >> 4;
  }
}

/*---------------------------------------------------------------------
  コントローラ初期化
  ---------------------------------------------------------------------*/
int     initcontroller(void)
{
  int             i;
  u8          pattern;

  osCreateMesgQueue(&contMessageQ, &dummyMessage, 1);
  osSetEventMesg(OS_EVENT_SI, &contMessageQ, (OSMesg) 0);

  osContInit(&contMessageQ, &pattern, &statusdata[0]);

  for (i = 0; i < MAXCONTROLLERS; i++) {
    if ((pattern & (1 << i)) &&
	!(statusdata[i].errno & CONT_NO_RESPONSE_ERROR)) {
      osContStartReadData(&contMessageQ);
      controller = i;
      return i;
    }
  }
  return -1;
}
/*---------------------------------------------------------------------
  コントローラ読み込み
  ---------------------------------------------------------------------*/
static void     readControllers(void)
{
  OSContPad      *pad;

  if (osRecvMesg(&contMessageQ, &dummyMessage, OS_MESG_NOBLOCK) == 0) {
    osContGetReadData(controllerdata);
    osContStartReadData(&contMessageQ);
  }

  pad = &controllerdata[controller];

  lastbutton = pad->button;
  lastx = pad->stick_x;
  lasty = pad->stick_y;
}

/*---------------------------------------------------------------------
  ディスプレイ初期化
  ---------------------------------------------------------------------*/
void start_display(void)
{
  int i;
  for (i = 0; i < SCREEN_WD * SCREEN_HT; i ++){
    cfb_16_a[i] = GPACK_RGBA5551(0,0,0,1);
    cfb_16_b[i] = GPACK_RGBA5551(0,0,0,1);
  } 
}

/*---------------------------------------------------------------------
  読み込みバッファ表示
  ---------------------------------------------------------------------*/
void print_read_buf(u32 page_num)
{
  u32 i;

  for (i=0; i<=31; i=i+2){
    if (read_buf[i + page_num*32]==write_buf[i + page_num*32])
      putint_h32(white, 40,6+i/2,read_buf[i + page_num*32],8);
    else
      putint_h32(red, 40,6+i/2,read_buf[i + page_num*32],8);
    if (read_buf[i+1 + page_num*32]==write_buf[i+1 + page_num*32])
      putint_h32(white, 49,6+i/2,read_buf[i+1 + page_num*32],8);
    else
      putint_h32(red, 49,6+i/2,read_buf[i+1 + page_num*32],8);
  }
  return;  
}

/*---------------------------------------------------------------------
  書き込みバッファ表示
  ---------------------------------------------------------------------*/
void print_write_buf(u32 page_num)
{
  u32 i;
  
  for (i=0; i<=31; i=i+2){
    putint_h32(white, 59,6+i/2,write_buf[i + page_num*32],8);
    putint_h32(white, 68,6+i/2,write_buf[i+1 + page_num*32],8);
  }  
  return;  
}

/*---------------------------------------------------------------------
  書き込みバッファを乱数データで埋める。ただし、最後の４バイトにはページ
  番号を入れる。
  ---------------------------------------------------------------------*/
void fill_write_buf(u32 page_num)
{
  u32 i;
  
  write_buf[page_num*32+31] = page_num;
  for (i=0; i<=30;++i)
    write_buf[page_num*32+i] = irnd();
  return;  
}

/*---------------------------------------------------------------------
  読み込みバッファ全領域の０クリア
  ---------------------------------------------------------------------*/
void clear_read_buf(void)
{
  u32 i;
  
  for (i=0; i<=32*0x400-1;++i){
    read_buf[i] = 0;
  }
  
  return;  
}

/*---------------------------------------------------------------------
  書き込みバッファ全領域の０クリア
  ---------------------------------------------------------------------*/
void clear_write_buf(void)
{
  u32 i;
  
  for (i=0; i<=32*0x400-1;++i)
    write_buf[i] = 0;
  return;  
}

/*---------------------------------------------------------------------
  １Ｍフラッシュの全領域へのデータを書き込み
  ---------------------------------------------------------------------*/
s32 all_write_data(void)
{
  u32	page_num,status;

  if (osFlashAllErase()){
    printstr(white, 11, RESULT_Y, "FAIL ERASE                   ");
    return -1;
  }
  
  for (page_num = 0; page_num <= 0x3ff; ++page_num){
    fill_write_buf(page_num);
    osWritebackDCache(&write_buf[page_num*32], 128);
    osFlashWriteBuffer(&dmaIOMessageBuf, OS_MESG_PRI_NORMAL,
		     &write_buf[page_num*32], &n_dmaMessageQ);
    (void)osRecvMesg(&n_dmaMessageQ, NULL, OS_MESG_BLOCK);
    status = osFlashWriteArray(page_num);

    if (status !=0){
      printstr(white, 11, RESULT_Y, "FAIL WRITE              ");
      putint_h32(white, 52,RESULT_Y, status,8);
      putint_h32(white, 42,RESULT_Y, page_num,4);
      return -1;
    }    
  }
  return 0;
}

/*---------------------------------------------------------------------
  １Ｍフラッシュの全データの読み込み
  ---------------------------------------------------------------------*/
void all_read_data(void)
{
  u32 page_num;

  for (page_num = 0; page_num <= 0x3ff; ++page_num){
    osInvalDCache(&read_buf[page_num*32], 128);

    osFlashReadArray(&dmaIOMessageBuf, OS_MESG_PRI_NORMAL, page_num, 
		     &read_buf[page_num*32], 1, &n_dmaMessageQ);
    (void)osRecvMesg(&n_dmaMessageQ, NULL, OS_MESG_BLOCK);
  }
  return ;
}

/*---------------------------------------------------------------------
  リードバッファとライトバッファのデータの比較(全領域)
  ---------------------------------------------------------------------*/
s32	all_compare_data(void)
{
  s32	i;
  for (i = 0; i < 0x400*32; ++i){
    if (read_buf[i]!=write_buf[i]){
      return (i/32);
    }
  }
  return	-1;
}

/*---------------------------------------------------------------------
  リードバッファとライトバッファのデータの比較(指定領域)
  ---------------------------------------------------------------------*/
s32	region_compare_data(u32 page_num, s32 n_pages)
{
  s32	i;
  for (i = page_num*32; i < (page_num + n_pages)*32; ++i){
    if (read_buf[i]!=write_buf[i]){
      return (i/32);
    }    
  }
  return	-1;
}

/*---------------------------------------------------------------------
  メイン
  ---------------------------------------------------------------------*/
public	void	mainproc(void *arg)
{
  u8	status;
  u16	trig, hold;
  int	i, frame, cur_y;
  s32 	page_num, result, n_pages, max_test;
  u32 	flash_num;
        
  /*---------------------------------------------------------------------
    初期化
    ---------------------------------------------------------------------*/

  handler = osCartRomInit();
  osFlashInit();
  frame = 0;
  flash_num = 0;
  page_num = 0;
  n_pages = 1; 
  max_test = 1;

  clear_read_buf();
  clear_write_buf();
  osViSetMode(&osViModeTable[OS_VI_NTSC_HPF1]);
  osViBlack(1);
  osViSwapBuffer( cfb_tbl[frame] );
  start_display();

  printstr(white, 6, 3, "1M FLASH TEST ver.1.25 L7.6");
  printstr(white, 35, 5, "READ DATA          WRITE DATA");

  /* 使用しているフラッシュ（１個目）のバージョンを表示 */
  osFlashReadId(&status_buf[0],&status_buf[1]);
  switch(status_buf[1]) {
  case FLASH_VERSION_MX_PROTO_A:
    printstr(white, FLASH_VERSION_X, TITLE_Y, "FLASH VERSION: MX PROTO A VERSION");
    break;

  case FLASH_VERSION_MX_A:
    printstr(white, FLASH_VERSION_X, TITLE_Y, "FLASH VERSION: MX A VERSION      ");
    break;
    
  case FLASH_VERSION_MX_C:
    printstr(white, FLASH_VERSION_X, TITLE_Y, "FLASH VERSION: MX C VERSION      ");
    break;

  case FLASH_VERSION_MX_B_AND_D:
    printstr(white, FLASH_VERSION_X, TITLE_Y, "FLASH VERSION: MX B or D VERSION ");
    break;
    
  case FLASH_VERSION_MEI:
    printstr(white, FLASH_VERSION_X, TITLE_Y, "FLASH VERSION: MEI VERSION       ");
    break;

  default:
    printstr(white, FLASH_VERSION_X, TITLE_Y, "FLASH VERSION: UNKOWN VERSION    ");
  }
  
  osViBlack(0);
  osWritebackDCacheAll();
  osViSwapBuffer( cfb_tbl[frame] );
  n_WaitMesg(retrace);
  initcontroller();
  
  lastbutton = 0;
  cur_y = 5;

  /*---------------------------------------------------------------------
    メインループ
    ---------------------------------------------------------------------*/
  while(1){
    /*---------------------------------------------------------------------
      コントローラ読み込み
      ---------------------------------------------------------------------*/
    trig = lastbutton;
    readControllers();
    trig = lastbutton & (lastbutton & ~trig);
    hold = lastbutton;

    /*---------------------------------------------------------------------
      カーソル移動
      ---------------------------------------------------------------------*/
    printstr(white, 5, cur_y, " ");
    
    if (trig & CONT_DOWN) {
      ++cur_y;
    }
    if (trig & CONT_UP)  {
      --cur_y;
    }

    if (cur_y < 5)
      cur_y = 25;
    if (cur_y > 25)
      cur_y = 5;
    
    printstr(white, 5, cur_y, "*");

    /*---------------------------------------------------------------------
      メニュー表示
      ---------------------------------------------------------------------*/
    printstr(white, COMMAND_X, 5, "FLASH CHANGE");
    putint_h32(white, 22,5,flash_num,1);

    printstr(white, COMMAND_X, 6, "READ ID");
    printstr(white, COMMAND_X, 7, "READ STATUS");
    printstr(white, COMMAND_X, 8, "CLEAR STATUS");
    printstr(white, COMMAND_X, 9, "ALL ERASE");
    printstr(white, COMMAND_X, 10, "ALL ERASE THROUGH");
    printstr(white, COMMAND_X, 11, "SECTOR ERASE");
    printstr(white, COMMAND_X, 12, "SECTOR ERASE THROUGH");
    printstr(white, COMMAND_X, 13, "READ ARRAY(DMA)");
    printstr(white, COMMAND_X, 14, "WRITE BUFFER");
    printstr(white, COMMAND_X, 15, "WRITE ARRAY");

    printstr(white, COMMAND_X, 17, "FILL WRITE DATA(1 PAGE)");
    printstr(white, COMMAND_X, 18, "CLEAR ALL READ DATA");
    printstr(white, COMMAND_X, 19, "CLEAR ALL WRITE DATA");
    printstr(white, COMMAND_X, 20, "ALL ERASE & FILL & WRITE");
    printstr(white, COMMAND_X, 21, "ALL READ");
    printstr(white, COMMAND_X, 22, "ALL COMPARE");
    printstr(white, COMMAND_X, 23, "REGION COMPARE");
    printstr(white, COMMAND_X, 24, "ALL TEST");
    putint_h32(white, NUM_INPUT_X,24,max_test,3);
    printstr(white, COMMAND_X, 25, "PAGE LENGTH");
    putint_h32(white, NUM_INPUT_X,25,n_pages,3);
    printstr(white, COMMAND_X, 26, "PAGE NUMBER");
    putint_h32(white, NUM_INPUT_X,26,page_num,3);

    /*---------------------------------------------------------------------
      バッファ表示
      ---------------------------------------------------------------------*/
    print_read_buf(page_num);
    print_write_buf(page_num);

    osWritebackDCacheAll();
    osViSwapBuffer( cfb_tbl[frame] );

    /*---------------------------------------------------------------------
      数値更新（by Cボタンユニット）
      ---------------------------------------------------------------------*/
    switch(cur_y) {
    case 5:
      if (trig & CONT_D)
	flash_num-=1;
      if (trig & CONT_E)
	flash_num+=1;
      flash_num &=3;
      break;
      
    case 24:
      if (trig & CONT_D)
	--max_test;
      if (trig & CONT_E)
	++max_test;
      if (hold & CONT_C)
	max_test = max_test - 0x1;
      if (hold & CONT_F)
	max_test= max_test + 0x1;
      if (max_test > 0x100)
	max_test = 1;
      if (max_test < 1)
	max_test = 0x100;
      break;
    case 25:
      if (trig & CONT_D)
	--n_pages;
      if (trig & CONT_E)
	++n_pages;
      if (hold & CONT_C)
	n_pages = n_pages - 0x8;
      if (hold & CONT_F)
	n_pages = n_pages + 0x8;
      if (n_pages > 0x400)
	n_pages = 1;
      if (n_pages < 1)
	n_pages = 0x400;
      break;
    default:
      if (trig & CONT_D)
	--page_num;
      if (trig & CONT_E)
	++page_num;
      if (hold & CONT_C)
	page_num = page_num - 0x1;
      if (hold & CONT_F)
	page_num = page_num + 0x1;
      if (page_num > 0x3ff)
	page_num = 0;
      if (page_num < 0)
	page_num = 0x3ff;
    }	
    
    /*---------------------------------------------------------------------
      メニュー選択
      ---------------------------------------------------------------------*/
    if((trig & CONT_A) != 0){
      switch(cur_y) {
      case 5:	/* Change Flash */
	osFlashChange(flash_num);
	printstr(white, RESULT_X, RESULT_Y, "CHANGE FLASH NUMBER           ");
	printstr(white, 11, STATUS_Y, "FLASH NUMBER:                 ");
	putint_h32(white, 35,STATUS_Y,flash_num,1);
	break;
	  
      case 6:	/* Read ID */
        osFlashReadId(&status_buf[0],&status_buf[1]);

	printstr(white, RESULT_X, RESULT_Y, "END READ ID                   ");
	printstr(white, 11, STATUS_Y, "ID:     ");
	putint_h32(white, 26,STATUS_Y,status_buf[0],8);
	putint_h32(white, 35,STATUS_Y,status_buf[1],8);
	break;

      case 7:	/* Read Status Register */
	osFlashReadStatus(&status);
	status_buf[0] = (u32)status;
	
	printstr(white, RESULT_X, RESULT_Y, "END READ STATUS               ");
	printstr(white, 11, STATUS_Y, "STATUS: ");	
	putint_h32(white, 26,STATUS_Y,status_buf[0],8);
	printstr(white, 27, STATUS_Y, "         ");
	break;

      case 8:	/* Clear Status Register */
	/* Clear Status Register */
	osFlashClearStatus();	

	printstr(white, 8, STATUS_Y, "                               ");
	printstr(white, RESULT_X, RESULT_Y, "END CLEAR STATUS              ");
	break;

      case 9:	/* All Erase */
	if (!osFlashAllErase())
	  printstr(white, RESULT_X, RESULT_Y, "SUCCESS ALL ERASE             ");
	else
	  printstr(white, RESULT_X, RESULT_Y, "FAIL ALL ERASE                ");
	printstr(white, 8, STATUS_Y, "                               ");
	break;

      case 10:	/* All Erase Through */
	/* All Erase */
	osFlashAllEraseThrough();
#if 0
	printstr(white, RESULT_X, RESULT_Y, "DO ALL ERASE THROUGH            ");
#else	
	while((result = osFlashCheckEraseEnd()) == FLASH_STATUS_ERASE_BUSY);
	if (result == FLASH_STATUS_ERASE_OK)
	  printstr(white, RESULT_X, RESULT_Y, "SUCCESS SECTOR ERASE THROUGH  ");
	else
	  printstr(white, RESULT_X, RESULT_Y, "FAIL SECTOR ERASE THROUGH     ");
	printstr(white, 8, STATUS_Y, "                               ");
#endif
	break;

      case 11:	/* Sector Erase */
	/* Sector Erase */
	if (!osFlashSectorErase(page_num))
	  printstr(white, RESULT_X, RESULT_Y, "SUCCESS SECTOR ERASE          ");
	else
	  printstr(white, RESULT_X, RESULT_Y, "FAIL SECTOR ERASE             ");
	printstr(white, 8, STATUS_Y, "                               ");
	break;
	
      case 12:	/* Sector Erase Through */
	/* Sector Erase */
	osFlashSectorEraseThrough(page_num);
#if 0
	printstr(white, RESULT_X, RESULT_Y, "DO ALL ERASE THROUGH            ");
#else	
	while((result = osFlashCheckEraseEnd()) == FLASH_STATUS_ERASE_BUSY);
	if (result == FLASH_STATUS_ERASE_OK)
	  printstr(white, RESULT_X, RESULT_Y, "SUCCESS SECTOR ERASE THROUGH  ");
	else
	  printstr(white, RESULT_X, RESULT_Y, "FAIL SECTOR ERASE THROUGH     ");
	printstr(white, 8, STATUS_Y, "                               ");
#endif
	break;

      case 13:	/* Read Array (DMA Read)*/
	osInvalDCache(&read_buf[page_num*32], n_pages*128);
	osFlashReadArray(&dmaIOMessageBuf, OS_MESG_PRI_NORMAL, page_num, 
			 &read_buf[page_num*32], n_pages, &n_dmaMessageQ);
	(void)osRecvMesg(&n_dmaMessageQ, NULL, OS_MESG_BLOCK);
	printstr(white, RESULT_X, RESULT_Y, "END DATA READ (DMA)           ");
	printstr(white, 8, STATUS_Y, "                               ");
	break;

      case 14:	/* Write Buffer */
	osWritebackDCache(&write_buf[0], 128);
	osFlashWriteBuffer(&dmaIOMessageBuf, OS_MESG_PRI_NORMAL, 
			 &write_buf[page_num*32], &n_dmaMessageQ);
	(void)osRecvMesg(&n_dmaMessageQ, NULL, OS_MESG_BLOCK);
	printstr(white, RESULT_X, RESULT_Y, "END BUFFER WRITE              ");
	printstr(white, 8, STATUS_Y, "                               ");
	break;

      case 15:	/* Write Array */
	if (!osFlashWriteArray(page_num))
	  printstr(white, RESULT_X, RESULT_Y, "SUCCESS WRITE               ");
	else
	  printstr(white, RESULT_X, RESULT_Y, "FAIL WRITE                    ");
	printstr(white, 8, STATUS_Y, "                               ");
	break;
	
      case 17:	/* Fill Write Buffer(rnd) */
	fill_write_buf(page_num);	
	printstr(white, RESULT_X, RESULT_Y, "END FIll WRITE BUFFER        ");
	printstr(white, 8, STATUS_Y, "                              ");
	break;

      case 18:	/* Clear Read Buffer */
	clear_read_buf();	
	printstr(white, RESULT_X, RESULT_Y, "END CLEAR READ BUFFER        ");
	printstr(white, 8, STATUS_Y, "                              ");
	break;

      case 19:	/* Clear Write Buffer */
	clear_write_buf();	
	printstr(white, RESULT_X, RESULT_Y, "END CLEAR WRITE BUFFER       ");
	printstr(white, 8, STATUS_Y, "                              ");
	break;

      case 20:	/* All Write Data */
	all_write_data();
	printstr(white, RESULT_X, RESULT_Y, "END ALL WRITE                ");
	printstr(white, 8, STATUS_Y, "                              ");
	break;

      case 21:	/* All Read Data */
	all_read_data();
	printstr(white, RESULT_X, RESULT_Y, "END ALL READ                 ");
	printstr(white, 8, STATUS_Y, "                              ");
	break;

      case 22:	/* All Compare Data */
	result = all_compare_data();
	if (result == -1)
	  printstr(white, RESULT_X, RESULT_Y, "SUCCESS COMPARE              ");
	else{
	  printstr(white, RESULT_X, RESULT_Y, "FAIL COMPARE                 ");
	  putint_h32(white, 27,RESULT_Y, result,3);
	}
	printstr(white, 8, STATUS_Y, "                              ");
	break;

      case 23:	/* Region Compare */
	result = region_compare_data(page_num,n_pages);
	if (result == -1)
	  printstr(white, RESULT_X, RESULT_Y, "SUCCESS COMPARE              ");
	else{
	  printstr(white, RESULT_X, RESULT_Y, "FAIL COMPARE                 ");
	  putint_h32(white, 27,RESULT_Y, result,3);
	}
	printstr(white, 8, STATUS_Y, "                              ");
	break;

      case 24:	/* All TEST */
	for (i = 1; i<=max_test; ++i){
	  if (all_write_data()!=0){
	    putint_h32(white, 36,RESULT_Y, i,5);
	    break;
	  }
	  all_read_data();
	  result = all_compare_data();
	  if (result == -1){
	    printstr(white, RESULT_X, RESULT_Y, "SUCCESS COMPARE              ");
	    putint_h32(white, 36,RESULT_Y, i,5);
	  }
	  else{
	    printstr(white, RESULT_X, RESULT_Y, "FAIL COMPARE                 ");
	    putint_h32(white, 36,RESULT_Y, i,5);
	    putint_h32(white, 27,RESULT_Y, result,3);
	    break;
	  }
	  printstr(white, 8, STATUS_Y, "                              ");
	}
	break;
      }
    }
  }
}  
