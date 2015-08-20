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
Stack stack_generic_create(void);
void stack_generic_destroy(Stack s);
void stack_generic_makeEmpty(Stack s);
bool stack_generic_isEmpty(Stack s);
bool stack_generic_isFull(Stack s);
void stack_generic_push(Stack s, void *i);
void *stack_generic_pop(Stack s);
int stack_generic_getNumberOfItems(Stack s);
void *stack_generic_peek(Stack s);

#endif /* defined(__my_stack__stack__) */
