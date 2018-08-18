#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "array.h"

int ** _array = NULL;
int _top[N_SET] = {-1};
int _size[N_SET] = {0};

void init_array(void) {
	int i;
	_array = malloc(N_SET * sizeof(*_array));
	assert(_array);

	for(i = 0; i < N_SET; i++) {
		_size[i] = SZ;
		_array[i] = malloc(_size[i] * sizeof(*_array[i]));
		assert(_array[i]);
	}
}

int size_array(int iarray) {
	return _top[iarray]+1;
}

int in_array(int iarray, int value) {
	int i;
	for(i = 0; i < size_array(iarray); i++) {
		if(_array[iarray][i] == value)
			return i;
	}

	return -1;
}

void remove_array(int iarray, int value) {
	int i, j, tmp, index;
	index = in_array(iarray, value);
	if(index < 0) return;

	for(i = index; i < size_array(iarray)-1; i++)
		_array[iarray][i] = _array[iarray][i+1];

	_top[iarray]--;
}

void resize_array(int iarray) {
	_size[iarray] *= 2;
	_array[iarray] = realloc(_array[iarray], _size[iarray] * sizeof(**_array));
	assert(_array[iarray]);
}

int get_array(int iarray, int index) {
	return _array[iarray][index];
}

void push_array(int iarray, int value) {
	if(_top[iarray] >= _size[iarray]-1)
		resize_array(iarray);
	_array[iarray][++_top[iarray]] = value;
}

int pop_array(int iarray) {
	return _array[iarray][_top[iarray]--];
}

int empty_array(int iarray) {
	return _top[iarray] < 0;
}