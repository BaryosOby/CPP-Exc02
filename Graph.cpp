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

//void Graph::addEdge(const string &from, const string &to, VehicleTypes vType, StationTypes sType, int drive) {
//    if(graph.find(from) == graph.end()){
//        addVertex(from);
//    }
//    if(graph.find(to) == graph.end()){
//        addVertex(to);
//    }
//    for(const auto& e: graph[from][vType]){
//        if (e->getDest() == to){
//            e->setDriveTime(drive);
//            return;
//        }
//    }
//
//    graph[from][vType].push_back(make_shared<Edge>(from, to, vType, sType, drive));
//}

//void Graph::addVertex(const string &name) {
//    vector< vector< shared_ptr<Edge> > > v(4);
//    graph.insert(pair<string, vector< vector< shared_ptr<Edge> > >>(name, v));
//}

void Graph::initVertices(const string &name, StationTypes sType){
    for(int i = 0; i < 4; i++){
        addVertex(name, static_cast<VehicleTypes>(i));
    }
    int transit = times.transitTimes[sType];
    for(int i = 0; i < 4; i++){
        auto idx = static_cast<VehicleTypes>(i);
        int stop_i = times.stopTimes[idx];
        for(int j = 0; j < 4; j++){
            auto jdx = static_cast<VehicleTypes>(j);
            int stop_j = times.stopTimes[jdx];
            if(i == j){ continue; }
            else{ alter.at(make_pair(name, idx)).push_back(make_shared<Edge>(name, name, jdx, sType, transit-stop_i-stop_j)); }
        }
    }
}

void Graph::addEdgeAlter(const string &from, const string &to, VehicleTypes vType, StationTypes fromType, StationTypes toType, int drive) {

    auto obj = make_pair(from, vType);
    // init station with all four vehicle types
    if(alter.find(obj) == alter.end()){
        initVertices(from, fromType);
    }

    if(alter.find(make_pair(to, vType)) == alter.end()){
        initVertices(to, toType);
    }

//    for(alter.at(make_pair(from, vType)).)

    for(const auto& e: alter.at(obj)){
        if (e->getDest() == to){
            e->setDriveTime(drive);
            return;
        }
    }

    alter.at(obj).push_back(make_shared<Edge>(from, to, vType, fromType, drive));
}

void Graph::addVertex(const string &name, VehicleTypes vt) {
    vector< shared_ptr<Edge> > v;
    pair<string, VehicleTypes> p(name, vt);
    alter.insert(pair<pair<string, VehicleTypes>, vector< shared_ptr<Edge> > >(p, v));
}

vector<string> Graph::BFSAlter(const string &from, VehicleTypes vType) const {
    vector<string> res;
    map<string, bool> discovered;
    queue<string> q;
    q.push(from);
    discovered.insert(pair<string, bool> (from, true));
    while(!q.empty()){
        auto curr = q.front();
        q.pop();
        for(const auto& e: alter.at(make_pair(curr, vType))){
            if(e->getDest() != curr){
                string dest = e->getDest();
                if(discovered.find(dest) == discovered.end()){
                    discovered.insert(pair<string, bool>(dest, true));
                    q.push(dest);
                    res.push_back(dest);
                }
            }
        }
    }
    return res;
}

//vector<string> Graph::BFSbyType(const string &from, VehicleTypes vType) const{
//    vector<string> res;
//    map<string, bool> discovered;
//    queue<string> q;
//    q.push(from);
//    discovered.insert(pair<string, bool> (from, true));
//    while(!q.empty()){
//        auto curr = q.front();
//        q.pop();
//        for(const auto& e: graph.at(curr)[vType]){
//            auto dest = e->getDest();
//            if(discovered.find(dest) == discovered.end()){
//                discovered.insert(pair<string, bool>(dest, true));
//                q.push(dest);
//                res.push_back(dest);
//            }
//        }
//    }
//    return res;
//}

int Graph::DijAlter(const string &from, const string &to, VehicleTypes vType) const {
    map<string, int> discovered;
    for(const auto& v: alter){
        auto curr = v.first;
        if(curr.second == vType){
            if(curr.first == from){ discovered.insert(pair<string, int>(curr.first, 0)); }
            else{ discovered.insert(pair<string, int>(curr.first, inf)); }
        }
    }
    auto cmp = [discovered](string& left, string& right){ return discovered.at(left) > discovered.at(right); };
    priority_queue<string, vector<string>, decltype (cmp)> q(cmp);
    q.push(from);

    while(!q.empty()){
        auto curr = q.top();
        q.pop();
        for(const auto& e: alter.at(make_pair(curr, vType))){
            if(e->getDest() != curr){
                auto dest = e->getDest();
                if(discovered[dest] > discovered[curr] + e->getDriveTime() + e->getStopTime()){
                    discovered[dest] = discovered[curr] + e->getDriveTime() + e->getStopTime();
                    q.push(dest);
                }
            }
        }
    }
    if(discovered[to] >= inf){return inf; }
    return discovered[to] - times.stopTimes[vType];
}

