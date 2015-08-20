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
#include "stack_generic.h"

/*----------Data Types-----------*/
struct node {
    void *data;
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

Stack stack_generic_create(void) {
    Stack s = (Stack) malloc(sizeof(struct stack_type));
    if(s == NULL) {
        terminate("Error: Malloc failed in create()");
    }
    
    s->top = NULL;
    s->count = 0;
    
    return s;
}

void stack_generic_destroy(Stack s) {
    stack_generic_makeEmpty(s);
    free(s);
}

void stack_generic_makeEmpty(Stack s) {
    while(!stack_generic_isEmpty(s)) {
        stack_generic_pop(s);
    }
}

bool stack_generic_isEmpty(Stack s) {
    return s->top == NULL;
}

bool stack_generic_isFull(Stack s) {
    return false;
}

void stack_generic_push(Stack s, void *i) {
    struct node *newNode = (struct node *) malloc(sizeof(struct node));
    if(newNode == NULL) {
        terminate("Error: Malloc failed in push()");
    }
    
    newNode->data = i;
    newNode->next = s->top;
    s->top = newNode;
    s->count += 1;
}

void *stack_generic_pop(Stack s) {
    struct node *oldTop;
    void *i;
    
    if(stack_generic_isEmpty(s)) {
        terminate("Error in pop(): Stack is empty");
    }
    
    oldTop = s->top;
    i = oldTop->data;
    s->top = oldTop->next;
    free(oldTop);
    s->count -= 1;
    return i;
}

int stack_generic_getNumberOfItems(Stack s) {
    return s->count;
}

void *stack_generic_peek(Stack s) {
    return s->top->data;
}
