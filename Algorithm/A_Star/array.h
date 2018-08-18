#ifndef ARRAY_H_
#define ARRAY_H_

#define SZ 1024
enum { OPEN_SET, CLOSED_SET, PATH, N_SET };

extern void 	init_array(void);
extern void 	push_array(int iarray, int value);
extern int 		pop_array(int iarray);
extern int 		in_array(int iarray, int value);
extern void 	remove_array(int iarray, int value);
extern int 		get_array(int iarray, int index);
extern int 		size_array(int iarray);
extern int 		empty_array(int iarray);
static void 	resize_array(int iarray);

#endif