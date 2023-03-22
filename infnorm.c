#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 16
int A[N][N] = {0};

void printMatrix()
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf("%3d ", *(*(A+i) + j));
		}
		printf("\n");
	}
}

void initSqrMatrix()
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			A[i][j] = rand() % 256;
		}	
	}
}

int infiniNorm()
{
	int *row_abs_sum = malloc(N * sizeof(int));
	if (row_abs_sum ==  NULL) {
		fprintf(stderr, "ERROR: Bad allocation");
		exit(1);
	}
	memset(row_abs_sum, 0, N*sizeof(int));
	
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			row_abs_sum[i] += A[i][j];
		}
	}

	int max = 0;
	for (int j = 0; j < N; j++) {
		if (row_abs_sum[j] > max) max = row_abs_sum[j];
	}

	free(row_abs_sum);
	return max;
}

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	initSqrMatrix();
	printf("La infini-norma de la matriu A de mida (%d,%d) és = %d \n", N, N, infiniNorm());
	return 0;
}