//int Graph::DijByType(const string &from, const string &to, VehicleTypes vType) const{
//    map<string, int> discovered;
//    for(const auto& v: graph){
//        auto curr = v.first;
//        if(curr == from){ discovered.insert(pair<string, int>(curr, 0)); }
//        else{ discovered.insert(pair<string, int>(curr, RAND_MAX)); }
//    }
//    auto cmp = [discovered](string& left, string& right){ return discovered.at(left) > discovered.at(right); };
//    priority_queue<string, vector<string>, decltype (cmp)> q(cmp);
//    for(const auto& v: graph){
//        q.push(v.first);
//    }
//    while(!q.empty()){
//        auto curr = q.top();
//        q.pop();
//        for(const auto& e: graph.at(curr)[vType]){
//            auto dest = e->getDest();
//            if(discovered[dest] > discovered[curr] + e->getDriveTime() + e->getStopTime()){
//                discovered[dest] = discovered[curr] + e->getDriveTime() + e->getStopTime();
//            }
//        }
//    }
//    return discovered[to] - times.stopTimes[vType];
//}


int Graph::belFord(const string &from, const string &to) {
    auto source = make_pair("temp source", bus);
    pair<string, VehicleTypes> dest = make_pair("temp dest", bus);
    addVertex("temp source", bus);
    addVertex("temp dest", bus);
    for(int i = 0; i < 4; i++){
        auto idx = static_cast<VehicleTypes>(i);
        alter.at(source).push_back(make_shared<Edge>("temp source", from, idx, stad, -times.stopTimes[bus]));
        alter.at(make_pair(to, idx)).push_back(make_shared<Edge>(to, "temp dest", bus, stad, -times.stopTimes[idx]));
    }

    map< pair<string, VehicleTypes>, int > weights;
    map< pair<string, VehicleTypes>, pair<string, VehicleTypes> > pi;
    for(const auto & v : alter){
        weights.insert(make_pair(v.first, inf));
        pi.insert(make_pair(v.first, make_pair("", bus)));
    }
    weights.at(source) = 0;
    for(int i = 0; i < weights.size() - 1; i++){
        for(const auto& v : alter){
            auto stop = times.stopTimes[v.first.second];
            for(const auto& e: v.second){
                auto curr_neighbour = make_pair(e->getDest(), e->getVtype());
                if(weights.at(curr_neighbour) > weights.at(v.first) + e->getDriveTime() + stop){
                    weights.at(curr_neighbour) = weights.at(v.first) + e->getDriveTime() + stop;
                    pi.at(curr_neighbour) = v.first;

                }
            }
        }
    }

    pair<string, VehicleTypes> origin = dest;
    while(origin.first != from){
        #ifdef TEST
        cout << "(" << origin.first << ", " << origin.second << ")" << weights[origin] << " <- ";
        #endif
        origin = pi.at(origin);
    }
    #ifdef TEST
    cout << "(" << origin.first << ", " << origin.second << ")" << weights[origin] << " <- ";
    #endif
    int result = weights.at(dest) - times.stopTimes[origin.second];

    alter.erase(source);
    alter.erase(dest);
    for(int i = 0; i < 4; i++){
        auto idx = static_cast<VehicleTypes>(i);
        alter.at(make_pair(to, idx)).pop_back();
    }

    return result;
}

//int Graph::Dij(const string &from, const string &to) const{
//    map<string, int> discovered;
//    for(const auto& v: graph){
//        auto curr = v.first;
//        if(curr == from){ discovered.insert(pair<string, int>(curr, 0)); }
//        else{ discovered.insert(pair<string, int>(curr, RAND_MAX)); }
//    }
//    auto cmp = [discovered](string& left, string& right){ return discovered.at(left) > discovered.at(right); };
//    priority_queue<string, vector<string>, decltype (cmp)> q(cmp);
//    for(const auto& v: graph){
//        q.push(v.first);
//    }
//    VehicleTypes startVehicle = bus;
//    VehicleTypes currType = bus;
//    while(!q.empty()){
//        auto curr = q.top();
//        q.pop();
//        for(const auto& vehicle: graph.at(curr)){
//            for(const auto& e: vehicle){
//                auto dest = e->getDest();
//                if(curr == from){
//                    currType = e->getVtype();
//                    if(discovered[dest] > discovered[curr] + e->getDriveTime()){
//                        discovered[dest] = discovered[curr] + e->getDriveTime();
//                        if(curr == from){
//                            startVehicle = e->getVtype();
//                        }
//                    }
//                }
//                else if(e->getVtype() == currType && curr != from){
//                    if(discovered[dest] > discovered[curr] + e->getDriveTime() + e->getStopTime()){
//                        discovered[dest] = discovered[curr] + e->getDriveTime() + e->getStopTime();
//                        if(curr == from){
//                            startVehicle = e->getVtype();
//                        }
//                    }
//                }
//                else{
//                    if(discovered[dest] > discovered[curr] + e->getDriveTime() + e->getTransit()){
//                        discovered[dest] = discovered[curr] + e->getDriveTime() + e->getTransit();
//                        currType = e->getVtype();
//                    }
//                }
//
//            }
//        }
//        if(curr == from){
//            currType = startVehicle;
//        }
//    }
//    return discovered[to];
//}

ostream &operator<<(ostream &out, const Graph& g) {
    out << "Neverland's roadmap:" << endl;
    for(const auto& v : g.alter){
        string from = v.first.first;
        if(v.second.size() > 3){
            out << "from " << from << " by "<< v_types_strings[v.first.second] <<" to:" <<endl;
            for(const auto& e: v.second){
                if(e->getDest() != from){
                    out << e->getDest() << " | ";
                }
            }
            out << "\n\n";
        }
    }
    return out;
}

bool Graph::findVertex(const string &vName) const {
    return alter.find(make_pair(vName,bus)) != alter.end();
}


