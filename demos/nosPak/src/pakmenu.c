 /*---------------------------------------------------------------------
  File : pakmenu.c	version 1.5

  Coded     by Yoshitaka Yasumoto.  Feb 20, 1996.
  Modified  by Koji Mitsunari.      Oct  9, 1996.
  Copyright by Nintendo, Co., Ltd.          1997,1998.
  ---------------------------------------------------------------------*/
#include	"os.h"
#include	<ultra64.h>
#include	"pakmenu.h"

extern	void	nosLoadFont(u16 *);
extern	void	canvas_putc(int, u16, u16);
extern	void	canvas_puts(u8 *, int, u16, u16);
extern	u16	*__cursor;
extern	void	canvas_u8_to_str(u16, u8*);

//
//
OSPfsState	pfs_stat[16];
s32		pfs_stat_ret[16];
  
void	__display_term(int sel, int n, u16 fg1, u16 fg2, u16 bg)
{
  u8	ibuf[4];
  int	cy;
  static  u8  *cmd_str[] = {
    STR_CMD_ERASE, STR_CMD_OK, STR_CMD_QUIT,
  };
  static  u16  cmd_str_len[] = {
    sizeof(STR_CMD_ERASE)-1, sizeof(STR_CMD_OK)-1, sizeof(STR_CMD_QUIT)-1,
  };
  static  s16  cmd_str_yofs[] = {
    8, MENU_IS_OK_YOFS*SCREEN_WD+8, MENU_IS_NO_YOFS*SCREEN_WD+8,
  };
  
  //
  // Check if pointer on the filename
  // --------------------------------
  cy = n * FONT_HSKIP + MENU_FILE_Y;
  if ( n < 16 ){
    
    //
    // Check if file exists
    // --------------------
    if ( !pfs_stat_ret[n] ){
      
      //
      // Draw filename and # of blocks
      // -----------------------------
      canvas_cursor( MENU_FILE_X, cy );
      canvas_puts( pfs_stat[n].game_name, 16, fg1, bg );
      if (pfs_stat[n].ext_name[0]){
	canvas_putc( 0x3c, fg1, bg );
	__cursor -= MENU_EXT_X;
	canvas_putc( pfs_stat[n].ext_name[0], fg1, bg );
      }
      canvas_cursor( MENU_BLOCK_X, cy );
      canvas_u8_to_str( (u16)((pfs_stat[n].file_size+255) >> 8), ibuf );
      canvas_puts( ibuf, 3, fg1, bg ); 

      //
      // Draw "ERASE"
      // ------------
      __cursor += cmd_str_yofs[sel];
      canvas_puts( cmd_str[sel], cmd_str_len[sel], fg2, bg );
    }
    
    //
    // Draw # (1-16):
    // --------------
    if (n < 9){
      canvas_cursor( MENU_NUM_X + 3, cy );
      canvas_putc  ( n+0x11, fg1, bg );
    } else {
      canvas_cursor( MENU_NUM_X - 4, cy );
      canvas_putc( 0x11, fg1, bg );
      canvas_cursor( MENU_NUM_X + 4, cy );
      canvas_putc( n+7, fg1, bg );
    }
    canvas_cursor( MENU_NUM_X + 15, cy );
    canvas_putc( 0x3e, fg1, bg );
    
  } else {
      
    //
    // Draw "QUIT"
    // -----------
    canvas_cursor( MENU_NUM_X-2, cy+4 );
    canvas_putstr( STR_CMD_END, fg1, bg );
  }
    
}

void	__display_free_block(u16 free_blocks)
{
  u8	ibuf[4];
  int	i;
  int	j;
  int	k;
  
  canvas_u8_to_str(free_blocks, ibuf);
  canvas_cursor( MENU_FREE_BLK_X2, MENU_FREE_BLK_Y2 );

  for (i = 0; i < FONT_HT; i ++){
    for (j = 0; j < FONT_WSKIP * 3; j ++){
      k = i * SCREEN_WD + j;
      ((u16 *)FBUF_ADRS(MENU_FREE_BLK_X2, MENU_FREE_BLK_Y2))[k] = (u16)BGCOLOR;
    }
  }
  canvas_puts( ibuf, 3, MENU_COLOR_FILE, MENU_COLOR_SHADOW );
}

