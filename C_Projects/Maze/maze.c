//
//  maze.c
//  Maze
//
//  Created by Michael on 10/11/2014.
//  Copyright (c) 2014 Michael. All rights reserved.
//
// ******************************************************************************
// I have implemented Basic + SDL. I've included the option for RANDOM in the code
// but the function isn't working as intended yet.
// ******************************************************************************
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "SDL2/SDL.h"
#include "SDL_Setup.h"

#define NUM_ROWS 40
#define NUM_COLS 40


void scanMazeDimensions(FILE *inputMaze, int *numRows, int *numCols);
void scanMaze(FILE *inputMaze, char maze[][NUM_COLS], int numRows, int numCols);
void printMaze(char maze[][NUM_COLS], int numRows, int numCols);
bool displayMazeGraphics(char maze[][NUM_COLS], int numRows, int numCols, int y, int x);
bool runGraphicsSimulation(char maze[][NUM_COLS], int numRows, int numCols, int y, int x, SDL_Surface *screen, SDL_Window *window);
void findEntrance(char maze[][NUM_COLS], int numRows, int numCols, int *entranceX, int *entranceY);
bool findExit(char maze[][NUM_COLS], int numRows, int numCols, int y, int x);
bool validIndex(int x, int y, int numRows, int numCols);
bool isPossibleExit(int y, int x, int numRows, int numCols);
void generateRandomMaze(char maze[][NUM_COLS], int yStart, int yEnd, int xStart, int xEnd);
bool makeHorizontal(int height, int width);
int getRandomNumber(int max, int min);
void initializeRandomMaze(char maze[][NUM_COLS]);
void createEntryandExit(char maze[][NUM_COLS]);

int main(int argc, const char * argv[]) {
    
    FILE *inputMaze;
    char maze[NUM_ROWS] [NUM_COLS];
    int numRows, numCols, entranceX, entranceY;
    bool random = false, sdl = false, exitFound;
    
    //Check first command line argument
    if(argv[1] == NULL) {
        fprintf(stderr, "Error: Command line argument not specified\n");
        exit(EXIT_FAILURE);
    }
    else if(strcmp(argv[1], "RANDOM") == 0) {
        random = true;
    }
    else {
        inputMaze = fopen(argv[1], "r");
        if(inputMaze == NULL) {
            fprintf(stderr, "Error: Could not open file\n");
            exit(EXIT_FAILURE);
        }
    }
    
    //Check second command line argument
    if(argv[2] != NULL && strcmp(argv[2], "SDL") == 0) {
        sdl = true;
    }
    
    //Carry out action accordingly
    if(!random) {
        scanMazeDimensions(inputMaze, &numRows, &numCols);
        scanMaze(inputMaze, maze, numRows, numCols);
        findEntrance(maze, numRows, numCols, &entranceX, &entranceY);
        maze[entranceY] [entranceX] = '.'; //Mark the entrance
        if(!sdl) {
            exitFound = findExit(maze, numRows, numCols, entranceY, entranceX);
            if(exitFound) {
                printf("Exit found\n\n");
                printMaze(maze, numRows, numCols);
                return 0;
            }
            else {
                printf("No exit was found\n\n");
                printMaze(maze, numRows, numCols);
                return 1;
            }
            
        }
        else
           exitFound = displayMazeGraphics(maze, numRows, numCols, entranceY, entranceX);
            if(exitFound) {
                return 0;
            }
            else{
                return 1;
            }
        }
    else {
        srand((unsigned int)(time(NULL)));
        initializeRandomMaze(maze);
        generateRandomMaze(maze, 1, NUM_ROWS - 1, 1, NUM_COLS - 1);
        createEntryandExit(maze);
        findEntrance(maze, NUM_ROWS, NUM_COLS, &entranceX, &entranceY);
        maze[entranceY] [entranceX] = '.'; //Mark the entrance
        if(!sdl) {
            exitFound = findExit(maze, NUM_ROWS, NUM_COLS, entranceY, entranceX);
            if(exitFound) {
                printf("Exit found\n\n");
                printMaze(maze, NUM_ROWS, NUM_COLS);
                return 0;
            }
            else {
                printf("No exit was found\n\n");
                printMaze(maze, NUM_ROWS, NUM_COLS);
                return 1;
            }

        }
        else {
            exitFound = displayMazeGraphics(maze, NUM_ROWS, NUM_COLS, entranceY, entranceX);
            if(exitFound) {
                return 0;
            }
            else{
                return 1;
            }
        }
    }
    
    return 0;
}

