#ifndef EXC02_SIMULATION_H
#define EXC02_SIMULATION_H
#include "Graph.h"


class Simulation {
private:
    Graph g;
    Graph g_t; //g.transpose
    std::ostream output;

    const string outMessage = "no outbound travel";
    const string inMessage = "no inbound travel";

    map<string, int> choices = {{"load", 0}, {"outbound", 1},
                                {"inbound", 2} , {"uniExpress", 3}, {"multiExpress", 4},
                                {"print", 5}, {"EXIT", 6}};

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


    // TODO exceptions of invalid input

    void load(const string& input);

    // bfs on g
    void inOutbound(const string& from, const Graph& gr, const string& message) const;  // TODO throw exceptions

    void shortestByCar(const string& from, const string& to) const;
    void shortest(const string& from, const string& to) const;
    void print() const;

    bool validInput(const string& input);        //TODO throw exceptions
    bool validConfiguration(const string& conf);
    bool validOutput(const string& output);
    bool validChoice(const string& output);


public:
    Simulation(const string& input, const string& conf="", const string& output="");
    // TODO big 5
    void run();

};


#endif
