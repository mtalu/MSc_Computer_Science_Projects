//
//  Display.c
//
//  All functions directly related to displaying the game
//
//

#include "../includes/Display.h"
#include "../includes/parser.h"
#include <stdbool.h>
#include "../includes/sput.h"
#include"../includes/Sound.h"

int SCREEN_WIDTH_GLOBAL;
int SCREEN_HEIGHT_GLOBAL;

#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

struct display {
    /* 
     *main objects 
     */
    SDL_Window  *window;
    SDL_Surface *surface;
    SDL_Renderer *renderer;
    SDL_Rect    srcRect;
    SDL_Rect    rect;
    SDL_Event event;
    TTF_Font *font;
    TTF_Font *playerScoreFont;
	    
    SDL_Texture *statsBarTexture;
    SDL_Texture *towerInfoTexture;
    
    SDL_Color white;
    SDL_Color red;
    SDL_Color green;
    
    SDL_Texture *map;
    
    SDL_Texture *towerMonitorTexture;

	SDL_Texture *startBackgroundTexture;
    SDL_Texture *finalBackgroundTexture;
	SDL_Texture *winBackgroundTexture;


	SDL_Texture *easyButton;
	SDL_Texture *practiseButton;
	SDL_Texture *hardButton;
    SDL_Texture *reStartButton;
   	SDL_Texture *returnButton; 
	SDL_Texture *tutorialButton;

    SDL_Texture *newtexture;
	SDL_Texture *terminalWindowTexture;

    SDL_Texture *actionQueueTexture;

    SDL_Texture *towerTexture[3];
    SDL_Texture *towerPositionTexture[26];

    SDL_Texture *bulletTexture[3];

    SDL_Texture *enemyTexture[5];
    
    SDL_Texture *circ1_Texture[2];
    SDL_Texture *circ2_Texture[2];
    
    SDL_Texture *firewall;
};

/** Functions prototypes for functions only used internally*/

void initTTF(void);
SDL_Surface *getInfoWindowTextSurface(char *outputString);
TTF_Font *getInfoWindowFont(TTF_Font *font);
void crash(char *message);
void getWindowSize(int *w, int *h);
void init_pic(SDL_Texture **texture, char *pic_name);
void check_load_images(SDL_Surface *surface, char *pic_name);
void draw_filled_range(int cx, int cy, int r);
void presentCircuit(SDL_Texture *text[2], int x,int y,int w, int h, int frames, int pic_width, int pic_height, int anim_speed);
void animateAnyPic(int x, int y, int w, int h, int pic_width, int pic_height, int frames, int anim_speed, SDL_Texture *texture);
void displayMonitor(int x, int y, int w, int h, SDL_Texture *texture);
void display_text(int x, int y, char *string, int text, TTF_Font *font, SDL_Color colour);

/*
 * Initialize display 
 */
