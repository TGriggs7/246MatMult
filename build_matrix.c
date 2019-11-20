/*

	Used to generate random matrices of given dimensions

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Usage: ./build_matrix <dimension of square matrix>");
		return 1;
	}

	// get requested dimension
	int dim = atoi(argv[1]);

	// create output file name
	char filename[64];
	sprintf(filename, "%dx%d.txt", dim, dim);

	// init output file
	FILE* outfile = fopen(filename, "w");
	char write_buf[64];
	sprintf(write_buf, "%d %d\n", dim, dim);
	for (int i = 0; i < 2; i++) {
		fwrite(write_buf, strlen(write_buf), 1, outfile);
	}

	time_t t;
	srand((unsigned) time(&t));

	// generate matrix
	for (int i = 0; i < 2 * dim; i++) {
		for (int j = 0; j < dim; j++) {
			sprintf(write_buf, "%d ", rand() % 10);
			fwrite(write_buf, strlen(write_buf), 1, outfile);
		}
		sprintf(write_buf, "\n");
		fwrite(write_buf, strlen(write_buf), 1, outfile);
	}

	fclose(outfile);

	return 0;
}