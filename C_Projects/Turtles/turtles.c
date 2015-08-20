//
//  main.c
//  turtles
//
//  Created by Michael on 06/01/2015.
//  Copyright (c) 2015 Michael. All rights reserved.
//
/*******************************************************************************************************************************
 
 MAIN PROGRAM
 
 This program reads a turtle file and executes the instructions, displaying output via SDL. The turtle script is read into an array of character pointers using the tokenize function. The parse function then reads from this array and checks to ensure the grammar is correct before executing the instructions and displaying them via SDL.
 
 The program should be run with the turtle file as its first argument; alternatively it can be run with TESTING as its first argument, which will run a series of test suites (written using the Sput test harness in the Sput.h header file) and show the results in stderr. SDL output can also be switched off at any time for testing purposes by running the program with SDL_OFF as its second argument.
 
 EXTENSION
 
 For my extension I have written an infix notation converter for the user's convenience when setting variables in a turtle script. This will allow you to use standard mathematical expressions, e.g. 2 * 2 + 3 * 8, as well as brackets (There is no support for exponentiation). The infix converter uses the Shunting-yard algorithm to store the infix notation in a binary tree, which is then converted to RPN notation via post-order traversal of the tree. To switch infix notation on, the INFIX macro in turtles.h should be set to 1.
 
 TURTLE SCRIPT NOTES
 
 Variables used in the script must have been set using the SET keyword prior to use, otherwise it is an error. Using the SET keyword with a previously set variable will overwrite the value of the previous variable. There is no scope in a turtle program, variables set within a DO loop or a DO loop header remain active when the loop ends and can still be used throughout the remainder of the turtle script.
 ********************************************************************************************************************************/
//

/*---------- Custom Headers	-----------*/
#include "turtles.h"
#include "testing.h"
#include "infix_converter.h"

/*---------- Global Variables -----------*/
bool SDL_OFF = false; //Used by quite a few functions, so felt it was appropriate to have global variable in this case

int main(int argc, const char *argv[]) {
    //Check if at least one command line argument given.
    if(argc < 2) {
        fprintf(stderr, "Error: No input file specified\n");
        exit(EXIT_FAILURE);
    }
    
    //First command line argument should be either input file or TESTING. Second command line argument can be SDL_OFF
    //to switch off graphics
    if(argc == 2 && strcmp(argv[1], "TESTING") == 0) {
        SDL_OFF = true;
        beginTesting();
        exit(EXIT_SUCCESS);
    }
    else if(argc == 3 && strcmp(argv[2], "SDL_OFF") == 0) {
        SDL_OFF = true;
    }
    else {
        initGraphics();
    }
    
    int numberOfTokens = 0;
    FILE *sourceFile = openFile(argv[1]);
    char **tokens = tokenize(sourceFile, &numberOfTokens); //tokens array will hold all "words" after tokenizing.
    int errorCode = parse(tokens, tokens, numberOfTokens);
    if(errorCode == -1) {
        handleCommandError("Failed to parse turtle file\n");
        exit(EXIT_FAILURE);
    }
    
    if(!SDL_OFF) {
        mySDL_renderPresent();
    
        bool quit = false;
        SDL_Event e;
        while(!quit) {
            while(SDL_PollEvent(&e) != 0) {
                if(e.type == SDL_QUIT) {
                    quit = true;
                }
            }
        }
    
        mySDL_quitSDL();
        Turtle *turtle = getTurtle(NULL);
        free(turtle);
    }
    
    destroyVarList();
    freeTokens(tokens, numberOfTokens);
    
    return 0;
}

/**
 Read every token in input file into string array, skipping over whitespace characters.
 Dynamically reallocate space for every new token.
 */
char **tokenize(FILE *sourceFile, int *numberOfTokens) {
    char **tokens = NULL;
    char tempToken[200];
    
    while(fscanf(sourceFile, "%s", tempToken) == 1) {
        char **tmp = (char **) realloc(tokens, sizeof(char *) * (*numberOfTokens + 1));
        if(tmp == NULL) {
            fprintf(stderr, "Error: Realloc failed in tokenize()\n");
            exit(EXIT_FAILURE);
        }
        tokens = tmp;
        tokens[*numberOfTokens] = getToken(tempToken);
        (*numberOfTokens)++;
    }
    
    return tokens;
}

/**
 Create dynamically allocated string for every token
 */
char *getToken(char *tempToken) {
    char *token = (char *) malloc((1+strlen(tempToken)) * sizeof(char));//myMalloc(strlen(tempToken) * sizeof(char), "getToken");
    strcpy(token, tempToken);
    
    return token;
}

