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

int MIN(int a, int b) {
    if (a <= b)
        return a;
    return b;
}

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

	// okay, now let each int* array be a contiguous array that
	// represents a whole block
	int block_sz = MIN(64, A->height);
	int block_dim = A->height / block_sz;

	// int array per block
	A->data = (int**) malloc(sizeof(int*) * block_dim * block_dim);
	B->data = (int**) malloc(sizeof(int*) * block_dim * block_dim);
	for (int i = 0; i < block_dim; i++) {
		for (int j = 0; j < block_dim; j++) {
			// A->data[x] is xth block reading left to right and top to bottom
			A->data[i*block_dim+j] = malloc(sizeof(int) * block_sz * block_sz);
			B->data[i*block_dim+j] = malloc(sizeof(int) * block_sz * block_sz);
		}
	}

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
			A->data[i/block_sz*block_dim + j/block_sz][(i%block_sz)*block_sz+j%block_sz] = 
				atoi(first_tok);
		}
	}

	// gross, i know. but reset for matrix B
	free(base);
	line_len = B->width * 2 + 64;
	base = malloc(line_len);

	// read in matrix B
	for (int i = 0; i < B->height; i++) {
		line_sz = getline(&base, &line_len, mfile);
		line = base;
		for (int j = 0; j < B->width; j++) {
			first_tok = strsep(&line, " ");
			B->data[i/block_sz*block_dim + j/block_sz][(i%block_sz)*block_sz+j%block_sz] = 
				atoi(first_tok);
		}
	}

	free(base);

	// build C
	C->height = A->height;
	C->width = B->width;
	C->data = (int**) malloc(sizeof(int*) * (C->height/block_sz) * (C->width/block_sz));
	for (int i = 0; i < C->height; i++) {
		C->data[i] = (int*) malloc(sizeof(int) * block_sz * block_sz);
	}

	return 0;
}

void multiply(matrix* A, matrix* B, matrix* C) {
	int block_sz = MIN(64, A->height);
	int block_dim = A->height/block_sz;

	// for each row of blocks in A
	for (int i = 0; i < A->height / block_sz; i++) {
		// for each column of blocks in B
		for (int j = 0; j < B->width / block_sz; j++) {
			// for each column of blocks in A and row of blocks in B
			for (int k = 0; k < A->width / block_sz; k++) {
				// for each element in C's current block
				for (int l = 0; l < block_sz; l++) {
					for (int m = 0; m < block_sz; m++) {
						// calculate the element using A and B!
						for (int n = 0; n < block_sz; n++) {
							C->data[i*block_dim+j][l*block_sz+m] +=
								A->data[i*block_dim+k][l*block_sz+n] * B->data[k*block_dim+j][n*block_sz+m];
						}
					}
				}
			}
		}
	}

	// // for each row of blocks in A
	// for (int i = 0; i < A->height / block_sz; i++) {
	// 	// for each column of blocks in B
	// 	for (int j = 0; j < B->width / block_sz; j++) {
	// 		// for each column of blocks in A and row of blocks in B
	// 		for (int k = 0; k < A->width / block_sz; k++) {
	// 			// for each element in C's current block
	// 			for (int l = 0; l < block_sz; l++) {
	// 				for (int m = 0; m < block_sz; m++) {
	// 					// calculate the element using A and B!
	// 					for (int n = 0; n < block_sz; n++) {
	// 						C->data[i*block_sz+l][j*block_sz+m] += 
	// 							A->data[i*block_sz+l][k*block_sz+n] * B->data[k*block_sz+n][j*block_sz+m];
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}
	// }
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

	if (build_matrices(&A, &B, &C, argv[1]) < 0) {
		printf("Could not build matrices\n");
		return 2;
	}

	multiply(&A, &B, &C);

	// int block_sz = MIN(64, A.height);
	// int block_dim = A.height/block_sz;
	// for (int i = 0; i < block_dim; i++) {
	// 	for (int j = 0; j < block_sz; j++) {
	// 		for (int k = 0; k < block_dim; k++) {
	// 			for (int l = 0; l < block_sz; l++) {
	// 				printf("%d\t", C.data[i*block_dim+k][j*block_sz+l]);
	// 			}
	// 		}
	// 		printf("\n");
	// 	}
	// }	

	return 0;
}








