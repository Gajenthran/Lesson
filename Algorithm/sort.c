#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
	if(*a != *b) {
		*a ^= *b;
		*b ^= *a;
		*a ^= *b;
	}
}

void insertionSort(int *t, int n) {
	int i, j, el;
	for(i = 0; i < n; i++) {
		el = t[i];
		for(j = i; j > 0 && t[j-1] > el; j--) {
			t[j] = t[j-1];
		}
		t[j] = el;
	}
}

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

void bubbleSort(int *t, int n) {
	int i, j;
	for(i = 0; i < n; i++)
		for(j = n-1; j >= i; j--)
			if(t[i] > t[j])
				swap(&t[i], &t[j]);
}

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

void printTab(int *t, int n) {
	int i;
	for(i = 0; i < n; i++) {
		printf("%d - ", t[i]);
	}
	printf("\n");
}

int main(void) {
	int t[] = {9, 8, 2, 1, 3, 5, 9 };
	int n = sizeof(t)/sizeof(*t);

	printf("BEFORE:\t"); printTab(t, n);
	cocktailSort(t, n);
	printf("AFTER:\t"); printTab(t, n);
}