#pragma once
#include "Config.hpp"
#include "RandomTool.hpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <climits>
#include <random>
#include <set>
#include <utility>

// #define Alpha1 0.5
// #define Alpha2 0.2

using namespace std ; 

template<typename ptype, typename gtype> class Chromosome ;
template<typename gtype> using MundaneChromosome = Chromosome<gtype,gtype> ;
template<typename ptype, typename gtype, typename ctype> class GeneticAlgorithm ;

template<typename ptype, typename gtype>
class Chromosome {
protected:
bool _reassignFlag = true ; 
public:
    using PhenotypeType = ptype ;
    using GenotypeType = gtype ;

    PhenotypeType phenotype ; 
    GenotypeType genotype ;
    double fitness = 0.0 ; 

    function<double(const PhenotypeType&)> fitnessFunction ;

    Chromosome() = default ; 
    Chromosome(const Chromosome& chromosome) = default ; 

    Chromosome(function<double(const PhenotypeType&)> fn) : fitnessFunction(fn) {}  ; 
    Chromosome(const PhenotypeType& phenotype, function<double(const PhenotypeType&)> fn) : fitnessFunction(fn) {} ;
    Chromosome(const GenotypeType& genotype, function<double(const PhenotypeType&)> fn) : fitnessFunction(fn) {} ;

    double eval(){
        fitness = fitnessFunction(phenotype) ;
        return fitness ; 
    }
    
    virtual PhenotypeType& operator=(const PhenotypeType& phenotype){
        this->phenotype = phenotype ;
        P2G(this->phenotype, this->genotype) ;
        this->_reassignFlag = true ;
        return this->phenotype ; 
    } 

    virtual GenotypeType& operator=(const GenotypeType& genotype){
        this->genotype = genotype ;
        G2P(this->genotype, this->phenotype) ;
        this->_reassignFlag = true ;
        return this->genotype ; 
    }

    virtual void P2G(const PhenotypeType& phenotype, GenotypeType& genotype) = 0 ;
    virtual void G2P(const GenotypeType& genotype, PhenotypeType& phenotype) = 0 ;
} ;

template<typename gtype>
class Chromosome<gtype, gtype> {
private:
    bool _reassignFlag = true ; 
public:
    double fitness = 0.0 ; 

    using PhenotypeType = gtype ;
    using GenotypeType  = gtype ;

    PhenotypeType phenotype ; 
    GenotypeType& genotype ; 

    function<double(const PhenotypeType&)> fitnessFunction ;

    Chromosome() : genotype(phenotype) {}
    Chromosome(const Chromosome& chromosome) : fitnessFunction(chromosome.fitnessFunction), genotype(phenotype), phenotype(chromosome.phenotype), fitness(chromosome.fitness), _reassignFlag(chromosome._reassignFlag) {} ;
    Chromosome(function<double(const PhenotypeType&)> fn) : genotype(phenotype), fitnessFunction(fn) {}  ; 
    Chromosome(const PhenotypeType& phenotype, function<double(const GenotypeType&)> fn) : genotype(this->phenotype), fitnessFunction(fn), phenotype(phenotype) {} ;

    double eval(){
        fitness = fitnessFunction(genotype) ;
        return fitness ; 
    }
    
    virtual PhenotypeType& operator=(const PhenotypeType& phenotype){
        this->phenotype = phenotype ;
        this->_reassignFlag = true ;
        return this->phenotype ; 
    } 

    Chromosome& operator=(const Chromosome& chromosome){
        this->fitness = chromosome.fitness ;
        this->_reassignFlag = chromosome._reassignFlag ;
        this->phenotype = chromosome.phenotype ;
        return *this ;
    }
} ;

template<typename ptype, typename gtype, typename ctype>
class GeneticAlgorithm {
public:
    using PhenotypeType = ptype ;
    using GenotypeType = gtype ;
    using ChromosomeType = ctype ;

    Configuration config ;

    default_random_engine generator;
    bernoulli_distribution crossoverDistrbution ;
    bernoulli_distribution mutationDistrbution ;

    function<double(const PhenotypeType&)> fitnessFunction ;

    // friend ostream& operator<<(ostream& os, const CandidateType& candidate) ;
    GeneticAlgorithm(const Configuration& config, function<double(const PhenotypeType&)> fn, int seed=0)
        : config(config), fitnessFunction(fn), crossoverDistrbution(config.cross_prob), mutationDistrbution(config.mut_prob), generator(seed){
        } ;
   
    virtual ChromosomeType Solve() ; 
    
