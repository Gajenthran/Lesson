#ifndef HASHTABLE_H_
#define HASHTABLE_H_


typedef struct bucket bucket;
struct bucket {
	int key, value;
	int free;
};

static int 		linear_probing(int h, int i);
static int 		quadratic_probing(int h, int i);
static int 		hash(char * s);
static int 		get_bucket_index(int key);
static void		bucket_clean(bucket * b);
extern void 	ht_init(int s);
extern int 		ht_get(int key);
extern void 	ht_put(int key, int value);
extern void 	ht_remove(int key);
extern void 	ht_clean(void);

#endif