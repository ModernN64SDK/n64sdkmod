
/*************************************************************

  test_menu.c : Nintendo 64 Music Tools Library Sample
  (c) Copyright 1998, Software Creations (Holdings) Ltd.

  Version 3.14

  TESTMENU demo simple menu file. Uses a simple menu to 
  illustrate the use of various music library functions.

**************************************************************/

/* include system header files */
#ifndef F3DEX_GBI
#define F3DEX_GBI
#endif

#include <ultra64.h>
#include <libmus.h>

/* include current header file */
#include "test_rom.h"

/* screen position for main menu */
#define MAIN_XPOS	130
#define MAIN_YPOS	24

/* number of songs and effects */
#define SONG_COUNT	4
#define EFFECT_COUNT	(MusFxBankNumberOfEffects(sfx_buffer1))

/* pointer character graphic */
extern unsigned short letterPointer[];

/* amount of heap memory used */
extern int heap_used;

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [GLOBAL FUNCTION]
  MenuProcessController()

  [Explanation]
  Process the controller and generate a pressed list to be used by the menu 
  processing functions.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

#define BUTTON_UP	0
#define BUTTON_DOWN	1
#define BUTTON_LEFT	2
#define BUTTON_RIGHT	3
#define BUTTON_L	4
#define BUTTON_R	5
#define BUTTON_A	6
#define BUTTON_B	7

#define BUTTONS		8

int pressed[BUTTONS];

