//
//  main.c
//  Conway.c
//
//  Created by Michael on 30/11/2014.
//  Copyright (c) 2014 Michael. All rights reserved.
//

#include "conway_extra.h"

int main(int argc, const char *argv[]) {
    Stack *solution;
    Node *initialBoard = createBoard();
    int targetX, targetY;
    
    getDestination(argc, argv, &targetX, &targetY);
    initializeBoard(initialBoard);
    solution = findSolution(initialBoard, targetX, targetY);
    //printSolution(solution, targetX, targetY);
    displaySolution(solution, targetX, targetY);
    
}

void getDestination(int argc, const char *argv[], int *x, int *y) {
    int argX, argY;
    
    if(argc != 3) {
        fprintf(stderr, "Error: Please enter two coordinates only\n");
        exit(EXIT_FAILURE);
    }
    
    argX = atoi(argv[1]);
    argY = NUM_ROWS - 1 - atoi(argv[2]); //Convert maths coordinates to array indeces
    
    if(argY >= 0 && argY < NUM_ROWS / 2 && argX >= 0 && argX < NUM_COLS) {
        *y = argY;
        *x = argX;
    }
    else {
        fprintf(stderr, "Error: Invalid coordinates\n");
        exit(EXIT_FAILURE);
    }
}

void initializeBoard(Node *board) {
    
    board->parent = board->next = NULL;
    
    for(int row = 0; row < NUM_ROWS; row++) {
        for(int col = 0; col < NUM_COLS; col++) {
            if(row >= 0 && row < NUM_ROWS / 2) {
                board->board[row][col] = 0;
            }
            else {
                board->board[row][col] = 1;
            }
        }
    }
}

void printBoard(int board[][NUM_COLS]) {
    for(int row = 0; row < NUM_ROWS; row++) {
        for(int col = 0; col < NUM_COLS; col++) {
            if(board[row][col] == 0) {
                printf(" %c ", 'O');
            }
            else if (board[row][col] == 1) {
                printf(" %c ", 'I');
            }
            else {
                printf(" %c ", 'X');
            }
        }
        printf("\n");
    }
}

Stack *findSolution(Node *initialBoard, int destX, int destY) {
    Stack *solution;
    Node *parent, *currentBoard;
    
    for(parent = currentBoard = initialBoard; parent != NULL && !isPeg(currentBoard, destY, destX); parent = parent->next) {
        currentBoard = getPossibleMoves(initialBoard, parent, currentBoard, destX, destY);
    }
    
    if(!isPeg(currentBoard, destX, destY)) {
        printf("It is not possible to reach this destination");
        exit(EXIT_SUCCESS);
    }
    
    solution = InitialiseStack();
    
    for( ;currentBoard != NULL; currentBoard = currentBoard->parent) {
        Push(solution, currentBoard);
    }
    
    return solution;
}

Node *getPossibleMoves(Node *initialBoard, Node *parent, Node *currentBoard, int destX, int destY) {
    
    for(int row = 0; row < 8; row++) {
        for(int col = 0; col < 7; col++) {
            if(isPeg(parent, row, col)) {
                if(canMove(UP, row, col, parent)) {
                    Node *newBoard = moove(UP, parent, row, col);
                    if(!inList(newBoard, initialBoard)) {
                        addToList(newBoard, parent, &currentBoard);
                    }
                    else {
                        free(newBoard);
                    }
                    
                    if(isPeg(currentBoard, destY, destX)) {
                        return currentBoard;
                    }
                    
                }
                
                if(canMove(LEFT, row, col, parent)) {
                    Node *newBoard = moove(LEFT, parent, row, col);
                    if(!inList(newBoard, initialBoard)) {
                        addToList(newBoard, parent, &currentBoard);
                    }
                    else {
                        free(newBoard);
                    }
                    
                    if(isPeg(currentBoard, destY, destX)) {
                        return currentBoard;
                    }
                }
                
                if(canMove(RIGHT, row, col, parent)) {
                    Node *newBoard = moove(RIGHT, parent, row, col);
                    if(!inList(newBoard, initialBoard)) {
                        addToList(newBoard, parent, &currentBoard);
                    }
                    else {
                        free(newBoard);
                    }
                    
                    if(isPeg(currentBoard, destY, destX)) {
                        return currentBoard;
                    }
                }
                
            }
            
        }
    }
    return currentBoard;
}

bool inList(Node *board, Node *startOfList) {
    Node *p;
    
    for(p = startOfList; p != NULL; p = p->next) {
        if(boardsAreEqual(board->board, p->board)) {
            return true;
        }
    }
    
    return false;
}

bool boardsAreEqual(int a1[][7], int a2[][7]) {
    for(int row = 0; row < 8; row++) {
        for(int col = 0; col < 7; col++) {
            if(a1[row][col] != a2[row][col]) {
                return false;
            }
        }
    }
    
    return true;
}