void scanMazeDimensions(FILE *inputMaze, int *numRows, int *numCols) {
    
     //Scan maze dimensions to variables in main and skip rest of line,
     //error message and exit if dimensions not be found
    
    char inputChar;
    
    if((fscanf(inputMaze, "%d%d", numCols, numRows)) != 2) {
        fprintf(stderr, "Error: Unable to scan maze dimensions\n");
        exit(EXIT_FAILURE);
    }
    
    while((inputChar = getc(inputMaze)) != '\n') {
        ;//Skip rest of line
    }
}

void scanMaze(FILE *inputMaze, char maze[][NUM_COLS], int numRows, int numCols) {
    //Scan maze into array in main, skip new-line character
    
    
    char inputChar;
    
    for(int i = 0; i < numRows; i++) {
        for(int j = 0; j < numCols; j++) {
            inputChar = getc(inputMaze);
            
            //Skip new-line character
            if(inputChar == '\n') {
                inputChar = getc(inputMaze);
            }
            
            //Error and exit if illegal symbols encountered
            if(inputChar != ' ' && inputChar != '#') {
                fprintf(stderr, "Error: illegal character encountered\n");
                exit(EXIT_FAILURE);
            }
            
            maze[i] [j] = inputChar;
        }
    }
}

void findEntrance(char maze[][NUM_COLS], int numRows, int numCols,int *entranceX, int *entranceY) {
     //Loop over start and end of every row and every column until entrance found.
     //Assign x and y coordinates of entrance if found, otherwise error and exit.
    
    //Is row or column the larger of the two
    int largestDimension = numRows > numCols ? numRows : numCols;
    
    
    //Loop alternating between points on first row and first column to find closest entrance to top-left corner
    for(int i = 0; i < largestDimension; i++) {
        if(validIndex(i, 0, numRows, numCols) && maze[i] [0] == ' ') {
            *entranceY = i;
            *entranceX = 0;
            return;
        }
        if(validIndex(0, i, numRows, numCols) && maze[0] [i] == ' ') {
            *entranceY = 0;
            *entranceX = i;
            return;
        }
    }
    
    //Loop alternating between points on last column and last row to find closest entrance to top-left corner if not yet found
    for(int i = 0; i < largestDimension; i++) {
        if(validIndex(i, numCols - 1, numRows, numCols) && maze[i] [numCols - 1] == ' ') {
            *entranceY = i;
            *entranceX = numCols - 1;
            return;
        }
        if(validIndex(numRows -1, i, numRows, numCols) && maze[numRows - 1] [i] == ' ') {
            *entranceY = numRows - 1;
            *entranceX = i;
            return;
        }
    }
    
    //No entrance found, print error and exit
    fprintf(stderr, "Error: maze entrance not found\n\n");
    printMaze(maze, numRows, numCols);
    exit(EXIT_FAILURE);
}

