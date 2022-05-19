#include "Simulation.h"

void Simulation::load(const string &input) {

}

void Simulation::inOutbound(const string &from, const Graph &gr, const string &message) const {
    for (int t = bus; t != rail; t++) {
        auto v = gr.BFSAlter(from, static_cast <VehicleTypes>(t));
        cout << v_types_strings[t] << ": ";
        if (v.empty()) {
            cout << message << endl;
        }
        copy(v.begin(), v.end(), ostream_iterator<string>(cout, " "));
        cout << endl;
    }
}


void Simulation::shortestByCar(const string &from, const string &to) const {
    for (int t = bus; t != rail; t++) {
        int v = g.DijAlter(from, to, static_cast <VehicleTypes>(t));
        cout << v_types_strings[t] << ": ";
        if (v < RAND_MAX) {
            cout << v << endl;
        } else {
            cout << "route unavailable" << endl;
        }
    }
}

void Simulation::shortest(const string &from, const string &to){
    int v = g.belFord(from, to);
    if (v < RAND_MAX) {
        cout << "shortest time from " << from << " to " << to << ": " << v << endl;
    } else {
        cout << "route unavailable" << endl;
    }
}

void Simulation::print() const {

}

bool Simulation::validInput(const string &input) {
    int spaceCounter = 0;
    string line = input, cityOrDriveTime;
    stringstream toNum;
    int driveTime;
    line += " ";
    for (int i = 0; i < line.size(); ++i) {
        if (line[i] == ' ' || line[i] == '\t') {
            if (cityOrDriveTime.size() > 32) throw runtime_error("City name to long!\n");
            spaceCounter++;
            if (spaceCounter > 2) break; // means its a number
            cityOrDriveTime = "";
        } else {
            cityOrDriveTime += line[i];
        }
    }
    toNum << cityOrDriveTime;
    toNum >> driveTime;
    toNum.clear();
    if (driveTime < 0) throw runtime_error("Negative drive time error!\n");
    return true;
}

bool Simulation::validConfiguration(string &conf) {
    string line, typeOrTime;
    stringstream toNum;
    int time, spaceCounter = 0;
    bool vMatch = false, sMatch = false;
    VehicleTypes vehicleMatch;
    StationTypes stationMatch;
    conf += " ";
    for (int i = 0; i < conf.size(); ++i) {
        if (conf[i] == ' ' || conf[i] == '\t') {
            spaceCounter++;
            if (spaceCounter > 1) break; // means its a number
            for (int j = 0; j < 4; ++j) {
                if (typeOrTime == v_types_strings[j]) {
                    vehicleMatch = static_cast<VehicleTypes>(j);
                    vMatch = true;
                    break;
                }
                if (j < 3 && !vMatch) {
                    if (typeOrTime == s_types_strings[j]) {
                        stationMatch = static_cast<StationTypes>(j);
                        sMatch = true;
                        break;
                    }
                }
            }
            typeOrTime = "";
        } else {
            typeOrTime += conf[i];
        }
    }
    toNum << typeOrTime;
    toNum >> time;
    if (time < 0) throw runtime_error("Negative drive time ERROR!\n");
    //changing configuration ->
    if (vMatch) times.stopTimes[vehicleMatch] = time;
    else if (sMatch) times.transitTimes[stationMatch] = time;
    else return false;
}

bool Simulation::validOutput(const string &output) {
    return false;
}
//
//Simulation::Simulation(const string &input, const string &conf, const string &out):g(), g_t(){
//
//}

void Simulation::run() {
    cout << "Hello and welcome to Neverland's public transportation app." << endl;
    while (true) {
        string user_choice;
        cout << main_screen;
        cin >> user_choice;
        // TODO check user_choice
        switch (choices[user_choice]) {
            case 0: {
                string path;
                cout << "Enter a file path: ";
                cin >> path;
                validInput(path);
                load(path); // TODO maybe will get a fstream from valid()
                break;
            }
            case 1: {
                string from;
                cout << "Enter source station name: ";
                cin >> from;
                // TODO find if source in graph
                inOutbound(from, g, outMessage);
                break;
            }
            case 2: {
                string to;
                cout << "Enter destination station name: ";
                cin >> to;
                // TODO find if to in graph
                inOutbound(to, g_t, inMessage);
                break;
            }
            case 3: {
                string from, to;
                cout << "Enter source station name: ";
                cin >> from;
                cout << "\nEnter destination station name: ";
                cin >> to;
                // TODO find stations in graph
                shortestByCar(from, to);
                break;
            }
            case 4: {
                string from, to;
                cout << "Enter source station name: ";
                cin >> from;
                cout << "\nEnter destination station name: ";
                cin >> to;
                // TODO find stations in graph
                shortest(from, to);
                break;
            }
            case 5: {
                print();
                break;
            }
            case 6: {
                cout << "Thank you, Goodbye";
                exit(0);
            }
            default:
                cout << "ERROR: INVALID INPUT,TRY AGAIN\n";
                continue;
        }
    }

}

int Simulation::validFileName(const string &file) {
    int equals = 0, counter = 0;
    int fileSize = file.size();
    for (int i = 0; i < v_types_strings->size(); ++i) {
        if (file[0] == v_types_strings[i][0]) {
            equals++;
            while (true) {
                counter++;
                if (counter == fileSize || counter == v_types_strings[i].size()) return -1;
                if (file[equals] == v_types_strings[i][equals]) equals++;
                if (equals == v_types_strings[i].size()) return i;
            }
        }
    }
}

void Simulation::addEdge(const string &data, VehicleTypes vehicle) {
    string line = data;
    string from, to, cityOrDriveTime, sType;
    StationTypes stationType;
    stringstream toNum;
    int driveTime, spaceCounter = 0;
    line += " ";
    for (int i = 0; i < line.size(); ++i) {
        if (line[i] == ' ' || line[i] == '\t') {
            switch (spaceCounter) {
                case 0:
                    from = cityOrDriveTime;
                    sType = cityOrDriveTime[0];
                    sType += cityOrDriveTime[1];
                    if (sType == "IC") stationType = intercity;
                    else if (sType == "CS") stationType = central;
                    else stationType = stad;
                    cityOrDriveTime = "";
                    spaceCounter++;
                    break;
                case 1:
                    to = cityOrDriveTime;
                    cityOrDriveTime = "";
                    spaceCounter++;
                    break;
                case 2:
                    toNum << cityOrDriveTime;
                    toNum >> driveTime;
                    toNum.clear();
                    break;
            }
        } else {
            cityOrDriveTime += line[i];
        }
    }
    g.addEdge(from, to, vehicle, stationType, driveTime);
    g_t.addEdge(to, from, vehicle, stationType, driveTime);
}



