#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ratio.h"

ratio * ratio_new(long int p, long int q) {
	int m = 1, s;
	ratio * r;

	if(q == 0) {
		fprintf(stderr, "Erreur : denominateur nul\n");
		exit(0);
	}

	s = ratio_sign(p, q); p = labs(p); q = labs(q);

	if(p) m = ratio_pgcd(p, q);

	r = malloc(sizeof * r);
	assert(r);

	ratio_num(r) = s * p / m;
	ratio_denom(r) = q / m;

	return r;
}

void ratio_delete(ratio * r) {
	free(r);
}

ratio * ratio_neg(ratio * r) {
	return ratio_new(-ratio_num(r), ratio_denom(r));
}


ratio * ratio_sub(ratio * r1, ratio * r2) {
	return ratio_new((ratio_num(r1) * ratio_denom(r2)) - (ratio_num(r2) * ratio_denom(r1)),
					 ratio_denom(r1) * ratio_denom(r2));
}


ratio * ratio_plus(ratio * r1, ratio * r2) {
	return ratio_new((ratio_num(r1) * ratio_denom(r2) + ratio_num(r2) * ratio_denom(r1)),
					 ratio_denom(r1) * ratio_denom(r2));
}


ratio * ratio_mul(ratio * r1, ratio * r2) {
	return ratio_new(ratio_num(r1) * ratio_num(r2), ratio_denom(r1) * ratio_denom(r2));
}


ratio * ratio_div(ratio * r1, ratio * r2) {
	return ratio_new(ratio_num(r1) * ratio_denom(r2), ratio_denom(r1) * ratio_num(r2));
}

void ratio_print(ratio * r) {
	printf("%ld / %ld\n", ratio_num(r), ratio_denom(r));
}

long int ratio_pgcd(long int p, long int q) {
	if(p == q) return p;
	else if(p > q) return ratio_pgcd(p-q, q);
	return ratio_pgcd(q-p, p);
}