bool findExit(char maze[][NUM_COLS], int numRows, int numCols, int y, int x) {
     //Function calls itself recursively until an exit is found or until all avenues have been
     //explored and no exit is found. Marks path if one is found, returns true or false accordingly.
     //Function limitation: If maze has more than one exit, it only marks one and not necessarily the shortest.
    
    
    bool exitFound = false;

    
    //Base condition: return when exit found
    if(maze[y] [x] == ' ' && isPossibleExit(y, x, numRows, numCols)) {
        exitFound = true;
        maze[y] [x] = '.'; //mark exit
        return exitFound;
    }
    
    //Mark current position so that we cannot return to it
    maze[y] [x] = '.';
    

    //Check north and call recursively with new coordinates if can go
    if(validIndex(y - 1, x, numRows, numCols) && (maze[y - 1] [x] == ' ')) {
        if(findExit(maze, numRows, numCols, y - 1, x)) {
            return 1;
        }
    }
    
    //Check west and call recursively with new coordinates if can go
    if(validIndex(y, x + 1, numRows, numCols) && (maze[y] [x + 1] == ' ')) {
        if(findExit(maze, numRows, numCols, y, x + 1)) {
            return 1;
        }
    }
    
    //Check south and call recursively with new coordinates if can go
    if(validIndex(y + 1, x, numRows, numCols) && (maze[y + 1] [x] == ' ')) {
        if(findExit(maze, numRows, numCols, y + 1, x)) {
            return 1;
        }
    }
    
    //Check east and call recursively with new coordinates if can go
    if(validIndex(y, x - 1, numRows, numCols) && (maze[y] [x - 1] == ' ')) {
        if(findExit(maze, numRows, numCols, y, x - 1)) {
            return 1;
        }
    }
    
    //Dead end reached, mark with space character again before returning and
    //exploring other avenues.
    if(!isPossibleExit(y, x, numRows, numCols)) {
        maze[y][x] = ' ';
    }
    
    return exitFound;
    
}

void printMaze(char maze[][NUM_COLS], int numRows, int numCols) {
     //Print maze, adding new-line character at the end of each row.
    
    for(int i = 0; i < numRows; i++) {
        for(int j = 0; j < numCols; j++) {
            printf("%c", maze[i] [j]);
        }
        printf("\n");
    }
}

bool validIndex(int y, int x, int numRows, int numCols) {
     //Check that maze coordinates are inside the maze
    
    
    if(y >= 0 && y <numRows && x >= 0 && x < numRows) {
        return true;
    }
    
    return false;
}

bool isPossibleExit(int y, int x, int numRows, int numCols) {
     //A possible exit is one that lies either anywhere
     //in the last row, last column, first row or first column.
     //Entrance is marked in main to ensure it can't go back.
     
    
    if(y == numRows - 1 || x == numCols - 1 || x == 0 || y == 0) {
        return true;
    }
    
    return false;
    
    
}

bool displayMazeGraphics(char maze[][NUM_COLS], int numRows, int numCols, int y, int x) {
     //Set up SDL and display the unsolved maze before running simulation
     
    
    InitializeSDL();
    
    SDL_Window *window = CreateWindow();
    SDL_Surface *screen = CreateSurface(window);
    int rectangleHeight = WINDOW_HEIGHT / numRows, rectangleWidth = WINDOW_WIDTH / numCols;
    Uint32 black = SDL_MapRGB(screen -> format, 0x00, 0x00, 0x00);
    Uint32 white = SDL_MapRGB(screen -> format, 0xFF, 0xFF, 0xFF);
    SDL_Rect rectangle;
    rectangle.x = 0, rectangle.y = 0, rectangle.h = rectangleHeight, rectangle.w = rectangleWidth;
    
    
    //Display unsolved maze
    for(int row = 0; row < numRows; row++) {
        for(int col = 0; col < numCols; col++) {
            if(maze[row][col] == '#') {
                rectangle.x = col * rectangleWidth, rectangle.y = row * rectangleHeight;
                SDL_FillRect(screen, &rectangle, black);
            } else{
                rectangle.x = col * rectangleWidth, rectangle.y = row * rectangleHeight;
                SDL_FillRect(screen, &rectangle, white);
            }
        }
    }
    
    SDL_ShowWindow(window);
    SDL_UpdateWindowSurface(window);
    
    //Run the graphics simulation with entrance coordinates
    bool exitFound = runGraphicsSimulation(maze, numRows, numCols, y, x, screen, window);
    
    //Keep window open until user x's out.
    bool quit = false;
    SDL_Event e;
    while(!quit) {
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
                quit = true;
            }
        }
    }
    
    QuitSDL(window, screen);
    
    return exitFound;
}

