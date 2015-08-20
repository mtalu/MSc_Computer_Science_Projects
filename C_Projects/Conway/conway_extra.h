//
//  conway.h
//  conway
//
//  Created by Michael on 05/12/2014.
//  Copyright (c) 2014 Michael. All rights reserved.
//

#ifndef conway_conway_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"
#include "SDL_Setup.h"

#define NUM_ROWS 8
#define NUM_COLS 7

typedef struct node {
    int board[NUM_ROWS][NUM_COLS];
    struct node *parent;
    struct node *next;
} Node;

typedef enum direction {UP, LEFT, RIGHT} Direction;

void getDestination(int argc, const char *argv[], int *x, int *y);
void initializeBoard(Node *board);
Stack *findSolution(Node *initialBoard, int destX, int destY);
Node *getPossibleMoves(Node *initialBoard, Node *parent, Node *currentBoard, int destX, int destY);
bool inList(Node *board, Node *startOfList);
bool boardsAreEqual(int a1[][NUM_COLS], int a2[][NUM_COLS]);
bool validIndex(int x, int y);
bool canMove(Direction direction, int row, int col, Node *board);
bool isPeg(Node *parent, int row, int col);
Node *moove(Direction direction, Node *parent, int row, int col);
Node *createBoard(void);
void printBoard(int board[][NUM_COLS]);
void addToList(Node *newBoard, Node *parent, Node **currentBoard);
void printSolution(Stack *solution, int destX, int destY);
Node *getDestinationBoard(int destX, int destY);
void displaySolution(Stack *solution, int destX, int destY);
void displayBoard(Display *d, Node *board);


#define conway_conway_h


#endif
