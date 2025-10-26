#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>

using std::cout; using std::string; using std::to_string; using std::setw; using std::left; 
class Configuration {
public:
    Configuration() = default; 

    int dimension = 10;
    string representation = "binary";
    int population_size = 100;
    bool uniform_crossover = false;
    string crossover_method = "2-point";
    float cross_prob = 0.9;
    float mut_prob = 0.1;
    int num_generations = 500;

    int tournament_size = 2 ; 
    float alpha1 = 0.5 ; // Uniform Crossover Pr
    float alpha2 = 0.2 ; // Whole Arithmetic Crossover 

    bool debug = false;

    void defaultSetting(){
        dimension = 10 ; 
        population_size = 100 ; 
        cross_prob = 0.9;
        mut_prob = 0.1;
        num_generations = 500 ; 
        tournament_size = 2 ; 
        alpha1 = 0.5 ; // Uniform Crossover Pr
        alpha2 = 0.2 ; // Whole Arithmetic Crossover 
    }
    string toString() const {
        string res = to_string(dimension) + "," + representation + "," + to_string(population_size) + "," + to_string(tournament_size) + "," + to_string(uniform_crossover) + "," +  crossover_method + "," 
                     + to_string(cross_prob) + "," + to_string(mut_prob) + "," + to_string(num_generations) ;
        return res ; 
    }
    void print_configuration() const {
        auto configurations = get_configurations();
        const int column_width = 20;
        const int num_columns = 2;
        const int frame_width = num_columns * column_width + 3;
        
        cout << string(frame_width, '-') << "\n";
        cout << '|' << left << setw(column_width) << "Parameter"
             << '|' << left << setw(column_width) << "Value" << '|' << "\n";
        cout << string(frame_width, '-') << "\n";
        
        for (const auto& [key, value] : configurations) {
            cout << '|' << left << setw(column_width) << key 
                 << '|' << left << setw(column_width) << value << "|\n";
        }
        cout << string(frame_width, '-') << "\n";
    }

private:
    std::vector<std::pair<string, string>> get_configurations() const {
        string cross_prob_num = to_string(cross_prob);
        string cross_prob_rounded = cross_prob_num.substr(0, cross_prob_num.find(".")+4);

        string mut_prob_num = to_string(mut_prob);
        string mut_prob_rounded = mut_prob_num.substr(0, mut_prob_num.find(".")+4);
      
        return {
            {"dimension", to_string(dimension)},
            {"representation", representation},
            {"population_size", to_string(population_size)},
            {"uniform_crossover", uniform_crossover ? "true" : "false"},
            {"crossover_method", crossover_method},
            {"cross_prob", cross_prob_rounded},
            {"mut_prob", mut_prob_rounded},
            {"num_generations", to_string(num_generations)},
            {"tournament_size", to_string(tournament_size)},
            {"alpha1", to_string(alpha1)},
            {"alpha2", to_string(alpha2)}
            // {"debug", (debug) ? "true" : "false"},
        };
    }
};
