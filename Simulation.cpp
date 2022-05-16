#include "Simulation.h"

void Simulation::load(const string &input) {

}

void Simulation::inOutbound(const string& from, const Graph& gr, const string& message) const {
    for(int t = bus; t != rail; t++){
        auto v = g.BFSbyType(from, static_cast <VehicleTypes>(t));
        cout << v_types_strings[t] << ": ";
        if(v.empty()){
            cout << message << endl;
        }
        copy(v.begin(), v.end(), ostream_iterator<string>(cout, " "));
        cout << endl;
    }
}


void Simulation::shortestByCar(const string& from, const string& to) const {
    for(int t = bus; t != rail; t++){
        int v = g.DijByType(from, to, static_cast <VehicleTypes>(t));
        cout << v_types_strings[t] << ": ";
        if(v < RAND_MAX){
            cout << v << endl;
        }
        else{
            cout << "route unavailable" << endl;
        }
    }
}

void Simulation::shortest(const string& from, const string& to) const {
    int v = g.Dij(from, to);
    if(v < RAND_MAX){
        cout << "shortest time from " << from << " to " << to << ": " << v << endl;
    }
    else{
        cout << "route unavailable" << endl;
    }
}

void Simulation::print() const {
    output.
}

bool Simulation::validInput(const string &input) {
    return false;
}

bool Simulation::validConfiguration(const string &conf) {
    return false;
}

bool Simulation::validOutput(const string &output) {
    return false;
}

Simulation::Simulation(const string &input, const string &conf, const string &out):g(), g_t(), output(out){}

void Simulation::run() {
    cout << "Hello and welcome to Neverland's public transportation app." << endl;
    while(true){
        string user_choice;
        cout << main_screen;
        cin >> user_choice;
        // TODO check user_choice
        switch(choices[user_choice]){
            case 0: {
                string path;
                cout << "Enter a file path: ";
                cin >> path;
                validInput(path);
                load(path); // TODO maybe will get a fstream from valid()
                break;
            }
            case 1:{
                string from;
                cout << "Enter source station name: ";
                cin >> from;
                // TODO find if source in graph
                inOutbound(from, g, outMessage);
                break;
            }
            case 2:{
                string to;
                cout << "Enter destination station name: ";
                cin >> to;
                // TODO find if to in graph
                inOutbound(to, g_t, inMessage);
                break;
            }
            case 3:{
                string from, to;
                cout << "Enter source station name: ";
                cin >> from;
                cout << "\nEnter destination station name: ";
                cin >> to;
                // TODO find stations in graph
                shortestByCar(from, to);
                break;
            }
            case 4:{
                string from, to;
                cout << "Enter source station name: ";
                cin >> from;
                cout << "\nEnter destination station name: ";
                cin >> to;
                // TODO find stations in graph
                shortest(from, to);
                break;
            }
            case 5:{
                print();
                break;
            }
            case 6:{
                cout << "Thank you, Goodbye";
                exit(0);
            }
            default: continue;
        }
    }

}
