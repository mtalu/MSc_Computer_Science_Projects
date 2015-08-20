//
//  infix_calculator.c
//  turtles
//
//  Created by Michael on 25/01/2015.
//  Copyright (c) 2015 Michael. All rights reserved.
//

/*---------- Custom Headers	-----------*/
#include "infix_converter.h"
#include "stack_generic.h"
#include "turtles.h"

/*----------Data Types-----------*/
struct tree_node {
    char *data;
    struct tree_node *left, *right;
};

/*----------Function Prototypes (Internal)-----------*/
Node *createTreeNode(char *data);
int operatorPrecedence(char *operator);
void traverseTree(Node *root, int *totalTokens, char **tokens);

/**
 Use Shunting-Yard algorithm to create binary expression tree from infix expression which can later be traversed
 in post-order form to get RPN expression.
 */
Node *parseInfixExpression(char **tokens, int numberOfTokens, int *infixTokensScanned) {
    Stack operators = stack_generic_create();
    Stack treeNodes = stack_generic_create();
    Node *treeRoot = NULL;
    char **p = tokens;
    
    //Parse token if it exists and is not a semi-colon
    for(; p - tokens < numberOfTokens && strcmp(*p, ";") != 0; p++) {
        if(isNumber(*p) || isValidKey(*p)) {
            stack_generic_push(treeNodes, createTreeNode(*p));
        }
        else if(isOperator(*p)) {
            while(!stack_generic_isEmpty(operators)) {
                if(operatorPrecedence(*p) <= operatorPrecedence(stack_generic_peek(operators))) {
                    Node *newNode = createTreeNode(stack_generic_pop(operators));
                    
                    if(!stack_generic_isEmpty(treeNodes)) {
                        newNode->right = stack_generic_pop(treeNodes);
                    }
                    if(!stack_generic_isEmpty(treeNodes)) {
                        newNode->left = stack_generic_pop(treeNodes);
                    }
                    stack_generic_push(treeNodes, newNode);
                }
                else {
                    break;
                }
            }
            stack_generic_push(operators, *p);
        }
        else if(strcmp(*p, "(") == 0) {
            stack_generic_push(operators, *p);
        }
        else if(strcmp(*p, ")") == 0) {
            while(!stack_generic_isEmpty(operators)) {
                if(strcmp(stack_generic_peek(operators), "(") == 0) {
                    break;
                }
                
                Node *newNode = createTreeNode(stack_generic_pop(operators));
                
                if(!stack_generic_isEmpty(treeNodes)) {
                    newNode->right = stack_generic_pop(treeNodes);
                }
                if(!stack_generic_isEmpty(treeNodes)) {
                    newNode->left = stack_generic_pop(treeNodes);
                }
                stack_generic_push(treeNodes, newNode);
            }
            if(stack_generic_isEmpty(operators) || strcmp(stack_generic_pop(operators), "(") != 0) {
                handleCommandError("Mismatched brackets in infix expressions");
                return treeRoot;
            }; //If stack is empty or operator is not left bracket, mismatched brackets.
        }
        else {
            handleCommandError("Unrecognized symbol detected in infix expression");
            return treeRoot;
        }
    }
    
    while(!stack_generic_isEmpty(operators)) {
        if(strcmp(stack_generic_peek(operators), "(") == 0 || strcmp(stack_generic_peek(operators), ")") == 0) {
            handleCommandError("Mismatched brackets in infix expressions");
            return treeRoot;
        } //If operator is left or right bracket it must mean mismatched brackets, otherwise would have been dealt with before
        
        Node *newNode = createTreeNode(stack_generic_pop(operators));
        
        if(!stack_generic_isEmpty(treeNodes)) {
            newNode->right = stack_generic_pop(treeNodes);
        }
        if(!stack_generic_isEmpty(treeNodes)) {
            newNode->left = stack_generic_pop(treeNodes);
        }
        stack_generic_push(treeNodes, newNode);
    }
    
    //The root node should be left on tree node stack after previous operations, if stack is empty there can't have been an expression
    if(stack_generic_isEmpty(treeNodes)) {
        handleCommandError("No expression detected after assignment \":=\" operator");
        return treeRoot;
    }
    
    treeRoot = stack_generic_pop(treeNodes);
    *infixTokensScanned = (int) (p - tokens);
    
    return treeRoot;
}

/**
 Create a tree node to hold token string
 */
Node *createTreeNode(char *data) {
    Node *newNode = (Node *) myMalloc(sizeof(Node), "createTreeNode");
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    
    return newNode;
}

/**
 Return precedence of an operator, operators with higher precedence have higher int value
 */
int operatorPrecedence(char *operator) {
    if(strcmp(operator, "+") == 0 || strcmp(operator, "-") == 0) {
        return 1;
    }
    
    if (strcmp(operator, "*") == 0 || strcmp(operator, "/") == 0) {
        return 2;
    }
    
    return -1;
}

/**
 Creates character pointer array to store converted RPN expression
 */
char **getReversePolish(Node *root, int *totalTokens) {
    char **reversePolish = (char **) myMalloc(sizeof(char *) * 100, "getReversePolish");
    
    traverseTree(root, totalTokens, reversePolish);
    
    return reversePolish;
    
}

/**
 Traverses binary expression tree in post-order to retrieve RPN expression, stores each token pointer
 in tokens array
 */
void traverseTree(Node *root, int *totalTokens, char **tokens) {
    if(root != NULL) {
        traverseTree(root->left, totalTokens, tokens);
        traverseTree(root->right, totalTokens, tokens);
        
        tokens[*totalTokens] = root->data;
        *totalTokens += 1;
    }
}
