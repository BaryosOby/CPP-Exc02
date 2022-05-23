#ifndef EXC02_GRAPH_H
#define EXC02_GRAPH_H

#include "Edge.h"
//#define TEST

/** Represents the roadmap.
 * Every station contains 4 actual vertices that connected between each other by 'transit' edges, each one for a different vehicle type.
 * a transit edge has a weight of (the station's transit time) - (its head's stop time).
 * the purpose is to calculate the weight of a path, regarding the differences of the calculations that needed:
 * stop time needs to be included, unless there is a transit within the station.
 *
 * a vertex is represented by a pair of its name and its Vehicle type.
 * the graph is implemented as adjacency list, which is a vector of smart pointers of Edges.
 *
 * @class_member graph: a map from a vertex to its list.
 * @class_member times: Times object to get the updated stop and transit times.
 *
 * @copy_constructor: default. no dynamic allocated memory.
 * @copy_operator=: gets rhs's map and times.
 * @destructor: default. same reason as copy c'tor.
 * @move_constructor: moving the rhs's map, and clears it in the rhs's side.
 * @move operator=: same as the c'tor.
 * */

class Graph {
private:
    map<pair<string, VehicleTypes>, vector<shared_ptr<Edge> >> graph;
    Times times;

    void initVertices(const string &from, StationTypes sType);

public:
    Graph() = default;

    Graph(const Graph &rhs) = default;

    Graph(Graph &&rhs, Times &times) noexcept;

    Graph &operator=(const Graph &rhs);

    Graph &operator=(Graph &&rhs) noexcept;

    ~Graph() = default;

    void addEdge(const string &from, const string &to, VehicleTypes vType, StationTypes fromType, StationTypes toType, int drive);

    void addVertex(const string &name, VehicleTypes vt);

    void addTime(Times &t) { times = t; }

    vector<string> BFSbytype(const string &from, VehicleTypes vType) const;

    int DijByType(const string &from, const string &to, VehicleTypes vType) const;

    int belFord(const string &from, const string &to);

    bool findVertex(const string &vName) const;

    friend ostream &operator<<(ostream &out, const Graph &g);
};

ostream &operator<<(ostream &out, const Graph &g);

#endif