Display init_SDL(){
    
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) crash("SDL_Init()");
    if(TTF_Init() != 0) crash("TTF_Init()");
    if(IMG_Init(0) != 0) crash("IMG_Init()");
    
    Display d = (Display)malloc(sizeof(struct display));
    getDisplayPointer(d);
    
    d->window = SDL_CreateWindow("TOWER DEFENSE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_MAXIMIZED);
    if (d->window == NULL) {
        crash("Cannot create window\n");
    }
    d->renderer = SDL_CreateRenderer(d->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (d->renderer == NULL) {
        crash("Cannot create renderer\n");
    }
    getWindowSize(&SCREEN_WIDTH_GLOBAL,&SCREEN_HEIGHT_GLOBAL);
  
    d->font= TTF_OpenFont("OpenSans-Regular.ttf", 10);
    d->playerScoreFont = TTF_OpenFont("final_screen.ttf", 40);
    if(d->font== NULL) crash("TTF_(OpenFont)");
    
    d->white = (SDL_Color) {255, 255, 255};
    d->red = (SDL_Color) {255, 0, 0};
    d->green = (SDL_Color) {124, 252, 0};
    
    /* 
     *improves quality of font
     */
    TTF_SetFontHinting(d->font, TTF_HINTING_LIGHT);
    putenv("SDL_VIDEODRIVER=dga");
    
    /* 
     *inititalize pictures (load picture to the texture)
     */
    init_pic(&d->firewall, "Images/firewall.png");
    init_pic(&d->reStartButton, "Images/RestartButton.png");
    init_pic(&d->finalBackgroundTexture, "Images/final_screen.png");
	init_pic(&d->winBackgroundTexture, "Images/win_screen.png");
    init_pic(&d->towerMonitorTexture, "Images/info_monitor.png");
    init_pic(&d->actionQueueTexture, "Images/action_queue-monitor.png");
    init_pic(&d->statsBarTexture, "Images/blackBar.png");
    init_pic(&d->towerInfoTexture, "Images/towerInfoBackground.png");
    init_pic(&d->startBackgroundTexture, "Images/anistrip_menu.png");
    init_pic(&d->easyButton, "Images/easyLevel.png");
    init_pic(&d->practiseButton, "Images/practiseMode.png");
    init_pic(&d->hardButton, "Images/HardLevel.png");
	init_pic(&d->returnButton,"Images/returnButton.png");
	init_pic(&d->tutorialButton,"Images/tutorialButton.png");
    init_pic(&d->terminalWindowTexture, "Images/terminalwindow.png");
    init_pic(&d->map, "Images/map1.png");
    init_pic(&d->towerPositionTexture[0], "Images/TowerLocationsA.png");
    init_pic(&d->towerPositionTexture[1], "Images/TowerLocationsB.png");
    init_pic(&d->towerPositionTexture[2], "Images/TowerLocationsC.png");
    init_pic(&d->towerPositionTexture[3], "Images/TowerLocationsD.png");
    init_pic(&d->towerPositionTexture[4], "Images/TowerLocationsE.png");
    init_pic(&d->towerPositionTexture[5], "Images/TowerLocationsF.png");
    init_pic(&d->towerPositionTexture[6], "Images/TowerLocationsG.png");
    init_pic(&d->towerPositionTexture[7], "Images/TowerLocationsH.png");
    init_pic(&d->towerPositionTexture[8], "Images/TowerLocationsI.png");
    init_pic(&d->towerPositionTexture[9], "Images/TowerLocationsJ.png");
    init_pic(&d->towerPositionTexture[10], "Images/TowerLocationsK.png");
    init_pic(&d->towerPositionTexture[11], "Images/TowerLocationsL.png");
    init_pic(&d->towerPositionTexture[12], "Images/TowerLocationsM.png");
    init_pic(&d->towerPositionTexture[13], "Images/TowerLocationsN.png");
    init_pic(&d->towerPositionTexture[14], "Images/TowerLocationsO.png");
    init_pic(&d->towerPositionTexture[15], "Images/TowerLocationsP.png");
    init_pic(&d->towerPositionTexture[16], "Images/TowerLocationsQ.png");
    init_pic(&d->towerPositionTexture[17], "Images/TowerLocationsR.png");
    init_pic(&d->towerPositionTexture[18], "Images/TowerLocationsS.png");
    init_pic(&d->towerPositionTexture[19], "Images/TowerLocationsT.png");
    init_pic(&d->towerPositionTexture[20], "Images/TowerLocationsU.png");
    init_pic(&d->towerPositionTexture[21], "Images/TowerLocationsV.png");
    init_pic(&d->towerPositionTexture[22], "Images/TowerLocationsW.png");
    init_pic(&d->towerPositionTexture[23], "Images/TowerLocationsX.png");
    init_pic(&d->towerPositionTexture[24], "Images/TowerLocationsY.png");
    init_pic(&d->towerPositionTexture[25], "Images/TowerLocationsZ.png");
    init_pic(&d->enemyTexture[0], "Images/sdl2-spritesheet-actual.png");
    init_pic(&d->enemyTexture[1], "Images/ENEMY_INT_noClip.png");
    init_pic(&d->enemyTexture[2], "Images/ENEMY_INT_noClip.png");
    init_pic(&d->enemyTexture[3], "Images/ENEMY_CHAR.png");
    init_pic(&d->enemyTexture[4], "Images/ENEMY_CHAR.png");
    init_pic(&d->towerTexture[0], "Images/tower.png");
    init_pic(&d->towerTexture[1], "Images/intTower.png");
    init_pic(&d->towerTexture[2], "Images/charTower.png");
    init_pic(&d->circ1_Texture[0], "Images/circ1_dark.png");
    init_pic(&d->circ1_Texture[1], "Images/circ1_light.png");
    init_pic(&d->circ2_Texture[0], "Images/circ3_dark.png");
    init_pic(&d->circ2_Texture[1], "Images/circ3_light.png");
    init_pic(&d->bulletTexture[0], "Images/greenBullet2.png");
    init_pic(&d->bulletTexture[1], "Images/redBullet2.png");

    return d;
}

/*
 *animate any pic 
 */
void animateAnyPic(int x, int y, int w, int h, int pic_width, int pic_height, int frames, int anim_speed, SDL_Texture *texture){
    Display d = getDisplayPointer(NULL);
    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite = (ticks / anim_speed) % frames;
    d->srcRect = (SDL_Rect){sprite * (pic_width/frames), 0, (pic_width/frames), pic_height};
    d->rect = (SDL_Rect) {x, y, w, h};
    /*
     *create animation by putting part of a spritesheet(image) into destination rect
     */
    SDL_RenderCopy(d->renderer, texture, &d->srcRect, &d->rect);
}


/*
 * Tower and enemy graphics functions
 */
void getBackgroundDimensions(int *w, int *h){
    Display d =getDisplayPointer(NULL);
    SDL_QueryTexture(d->map, NULL, NULL, w, h);
}
/*
 * Draw kill all ability
 */
void drawKillAll(){
    Display d = getDisplayPointer(NULL);
    d->rect = (SDL_Rect) {0,0, SCREEN_WIDTH_GLOBAL, SCREEN_HEIGHT_GLOBAL};
    SDL_SetRenderDrawBlendMode(d->renderer, SDL_BLENDMODE_BLEND);
    int saturation = 0;
    while (saturation < 255) {
        SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, saturation += 8);
        SDL_RenderFillRect(d->renderer, &d->rect);
        SDL_RenderPresent(d->renderer);
    }
}

