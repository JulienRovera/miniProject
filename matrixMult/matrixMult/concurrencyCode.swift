//
//  concurrencyCode.swift
//  matrixMult
//
//  Created by Rovera, Julien Anthony on 4/9/22.
//

import Foundation

struct threadStats{
    var N_start: Int
    var N_stop: Int
    var vals: [Double] = []
}

func matrixMult(A: [Double], B: [Double], N_start: Int, N_stop: Int, N: Int, threadNum: Int) async throws -> [Double]{
    var C = Array(repeating: 0.0, count: ((N_stop - N_start) * N))
    for i in N_start..<N_stop{
        let c_i = i - N_start
        for j in 0..<N{
            for k in 0..<N{
                C[c_i * N + j] += A[i*N+k] * B[k * N + j]
            }
        }
    }
    print("returning from matrix mult: \(threadNum)")
    return C
}

func callMatrixMult(A: [Double], B: [Double], N: Int, T_threads: [threadStats]) async throws -> [Double]{
    async let c1 = matrixMult(A: A, B: B, N_start: T_threads[0].N_start, N_stop: T_threads[0].N_stop, N: N, threadNum: 1)
    async let c2 = matrixMult(A: A, B: B, N_start: T_threads[1].N_start, N_stop: T_threads[1].N_stop, N: N, threadNum: 2)
    return try await c1
}


func callAsyncFunctions(){
    let N = 200
    let numThreads = 2
    let loopSize = Int(ceil(Double(N)/Double(numThreads)))
    var threads: [threadStats] = []
    var a: [Double] = []
    var b: [Double] = []
    var C = Array(repeating: 0.0, count: (N * N))
    var C_orig = Array(repeating: 0.0, count: (N * N))
    
    for _ in 0..<N*N{
        a.append(Double.random(in: 0..<1000))
        b.append(Double.random(in: 0..<1000))
    }
    
    for i in 0..<numThreads{
        threads.append(threadStats(N_start: i*loopSize, N_stop: (i+1)*loopSize))
        if threads[i].N_stop > N{
            threads[i].N_stop = N
        }
    }
    
    let A = a
    let B = b
    let T_threads = threads
    print("about to enter task")
    Task{
        try await callMatrixMult(A: A, B: B, N: N, T_threads: T_threads)
    }
}
