//
//  ContentView.swift
//  matrixMult
//
//  Created by Rovera, Julien Anthony on 4/9/22.
//

import SwiftUI

struct ContentView: View {
    var body: some View {
        Text("Performing Matrix Mult")
            .task{
                await callAsyncFunctions(numThreads: 1)
                await callAsyncFunctions(numThreads: 2)
                await callAsyncFunctions(numThreads: 3)
                await callAsyncFunctions(numThreads: 4)
                await callAsyncFunctions(numThreads: 10)
            }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
