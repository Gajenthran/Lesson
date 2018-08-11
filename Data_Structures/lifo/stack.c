/*!
 * \file stack.c
 * \brief a LIFO (last-in-first-out) data structure
 * \author PANCHALINGAMOORTHY Gajenthran. Adapted from Farès Belhadj's course (code).
 * \date 4 August 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

/*! \brief the stack which will stock all the elements. */
static int * stack = NULL;
/*! \brief the actual size of the stack. */
static int size = 0;
/*! \brief the top of the stack is the index of the 
 * last pushed element. */
static int top = -1;

/*! \brief initialize the stack if it's not the case. Or
 * resize the queue. */
void initStack(void) {
	if(stack) {
		size *= 2;
		stack = realloc(stack, size);
	} 

	else {
		size = SIZE_STACK;
		stack = malloc(size * sizeof(*stack));
		assert(stack);
		top = -1;
		atexit(clean);
	}
}

/*! \brief push an element. And resize if the stack is full. */
void push(int val) {
	if(top > size-1)
		initStack();

	stack[++top] = val;
}

/*! \brief pop an element. */
int pop(void) {
	return stack[top--];
}

/*! check if the stack is empty. */
int empty(void) {
	return top < 0;
}

/*! \brief clean the stack. */
void clean(void) {
	if(stack)
		free(stack);
	top = -1;
}