/**
 Free all string tokens and the tokens array
 */
void freeTokens(char **tokens, int numberOfTokens) {
    for(int i = 0; i < numberOfTokens; i++) {
        free(tokens[i]);
    }
    
    free(tokens);
}

/**
 Trivial wrapper for malloc, handles errors
 */
void *myMalloc(size_t size, char *functionName) {
    void *mallocedSpace = (void *) malloc(size);
    if(!mallocedSpace) {
        fprintf(stderr, "Error: Malloc failed in %s()\n", functionName);
        exit(EXIT_FAILURE);
    }
    
    return mallocedSpace;
}

/**
 Parse tokens to check if grammar is correct and display output if not switched
 off. Returns -1 on command error or tokens scanned during current call
 */
int parse(char **start, char **tokens, int numberOfTokens) {
    if(numberOfTokens == 0) {
        handleCommandError("Turtle file appears to be empty\n");
        return -1;
    }
    
    char **p = tokens;
    int tokensScanned, totalTokensScanned; //tokens scanned during current call and all tokens scanned since start
    int errorCode = 0;

    if(!tokenExists(p, start, numberOfTokens) || strcmp(*p++, "{") != 0) {
        handleCommandError("Expected initial brace\n");
        return -1;
    }
    
    for( ; tokenExists(p, start, numberOfTokens) && strcmp(*p, "}") != 0; p++) {        
        if(strcmp(*p, "FD") == 0) {
            if(!tokenExists(++p, start, numberOfTokens)) {
                handleCommandError("FD expected a number or valid variable\n");
                return -1;
            }
            errorCode = parseFD(*p);
        }
        else if(strcmp(*p, "LT") == 0) {
            if(!tokenExists(++p, start, numberOfTokens)) {
                handleCommandError("LT expected a number or valid variable\n");
                return -1;
            }
            errorCode = parseLT(*p);
        }
        else if(strcmp(*p, "RT") == 0) {
            if(!tokenExists(++p, start, numberOfTokens)) {
                handleCommandError("RT expected a number or valid variable\n");
                return -1;
            }
            errorCode = parseRT(*p);
        }
        else if(strcmp(*p, "SET") == 0) {
            tokensScanned = errorCode = parseSET(++p, start, numberOfTokens);
            p += tokensScanned;
        }
        else if(strcmp(*p, "DO") == 0) {
            tokensScanned = errorCode = parseDO(start, ++p, numberOfTokens);
            p += tokensScanned;
        }
        else{
            handleCommandError("Unrecognized command or symbol encountered\n");
            return -1;
        }
        
        if(errorCode == -1) {
            return errorCode;
        }
    }
    
    tokensScanned = (int) (p - tokens);
    totalTokensScanned = (int) (p - start);
    
    if(!tokenExists(p, start, numberOfTokens) || strcmp(*p++, "}") != 0) {
        handleCommandError("Expected final brace\n");
        return -1;
    }
    
    return tokensScanned;
}

/**
 Parse FD command and deal with errors
 */
int parseFD(char *nextToken) {
    if(!isNumber(nextToken) && (!isValidKey(nextToken) || !keyInList(nextToken[0]))) {
        handleCommandError("FD expected a number or valid variable\n");
        return -1;
    }
    
    if(!SDL_OFF) {
        executeFD(nextToken);
    }
    
    return 0;
}

/**
 Parse LT command and deal with errors
 */
int parseLT(char *nextToken) {
    if(!isNumber(nextToken) && (!isValidKey(nextToken) || !keyInList(nextToken[0]))) {
        handleCommandError("LT expected a number or valid variable\n");
        return -1;
    }
    
    if(!SDL_OFF) {
        executeLT(nextToken);
    }
    
    return 0;
}

/**
 Parse RT command and deal with errors
 */
int parseRT(char *nextToken) {
    if(!isNumber(nextToken) && (!isValidKey(nextToken) || !keyInList(nextToken[0]))) {
        handleCommandError("RT expected a number or valid variable\n");
        return -1;
    }
    
    if(!SDL_OFF) {
        executeRT(nextToken);
    }
    
    return 0;
}

/**
 Parse DO command and deal with erros
 */
