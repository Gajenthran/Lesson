#ifndef FIFO_H_
#define FIFO_H_

#define SIZE_QUEUE 1024

extern void initQueue(void);
extern void enqueue(int v);
extern int 	dequeue(void);
extern int 	empty(void);
extern void clean(void);

#endif