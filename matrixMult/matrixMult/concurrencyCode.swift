//
//  concurrencyCode.swift
//  matrixMult
//
//  Created by Rovera, Julien Anthony on 4/9/22.
//

import Foundation
import SwiftUI
import UIKit

struct threadStats{
    var N_start: Int
    var N_stop: Int
    var vals: [Double] = []
    var threadNum: Int;
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
    
    //print("returning from matrix mult: \(threadNum)")
    return C
}

func callMatrixMult(A: [Double], B: [Double], N: Int, numThreads: Int) async throws -> [Double]{
    var C: [Double] = []
    var threads: [threadStats] = []
    let loopSize = Int(ceil(Double(N)/Double(numThreads)))
    for i in 0..<numThreads{
        threads.append(threadStats(N_start: i*loopSize, N_stop: (i+1)*loopSize, threadNum: i))
        if threads[i].N_stop > N{
            threads[i].N_stop = N
        }
    }/*
    try await withThrowingTaskGroup(of: [Double].self){group in
        for thread in threads{
            group.addTask{
                let partialMatrix = try await matrixMult(A: A, B: B, N_start: thread.N_start, N_stop: thread.N_stop, N: N, threadNum: thread.threadNum)
                return partialMatrix
            }
        }
        for try await item in group{
            C += item
        }
    }*/
    let T_threads = threads;
    print("using async lets")
    if numThreads == 1{
        async let c1 = matrixMult(A: A, B: B, N_start: T_threads[0].N_start, N_stop: T_threads[0].N_stop, N: N, threadNum: 0)
        try await C += c1
    }else if numThreads == 2{
        async let c1 = matrixMult(A: A, B: B, N_start: T_threads[0].N_start, N_stop: T_threads[0].N_stop, N: N, threadNum: 0)
        async let c2 = matrixMult(A: A, B: B, N_start: T_threads[1].N_start, N_stop: T_threads[1].N_stop, N: N, threadNum: 1)
        try await C = C + c1 + c2
    }else{
        print("not handled yet")
    }
    
    return C
 }




func callAsyncFunctions() async throws{
    print("hello")
    if Thread.isMainThread{
        print("Thread callAsync on main thread")
    }
    //let calendar = Calendar.current
    let N = 200
    //let numThreads = 2
    var a: [Double] = []
    var b: [Double] = []
    //var C_orig = Array(repeating: 0.0, count: (N * N))
    
    for _ in 0..<N*N{
        a.append(Double.random(in: 0..<1000))
        b.append(Double.random(in: 0..<1000))
    }
    
    
    
    let A = a
    let B = b
    let P_vals = [1, 2, 1, 1, 2, 2]
    
    for P in P_vals{
        let startingTime = DispatchTime.now()
        let _ = try await callMatrixMult(A: A, B: B, N: N, numThreads: P)
        let endingTime = DispatchTime.now()
        let concurrentTime = endingTime.uptimeNanoseconds - startingTime.uptimeNanoseconds
        print("time for \(P) threads: \(concurrentTime)")
    }
    
    let serialStartTime = DispatchTime.now()
    try await matrixMult(A: A, B: B, N_start: 0, N_stop: N, N: N, threadNum: 0)
    let serialEndTime = DispatchTime.now()
    let serialTime = serialEndTime.uptimeNanoseconds - serialStartTime.uptimeNanoseconds
    print("Serial Run Time: \(serialTime)")
    
    /*
    let endingTime = DispatchTime.now()
    let concurrentTime = endingTime.uptimeNanoseconds - startingTime.uptimeNanoseconds
    print("Threaded Time: \(concurrentTime) For \(numThreads) threads")
    
    //let startingSerialTime = DispatchTime.now()
    async let C_new = matrixMult(A: A, B: B, N_start: 0, N_stop: N, N: N, threadNum: 0)
    let endingSerialTime = DispatchTime.now()
    let serialTime = endingSerialTime.uptimeNanoseconds - startingSerialTime.uptimeNanoseconds
    //print("serial time: \(serialTime)")
    try await print("C new: \(C_new.count)")
    print("C: \(C.count)")*/
    /*let concurrentTime = calendar.dateComponents([.nanosecond], from: startingTime, to: endingTime).nanosecond!
     print("Threaded Time: \(concurrentTime) For \(numThreads) threads")
     print(startingTime)
     print(endingTime)
     return C*/
}