bool runGraphicsSimulation(char maze[][NUM_COLS], int numRows, int numCols, int y, int x, SDL_Surface *screen, SDL_Window *window) {
    //Same as findExit function, but with graphic simulation instead of printing to terminal
 
    
    //Call first to make sure user can x out window while simulation running
    SDL_Event e;
    
    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
            exit(1);
        }
    }
    
    
    SDL_Rect rectangle;
    Uint32 yellow = SDL_MapRGB(screen -> format, 0xFF, 0xFF, 0x00);
    Uint32 white = SDL_MapRGB(screen -> format, 0xFF, 0xFF, 0xFF);
    int rectangleHeight = WINDOW_HEIGHT / numRows, rectangleWidth = WINDOW_WIDTH / numCols;
    rectangle.x = 0, rectangle.y = 0, rectangle.h = rectangleHeight, rectangle.w = rectangleWidth;
    
    bool exitFound = false;
    
    //Base condition: return when exit found
    if(maze[y] [x] == ' ' && isPossibleExit(y, x, numRows, numCols)) {
        exitFound = true;
        maze[y] [x] = '.'; //mark exit
        
        rectangle.x = x * rectangleWidth, rectangle.y = y * rectangleHeight;
        SDL_FillRect(screen, &rectangle, yellow);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(200);
        
        return exitFound;
    }
    
    //Mark current position so that we cannot return to it
    maze[y] [x] = '.';
    
    rectangle.x = x * rectangleWidth, rectangle.y = y * rectangleHeight;
    SDL_FillRect(screen, &rectangle, yellow);
    SDL_UpdateWindowSurface(window);
    SDL_Delay(200);
    
    
    //Check north and call recursively with new coordinates if can go
    if(validIndex(y - 1, x, numRows, numCols) && (maze[y - 1] [x] == ' ')) {
        rectangle.x = x * rectangleWidth, rectangle.y = (y - 1) * rectangleHeight;
        SDL_FillRect(screen, &rectangle, yellow);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(200);
        if(runGraphicsSimulation(maze, numRows, numCols, y - 1, x, screen, window)) {
            return 1;
        }
    }
    
    //Check east and call recursively with new coordinates if can go
    if(validIndex(y, x + 1, numRows, numCols) && (maze[y] [x + 1] == ' ')) {
        rectangle.x = (x + 1) * rectangleWidth, rectangle.y = y * rectangleHeight;
        SDL_FillRect(screen, &rectangle, yellow);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(200);
        if(runGraphicsSimulation(maze, numRows, numCols, y, x + 1, screen, window)) {
            return 1;
        }
    }
    
    //Check south and call recursively with new coordinates if can go
    if(validIndex(y + 1, x, numRows, numCols) && (maze[y + 1] [x] == ' ')) {
        rectangle.x = x * rectangleWidth, rectangle.y = (y + 1) * rectangleHeight;
        SDL_FillRect(screen, &rectangle, yellow);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(200);
        if(runGraphicsSimulation(maze, numRows, numCols, y + 1, x, screen, window)) {
            return 1;
        }
    }
    
    //Check west and call recursively with new coordinates if can go
    if(validIndex(y, x - 1, numRows, numCols) && (maze[y] [x - 1] == ' ')) {
        rectangle.x = (x - 1) * rectangleWidth, rectangle.y = y * rectangleHeight;
        SDL_FillRect(screen, &rectangle, yellow);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(200);
        if(runGraphicsSimulation(maze, numRows, numCols, y, x - 1, screen, window)) {
            return 1;
        }
    }
    
    //Dead end reached, mark with space character again before returning and
    //exploring other avenues.
    if(!isPossibleExit(y, y, numRows, numCols)) {
        maze[y][x] = ' ';
        rectangle.x = x * rectangleWidth, rectangle.y = y * rectangleHeight;
        SDL_FillRect(screen, &rectangle, white);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(200);
    }
    
    return exitFound;
}

