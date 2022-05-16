#ifndef EXC02_GRAPH_H
#define EXC02_GRAPH_H
#include "Edge.h"

class Graph {
private:
    map< string, vector< vector< shared_ptr<Edge> > > > graph;

public:
    Graph() = default;
    Graph(const Graph& rhs) = default;             //copy ctor
    Graph(Graph&& rhs) noexcept;         // move ctor
    Graph& operator=(const Graph& rhs);  //copy =
    Graph& operator=(Graph&& rhs) noexcept;        // move =
    ~Graph() = default;

    //TODO: exceptions if source not found

    void addEdge(const string& from, const string& to, VehicleTypes vType, StationTypes sType, int drive);
    void addVertex(const string& name);

    vector<string> BFSbyType(const string& from, VehicleTypes vType) const;
    int DijByType(const string& from, const string& to, VehicleTypes vType) const;
    int Dij(const string& from, const string& to) const;

    friend ostream& operator<<(ostream& out, const Graph& g);
};

ostream& operator<<(ostream& out, const Graph& g);

#endif
