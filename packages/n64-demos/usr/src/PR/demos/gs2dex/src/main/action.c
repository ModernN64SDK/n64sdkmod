/*---------------------------------------------------------------------
	Copyright (C) 1997, Nintendo.
	
	File		action.c
	Coded    by	Yoshitaka Yasumoto.	Mar 19, 1997.
	Modified by	
	Comments	
	
	$Id: action.c,v 1.12 1998/12/23 08:52:25 n64os Exp $
  ---------------------------------------------------------------------*/
#include	<ultra64.h>
#include	<PR/gs2dex.h>
#include	"system.h"
#include	"sprite.h"
#include	"action.h"

void	moveBg(myContPad *);
void	moveObject(myContPad *);
void	moveByPlusKey(u16, s16 *, s16 *, s16);
void	moveByCButtons(u16, s16 *, s16 *, s16);
void	moveBy3DStick(myContPad *, s16 *, s16 *, s16);

#ifdef DEBUG
void	printMenu(s16, s16);
void	menuInit(void);
void	menuHandler(myContPad *);
#endif

/*---------------------------------------------------------------------------*
 * Action structure
 *---------------------------------------------------------------------------*/
Action	Ac;

/*---------------------------------------------------------------------------*
 * menu value
 *---------------------------------------------------------------------------*/
u8	aMenu[NUM_MENU];

/*---------------------------------------------------------------------------*
 * Initialize the Action structure
 *---------------------------------------------------------------------------*/
void	actionInit(void)
{
  int	i;
  
  Ac.active        = 0;
  Ac.imageX        = 0<<2;
  Ac.imageY        = 0<<2;
  Ac.imageYorig    = 0<<2;
  Ac.frameX        = 0<<2;
  Ac.frameY        = 0<<2;
  Ac.frameW        = 320<<2;
  Ac.frameH        = 240<<2;
  Ac.objX          = (SCREEN_WD/2 - 16)<<2;
  Ac.objY          = (SCREEN_HT/2 - 16)<<2;
  Ac.scaleW        = 1<<10;
  Ac.scaleH        = 1<<10;
  Ac.theta         = 0;
  Ac.frameScaleW   = 1<<10;
  Ac.frameScaleH   = 1<<10;
  
  for (i = 0; i < MAXCONTROLLERS; i ++){
    Ac.pad[i].c = contPad + i;
    Ac.pad[i].button = Ac.pad[i].push = Ac.pad[i].release = 0;
  }
  return;
}

/*---------------------------------------------------------------------------*
 * Set Action structure members according to the Menu state.
 *---------------------------------------------------------------------------*/
void	actionUpdate(void)
{
  int	i;
  u16	button, change;
  
  /*---- Calculate Controller trigger ----*/
  for (i = 0; i < MAXCONTROLLERS; i ++){
    button = Ac.pad[i].c->button;
    change = Ac.pad[i].button ^ button;
    Ac.pad[i].release = (Ac.pad[i].push = change & button) ^ change;
    Ac.pad[i].button = button;
  }

#ifdef DEBUG  
  /*---- Menu selection process ----*/
  menuHandler(Ac.pad+0);
#endif
  
  /*---- Move sprite process ----*/
  if (aMenu[MENU_CONTROL]){
    moveBg    (Ac.pad+0);
    moveObject(Ac.pad+1);
  } else {
    moveObject(Ac.pad+0);  
    moveBg    (Ac.pad+1);
  }
  return;
}

/*---------------------------------------------------------------------------*
 * Bg manipulations
 *---------------------------------------------------------------------------*/
