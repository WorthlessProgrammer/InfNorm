#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <pthread.h>

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

int infiniNormSeq()
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

#ifndef NUM_THREADS
#define NUM_THREADS 4
#endif

typedef struct {
	int n_elements;
	int row;
	int *result;
} thread_args;

void *thread_compute_row_sum(void* args)
{
	thread_args *t = (thread_args*) args;

	if (t == NULL) {
		exit(69);
	}

	int r_idx = 0;
	for (int i = t->row; i < t->row + t->n_elements; i++) {
		for (int j = 0; j < N; j++) {
			t->result[r_idx] += A[i][j];
		}
		r_idx += 1;
	}

	return NULL;
}

int infiniNormThreads()
{
	pthread_t threads[NUM_THREADS];
	thread_args args[NUM_THREADS] = {0};
	int max = 0;

	int *row_abs_sum = malloc(N * sizeof(int));
	if (row_abs_sum ==  NULL) {
		fprintf(stderr, "ERROR: Bad allocation");
		exit(1);
	}
	memset(row_abs_sum, 0, N*sizeof(int));

	int elements = N/NUM_THREADS;
	for (int i = 0; i < NUM_THREADS; i++) {
		int fila = i*elements;
		elements += (16%5 && i == NUM_THREADS-1) ? N%NUM_THREADS : 0;

		//TODO: Why does t = {elements, fila, row_abs_sum + i*elements};
		// not work? is it the thread lib or the internals of for()?
		args[i] = (thread_args) {elements, fila, row_abs_sum + i*elements};
		pthread_create(&threads[i], NULL, thread_compute_row_sum, (void*) &args[i]);
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

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
	//printf("La infini-norma de la matriu A de mida (%d,%d) és = %d \n", N, N, infiniNormSeq());
	printf("La infini-norma de la matriu A de mida (%d,%d) és = %d \n", N, N, infiniNormThreads());
	return 0;
}
