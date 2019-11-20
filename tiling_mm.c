/*
	
	Read matrix input

*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct matrix {
	int height;
	int width;
	int** data;
} matrix;

// reads matrix input from filename and stores matrices in *A and *B
int build_matrices(matrix* A, matrix* B, matrix* C, char* filename) {
	FILE* mfile = fopen(filename, "r");
	if (!mfile) {
		printf("Could not open file: %s\n", filename);
		return -1;
	}

	char* linebuf = malloc(16);
	char* first_tok;
	size_t linebuf_sz = 16;
	size_t line_sz;

	// get A dimensions
	line_sz = getline(&linebuf, &linebuf_sz, mfile);
	first_tok = strsep(&linebuf, " ");

	// get dimensions of first matrix
	A->height = atoi(first_tok);
	A->width = atoi(linebuf);

	// reset linebuf
	linebuf = first_tok;

	// get B dimensions
	line_sz = getline(&linebuf, &linebuf_sz, mfile);	
	first_tok = strsep(&linebuf, " ");

	// get dimensions of second matrix
	B->height = atoi(first_tok);
	B->width = atoi(linebuf);

	// free linebuf 
	free(first_tok);

	// allocate space for matrix A
	A->data = (int**) malloc(sizeof(int*) * A->height);
	B->data = (int**) malloc(sizeof(int*) * B->height);

	// init line reading for matrix data
	char* line = NULL;
	size_t line_len = 0;

	for (int i = 0; i < A->height; i++) {
		A->data[i] = (int*) malloc(sizeof(int) * A->width);

		line_sz = getline(&line, &line_len, mfile);

		for (int j = 0; j < A->width; j++) {
			first_tok = strsep(&line, " ");
			A->data[i][j] = atoi(first_tok);
		}
	}

	for (int i = 0; i < B->height; i++) {
		B->data[i] = (int*) malloc(sizeof(int) * B->width);

		line_sz = getline(&line, &line_len, mfile);

		for (int j = 0; j < B->width; j++) {
			first_tok = strsep(&line, " ");
			B->data[i][j] = atoi(first_tok);
		}
	}


	// build C
	C->height = A->height;
	C->width = B->width;
	C->data = (int**) malloc(sizeof(int*) * C->height);
	for (int i = 0; i < C->height; i++) {
		C->data[i] = (int*) malloc(sizeof(int) * C->width);
	}

	// printf("MATRIX A\n");
	// for (int i = 0; i < A->height; i++) {
	// 	for (int j = 0; j < A->width; j++) {
	// 		printf("%d ", A->data[i][j]);
	// 	}
	// 	printf("\n");
	// }

	// printf("\nMATRIX B\n");
	// for (int i = 0; i < B->height; i++) {
	// 	for (int j = 0; j < B->width; j++) {
	// 		printf("%d ", B->data[i][j]);
	// 	}
	// 	printf("\n");
	// }

	return 0;
}

void multiply(matrix* A, matrix* B, matrix* C) {
	for (int i = 0; i < B->width; i++) {
		for (int j = 0; j < A->height; j++) {
			for (int k = 0; k < A->width; k++) {
				C->data[i][j] += A->data[i][k] * B->data[k][j];
			}
		}
	}
}

// first two lines indicates dimensions of matrices
int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Usage: ./mm <txt file with matrices>\n");
		return 1;
	}

	matrix A;
	matrix B;
	matrix C;

	if (build_matrices(&A, &B, &C, argv[1]) < 0) {
		printf("Could not build matrices\n");
		return 2;
	}

	multiply(&A, &B, &C);

	// print_matrix(C);
	printf("\nMATRIX C\n");
	for (int i = 0; i < C.height; i++) {
		for (int j = 0; j < C.width; j++) {
			printf("%d\t", C.data[i][j]);
		}
		printf("\n");
	}


	return 0;
}








