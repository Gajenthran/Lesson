/*!
 * \file sort.c
 * \brief Sorting algorithm
 * \author PANCHALINGAMOORTHY Gajenthran. Adapted from Farès Belhadj's course (code).
 * \date 6 August 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "sort.h"

#define N 10000

/*! \brief exchange the value of an element A with the value 
 * of an element B */
static void swap(int *a, int *b) {
	if(*a != *b) {
		*a ^= *b;
		*b ^= *a;
		*a ^= *b;
	}
}

/*! \brief display all the elements of the table. */
static void printTab(int *t, int n) {
	int i;
	for(i = 0; i < n; i++) {
		printf("%d - ", t[i]);
	}
	printf("\n");
}

/*! \brief complementary function of the mergeSort function. */
static void merge(int *t, int m, int n) {
	int i, j, k;
	int *t0 = malloc(n * sizeof(*t0));
	assert(t0);

	for(i = k = 0, j = m; k < n; k++) {
		if(j == n)
			t0[k] = t[i++];
		else if(i == m)
			t0[k] = t[j++];
		else if(t[j] < t[i])
			t0[k] = t[j++];
		else
			t0[k] = t[i++];
	}

	for(i = 0; i < n; i++) {
		t[i] = t0[i];
	}

	free(t0);
}

/*! \brief Merge sort.
 * Time complexity: nlog(n), nlog(n), nlog(n).
 * Space: n.
 * Stable: Yes. */
void mergeSort(int *t, int n) {
	if(n < 2) return;
	int mid = n/2;
	mergeSort(t, mid);
	mergeSort(t + mid, n - mid);
	merge(t, mid, n);
}

/*! \brief complementary function of the quickSort function. */
static int partition(int *t, int start, int end) {
	int pivot = t[end];
	int i = start-1, j;
	for(j = start; j < end; j++) {
		if(t[j] <= pivot) {
			i++;
			swap(&t[i], &t[j]);
		}
	}

	swap(&t[i+1], &t[end]);
	return (i + 1);
}

static void quick(int *t, int start, int end) {
	if(start < end) {
		int p = partition(t, start, end);
		quick(t, start, p-1);
		quick(t, p+1, end);
	}
}

/*! \brief Quick sort.
 * Time complexity: nlog(n), nlog(n), n^2.
 * Space: log(n), n.
 * Stable: No. */
void quickSort(int *t, int n) {
	quick(t, 0, n-1);
}

/*! \brief Insertion sort.
 * Time complexity: n, n^2, n^2.
 * Space: 1.
 * Stable: Yes. */
void insertionSort(int *t, int n) {
	int i, j, el;
	for(i = 1; i < n; i++) {
		el = t[i];
		for(j = i; j >= 1 && t[j-1] > el; j--) {
			t[j] = t[j-1];
		}
		t[j] = el;
	}
}

/*! \brief Selection sort.
 * Time complexity: n^2, n^2, n^2.
 * Space: 1.
 * Stable: No. */
void selectionSort(int *t, int n) {
	int i, j, min, tmp;
	for(i = 0; i < n-1; i++) {
		min = i;
		for(j = i+1; j < n; j++) {
			if(t[min] > t[j]) min = j;
		}
		swap(&t[min], &t[i]);
	}
}

/*! \brief Insertion sort.
 * Time complexity: n, n^2, n^2.
 * Space: 1.
 * Stable: No. */
void bubbleSort(int *t, int n) {
	int i, j;
	for(i = 0; i < n; i++)
		for(j = n-1; j >= i; j--)
			if(t[i] > t[j])
				swap(&t[i], &t[j]);
}

/*! \brief Cocktail sort.
 * Time complexity: n, n^2, n^2.
 * Space: 1.
 * Stable: Yes. */
void cocktailSort(int *t, int n) {
	int swaping = 1, start = 0, end = n-2, i;

	while(swaping) {
		swaping = 0;

		for(i = start; i <= end; i++) {
			if(t[i] > t[i+1]) {
				swap(&t[i], &t[i+1]);
				swaping = 1;
			}
		}
		end--;

		for(i = end; i >= start; i--) {
			if(t[i] > t[i+1]) {
				swap(&t[i], &t[i+1]);
				swaping = 1;
			}
		}
		start++;
	}
}

/*! \brief Initialize the values of the table. */
void init(int *t, int n) {
	int i;
	srand(time(NULL));
	for(i = 0; i < n; i++)
		t[i] = n * (rand() / (RAND_MAX + 1.));
}

/*! \brief Use different sorting algorithm and compare their times. */
void sort(int *t, int n, sorting * s) {
	clock_t start, end, dt;
	while(s->fct) {
		init(t, n);
		start = clock();
		s->fct(t, n);
		end = clock();
		dt = end - start;
		fprintf(stderr, "%s with %d elements:\t %lu ms.\n", s->name, n, dt);
		s++;
	}
}

int main(void) {
	int t[N] = {0};
	sorting s[] = {
		{"Bubble",		bubbleSort},
		{"Selection", 	selectionSort},
		{"Cocktail", 	cocktailSort},
		{"Insertion", 	insertionSort},
		{"Quicksort", 		quickSort},
		{"Mergesort", 		mergeSort},
		{"", 			NULL}
	};
	sort(t, N, s);
	return 0;
}