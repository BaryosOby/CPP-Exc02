#ifndef EXC02_TIMES_H
#define EXC02_TIMES_H
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

Times times;

#endif