int parseDO(char **start, char **tokens, int numberOfTokens) {
    char **p = tokens; //Pointer to first token after do, start pointer needed later to keep track of number scanned
    int tokensScanned = 0;
    char key;
    int value;
    int loopEnd;
    int errorCode = 0;
    
    if(!tokenExists(p, start, numberOfTokens) || !isValidKey(*p)) {
        handleCommandError("DO expected a valid variable name, i.e. an uppercase letter between A-Z\n");
        return -1;
    }
    
    key = *p[0]; //Key or variable is first character of token, isValidKey function ensures it is only actually one char long
    
    if(!tokenExists(p, start, numberOfTokens) || strcmp(*(++p), "FROM") != 0) {
        handleCommandError("DO expected the FROM keyword after variable\n");
        return -1;
    }
    
    if(!tokenExists(p, start, numberOfTokens) || !isInt(*(++p))) {
        handleCommandError("DO expected a valid integer after FROM keyword\n");
        return -1;
    }
    
    value = atoi(*p);
    
    if(!tokenExists(p, start, numberOfTokens) || strcmp(*(++p), "TO") != 0) {
        handleCommandError("DO expected the TO keyword after first integer\n");
        return -1;
    }
    
    if(!tokenExists(p, start, numberOfTokens) || !isInt(*(++p))) {
        handleCommandError("DO expected a valid integer after TO keyword\n");
        return -1;
    }
    
    loopEnd = atoi(*p++);
    
    //Add variable to list or change value if it already exists
    if(!keyInList(key)) {
        addToVarList(key, value);
    } else {
        setKeyValue(key, value);
    }
    
    for(int i = getKeyValue(key); i <= loopEnd; i++) {
        if(errorCode == -1) {
            return -1;
        }
        
        tokensScanned = errorCode = parse(start, p, numberOfTokens);
        setKeyValue(key, (double) i); //Reset variable value as i increases
    }
    
    tokensScanned += p - tokens;
    
    return tokensScanned;
}

/**
 Parse SET command and deal with errors
 */
int parseSET(char **tokens, char **start, int numberOfTokens) {
    char **p = tokens; //Pointer to first token after SET, tokens pointer needed later to keep track of number scanned
    int tokensScanned, tempTokensScanned;
    char key = (*p)[0];
    double value;
    
    if(!tokenExists(p, start, numberOfTokens) || !isValidKey(*p++)) {
        handleCommandError("SET expected a valid variable name, i.e. an uppercase letter between A-Z\n");
        return -1;
    }
    
    if(!tokenExists(p, start, numberOfTokens) || strcmp(*p++, ":=") != 0) {
        handleCommandError("SET expected an assignment \":=\" operator\n");
        return -1;
    }
    
    if(INFIX) {
        tempTokensScanned = calculateInfixValue(p, numberOfTokens, &value);
        if(tempTokensScanned == -1) {
            return -1;
        }
        
    }
    else {
        tempTokensScanned = calculateRPNValue(p, start, numberOfTokens, &value);
        if(tempTokensScanned == -1) {
            return -1;
        }
        

    }
    
    p += tempTokensScanned;
    
    
    if(!tokenExists(p, start, numberOfTokens) || strcmp(*p, ";") != 0) {
        handleCommandError("SET expected a semi-colon \";\" after the RPN expression\n");
        return -1;
    }
    
    if(!keyInList(key)) {
        addToVarList(key, value);
    } else {
        setKeyValue(key, value);
    }
    
    tokensScanned = (int) (p - tokens); //Calculate tokens scanned by subtracting start position from current pointer position.
    return tokensScanned;
}

/**
 Check if token is valid double
 */
bool isNumber(char *token) {
    double a;
    char dummy[100];
    
    if(sscanf(token, "%lf%s", &a, dummy) != 1) {
        return false;
    }
    
    return true;
}

/**
 Handles errors encountered while parsing individual commands. Messages
 can later be displayed via SDL instead of stderr
 */
void handleCommandError(char *errorMessage) {
    fprintf(stderr, "\nError: %s\n", errorMessage);
}

/**
 Initializes a new turtle and stores pointer to it in getTurtle function.
 */
void initializeTurtle(void) {
    Turtle *newTurtle = (Turtle *) myMalloc(sizeof(Turtle), "main");
    
    newTurtle->xPos = WINDOW_WIDTH / 2;
    newTurtle->yPos = WINDOW_HEIGHT / 2;
    newTurtle->angle = 0;
    newTurtle->visible = true;
    
    getTurtle(newTurtle);
}


/**
 Return pointer to stored turtle, must be initialized via initializeTurtle function
 */
Turtle *getTurtle(Turtle *newTurtle) {
    static Turtle *turtle = NULL;
    
    if(newTurtle != NULL) {
        turtle = newTurtle;
    }
    
    return turtle;
}

