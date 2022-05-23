#ifndef EXC02_SIMULATION_H
#define EXC02_SIMULATION_H

#include "Graph.h"

/** Runs the program.
 * responsible for input validation.
 *
 * holds two Graphs: g and its transpose. when adding an edge and vertex, both are updated.
 *
 * @class_member g: a graph of the city's roadmap.
 * @class_member g_t: g transpose. g with its edges directed the other way. used for the 'inbound' query.
 * @class_member output: name of the output file. default inits to "output.dat".
 * @class_member times: a Times object that holds the updated stop and transit times.
 * @class_member tempFrom, tempTo. tempDriveTime: used for the edge adding process.
 *
 * @Big_5: copy and move constructors and operators= are deleted. a Simulation object isn't purposed to be copied or move.
 *         destructor is default - no dynamic allocated memory.
 * */
class Simulation {
private:
    Graph g;
    Graph g_t;
    string output;
    Times& times;


    string tempFrom;
    string tempTo;
    int tempDriveTime;

    const string outMessage = "no outbound travel";
    const string inMessage = "no inbound travel";
    map<string, int> choices = {{"load",         0},
                                {"outbound",     1},
                                {"inbound",      2},
                                {"uniExpress",   3},
                                {"multiExpress", 4},
                                {"print",        5},
                                {"EXIT",         6}};

    const string main_screen = "Enter a command to operate:\n"
                               "\t1. 'load' to load an input file.\n"
                               "\t2. 'outbound' to see all the stations that can be reached from a certain source station.\n"
                               "\t3. 'inbound' to see all the stations that reach to a certain destination station.\n"
                               "\t4. 'uniExpress' to see the shortest amount of times to reach\n"
                               "from a certain source station to a destination, classified by vehicle type\n"
                               "\t5. 'multiExpress' to see the shortest amount of time to reach\n"
                               "from a certain source station to a destination, with transits\n"
                               "\t6. 'print' to print Neverland's roadmap to the output.\n"
                               "\t7. 'EXIT' to quit.\n";


    vector<string> split(string& text);

    void load(const string& path);

    void outboundFunc(const string& from) const;

    void inboundFunc(const string& to) const;

    void inOutbound(const string &from, const Graph &gr, const string &message) const;

    void shortestByCar(string &from, string &to) const;

    void shortest(string &from, string &to);

    void print() const;

    bool validInput(const string &input);

    void validConfiguration(string &conf);

    int validFileName(const string &file); //added to check legal file name for vehicle info

    void validFile(const string& fileName, fstream& file);

    void getInput(fstream& file, VehicleTypes vType);

    void addEdge(const string& data,VehicleTypes vehicle); //adding edge from this class after validation as well to g_t

public:
    class SimulationException{
        string error;
    public:
        explicit SimulationException(string err): error(err){}
        void what(){
            cerr << error << endl;
        }
    };

    Simulation(int argc, char** argv,Times& timesRHS);
    Simulation(Simulation& rhs) = delete;
    Simulation(Simulation&& rhs) = delete;
    Simulation& operator=(Simulation& rhs) = delete;
    Simulation& operator=(Simulation&& rhs) = delete;
    ~Simulation() = default;
    void run();
};


#endif
