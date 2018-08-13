/*!
 * \file queue.c
 * \brief a FIFO (first-in-first-out) data structure
 * \author PANCHALINGAMOORTHY Gajenthran. Adapted from Farès Belhadj's course (code).
 * \date 4 August 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "fifo.h"

/*! \brief the queue which will stock all the elements. */
static int * queue = NULL;
/*! \brief the actual size of the queue. */
static int size;
/*! \brief the bottom of the queue is the index of the 
 * last dequeued element. */
static int bottom = 0;
/*! \brief the top of the queue is the index of the 
 * last enqueued element. */
static int top = 0;

/*! \brief initialize the queue if it's not the case. Or
 * resize the queue. */
extern void initQueue(void) {
	if(queue) {
		size *= 2;
		queue = realloc(queue, size);
	} 

	else {
		size = SIZE_QUEUE;
		queue = malloc(size * sizeof(*queue));
		assert(queue);
		top = bottom = 0;
		atexit(clean);
	}
}

/*! \brief enqueue an element. And resize if the queue is full. */
extern void enqueue(int v) {
	queue[top++] = v;
	if(top > size-1)
		initQueue();
}

/*! \brief dequeue an element. */
extern int dequeue(void) {
	int v = queue[bottom++];
	if(bottom > size)
		bottom = 0;

	return v;
}

/*! check if the queue is empty. */
extern int empty(void) {
	return bottom == top;
}

/*! \brief clean the queue. */
extern void clean(void) {
	if(queue)
		free(queue);
	top = bottom = 0;
}