void	moveBg(myContPad *mc)
{	
  s16	imageW, imageH;
  
  imageW = objBg.b.imageW * 8;
  imageH = objBg.b.imageH * 8;

  /*------ Scroll with 3D stick ------*/
  moveBy3DStick(mc, &Ac.imageX, &Ac.imageY, 8);
  
  /*------ Fine-move with the control pad ------*/
  moveByPlusKey(mc->push, &Ac.imageX, &Ac.imageY, 8);
  
  /*------ Screen edga Wrap process ------*/
  if (Ac.imageX < 0){
    Ac.imageX     += imageW;
    Ac.imageY     -= 32;
    Ac.imageYorig -= 32;
  }
  if (Ac.imageX >= imageW){
    Ac.imageX     -= imageW;
    Ac.imageY     += 32;
    Ac.imageYorig += 32;
  }
  if (Ac.imageY < 0){
    Ac.imageY     += imageH;
    Ac.imageYorig += imageH;
  }
  if (Ac.imageY >= imageH){
    Ac.imageY     -= imageH;
    Ac.imageYorig -= imageH;
  }
  
  /*------ Move frame with C Button ------*/
  if (mc->button & Z_TRIG){
    moveByCButtons(mc->button, (s16 *)&Ac.frameW, (s16 *)&Ac.frameH, 8);
  } else {
    moveByCButtons(mc->button, &Ac.frameX, &Ac.frameY, 8);
  }
  
  /*------ Scale with LR keys ------*/
  if (mc->button & L_TRIG){
    Ac.frameScaleW -= 32;
    Ac.frameScaleH -= 32;
    if (Ac.frameScaleW <= 0) Ac.frameScaleW = 32;
    if (Ac.frameScaleH <= 0) Ac.frameScaleH = 32;
    Ac.imageYorig = Ac.imageY;
  }
  if (mc->button & R_TRIG){
    Ac.frameScaleW += 32;
    Ac.frameScaleH += 32;
    Ac.imageYorig = (s32)Ac.imageY;
  }

  /*------ To initial position with START ------*/
  if (mc->push & START_BUTTON){
    Ac.imageX = Ac.imageY = 0;
    Ac.frameX = Ac.frameY = 0;    
    Ac.imageYorig = 0L;
    Ac.frameScaleW = Ac.frameScaleH = 1<<10;
  }
  return;
}

/*---------------------------------------------------------------------------*
 * Obj manipulations
 *---------------------------------------------------------------------------*/
void	moveObject(myContPad *mc)
{	
  /*------ Move with 3D stick ------*/
  moveBy3DStick(mc, &Ac.objX, &Ac.objY, 1);
  
  /*------ Fine-move with control pad ------*/
  moveByPlusKey(mc->push, &Ac.objX, &Ac.objY, 1);
  
  /*------ Scale with the C Button ------*/
  moveByCButtons(mc->button, (s16 *)&Ac.scaleW, (s16 *)&Ac.scaleH, -32);
  if (Ac.scaleW <= 0) Ac.scaleW = 32;
  if (Ac.scaleH <= 0) Ac.scaleH = 32;
  
  /* Rotate with LR keys */
  if (mc->button & L_TRIG) Ac.theta -= 64;
  if (mc->button & R_TRIG) Ac.theta += 64;
  
  /*------ To initial position with START ------*/
  if (mc->push & START_BUTTON){
    Ac.objX = (SCREEN_WD/2 - 16)<<2;
    Ac.objY = (SCREEN_HT/2 - 16)<<2;
    Ac.scaleW = Ac.scaleH = 1<<10;
    Ac.theta = 0;
  }
  return;
}

/*---------------------------------------------------------------------------*
  Calculate and return the amount of movement corresponding 
  to the input of the control pad. 
 *---------------------------------------------------------------------------*/
void	moveByPlusKey(u16 button, s16 *x, s16 *y, s16 factor)
{
  if (button & U_JPAD) (*y) -= factor;
  if (button & D_JPAD) (*y) += factor;
  if (button & L_JPAD) (*x) -= factor;
  if (button & R_JPAD) (*x) += factor;
}  

/*---------------------------------------------------------------------------*
 Calculate and return the amount of movement corresponding 
 to the input of the C Button unit 
 *---------------------------------------------------------------------------*/
void	moveByCButtons(u16 button, s16 *x, s16 *y, s16 factor)
{
  if (button & U_CBUTTONS) (*y) -= factor;
  if (button & D_CBUTTONS) (*y) += factor;
  if (button & L_CBUTTONS) (*x) -= factor;
  if (button & R_CBUTTONS) (*x) += factor;
} 

