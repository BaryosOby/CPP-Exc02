#ifndef EXC02_EDGE_H
#define EXC02_EDGE_H
#include "Times.h"
/** Represents directed weighted edge in a graph (road in the simulation).
 * an Edge is specified for a certain vehicle type.
 *
 * @class_member source, dest: names of head and tail vertices of the edge.
 * @class_member vehicleType: type of vehicle that the edge is built for.
 * @class_member driveTime: edge's weight.
 * @class member times: holds a Times object, to calculate its stop time for the total weight.
 *
 * @Big_5: all default. no dynamic allocated memory.
 * */

class Edge {
private:
    string source;
    string dest;
    VehicleTypes vehicleType;
    int driveTime;
    Times &times;

public:
    Edge(const string &from, const string &to, VehicleTypes vType, int drive, Times &timesRHS)
    : source(from), dest(to), vehicleType(vType), driveTime(drive), times(timesRHS) {
    }

    int getDriveTime() const {
        return driveTime;
    }

    int getStopTime() const {
        return times.stopTimes[vehicleType];
    }

    const string &getDest() const {
        return dest;
    }

    void setDriveTime(int newTime) {
        driveTime = min(driveTime, newTime);
    }

    VehicleTypes getVtype() {
        return vehicleType;
    }

    friend ostream &operator<<(ostream &out, const Edge &e);

};

inline ostream &operator<<(ostream &out, const Edge &e) {
    out << "\t" << e.dest << " by " << v_types_strings[e.vehicleType] << endl;
    return out;
}

#endif