/**
 Execute FD command displaying output with SDL
 */
void executeFD(char *nextToken) {
    Turtle *turtle = getTurtle(NULL);
    Display *d = mySDL_getDisplay(NULL);
    
    double distance;
    
    if(!isValidKey(nextToken)) {
        sscanf(nextToken, "%lf", &distance);
    }
    else {
        distance = getKeyValue(nextToken[0]);
    }
    
    calculateNewPosition(distance);
    
    SDL_RenderDrawLine(d->renderer, ROUND_TO_INT(turtle->xPos), ROUND_TO_INT(turtle->yPos), ROUND_TO_INT(turtle->newXPos), ROUND_TO_INT(turtle->newYPos));
    
    turtle->xPos = turtle->newXPos;
    turtle->yPos = turtle->newYPos;
}

/**
 Execute LT command displaying output with SDL
 */
int executeLT(char *nextToken) {
    Turtle *turtle = getTurtle(NULL);
    
    double angle;
    
    if(!isValidKey(nextToken)) {
        sscanf(nextToken, "%lf", &angle);
    }
    else {
        angle = getKeyValue(nextToken[0]);
    }

    turtle->angle = turtle->angle - ((int) angle % 360);
    
    if(turtle->angle < 0) {
        turtle->angle = 360 + turtle->angle;
    }
    
    return turtle->angle;
}

/**
 Execute RT command displaying output with SDL
 */
int executeRT(char *nextToken) {
    Turtle *turtle = getTurtle(NULL);
    
    double angle;
    
    if(!isValidKey(nextToken)) {
        sscanf(nextToken, "%lf", &angle);
    }
    else {
        angle = getKeyValue(nextToken[0]);
    }

    turtle->angle = (int) (turtle->angle + angle) % 360;
        
    if(turtle->angle < 0) {
        turtle->angle = 360 + turtle->angle;
    }
    
    return turtle->angle;
}

/**
 Return enum of operator if operator, otherwise NOT_OPERATOR (NOT_OPERATOR also has value 0)
 */
Operator isOperator(char *token) {
    if(strcmp(token, "+") == 0) {
        return PLUS;
    }
    else if(strcmp(token, "-") == 0) {
        return MINUS;
    }
    else if(strcmp(token, "*") == 0) {
        return TIMES;
    }
    else if(strcmp(token, "/") == 0) {
        return DIVIDED;
    }
    else {
        return NOT_OPERATOR;
    }
}

/**
 Calculate and return sum of two double operands
 */
double operation(double a, double b, Operator operator) {
    double sum;
    
    if(operator == PLUS) {
        sum = a + b;
    }
    else if(operator == MINUS) {
        sum = a - b;
    }
    else if(operator == TIMES) {
        sum = a * b;
    }
    else {
        sum = a / b;
    }
    
    return sum;
}

/**
 Initialize variable list when called the first time, return
 static pointer to variable list every subsequent call
 */
VarList *getVarlist(void) {
    static VarList *varList;
    static bool initialized = false;
    
    if(!initialized) {
        varList = (VarList *) myMalloc(sizeof(VarList), "initializeVarlist");
        varList->start = NULL;
        initialized = true;
    }
    
    return varList;
}

/**
 Creates node for given key and value and adds to varList
 */
void addToVarList(char key, double value) {
    VarList *varList = getVarlist();
    VarNode *p;
    
    VarNode *varNode = (VarNode *) myMalloc(sizeof(VarNode), "createVarNode");
    varNode->key = key;
    varNode->value = value;
    varNode->next = NULL;
    
    if(varList->start == NULL) {
        varList->start = varNode;
        return;
    }
    
    for(p = varList->start; p->next != NULL; p = p->next) {
        //Find last node in list
    }
    
    p->next = varNode;
}

void destroyVarList(void) {
    VarList *varList = getVarlist();
    
        for(VarNode *p = varList->start; p != NULL;) {
            VarNode *temp = p;
            p = p->next;
            free(temp);
        }
        
        free(varList);
}

void printVarList(void) {
    VarList *list = getVarlist();
    for(VarNode *p = list->start; p != NULL; p = p->next) {
        printf("Key: %c, Value: %lf\n", p->key, p->value);
    }
}

/**
 Checks if variable is an upper case letter between A-Z
 */
bool isValidKey(char *token) {
    char key;
    
    if(strlen(token) > 1) {
        return false;
    }
    
    key = token[0];
    
    if(key < 'A' || key > 'Z') {
        return false;
    }
    
    return true;
}

