//
//  sdl_setup.c
//  sdl_setup
//
//  Created by Michael on 30/11/2014.
//  Copyright (c) 2014 Michael. All rights reserved.
//

/*---------- Standard Headers -----------*/
#include <stdbool.h>

/*---------- Custom Headers	-----------*/
#include "my_sdl_setup.h"

/*----------Function Prototypes (Internal)-----------*/
SDL_Window *mySDL_createWindow();
SDL_Renderer *mySDL_createRenderer(SDL_Window *window);

/**
 Create new display object when called with display pointer or return previously created display
 when called with NULL
 */
Display *mySDL_getDisplay(Display *newDisplay) {
    static Display *d;
    
    if(newDisplay != NULL) {
        d = newDisplay;
    }
    
    return d;
}

/**
 Wrapper for mySDL_getDisplay function when initializing display object the first time
 */
void mySDL_initializeDisplay(void) {
    static Display *d;
    d = (Display *) malloc(sizeof(Display));
    
    mySDL_initializeSDL();
    
    d->window = mySDL_createWindow();
    d->renderer = mySDL_createRenderer(d->window);
    
    SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(d->renderer);
    mySDL_getDisplay(d);
}

/**
 Initialize SDL and handle errors
 */
void mySDL_initializeSDL() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "SDL could not initialize. SDL_Error %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 Create SDL window and handle errors
 */
SDL_Window *mySDL_createWindow() {
    
    SDL_Window *window = SDL_CreateWindow("Turtles", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    
    /*window = SDL_CreateWindow("Turtles", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);*/
    
    if(window == NULL) {
        fprintf(stderr, "Failed to create window\n");
        exit(EXIT_FAILURE);
    }
    
    return window;
}

/**
 Create SDL renderer and handle errors
 */
SDL_Renderer *mySDL_createRenderer(SDL_Window *window) {
    SDL_Renderer *renderer = NULL;
    
    renderer = SDL_CreateRenderer(window, -1, 0);
    
    if(renderer == NULL) {
        fprintf(stderr, "Failed to create renderer");
        exit(EXIT_FAILURE);
    }
    
    return renderer;
}

/**
 Destroy all SDL objects and quit SDL
 */
void mySDL_quitSDL(void) {
    
    Display *d = mySDL_getDisplay(NULL);
    
    SDL_DestroyWindow(d->window);
    SDL_Quit();
}

void mySDL_setDrawColour(Uint8 r, Uint8 g, Uint8 b) {
    Display *d = mySDL_getDisplay(NULL);
    
    SDL_SetRenderDrawColor(d->renderer, r, g, b, SDL_ALPHA_OPAQUE);
}

/**
 Draw a circle
 */
void mySDL_renderFillCircle(SDL_Renderer *rend, int cx, int cy, int r)
{
    
    for (double dy = 1; dy <= r; dy += 1.0) {
        double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        SDL_RenderDrawLine(rend, cx-dx, cy+r-dy, cx+dx, cy+r-dy);
        SDL_RenderDrawLine(rend, cx-dx, cy-r+dy, cx+dx, cy-r+dy);
    }
    
}

void mySDL_renderPresent(void) {
    Display *d = mySDL_getDisplay(NULL);
    
    SDL_RenderPresent(d->renderer);
}


