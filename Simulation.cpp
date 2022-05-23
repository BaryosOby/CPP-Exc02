#include "Simulation.h"


void Simulation::inOutbound(const string &from, const Graph &gr, const string &message) const {
    for (int t = 0; t < 4; t++) {
        auto v = gr.BFSbytype(from, static_cast <VehicleTypes>(t));
        cout << v_types_strings[t] << ": ";
        if (v.empty()) {
            cout << message;
        }
        copy(v.begin(), v.end(), ostream_iterator<string>(cout, " "));
        cout << endl;
    }
}


void Simulation::shortestByCar(const string &from, const string &to) const {
    for (int t = 0; t < 4; t++) {
        int v = g.DijByType(from, to, static_cast <VehicleTypes>(t));
        cout << v_types_strings[t] << ": ";
        if (v < inf) {
            cout << v << endl;
        } else {
            cout << "route unavailable" << endl;
        }
    }
}

void Simulation::shortest(const string &from, const string &to) {
    int v = g.belFord(from, to);
    if (v < inf) {
        cout << "shortest time from " << from << " to " << to << ": " << v << endl;
    } else {
        cout << "route unavailable" << endl;
    }
}

void Simulation::print() const {
    ofstream out;
    out.open(output);
    out << g;
    out.close();
}

// validate input file content
bool Simulation::validInput(const string &input) {
    int spaceCounter = 0;
    string line = input, cityOrDriveTime;
    stringstream toNum;
    int driveTime;
    line += " ";
    //cityOrDriveTime element because it could be one of the vertices or the drive time between them
    for (int i = 0; i < line.size(); ++i) {
        if (line[i] == ' ' ||
            line[i] == '\t') { //if we got into space char that means we've compose the whole string for parsing
            if (cityOrDriveTime.size() > 32) throw runtime_error("City name to long!\n");
            spaceCounter++;
            if (spaceCounter == 1) { //from vertex
                tempFrom = cityOrDriveTime;
            } else if (spaceCounter == 2) { //to vertex
                tempTo = cityOrDriveTime;
            } else break; // means its a number
            cityOrDriveTime = "";
        } else {
            cityOrDriveTime += line[i];
        }
    }
    toNum << cityOrDriveTime;
    toNum >> driveTime;
    toNum.clear();
    if (driveTime < 0) throw runtime_error("Negative drive time error!\n");
    tempDriveTime = driveTime;
    return true;
}

