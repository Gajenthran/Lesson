#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "ll.h"

list_t * init_list(void) {
  list_t * l = (list_t *)malloc(sizeof *l);
  assert(l);
  l->head = NULL;
  l->size = 0;
  return l;
}

int * get_index(list_t * l) {
  srand(time(NULL));
  int r = (rand() % l->size), it = 0;
  lnode_t * n;
  while(l->head) {
    n = l->head;
    l->head = l->head->next;
    it++;
    if(it == r) break;
  }

  int * idx = (int *)malloc(2 * sizeof *idx);
  idx[0] = n->index_l;
  idx[1] = n->index_c;
  return idx;
}

void insert_list(list_t * l, int i_l, int i_c) {
  lnode_t * n = (lnode_t *)malloc(sizeof *n);
  n->index_l = i_l;
  n->index_c = i_c;
  n->next = l->head;
  l->head = n;
  l->size++;
}

void reinit_list(list_t * l, int i_l, int i_c) {
  if(!l)
    return;

  if(l->head) {
    l->head->index_l = i_l;
    l->head->index_c = i_c;
  }

  while(l->head) {
    lnode_t * n = l->head;
    l->head = l->head->next;
    free(n);
  }
  l->size = 1;
}

void print_list(list_t * l) {
  if(!l || !l->head)
    return;

  lnode_t * h = l->head;
  while(h) {
    printf("%d,%d -> ", h->index_l, h->index_c);
    h = h->next;
  }
  printf("NULL\n");
}

void free_list(list_t * l) {
  if(!l)
    return;

  while(l->head) {
    lnode_t * n = l->head;
    l->head = l->head->next;
    free(n);
  }
  l->size = 0;
}

/* int main(void) {
  list_t * l = init_list();
  insert_list(l, 10);
  insert_list(l, 12);
  print_list(l);
  free_list(l);
  return 0;
} */