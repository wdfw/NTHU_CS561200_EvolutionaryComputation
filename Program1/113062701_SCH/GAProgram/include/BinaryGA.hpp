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


class BinaryChromosome : public Chromosome<vector<int>, vector<bool>> {
public:
    using Chromosome::operator= ;

    friend ostream& operator<<(ostream& os, const BinaryChromosome& candidate) ;

    BinaryChromosome() = default ;
    BinaryChromosome(const BinaryChromosome& candidate) = default ; 
    BinaryChromosome(function<double(const PhenotypeType&)> fn) : Chromosome<vector<int>, vector<bool>>(fn) {} ; 
    BinaryChromosome(const PhenotypeType& phenotype, function<double(const PhenotypeType&)> fn) : BinaryChromosome(fn){ (*this) = phenotype ;}
    BinaryChromosome(const GenotypeType& genotype, function<double(const PhenotypeType&)> fn) : BinaryChromosome(fn){(*this) = genotype ;}
    
    void P2G(const PhenotypeType& phenotype, GenotypeType& genotype) override  {
        genotype = GenotypeType(10*phenotype.size()) ;
        for(int i=0; i<phenotype.size(); i++){
            for(int n=phenotype[i]+512, j=0; j<10; j++){
                genotype[10*i+9-j] = n%2 ; n >>= 1 ; 
            }
        }
    }

    void G2P(const GenotypeType& genotype, PhenotypeType& phenotype) override {
        phenotype = PhenotypeType(genotype.size()/10) ;
        for(int i=0; i<phenotype.size(); i++){
            for(int j=0; j<10; j++){
                phenotype[i] <<= 1 ;
                phenotype[i] += genotype[10*i+j] ;
            }
            phenotype[i] -= 512 ;
        }
    }

    void random(default_random_engine& generator) {
        for(int i=0; i<genotype.size(); ++i) genotype[i] = generator()%2 ;
        G2P(genotype, phenotype) ;
    }

} ;



class BinaryGeneticAlgorithm : public GeneticAlgorithm<vector<int>, vector<bool>, BinaryChromosome> {
public:
    using GeneticAlgorithm::GeneticAlgorithm ;
    bernoulli_distribution uniformCrossoverTesting ; // = bernoulli_distribution(Alpha1) ; 

    void Initialize(vector<ChromosomeType>& population) override { 
        population = vector<ChromosomeType>(config.population_size, {PhenotypeType(config.dimension), fitnessFunction}) ;
        uniformCrossoverTesting = bernoulli_distribution(config.alpha1) ; 

        for(int i=0; i<config.population_size; ++i) population[i].random(generator) ; 
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
            offspring1 = offspring1.genotype ; offspring2 = offspring2.genotype ; 
            if(offsprings.size()<config.population_size) offsprings.push_back(offspring1) ;
            if(offsprings.size()<config.population_size) offsprings.push_back(offspring2) ;
        }
    }

    
    // RouletteWheel<int> ru =  RouletteWheel<int>([](const int& i){return double(i);}, clock(), vector<int>{1,2,3,4,5,6,7,8,9,10,11}) ; 
    void Crossover(vector<ChromosomeType>& population1, vector<ChromosomeType>& population2, vector<ChromosomeType>& offsprings) override {
        offsprings.clear() ;

        for(int i=0, p1, p2; offsprings.size()<config.population_size; ++i){
            ChromosomeType offspring1 = population1[i%population1.size()], offspring2 = population2[i%population2.size()] ;
            
            if(crossoverDistrbution(generator)){
                p1 = generator() % (1+offspring1.genotype.size()), p2 = generator() % (1+offspring2.genotype.size()) ;
                while(p1==p2){ p1 = generator() % (1+offspring1.genotype.size()) ; p2 = generator() % (1+offspring2.genotype.size()) ;}
                if(p1>p2) swap(p1, p2) ; 
                for(int j=p1; j<p2; j++) swap(offspring1.genotype[j], offspring2.genotype[j]) ;
            }
            
            offspring1 = offspring1.genotype ; offspring2 = offspring2.genotype ; 
            if(offsprings.size()<config.population_size) offsprings.push_back(offspring1) ;
            if(offsprings.size()<config.population_size) offsprings.push_back(offspring2) ;
        }
    }

    void Mutation(vector<ChromosomeType>& population, vector<ChromosomeType>& offsprings){
        offsprings = population ; 
        for(int i=0; i<offsprings.size(); ++i){
            // for(int j=0; j<offsprings[i].phenotype.size(); ++j){
            //     for(int k=0; k<10; ++k){
            //         if(generator()%(9-k+2+10)==0) offsprings[i].genotype[10*j+k] = !offsprings[i].genotype[10*j+k] ;
            //     }
            // }
            for(int j=0; j<offsprings[i].genotype.size(); ++j){
                if(mutationDistrbution(generator)) offsprings[i].genotype[j] = !offsprings[i].genotype[j] ;
            }
            offsprings[i] = offsprings[i].genotype ; 
        }
    }
} ;
