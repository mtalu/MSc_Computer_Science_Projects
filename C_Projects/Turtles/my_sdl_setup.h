//
//  sdl_setup.h
//  sdl_setup
//
//  Created by Michael on 05/12/2014.
//  Copyright (c) 2014 Michael. All rights reserved.
//

#ifndef sdl_setup_h

/*----------Standard Headers-----------*/
#include <SDL2/SDL.h>

/*----------Data Types-----------*/
typedef struct display {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Rect rectangle;
} Display;

/*----------Hash Defines-----------*/
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

/*----------Function Prototypes (External)-----------*/
void mySDL_initializeSDL(void);
void mySDL_initializeDisplay(void);
Display *mySDL_getDisplay(Display *newDisplay);
void mySDL_setDrawColour(Uint8 r, Uint8 g, Uint8 b);
void mySDL_renderFillCircle(SDL_Renderer *rend, int cx, int cy, int r);
void mySDL_quitSDL(void);
void mySDL_renderPresent(void);


#define sdl_setup_h

#endif
