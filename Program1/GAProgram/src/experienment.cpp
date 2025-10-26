#include "Parser.hpp"
#include "BinaryGA.hpp"
#include "RealValueGA.hpp"
#include "Config.hpp"
#include "Utilities.hpp"
#include "GeneticAlgorithm.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <limits>
#include <cmath>

using namespace std ; 


class ExpirenmentRealValueGeneticAlgorithm : public RealValueGeneticAlgorithm {
public:
    using RealValueGeneticAlgorithm::RealValueGeneticAlgorithm ; 

    int callCount = 0 ; 
    int samplePeriod ;  
    vector<pair<int, double>> averageFitnesses ;
    vector<pair<int, double>> minimumFitnesses ;

    ExpirenmentRealValueGeneticAlgorithm(const Configuration& config, function<double(const PhenotypeType&)> fn, int seed=0, int samplePeriod=1) :
                        RealValueGeneticAlgorithm(config, fn, seed), samplePeriod(samplePeriod) {}

    void EvaluateAll(vector<ChromosomeType>& population) override {
        double sum = 0, minValue = numeric_limits<double>::max() ;
        for(int i=0; i<population.size(); i++){
            sum += population[i].eval() ; 
            minValue = std::min(minValue, population[i].fitness) ;
        }
        if(callCount%samplePeriod==0){
            minimumFitnesses.push_back({minimumFitnesses.size(), minValue}) ; 
            averageFitnesses.push_back({averageFitnesses.size(), sum/population.size()}) ; 
        }
        ++callCount ; 

    } 

} ;

class ExpirenmentBinaryGeneticAlgorithm : public BinaryGeneticAlgorithm {
public:
    using BinaryGeneticAlgorithm::BinaryGeneticAlgorithm ; 
    vector<pair<int, double>> averageFitnesses ;
    vector<pair<int, double>> minimumFitnesses ;

    int callCount = 0 ; 
    int samplePeriod ;  

    ExpirenmentBinaryGeneticAlgorithm(const Configuration& config, function<double(const PhenotypeType&)> fn, int seed=0, int samplePeriod=1) :
                        BinaryGeneticAlgorithm(config, fn, seed), samplePeriod(samplePeriod) {}
    void EvaluateAll(vector<ChromosomeType>& population) override {
        double sum = 0, minValue = numeric_limits<double>::max() ;
        for(int i=0; i<population.size(); i++){
            sum += population[i].eval() ; 
            minValue = std::min(minValue, population[i].fitness) ;
        }
        if(callCount%samplePeriod==0){
            minimumFitnesses.push_back({minimumFitnesses.size(), minValue}) ; 
            averageFitnesses.push_back({averageFitnesses.size(), sum/population.size()}) ; 
        }
        ++callCount ; 
    } 
} ;

