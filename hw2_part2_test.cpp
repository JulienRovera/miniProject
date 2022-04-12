#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#include <hw4_part2.hpp>

void get_walltime(double* wcTime) {

     struct timeval tp;

     gettimeofday(&tp, NULL);

     *wcTime = (double)(tp.tv_sec + tp.tv_usec/1000000.0);

}

// complex algorithm for evaluation
void matrix_mult_orig(double *A, double *B, double *C, int N)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			C[i * N + j] = 0;
			for (int k = 0; k < N; k++)
			{
				C[i * N + j] += A[i * N + k] * B[k * N + j];
			}
		}
}

void compareOutputs(double *output1, double *output2, int length)
{
	for (int i = 0; i < length; i++)
		for (int j = 0; j < length; j++)
		{
			if (output1[i * length + j] != output2[i * length + j])
			{
				printf("Outputs do not match! (%i, %i) (%f, %f)\n", 
					i, j, 
					output1[i * length + j], output2[i * length + j]);
				return;
			}
		}
	
	printf("Output match, test passed!\n");
						
}


int main(int argc, char *argv[])
{

	int N = 2000;
	int P = 10;

	if(argc > 1){
		P = atoi(argv[1]);
		//printf("P: %d\n", P);
	}
	/*long* N_vals = new long[30];
	long optimized_N_vals [12] = {320,  400, 480, 560, 1600, 1680, 1760, 1840, 2000, 2080, 2160, 2240};
	int P_vals [9] = {1, 2, 4, 8, 10, 15, 20, 40, 100};
	long offsets [3] = {500, 1700, 2000};
	long ranges [3] = {150, 100, 1000};*/
	int N_vals [4] = {208, 624, 1904, 2160}; 
	int P_vals [9] = {1, 2, 4, 8, 10, 15, 20, 40, 100};
	long random = 0;
	/*for(int i = 0; i < 3; i ++){
		for(int j = 0; j < 10; j ++){
			random = offsets[i] + (rand()%ranges[i]);
			N_vals[10 * i + j] = random;
		} 
	}*/
	double d_S, d_E;
	double orig_d_S, orig_d_E;
	double op_d_S, op_d_E;
	// some declarations
	double *A;
	double *B;
	double *C;
	double *orig_C;
	//double *orig_C = new double[N * N];
	N = N_vals[3];
	printf("N: %d\n", N);
	printf("P\t\torig time\t\treg time\t\toptimized time\n");
	for (int k = 0; k < 9; k ++){
		P = P_vals[k];
		A = new double[N * N]; 
		B = new double[N * N]; 
		C = new double[N * N]; 
		orig_C = new double[N * N];
		// populate memory with some random data
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				A[i * N + j] = i * i;
				B[i * N + j] = (double)j / (double) (i + 1);
			}
		}

	

		// run the original for functional verification
		if(k == 0){
			get_walltime(&orig_d_S);
			matrix_mult_orig(A, B, orig_C, N);
			get_walltime(&orig_d_E);
		}
		// start benchmark
		
		get_walltime(&d_S);
		matrix_mult(A, B, C, N, P);
		get_walltime(&d_E);
		// check the two matrices
		//compareOutputs(orig_C, C, N);
		
		get_walltime(&op_d_S);
		optimized_matrix_mult(A, B, C, N, P);
		get_walltime(&op_d_E);
		// report results
		//printf("P\t\torig time\t\treg time\t\toptimized time\n");
		printf("%d\t\t%f\t\t%f\t\t%f\n", P, (orig_d_E - orig_d_S), (d_E - d_S), (op_d_E - op_d_S));

		// cleanup!
		delete[] A;
		delete[] B;
		delete[] C;
		delete[] orig_C;
	}
	return 0;
}