//
//  Returns by 0 if "PAK MENU" no used, 1 if "PAK MENU" used.
//
int	nosPakMenu( OSMesgQueue *siMessageQ, OSMesgQueue *retraceMessageQ )
{
  OSMesg	dummyMesg;
  
  OSContStatus	stat[MAXCONTROLLERS];
  OSContPad	data[MAXCONTROLLERS];
  u8		exist;
  u16		trig;
  u16		fg;
  u16		repeat_trig, repeat_cnt; 
  
  OSPfs		pfs;
  
  s32		file_num, file_num_max, free_blocks;
  
  int		mode, save_mode;
  int		target;
  int		save_target;
  int		i;
  int		flash_cnt;
  int		screen_type;
  
  static	u16	cursor_color[] = {
    MENU_COLOR_TARGET_7, MENU_COLOR_TARGET_8, MENU_COLOR_TARGET_9,
    MENU_COLOR_TARGET_0, MENU_COLOR_TARGET_1, MENU_COLOR_TARGET_2,
    MENU_COLOR_TARGET_3, MENU_COLOR_TARGET_4, MENU_COLOR_TARGET_5,
    MENU_COLOR_TARGET_6,
  };
  
  //
  // Check if plugged on port 1
  // --------------------------
//  osContSetCh(PORT_ID+1);			// Check for PORT 1
  osContInit(siMessageQ, &exist, stat);		// Initialize
  osContStartReadData(siMessageQ);		// Scan controller data
  nWaitMesg(si);
  osContGetReadData(data);			// Get controller data
  if (!exist) return 0;		 		// Return if no connect

  //
  // Check if any device plugged on
  // ------------------------------
  if (!(stat[PORT_ID].status & CONT_CARD_ON)) return 0;
  
  //
  // Check if START button pushed
  // ----------------------------
  osContStartReadData(siMessageQ);		// Scan controller data
  nWaitMesg(si);
  osContGetReadData(data);			// Get controller data

  //
  // Check if CONTROLLER PAK plugged
  // -------------------------------
  if (data[PORT_ID].button & START_BUTTON) {
    mode = osPfsInitPak(siMessageQ, &pfs, PORT_ID);
    if (mode != 0 && mode != PFS_ERR_ID_FATAL) {
      return 0; // Return if no Controller Pak
    }
  //
  // Load font
  // --------- 
    nosLoadFont((u16 *)FONT_BUF);
  //
  // Clear screen
  // -----------------
    for (i = 0; i < FBUF_SIZE / sizeof(u32); i ++){
      ((u32 *)FBUF_TOP)[i] = (BGCOLOR << 16) | BGCOLOR;
    }
    osViSwapBuffer((u32 *)FBUF_TOP);
#if 0
#if	SCREEN_LOW
    osViSetMode(&osViModeTable[OS_VI_NTSC_LPN1]);
#else
    osViSetMode(&osViModeTable[OS_VI_NTSC_HPF1]);
#endif
#endif
    osViBlack(0);
  
    if ( mode == PFS_ERR_ID_FATAL ) {
      canvas_cursor( MENU_IDERR_X, MENU_IDERR_Y );
      canvas_putstr( STR_PAK_IDERR, MENU_COLOR_IDERR, MENU_COLOR_TITLE );
      canvas_cursor( MENU_IDERR_X1, MENU_IDERR_Y1 );
      canvas_putstr( STR_PAK_IDERR1, MENU_COLOR_TITLE, MENU_COLOR_TITLE );
      canvas_cursor( MENU_IDERR_X2, MENU_IDERR_Y2 );
      canvas_putstr( STR_PAK_IDERR2, MENU_COLOR_TITLE, MENU_COLOR_TITLE );
      osWritebackDCacheAll();
      nWaitMesg(retrace);
      do {
	osContStartReadData(siMessageQ);	// Scan controller data
	nWaitMesg(si);
        osContGetReadData(data);		// Get controller data
        if ( data[PORT_ID].button == 0 ) {
	  continue;
	} else if ( data[PORT_ID].button == (L_TRIG|R_TRIG) ) {
	  osPfsRepairId(&pfs);
	}
	mode = osPfsInitPak(siMessageQ, &pfs, PORT_ID);	/* Any button */
      } while (mode != 0);
    }
  } else {
    return 2;// Return if not START
  }

  //
  // Get number of files
  // -------------------
  if (osPfsNumFiles(&pfs, &file_num_max, &file_num)) return 0;
  
  //
  // Get file status
  // ---------------
  for (i = 0; i < 16; i ++){
    pfs_stat_ret[i] = osPfsFileState(&pfs, i, pfs_stat+i);
  }
  
  //
  // Get free memory
  // ---------------
  if (osPfsFreeBlocks(&pfs, &free_blocks)) return 0;
  free_blocks >>= 8;

  
  //
  // Initialize screen
  // -----------------
  screen_type = STYPE_MENU;
 display_screen:
  for (i = 0; i < FBUF_SIZE / sizeof(u32); i ++){
    ((u32 *)FBUF_TOP)[i] = (BGCOLOR << 16) | BGCOLOR;
  }
  
  switch(screen_type){
  case	STYPE_MENU:
    //
    // Display table of game data
    // --------------------------
    canvas_cursor( MENU_TITLE_X, MENU_TITLE_Y );
    canvas_putstr( STR_MENU_TITLE, MENU_COLOR_TITLE, MENU_COLOR_SHADOW );
/* Display version for controller pak function */
#ifdef	OS_PFS_VERSION
    canvas_cursor( MENU_TITLE_X+16*19, MENU_TITLE_Y );
    canvas_putstr( STR_MENU_VER, MENU_COLOR_TITLE, MENU_COLOR_SHADOW );
    canvas_putc( (OS_PFS_VERSION_HI+0x10), MENU_COLOR_TITLE, MENU_COLOR_SHADOW );
    canvas_putc( 0x3c, MENU_COLOR_TITLE, MENU_COLOR_SHADOW );
    canvas_putc( (OS_PFS_VERSION_LO+0x10), MENU_COLOR_TITLE, MENU_COLOR_SHADOW );
#endif
    canvas_cursor( MENU_NUM_X, MENU_FILE_Y-FONT_HSKIP );
    canvas_putstr( STR_MENU_ITEMS, MENU_COLOR_ITEM, MENU_COLOR_SHADOW );
    canvas_cursor( MENU_FREE_BLK_X1, MENU_FREE_BLK_Y1 );
    canvas_putstr( STR_BLK_LEFT, MENU_COLOR_ITEM, MENU_COLOR_SHADOW );
    __display_free_block( (u16)(free_blocks) );
    
    for (i = 0; i < 17; i ++){
      __display_term( 0, i,
		      MENU_COLOR_FILE, MENU_COLOR_SHADOW, MENU_COLOR_SHADOW );
    }
    break;
    
  case	STYPE_NODEL:
    //
    // Display message "Cannot delete file"
    // ------------------------------------
    canvas_cursor( MENU_WARN_X, MENU_WARN_Y );
    canvas_putstr( STR_PAK_NODEL, MENU_COLOR_TITLE, MENU_COLOR_TITLE );
    file_num = 0;
    break;
  }
  
  osWritebackDCacheAll();
  nWaitMesg(retrace);
  
  repeat_cnt  = 0;
  repeat_trig = 0;
  target    = 16;
  trig      = D_JPAD;
  flash_cnt = 0;
  mode      = 0;
  while(1){
    save_target = target;
    save_mode   = mode;
    switch(mode){
    case	0:
      if (trig & A_BUTTON){
	
	// Return if "QUIT" selected
	if (target == 16) {
	  if (screen_type == STYPE_NODEL) {
	    return 1;
	  } else {
	    return 3;
	  }
	}
	
	// Block out other selections
	for (i = 0; i < 17; i ++){
	  __display_term( 0, i, MENU_COLOR_NOTSEL,
			        MENU_COLOR_SHADOW, MENU_COLOR_SHADOW );
	}
	mode = 2;
	
      } else if ( (trig | repeat_trig) & U_JPAD) {
	// Go previous file
	while (pfs_stat_ret[--target] && target >= 0);
	if (target == -1) target = 16;
		
      } else if ( (trig | repeat_trig) & D_JPAD) {
	
	// Go next file
      next_target:
	if (target == 16) target = -1;
	while (pfs_stat_ret[++target] && target <= 15);
	
      } else if (trig & B_BUTTON){
	
	// Go onto "QUIT"
	target = 16;

      }
      break;
      
    case	1:
      if (trig & D_JPAD)   mode = 2;
      if (trig & B_BUTTON) mode = 0;
      if (trig & A_BUTTON){	
	if (osPfsDeleteFile(&pfs,
			     pfs_stat[target].company_code,
			     pfs_stat[target].game_code,
			     pfs_stat[target].game_name,
			     pfs_stat[target].ext_name)){
	  screen_type = STYPE_NODEL;
	  goto display_screen;
	}
	__display_term(1, target, MENU_COLOR_EMPTY,
		                  MENU_COLOR_SHADOW, MENU_COLOR_SHADOW );
	__display_term(2, target, MENU_COLOR_EMPTY,
		                  MENU_COLOR_SHADOW, MENU_COLOR_SHADOW );
	mode = 0;
	pfs_stat_ret[target] = -1;
	free_blocks += (u8)((pfs_stat[target].file_size+255) >> 8);
	__display_free_block( (u16)(free_blocks) );
	goto next_target;
      }      
      break;
      
    case	2:
      if (trig & U_JPAD)             mode = 1;
      if (trig &(A_BUTTON|B_BUTTON)) mode = 0;
      break;      
    }
    
    if (!file_num) target = 16;
    
    if (mode != save_mode){
      fg = (u16)MENU_COLOR_FILE;
      if (mode == 0){

	for (i = 0; i < 17; i ++){
	  __display_term(0, i, MENU_COLOR_FILE,
			       MENU_COLOR_SHADOW, MENU_COLOR_SHADOW);
	}
	fg = (u16)MENU_COLOR_SHADOW;
      }
      __display_term(1, save_target, MENU_COLOR_FILE, fg, MENU_COLOR_SHADOW);
      __display_term(2, save_target, MENU_COLOR_FILE, fg, MENU_COLOR_SHADOW);
      flash_cnt = 0;
    
    } else if ( target != save_target ){
      
      __display_term(0, save_target,
		     MENU_COLOR_FILE, MENU_COLOR_SHADOW, MENU_COLOR_SHADOW );
      flash_cnt = 0;
      
    }
    
    __display_term(mode, target,
		   cursor_color[flash_cnt/3],
		   cursor_color[flash_cnt/3], cursor_color[flash_cnt/3]);
    if ((++ flash_cnt) == 30) flash_cnt = 0;
    
    osContStartReadData(siMessageQ);		// Scan controller data
    osWritebackDCacheAll();
    nWaitMesg(retrace);
    nWaitMesg(si);
    trig = data[PORT_ID].button;
    osContGetReadData(data);			// Get controller data

    repeat_trig = data[PORT_ID].button & trig;
    if (repeat_trig) {
      repeat_cnt ++;
      if ( (repeat_cnt>>5)==0 || (repeat_cnt&7)!=0 ) repeat_trig = 0;
    } else {
      repeat_cnt = 0;
    }

    trig = data[PORT_ID].button & (data[PORT_ID].button ^ trig);
  }
  return 0;
}
