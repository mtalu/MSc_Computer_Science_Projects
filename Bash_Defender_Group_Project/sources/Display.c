//
//  Display.c
//
//  All functions directly related to displaying the game
//
//

#include "../includes/Display.h"
#include "../includes/parser.h"
#include <stdbool.h>


int SCREEN_WIDTH_GLOBAL;
int SCREEN_HEIGHT_GLOBAL;

#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

struct display {
    //main objects
    SDL_Window  *window;
    SDL_Surface *surface;
    SDL_Renderer *renderer;
    SDL_Rect    srcRect;
    SDL_Rect    rect;
    SDL_Event event;
    SDL_Color white;
    SDL_Color red;
    TTF_Font *font;
	    
    SDL_Texture *statsBarTexture;
    SDL_Texture *towerInfoTexture;
    
    //background
    SDL_Texture *map;
    
    //tower monitor
    SDL_Texture *towerMonitorTexture;

	//menu Screens
	SDL_Texture *startBackgroundTexture;
    SDL_Texture *finalBackgroundTexture;


	SDL_Texture *startButton;
    SDL_Texture *reStartButton;
   	SDL_Texture *returnButton; 
	SDL_Texture *tutorialButton;

	//terminal Window
    SDL_Texture *newtexture;
	SDL_Texture *terminalWindowTexture;

    //Action queue monitor objects
    SDL_Texture *actionQueueTexture;

    //Tower objects
    SDL_Texture *towerTexture[3];
    SDL_Texture *towerPositionTexture[26];

    //Projectile objects
    SDL_Texture *bulletTexture[3];

    //enemy
    SDL_Texture *enemyTexture[5];
    
    //animation
    SDL_Texture *circ1_Texture[2];
    SDL_Texture *circ2_Texture[2];
    
    SDL_Texture *firewall;
};

/*Functions prototypes for functions only used internally*/

void initTTF(void);
SDL_Surface *getInfoWindowTextSurface(char *outputString);
TTF_Font *getInfoWindowFont(TTF_Font *font);
void crash(char *message);
void getWindowSize(int *w, int *h);
void init_pic(SDL_Texture **texture, char *pic_name);
void check_load_images(SDL_Surface *surface, char *pic_name);
void draw_filled_range(int cx, int cy, int r);
void presentCircuit(Display d,SDL_Texture *text[2], int x,int y,int w, int h, int frames, int pic_width, int pic_height, int anim_speed);
void animateAnyPic(int x, int y, int w, int h, int pic_width, int pic_height, int frames, int anim_speed, SDL_Texture *texture);


