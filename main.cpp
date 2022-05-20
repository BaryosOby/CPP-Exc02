#include <cstring>
#include "Simulation.h"

int main(int argc, char *argv[]) {
    try {
        if (argc < 3) throw runtime_error("Not enough arguments!\n");
        cout << strlen(argv[1]);
        if (strcmp(argv[1], "-i") != 0) throw runtime_error("-i flag is missing!\n");
        Simulation sim(argc,argv);
        sim.run();
    } catch (runtime_error& error) {
        cerr << error.what();
        exit(0);
    }

    return 0;
}