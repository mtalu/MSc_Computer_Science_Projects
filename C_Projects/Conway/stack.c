//
//  stack.c
//  conway
//
//  Created by Michael on 05/12/2014.
//  Copyright (c) 2014 Michael. All rights reserved.
//

#include "stack.h"

Stack *InitialiseStack()
{
    Stack *s = (Stack *) malloc(sizeof(Stack));
    
    if(s == NULL) {
        fprintf(stderr, "Error: Malloc failed in InitialiseStack()");
    }
    
    s->top = 0;
    
    return s;
}

void Push(Stack *s, struct node *n)
{
    /* Make sure stack doesnt overflow */
    assert(s->top < STACKSIZE);
    s->stk[s->top].node = n;
    s->top = s->top + 1;
}

struct node  *Pop(Stack *s)
{
    /* Can’t pop empty stack (underflow) */
    assert(s->top > 0);
    s->top = s->top - 1;
    return s->stk[s->top].node;
}

bool isEmpty(Stack *s) {
    return s->top == 0;
}


