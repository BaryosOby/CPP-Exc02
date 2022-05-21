#include "Simulation.h"


void Simulation::inOutbound(const string &from, const Graph &gr, const string &message) const {
    for (int t = 0; t < 4; t++) {
        auto v = gr.BFSAlter(from, static_cast <VehicleTypes>(t));
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
        int v = g.DijAlter(from, to, static_cast <VehicleTypes>(t));
        cout << v_types_strings[t] << ": ";
        if (v < inf) {
            cout << v << endl;
        } else {
            cout << "route unavailable" << endl;
        }
    }
}

void Simulation::shortest(const string &from, const string &to){
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
    for (int i = 0; i < line.size(); ++i) {
        if (line[i] == ' ' || line[i] == '\t') {
            if (cityOrDriveTime.size() > 32) throw runtime_error("City name to long!\n");
            spaceCounter++;
            if(spaceCounter == 1){
                tempFrom = cityOrDriveTime;
            }
            else if(spaceCounter == 2){
                tempTo = cityOrDriveTime;
            }
            else break; // means its a number
            cityOrDriveTime = "";
        }
        else {
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
    else throw runtime_error("invalid data\n");
}

bool Simulation::validOutput(const string &output) {
    return false;
}


void Simulation::run() {
    cout << "Hello and welcome to Neverland's public transportation app." << endl;
    while (true) {
        string user_choice;
        cout << main_screen;
        cin >> user_choice;
        int choice = choices.find(user_choice) == choices.end()? -1 : choices[user_choice];
        try {
            switch (choice) {
                case 0: {
                    string path;
                    int vType;
                    fstream file;
                    cout << "Enter a file path: ";
                    cin >> path;
                    vType = validFileName(path);
                    if (vType < 0) {
                        throw SimulationException("Invalid file name, try again");
                        break;
                    }
                    validFile(path, file);
                    getInput(file, static_cast<VehicleTypes>(vType));
                    break;
                }
                case 1: {
                    string from;
                    cout << "Enter source station name: ";
                    cin >> from;
                    if (!g.findVertex(from)) {
                        from += " didn't found";
                        throw SimulationException(from);
                    }
                    inOutbound(from, g, outMessage);
                    break;
                }
                case 2: {
                    string to;
                    cout << "Enter destination station name: ";
                    cin >> to;
                    if (!g.findVertex(to)) {
                        to += " didn't found";
                        throw SimulationException(to);
                    }
                    inOutbound(to, g_t, inMessage);
                    break;
                }
                case 3: {
                    string from, to;
                    cout << "Enter source station name: ";
                    cin >> from;
                    cout << "\nEnter destination station name: ";
                    cin >> to;
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
                    string from, to;
                    cout << "Enter source station name: ";
                    cin >> from;
                    cout << "\nEnter destination station name: ";
                    cin >> to;
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
        catch(SimulationException& e){
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
                if (counter == fileSize || counter == v_types_strings[i].size()) return -1;
                if (file[equals] == v_types_strings[i][equals]) equals++;
                if (equals == v_types_strings[i].size()) return i;
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
    g.addEdgeAlter(tempFrom, tempTo, vehicle, fromStationType, toStationType,  tempDriveTime);
    g_t.addEdgeAlter(tempTo, tempFrom, vehicle, toStationType, fromStationType, tempDriveTime);
}

void Simulation::validFile(const string &fileName, fstream& file) {
    file.open(fileName, ios_base::in);
    if(!file.is_open()){
        throw SimulationException(fileName + " is not open");
    }
}

Simulation::Simulation(int argc, char **argv): output("output.dat"){
    string currArg, line;
    char flag = 'i';
    int vType;
    fstream file;
    for(int i = 2; i < argc; i++){
        currArg = argv[i];
        if(currArg == "-o" || currArg == "-c"){
            flag = currArg[1];
        }
        else{
            switch (flag) {
                case 'i':{
                    vType = validFileName(currArg);
                    if(vType < 0){ throw runtime_error("invalid file name"); }
                    validFile(currArg,file);
                    getInput(file,static_cast<VehicleTypes>(vType));
                    file.close();
                    break;
                }
                case 'c':{
                    validFile(currArg, file);
                    while(!file.eof()){
                        getline(file,line);
                        validConfiguration(line);
                    }
                    file.close();
                    break;
                }
                case 'o':{
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
    while(!file.eof()){
        getline(file,line);
        addEdge(line, vType);
    }
}



