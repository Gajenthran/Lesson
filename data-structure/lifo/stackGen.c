/*!
 * \file stackGen.c
 * \brief a generic FIFO (first-in-first-out) data structure
 * \author PANCHALINGAMOORTHY Gajenthran. Adapted from Farès Belhadj's course (code).
 * \date 7 August 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*! \brief the stack which will stock all the elements. */
static unsigned char * stack = NULL;
/*! \brief the size of an element of the stack. */
static size_t sizeEl = 1
/*! \brief the number of elements in the stack. */
static int nEl = 0;
/*! \brief the top of the stack is the index of the 
 * last pushed element. */
static int top = -1;

/*! \brief initialize the stack if it's not the case. */
void stackInit(size_t s) {
	sizeEl = s;

	if(stack)
		free(stack);
	else
		atexit(quit);

	stack = malloc((nEl = 1024) * sizeEl * sizeof *stack);
	assert(stack);
	top = -1;
}

/*! \brief push an element. And resize if the stack is full. */
void push(void * data) {
	if(top == (nEl-1)) {
		stack = realloc(stack, (nEl *= 2) * sizeEl * sizeof *stack);
		assert(stack);
	}

	memcpy((void *)&stack[sizeEl * (++top)], data, sizeEl);
}

/*! \brief pop an element. */
void * pop(void) {
	return (void *)&stack[sizeEl * top--];
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