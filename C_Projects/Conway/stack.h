//
//  stack.h
//  conway
//
//  Created by Michael on 05/12/2014.
//  Copyright (c) 2014 Michael. All rights reserved.
//

#ifndef __conway__stack__

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#define STACKSIZE 200


struct stackelem{
    struct node *node;
};

typedef struct stackelem Elem;

struct thestack{
    Elem stk[STACKSIZE];
    int top;
};

typedef struct thestack Stack;

Stack *InitialiseStack();
void Push(Stack *s, struct node *n);
struct node *Pop(Stack *s);
bool isEmpty(Stack *s);

#define __conway__stack__

#endif /* defined(__conway__stack__) */
