#include "Graph.h"
#include <memory>

Graph::Graph(Graph &&rhs, Times &times) noexcept: graph(move(rhs.graph)), times(move(rhs.times)) {
    rhs.graph.clear();
}

Graph &Graph::operator=(const Graph &rhs) {
    if (&rhs == this) {
        return *this;
    }
    graph = rhs.graph;
    times = rhs.times;
    return *this;
}

Graph &Graph::operator=(Graph &&rhs) noexcept {
    graph = move(rhs.graph);
    times = move(rhs.times);
    rhs.graph.clear();
    return *this;
}


void Graph::initVertices(const string &name, StationTypes sType) {
    for (int i = 0; i < 4; i++) {
        addVertex(name, static_cast<VehicleTypes>(i));
    }
    int transit = times.transitTimes[sType];
    for (int i = 0; i < 4; i++) {
        auto idx = static_cast<VehicleTypes>(i);
        int stop_i = times.stopTimes[idx];
        for (int j = 0; j < 4; j++) {
            auto jdx = static_cast<VehicleTypes>(j);
//            int stop_j = times.stopTimes[jdx];
            if (i == j) { continue; }
            else {
                graph.at(make_pair(name, idx)).push_back(
                        make_shared<Edge>(name, name, jdx, sType, transit - stop_i, times));
            }
        }
    }
}

void Graph::addEdge(const string &from, const string &to, VehicleTypes vType, StationTypes fromType,
                    StationTypes toType, int drive) {

    auto obj = make_pair(from, vType);
    // init station with all four vehicle types
    if (graph.find(obj) == graph.end()) {
        initVertices(from, fromType);
    }

    if (graph.find(make_pair(to, vType)) == graph.end()) {
        initVertices(to, toType);
    }
    for (const auto &e: graph.at(obj)) {
        if (e->getDest() == to) {
            e->setDriveTime(drive);
            return;
        }
    }

    graph.at(obj).push_back(make_shared<Edge>(from, to, vType, fromType, drive, times));
}

void Graph::addVertex(const string &name, VehicleTypes vt) {
    vector<shared_ptr<Edge> > v;
    pair<string, VehicleTypes> p(name, vt);
    graph.insert(pair<pair<string, VehicleTypes>, vector<shared_ptr<Edge> > >(p, v));
}

vector<string> Graph::BFSbytype(const string &from, VehicleTypes vType) const {
    vector<string> res;
    map<string, bool> discovered;
    queue<string> q;
    q.push(from);
    discovered.insert(pair<string, bool>(from, true));
    while (!q.empty()) {
        auto curr = q.front();
        q.pop();
        for (const auto &e: graph.at(make_pair(curr, vType))) {
            if (e->getDest() != curr) {
                string dest = e->getDest();
                if (discovered.find(dest) == discovered.end()) {
                    discovered.insert(pair<string, bool>(dest, true));
                    q.push(dest);
                    res.push_back(dest);
                }
            }
        }
    }
    return res;
}


int Graph::DijByType(const string &from, const string &to, VehicleTypes vType) const {
    map<string, int> discovered;
    for (const auto &v: graph) {
        auto curr = v.first;
        if (curr.second == vType) {
            if (curr.first == from) { discovered.insert(pair<string, int>(curr.first, 0)); }
            else { discovered.insert(pair<string, int>(curr.first, inf)); }
        }
    }
    auto cmp = [discovered](string &left, string &right) { return discovered.at(left) > discovered.at(right); };
    priority_queue<string, vector<string>, decltype(cmp)> q(cmp);
    q.push(from);

    while (!q.empty()) {
        auto curr = q.top();
        q.pop();
        for (const auto &e: graph.at(make_pair(curr, vType))) {
            if (e->getDest() != curr) {
                auto dest = e->getDest();
                if (discovered[dest] > discovered[curr] + e->getDriveTime() + e->getStopTime()) {
                    discovered[dest] = discovered[curr] + e->getDriveTime() + e->getStopTime();
                    q.push(dest);
                }
            }
        }
    }
    if (discovered[to] >= inf) { return inf; }
    return discovered[to] - times.stopTimes[vType];
}



int Graph::belFord(const string &from, const string &to) {
    auto source = make_pair("temp source", bus);
    pair<string, VehicleTypes> dest = make_pair("temp dest", bus);
    addVertex("temp source", bus);
    addVertex("temp dest", bus);
    for (int i = 0; i < 4; i++) {
        auto idx = static_cast<VehicleTypes>(i);
        graph.at(source).push_back(make_shared<Edge>("temp source", from, idx, stad, -times.stopTimes[bus], times));
        graph.at(make_pair(to, idx)).push_back(make_shared<Edge>(to, "temp dest", bus, stad, -times.stopTimes[idx], times));
    }

    map<pair<string, VehicleTypes>, int> weights;
    map<pair<string, VehicleTypes>, pair<string, VehicleTypes> > pi;
    for (const auto &v : graph) {
        weights.insert(make_pair(v.first, inf));
    }
    weights.at(source) = 0;
    for (int i = 0; i < weights.size() - 1; i++) {
        for (const auto &v : graph) {
            auto stop = times.stopTimes[v.first.second];
            for (const auto &e: v.second) {
                auto curr_neighbour = make_pair(e->getDest(), e->getVtype());
                if (weights.at(curr_neighbour) > weights.at(v.first) + e->getDriveTime() + stop) {
                    weights.at(curr_neighbour) = weights.at(v.first) + e->getDriveTime() + stop;
                    if(pi.find(curr_neighbour) == pi.end()){
                        pi.insert(make_pair(curr_neighbour,v.first));
                    }
                    else{
                        pi.at(curr_neighbour) = v.first;
                    }

                }
            }
        }
    }

    pair<string, VehicleTypes> origin = dest;
    while (origin.first != from) {
#ifdef TEST
        cout << "(" << origin.first << ", " << origin.second << ")" << weights[origin] << " <- ";
#endif

        origin = pi.at(origin);
    }
#ifdef TEST
    cout << "(" << origin.first << ", " << origin.second << ")" << weights[origin] << " <- ";
#endif
    int result = weights.at(dest) - times.stopTimes[origin.second];

    graph.erase(source);
    graph.erase(dest);
    for (int i = 0; i < 4; i++) {
        auto idx = static_cast<VehicleTypes>(i);
        graph.at(make_pair(to, idx)).pop_back();
    }

    return result;
}


ostream &operator<<(ostream &out, const Graph &g) {
    out << "Neverland's roadmap:" << endl;
    for (const auto &v : g.graph) {
        string from = v.first.first;
        if (v.second.size() > 3) {
            out << "from " << from << " by " << v_types_strings[v.first.second] << " to:" << endl;
            for (const auto &e: v.second) {
                if (e->getDest() != from) {
                    out << e->getDest() << " | ";
                }
            }
            out << "\n\n";
        }
    }
    return out;
}

bool Graph::findVertex(const string &vName) const {
    return graph.find(make_pair(vName, bus)) != graph.end();
}


