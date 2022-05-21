#ifndef EXC02_EDGE_H
#define EXC02_EDGE_H


#include "Times.h"

class Edge {
private:
    string source;
    string dest;
    VehicleTypes vehicleType;
    StationTypes sourceStationType;
    int driveTime;
    Times &times;

public:
    Edge(const string &from, const string &to, VehicleTypes vType, StationTypes sType, int drive, Times &timesRHS)
            : source(from),
              dest(to), vehicleType(vType), sourceStationType(sType), driveTime(drive), times(timesRHS) {
    }

    int getDriveTime() const {
        return driveTime;
    }

    int getStopTime() const {
        return times.stopTimes[vehicleType];
    }

    int getTransit() const {
        return times.transitTimes[sourceStationType];
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
