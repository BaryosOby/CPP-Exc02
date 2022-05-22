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

/** @enum_VehicleTypes: holds values of the vehicle types, which used as indexes for Times class.
 * @enum_StationTypes: holds values of the station types, which used as indexes for Times class.
 * */

using namespace std;

enum VehicleTypes {
    bus, tram, sprinter, rail
};
enum StationTypes {
    intercity, central, stad
};
static string v_types_strings[]{"bus", "tram", "sprinter", "rail"};
static string s_types_strings[]{"intercity", "central", "stad"};
static int inf = numeric_limits<int>::max() / 2;

/** a class to hold stop and transit times, which can be modified at run-time.
 * @Big_5 : all are default, no special usage.
 * */

struct Times {
    vector<int> stopTimes = {1, 2, 3, 5};
    vector<int> transitTimes = {15, 10, 5};
};


#endif