/*
 * call fucntion in the while loop to present all the animations
 */
void presentAnimation(){
    int w, h;
    getWindowSize(&w, &h);
    Display d = getDisplayPointer(NULL);
    presentCircuit(d->circ1_Texture,(double)w/1.35, (double)h/3, (double)w/50, (double)h/10, 6,385, 324, 600);
    presentCircuit(d->circ2_Texture,(double)w/5, (double)h/10, (double)w/45, (double)h/13, 7,386, 195, 750);
    presentCircuit(d->circ2_Texture,(double)w/3, (double)h/1.7, (double)w/45, (double)h/13, 7,386, 195, 550);
}

void presentFirewall() {
    int w, h;
    Display d = getDisplayPointer(NULL);
    getWindowSize(&w, &h);
    animateAnyPic((double)w/1.57, (double)h/3.8, (double)w/10.9, (double)h/13, 1036, 130, 4, 200, d->firewall);
}

/*
 * draw background image
 */
void drawBackground(){
    Display d = getDisplayPointer(NULL);
    SDL_RenderCopy(d->renderer, d->map, NULL, NULL);
}

/*
 * Draw tower position
 */
void drawTowerPosition(int x, int y, int w, int h,tPosIcon tIcon){
    Display d =getDisplayPointer(NULL);
    d->rect = (SDL_Rect) {x, y, w, h};
    SDL_RenderCopy(d->renderer, d->towerPositionTexture[tIcon-1], NULL, &d->rect);
}

/*
 * present any animation with one staitc image at the back and one above it
 */
void presentCircuit(SDL_Texture *text[2], int x,int y,int w, int h, int frames, int pic_width, int pic_height, int anim_speed) {
    Display d = getDisplayPointer(NULL);
    d->rect= (SDL_Rect) {x, y , w * frames, h};
    SDL_RenderCopy(d->renderer, text[0], NULL, &d->rect);
    
    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite = (ticks / anim_speed) % frames;
    /*
     *srcrect runs through the actual picture size
     */
    d->srcRect = (SDL_Rect){ sprite * (pic_width/frames), 0, (pic_width/frames), pic_height};
    
    /*
     *dstrect size could be decided by the user
     */
    d->rect = (SDL_Rect) {sprite * w + x, y, w, h};
    SDL_RenderCopy(d->renderer, text[1], &d->srcRect, &d->rect);
}

/*
 * Initialize images and check whether they were loaded successfull
 */
void init_pic(SDL_Texture **texture, char *pic_name){
    Display d = getDisplayPointer(NULL);
    d->surface = IMG_Load(pic_name);
    check_load_images(d->surface, pic_name);
    *texture = SDL_CreateTextureFromSurface(d->renderer, d->surface);
    SDL_FreeSurface(d->surface);
}

/*
 * Get pointer to the Display object
 */
Display getDisplayPointer(Display d){
	static Display disp;
	if(d != NULL) {
		disp = d;
	}
	return disp;
}

/*
 * Prints last SDL error message to stderr, along withb message included in first parameter.
 */
void crash(char *message) {
    fprintf(stderr, "%s: %s\n", message, SDL_GetError());
    SDL_Quit();
}

/*
 * fill variables with the width and height values of the screen
 */
void getWindowSize(int *w, int *h){
    Display d = getDisplayPointer(NULL);
    SDL_GetWindowSize(d->window, w, h);
}


/*draw damage line from X & Y to target X & Y*/
void drawLaser(Display d, int X_from, int Y_from, int X_target, int Y_target, int laserType, int drawCount, int maxDrawCount){
      // choose laser colour depending on type
      int sat = 250 - (int) (((double) drawCount/ (double) maxDrawCount) * 250.0);
      SDL_SetRenderDrawBlendMode(d->renderer, SDL_BLENDMODE_BLEND);
      if(laserType == INT_TYPE) {
            SDL_SetRenderDrawColor(d->renderer, 0, 252, 0, sat);
        } else {
            SDL_SetRenderDrawColor(d->renderer, 252, 0, 0, sat);
        }
      for(int i = -2; i < 3; i++) {
          for(int j = -1; j < 2; j++) {
              SDL_RenderDrawLine(d->renderer, X_from+i, Y_from+j, X_target, Y_target);
          }
      }
}


void drawLine(Display d, int X_from, int Y_from, int X_target, int Y_target, int laserType){
      // choose laser colour depending on type
      int sat = 5;
      int adjust = -5;
      SDL_SetRenderDrawBlendMode(d->renderer, SDL_BLENDMODE_BLEND);
    for(int i = 0; i < 10; i++) {
        
        if(laserType == INT_TYPE) {
            SDL_SetRenderDrawColor(d->renderer, 0, 252, 0, sat);
        } else {
            SDL_SetRenderDrawColor(d->renderer, 252, 0, 0, sat);
        }
        SDL_RenderDrawLine(d->renderer, X_from+adjust, Y_from-adjust, X_target, Y_target);
        
        adjust++;
        if(i < 5) {
            sat+=25;
        } else {
            sat-=25;
        }
    }
}


