#ifndef EXC02_GRAPH_H
#define EXC02_GRAPH_H
#include "Edge.h"
#define TEST

class Graph {
private:
    map< string, vector< vector< shared_ptr<Edge> > > > graph;
    map<pair<string, VehicleTypes>, vector< shared_ptr<Edge> >> alter;

    void initVertices(const string &from, StationTypes sType);

public:
    Graph() = default;
    Graph(const Graph& rhs) = default;
    Graph(Graph&& rhs) noexcept;
    Graph& operator=(const Graph& rhs);
    Graph& operator=(Graph&& rhs) noexcept;
    ~Graph() = default;

    //TODO: exceptions if source not found

    void addEdge(const string& from, const string& to, VehicleTypes vType, StationTypes sType, int drive);
    void addEdgeAlter(const string& from, const string& to, VehicleTypes vType, StationTypes fromType, StationTypes toType, int drive);
    void addVertex(const string& name);
    void addVertex(const string& name, VehicleTypes vt);

    vector<string> BFSbyType(const string& from, VehicleTypes vType) const;
    vector<string> BFSAlter(const string& from, VehicleTypes vType) const;
    int DijByType(const string& from, const string& to, VehicleTypes vType) const;
    int DijAlter(const string& from, const string& to, VehicleTypes vType) const;
    int Dij(const string& from, const string& to) const;
    int belFord(const string& from, const string& to);

    friend ostream& operator<<(ostream& out, const Graph& g);
};

ostream& operator<<(ostream& out, const Graph& g);

#endif
