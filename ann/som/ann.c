#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024

typedef struct data_t data_t;
struct data_t {
	double * v;
	char * name;
	double norm;
};

typedef struct node_t node_t;
struct node_t {
	double * w;
	char * etq;
	double act;
};

char * readFile(char * f);
data_t * tokenize(char * t, int * size);
void printData(data_t * data, int size);

// format suivant: slength(double), swidth(double), plength(double), pwidth(double), class(str)
int main(int argc, char *argv[]) {
	int size;
	data_t * d = NULL;

	char * t = readFile(argv[1]);
	d = tokenize(t, &size);
	printData(d, size);
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

data_t * tokenize(char * t, int *size) {
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