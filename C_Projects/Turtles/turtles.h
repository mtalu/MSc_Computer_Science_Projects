//
//  turtles.h
//  turtles
//
//  Created by Michael on 19/01/2015.
//  Copyright (c) 2015 Michael. All rights reserved.
//

#ifndef turtles_turtles_h
#define turtles_turtles_h

/*---------- Standard Headers -----------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>

/*---------- Custom Headers	-----------*/
#include "my_sdl_setup.h"
#include "stack.h"

/*---------- Data Types -----------*/
typedef struct turtle{
    double xPos, yPos, newXPos, newYPos, angle;
    bool visible;
} Turtle;

typedef struct varNode {
    char key;
    double value;
    struct varNode *next;
} VarNode;

typedef struct varList {
    struct varNode *start;
} VarList;

typedef enum operator {NOT_OPERATOR, PLUS, MINUS, TIMES, DIVIDED} Operator;

/*---------- Hash Defines -----------*/
#define DEGREE_TO_RADIAN(a) (a * 0.017453292519)
#define ROUND_TO_INT(f) ((int)(f >= 0.0 ? (f + 0.5) : (f - 0.5)))
#define INFIX 1

/*----------Function Prototypes (External)-----------*/
char **tokenize(FILE *sourceFile, int *numberOfTokens);
char *getToken(char *tempToken);
void *myMalloc(size_t size, char *functionName);
void freeTokens(char **tokens, int numberOfTokens);
int parse(char **start, char **tokens, int numberOfTokens);
int parseFD(char *nextToken);
int parseLT(char *nextToken);
int parseRT(char *nextToken);
int parseSET(char **tokens, char **start, int numberOfTokens);
int parseDO(char **start, char **tokens, int numberOfTokens);
bool isNumber(char *token);
void handleCommandError(char *errorMessage);
void initializeTurtle(void);
Turtle *getTurtle(Turtle *newTurtle);
void executeFD(char *nextToken);
int executeLT(char *nextToken);
int executeRT(char *nextToken);
Operator isOperator(char *token);
double operation(double a, double b, Operator operator);
VarList *getVarlist(void);
void addToVarList(char key, double value);
void destroyVarList(void);
void printVarList(void);
bool alreadyInList(char key);
bool isValidKey(char *token);
bool keyInList(char key);
double getKeyValue(char key);
void calculateNewPosition(double distance);
bool isInt(char *token);
void initGraphics(void);
void setKeyValue(char key, double value);
FILE *openFile(const char *fileName);
bool tokenExists(char **token, char **start, int numberOfTokens);
void freeTurtle(void);
int calculateRPNValue(char **token, char **start, int numberOfTokens, double *value);
int calculateInfixValue(char **token, int numberOfTokens, double *value);

#endif
