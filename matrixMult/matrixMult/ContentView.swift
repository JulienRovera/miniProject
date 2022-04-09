//
//  ContentView.swift
//  matrixMult
//
//  Created by Rovera, Julien Anthony on 4/9/22.
//

import SwiftUI

struct ContentView: View {
    var body: some View {
        Button(action: callAsyncFunctions){
            Text("Perform Matrix Mult")
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
