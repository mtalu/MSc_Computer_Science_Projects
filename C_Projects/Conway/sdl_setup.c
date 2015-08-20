//
//  sdl_setup.c
//  sdl_setup
//
//  Created by Michael on 30/11/2014.
//  Copyright (c) 2014 Michael. All rights reserved.
//

#include "SDL_Setup.h"

Display *createDisplay(void) {
    Display *d = (Display *) malloc(sizeof(Display));
    
    initializeSDL();
    
    d->window = createWindow();
    d->renderer = createRenderer(d->window);
    
    SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(d->renderer);
    SDL_RenderPresent(d->renderer);
    
    return d;
}

void initializeSDL() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "SDL could not initialize. SDL_Error %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

SDL_Window *createWindow() {
    
    SDL_Window *window = NULL;
    
    window = SDL_CreateWindow("Conway's Soldiers", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    
    if(window == NULL) {
        fprintf(stderr, "Failed to create window");
        exit(EXIT_FAILURE);
    }
    
    return window;
}

SDL_Renderer *createRenderer(SDL_Window *window) {
    SDL_Renderer *renderer = NULL;
    
    renderer = SDL_CreateRenderer(window, -1, 0);
    
    if(renderer == NULL) {
        fprintf(stderr, "Failed to create renderer");
        exit(EXIT_FAILURE);
    }
    
    return renderer;
}

void quitSDL(Display *d) {
    
    SDL_DestroyWindow(d->window);
    SDL_Quit();
}

void renderFillCircle(SDL_Renderer *rend, int cx, int cy, int r)
{
    
    for (double dy = 1; dy <= r; dy += 1.0) {
        double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        SDL_RenderDrawLine(rend, cx-dx, cy+r-dy, cx+dx, cy+r-dy);
        SDL_RenderDrawLine(rend, cx-dx, cy-r+dy, cx+dx, cy-r+dy);
    }
    
}