/*
 * draw an enemy at x and y coor with the health bar above it
 */
void drawEnemy(int x, int y, int w, int h, int pic_width, int pic_height, int type, int frames, int anim_speed){
    Display d = getDisplayPointer(NULL);
    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite = (ticks / anim_speed) % frames;
    d->srcRect = (SDL_Rect){sprite * (pic_width/frames), 0, (pic_width/frames), pic_height};
    d->rect = (SDL_Rect) {x, y, w, h};
    /*create animation by putting part of a spritesheet(image) into destination rect*/
    SDL_RenderCopy(d->renderer, d->enemyTexture[type], &d->srcRect, &d->rect);
}

/*
 *  draw CPU rect
 */
void drawRect(int x, int y, int red, int blue, int max_width, int max_height, int current, int total){
    Display d = getDisplayPointer(NULL);
    SDL_SetRenderDrawBlendMode(d->renderer, SDL_BLENDMODE_NONE);
    /*presenting and manipulating color and width of the health bar*/
    double color = (255*((double)current/total));
    SDL_SetRenderDrawColor(d->renderer, red, color, blue, 0);
    double width = ((double)(current * max_width/(double)total));
    d->rect = (SDL_Rect) {x, y, (double)width, max_height};
    SDL_RenderFillRect(d->renderer, &d->rect);
}

void drawRectVertical(int x, int y, int red, int blue, int max_width, int max_height, int current, int total){
    Display d = getDisplayPointer(NULL);
    SDL_SetRenderDrawBlendMode(d->renderer, SDL_BLENDMODE_NONE);
    /*presenting and manipulating color and width of the health bar*/
    double color = (255*((double)current/total));
    SDL_SetRenderDrawColor(d->renderer, red, color, blue, 0);
    double height = ((double)(current * max_height/(double)total));
    d->rect = (SDL_Rect) {x, y + max_height-height, max_width, (double)height};
    SDL_RenderFillRect(d->renderer, &d->rect);
}



/*
 * Draw range with transparency
 */