bool validIndex(int x, int y) {
    if(x >= 0 && x < NUM_COLS && y >= 0 && y < NUM_ROWS) {
        return true;
    }
    
    return false;
}

bool canMove(Direction direction, int row, int col, Node *board) {
    switch(direction) {
        case UP:
            return validIndex(col, row - 1) && board->board[row - 1][col] == 0 && validIndex(col, row + 1) && board->board[row + 1][col] == 1;
            break;
        case LEFT:
            return validIndex(col - 1, row) && board->board[row][col - 1] == 0 && validIndex(col + 1, row) && board->board[row][col + 1] == 1;
            break;
        case RIGHT:
            return validIndex(col + 1, row) && board->board[row][col + 1] == 0 && validIndex(col - 1, row) && board->board[row][col - 1] == 1;
            break;
    }
    
    return false;
}

Node *createBoard(void) {
    Node *newBoard = (Node *) malloc(sizeof(Node));
    
    if(newBoard == NULL) {
        fprintf(stderr, "Error: failure in createBoard() function");
        exit(EXIT_FAILURE);
    }
    
    return newBoard;
}

Node *moove(Direction direction, Node *parent, int row, int col) {
    Node *newBoard = createBoard();
    *newBoard = *parent;
    
    switch(direction) {
        case UP:
            newBoard->board[row][col] = newBoard->board[row + 1] [col] = 0;
            newBoard->board[row - 1] [col] = 1;
            break;
        case LEFT:
            newBoard->board[row][col] = newBoard->board[row] [col + 1] = 0;
            newBoard->board[row] [col - 1] = 1;
            break;
        case RIGHT:
            newBoard->board[row][col] = newBoard->board[row] [col - 1] = 0;
            newBoard->board[row] [col + 1] = 1;
            break;
    }
    
    return newBoard;
}

bool isPeg(Node *parent, int row, int col) {
    return parent->board[row][col] == 1;
}

void addToList(Node *newBoard, Node *parent, Node **currentBoard) {
    newBoard->parent = parent;
    newBoard->next = NULL;
    (*currentBoard)->next = newBoard;
    *currentBoard = newBoard;
    
    //printBoard((*currentBoard)->board);
    //printf("\n");
}

void printSolution(Stack *solution, int destX, int destY) {
    Node *node;
    int step = 0;
    
    printf("TARGET LOCATION\n\n");
    node = getDestinationBoard(destX, destY);
    printBoard(node->board);
    printf("\n");
    
    while(!isEmpty(solution)) {
        node = Pop(solution);
        printf("STEP %d\n\n", step++);
        printBoard(node->board);
        printf("\n");
    }

}

Node *getDestinationBoard(int destX, int destY) {
    Node *destination = createBoard();
    
    initializeBoard(destination);
    destination->board[destY][destX] = 2;
    
    return destination;
}

void displaySolution(Stack *solution, int destX, int destY) {
    Display *d = createDisplay();
    Node *board;
    int rectWidth, rectHeight;
    
    rectWidth = WINDOW_WIDTH / NUM_COLS, rectHeight = WINDOW_HEIGHT / NUM_ROWS,
    d->rectangle.x = 0, d->rectangle.y = 0, d->rectangle.h = rectHeight, d->rectangle.w = rectWidth;
    
    bool quit = false;
    SDL_Event e;
    bool showDestination = true;
    int count = 0;
    while(!quit) {
        if(showDestination) {
            board = getDestinationBoard(destX, destY);
            SDL_RenderClear(d->renderer);
            displayBoard(d, board);
            SDL_RenderPresent(d->renderer);
            SDL_Delay(500);
            SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(d->renderer);
            initializeBoard(board);
            displayBoard(d, board);
            SDL_RenderPresent(d->renderer);
            SDL_Delay(500);
            SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(d->renderer);
            
            count++;
            
            if(count == 3) {
                showDestination = false;
            }
        }
        else if(!isEmpty(solution)) {
            SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(d->renderer);
            board = Pop(solution);
            displayBoard(d, board);
            SDL_RenderPresent(d->renderer);
            SDL_Delay(1000);
        }
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
                quit = true;
            }
        }
    }
    
    quitSDL(d);
}

void displayBoard(Display *d, Node *board) {
    
    d->rectangle.x = 0, d->rectangle.y = 0;
    
    for(int row = 0; row < NUM_ROWS; row++) {
        for(int col = 0; col < NUM_COLS; col++) {
        
            SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawRect(d->renderer, &(d->rectangle));
            
            if (board->board[row][col] == 1) {
                SDL_SetRenderDrawColor(d->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
                renderFillCircle(d->renderer, d->rectangle.x + (d->rectangle.w / 2), d->rectangle.y + (d->rectangle.h / 2), 20);
            }
            else if (board->board[row][col] == 2) {
                SDL_SetRenderDrawColor(d->renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(d->renderer, &(d->rectangle));
            }
            
            d->rectangle.x += d->rectangle.w;
        }
        d->rectangle.x = 0, d->rectangle.y += d->rectangle.h;
    }


}
