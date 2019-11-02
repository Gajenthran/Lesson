#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct lnode lnode_t;
struct lnode {
  int index;
  lnode_t * next;
};

typedef struct list list_t;
struct list {
  lnode_t * head;
  int size;
};

list_t * initList(void) {
  list_t * l = (list_t *)malloc(sizeof *l);
  assert(l);
  l->head = NULL;
  l->size = 0;
  return l;
}

void insertList(list_t * l, int index) {
  lnode_t * n = (lnode_t *)malloc(sizeof *n);
  n->index = index;
  n->next = l->head;
  l->head = n;
  l->size++;
}

void printList(list_t * l) {
  if(!l || !l->head)
    return;

  lnode_t * h = l->head;
  while(h) {
    printf("%d -> ", h->index);
    h = h->next;
  }
  printf("NULL\n");
}

void freeList(list_t * l) {
  if(!l)
    return;

  while(l->head) {
    lnode_t * n = l->head;
    l->head = l->head->next;
    free(n);
  }
  l->size = 0;
}

int main(void) {
  list_t * l = initList();
  insertList(l, 10);
  insertList(l, 12);
  printList(l);
  freeList(l);
  return 0;
}