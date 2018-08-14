#ifndef RATIO_H_
#define RATIO_H_

#define ratio_sign(p, q) ((p) * (q) < 0 ? -1 : 1)
#define ratio_num(r) ((r)->p)
#define ratio_denom(r) ((r)->q)

typedef struct ratio ratio;
struct ratio {
	long int p, q;
};

extern ratio *	ratio_new(long int p, long int q);
extern void		ratio_delete(ratio * r);
extern ratio *	ratio_neg(ratio * r);
extern ratio *	ratio_sub(ratio * r1, ratio * r2);
extern ratio *	ratio_plus(ratio * r1, ratio * r2);
extern ratio *	ratio_mul(ratio * r1, ratio * r2);
extern ratio *	ratio_div(ratio * r1, ratio * r2);
extern long int	ratio_pgcd(long int p, long int q);
extern void		ratio_print(ratio * r);

#endif
