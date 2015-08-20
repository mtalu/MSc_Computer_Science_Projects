//
//  sdl_setup.h
//  sdl_setup
//
//  Created by Michael on 05/12/2014.
//  Copyright (c) 2014 Michael. All rights reserved.
//

#ifndef sdl_setup_h

#include <SDL2/SDL.h>

#define WINDOW_WIDTH 350
#define WINDOW_HEIGHT 400

typedef struct display {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Rect rectangle;
} Display;

void initializeSDL();
Display *createDisplay(void);
SDL_Window *createWindow();
SDL_Renderer *createRenderer(SDL_Window *window);
void renderFillCircle(SDL_Renderer *rend, int cx, int cy, int r);
void quitSDL(Display *d);


#define sdl_setup_h

#endif
