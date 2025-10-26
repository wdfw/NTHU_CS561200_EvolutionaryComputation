#include "Parser.hpp"
#include "BinaryGA.hpp"
#include "RealValueGA.hpp"
#include "Config.hpp"
#include "Utilities.hpp"
#include "GeneticAlgorithm.hpp"

#include <iostream>
#include <vector>
#include <ctime>

using namespace std ; 
int main(int argc, char *argv[]) {
    Configuration config;
    
    if (parse_cmd(argc, argv, config)) {
        std::cerr << "Error when parsing.\n";
        return 0;
    }
    if (config.debug)  config.print_configuration();
    
    if(config.representation=="binary"){
        BinaryGeneticAlgorithm ga(config, [&config](const vector<int>& x){return SchwefelFunction<int>(x, config.dimension);}, clock()) ;
        auto res = ga.Solve() ;
        cout << res.eval() ; 
    }else if (config.representation=="real"){
        RealValueGeneticAlgorithm ga(config, [&config](const vector<double>& x){return SchwefelFunction(x, config.dimension);}, clock()) ;
        auto res = ga.Solve() ;
        cout << res.eval()  ; 
    }else {
        throw runtime_error("Unknown Representation: " + config.representation) ; 
    }

    return 0;
}
