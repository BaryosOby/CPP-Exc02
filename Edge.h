#ifndef EXC02_EDGE_H
#define EXC02_EDGE_H
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <memory>
#include <queue>
#include <iterator>
#include <exception>
#include <sstream>
#include <limits>

using namespace std;

enum VehicleTypes {bus, tram, sprinter, rail};
enum StationTypes {intercity, central, stad};

static string v_types_strings[] {"bus", "tram", "sprinter", "rail"};
static string s_types_strings[] {"intercity", "central", "stad"};
static int inf = numeric_limits<int>::max() / 2;

struct Times{
    map<StationTypes, int> transitTimes;
    map<VehicleTypes, int> stopTimes;

    Times(){
        transitTimes.insert(pair<StationTypes, int>(intercity, 15));
        transitTimes.insert(pair<StationTypes, int>(central, 10));
        transitTimes.insert(pair<StationTypes, int>(stad, 5));

        stopTimes.insert(pair<VehicleTypes, int>(bus, 1));
        stopTimes.insert(pair<VehicleTypes, int>(tram, 2));
        stopTimes.insert(pair<VehicleTypes, int>(sprinter, 3));
        stopTimes.insert(pair<VehicleTypes, int>(rail, 5));
    }
};

static Times times;

class Edge {
private:
    string source;
    string dest;
    VehicleTypes vehicleType;
    StationTypes sourceStationType;
    int driveTime;


public:
    Edge(const string& from, const string& to, VehicleTypes vType, StationTypes sType, int drive): source(from),
    dest(to), vehicleType(vType), sourceStationType(sType), driveTime(drive){
    }

    int getDriveTime() const{
        return driveTime;
    }

    int getStopTime() const{
        return times.stopTimes[vehicleType];
    }

    int getTransit() const{
        return times.transitTimes[sourceStationType];
    }

    const string& getDest() const{
        return dest;
    }

    void setDriveTime(int newTime){
        driveTime = min(driveTime, newTime);
    }

    VehicleTypes getVtype(){
        return vehicleType;
    }

    friend ostream& operator<<(ostream& out, const Edge& e);

};

inline ostream& operator<<(ostream& out, const Edge& e){
    out << "\t" << e.dest << " by "<< v_types_strings[e.vehicleType] << endl;
    return out;
}

#endif
