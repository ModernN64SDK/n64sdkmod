------------------------------------------------------------------
N64 NuSystem Sample Game
SNAKE TAIL HACK
------------------------------------------------------------------

Explanation of files and functions

font.c	Display of letters and characters on the screen
* Display letters of 8 x 8, 4 x 8 on the screen.

--gamemain.c	Main process of a game--
void makegameDL(void)           Creates a display list and  
				activates the graphic task
void updateGame(void)           Processes the progress in the  
				main part of the game
void InitSnake(void)            Initializes various parameters
void InitShotTail(void)         Initializes the tail to be shot  
				out
void EntryShotTail(int, int)    Makes an entry of the tail for  
				the shot
void EraseShotTail(int)         Erases an entry of the tail for  
				the shot
void SlideTail(int)             Rotates the the player's own tail
void ShotTailMove(void)         Moves the tail that was shot out
void DrawHead(Dynamic*, int)    Displays the player's own head
void DrawTail(Dynamic*, int)    Displays the player's own tail
void RecordHeadPos(int)         Records the track of the head in  
				the buffer
void UpdateTailPos(int)         The tail follows the track of  
				the head
void DrawShotTail(Dynamic*)     Displays the tail that was shot  
				out
void ChgHeadDir(int, int, int)  Turn the head toward the Control  
				Stick (by 90 degrees)
void ReadMovie(int)             Read the movie texture from ROM  
				to RAM.
void DrawInfo(void)             Displays the screen information
int CheckTailNum(int)           Counts the number of tails of  
				each snake upon 
				decision that the game is over  
void InitGame(void)             Initializes various items of the  
				main game


gametitle.c	Processing the titles of the games
void maketitleDL(void)          Creates a display list and  
				activates the graphic task
void updateTitle(void)          Processes the progress of the  
				titles
void DrawTitle(void)            Draws the title screen


gameover.c	Processing the game over
void makegameoverDL(void)       Creates a display list and  
				activates the graphic task
void updateGameOver(void)       Processes the game over progress
void DrawGameOver(void)         Draws the game over screen


gamepakerr.c	Processing the errors of the Controller Pak (in  
		case of connection error)
void makepakerrDL(void)         Creates a display list and  
				activates the graphic task
void updatePakErr(void)         Processes the progress of the  
				Controller Pak error
void DrawErr(void)              Draws the Pak error screen


gamepakrwerr.c	Processing the errors of the Controller Pak (in  
		case of writing/reading errors)
void makepakrwerrDL(void)       Creates a display list and  
				activates the graphic task            
void updateRwPakErr(void)       Processes the progress of Pak  
				errors
void DrawRwErr(void)            Draws the Pak error screen


gfxinit.c	Display list data of many settings for N64

graphic.c	General routine for graphics
void gfxRCPInit(void)           Initializes RCP
void gfxClearCfb(void)          Clears the frame buffer/Z buffer


hitchk.c	 Collision detection in the game
void Snake2Wall(int)            Perform the collision detection  
				of the head and the walls
void Snake2Obj(int)             Perform the collision detection  
				of the head and the map object
void Snake2Enemy(int)           Perform the collision detection  
				of the head and the enemies
int ShotTail2Wall(float, float) Perform the collision detection  
				of the tail which was shot and  
				the walls
int ShotTail2Obj(float, float)  Perform the collision detection  
				of the tail which was shot and  
				the obstacle
void Snake2ShotTail(int)        Perform the collision detection  
				of the player's own tail and  
				the tail which was shot
void Snake2EnemyTail(int)       Perform the collision detection  
				of the player's own tail and the  
				enemy's tail

main.c		Main
void mainproc(void)             Main processing
void gametitle(int)             Title processing
void gamemain(int)              Game main processing
void gameover(int)              Game over processing
void pakerr(int)                Processing of Controller Pak  
				errors
void pakrwerr(int)              Processing of Controller Pak  
				reading/writing errors
void SavePadData(void)          Records the Controller Pad data  
				in the buffer
void SaveContPak(void)          Write the Controller Pad data in  
				the Controller Pak
void LoadContPak(void)          Read the Controller Pad data  
				from the Controller Pak
void ReplayPadData(void)        Replay with the saved Controller  
				Pad data
void ContPakOpenAll(void)       Opens the Controller Pak
int ChkPakStat(void)            Checks the status of the opened  
				Controller Pak
void NoteOpenAll(void)          Opens the game note of the  
				opened Controller Pak


map.c		Game maps
void DrawMap(Dynamic*)          Draws the base of the map
void DrawMapSaku(Dynamic*)      Draws the fence of the map
void DrawMapObj(Dynamic*)       Draws the object on the map
void DrawMovie( Dynamic*, unsigned short *)  Draws the movie of  
					     the center of the  
					     map


math.c		Calculation (arc tangent)
float atan2bodyf(float,float)   The calculating part of the  
				actual arc tangent
float Atan2f(float, float)      The calling part of atan2bodyf()
s8 Zettai(s8)                   Returns the absolute value


view.c		Setting the image point matrix
void SetViewMtx(Dynamic*)		Sets the  image point  
					matrix (when normal)
void SetViewMtxGameOver(Dynamic*)	Sets the image point  
					matrix (when game is  
					over)
