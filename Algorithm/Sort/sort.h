#ifndef SORT_H_
#define SORT_H_

typedef struct sorting sorting;

struct sorting {
	char name[20];
	void (* fct)(int *, int);
};

static void swap(int *a, int *b);
static void printTab(int *t, int n);
static void merge(int *t, int m, int n);
static int 	partition(int *t, int start, int end);
static void quick(int *t, int start, int end);
extern void quickSort(int *t, int n);
extern void mergeSort(int *t, int n);
extern void insertionSort(int *t, int n);
// extern void shellSort(int *t, int n);
extern void selectionSort(int *t, int n);
extern void bubbleSort(int *t, int n);
extern void cocktailSort(int *t, int n);
extern void sort(int *t, int n, sorting * s);
extern void init(int *t, int n);

#endif