/*---------------------------------------------------------------------------*
  Calculate and return the amount of movement corresponding 
 to the tilt of the control stick.  
 *---------------------------------------------------------------------------*/
void	moveBy3DStick(myContPad *p, s16 *x, s16 *y, s16 factor)
{
  static s16 MoveFactor[] = { -8,-8,-4,-2,-1, 0,0, +1,+2,+4,+8,+8 };
  s16	nx, ny;
  
  nx = (p->c->stick_x + 96) >> 4;
  ny = (p->c->stick_y + 96) >> 4;
  
  (*x) += MoveFactor[nx] * factor;
  (*y) -= MoveFactor[ny] * factor;
} 


#ifdef DEBUG

/*---------------------------------------------------------------------------*
  Menu definitions
 *---------------------------------------------------------------------------*/
char	*menu_mesg[NUM_MENU][8] = {
{ "Control Pad 1      => ", "Object            ", "Bg                ", 0 },
{ "OBJ Render Mode    => ", "NonAA+Point+1cycle", "NonAA+Point+Copy  ",
                            "NonAA+Bilerp      ", "AA+Bilerp         ", 0 },
{ "OBJ Render Mode 2  => ", "Opaque            ", "Translucent       ", 0 },
{ "OBJ Texture Type   => ", "RGBA16            ", "CI4               ", 0 },
{ "OBJ TextureLoad by => ", "LoadBlock         ", "LoadTile          ", 0 },
{ "OBJ Texture Window => ", "on                ", "off               ", 0 },
{ "OBJ Shrink panel   => ", "off               ", "-1                ",
                            "-2                ", 0 },
{ "OBJ Flip S         => ", "off               ", "on                ", 0 },
{ "OBJ Flip T         => ", "off               ", "on                ", 0 },

{ "BG Scalable        => ", "off               ", "on  (Emulated)    ",
                            "on  (GBI)         ", 0 },
{ "BG TextureLoad by  => ", "LoadBlock         ", "LoadTile          ", 0 },
{ "BG Texture Format  => ", "RGBA16            ", "CI8               ", 0 },
{ "BG Flip S          => ", "off               ", "on                ", 0 },
};

/*---------------------------------------------------------------------------*
  Display of menu items
 *---------------------------------------------------------------------------*/
void	printMenu(s16 id, s16 sw)
{
  /* Menu line display, if sw=1, highlight */
  osSyncPrintf( "[ Menu No %02d ] %s ( %s )\n", ( id + 1 ),
		menu_mesg[id][0], menu_mesg[id][1+aMenu[id]] );
}

/*---------------------------------------------------------------------------*
  Menu initialization
 *---------------------------------------------------------------------------*/
void	menuInit(void)
{
  int	i;
  
  osSyncPrintf("\033[2J");	/* Screen clear */
  for (i = 1; i < NUM_MENU; i ++){
    aMenu[i] = 0;
    printMenu(i, 0);
  }
  printMenu(0, 1);
}

/*---------------------------------------------------------------------------*
  Manipulation of menu with controller
 *---------------------------------------------------------------------------*/
void	menuHandler(myContPad *mc)
{
  static s16 active = 0;
  
  /* Switch options with the A button */
  if (mc->push & A_BUTTON){
    aMenu[active] ++;
    if (!menu_mesg[active][aMenu[active]+1]) aMenu[active] = 0;
    osSyncPrintf( "Commit Menu!!\n" );
    printMenu(active, 1);
  }
  
  /* Select items with the B button. */
  if ( mc->push & D_JPAD ) {
    active += (active < NUM_MENU-1)? 1 : 1-NUM_MENU;
    printMenu(active, 1);
  } else if ( mc->push & U_JPAD ) {
    --active;
    if ( active < 0 ) active = ( NUM_MENU - 1 );
    printMenu(active, 1);
  }

  return;
}

#endif

/*======== End of action.c ========*/
