#include <iostream>
#include "Graph.h"

int main() {
    Graph g;
    g.addEdge("from1", "to1", bus, intercity, 5);
    g.addEdge("from1", "to1", sprinter, stad, 5);
    g.addEdge("from1", "to2", bus, intercity, 5);
    g.addEdge("from1", "to2", tram, intercity, 5);
    g.addEdge("to1", "to4", bus, intercity, 3);
    g.addEdge("to1", "to4", rail, central, 3);
    g.addEdge("to4", "to2", bus, intercity, 5);
    g.addEdge("to4", "to5", sprinter, stad, 5);
    g.addEdge("to1", "to3", rail, intercity, 4);
    g.addEdge("to1", "from1", bus, intercity, 6);
    g.addEdge("from2", "from1", tram, intercity, 6);
    g.addEdge("to4", "to6", bus, stad, 6);
    g.addEdge("to2", "to5", rail, intercity, 2);
    g.addEdge("from1", "to6", rail, stad, 2);


    auto v = g.BFSbyType("from1", bus);
//    for(auto i: v){
//        cout << i << ": " << g.Dij("from1", i)<< endl;
//    }

    cout << g;
//    copy(v.begin(), v.end(), ostream_iterator<string>(cout, " "));

    return 0;
}
