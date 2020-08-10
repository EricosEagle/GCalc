#include "graph/gcalc.h"
#include <fstream>

int main(int argc, char** argv) {
    if(argc != 1 && argc != 3) {
        throw std::invalid_argument("Invalid paramaters. Usage: " + std::string(argv[0]) + " [infile] [outfile].");
    }
    GCalc gcalc = (argc == 3) ? GCalc(new std::ifstream(argv[1]), new std::ofstream(argv[2])):GCalc();
    gcalc.run();
    return 0;
}