void MenuProcessController(void)
{
   static unsigned long movement[BUTTONS]=
   {
      CONT_UP, CONT_DOWN, CONT_LEFT, CONT_RIGHT, CONT_L, CONT_R,
      CONT_A, CONT_B,
   };
   static unsigned long count_limit[BUTTONS]=
   {
      15,15,15,15,15,15,
      -1,-1,
   };
   static unsigned long count[BUTTONS]=
   {
      0,0,0,0,0,0,
      0,0,
   };
   int i;

   /* read controllers */
   osContStartReadData(&serial_queue);
   osRecvMesg(&serial_queue, NULL, OS_MESG_BLOCK);
   osContGetReadData(controllerdata);

   /* process movements */
   for (i=0; i<BUTTONS; i++)
   {
      pressed[i]=0;
      if (controllerdata[0].button&movement[i])
      {
         if (count[i])
         {
            count[i]++;
            if (count[i]>count_limit[i])
               count[i]=0;
         }
         else
         {
            count[i]=1;
            pressed[i]=1;
         }
      }
      else
      {
         count[i]=0;
      }
   }
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

  Menu structures and data...

*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

typedef struct
{
   char *text;
   void (*process)(int);
   unsigned long data;
} menu_entry_t;

typedef struct
{
   int xpos, ypos;
   int entries;
   void (*draw)(void);
   menu_entry_t *entry_list;
} menu_t;

/* function prototypes for main menu processing functions */
void ProcessSong(int type);
void ProcessSongVol(int type);
void ProcessSongPan(int type);
void ProcessSongTempo(int type);
void ProcessEffect(int type);
void ProcessEffectVol(int type);
void ProcessEffectPan(int type);
void ProcessEffectFreq(int type);
void ProcessStopAllSongs(int type);
void ProcessStopAllEffects(int type);
void ProcessStopAll(int type);
void ProcessStopSong(int type);
void ProcessStopEffect(int type);
void ProcessSongsVolume(int type);
void ProcessEffectsVolume(int type);

/* main menu entries */
menu_entry_t main_entries[]=
{
  { "SONG",		ProcessSong },
  { "SONG VOL",		ProcessSongVol },
  { "SONG PAN",		ProcessSongPan },
  { "SONG TEMPO",	ProcessSongTempo },
  { "EFFECT", 		ProcessEffect },
  { "EFFECT VOL",	ProcessEffectVol },
  { "EFFECT PAN",	ProcessEffectPan },
  { "EFFECT FREQ",	ProcessEffectFreq },
  { NULL,NULL },
  { "STOP SONG", 	ProcessStopSong },
  { "STOP EFFECT", 	ProcessStopEffect },
  { NULL,NULL},
  { "STOP ALL SONGS", 	ProcessStopAllSongs },
  { "STOP ALL EFFECTS", ProcessStopAllEffects },
  { "STOP ALL", 	ProcessStopAll },
  { NULL,NULL},
  { "SONGS VOL",	ProcessSongsVolume },
  { "EFFECTS VOL",	ProcessEffectsVolume },
};

/* function prototypes for menu definitions */
void DrawMain(void);

/* main menu */
menu_t main_menu=
{
   MAIN_XPOS, MAIN_YPOS,
   18,
   DrawMain,
   main_entries
};

/* menu control vars */
static menu_t	*current_menu;
static int	current_pos;
static int	menu_counter;

/* processing function vars */
static int 	song_number;
static int 	song_vol;
static int 	song_pan;
static int	song_tempo;
static int 	effect_number;
static int 	effect_vol;
static int 	effect_pan;
static int	effect_tempo;
static int 	song_fade_time;
static int 	effect_fade_time;
static int 	song_fade_time2;
static int 	effect_fade_time2;
static int 	last_stop_song;
static int 	last_stop_effect;

static int 	song_volume;
static int 	effect_volume;

static float	effect_freq_offset;

static unsigned long last_song_handle;
static unsigned long last_effect_handle;


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [GLOBAL FUNCTION]
  MenuInit()

  [Explanation]
  Initialise the menu routine and all processing functions variables.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

void MenuInit(void)
{
  current_menu = &main_menu;
  current_pos = menu_counter = 0;
  song_number = effect_number = 0;
  last_song_handle = last_effect_handle = 0;
  song_fade_time = song_fade_time2 = 0;
  effect_fade_time = effect_fade_time2 = 0;
  song_volume = effect_volume = 0x7fff;
  song_vol = effect_vol = 0x80;
  song_pan = effect_pan = 0x80;
  song_tempo = effect_tempo = 0x80;
  effect_freq_offset = 0.0;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [GLOBAL FUNCTION]
  MenuProcess()

  [Explanation]
  Process the menu. This first draws the menu and then passes any key presses to the
  relevant menu entry processing function.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

void MenuProcess(void)
{
   int i;
   menu_entry_t *ent;
   static unsigned long process_buttons[6]=
   {
      BUTTON_LEFT, BUTTON_RIGHT, BUTTON_L, BUTTON_R, BUTTON_A, BUTTON_B
   };
  
   /* display menu */
   for (i=0; i<current_menu->entries; i++)
   {
      ent = &current_menu->entry_list[i];
      if (ent->text)
         DrawString(ent->text, current_menu->xpos, current_menu->ypos+(i*8));
   }
   if (current_menu->draw)
      current_menu->draw();

   if (menu_counter++%60 > 30)
      DrawChar(letterPointer, current_menu->xpos-10, current_menu->ypos+(current_pos*8));

   /* move up and down menu */
   do
   {
      if (pressed[BUTTON_UP] && current_pos)
         current_pos--;
      else if (pressed[BUTTON_DOWN] && current_pos<current_menu->entries-1)
         current_pos++;
   } while (!current_menu->entry_list[current_pos].process);

   /* stop all sound if button B is pressed */
   if (pressed[BUTTON_B])
      MusStop(MUSFLAG_EFFECTS|MUSFLAG_SONGS, 0);

   /* process pressed buttons */
   if (current_menu->entry_list[current_pos].process)
   {
      for (i=0; i<6; i++)
      {
         if (pressed[process_buttons[i]])
         {
            current_menu->entry_list[current_pos].process(process_buttons[i]);
            break;
         }
      }
   }
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  GetSongAddr()

  [Parameters]
  song		song number

  [Explanation]
  Get the address of the specified song.

  [Return value]
  address of the song in RAM
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

static int *GetSongAddr(int song)
{
   static int *list[]=
   {
      tune_buffer1,tune_buffer2,tune_buffer3,tune_buffer3,
   };

   return (list[song]);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  [INTERNAL FUNCTION]
  DrawMain()

  [Explanation]
  Update the main menu.

  [Return value]
  NONE
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

void DrawMain(void)
{
   char work[32];
   int song_channels;
   int effect_channels;
   int count;

   song_channels = MusHandleAsk(last_song_handle);
   effect_channels = MusHandleAsk(last_effect_handle);

   sprintf(work,": %d (%d)",song_number+1, song_channels);
   DrawString(work, MAIN_XPOS+(12*8), MAIN_YPOS);

   sprintf(work,": %02x",song_vol);
   DrawString(work, MAIN_XPOS+(12*8), MAIN_YPOS+(1*8));
   sprintf(work,": %02x",song_pan);
   DrawString(work, MAIN_XPOS+(12*8), MAIN_YPOS+(2*8));
   sprintf(work,": %02x",song_tempo);
   DrawString(work, MAIN_XPOS+(12*8), MAIN_YPOS+(3*8));

   sprintf(work,": %d (%d)",effect_number+1, effect_channels);
   DrawString(work, MAIN_XPOS+(12*8), MAIN_YPOS+(4*8));

   sprintf(work,": %02x",effect_vol);
   DrawString(work, MAIN_XPOS+(12*8), MAIN_YPOS+(5*8));
   sprintf(work,": %02x",effect_pan);
   DrawString(work, MAIN_XPOS+(12*8), MAIN_YPOS+(6*8));

   sprintf(work,": %0.02f", effect_freq_offset);
   DrawString(work, MAIN_XPOS+(12*8), MAIN_YPOS+(7*8));

   if (song_fade_time2)
   {
      song_fade_time2--;
      sprintf(work,": %d (%d)",song_fade_time,last_stop_song);
   }
   else
   {
      sprintf(work,": %d",song_fade_time);
   }
   DrawString(work, MAIN_XPOS+(12*8), MAIN_YPOS+(9*8));

   if (effect_fade_time2)
   {
      effect_fade_time2--;
      sprintf(work,": %d (%d)",effect_fade_time,last_stop_effect);
   }
   else
   {
      sprintf(work,": %d",effect_fade_time);
   }
   DrawString(work, MAIN_XPOS+(12*8), MAIN_YPOS+(10*8));

   sprintf(work,": %04x",song_volume);
   DrawString(work, MAIN_XPOS+(12*8), MAIN_YPOS+(16*8));
   sprintf(work,": %04x",effect_volume);
   DrawString(work, MAIN_XPOS+(12*8), MAIN_YPOS+(17*8));

   DrawString("CHANNELS", MAIN_XPOS, MAIN_YPOS+(20*8));
   sprintf(work, "SONGS  =%2d", MusAsk(MUSFLAG_SONGS));
   DrawString(work, MAIN_XPOS, MAIN_YPOS+(21*8));
   sprintf(work, "EFFECTS=%2d", MusAsk(MUSFLAG_EFFECTS));
   DrawString(work, MAIN_XPOS, MAIN_YPOS+(22*8));
   sprintf(work, "ALL    =%2d", MusAsk(MUSFLAG_SONGS|MUSFLAG_EFFECTS));
   DrawString(work, MAIN_XPOS, MAIN_YPOS+(23*8));

   DrawString("HEAP USED", MAIN_XPOS+(13*8), MAIN_YPOS+(20*8));
   sprintf(work, "%d", heap_used);
   DrawString(work, MAIN_XPOS+(13*8), MAIN_YPOS+(21*8));
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

  Processing functions...

*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

extern int		pointer_buf	[];

void ProcessSong(int type)
{
   int *addr;

   switch (type)
   {
      case BUTTON_A:
         addr = GetSongAddr(song_number);
         last_song_handle = MusStartSong(addr);
         break;
      case BUTTON_LEFT:
         if (song_number)
            song_number--;
         break;
      case BUTTON_RIGHT:
         if (song_number<SONG_COUNT-1)
            song_number++;
         break;
   }
}

void ProcessSongVol(int type)
{
   switch (type)
   {
      case BUTTON_LEFT:
         song_vol--;
         break;
      case BUTTON_RIGHT:
         song_vol++;
         break;
      case BUTTON_L:
         song_vol-=16;
         break;
      case BUTTON_R:
         song_vol+=16;
         break;
      case BUTTON_A:
         MusHandleSetVolume(last_song_handle, song_vol);
         break;
   }
   if (song_vol<0)
      song_vol=0;
   else if (song_vol>0x100)
      song_vol=0x100;
}

void ProcessSongPan(int type)
{
   switch (type)
   {
      case BUTTON_LEFT:
         song_pan--;
         break;
      case BUTTON_RIGHT:
         song_pan++;
         break;
      case BUTTON_L:
         song_pan-=16;
         break;
      case BUTTON_R:
         song_pan+=16;
         break;
      case BUTTON_A:
         MusHandleSetPan(last_song_handle, song_pan);
         break;
   }
   if (song_pan<0)
      song_pan=0;
   else if (song_pan>0x100)
      song_pan=0x100;
}

void ProcessSongTempo(int type)
{
  switch (type)
  {
      case BUTTON_LEFT:
         song_tempo--;
         break;
      case BUTTON_RIGHT:
         song_tempo++;
         break;
      case BUTTON_L:
         song_tempo-=16;
         break;
      case BUTTON_R:
         song_tempo+=16;
         break;
      case BUTTON_A:
         MusHandleSetTempo(last_song_handle, song_tempo);
         break;
   }
   if (song_tempo<0)
      song_tempo=0;
   else if (song_tempo>0x100)
      song_tempo=0x100;
}

void ProcessEffect(int type)
{
   switch (type)
   {
      case BUTTON_A:
         last_effect_handle=MusStartEffect2(effect_number, effect_vol, effect_pan, 0, -1);
         break;
      case BUTTON_LEFT:
         if (effect_number)
            effect_number--;
         break;
      case BUTTON_RIGHT:
         if (effect_number<EFFECT_COUNT-1)
            effect_number++;
         break;
   }
}

void ProcessEffectFreq(int type)
{
   switch (type)
   {
      case BUTTON_LEFT:
         effect_freq_offset-=0.1;
         break;
      case BUTTON_RIGHT:
         effect_freq_offset+=0.1;
         break;
      case BUTTON_L:
         effect_freq_offset-=1.0;
         break;
      case BUTTON_R:
         effect_freq_offset+=1.0;
         break;
      case BUTTON_A:
         MusHandleSetFreqOffset(last_effect_handle, effect_freq_offset);
         break;
   }
   if (effect_freq_offset<-6.0)
      effect_freq_offset = -6.0;
   else if (effect_freq_offset>6.0)
      effect_freq_offset = 6.0;

}

void ProcessEffectVol(int type)
{
   switch (type)
   {
      case BUTTON_LEFT:
         effect_vol--;
         break;
      case BUTTON_RIGHT:
         effect_vol++;
         break;
      case BUTTON_L:
         effect_vol-=16;
         break;
      case BUTTON_R:
         effect_vol+=16;
         break;
   }
   if (effect_vol<0)
      effect_vol=0;
   else if (effect_vol>0x100)
      effect_vol=0x100;
}

void ProcessEffectPan(int type)
{
   switch (type)
   {
      case BUTTON_LEFT:
         effect_pan--;
         break;
      case BUTTON_RIGHT:
         effect_pan++;
         break;
      case BUTTON_L:
         effect_pan-=16;
         break;
      case BUTTON_R:
         effect_pan+=16;
         break;
   }
   if (effect_pan<0)
      effect_pan=0;
   else if (effect_pan>0x100)
      effect_pan=0x100;
}

void ProcessStopAllSongs(int type)
{
   if (type==BUTTON_A)
      MusStop(MUSFLAG_SONGS, 0);
}

void ProcessStopAllEffects(int type)
{
   if (type==BUTTON_A)
      MusStop(MUSFLAG_EFFECTS, 0);
}

void ProcessStopAll(int type)
{
   if (type==BUTTON_A)
      MusStop(MUSFLAG_SONGS|MUSFLAG_EFFECTS, 0);
}

void ProcessStopSong(int type)
{
   switch (type)
   {
      case BUTTON_A:
         last_stop_song  = MusHandleStop(last_song_handle, song_fade_time);
         song_fade_time2 = song_fade_time;
         break;
      case BUTTON_LEFT:
         song_fade_time--;
         break;
      case BUTTON_RIGHT:
         song_fade_time++;
         break;
      case BUTTON_L:
         song_fade_time-=10;
         break;
      case BUTTON_R:
         song_fade_time+=10;
      break;
   }
   if (song_fade_time<0)
      song_fade_time=0;
   else if (song_fade_time>1000)
      song_fade_time=1000;
}

void ProcessStopEffect(int type)
{
   switch (type)
   {
      case BUTTON_A:
         last_stop_effect = MusHandleStop(last_effect_handle, effect_fade_time);
         effect_fade_time2 = effect_fade_time;
         break;
      case BUTTON_LEFT:
         effect_fade_time--;
         break;
      case BUTTON_RIGHT:
         effect_fade_time++;
         break;
      case BUTTON_L:
         effect_fade_time-=10;
         break;
      case BUTTON_R:
         effect_fade_time+=10;
         break;
   }
   if (effect_fade_time<0)
      effect_fade_time=0;
   else if (effect_fade_time>1000)
      effect_fade_time=1000;
}

void ProcessSongsVolume(int type)
{
   switch (type)
   {
      case BUTTON_LEFT:
         song_volume-=256;
         break;
      case BUTTON_RIGHT:
         song_volume+=256;
         break;
      case BUTTON_L:
         song_volume-=4*256;
         break;
      case BUTTON_R:
         song_volume+=4*256;
         break;
      default:
         return;
   }
   if (song_volume<0)
      song_volume=0;
   else if (song_volume>0x7fff)
      song_volume=0x7fff;
   MusSetMasterVolume(MUSFLAG_SONGS,song_volume);
}

void ProcessEffectsVolume(int type)
{
   switch (type)
   {
      case BUTTON_LEFT:
         effect_volume-=256;
         break;
      case BUTTON_RIGHT:
         effect_volume+=256;
         break;
      case BUTTON_L:
         effect_volume-=4*256;
         break;
      case BUTTON_R:
         effect_volume+=4*256;
         break;
      default:
         return;
   }
   if (effect_volume<0)
      effect_volume=0;
   else if (effect_volume>0x7fff)
      effect_volume=0x7fff;
   MusSetMasterVolume(MUSFLAG_EFFECTS, effect_volume);
}

/* end of file */
