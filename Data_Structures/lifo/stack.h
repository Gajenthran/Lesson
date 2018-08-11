#ifndef STACK_H___
#define STACK_H___

#define SIZE_STACK 1024 

extern void 	initStack(void);
extern void	push(int val);
extern int		pop(void);
extern int		empty(void);
extern void	clean(void);

#endif