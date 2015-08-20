#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "SDL_Setup.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

void InitializeSDL();
SDL_Window *CreateWindow();
SDL_Surface *CreateSurface(SDL_Window *window);
SDL_Rect *CreateRect(SDL_Surface *surface, int x, int y, int h, int w, Uint32 colour);
void QuitSDL(SDL_Window *window, SDL_Surface *surface);


void InitializeSDL() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "SDL could not initialize. SDL_Error %s\n", SDL_GetError());
        exit(1);
    }
}

SDL_Window *CreateWindow() {
    
    SDL_Window *window = NULL;
    
    window = SDL_CreateWindow("Maze", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_HIDDEN);
    
    if(window == NULL) {
        fprintf(stderr, "Failed to create window");
    }
    
    return window;
}

SDL_Surface *CreateSurface(SDL_Window *window) {
    SDL_Surface *surface = NULL;
    surface = SDL_GetWindowSurface(window);
    
    if(surface == NULL) {
        fprintf(stderr, "Failed to create surface");
    }
    
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface -> format, 0xFF, 0xFF, 0xFF));
    
    return surface;
}

SDL_Rect *CreateRect(SDL_Surface *surface, int x, int y, int h, int w, Uint32 colour) {
    static SDL_Rect rectangle;
    
    rectangle.x = x, rectangle.y = y, rectangle.h = h, rectangle.w = w;
    SDL_FillRect(surface, &rectangle, colour);
    
    return &rectangle;
}

void QuitSDL(SDL_Window *window, SDL_Surface *surface) {
    
    SDL_DestroyWindow(window);
    SDL_Quit();
}


