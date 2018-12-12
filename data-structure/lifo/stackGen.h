#ifndef STACKGEN_H_
#define STACKGEN_H_

#include <stdlib.h>

extern void		stackInit(size_t s);
extern void *	pop(void);
extern int		empty(void);
extern void		push(void * data);
extern void		quit(void);

#endif