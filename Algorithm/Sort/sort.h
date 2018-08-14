#ifndef SORT_H_
#define SORT_H_

static void swap(int *a, int *b);
static void printTab(int *t, int n);
static void merge(int *t, int m, int n);
extern void insertionSort(int *t, int gap, int start, int end);
extern void shellSort(int *t, int n);
extern void selectionSort(int *t, int n);
extern void bubbleSort(int *t, int n);
extern void cocktailSort(int *t, int n);
extern void mergeSort(int *t, int n);

#endif