void draw_filled_range(int cx, int cy, int r)
{
    Display d = getDisplayPointer(NULL);
    Uint32 ticks = SDL_GetTicks();
    Uint32 clock = (ticks / 150)% 20;
    /* 
     *mx saturation is a max extreme to ensure saturation does not get bigger than pre-set value
     */
    float max_satur = 30;
    /* 
     *set initial saturation
     */
    float initial_saturation = 30;
    /*
     *set variable saturation to equal initial saturation
     */
    static float saturation = 30;

    if (clock < 10 && saturation < max_satur + initial_saturation) {
        saturation += 0.5;
    }
    else if (clock >= 10 && saturation > initial_saturation){
        saturation -= 0.5;
    }
    SDL_SetRenderDrawBlendMode(d->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(d->renderer, 255, 50, 0, saturation);
    for (double dy = 1; dy <= r; dy += 1.0) {
        double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        SDL_RenderDrawLine(d->renderer, cx-dx, cy+r-dy, cx+dx, cy+r-dy);
        SDL_RenderDrawLine(d->renderer, cx-dx, cy-r+dy, cx+dx, cy-r+dy);
    }
}

/*
 * draws the tower at x and y coor
 */
void drawTower(int x, int y, int w, int h, int type, int range, int frames, int anim_speed, int pic_width, int pic_height){
    Display d = getDisplayPointer(NULL);
    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite = (ticks / anim_speed) % frames;
    d->srcRect = (SDL_Rect){sprite * (pic_width/frames), 0, (pic_width/frames), pic_height};
    d->rect = (SDL_Rect) {x, y, w, h};
    /* 
     *create animation by putting part of a spritesheet(image) into destination rect
     */
    SDL_RenderCopy(d->renderer, d->towerTexture[type], &d->srcRect, &d->rect);
    draw_filled_range(x + (double)w/2, y + (double)h/2, range);
}


/*
 * draws projectile
 */
void drawBullet(int x, int y, int w, int h, int bulletType) {
    Display d = getDisplayPointer(NULL);
    d->rect = (SDL_Rect) {x, y, w, h};
      /*
       *draw red or green bullet depending on type
       */
    if(bulletType == INT_TYPE) {
        SDL_RenderCopy(d->renderer, d->bulletTexture[0], NULL, &d->rect);
    } else {
        SDL_RenderCopy(d->renderer, d->bulletTexture[1], NULL, &d->rect);
    }
}

/*
 * draws AOE range 
 */
void drawAOE(int damageType, int x, int y, int range, int currentCount, int maxCount) {
    Display d = getDisplayPointer(NULL);
    
    float saturation = MAX_AOE_SATURATION - ( ((float)currentCount/(float)maxCount) * MAX_AOE_SATURATION);
    SDL_SetRenderDrawBlendMode(d->renderer, SDL_BLENDMODE_BLEND);
    
    if(damageType == INT_TYPE) {
        SDL_SetRenderDrawColor(d->renderer, 0, 252, 0, saturation);
    } else {
        SDL_SetRenderDrawColor(d->renderer, 252, 0, 0, saturation);
    }
    
    for (double dy = 1; dy <= range; dy += 1.0) {
        double dx = floor(sqrt((2.0 * range * dy) - (dy * dy)));
        SDL_RenderDrawLine(d->renderer, x-dx, y+range-dy, x+dx, y+range-dy);
        SDL_RenderDrawLine(d->renderer, x-dx, y-range+dy, x+dx, y-range+dy);
    }
}


/*
 * peresent everything renderer has to draw
 */
void endFrame() {
    Display d = getDisplayPointer(NULL);
    SDL_RenderPresent(d->renderer);
    SDL_Delay(20);
}

/*
 * check whether image was loaded successfully 
 */
void check_load_images(SDL_Surface *surface, char *pic_name){
    if(surface == NULL){
        printf("Cannot find %s\n", pic_name);
        SDL_Quit();
        exit(1);
    }
}

/*
 * destroy everything 
 */
void shutSDL() {
    Display d = getDisplayPointer(NULL);
    SDL_DestroyTexture(d->statsBarTexture);
    SDL_DestroyTexture(d->map);
    SDL_DestroyTexture(d->towerMonitorTexture);
    SDL_DestroyTexture(d->startBackgroundTexture);
    SDL_DestroyTexture(d->easyButton);
    SDL_DestroyTexture(d->practiseButton);
    SDL_DestroyTexture(d->hardButton);
    SDL_DestroyTexture(d->newtexture);
    SDL_DestroyTexture(d->terminalWindowTexture);
    SDL_DestroyTexture(d->actionQueueTexture);

    for (int i = 0; i < 26; i++) {
        SDL_DestroyTexture(d->towerPositionTexture[i]);
    }
    SDL_DestroyTexture(d->reStartButton);
    SDL_DestroyTexture(d->startBackgroundTexture);

    SDL_DestroyRenderer(d->renderer);
    SDL_DestroyWindow(d->window);
    SDL_Quit();
    IMG_Quit();
    TTF_CloseFont(d->font);
    TTF_Quit();
}

/*
 * End of tower and enemy graphics functions
 */



/*
 * Information window functions
 */

/*
 * Display monitor
 */
void displayMonitor(int x, int y, int w, int h, SDL_Texture *texture){
    Display d = getDisplayPointer(NULL);
    d->rect= (SDL_Rect){x, y, w, h};
    SDL_RenderCopy(d->renderer, texture, NULL, &d->rect);
}

/*
 * Display empty stats bar at top of screen
 */
void displayStatsBar() {
    Display d = getDisplayPointer(NULL);
    displayMonitor(STATS_BAR_X,  STATS_BAR_Y,  SCREEN_WIDTH_GLOBAL, STATS_BAR_HEIGHT, d->statsBarTexture);
}

/*
 *Display output string in tower monitor
 */
void updateTowerMonitor(char *outputString) {
    Display d = getDisplayPointer(NULL);
    displayMonitor(TOWER_MONITOR_X, TOWER_MONITOR_Y, TOWER_MONITOR_WIDTH, TOWER_MONITOR_HEIGHT, d->towerMonitorTexture);
    display_text(TOWER_MONITOR_X + TOWER_TEXT_BORDER_X,  TOWER_MONITOR_Y + TOWER_TEXT_BORDER_Y, outputString, blended_wrapped, d->font, d->white);
}

/*
 * Display output string in stats monitor
 */
void updateStatsBar(char *outputString) {
    Display d = getDisplayPointer(NULL);
    displayStatsBar();
    display_text(STATS_BAR_X + (double)SCREEN_WIDTH_GLOBAL / 8,  STATS_BAR_Y + 10, outputString, blended, d->font, d->white);
    free(outputString);
}

/*
 *Display output string in action queue monitor 
 */
void updateActionQueueMonitor(char *outputString) {
    Display d = getDisplayPointer(NULL);
    
     displayMonitor(ACTION_QUEUE_X, ACTION_QUEUE_Y, TERMINAL_WINDOW_WIDTH, TERMINAL_WINDOW_HEIGHT, d->actionQueueTexture);
    if(strlen(outputString) > 0) {
        display_text(ACTION_QUEUE_X + ACTION_QUEUE_BORDER_X, ACTION_QUEUE_Y + ACTION_QUEUE_BORDER_Y, outputString, blended_wrapped, d->font, d->green);
    }


}

/*
 * UpdateTowerInformation 
 */
void updateTowerInformation(int towerX, int towerY, char *string, int towerID) {
    Display d = getDisplayPointer(NULL);
    
    int towerWidth = getTowerWidth(towerID);
    
    displayMonitor(towerX - 5, towerY - 20, towerWidth + 10, 30, d->towerInfoTexture);
    display_text(towerX + 5, towerY - 10, string, blended, d->font, d->red);
}

/*
 * Display output string in terminal window
 */
void updateTerminalWindow(char *outputString) {
    Display d = getDisplayPointer(NULL);
    
    if(outputString != NULL) {
        display_text(TERMINAL_WINDOW_X + TOWER_TEXT_BORDER_X, TERMINAL_WINDOW_Y + TOWER_TEXT_BORDER_Y, outputString, blended_wrapped, d->font, d->white);
    }
}

/*
 *End of information window functions
 */


/*
 *Terminal functions
*/
/*
 *terminal_window detects input from SDL and calls display_text
 */
int terminal_window(char *pass, char *clear, int *pause,int restart)
{
    Display d = getDisplayPointer(NULL);
    char *pass2;
    /*
     *Keeps text on screen
     */
    displayMonitor(TERMINAL_WINDOW_X, TERMINAL_WINDOW_Y, TERMINAL_WINDOW_WIDTH, TERMINAL_WINDOW_HEIGHT, d->terminalWindowTexture);
    display_text(TERMINAL_WINDOW_X + (TERMINAL_WINDOW_WIDTH / 10),TERMINAL_WINDOW_Y + (TERMINAL_WINDOW_HEIGHT - (TERMINAL_WINDOW_HEIGHT / 7)), pass,solid, d->font, d->white);    int check = 0;
    SDL_Event *event = &d->event;
    check = (SDL_PollEvent(event));
    if(check != 0)
    {
        switch (event->type)
        {
			/*
             *Detects input and appends to a string
             */
            case SDL_TEXTINPUT:
            {
                strcat(pass, event->text.text);
                display_text(TERMINAL_WINDOW_X + (TERMINAL_WINDOW_WIDTH / 10),TERMINAL_WINDOW_Y + (TERMINAL_WINDOW_HEIGHT - (TERMINAL_WINDOW_HEIGHT / 7)), pass,solid, d->font, d->white);                break;
            }
            case SDL_KEYDOWN:
            {
				/*
				*If return key is pressed, clears string and sends desired input to parser
				*/		
                if(event->key.keysym.sym == SDLK_RETURN)
                {
                    if(strcmp(pass, clear) != 0)
                    {
                         display_text(TERMINAL_WINDOW_X + (TERMINAL_WINDOW_WIDTH / 10),TERMINAL_WINDOW_Y + (TERMINAL_WINDOW_HEIGHT - (TERMINAL_WINDOW_HEIGHT / 7)), pass,solid, d->font, d->white);
                    }
                    
                    pass2 = pass + 2;
                    parse(pass2);
					test_string_1(pass2);
					test_string_2(clear);
                    strcpy(pass, clear);
                }
				/*
                 *If backspace key is pressed, removes end char of string
                 */
				if(event->key.keysym.sym == SDLK_BACKSPACE)
				{
					if(pass[strlen(pass) - 1] != '>')
					{
						pass[strlen(pass) - 1] = '\0';
					}
                    display_text(TERMINAL_WINDOW_X + (TERMINAL_WINDOW_WIDTH / 10),
					TERMINAL_WINDOW_Y + (TERMINAL_WINDOW_HEIGHT - (TERMINAL_WINDOW_HEIGHT / 7)), pass, solid, d->font, d->white);
				}
                switch(d->event.key.keysym.sym)
                {
                	case SDLK_ESCAPE:
					*pause = 1;
                	break;
                }
                break;
            }
        }
    }

	if(restart)	{
		return 1;
	} else {
    	return 0;
	}
}
	
void testTerminalWindowInput()
{
	sput_start_testing();
	sput_set_output_stream(NULL);

	sput_enter_suite("terminal_window(): Testing, terminal window");
	sput_run_test(testtermwin);
	sput_leave_suite();

	sput_finish_testing();
}

void testtermwin()
{
	int restart = 0, pause1 = 0;
	int *pause = &pause1;
	terminal_window(">>", ">>", pause, restart);
	sput_fail_if(*test_string_1(NULL) == '>', "Incorrect string parsing");
	sput_fail_if(strlen(test_string_2(NULL)) > 2, "Clear string failure");
}

char *test_string_1(char *pass2)
{
	static char string[128];
	if(pass2 != NULL)
	{
		strcpy(string, pass2);
	}
	return string;
}
char *test_string_2(char *clear)
{
	static char string[128];
	if(clear != NULL)
	{
		strcpy(string, clear);
	}
	return string;
}

/*
 *display_text builds textures from surfaces and calls renderer to output them to screen
 */
void display_text(int x, int y, char *string, int text, TTF_Font *font, SDL_Color colour)
{
    Display d = getDisplayPointer(NULL);
    
    
    switch (text) {
        case solid:
            d->surface = TTF_RenderText_Solid(font, string, colour);
            break;
        case blended:
            d->surface = TTF_RenderText_Blended(font, string, colour);
            break;
        case blended_wrapped:
            d->surface = TTF_RenderText_Blended_Wrapped(font, string, colour, TOWER_MONITOR_WIDTH - 40);
            break;
    }
    
    d->newtexture = SDL_CreateTextureFromSurface(d->renderer, d->surface);
    
    if(d->newtexture == NULL) {
        printf("Panic\n");
    }
    
    d->rect = (SDL_Rect) {x, y, d->surface->w, d->surface->h};
    
    SDL_RenderCopy(d->renderer, d->newtexture, NULL, &d->rect);
    SDL_FreeSurface(d->surface);
    SDL_DestroyTexture(d->newtexture);
}

void menu_screen(gameState *state)
{
    Display d = getDisplayPointer(NULL);
    //SDL_RenderCopy(d->renderer, d->startBackgroundTexture, NULL, NULL);
    animateAnyPic(0, 0, SCREEN_WIDTH_GLOBAL, SCREEN_HEIGHT_GLOBAL, 7602, 292, 14, 170, d->startBackgroundTexture);

    d->rect = (SDL_Rect) {(SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2), 
			(SCREEN_HEIGHT_GLOBAL/3)*2-(SCREEN_HEIGHT_GLOBAL/6), 
			SCREEN_HEIGHT_GLOBAL/6, 
			SCREEN_HEIGHT_GLOBAL/6};

    SDL_RenderCopy(d->renderer, d->easyButton, NULL, &d->rect);

    d->rect = (SDL_Rect) {(SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2), 
			(SCREEN_HEIGHT_GLOBAL/3)*2, 
			SCREEN_HEIGHT_GLOBAL/6, 
			SCREEN_HEIGHT_GLOBAL/6};

    SDL_RenderCopy(d->renderer, d->hardButton, NULL, &d->rect);

    d->rect = (SDL_Rect) {
            (SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2) - (SCREEN_HEIGHT_GLOBAL/6),  //!x
        ((SCREEN_HEIGHT_GLOBAL/3)*2)+(SCREEN_HEIGHT_GLOBAL/6),      //!y
            SCREEN_HEIGHT_GLOBAL/6,         //!Width
            SCREEN_HEIGHT_GLOBAL/6      //!height
    };

    SDL_RenderCopy(d->renderer, d->tutorialButton, NULL, &d->rect);

    d->rect = (SDL_Rect) {
            (SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2) + (SCREEN_HEIGHT_GLOBAL/6),  //!x
        ((SCREEN_HEIGHT_GLOBAL/3)*2)+(SCREEN_HEIGHT_GLOBAL/6),      //!y
            SCREEN_HEIGHT_GLOBAL/6,         //!Width
            SCREEN_HEIGHT_GLOBAL/6      //!height
    };

    SDL_RenderCopy(d->renderer, d->practiseButton, NULL, &d->rect);

	SDL_RenderPresent(d->renderer);

    int check = 0;
    check = (SDL_PollEvent(&d->event));
    if(check != 0)
    {
		switch(d->event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
			{
				if(d->event.button.x >= (SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2)
                        && d->event.button.x <= (SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2) + SCREEN_WIDTH_GLOBAL/6
                        && d->event.button.y >= (SCREEN_HEIGHT_GLOBAL/3)*2 - (SCREEN_HEIGHT_GLOBAL/6) 
                        &&  d->event.button.y < (SCREEN_HEIGHT_GLOBAL/3)*2)	{
                        if(d->event.button.button == SDL_BUTTON_LEFT){
							//!Start Level
                            *state = easyLevel;
                        }
				}	else if(d->event.button.x >= (SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2) 
						&& d->event.button.x <= (SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2) + SCREEN_WIDTH_GLOBAL/6 
						&& d->event.button.y > (SCREEN_HEIGHT_GLOBAL/3)*2 
						&&  d->event.button.y <= (SCREEN_HEIGHT_GLOBAL/3)*2 + SCREEN_HEIGHT_GLOBAL/6)	{
                        if(d->event.button.button == SDL_BUTTON_LEFT){
                            *state = hardLevel;
                        }
				}	else if(d->event.button.x >= (SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2) - (SCREEN_HEIGHT_GLOBAL/6) 
                             && d->event.button.x <= (SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2) /*+ SCREEN_HEIGHT_GLOBAL/6*/
                             && d->event.button.y >= (SCREEN_HEIGHT_GLOBAL/3)*2 + ((SCREEN_HEIGHT_GLOBAL/6)+5)
                             &&  d->event.button.y <= (SCREEN_HEIGHT_GLOBAL/3)*2 + (2*(SCREEN_HEIGHT_GLOBAL/6)))    {
                        if(d->event.button.button == SDL_BUTTON_LEFT){
							*state = tutorial; 
                        }
				}	else if(d->event.button.x >= (SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2) + (SCREEN_HEIGHT_GLOBAL/6) 
                             && d->event.button.x <= (SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2) + (2*(SCREEN_HEIGHT_GLOBAL/6))
                             && d->event.button.y >= (SCREEN_HEIGHT_GLOBAL/3)*2 + ((SCREEN_HEIGHT_GLOBAL/6)+5)
                             &&  d->event.button.y <= (SCREEN_HEIGHT_GLOBAL/3)*2 + (2*(SCREEN_HEIGHT_GLOBAL/6)))    {
                        if(d->event.button.button == SDL_BUTTON_LEFT){
							*state = practise;
                        }
                } 
			}
			case SDL_KEYDOWN:
			{
				if(d->event.key.keysym.sym == SDLK_ESCAPE)
				{
					SDL_Quit();
					exit(1);
				}
			}
		}
	}
}

void pause_screen(int *pause, int *restart)
{
    Display d = getDisplayPointer(NULL);
    //SDL_RenderCopy(d->renderer, d->startBackgroundTexture, NULL, NULL);
    animateAnyPic(0, 0, SCREEN_WIDTH_GLOBAL, SCREEN_HEIGHT_GLOBAL, 7602, 292, 14, 170, d->startBackgroundTexture);

    d->rect = (SDL_Rect) {(SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2), 
				(SCREEN_HEIGHT_GLOBAL/3)*2, 
				SCREEN_HEIGHT_GLOBAL/6, 
				SCREEN_HEIGHT_GLOBAL/6
	};

    SDL_RenderCopy(d->renderer, d->returnButton, NULL, &d->rect);
    
	d->rect = (SDL_Rect) {
			(SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2),  //!x 
		((SCREEN_HEIGHT_GLOBAL/3)*2)+(SCREEN_HEIGHT_GLOBAL/6), 		//!y
			SCREEN_HEIGHT_GLOBAL/6, 		//!Width
			SCREEN_HEIGHT_GLOBAL/6		//!height
	};

    SDL_RenderCopy(d->renderer, d->reStartButton, NULL, &d->rect);
	SDL_RenderPresent(d->renderer);

    int check = 0;
    check = (SDL_PollEvent(&d->event));
    if(check != 0)
    {
		switch(d->event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
			{
				if(d->event.button.x >= (SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2) 
					&& d->event.button.x <= (SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2) + SCREEN_WIDTH_GLOBAL/6 
					&& d->event.button.y >= (SCREEN_HEIGHT_GLOBAL/3)*2 
					&&  d->event.button.y <= (SCREEN_HEIGHT_GLOBAL/3)*2 + SCREEN_HEIGHT_GLOBAL/6)	{
                        if(d->event.button.button == SDL_BUTTON_LEFT){
                            *pause = 0;
                        }
				 } else if(d->event.button.x >= (SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2) 
							 && d->event.button.x <= (SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2) + SCREEN_WIDTH_GLOBAL/6 
							 && d->event.button.y >= (SCREEN_HEIGHT_GLOBAL/3)*2 + ((SCREEN_HEIGHT_GLOBAL/6)+5)  
							 &&  d->event.button.y <= (SCREEN_HEIGHT_GLOBAL/3)*2 + (2*(SCREEN_HEIGHT_GLOBAL/6)))	{
                        if(d->event.button.button == SDL_BUTTON_LEFT){
                            *restart = 1;
                            *pause = 0;
						}
				}
			}
			case SDL_KEYDOWN:
			{
				if(d->event.key.keysym.sym == SDLK_ESCAPE)
				{
					SDL_Quit();
					exit(1);
				}
			}
		}
	}
}

int final_screen()
{
    Display d = getDisplayPointer(NULL);
    SDL_Delay(20);
    
        char score[200];
        int scoreStringWidth;
        sprintf(score, "PLAYER SCORE: %d", getPlayerScore());
        TTF_SizeText(d->playerScoreFont, score, &scoreStringWidth, NULL);
	
		if(getHealth(getGame(NULL)) > 0)
		{
			displayMonitor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, d->winBackgroundTexture);
			display_text(SCREEN_WIDTH_GLOBAL / 2 - scoreStringWidth / 2, SCREEN_HEIGHT_GLOBAL / 4, score, blended, d->playerScoreFont, d->red);
		}
		else
		{
        	animateAnyPic(0, 0, SCREEN_WIDTH_GLOBAL, SCREEN_HEIGHT_GLOBAL, 3072, 645, 3, 150, d->finalBackgroundTexture);
			display_text(SCREEN_WIDTH_GLOBAL / 2 - scoreStringWidth / 2, SCREEN_HEIGHT_GLOBAL / 4, score, blended, d->playerScoreFont, d->white);
		}
        d->rect = (SDL_Rect) {(SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2), (SCREEN_HEIGHT_GLOBAL/3)*2, SCREEN_HEIGHT_GLOBAL/6, SCREEN_HEIGHT_GLOBAL/6};
    
        SDL_RenderCopy(d->renderer, d->reStartButton, NULL, &d->rect);
        SDL_RenderPresent(d->renderer);
        int check = 0;
        check = (SDL_PollEvent(&d->event));
        if(check != 0)
        {
            switch(d->event.type)
            {
                case SDL_MOUSEBUTTONDOWN:
                {
                    if(d->event.button.x >= (SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2) && d->event.button.x <= (SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2) + SCREEN_WIDTH_GLOBAL/6 && d->event.button.y >= (SCREEN_HEIGHT_GLOBAL/3)*2 &&  d->event.button.y <= (SCREEN_HEIGHT_GLOBAL/3)*2 + SCREEN_HEIGHT_GLOBAL/6)
                        if(d->event.button.button == SDL_BUTTON_LEFT){
                            return 1;
                            //SDL_Quit();
                            //shutSDL();
                        }
                }
                case SDL_KEYDOWN:
                {
                    if(d->event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        SDL_Quit();
                        exit(1);
                    }
                }
            }
        }
    return 0;
}

/* 
 *End of terminal functions
 */

