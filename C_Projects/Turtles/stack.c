//
//  stack.c
//  my_stack
//
//  Created by Michael on 14/01/2015.
//  Copyright (c) 2015 Michael. All rights reserved.
//

/*----------Standard Headers-----------*/
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

/*----------Data Types-----------*/
struct node {
    Item data;
    struct node *next;
};

struct stack_type {
    struct node *top;
    int count;
};

/*----------Hash Defines-----------*/
#define STACK_SIZE 100


static void terminate(const char *message) {
    printf("%s\n", message);
    exit(EXIT_FAILURE);
}

Stack stack_create(void) {
    Stack s = (Stack) malloc(sizeof(struct stack_type));
    if(s == NULL) {
        terminate("Error: Malloc failed in create()");
    }
    
    s->top = NULL;
    s->count = 0;
    
    return s;
}

void stack_destroy(Stack s) {
    stack_makeEmpty(s);
    free(s);
}

void stack_makeEmpty(Stack s) {
    while(!stack_isEmpty(s)) {
        stack_pop(s);
    }
}

bool stack_isEmpty(Stack s) {
    return s->top == NULL;
}

bool stack_isFull(Stack s) {
    return false;
}

void stack_push(Stack s, Item i) {
    struct node *newNode = (struct node *) malloc(sizeof(struct node));
    if(newNode == NULL) {
        terminate("Error: Malloc failed in push()");
    }
    
    newNode->data = i;
    newNode->next = s->top;
    s->top = newNode;
    s->count += 1;
}

Item stack_pop(Stack s) {
    struct node *oldTop;
    Item i;
    
    if(stack_isEmpty(s)) {
        terminate("Error in pop(): Stack is empty");
    }
    
    oldTop = s->top;
    i = oldTop->data;
    s->top = oldTop->next;
    free(oldTop);
    s->count -= 1;
    return i;
}

int stack_getNumberOfItems(Stack s) {
    return s->count;
}
