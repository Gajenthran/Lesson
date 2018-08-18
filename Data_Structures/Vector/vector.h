#ifndef VECTOR_H_
#define VECTOR_H_

#define VECTOR_INIT_CAPACITY 5

typedef struct vector vector;
struct vector {
	int * data;
    int capacity;
    int size;
};

static void 	vector_resize(vector *v, int capacity);
extern void 	vector_init(vector *v);
extern int 		vector_length(vector *v);
extern void 	vector_add(vector *v, int data);
extern void 	vector_set(vector *v, int index, int data);
extern void 	vector_put(vector *v, int index, int data);
extern int 		vector_get(vector *v, int index);
extern void 	vector_delete(vector *v, int index);
extern void 	vector_print(vector *v);
extern void 	vector_free(vector *v);

#endif