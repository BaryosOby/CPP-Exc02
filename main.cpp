#include <iostream>
#include "Graph.h"
#include "Simulation.h"
int main(int argc, char *argv[]) {
    Graph g;
    g.addEdgeAlter("A", "B", bus, intercity, stad,10);
    g.addEdgeAlter("A", "B", tram, intercity, stad,1);
    g.addEdgeAlter("A", "B", rail, intercity, stad, 30);
    g.addEdgeAlter("B", "C", bus, stad, central,10);
    g.addEdgeAlter("B", "C", tram, stad, central,20);
    g.addEdgeAlter("B", "D", rail, stad, intercity,1);
    g.addEdgeAlter("C", "D", bus, central, intercity,20);
    g.addEdgeAlter("C", "D", tram, central, intercity,20);
//    g.addEdgeAlter("A", "B", rail, intercity, stad, 30);
//    g.addEdge("to1", "to4", rail, central, 3);
//    g.addEdge("to4", "to2", bus, intercity, 5);
//    g.addEdge("to4", "to5", sprinter, stad, 5);
//    g.addEdge("to1", "to3", rail, intercity, 4);
//    g.addEdge("to1", "from1", bus, intercity, 6);
//    g.addEdge("from2", "from1", tram, intercity, 6);
//    g.addEdge("to4", "to6", bus, stad, 6);
//    g.addEdge("to2", "to5", rail, intercity, 2);
//    g.addEdge("from1", "to6", rail, stad, 2);

//    cout << g.belFord("A", "D");

//    auto v = g.BFSAlter("A", bus);
//    for(auto i: v){
//        cout << i << ": " << g.DijAlter("A", i, bus)<< endl;
//    }

    cout << g.DijAlter("A", "C", rail)<< endl;
//    copy(v.begin(), v.end(), ostream_iterator<string>(cout, " "));



    return 0;
}
