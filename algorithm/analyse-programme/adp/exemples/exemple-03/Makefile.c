#include <stdlib.h>
#include <stdio.h>

static int pgcd(int a, int b) {
  if(a == b) return a;
  else if(a > b) return pgcd(a-b, b);
  return pgcd(b-a, a);
}

int afficherValeur(int *t, int n, int idx) {
  return (idx >= n || idx < 0) ? -1 : t[idx];
}

void afficherTab(int *t, int n) {
  int i;
  for(i = 0; i < n; i++)
    printf("%d\n", t[i]);
}

void initTab(int *t, int n) {
  int i, j = rand() % n;
  for(i = 0; i < n; i++) 
    t[i] = pgcd(i+1, j+1);
}

int main(void) {
  int t[10] = {0}, n = 10;
  initTab(t, n);
  afficherTab(t, n);
}