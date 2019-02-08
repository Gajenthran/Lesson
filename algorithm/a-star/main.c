#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "array.h"

void 	a_star(void);
void 	init(int rows, int cols);
void 	init_grid(int rows, int cols);
void 	print_laby(void);
int 	in_position(int x0, int y0, int x1, int y1);
void 	print_laby(void);
int 	euclidean_distance(int x0, int y0, int x1, int y1);
int 	manhattan_distance(int x0, int y0, int x1, int y1);
int 	get_random_num(int n);

enum { EMPTY = '.', WALL = '+', STARTING = 'D', ENDING = 'E', O_SET = 'o', C_SET = 'x', NCOLOR };
enum { LEFT, UP, RIGHT, DOWN, NDIR };

typedef struct cell cell;
struct cell {
	int x, y;
	int f, g, h;
	int c;

};

static int _rows, _cols;
static cell * _grid = NULL;
static cell _start, _end;
static int _dirx[NDIR] = { -1,  0, 1,  0 };
static int _diry[NDIR] = {  0,  1, 0, -1 };

void print_array(int iarray) {
	int i, idx;
	for(i = 0; i < size_array(iarray); i++) {
		idx = get_array(iarray, i);
		int x = idx%_cols; int y = idx/_cols;
		printf("(%d;%d) - ", x, y);
	}
	printf("\n");
}

int get_random_num(int n) {
	return(int)(n *(rand()/(RAND_MAX + 1.0)));
}

void init_grid(int rows, int cols) {
	int r, c, f = 4;
	_rows = rows;
	_cols = cols;
	if(_grid)
		free(_grid);

	_grid = malloc(_rows * _cols * sizeof(*_grid));
	assert(_grid);

	for(r = 0; r < _rows; r++) {
		for(c = 0; c < _cols; c++) {
			_grid[r * _cols + c].x = c;
			_grid[r * _cols + c].y = r;
			_grid[r * _cols + c].f = _grid[r * _cols + c].g = _grid[r * _cols + c].h = 0;
			_grid[r * _cols + c].c = get_random_num(f) > 0 ? EMPTY : WALL;
		}
	}
}

void init_target(int istart, int iend) {
	_start = _grid[istart];
	_grid[istart].c = _start.c = STARTING;
	_end = _grid[iend];
	_grid[iend].c = _end.c = ENDING;
	push_array(OPEN_SET, _start.y * _cols + _start.x);
}
void init(int rows, int cols) {
	init_grid(rows, cols);
	init_array();
	init_target(1 * _cols + 2, 1 * _cols + 6);
	a_star();
}


int in_position(int x0, int y0, int x1, int y1) {
	return x0 == x1 && y0 == y1;
}

void print_laby(void) {
	int r, c, open, closed;
	for(r = 0; r < _rows; r++) {
		for(c = 0; c < _cols; c++) {
			printf(" %c ", _grid[r * _cols + c].c);
		}
		printf("\n");
	}
	printf("\n\n");
}

void a_star(void) {
	int i, low, current, x, y, d, neighbor, idx, tmp_g;
	while(!empty_array(OPEN_SET)) { 
		print_laby();

		low = 0;
		for(i = 0; i < size_array(OPEN_SET); i++) {
			idx = get_array(OPEN_SET, i); 
			if(_grid[idx].f < _grid[low].f)
				low = idx;
		}

		current = get_array(OPEN_SET, low);
		x = current%_cols; y = current/_cols;

		if(in_position(x, y, _end.x, _end.y)) {
			fprintf(stderr, "FAIT! \n");
			return;
		}

		remove_array(OPEN_SET, current);
		push_array(CLOSED_SET, current);
		_grid[current].c = C_SET;

		for(d = 0; d < NDIR; d++) {
			int nx = x + _dirx[d], ny = y + _diry[d];
			if( nx < 0 || nx >= _cols ||
				ny < 0 || ny >= _rows ) {
				continue;
			}

			neighbor = ny * _cols + nx;
			if(_grid[neighbor].c == WALL)
				continue;

			if(!contains_array(CLOSED_SET, neighbor)) {
				 tmp_g = _grid[current].g + 1;

				if(contains_array(OPEN_SET, neighbor)) {
					if(tmp_g < _grid[neighbor].g) {
						_grid[neighbor].g = tmp_g;
					}
				}

				else {
					_grid[neighbor].g = tmp_g;
					push_array(OPEN_SET, neighbor);
					_grid[current].c = O_SET;
				}

				_grid[neighbor].h = manhattan_distance(_grid[neighbor].x, _grid[neighbor].y, _end.x, _end.y);
				_grid[neighbor].f = _grid [neighbor].h + _grid[neighbor].g;
			}
		}
	}
}

int manhattan_distance(int x0, int y0, int x1, int y1) {
	return abs(x1 - x0) + abs(y1 - y0);
}

int euclidean_distance(int x0, int y0, int x1, int y1) {
	return ((x1 - x0) * (x1 - x0)) + ((y1 - y0) * (y1 - y0));
}

int main(void) {
	init(10, 10);
	return 0;
}