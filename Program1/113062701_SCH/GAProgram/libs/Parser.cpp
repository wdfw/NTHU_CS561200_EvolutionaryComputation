#include "Parser.hpp"
#include <iostream>
#include <getopt.h>
#include <cstring>
#include <map>
#include <string>
#include <vector>

using std::cout; using std::string;

void determine_crossover_method(Configuration& config) {
    if (!(config.uniform_crossover)) {
        if (config.representation == "binary") {
            config.crossover_method = "2-point";
        } else if (config.representation == "real") {
            config.crossover_method = "whole arithmetic";
        } 
    }
    return;
}


void usage(const char* progname) {
    std::cerr << "Usage: " << progname << " [options] [values]\n"
              << "Program Options:\n"
              << "  -n  --dimension <INT>          The dimension of Schwefel function (default: 10) \n"
              << "  -r  --representation <STRING>  The representation to use. Binary or real-valued (binary, real) (default: binary) \n"
              << "  -p  --population_size <INT>    Number of the population (default: 100) \n"
              << "  -u  --uniform_crossover <BOOL> The crossover method using uniform crossover (1) or not (0). If not, then for binary GA, it will use 2-point crossover and for real-valued GA will use whole arithmetic crossover (default: 0) \n"
              << "  -c  --pc <DOUBLE>              Probability for the crossover (default: 0.9) \n"
              << "  -m  --pm <DOUBLE>              Probability for the mutation (default: 0.1)  \n"
              << "  -g  --generations <INT>        Max number of generations to terminate (default: 500) \n"
              << "  -k  --tournament_size <INT>    Implement k-tournament selection (default: 2) \n"
              << "  -a1 --alpha1 <DOUBLE>           Probability of swapping at each position in uniform crossover (default: 0.5) \n"
              << "  -a2 --alpha2 <DOUBLE>           Weight in whole arithmetic crossover (default: 0.2) \n"
              << "  -d  --debug <BOOL>             Turn on debug prints (default: false)\n"
              << "  -?  --help                     This message\n";
    return;

}

int parse_cmd(int argc, char * const argv[], Configuration& config) {
    int opt;
    static struct option long_options[] = {
        {"dimension", 1, NULL, 'n'},
        {"representation", 1, NULL, 'r'},
        {"population_size", 1, NULL, 'p'},
        {"uniform_crossover", 1, NULL, 'u'},
        {"pc", 1, NULL, 'c'},
        {"pm", 1, NULL, 'm'},
        {"generations", 1, NULL, 'g'},

        {"tournament_size", 1, NULL, 'k'},
        {"alpha1", 1, NULL, 'a'},
        {"alpha2", 1, NULL, 'b'},
        {"a1", 1, NULL, 'a'},
        {"a2", 1, NULL, 'b'},

        {"help", 0, NULL, '?'},
        {"debug", 0, NULL, 'd'},
        {0, 0, 0, 0},
    };

    while ((opt = getopt_long(argc, argv, "?n:r:p:u:c:m:g:k:d", long_options, NULL)) != EOF) {
        switch (opt) {
            case 'n':
                config.dimension = atoi(optarg);
                if (config.dimension <= 0) {
                    fprintf(stderr, "dimension must be positive!\n");
                    return 1;
                }
                break;
            case 'r':
                if (strcmp(optarg, "binary") == 0 || strcmp(optarg, "real") == 0) {
                    config.representation = optarg;
                } else {
                    fprintf(stderr, "Wrong representation! Choose 'binary' or 'real'.\n");
                    return 1;
                }
                break;
            case 'p':
                config.population_size = atoi(optarg);
                if (config.population_size <= 0) {
                    fprintf(stderr, "population size must be positive!\n");
                    return 1;
                }
                break;
            case 'u':
                if (strcmp(optarg, "1") == 0) {
                    config.uniform_crossover = true;
                    config.crossover_method = "uniform";
                } else if (strcmp(optarg, "0") == 0){
                    config.uniform_crossover = false;
                } else {
                    fprintf(stderr, "Wrong value to set uniform crossover!\n");
                    return 1;
                }
                break;
            case 'c':
                config.cross_prob = atof(optarg);
                if (config.cross_prob < 0 || config.cross_prob > 1) {
                    fprintf(stderr, "crossover probability must be a valid probability!\n");
                    return 1;
                }
                break;
            case 'm':
                config.mut_prob = atof(optarg);
                if (config.mut_prob < 0 || config.mut_prob > 1) {
                    fprintf(stderr, "mutation probability must be a valid probability!\n");
                    return 1;
                }
                break;
            case 'g':
                config.num_generations = atoi(optarg);
                if (config.num_generations <= 0) {
                    fprintf(stderr, "number of generations must be positive!\n");
                    return 1;
                }
                break;
            case 'd':
                config.debug = true;
                break;
            case 'a' :
                config.alpha1 = atof(optarg); ;
                if (config.alpha1 < 0 || config.alpha1 > 1) {
                    fprintf(stderr, "alpha1 must in range [0.0, 1.0]!\n");
                    return 1;
                }
                break;
            case 'b' :
                config.alpha2 = atof(optarg); ;
                if (config.alpha2 < 0 || config.alpha2 > 1) {
                    fprintf(stderr, "alpha2 must in range [0.0, 1.0]!\n");
                    return 1;
                }
                break;
            case 'k' :
                config.tournament_size = atoi(optarg); ;
                if (config.tournament_size <= 0) {
                    fprintf(stderr, "tournament size must be positive!\n");
                    return 1;
                }
                break;
            case '?':
                usage(argv[0]);
                return 0;
        }
    }
    determine_crossover_method(config);
    return 0;
}