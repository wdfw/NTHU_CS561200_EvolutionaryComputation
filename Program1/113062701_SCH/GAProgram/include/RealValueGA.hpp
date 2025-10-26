#pragma once
#include "GeneticAlgorithm.hpp"

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

class RealValueChromosome : public MundaneChromosome<vector<double>> {
public:
    uniform_real_distribution<double> uniformRealDistribution = uniform_real_distribution<double>(-512.0, 511.0);

    using Chromosome::operator= ;

    friend ostream& operator<<(ostream& os, const RealValueChromosome& candidate) ;

    RealValueChromosome() = default ;
    RealValueChromosome(const RealValueChromosome& candidate) = default ; 
    RealValueChromosome(function<double(const PhenotypeType&)> fn) : MundaneChromosome<vector<double>>(fn) {} ; 
    RealValueChromosome(const PhenotypeType& phenotype, function<double(const PhenotypeType&)> fn) : RealValueChromosome(fn){ (*this) = phenotype ;}

    void random(default_random_engine& generator) {
        for(int i=0; i<phenotype.size(); i++) phenotype[i] = uniformRealDistribution(generator) ;
    }
} ;


class RealValueGeneticAlgorithm : public GeneticAlgorithm<vector<double>, vector<double>, RealValueChromosome> {
public:
    using GeneticAlgorithm::GeneticAlgorithm ;

    uniform_real_distribution<double> uniformRealDistribution = uniform_real_distribution<double>(-512.0, 511.0);
    uniform_real_distribution<double> wholeArithmeticDistribution = uniform_real_distribution<double>(0.0, 1.0);
    bernoulli_distribution uniformCrossoverTesting ; // = bernoulli_distribution(Alpha1) ; 
    void Initialize(vector<ChromosomeType>& population) override { 
        population = vector<ChromosomeType>(config.population_size, {PhenotypeType(config.dimension), fitnessFunction}) ;
        uniformCrossoverTesting = bernoulli_distribution(config.alpha1) ; 
        for(int i=0; i<config.population_size; i++) population[i].random(generator) ; 
    } 

    void UniformCrossover(vector<ChromosomeType>& population1, vector<ChromosomeType>& population2, vector<ChromosomeType>& offsprings) override {
        offsprings.clear() ;
        for(int i=0, p1, p2; offsprings.size()<config.population_size; ++i){
            ChromosomeType offspring1 = population1[i%population1.size()], offspring2 = population2[i%population2.size()] ;
            
            if(crossoverDistrbution(generator)){
                // bool isFliped = generator()%2 ;
                // for(int j=0; j<offspring1.genotype.size(); ){
                //     int w = generator() % (offspring1.genotype.size()-j) + j + 1 ; 
                //     if(isFliped){
                //         for(int k=j; k<w; ++k) swap(offspring1.genotype[k], offspring2.genotype[k]) ;
                //     }
                //     isFliped = !isFliped ; 
                //     j = w ; 
                // }
                
                for(int j=0; j<offspring1.genotype.size(); ++j){
                    if(uniformCrossoverTesting(generator)) swap(offspring1.genotype[j], offspring2.genotype[j]) ;
                }
            }
            if(offsprings.size()<config.population_size) offsprings.push_back(offspring1) ;
            if(offsprings.size()<config.population_size) offsprings.push_back(offspring2) ;
        }
    }

    void Crossover(vector<ChromosomeType>& population1, vector<ChromosomeType>& population2, vector<ChromosomeType>& offsprings) override {
        offsprings.clear() ;

        // double alpha1 = 0.2 ; // wholeArithmeticDistribution(generator) ;

        for(int i=0, p1, p2, o1, o2; offsprings.size()<config.population_size; ++i){
            o1 = i%population1.size() ; o2 = i%population2.size() ; 
            ChromosomeType offspring1 = population1[o1], offspring2 = population2[o2] ;
            
            if(crossoverDistrbution(generator)){
                for(int j=0; j<offspring1.genotype.size(); ++j){
                    offspring1.genotype[j] = config.alpha2*population1[o1].genotype[j] + (1.0-config.alpha2)*population2[o2].genotype[j] ; 
                    offspring2.genotype[j] = config.alpha2*population2[o2].genotype[j] + (1.0-config.alpha2)*population1[o1].genotype[j] ; 
                }
            }

            offspring1 = offspring1.genotype ; offspring2 = offspring2.genotype ; 

            if(offsprings.size()<config.population_size) offsprings.push_back(offspring1) ;
            if(offsprings.size()<config.population_size) offsprings.push_back(offspring2) ;
        }
    }


    void Mutation(vector<ChromosomeType>& population, vector<ChromosomeType>& offsprings){
        offsprings = population ; 
        for(int i=0; i<offsprings.size(); ++i){
            for(int j=0; j<offsprings[i].genotype.size(); ++j){
                if(mutationDistrbution(generator)) offsprings[i].genotype[j] = uniformRealDistribution(generator) ; 
            }
        }
    }
} ;
