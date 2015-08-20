//
//  Display.h
//  TEST


#ifndef TEST_Display_h
#define TEST_Display_h

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

extern int SCREEN_WIDTH_GLOBAL;
extern int SCREEN_HEIGHT_GLOBAL;

#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   700


//#define FULLSCREEN_YN SDL_WINDOW_FULLSCREEN
#define FULLSCREEN_YN 1

/* 
 *Tower monitor dimensions 
 */
#define TOWER_MONITOR_HEIGHT (SCREEN_HEIGHT_GLOBAL / 3.5)
#define TOWER_MONITOR_WIDTH (SCREEN_WIDTH_GLOBAL / 3.5)
#define TOWER_MONITOR_X (SCREEN_WIDTH_GLOBAL - TOWER_MONITOR_WIDTH)
#define TOWER_MONITOR_Y (SCREEN_HEIGHT_GLOBAL - TOWER_MONITOR_HEIGHT)
#define TOWER_TEXT_BORDER_X 25
#define TOWER_TEXT_BORDER_Y 25

/* 
 * Stats monitor dimensions 
 */
#define STATS_BAR_X 0
#define STATS_BAR_Y 0
#define STATS_BAR_WIDTH (SCREEN_WIDTH)
#define STATS_BAR_HEIGHT 30
#define STATS_BAR_BORDER_X 300
#define STATS_BAR_BORDER_Y 10

/* 
 *Action queue dimensions 
 */
#define ACTION_QUEUE_WIDTH (SCREEN_WIDTH_GLOBAL / 5)
#define ACTION_QUEUE_HEIGHT (SCREEN_HEIGHT_GLOBAL / 5)
#define ACTION_QUEUE_X (SCREEN_WIDTH_GLOBAL / 2 - 120)
#define ACTION_QUEUE_Y (SCREEN_HEIGHT_GLOBAL - TOWER_MONITOR_HEIGHT)
#define ACTION_QUEUE_BORDER_X 40
#define ACTION_QUEUE_BORDER_Y 40
#define ACTION_QUEUE_NUM_X	((ACTION_QUEUE_X + ACTION_QUEUE_WIDTH + ACTION_QUEUE_BORDER_X))
#define ACTION_QUEUE_NUM_Y	(ACTION_QUEUE_Y + 10)

/*
 *Terminal window dimensions 
 */
#define TERMINAL_WINDOW_HEIGHT (SCREEN_HEIGHT_GLOBAL / 3.5)
#define TERMINAL_WINDOW_WIDTH (SCREEN_WIDTH_GLOBAL / 3.5)
#define TERMINAL_WINDOW_X (0)
#define TERMINAL_WINDOW_Y (SCREEN_HEIGHT_GLOBAL - TERMINAL_WINDOW_HEIGHT)

/*
 *Map Dimensions
*/
#define MAP_X 	0	
#define MAP_Y	0
#define MAP_WIDTH SCREEN_WIDTH_GLOBAL
#define MAP_HEIGHT	(SCREEN_HEIGHT_GLOBAL - TOWER_MONITOR_HEIGHT)

/*
 *Tower Position Dimensions 
 */
#define MAX_TOWER_X	1000
#define MAX_TOWER_Y	1000

/*
 *Saturation maximum for AOE explosions
 */
#define MAX_AOE_SATURATION 200

#include <stdlib.h>
#include <string.h>


typedef struct display *Display;

enum font_types{solid, blended, blended_wrapped};

typedef enum	{
	towerPosA = 1,
	towerPosB = 2,
	towerPosC = 3,
	towerPosD = 4,
	towerPosE = 5,
	towerPosF = 6,
	towerPosG = 7,
} tPosIcon;

typedef enum gameState  {
    menu,
    tutorial,
    hardLevel,
	easyLevel,
	practise

} gameState;

Display init_SDL();

Display getDisplayPointer(Display d);

void endFrame();
void shutSDL();


void draw_filled_range(int cx, int cy, int r);
void drawRect(int x, int y, int red, int blue, int max_width, int max_height, int current, int total);
void drawRectVertical(int x, int y, int red, int blue, int max_width, int max_height, int current, int total);


/*
 * tower
 */
void drawTower(int x, int y, int w, int h, int type, int range, int frames, int anim_speed, int pic_width, int pic_height);
void drawTowerPosition(int x, int y, int w, int h,tPosIcon tIcon);

/*
 * projectiles
 */
void drawBullet(int x, int y, int w, int h, int bulletType);
void drawLaser(Display d, int X_from, int Y_from, int X_target, int Y_target, int laserType, int drawCount, int maxDrawCount);

/*
 * AOE
 */
void drawAOE(int damageType, int x, int y, int range, int currentCount, int maxCount);

/*
 * enemy 
 */
void getBackgroundDimensions(int *w, int *h);
void drawEnemy(int x, int y, int w, int h, int pic_width, int pic_height, int type, int frames, int anim_speed);

void presentAnimation();
void presentFirewall();
void drawBackground();
void getWindowSize(int *w, int *h);

void drawKillAll();


#include <stdio.h>
#include "Information_Window.h"
#include "gameProperties.h"

void displayTowerMonitor();
void displayStatsBar();
void displayActionQueueMonitor();
void updateTowerMonitor(char *outputString);
void updateStatsBar(char *outputString);
void updateActionQueueMonitor(char *outputString);
char *strdup2(char * s);
void updateTerminalWindow(char *outputString);
void updateTowerInformation(int towerX, int towerY, char *string, int towerID);

void pause_screen(int *pause, int *restart);
int terminal_window(char *pass, char *clear, int *pause,int restart);
void menu_screen (gameState *state);
void testTerminalWindowInput();
void testtermwin();
char *test_string_1(char *pass2);
char *test_string_2(char *clear);

int final_screen();
int getBackgroundWidth();
int getBackgroundHeight();


#endif
