#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hashtable.h"

static bucket * _bucketArray;
static int _nbBucket = 0;
static int _size = 0;

void ht_init(int s) {
	int i;
	_size = s;
	_bucketArray = malloc(_size * sizeof(*_bucketArray));
	for(i = 0; i < _size; i++) {
		_bucketArray[i].free = 1;
	}
}

void ht_put(int key, int value) {
	if(_size == _nbBucket) {
		fprintf(stderr, "Table de hachage pleine.\n");
		exit(0);
	}
	int index = get_bucket_index(key), inc = 1;

	if(_bucketArray[index].free) {
		_bucketArray[index].key = key;
		_bucketArray[index].value = value;
		_bucketArray[index].free = 0;
		_nbBucket++;		
	}

	else {
		_bucketArray[index].key = key;
		_bucketArray[index].value = value;
	}
}

int ht_get(int key) {
	if(_nbBucket == 0) {
		fprintf(stderr, "Table de hachage vide.\n");
		exit(0);
	}

	bucket b = _bucketArray[get_bucket_index(key)];

	if(!b.free)
		return b.value;
	else {
		fprintf(stderr, "Clé non trouvée.\n");
		exit(0);
	}

}

void ht_remove(int key) {
	if(_nbBucket == 0) {
		fprintf(stderr, "Table de hachage vide.\n");
		exit(0);
	}

	bucket * b = &_bucketArray[get_bucket_index(key)];

	if(!b->free) { bucket_clean(b); _nbBucket--; }
}

void ht_clean(void) {
	if(_bucketArray)
		free(_bucketArray);
	_size = _nbBucket = 0;
}

static int linear_probing(int h, int i) {
	return h + i;
}

static int quadratic_probing(int h, int i) {
	int c1 = 0, c2 = 1;
	return h + c1 * i + c2 * (i * i);
}

static int hash(char * s) {
	int i = 0, c = 38, h = 0, p = 2;
	while(*s) {
		h += (int)*s * (int)pow(c, i);
		i++; s++;
	}
	return (abs(h) % p) % _size;
}

static int get_bucket_index(int key) {
	char s[10];
	int h, index, i;
	sprintf(s, "%d", key);

	h = hash(s);
	index = h;

	for(i = 0; !_bucketArray[index].free && _bucketArray[index].key != key; i++) {
		index = quadratic_probing(h, i) % _size;
	}

	return index;
}

static void bucket_clean(bucket * b) {
	// free(b);
	b->value = -1;
	b->free = 1;
}