    virtual void Evaluate(vector<ChromosomeType>& population) ; // 計算一個群體的fitness
    virtual void EvaluateAll(vector<ChromosomeType>& population) ; // 計算一個群體的fitness
    virtual void SelectParents(vector<ChromosomeType>& population, vector<ChromosomeType>& parents1, vector<ChromosomeType>& parents2) ; // 從群體中選出部分母群
    virtual void SelectSurvior(vector<ChromosomeType>& population, vector<ChromosomeType>& offsprings, vector<ChromosomeType>& survivors) ; // 從母群或子群中選存活者
    
    virtual void Initialize(vector<ChromosomeType>& population) = 0 ; // 初始化整個群體
    virtual void UniformCrossover(vector<ChromosomeType>& population1, vector<ChromosomeType>& population2, vector<ChromosomeType>& offsprings) = 0 ;
    virtual void Crossover(vector<ChromosomeType>& population1, vector<ChromosomeType>& population2, vector<ChromosomeType>& offsprings) = 0 ;
    virtual void Mutation(vector<ChromosomeType>& population, vector<ChromosomeType>& offsprings) = 0 ;
} ; 

template<typename ptype, typename gtype, typename ctype>
typename GeneticAlgorithm<ptype,gtype,ctype>::ChromosomeType GeneticAlgorithm<ptype,gtype,ctype>::Solve(){
    vector<ChromosomeType> population, parents1, parents2, crossoverOffsprings, mutationOffsprings, surviors ; 

    Initialize(population) ; 
    EvaluateAll(population) ; 
    for(int i=0; i<config.num_generations ; i++){
        SelectParents(population, parents1, parents2) ;

        if(config.uniform_crossover) UniformCrossover(parents1, parents2, crossoverOffsprings) ;
        else Crossover(parents1, parents2, crossoverOffsprings) ;
        Mutation(crossoverOffsprings, mutationOffsprings) ;

        Evaluate(mutationOffsprings) ; 
        SelectSurvior(population, mutationOffsprings, surviors) ; population = surviors ;
        EvaluateAll(population) ; 
    }

    return *min_element(population.begin(), population.end(), [](ChromosomeType& c1, ChromosomeType& c2){return c1.eval()<c2.eval();}) ; 
}

template<typename ptype, typename gtype, typename ctype>
void GeneticAlgorithm<ptype,gtype,ctype>::Evaluate(vector<ChromosomeType>& population){
    double sum = 0 ;
    for(int i=0; i<population.size(); i++) sum += population[i].eval() ; 
    // cout << "Ave: " << sum/population.size() << "\n" ;
}


template<typename ptype, typename gtype, typename ctype>
void GeneticAlgorithm<ptype,gtype,ctype>::EvaluateAll(vector<ChromosomeType>& population){
    double sum = 0 ;
    for(int i=0; i<population.size(); i++) sum += population[i].eval() ; 
    // cout << "Ave: " << sum/population.size() << "\n" ;
}

template<typename ptype, typename gtype, typename ctype>
void GeneticAlgorithm<ptype,gtype,ctype>::SelectParents(vector<ChromosomeType>& population, vector<ChromosomeType>& parents1, vector<ChromosomeType>& parents2){
    
    parents1.clear() ; parents2.clear() ; 
    
    for(int i=0; i<config.population_size; i++){
        vector<ChromosomeType> candidates ;
        vector<ChromosomeType>& parents = (i%2) ? parents2 : parents1 ; 
        for(int j=0; j<config.tournament_size ; j++) candidates.push_back(population[generator()%population.size()]) ; 

        // 機率型競爭法
        // RouletteWheel<ChromosomeType> rouletteWheel([](ChromosomeType& candidate){return candidate.eval();}, generator) ;
        // rouletteWheel.ReversedInitialize(candidates) ; 
        // parents.push_back(candidates[rouletteWheel()]) ; 
        
        // 絕對型競爭法
        parents.push_back(*min_element(candidates.begin(), candidates.end(), [](ChromosomeType& c1, ChromosomeType& c2){return c1.fitness<c2.fitness;}) ) ; 
    }
}

template<typename ptype, typename gtype, typename ctype>
void GeneticAlgorithm<ptype,gtype,ctype>::SelectSurvior(vector<ChromosomeType>& population,  vector<ChromosomeType>& offsprings, vector<ChromosomeType>& survivors){
    survivors = population ;
    survivors.insert(survivors.end(), offsprings.begin(), offsprings.end()) ;  
    sort(survivors.begin(), survivors.end(), [](ChromosomeType& c1, ChromosomeType& c2){return c1.fitness<c2.fitness;}) ; 

    for(;survivors.size()>population.size();) survivors.pop_back() ; 
} 



