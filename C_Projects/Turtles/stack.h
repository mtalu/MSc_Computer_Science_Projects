//
//  stack.h
//  my_stack
//
//  Created by Michael on 14/01/2015.
//  Copyright (c) 2015 Michael. All rights reserved.
//

#ifndef __my_stack__stack__
#define __my_stack__stack__

/*----------Standard Headers-----------*/
#include <stdio.h>
#include <stdbool.h>

/*----------Data Types-----------*/
typedef double Item;
typedef struct stack_type *Stack;

/*----------Function Prototypes (External)-----------*/
Stack stack_create(void);
void stack_destroy(Stack s);
void stack_makeEmpty(Stack s);
bool stack_isEmpty(Stack s);
bool stack_isFull(Stack s);
void stack_push(Stack s, Item i);
Item stack_pop(Stack s);
int stack_getNumberOfItems(Stack s);

#endif /* defined(__my_stack__stack__) */
