#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 1024

typedef struct data data_t;
struct data {
	double * v;
	char * name;
	double norm;
};

typedef struct node node_t;
struct node {
	double * w;
	char * etq;
	double act;
};

char * readFile(char * f);
data_t * tokenize(char * t, int * size);
void printData(data_t * data, int size);
int * initDraw(int n);

int main(int argc, char *argv[]) {
	int size;
	data_t * data = NULL;

	char * t = readFile(argv[1]);
	data = tokenize(t, &size);
	int * draw = initDraw(size);
	return 0;
}

char * readFile(char * f) {
  char * d = NULL;
  size_t size = 0;

  FILE * fp = fopen(f, "r");
  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  rewind(fp);

  d = (char *)malloc((size + 1) * sizeof *d);
 	fread(d, size, 1, fp);

 	d[size] = '\0';
  return d;
}

int * initDraw(int n) {
	srand(time(0));
	int * draw = (int *)malloc(n * sizeof *draw);
	int * seen = (int *)calloc(0, n * sizeof *draw);
	assert(draw);

	int i, offset, r;
	for(i = 0; i < n; i++) {
		r = rand() % n;
		if(seen[r]) {
			offset = 1;
			while(1) {
				if(r + offset < n && !seen[r + offset]) {
					draw[i] = r + offset;
					seen[n] = 1;
					break;
				} else if(r - offset >= 0 && seen[r - offset]) {
					draw[i] = r - offset;
					seen[n] = 1;
					break;
				}
				offset++;
			}
		}
		draw[i] = r;
		seen[n] = 1;
	}
	return draw;
}

data_t * tokenize(char * t, int * size) {
	int i = 0, j = 0, c = 1;
	while(t[i]) {
		if(t[i] == '\n') c++;
		i++;
	}

	char * tok = strtok(t, "\n");
	char ** lines = (char **)malloc(c * sizeof *lines);
	i = 0;

	while(tok != NULL) {
		lines[i++] = strdup(tok);
		tok = strtok(NULL, "\n");
	}

	data_t * data = (data_t *)malloc(c * sizeof *data);
	assert(data);
	char * strdouble, * end;
	double v;
	for(i = 0; i < c; i++) {
		data[i].v = (double *)malloc(4 * sizeof *data[i].v);
		assert(data[i].v);
		tok = strtok(lines[i], ",");
		j = 0;
		while(tok != NULL) {
			if(j & 4) {
				data[i].name = strdup(tok);
			} else {
				v = strtod(tok, &end);
				if(end == tok) {
					fprintf(stderr, "Erreur lors de la conversion des données.\n");
					exit(0);
				} else {
					data[i].v[j] = v;
				}
			}
			tok = strtok(NULL, ",");
			j++;
		}
	}
	*size = c;
	return data;
}

void printData(data_t * data, int size) {
	int i, j;
	for(i = 0; i < size; i++) {
		for(j = 0; j < 4; j++) {
			printf("%.1f,", data[i].v[j]);
		}
		printf("%s\n", data[i].name);
	}
}