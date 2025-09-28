#include "parser.h"
// #include "config.h"
#include "ga.h"
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
    Configuration config;
    
    if (parse_cmd(argc, argv, config)) {
        std::cerr << "Error when parsing.\n";
        return 0;
    }
    if (config.debug)  config.print_configuration();
    
    // TODO: write your GA program
    int n = 10 ;
    GeneticAlgorithm ga(config, [&n](const vector<int>& x){return SchwefelFunction(x, n);}) ;
    ga.Initialize() ; 
    for(int i=0; i<ga.candidates.size(); i++){
        cout << ga.candidates[i] << "\n" ;
    }
    return 0;
}