string GetCrossoverMethod(const Configuration& config){
    if(config.uniform_crossover) return "uniform" ;
    else if(config.representation=="binary") return "2-point" ;
    else if(config.representation=="real") return "whole arithmetic" ;
    return "unknown" ;
}
void testing(int testNum, Configuration& config,  int samplePeriod = 1, ostream& os=std::cout){
    vector<double> averageFitnesses, minimumFitnesses, variances ; 

    vector<vector<double>> allRecords = vector<vector<double>>(testNum) ; 
    config.print_configuration() ;

    // string outString1 = "Average," + config.toString() ;  
    string outString2 = "Minimum," + config.toString() ;  

    for(int i=0; i<testNum; ++i){
        cout << "\r" << i+1 << "/" << testNum ; cout.flush();
        if(config.representation=="binary"){
            ExpirenmentBinaryGeneticAlgorithm BinaryGA(config, [&config](const vector<int>& x){return SchwefelFunction<int>(x, config.dimension);}, clock(), samplePeriod) ;
            BinaryGA.Solve() ; 

            if(!averageFitnesses.size()) averageFitnesses.resize(BinaryGA.averageFitnesses.size(), 0.0) ; 
            for(int i=0; i<BinaryGA.averageFitnesses.size(); ++i) averageFitnesses[i] += BinaryGA.averageFitnesses[i].second ; 

            if(!minimumFitnesses.size()) minimumFitnesses.resize(BinaryGA.minimumFitnesses.size(), 0.0) ; 
            for(int i=0; i<BinaryGA.minimumFitnesses.size(); ++i) minimumFitnesses[i] += BinaryGA.minimumFitnesses[i].second ; 

            if(!allRecords[i].size()) allRecords[i].resize(BinaryGA.minimumFitnesses.size(), 0.0) ; 
            for(int j=0; j<BinaryGA.minimumFitnesses.size(); ++j) allRecords[i][j] = BinaryGA.minimumFitnesses[j].second ; 
            

        }else{
            ExpirenmentRealValueGeneticAlgorithm RealGA(config, [&config](const vector<double>& x){return SchwefelFunction<double>(x, config.dimension);}, clock(), samplePeriod) ;
            RealGA.Solve() ; 

            if(!averageFitnesses.size()) averageFitnesses.resize(RealGA.averageFitnesses.size(), 0.0) ; 
            for(int i=0; i<RealGA.averageFitnesses.size(); ++i) averageFitnesses[i] += RealGA.averageFitnesses[i].second ; 
            
            if(!minimumFitnesses.size()) minimumFitnesses.resize(RealGA.minimumFitnesses.size(), 0.0) ; 
            for(int i=0; i<RealGA.minimumFitnesses.size(); ++i) minimumFitnesses[i] += RealGA.minimumFitnesses[i].second ; 

            if(!allRecords[i].size()) allRecords[i].resize(RealGA.minimumFitnesses.size(), 0.0) ; 
            for(int j=0; j<RealGA.minimumFitnesses.size(); ++j) allRecords[i][j] += RealGA.minimumFitnesses[j].second ; 
        }
    }

    // for(auto& r : averageFitnesses){
    //     r /= testNum ;
    //     outString1 +=  "," + to_string(r)  ; 
    // }
    for(auto& r : minimumFitnesses){
        r /= testNum ;
        outString2 += "," + to_string(r)  ; 
    }

    variances = vector<double>(allRecords.front().size(), 0) ; 
    for(int i=0; i<allRecords.front().size(); ++i){
        double var = 0.0 ; 
        for(int j=0; j<testNum; ++j){
            var += pow(allRecords[j][i]-minimumFitnesses[i], 2) ;
        }
        var /= (testNum-1) ; 
        variances[i] = sqrt(var) ; 
    }

    // cout << "\nAve: " << averageFitnesses.back() ; 
    cout << "\nBest: " << minimumFitnesses.back() ;
    cout << "\nVar: " << variances.back() ; 
    cout << "\n" ;

    // os << outString1 << "\n" ;
    os << outString2 << "\n" ;
}
void RunFourMethod(Configuration& config, int testNum, int samplePeriod=1, ostream& os=std::cout){
    config.representation = "binary" ; config.uniform_crossover = false ; config.crossover_method = GetCrossoverMethod(config) ;
    testing(testNum, config, samplePeriod, os) ;

    config.representation = "binary" ; config.uniform_crossover = true ; config.crossover_method = GetCrossoverMethod(config) ;
    testing(testNum, config, samplePeriod, os) ;

    config.representation = "real" ; config.uniform_crossover = false ; config.crossover_method = GetCrossoverMethod(config) ;
    testing(testNum, config, samplePeriod, os) ;

    config.representation = "real" ; config.uniform_crossover = true ; config.crossover_method = GetCrossoverMethod(config) ;
    testing(testNum, config, samplePeriod, os) ;
}

