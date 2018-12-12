#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ll.h"

node_t * new_node(int data) {
  node_t * n = malloc (sizeof *n);
  assert(n);

  n->next = NULL;
  n->data = data;

  return n;
}

void add_inode(node_t ** l, int index, node_t * n) {
  while (*l) {
    if (index == 0) {
      n->next = (*l);
      (*l) = n;
      return;
    }

    l = &((*l)->next);
    index--;
  }

  n->next = *l;
  (*l) = n;

}

void add_node(node_t **l, node_t * n) {
  while(*l)
    l = &((*l)->next);

  n->next = *l;
  (*l) = n;
}

void push_node(node_t ** l, node_t * n) {
  n->next = *l;
  *l = n;
}

void print_node(node_t * l) {
  while(l) {
    printf("%d - ", l->data);
    l = l->next;
  }
  printf("\n");
}

int size_node(node_t * l) {
  int i = 0;
  while(l) {
    l = l->next;
    i++;
  }

  return i;
}

int contains_node(node_t * l, int data) {
  while(l) {
    if(l->data == data)
      return 1;
    l = l->next;
  }

  return 0;
}

void delete_node(node_t **l, int index) {
  node_t ** tmp = NULL, ** l2 = NULL;
  while(*l) {
    if(index == 1)
      l2 = l;

    if(index == 0) {
      tmp = l;
      l = &((*l)->next);
      free(*tmp);
      (*l2)->next = *l;
    }

    l = &((*l)->next);
    index--;
  }
}

void free_node(node_t * l) {
  node_t * l2;

  while (l) {
    l2 = l;
    l = l->next;
    free(l2);
  }
}

int main(void) {
  node_t * l = NULL;
  node_t * n = NULL;
  // l = new_node(10);

  for(int i = 0; i < 10; i++) {
    n = new_node(10 + i);
    add_node(&l, n);
  }
  print_node(l);
  return 0;

}