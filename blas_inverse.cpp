/*
	
	Read matrix input

*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cblas.h>
#include "lapacke.h"

typedef struct matrix {
	int height;
	int width;
	double* data;
} matrix;

// reads matrix input from filename and stores matrices in *A and *B
int build_matrices(matrix* A, char* filename) {
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

	// get B dimensions (and ignore them)
	getline(&linebuf, &linebuf_sz, mfile);	

	// free linebuf 
	free(first_tok);

	// allocate space for matrix A
	A->data = (double*) malloc(sizeof(double) * A->height * A->width);

	// init line reading for matrix data
	size_t line_len = A->width * 2 + 64;
	char* base = malloc(line_len);
	char* line;

	// read in matrix A
	for (int i = 0; i < A->height; i++) {
		line_sz = getline(&base, &line_len, mfile);
		line = base;

		for (int j = 0; j < A->width; j++) {
			first_tok = strsep(&line, " ");
			A->data[i*A->height+j] = atof(first_tok);
		}
	}
	free(base);

	// INVERSE

	int ipiv[A->height+1];
    lapack_int ret;

    LAPACKE_dgetrf(LAPACK_ROW_MAJOR,
                          A->height,
                          A->height,
                          A_data,
                          A->height,
                          ipiv);


    LAPACKE_dgetri(LAPACK_COL_MAJOR,
                       A->height,
                       A_data,
                       A->height,
                       ipiv);

    /*
    for (int i = 0; i < A->height; i++) {
    	for (int j = 0; j < A->width; j++) {
    		printf("%f ", A_data[i*A->height+j]);
    	}
    	printf("\n");
    }
    */

	return 0;
}

// first two lines indicates dimensions of matrices
int main(int argc, char** argv) {
	if (argc != 2 && argc != 3) {
		printf("Usage: ./mm <txt file with matrices> [outfile]\n");
		return 1;
	}

	FILE* outfile = NULL;
	if (argc == 3) {
		outfile = fopen(argv[2], "w");
	}

	matrix A;
	matrix B;
	matrix C;

	if (build_and_inverse(&A, argv[1]) < 0) {
		printf("Could not build matrices\n");
		return 2;
	}

	return 0;
}