Display init_SDL(){
    
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) crash("SDL_Init()");
    if(TTF_Init() != 0) crash("TTF_Init()");
    if(IMG_Init(0) != 0) crash("IMG_Init()");
    
    Display d = (Display)malloc(sizeof(struct display));
    getDisplayPointer(d);
    
    d->window = SDL_CreateWindow("TOWER DEFENSE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_YN);
    d->renderer = SDL_CreateRenderer(d->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    getWindowSize(&SCREEN_WIDTH_GLOBAL,&SCREEN_HEIGHT_GLOBAL);
  
    d->font= TTF_OpenFont("OpenSans-Regular.ttf", 10);
    if(d->font== NULL) crash("TTF_(OpenFont)");
    
    d->white = (SDL_Color) {255, 255, 255};
    d->red = (SDL_Color) {255, 0, 0};
    
    /*improves quality of font*/
    TTF_SetFontHinting(d->font, TTF_HINTING_LIGHT);
    putenv("SDL_VIDEODRIVER=dga");
    
    /*inititalize pictures (load picture to the texture)*/
    init_pic(&d->reStartButton, "Images/RestartButton.png");
    init_pic(&d->finalBackgroundTexture, "Images/final_screen.png");
    init_pic(&d->towerMonitorTexture, "Images/info_monitor.png");
    init_pic(&d->actionQueueTexture, "Images/action_queue-monitor.png");
    init_pic(&d->statsBarTexture, "Images/blackBar.png");
    init_pic(&d->towerInfoTexture, "Images/towerInfoBackground.png");
    init_pic(&d->startBackgroundTexture, "Images/anistrip_menu.png");
    init_pic(&d->startButton, "Images/start-button.png");
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
    init_pic(&d->enemyTexture[1], "Images/int_enemy_basic.png");
    init_pic(&d->enemyTexture[2], "Images/int_enemy_basic.png");
    init_pic(&d->enemyTexture[3], "Images/char_enemy_basic.png");
    init_pic(&d->enemyTexture[4], "Images/char_enemy_basic.png");
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

/*draw firewall*/
void animateAnyPic(int x, int y, int w, int h, int pic_width, int pic_height, int frames, int anim_speed, SDL_Texture *texture){
    Display d = getDisplayPointer(NULL);
    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite = (ticks / anim_speed) % frames;
    d->srcRect = (SDL_Rect){sprite * (pic_width/frames), 0, (pic_width/frames), pic_height};
    d->rect = (SDL_Rect) {x, y, w, h};
    /*create animation by putting part of a spritesheet(image) into destination rect*/
    SDL_RenderCopy(d->renderer, texture, &d->srcRect, &d->rect);
}



/*Tower and enemy graphics functions*/
void getBackgroundDimensions(int *w, int *h){
    Display d =getDisplayPointer(NULL);
    SDL_QueryTexture(d->map, NULL, NULL, w, h);
}
/* Draw kill all ability*/
void drawKillAll(){
    SDL_Delay(3);
    Display d = getDisplayPointer(NULL);
    d->rect = (SDL_Rect) {0,0, SCREEN_WIDTH_GLOBAL, SCREEN_HEIGHT_GLOBAL};
    SDL_SetRenderDrawBlendMode(d->renderer, SDL_BLENDMODE_BLEND);
    int saturation = 0;
    while (saturation < 255) {
        SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, saturation += 4);
        SDL_RenderFillRect(d->renderer, &d->rect);
        SDL_RenderPresent(d->renderer);
    }
}

/*call fucntion in the while loop to present all the animations*/
void presentAnimation(){
    Display d = getDisplayPointer(NULL);
    presentCircuit(d, d->circ1_Texture,100, 100, 30, 100, 6,385, 324, 600);
    presentCircuit(d, d->circ2_Texture,300, 300, 30, 60, 7,386, 195, 350);
}

/*draw background image*/
void drawBackground(){
    Display d = getDisplayPointer(NULL);
    SDL_RenderCopy(d->renderer, d->map, NULL, NULL);
}

/*Draw tower position*/
void drawTowerPosition(int x, int y, int w, int h,tPosIcon tIcon){
    Display d =getDisplayPointer(NULL);
    d->rect = (SDL_Rect) {x, y, w, h};
    SDL_RenderCopy(d->renderer, d->towerPositionTexture[tIcon-1], NULL, &d->rect);
}

/*present any animation with one staitc image at the back and one above it*/
void presentCircuit(Display d,SDL_Texture *text[2], int x,int y,int w, int h, int frames, int pic_width, int pic_height, int anim_speed){
    
    d->rect= (SDL_Rect) {x, y , w * frames, h};
    SDL_RenderCopy(d->renderer, text[0], NULL, &d->rect);
    
    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite = (ticks / anim_speed) % frames;
    //srcrect runs through the actual picture size
    d->srcRect = (SDL_Rect){ sprite * (pic_width/frames), 0, (pic_width/frames), pic_height};
    
    //dstrect size could be decided by the user
    d->rect = (SDL_Rect) {sprite * w + x, y, w, h};
    SDL_RenderCopy(d->renderer, text[1], &d->srcRect, &d->rect);
}

/*Initialize images and check whether they were loaded successfully*/
void init_pic(SDL_Texture **texture, char *pic_name){
    Display d = getDisplayPointer(NULL);
    d->surface = IMG_Load(pic_name);
    check_load_images(d->surface, pic_name);
    *texture = SDL_CreateTextureFromSurface(d->renderer, d->surface);
    SDL_FreeSurface(d->surface);
}

/*Get pointer to the Display object*/
Display getDisplayPointer(Display d){
	static Display disp;
	if(d != NULL) {
		disp = d;
	}
	return disp;
}

/*Prints last SDL error message to stderr, along withb message included in first parameter.*/
void crash(char *message) {
    fprintf(stderr, "%s: %s\n", message, SDL_GetError());
    SDL_Quit();
}

/* fill variables with the width and height values of the screen*/
void getWindowSize(int *w, int *h){
    Display d = getDisplayPointer(NULL);
    SDL_GetWindowSize(d->window, w, h);
}

/*draw damage line from X & Y to target X & Y*/
void drawLine(Display d, int X_from, int Y_from, int X_target, int Y_target, int laserType){
      // choose laser colour depending on type
      int sat = 5;
      int adjust = -5;
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

/* draw an enemy at x and y coor with the health bar above it*/
void drawEnemy(int x, int y, int w, int h, int pic_width, int pic_height, int type, int frames, int anim_speed){
    Display d = getDisplayPointer(NULL);
    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite = (ticks / anim_speed) % frames;
    d->srcRect = (SDL_Rect){sprite * (pic_width/frames), 0, (pic_width/frames), pic_height};
    d->rect = (SDL_Rect) {x, y, w, h};
    /*create animation by putting part of a spritesheet(image) into destination rect*/
    SDL_RenderCopy(d->renderer, d->enemyTexture[type], &d->srcRect, &d->rect);
}

/* draw CPU rect*/
void drawRect(int x, int y, int red, int blue, int max_width, int max_height, int current, int total){
    Display d = getDisplayPointer(NULL);
    SDL_SetRenderDrawBlendMode(d->renderer, SDL_BLENDMODE_NONE);
    /*presenting and manipulating color and width of the health bar*/
    double color = (255*((double)current/total));
    SDL_SetRenderDrawColor(d->renderer, red, color, blue, 0);
    double memory = ((double)(current * max_width/(double)total));
    d->rect = (SDL_Rect) {x, y, (double)memory, max_height};
    SDL_RenderFillRect(d->renderer, &d->rect);
}

    

/*Draw range with transparency*/
void draw_filled_range(int cx, int cy, int r)
{
    Display d = getDisplayPointer(NULL);
    Uint32 ticks = SDL_GetTicks();
    Uint32 clock = (ticks / 150)% 20;
    //mx saturation is a max extreme to ensure saturation does not get bigger than pre-set value
    float max_satur = 30;
    // set initial saturation
    float initial_saturation = 30;
    // set variable saturation to equal initial saturation
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

/* draws the tower at x and y coor */
void drawTower(int x, int y, int w, int h, int type, int range, int frames, int anim_speed, int pic_width, int pic_height){
    Display d = getDisplayPointer(NULL);
    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite = (ticks / anim_speed) % frames;
    d->srcRect = (SDL_Rect){sprite * (pic_width/frames), 0, (pic_width/frames), pic_height};
    d->rect = (SDL_Rect) {x, y, w, h};
    /*create animation by putting part of a spritesheet(image) into destination rect*/
    SDL_RenderCopy(d->renderer, d->towerTexture[type], &d->srcRect, &d->rect);
    draw_filled_range(x + (double)w/2, y + (double)h/2, range);
}


/* draws projectile */
void drawBullet(int x, int y, int w, int h, int bulletType) {
    Display d = getDisplayPointer(NULL);
    d->rect = (SDL_Rect) {x, y, w, h};
      // draw red or green bullet depending on type
    if(bulletType == INT_TYPE) {
        SDL_RenderCopy(d->renderer, d->bulletTexture[0], NULL, &d->rect);
    } else {
        SDL_RenderCopy(d->renderer, d->bulletTexture[1], NULL, &d->rect);
    }
}
  

/*clear the screen before making any drawings */
void startFrame(Display d) {
    SDL_RenderClear(d->renderer);
}

/*peresent everything renderer has to draw*/
void endFrame(Display d) {
    SDL_RenderPresent(d->renderer);
    SDL_Delay(20);
}

/* check whether image was loaded successfully */
void check_load_images(SDL_Surface *surface, char *pic_name){
    if(surface == NULL){
        printf("Cannot find %s\n", pic_name);
        SDL_Quit();
        exit(1);
    }
}

/*destroy everything */
void shutSDL() {
    Display d = getDisplayPointer(NULL);
    SDL_DestroyTexture(d->statsBarTexture);
    SDL_DestroyTexture(d->map);
    SDL_DestroyTexture(d->towerMonitorTexture);
    SDL_DestroyTexture(d->startBackgroundTexture);
    SDL_DestroyTexture(d->startButton);
    SDL_DestroyTexture(d->newtexture);
    SDL_DestroyTexture(d->terminalWindowTexture);
    SDL_DestroyTexture(d->actionQueueTexture);
    SDL_DestroyTexture(d->towerPositionTexture[0]);
    SDL_DestroyTexture(d->startButton);
    SDL_DestroyTexture(d->startBackgroundTexture);

    SDL_DestroyRenderer(d->renderer);
    SDL_DestroyWindow(d->window);
    SDL_Quit();
    IMG_Quit();
    TTF_CloseFont(d->font);
    TTF_Quit();
}

/*End of tower and enemy graphics functions */



/*Information window functions*/

/*Display monitor*/
void displayMonitor(int x, int y, int w, int h, SDL_Texture *texture){
    Display d = getDisplayPointer(NULL);
    d->rect= (SDL_Rect){x, y, w, h};
    SDL_RenderCopy(d->renderer, texture, NULL, &d->rect);
}

/*Display empty stats bar at top of screen*/
void displayStatsBar() {
    Display d = getDisplayPointer(NULL);
    displayMonitor(STATS_BAR_X,  STATS_BAR_Y,  SCREEN_WIDTH_GLOBAL, STATS_BAR_HEIGHT, d->statsBarTexture);
}

/**Display output string in tower monitor*/
void updateTowerMonitor(char *outputString) {
    Display d = getDisplayPointer(NULL);
    displayMonitor(TOWER_MONITOR_X, TOWER_MONITOR_Y, TOWER_MONITOR_WIDTH, TOWER_MONITOR_HEIGHT, d->towerMonitorTexture);
    display_text(TOWER_MONITOR_X + TOWER_TEXT_BORDER_X,  TOWER_MONITOR_Y + TOWER_TEXT_BORDER_Y, outputString, blended_wrapped, d->white);

    //free(outputString);
}

/** Display output string in stats monitor*/
void updateStatsBar(char *outputString) {
    Display d = getDisplayPointer(NULL);
    displayStatsBar();
    display_text(STATS_BAR_X + (double)SCREEN_WIDTH_GLOBAL / 8,  STATS_BAR_Y + 10, outputString, blended, d->white);
    free(outputString);
}

/**Display output string in action queue monitor*/
void updateActionQueueMonitor(char *outputString) {
    Display d = getDisplayPointer(NULL);
    
     displayMonitor(ACTION_QUEUE_X, ACTION_QUEUE_Y, TERMINAL_WINDOW_WIDTH, TERMINAL_WINDOW_HEIGHT, d->actionQueueTexture);
    if(strlen(outputString) > 0) {
        display_text(ACTION_QUEUE_X + ACTION_QUEUE_BORDER_X, ACTION_QUEUE_Y + ACTION_QUEUE_BORDER_Y, outputString, blended_wrapped, d->white);
    }
}

void updateTowerInformation(int towerX, int towerY, char *string, int towerID) {
    Display d = getDisplayPointer(NULL);
    
    int towerWidth = getTowerWidth(towerID);
    
    displayMonitor(towerX - 5, towerY - 20, towerWidth + 10, 30, d->towerInfoTexture);
    display_text(towerX + 5, towerY - 10, string, blended, d->red);
}

/**Display output string in terminal window*/
void updateTerminalWindow(char *outputString) {
    Display d = getDisplayPointer(NULL);
    
    if(outputString != NULL) {
        display_text(TERMINAL_WINDOW_X + TOWER_TEXT_BORDER_X, TERMINAL_WINDOW_Y + TOWER_TEXT_BORDER_Y, outputString, blended_wrapped, d->white);
    }
}

/*End of information window functions*/


//Terminal functions

/*terminal_window detects input from SDL and calls display_text*/
int terminal_window(Display d, char *pass, char *clear, int *pause,int restart)
{
	int done = 0;
    char *pass2;
    //Keeps text on screen
    displayMonitor(TERMINAL_WINDOW_X, TERMINAL_WINDOW_Y, TERMINAL_WINDOW_WIDTH, TERMINAL_WINDOW_HEIGHT, d->terminalWindowTexture);
    display_text(TERMINAL_WINDOW_X + (TERMINAL_WINDOW_WIDTH / 10),TERMINAL_WINDOW_Y + (TERMINAL_WINDOW_HEIGHT - (TERMINAL_WINDOW_HEIGHT / 7)), pass,solid, d->white);    int check = 0;
    SDL_Event *event = &d->event;
    check = (SDL_PollEvent(event));
    if(check != 0)
    {
        switch (event->type)
        {
			//Detects input and appends to a string
            case SDL_TEXTINPUT:
            {
                strcat(pass, event->text.text);
                display_text(TERMINAL_WINDOW_X + (TERMINAL_WINDOW_WIDTH / 10),TERMINAL_WINDOW_Y + (TERMINAL_WINDOW_HEIGHT - (TERMINAL_WINDOW_HEIGHT / 7)), pass,solid, d->white);                break;
            }
            case SDL_KEYDOWN:
            {
				/*
				If return key is pressed, clears string and sends desired
				input to parser
				*/		
                if(event->key.keysym.sym == SDLK_RETURN)
                {
                    if(strcmp(pass, clear) != 0)
                    {
                         display_text(TERMINAL_WINDOW_X + (TERMINAL_WINDOW_WIDTH / 10),TERMINAL_WINDOW_Y + (TERMINAL_WINDOW_HEIGHT - (TERMINAL_WINDOW_HEIGHT / 7)), pass,solid, d->white);
                    }
                    
                    pass2 = pass + 2;
                    parse(pass2);
                    strcpy(pass, clear);
                }
				//If backspace key is pressed, removes end char of string
				if(event->key.keysym.sym == SDLK_BACKSPACE)
				{
					if(pass[strlen(pass) - 1] != '>')
					{
						pass[strlen(pass) - 1] = '\0';
					}

                    display_text(TERMINAL_WINDOW_X + (TERMINAL_WINDOW_WIDTH / 10),TERMINAL_WINDOW_Y + (TERMINAL_WINDOW_HEIGHT - (TERMINAL_WINDOW_HEIGHT / 7)), pass,solid, d->white);
				}
                switch(d->event.key.keysym.sym)
                {
                	case SDLK_ESCAPE:
					*pause = 1;
                	//done = 1;
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

/*display_text builds textures from surfaces and calls renderer to output them to screen.*/
void display_text(int x, int y, char *string, int text, SDL_Color colour)
{
    Display d = getDisplayPointer(NULL);
    switch (text) {
        case solid:
            d->surface = TTF_RenderText_Solid(d->font, string, colour);
            break;
        case blended:
            d->surface = TTF_RenderText_Blended(d->font, string, colour);
            break;
        case blended_wrapped:
            d->surface = TTF_RenderText_Blended_Wrapped(d->font, string, colour, TOWER_MONITOR_WIDTH - TOWER_TEXT_BORDER_X);
            break;
    }
    d->newtexture = SDL_CreateTextureFromSurface(d->renderer, d->surface);
    if(d->newtexture == NULL) {
        printf("Panic\n");
    }
    d->rect = (SDL_Rect) {x, y, d->surface->w, d->surface->h};
    
    //Display what is necessary using renderer
    SDL_RenderCopy(d->renderer, d->newtexture, NULL, &d->rect);
    SDL_FreeSurface(d->surface);
    //Destroy textures to save memory
    SDL_DestroyTexture(d->newtexture);
    
}

void menu_screen(Display d, gameState *state)
{
    //SDL_RenderCopy(d->renderer, d->startBackgroundTexture, NULL, NULL);
    animateAnyPic(0, 0, SCREEN_WIDTH_GLOBAL, SCREEN_HEIGHT_GLOBAL, 7602, 292, 14, 170, d->startBackgroundTexture);

    d->rect = (SDL_Rect) {(SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2), (SCREEN_HEIGHT_GLOBAL/3)*2, SCREEN_HEIGHT_GLOBAL/6, SCREEN_HEIGHT_GLOBAL/6};

    SDL_RenderCopy(d->renderer, d->startButton, NULL, &d->rect);

    d->rect = (SDL_Rect) {
            (SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2),  //!x
        ((SCREEN_HEIGHT_GLOBAL/3)*2)+(SCREEN_HEIGHT_GLOBAL/6),      //!y
            SCREEN_HEIGHT_GLOBAL/6,         //!Width
            SCREEN_HEIGHT_GLOBAL/6      //!height
    };

    SDL_RenderCopy(d->renderer, d->tutorialButton, NULL, &d->rect);
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
							//!Start Level
                            *state = level1;
							printf("level1\n");
                        }
				}	else if(d->event.button.x >= (SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2)
                             && d->event.button.x <= (SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2) + SCREEN_WIDTH_GLOBAL/6
                             && d->event.button.y >= (SCREEN_HEIGHT_GLOBAL/3)*2 + ((SCREEN_HEIGHT_GLOBAL/6)+5)
                             &&  d->event.button.y <= (SCREEN_HEIGHT_GLOBAL/3)*2 + (2*(SCREEN_HEIGHT_GLOBAL/6)))    {
                        if(d->event.button.button == SDL_BUTTON_LEFT){
							*state = tutorial; 
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

void pause_screen(Display d, int *pause, int *restart)
{
    //SDL_RenderCopy(d->renderer, d->startBackgroundTexture, NULL, NULL);
    animateAnyPic(0, 0, SCREEN_WIDTH_GLOBAL, SCREEN_HEIGHT_GLOBAL, 7602, 292, 14, 170, d->startBackgroundTexture);

    d->rect = (SDL_Rect) {(SCREEN_WIDTH_GLOBAL/2) - ((SCREEN_HEIGHT_GLOBAL/6)/2), 
				(SCREEN_HEIGHT_GLOBAL/3)*2, 
				SCREEN_HEIGHT_GLOBAL/6, 
				SCREEN_HEIGHT_GLOBAL/6
	};

    SDL_RenderCopy(d->renderer, d->returnButton, NULL, &d->rect);
	//SDL_RenderPresent(d->renderer);
    
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

        animateAnyPic(0, 0, SCREEN_WIDTH_GLOBAL, SCREEN_HEIGHT_GLOBAL, 3072, 645, 3, 150, d->finalBackgroundTexture);
        
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

//End of terminal functions