/**
 Checks if variable is already in list
 */
bool keyInList(char key) {
    VarList *list = getVarlist();
    
    for(VarNode *p = list->start; p != NULL; p = p->next) {
        if(p->key == key) {
            return true;
        }
    }
    
    return false;
}

double getKeyValue(char key) {
    VarList *list = getVarlist();
    double value = -1;
    
    for(VarNode *p = list->start; p != NULL; p = p->next) {
        if(p->key == key) {
            value = p->value;
        }
    }
    
    return value;
}

/**
 Use current angle of turtle and distance travelled to calculate new coordinates
 */
void calculateNewPosition(double distance) {
    Turtle *turtle = getTurtle(NULL);
    
    turtle->newXPos = turtle->xPos + distance * sin(DEGREE_TO_RADIAN(turtle->angle));
    turtle->newYPos = turtle->yPos - distance * cos(DEGREE_TO_RADIAN(turtle->angle));
}

/**
Check if token is valid integer, using dummy to make sure it is not followed by anything
*/
bool isInt(char *token) {
    int a;
    char dummy[100];
    
    if(sscanf(token, "%d%s", &a, dummy) != 1) {
        return false;
    }
    
    return true;
}

/**
 Trivial wrapper to initialize turtle and set up SDL
 */
void initGraphics(void) {
    initializeTurtle();
    
    mySDL_initializeDisplay();
    mySDL_setDrawColour(0, 0, 0);
}

void setKeyValue(char key, double value) {
    VarList *list = getVarlist();
    
    for(VarNode *p = list->start; p != NULL; p = p->next) {
        if(p->key == key) {
            p->value = value;
        }
    }
}

/**
 Trivial wrapper, opens source file in read mode and return file point to it
 */
FILE *openFile(const char *fileName) {
    FILE *fp = fopen(fileName, "r");
    if(fp == NULL) {
        fprintf(stderr, "Error: Could not open file\n");
        exit(EXIT_FAILURE);
    }
    
    return fp;
}

/**
 Returns true if next token exists. Ignores the last token which is assumed to be a final
 brace until tested at the end.
 */
bool tokenExists(char **token, char **start, int numberOfTokens) {
    if((token - start) < numberOfTokens) {
        return true;
    }
    
    return false;
}

void freeTurtle(void) {
    Turtle *turtle = getTurtle(NULL);
    
    free(turtle);
}

/**
 Calculate value of RPN expression by adding numbers to stack and performing operation at every operator
 */
int calculateRPNValue(char **token, char **start, int numberOfTokens, double *value) {
    Stack s = stack_create();
    getVarlist();
    char **p = token;
    
    while(tokenExists(p, start, numberOfTokens) && (isNumber(*p) || isOperator(*p) || isValidKey(*p))) {
        if(isNumber(*p)) {
            double number;
            sscanf(*p++, "%lf", &number);
            stack_push(s, number);
        }
        else if(isOperator(*p)) {
            Operator operator = isOperator(*p++);
            if(!(stack_getNumberOfItems(s) >= 2)) {
                handleCommandError("SET expected a valid RPN expression. Please check expression for errors\n");
                return -1;
            }
            double b = stack_pop(s);
            double a = stack_pop(s);
            stack_push(s, operation(a, b, operator));
        }
        else {
            double number = getKeyValue((*p++)[0]);
            if(number == -1) {
                handleCommandError("SET expected a valid variable. Please ensure that variable has been set\n");
                return -1;
            }
            stack_push(s, number);
        }
    }
    
    //Number of items on stack at the end of operation should equal one, otherwise RPN expression must be wrong
    if(stack_getNumberOfItems(s) != 1) {
        handleCommandError("SET expected a valid RPN expression. Please check expression for errors\n");
        return -1;
    }
    
    *value = stack_pop(s);
    stack_destroy(s);
    
    return (int) (p - token);
}

int calculateInfixValue(char **token, int numberOfTokens, double *value) {
    int tokensScanned = 0, totalRPNtokens = 0;
    char **RPNFromInfix;
    
    Node *expressionTree = parseInfixExpression(token, numberOfTokens, &tokensScanned);
    if(expressionTree == NULL) {
        handleCommandError("SET expected a valid infix expression after the assignment \":=\" operator ");
        return -1;
    }
    
    RPNFromInfix = getReversePolish(expressionTree, &totalRPNtokens);
    
    if(calculateRPNValue(RPNFromInfix, RPNFromInfix, totalRPNtokens, value) == -1) {
        return -1;
    };
    
    return tokensScanned;
}

