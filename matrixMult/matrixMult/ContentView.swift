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
                do{
                    try await callAsyncFunctions()
                }catch{
                    print("an error happened")
                }
            }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
