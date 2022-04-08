///////////////////////////////////////
//
//CMPSC 450
//
//Part 2
//
//Author: Julien Rovera
#include <iostream>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <mutex>

//the structure to represent each thread
struct MATPARAM{
	int i_start;
	int i_stop;
};

#define NUMTHREADS 4
//the size of the row segment taken from A, as well as the dimensions of the block in B
#define BSIZE 16

//This function performs multiplies the rows that each thread has been assigned to all of the columns in the matrix
void innerLoop(double *A, double *B, double *C, int N, struct MATPARAM *mat_params){
	for(int i = mat_params->i_start; i < mat_params->i_stop; i ++){
		for (int j = 0; j < N; j++)
		{
			C[i * N + j] = 0;
				for (int k = 0; k < N; k++)
				{
					C[i * N + j] += A[i * N + k] * B[k * N + j];
				}
		}
	}
}

//performs a thread matrix-matrix multiplication algorithm that was not optimized for serial execution
void matrix_mult(double *A, double *B, double *C, int N)
{
	//set up variables
	std::thread t[NUMTHREADS];
	struct MATPARAM *mat_params = new struct MATPARAM[NUMTHREADS];

	//set up threads. Use the ceiling function to make sure that if N is not divisible by NUMTHREADS, the alogrithm still works 
	//Each thread deals with a certain number of rows in the matrix. This is to make sure that data race cases do not occur between 
	//threads performing operation using the same rows and columns which could result in two threads trying to write to the same position in C
	int loopSize = ceil(N / NUMTHREADS);
	for(int i = 0; i < NUMTHREADS; i ++){
		mat_params[i].i_start = i * loopSize;
		mat_params[i].i_stop = (i + 1) * loopSize;
		if(mat_params[i].i_stop > N){
			mat_params[i].i_stop = N;
		}
	}
	
	//run the threads
	for(int i = 0; i < NUMTHREADS; i ++){
		t[i] = std::thread(innerLoop, A, B, C, N, &mat_params[i]);
	}
	for(int i = 0; i < NUMTHREADS; i ++){
		t[i].join();
	}
}

//This code for spatial blocking was adapted from an example given at this source: https://csapp.cs.cmu.edu/public/waside/waside-blocking.pdf


void innerLoopOptimized(double *A, double*B, double*C, int N, int jj, int kk, struct MATPARAM *mat_params){
	int row;
	double sum;
	for(int i = mat_params->i_start; i < mat_params->i_stop; i ++)
	{ 
		row = i * N;
		for (int j = jj; j < jj + BSIZE; j++)
		{
			sum = C[row + j];
			for(int k = kk; k < kk + BSIZE; k ++)
			{
				sum += A[row + k] * B[k * N + j];
			}
			C[row + j] = sum;
		}
	}
}

//performs a theraded matrix-matrix multiplication aglorithm that was optimized for serial execution
void matrix_mult_optimized(double *A, double *B, double *C, int N, int P)
{
	//variable declaration
	int en = BSIZE * (N/BSIZE);
	int row;
	std::thread t[P];
	struct MATPARAM * mat_params = new MATPARAM[P];

	//sets up the array to be returned
	int loopSize = ceil(N / P);
	for(int i = 0; i < N; i ++){
		row = i * N;
		for(int j = 0; j < N; j ++){
			C[row + j] = 0;
		}
	}
	//sets up threads in the same way as above
	for(int i = 0; i < P; i ++){
		mat_params[i].i_start = i * loopSize;
		mat_params[i].i_stop = (i + 1) * loopSize;
		if(mat_params[i].i_stop > N){
			mat_params[i].i_stop = N;
		}
	}
	//performs the loops to perform spatial blocking
	//Threads each are given sets of whole rows to operate over. This is to prevent data race cases 
	for (int kk = 0; kk < en; kk+=BSIZE)
	{
		for (int jj = 0; jj < N; jj+=BSIZE)
		{
			for(int i = 0; i < P; i ++){
				t[i] = std::thread(innerLoopOptimized, A, B, C, N, jj, kk, &mat_params[i]);
			}
			for(int i = 0; i < P; i ++){
				t[i].join();
			}
		}
	}
}



