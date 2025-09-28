#pragma once
#include "config.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <climits>

using namespace std ; 

class GeneticAlgorithm ;

double SchwefelFunction(const vector<int>& x, int n=10) ;

class GeneticAlgorithm {
public:
    using PhenotypeType = vector<int> ; 
    using GenotypeType = vector<bool> ; 
    class CandidateType {
        public:
            PhenotypeType phenotype ; 
            GenotypeType genotype ;
            function<double(const PhenotypeType&)> fitnessFunction ;
            
            CandidateType(function<double(const PhenotypeType&)> fn) : fitnessFunction(fn) {}  ; 
            CandidateType(const CandidateType&) = default ; 
            CandidateType(const PhenotypeType& phenotype, function<double(const PhenotypeType&)> fn) ;
            CandidateType(const GenotypeType& genotype, function<double(const PhenotypeType&)> fn) ;
            
            double fitness() ;
            void random(int minNum=INT_MIN, int maxNum=INT_MAX) ;

            PhenotypeType& operator=(const PhenotypeType& phenotype) ;
            GenotypeType& operator=(const GenotypeType& genotype) ;
        private:
            bool _changeFlag = true ; 
            double _fitness = 0 ; 
    
            void P2G(const PhenotypeType& phenotype, GenotypeType& genotype) ;
            void G2P(const GenotypeType& genotype, PhenotypeType& phenotype) ;

            friend class GeneticAlgorithm ;
    };
    
    friend ostream& operator<<(ostream& os, const CandidateType& candidate) ;
    GeneticAlgorithm(const Configuration& config, function<double(const PhenotypeType&)> fn ) : config(config), fitnessFunction(fn)  {} ;
    void Solve() ;
    void Initialize() ; 
    double Evaluate(CandidateType& candidate) ; 
    void Select() ;
    void CrossOver() ;
    void Mutate() ;
    void Survive() ;

    vector<CandidateType> candidates ; 
    Configuration config ;

    function<double(const PhenotypeType&)> fitnessFunction ;
    

} ; 