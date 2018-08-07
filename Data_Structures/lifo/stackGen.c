#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static size_t sizeEl = 1, nEl = 0;
static unsigned char * stack = NULL;
static int top = -1;

void quit(void) {
	if (stack)
		free(stack);
}	

void stackInit(size_t s) {
	sizeEl = s;

	if (stack)
		free(stack);
	else
		atexit(quit);

	stack = malloc((nEl = 1024) * sizeEl * sizeof *stack);
	assert(stack);
	top = -1;
}

void * pop(void) {
	return (void *)&stack[sizeEl * top--];
}


int empty(void) {
	return top < 0;
}

void push(void * data) {
	if (top == (nEl-1)) {
		stack = realloc(stack, (nEl *= 2) * sizeEl * sizeof *stack);
		assert(stack);
	}

	memcpy((void*)&stack[sizeEl * (++top)], data, sizeEl);
}