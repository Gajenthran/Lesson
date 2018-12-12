#ifndef LL_H_
#define LL_H_

typedef struct node_t node_t;
struct node_t {
	int data;
	struct note_t * next;
};

extern node_t *	new_node(int data);
// extern void 	add_node(node_t ** l, node_t * n);
extern void 	add_inode(node_t ** l, int index, node_t * n);
extern void 	push_node(node_t ** l, node_t * n);
extern void 	print_node(node_t * l);
extern void 	delete_node(node_t ** l, int index);
extern void 	free_node(node_t * l);
extern int		size_node(node_t * l);
extern int 		contains_node(node_t * l, int data);

#endif