#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

void InitializeSDL();
SDL_Window *CreateWindow();
SDL_Surface *CreateSurface(SDL_Window *window);
SDL_Rect *CreateRect(SDL_Surface *surface, int x, int y, int h, int w, Uint32 colour);
void KeepWindowOpen(SDL_Window *window, SDL_Rect *rectangle);
void QuitSDL(SDL_Window *window, SDL_Surface *surface);