int main(int argc, char *argv[]) {
    int testNum = 120 ; 
    int samplePeriod = 1 ;
    Configuration config;

    string folder = "NewExpirenments/" ; 
    ofstream csvFile ; 

    // csvFile.open(folder+"CrossoverAndMutation.csv", ofstream::out) ; csvFile << "Result Type, Dimension, Representation, Population Size, Tournament Size, is Uniform Crossover, Crossover Method, Crossover Prob., Mutation Prob., # of Generations\n" ;
    // config.defaultSetting() ; 
    // for(int i=9; i<=9; ++i){
    //     for(int j=1; j<=1; ++j){
    //         config.cross_prob = 0.1*i ; config.mut_prob = 0.1*j ; 
    //         RunFourMethod(config, testNum, samplePeriod, csvFile) ; 
    //     }
    // }
    // csvFile.close() ;
    
    // csvFile.open(folder+"TorunmentSize.csv", ofstream::out) ; csvFile << "Result Type, Dimension, Representation, Population Size, Tournament Size, is Uniform Crossover, Crossover Method, Crossover Prob., Mutation Prob., # of Generations\n" ;
    // config.defaultSetting() ; 
    // for(auto& i : [2,3,4,5,6,7,8,9,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100]){
    //     config.tournament_size = i ; 
    //     RunFourMethod(config, testNum, samplePeriod, csvFile) ; 
    // }
    // csvFile.close() ;
       
    // csvFile.open(folder+"Population.csv", ofstream::out) ; csvFile << "Result Type, Dimension, Representation, Population Size, Tournament Size, is Uniform Crossover, Crossover Method, Crossover Prob., Mutation Prob., # of Generations\n" ;
    // config.defaultSetting() ; 
    // for(int i=50; i<=500; i+=50){
    //     config.population_size = i ; 
    //     RunFourMethod(config, testNum, samplePeriod, csvFile) ; 
    // }
    // csvFile.close() ;
       
    // csvFile.open(folder+"Alpha1.csv", ofstream::out) ; csvFile << "Result Type, Dimension, Representation, Population Size, Tournament Size, is Uniform Crossover, Crossover Method, Crossover Prob., Mutation Prob., # of Generations\n" ;
    // config.defaultSetting() ; 
    // for(int i=0; i<=10; i++){
    //     config.alpha1 = 0.1*i ; 
    //     config.representation = "binary" ; config.uniform_crossover = true ; config.crossover_method = GetCrossoverMethod(config) ;
    //     testing(testNum, config, samplePeriod, csvFile) ;

    //     config.representation = "real" ; config.uniform_crossover = true ; config.crossover_method = GetCrossoverMethod(config) ;
    //     testing(testNum, config, samplePeriod, csvFile) ;
    // }
    // csvFile.close() ;
       
    csvFile.open(folder+"Alpha2.csv", ofstream::out) ; csvFile << "Result Type, Dimension, Representation, Population Size, Tournament Size, is Uniform Crossover, Crossover Method, Crossover Prob., Mutation Prob., # of Generations\n" ;
    config.defaultSetting() ; 
    for(int i=-1; i<=5; i++){
        config.alpha2 = 0.1*i ; 

        if(i==-1) config.alpha2 = 0.01 ;
        if(i==0) config.alpha2 = 0.05 ;
        config.representation = "real" ; config.uniform_crossover = false ; config.crossover_method = GetCrossoverMethod(config) ;
        testing(testNum, config, samplePeriod, csvFile) ;
        // RunFourMethod(config, testNum, samplePeriod, csvFile) ;
    }
    csvFile.close() ;

    // csvFile.open(folder+"LongLongGeneration_100.csv", ofstream::out) ; csvFile << "Result Type, Dimension, Representation, Population Size, Tournament Size, is Uniform Crossover, Crossover Method, Crossover Prob., Mutation Prob., # of Generations\n" ;
    // config.defaultSetting() ; 
    // config.dimension = 100 ; config.mut_prob = 1.0 / config.dimension ;
    // config.num_generations = 5000 ; 
    // RunFourMethod(config, testNum, samplePeriod, csvFile) ;
    // csvFile.close() ;

    // csvFile.open(folder+"ChangeDimension.csv", ofstream::out) ; csvFile << "Result Type, Dimension, Representation, Population Size, Tournament Size, is Uniform Crossover, Crossover Method, Crossover Prob., Mutation Prob., # of Generations\n" ;
    // config.defaultSetting() ; 
    // for(int i=10; i<=200; i+=10){
    //     config.dimension = i ; config.mut_prob = 1.0 / config.dimension ; config.num_generations = 500 ; 
    //     RunFourMethod(config, testNum, samplePeriod, csvFile) ;
    // }
    // csvFile.close() ;



    return 0;
}
// g++ -IGAProgram/include  -lboost_system -lboost_iostreams -lboost_filesystem --std=c++17 -O3  -o GAProgram/bin/experienment GAProgram/libs/*.cpp GAProgram/src/experienment.cpp 