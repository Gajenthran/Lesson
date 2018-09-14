#include <stdio.h>
#include <stdlib.h>

#define COLS 3
#define ROWS 3
#define N 3

#define EMPTY 0
#define P1 1
#define P2 2
#define DRAW 3

enum dir { LEFT, UP, RIGHT, DOWN, DIAG_RD, DIAG_RU, DIAG_LD, DIAG_LU };

void minimax(int depth);
int min(int depth);
int max(int depth);
int checkAlignment(int p, int x, int y, int dx, int dy);
int gagnant(void);
int eval(void);

static int _grid[ROWS * COLS] = {EMPTY};
static int _dirx[] = {-1, 0, 1, 0, 1, 1, -1, -1};
static int _diry[] = {0, -1, 0, 1, 1, -1, 1, -1};

void minimax(int depth) {
	int m = -1000;
	int tmp, maxi, maxj, i, j;

	for(i = 0; i < ROWS; i++) {
		for(j = 0; j < COLS; j++) {
			if(_grid[i * COLS + j] == EMPTY) {
				_grid[i * COLS + j] = P1;
				tmp = min(depth-1);

				if(tmp > m) {
					m = tmp;
					maxi = i;
					maxj = j;
				}

				_grid[i * COLS + j] = EMPTY;
			}
		}
	}

	_grid[maxi * COLS + maxj] = P1;
}

int min(int depth) {
	if(depth == 0 || gagnant())
		return eval();

	int i, j, tmp, m = 1000;

	for(i = 0; i < ROWS; i++) {
		for(j = 0; j < COLS; j++) {
			if(_grid[i * COLS + j] == EMPTY) {
				_grid[i * COLS + j] = P1;
				tmp = max(depth-1);

				if(tmp < m)
					m = tmp;

				_grid[i * COLS + j] = EMPTY;
			}
		}
	}

	return m;
}

int max(int depth) {
	if(depth == 0 || gagnant())
		return eval();

	int i, j, tmp, m = -1000;

	for(i = 0; i < ROWS; i++) {
		for(j = 0; j < COLS; j++) {
			if(_grid[i * COLS + j] == EMPTY) {
				_grid[i * COLS + j] = P2;
				tmp = min(depth-1);

				if(tmp > m)
					m = tmp;

				_grid[i * COLS + j] = EMPTY;
			}
		}
	}

	return m;
}

int full(void) {
	int i;
	for(i = 0; i < ROWS * COLS; i++) {
		if(_grid[i] == EMPTY)
			return 0;
	}

	return 1;
}

int checkAlignment(int p, int x, int y, int dx, int dy) {
	int i, j, n = 0;
	while(x < COLS && y < ROWS && x >= 0 && y >= 0) {
		n = _grid[y * COLS + x] == p ? n+1 : 0;
		if(n == N)
			return 1;

		x += dx;
		y += dy;
		printf("%d - %d\n", x, y);
	}

	printf("\n");
	return 0;
}

int checkGrid(void) {
	int i, j, a, p;
	const int align[4][2] = {
		{_dirx[RIGHT], _diry[RIGHT]},
		{_dirx[DOWN], _diry[DOWN]},
		{_dirx[DIAG_RD], _diry[DIAG_RD]},
		{_dirx[DIAG_LD], _diry[DIAG_LD]}
	};

	for(p = 1; p <= 2; p++) {
		for(i = 0; i < ROWS; i++) {
			for(j = 0; j < COLS; j++) {
				for(a = 0; a < 4; a++) {
					if(_grid[i * COLS + j] != EMPTY && 
						checkAlignment(p, j, i, align[a][0], align[a][1]))
						return p;
				}
			}
		}
	}

	if(full()) return DRAW;

	return 0;
}

int gagnant(void) {
	return 0;
}

int eval(void) {
	return 0;
}
int main(void) {
	int n = -1;
	n = checkGrid();
	printf("%d\n", n);

	return 0;
}