void generateRandomMaze(char maze[][NUM_COLS], int yStart, int yEnd, int xStart, int xEnd) {
    //Create horizontal or vertical wall at random in empty maze, ensuring that each wall
    //has an opening.
    
    int width = xEnd - xStart;
    int height = yEnd - yStart;
    
    //Base condition: too small to add any more walls
    if(width < 1 || height < 1) {
        return;
    }
    
    if(makeHorizontal(height, width)) {
        int randomRow = getRandomNumber(yEnd, yStart), randomPassage;
        
        for(int col = xStart; col < xEnd; col++) {
            maze[randomRow][col] = '#';
        }
        
        randomPassage = getRandomNumber(xEnd, xStart);
        maze[randomRow] [randomPassage] = ' ';
        
        generateRandomMaze(maze, yStart, randomRow - 1, xStart, xEnd);
        generateRandomMaze(maze, randomRow + 1, yEnd, xStart, xEnd);
    } else {
        int randomColumn = getRandomNumber(xEnd, xStart), randomPassage;
        
        for(int row = yStart; row < yEnd; row++) {
            maze[row][randomColumn] = '#';
        }
        
        randomPassage = getRandomNumber(yEnd, yStart);
        maze[randomPassage] [randomColumn] = ' ';
        
        generateRandomMaze(maze, yStart, yEnd, xStart, randomColumn - 1);
        generateRandomMaze(maze, yStart, yEnd, randomColumn + 1, xEnd);
    }
}

bool makeHorizontal(int height, int width) {
    //If the maze section is wider than it is tall, splitting it
    //vertically generates a better maze
    
    if(width < height) {
        return true;
    }
    else if (height < width) {
        return false;
    }
    else {
        int randomNumber = rand() % 2;
        return randomNumber == 0;
    }
}

int getRandomNumber(int max, int min) {
    //Generate random number between starting coordinate and end coordinate
    //of maze section
    
    int randomNumber = rand() % (max - min) + min;
    return randomNumber;
}

void initializeRandomMaze(char maze[][NUM_COLS]) {
    for(int i = 0; i < NUM_ROWS; i++) {
        for(int j = 0; j < NUM_COLS; j++) {
            maze[i] [j] = ' ';
        }
    }
    
    //Surround maze with a wall
    
    for(int i = 0; i < NUM_ROWS; i++) {
        for(int j = 0; j < NUM_COLS; j++) {
            if(i == 0) {
                maze[i][j] = '#';
            }
            if(j == 0) {
                maze[i] [j] = '#';
            }
            
            if(i == NUM_ROWS - 1) {
                maze[i] [j] = '#';
            }
            
            if(j == NUM_COLS - 1) {
                maze[i] [j] = '#';
            }
        }
    }
}

void createEntryandExit(char maze[][NUM_COLS]) {
    int randomEntrance = rand() % NUM_ROWS, randomExit = rand() % NUM_ROWS;
    
    //Make sure that nothing is blocking the entrance or the exit before creating it
    while (maze[randomEntrance] [1] == '#') {
        randomEntrance = rand() % NUM_ROWS;
    }
    
    maze[randomEntrance] [0] = ' ';
    
    while (maze[randomExit] [NUM_COLS - 2] == '#') {
        randomEntrance = rand() % NUM_ROWS;
    }
    
    maze[randomEntrance] [NUM_COLS - 1] = ' ';
}


