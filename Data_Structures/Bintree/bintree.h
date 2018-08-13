#ifndef BINTREE_H_
#define BINTREE_H_

typedef struct node_t node_t;

struct node_t {
	int v;
	struct node_t * lc, *rc;
};

extern void		bt_add_value(node_t ** bt, int v);
extern void 	bt_print(node_t ** bt);
extern void 	bt_contains(node_t * bt, int v);
extern void 	bt_free(node_t ** bt);
static int 		max(int x, int y);
static int 		sub_depth(node_t * bt, int depth);
extern int 		bt_depth (node_t * bt);

#endif