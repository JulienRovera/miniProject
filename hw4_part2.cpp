///////////////////////////////////////
//
//CMPSC 450
//
//Part 2
//
//Author: Julien Rovera
#include <omp.h>
#include "hw4_part2.hpp"
#include <stdio.h>
// complex algorithm for evaluation

#define NUM_THREADS 10
//the dimensions of the blocks for spatial blocking
#define BSIZE 16
//performs matrix matrix multiplication in an un-serially optimized fasion
void matrix_mult(double *A, double *B, double *C, int N)
{	
	//initialize and declare variables, also set the number of omp threads
	omp_set_num_threads(NUM_THREADS);
	int i;
	//split the matrix up by rows and assign each group of rows to a different thread
	#pragma omp parallel for private (i) 
	for (i = 0; i < N; i++)
		//inner loops are unchanged 
		for (int j = 0; j < N; j++)
		{
			C[i * N + j] = 0;
				for (int k = 0; k < N; k++)
				{
					C[i * N + j] += A[i * N + k] * B[k * N + j];
				}
		}
}

//performs matrix matrix multiplication in a serially optimized fasion
void optimized_matrix_mult(double *A, double *B, double *C, int N, int P)
{
	//declare and initialize variables
	double sum; 
	int en = BSIZE * (N/BSIZE);
	int row;
	int i;
	omp_set_num_threads(P);
	//declare and initialize C
	#pragma omp parallel for private(i)
	for(int i = 0; i < N; i ++){
		row = i * N;
		for(int j = 0; j < N; j ++){
			C[row + j] = 0;
		}
	}
	//loop down through B creating blocks as you go (first block is in top left, the next one will be created bsize elements below that)
	for (int kk = 0; kk < en; kk+=BSIZE)
	{
		//loop right across B creating blocks (first block is in top left, the next one will be creatred bsize elements to the right of that)
		for (int jj = 0; jj < N; jj+=BSIZE)
		{
			//loop through the row sections of size bsize from A that will be multiplied with the block (for each block multiply bsize elements from
			//each row with each element in the block)
			//split this up similar to in unoptimized function to avoid data race cases
			#pragma omp parallel for private (i, row, sum)
			for(i = 0; i < N; i ++)
			{ 
				row = i * N;
				//loop through the columns of the block as well as the columns for the row sections  
				for (int j = jj; j < jj + BSIZE; j++)
				{
					sum = C[row + j];
					//loop through the rows of the block
					for(int k = kk; k < kk + BSIZE; k ++)
					{
						sum += A[row + k] * B[k * N + j];
					}
					C[row + j] = sum;
				}
			}
		}
	}
}

