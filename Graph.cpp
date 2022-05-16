#include "Graph.h"

#include <memory>

Graph::Graph(Graph &&rhs) noexcept : graph(move(rhs.graph)) {
    rhs.graph.clear();
}

Graph &Graph::operator=(const Graph &rhs) {
    if(&rhs == this){
        return *this;
    }
    graph = rhs.graph;
    return *this;
}

Graph& Graph::operator=(Graph &&rhs) noexcept{
    graph = move(rhs.graph);
    rhs.graph.clear();
    return *this;
}

void Graph::addEdge(const string &from, const string &to, VehicleTypes vType, StationTypes sType, int drive) {
    if(graph.find(from) == graph.end()){
        addVertex(from);
    }
    if(graph.find(to) == graph.end()){
        addVertex(to);
    }
    for(const auto& e: graph[from][vType]){
        if (e->getDest() == to){
            e->setDriveTime(drive);
            return;
        }
    }

    graph[from][vType].push_back(make_shared<Edge>(from, to, vType, sType, drive));
}

void Graph::addVertex(const string &name) {
    vector< vector< shared_ptr<Edge> > > v(4);
    graph.insert(pair<string, vector< vector< shared_ptr<Edge> > >>(name, v));
}

vector<string> Graph::BFSbyType(const string &from, VehicleTypes vType) const{
    vector<string> res;
    map<string, bool> discovered;
    queue<string> q;
    q.push(from);
    discovered.insert(pair<string, bool> (from, true));
    while(!q.empty()){
        auto curr = q.front();
        q.pop();
        for(const auto& e: graph.at(curr)[vType]){
            auto dest = e->getDest();
            if(discovered.find(dest) == discovered.end()){
                discovered.insert(pair<string, bool>(dest, true));
                q.push(dest);
                res.push_back(dest);
            }
        }
    }
    return res;
}

int Graph::DijByType(const string &from, const string &to, VehicleTypes vType) const{
    map<string, int> discovered;
    for(const auto& v: graph){
        auto curr = v.first;
        if(curr == from){ discovered.insert(pair<string, int>(curr, 0)); }
        else{ discovered.insert(pair<string, int>(curr, RAND_MAX)); }
    }
    auto cmp = [discovered](string& left, string& right){ return discovered.at(left) > discovered.at(right); };
    priority_queue<string, vector<string>, decltype (cmp)> q(cmp);
    for(const auto& v: graph){
        q.push(v.first);
    }
    while(!q.empty()){
        auto curr = q.top();
        q.pop();
        for(const auto& e: graph.at(curr)[vType]){
            auto dest = e->getDest();
            if(discovered[dest] > discovered[curr] + e->getDriveTime() + e->getStopTime()){
                discovered[dest] = discovered[curr] + e->getDriveTime() + e->getStopTime();
            }
        }
    }
    return discovered[to] - times.stopTimes[vType];
}

int Graph::Dij(const string &from, const string &to) const{
    map<string, int> discovered;
    for(const auto& v: graph){
        auto curr = v.first;
        if(curr == from){ discovered.insert(pair<string, int>(curr, 0)); }
        else{ discovered.insert(pair<string, int>(curr, RAND_MAX)); }
    }
    auto cmp = [discovered](string& left, string& right){ return discovered.at(left) > discovered.at(right); };
    priority_queue<string, vector<string>, decltype (cmp)> q(cmp);
    for(const auto& v: graph){
        q.push(v.first);
    }
    VehicleTypes startVehicle = bus;
    VehicleTypes currType = bus;
    while(!q.empty()){
        auto curr = q.top();
        q.pop();
        for(const auto& vehicle: graph.at(curr)){
            for(const auto& e: vehicle){
                auto dest = e->getDest();
                if(curr == from){
                    currType = e->getVtype();
                    if(discovered[dest] > discovered[curr] + e->getDriveTime()){
                        discovered[dest] = discovered[curr] + e->getDriveTime();
                        if(curr == from){
                            startVehicle = e->getVtype();
                        }
                    }
                }
                else if(e->getVtype() == currType && curr != from){
                    if(discovered[dest] > discovered[curr] + e->getDriveTime() + e->getStopTime()){
                        discovered[dest] = discovered[curr] + e->getDriveTime() + e->getStopTime();
                        if(curr == from){
                            startVehicle = e->getVtype();
                        }
                    }
                }
                else{
                    if(discovered[dest] > discovered[curr] + e->getDriveTime() + e->getTransit()){
                        discovered[dest] = discovered[curr] + e->getDriveTime() + e->getTransit();
                        currType = e->getVtype();
                    }
                }

            }
        }
        if(curr == from){
            currType = startVehicle;
        }
    }
    return discovered[to];
}

ostream &operator<<(ostream &out, const Graph& g) {
    out << "Neverland's roadmap:" << endl;
    for(const auto& v : g.graph){
        out << "from " << v.first << " to:" <<endl;
        for(int i = 0; i < 3; i++){
            auto curr = v.second[i];
            for(const auto& e: curr){
                out << *e;
            }
        }
        out << endl;
    }
    return out;
}
