#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bintree.h"

void bt_add_value(node_t ** bt, int v) {
	while (*bt != NULL) {
		if (v < (*bt)->v)
			bt = &((*bt)->lc);
		else
			bt = &((*bt)->rc);
	}

	*bt = malloc(1 * sizeof **bt);
	assert(*bt);
	(*bt)->v = v;
	(*bt)->rc = (*bt)->lc = NULL;
}

void bt_print(node_t ** bt) {
	if ((*bt) == NULL)
		return;

	bt_print(&(*bt)->lc);
	printf("%d - ", (*bt)->v);
	bt_print(&(*bt)->rc);
}

void bt_contains(node_t * bt, int value) {
	if (bt == NULL)
		return;

	if (bt->v == value)
		printf("CONTAINS !\n");

	bt_contains(bt->lc, v);
	bt_contains(bt->rc, v);
}

void bt_free(node_t ** bt) {
	if ((*bt) == NULL)
		return;

	bt_free(&(*bt)->lc);
	bt_free(&(*bt)->rc);
	free(*bt);
}

static int max(int x, int y) {
	if (x > y)
		return x;
	return y;
}

static int sub_depth(node_t * bt, int depth) {
	if (bt == NULL)
		return depth;

	depth++;
	return max(sub_depth(bt->lc, depth), sub_depth(bt->rc, depth));
}

int bt_depth (node_t * bt) {
	return sub_depth(bt, 0);
}