void Simulation::validConfiguration(string &conf) {
    string line, typeOrTime;
    stringstream toNum;
    int time, spaceCounter = 0;
    bool vMatch = false, sMatch = false;
    VehicleTypes vehicleMatch;
    StationTypes stationMatch;
    conf += " ";
    for (int i = 0; i < conf.size(); ++i) {
        if (conf[i] == ' ' || conf[i] == '\t') {
            spaceCounter++; // by space counter we can know if the current value is station/vehicle or time
            if (spaceCounter > 1) break; // means its a number
            //iterate all the transport types to find where to change configuration
            for (int j = 0; j < 4; ++j) {
                if (typeOrTime == v_types_strings[j]) {
                    vehicleMatch = static_cast<VehicleTypes>(j);
                    vMatch = true; //found a match from conf file to one of the vehicles
                    break;
                }
                if (j < 3 && !vMatch) {
                    //iterate all the stations types to find where to change configuration
                    if (typeOrTime == s_types_strings[j]) {
                        stationMatch = static_cast<StationTypes>(j);
                        sMatch = true; //found a match from conf file to one of the stations
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
    if (vMatch) times.stopTimes[vehicleMatch] = time; //update stop time to the given transport type
    else if (sMatch) times.transitTimes[stationMatch] = time; //update transit time to the given station type
    else throw runtime_error("invalid data\n");
}


void Simulation::run() {
    cout << "Hello and welcome to Neverland's public transportation app." << endl;
    while (true) {

        string user_choice;
        cout << main_screen;
        getline(cin,user_choice);
        user_choice += " ";
        vector<string> input = move(split(user_choice));
        if(input.size() < 1) throw SimulationException("Missing arguments, try again");
        int choice = choices.find(input[0]) == choices.end() ? -1 : choices[input[0]];
        try {
            switch (choice) {
                case 0: {
                    if(input.size() < 2) throw SimulationException("Missing arguments, try again");
                    string path = input[1];
                    int vType;
                    fstream file;
                    //cout << "Enter a file path: ";
                    //cin >> path;
                    vType = validFileName(path);
                    if (vType < 0) {
                        throw SimulationException("Invalid file name, try again");
                    }
                    validFile(path, file);
                    getInput(file, static_cast<VehicleTypes>(vType));
                    break;
                }
                case 1: {
                    if(input.size() < 2) throw SimulationException("Missing arguments, try again");
                    string from = input[1];
                    //cout << "Enter source station name: ";
                    //cin >> from;
                    if (!g.findVertex(from)) {
                        from += " didn't found";
                        throw SimulationException(from);
                    }
                    inOutbound(from, g, outMessage);
                    break;
                }
                case 2: {
                    if(input.size() < 2) throw SimulationException("Missing arguments, try again");
                    string to = input[1];
                    //cout << "Enter destination station name: ";
                    //cin >> to;
                    if (!g.findVertex(to)) {
                        to += " didn't found";
                        throw SimulationException(to);
                    }
                    inOutbound(to, g_t, inMessage);
                    break;
                }
                case 3: {
                    if(input.size() < 3) throw SimulationException("Missing arguments, try again");
                    string from = input[1], to = input[2];
                    //cout << "Enter source station name: ";
                    //cin >> from;
                    //cout << "\nEnter destination station name: ";
                    //cin >> to;
                    if (!g.findVertex(from)) {
                        from += " didn't found";
                        throw SimulationException(from);
                    } else if (!g.findVertex(to)) {
                        to += " didn't found";
                        throw SimulationException(to);
                    }
                    shortestByCar(from, to);
                    break;
                }
                case 4: {
                    if(input.size() < 3) throw SimulationException("Missing arguments, try again");
                    string from = input[1], to = input[2];
                  //  cout << "Enter source station name: ";
                   // cin >> from;
                    //cout << "\nEnter destination station name: ";
                    //cin >> to;
                    if (!g.findVertex(from)) {
                        from += " didn't found";
                        throw SimulationException(from);
                    } else if (!g.findVertex(to)) {
                        to += " didn't found";
                        throw SimulationException(to);
                    }
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
                    throw SimulationException("ERROR: INVALID INPUT,TRY AGAIN");
            }
        }
        catch (SimulationException &e) {
            e.what();
        }
    }
}

//validate input file name
int Simulation::validFileName(const string &file) {
    int equals = 0, counter = 0;
    int fileSize = file.size();
    for (int i = 0; i < 4; ++i) {
        if (file[0] == v_types_strings[i][0]) {
            equals++;
            while (true) {
                counter++;
                if (counter == fileSize || counter == v_types_strings[i].size())
                    return -1; //if the file name dont have one of the transport time within him return -1
                if (file[equals] == v_types_strings[i][equals]) equals++;
                if (equals == v_types_strings[i].size())
                    return i; //return the type of transport according to the costume enum VehicleType
            }
        }
    }
    return -1;
}

void Simulation::addEdge(const string &data, VehicleTypes vehicle) {
    string sType;
    StationTypes fromStationType, toStationType;
    validInput(data);
    int spaceCounter = 0;
    for (int i = 0; i < 2; ++i) {
        switch (spaceCounter) {
            case 0:
                sType = tempFrom[0];
                sType += tempFrom[1];
                if (sType == "IC") fromStationType = intercity;
                else if (sType == "CS") fromStationType = central;
                else fromStationType = stad;
                sType = "";
                spaceCounter++;
                break;
            case 1:
                sType = tempTo[0];
                sType += tempTo[1];
                if (sType == "IC") toStationType = intercity;
                else if (sType == "CS") toStationType = central;
                else toStationType = stad;
                sType = "";
                spaceCounter++;
                break;
        }

    }
    g.addEdge(tempFrom, tempTo, vehicle, fromStationType, toStationType, tempDriveTime);
    g_t.addEdge(tempTo, tempFrom, vehicle, toStationType, fromStationType, tempDriveTime);
}

//check if the file is open if not throw exception
void Simulation::validFile(const string &fileName, fstream &file) {
    file.open(fileName, ios_base::in);
    if (!file.is_open()) {
        throw SimulationException(fileName + " is not open");
    }
}

Simulation::Simulation(int argc, char **argv, Times &times1) : output("output.dat"), times(times1), tempDriveTime(0) {
    string currArg, line;
    char flag = 'i';
    int vType;
    fstream file;
    for (int i = 0; i < argc; ++i) {
        currArg = argv[i];
        if (currArg == "-c") {
            currArg = argv[++i];
            validFile(currArg, file);
            while (!file.eof()) {
                getline(file, line);
                validConfiguration(line);
            }
            file.close();
            g.addTime(times);
            g_t.addTime(times);
            break;
        }
    }
    for (int i = 2; i < argc; i++) {
        currArg = argv[i];
        if (currArg == "-o" || currArg == "-c") {
            flag = currArg[1];
        } else {
            switch (flag) {
                case 'i': {
                    vType = validFileName(currArg);
                    if (vType < 0) { throw runtime_error("invalid file name"); }
                    validFile(currArg, file);
                    getInput(file, static_cast<VehicleTypes>(vType));
                    file.close();
                    break;
                }
                case 'o': {
                    validFile(currArg, file);
                    output = currArg;
                    file.close();
                }
            }
        }

    }

}

void Simulation::getInput(fstream &file, VehicleTypes vType) {
    string line;
    while (!file.eof()) {
        getline(file, line);
        addEdge(line, vType);
    }
}

vector<string> Simulation::split(string &text) {
    vector<string> words{};
    string space_delimiter = " ";
    size_t pos = 0;
    while ((pos = text.find(space_delimiter)) != string::npos) {
        words.push_back(text.substr(0, pos));
        text.erase(0, pos + space_delimiter.length());
    }
    return words;
}



