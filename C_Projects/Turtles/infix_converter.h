//
//  infix_calculator.h
//  turtles
//
//  Created by Michael on 25/01/2015.
//  Copyright (c) 2015 Michael. All rights reserved.
//

#ifndef __turtles__infix_calculator__
#define __turtles__infix_calculator__

/*---------- Standard Headers -----------*/
#include <stdio.h>

/*---------- Data Types -----------*/
typedef struct tree_node Node;

/*----------Function Prototypes (External)-----------*/
Node *parseInfixExpression(char **tokens, int numberOfTokens, int *infixTokensScanned);
char **getReversePolish(Node *root, int *totalTokens);


#endif /* defined(__turtles__infix_calculator__) */
