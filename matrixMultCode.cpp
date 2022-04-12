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
void matrix_mult(double *A, double *B, double *C, int N, int P)
{
    //set up variables
    std::thread t[P];
    struct MATPARAM *mat_params = new struct MATPARAM[NUMTHREADS];
    int loopSize = ceil(N / P);
    for(int i = 0; i < P; i ++){
        mat_params[i].i_start = i * loopSize;
        mat_params[i].i_stop = (i + 1) * loopSize;
        if(mat_params[i].i_stop > N){
            mat_params[i].i_stop = N;
        }
    }
    
    //run the threads
    for(int i = 0; i < P; i ++){
        t[i] = std::thread(innerLoop, A, B, C, N, &mat_params[i]);
    }
    for(int i = 0; i < P; i ++){
        t[i].join();
    }
    
    delete[] mat_params;
}


