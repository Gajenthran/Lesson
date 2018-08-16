#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "vector.h"

void vector_init(vector *v) {
	v->capacity = VECTOR_INIT_CAPACITY;
	v->size = 0;
	v->data = malloc(v->capacity * sizeof(v->data));
	assert(v->data);
}

int vector_length(vector *v) {
	return v->size;
}

static void vector_resize(vector *v, int capacity) {
	v->capacity = capacity;
	v->data = realloc(v->data, sizeof(v->data) * v->capacity);

}

void vector_add(vector *v, int data) {
	if(v->size == v->capacity)
		vector_resize(v, v->capacity * 2);
	v->data[v->size++] = data;
}


void vector_set(vector *v, int index, int data) {
	if(index < 0 || index >= v->size) {
		fprintf(stderr, "Index out of bounds.\n");
		exit(0);
	}

	v->data[index] = data;
}

void vector_put(vector *v, int index, int data) {
	if(index < 0 || index >= v->size) {
		fprintf(stderr, "Index out of bounds.\n");
		exit(0);
	}

	int i;
	for(i = index; i < v->size; i++)
		v->data[i] = v->data[i+1];

	v->data[index] = data;
	v->size++;
}

int vector_get(vector *v, int index) {
	if(index < 0 || index >= v->size) {
		fprintf(stderr, "Index out of bounds.\n");
		exit(0);
	}
	return v->data[index];
}

void vector_delete(vector *v, int index) {
	if(index < 0 || index >= v->size) {
		fprintf(stderr, "Index out of bounds.\n");
		exit(0);
	}

	int i;
	for(i = index; i < v->size; i++)
		v->data[i] = v->data[i+1];

	v->size--;
	if(v->size > 0 && v->size == v->capacity/4)
		vector_resize(v, v->capacity/2);
}

void vector_print(vector *v) {
	int i;
	for(i = 0; i < v->size; i++)
		printf("%d - ", v->data[i]);
	printf("\n");
}

void vector_free(vector *v) {
	free(v->data);
}
