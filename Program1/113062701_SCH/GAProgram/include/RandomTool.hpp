#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <functional>

using namespace std ; 

template <typename T>
class RouletteWheel {
private:
    default_random_engine generator ;
    uniform_real_distribution<double> uniformDistribution = uniform_real_distribution<double>(0.0, 1.0);
    function<double(T&)> evaluteFunction ;
    vector<double> cumulativeDistributionFunction ; 
public:
    RouletteWheel(function<double( T&)> fn, int seed=42) : generator(seed), evaluteFunction(fn) {
    } ; 
    RouletteWheel(function<double( T&)> fn, const default_random_engine& generator) : generator(generator), evaluteFunction(fn) {
    } ; 
    void Initialize( vector<T>& seq) ;
    void ReversedInitialize( vector<T>& seq) ;
    inline int operator()() ;
} ;

template <typename T>
inline int RouletteWheel<T>::operator()(){ 
    double x = uniformDistribution(generator) ; 
    for(int i=0; i<cumulativeDistributionFunction.size(); ++i) 
        if(x<=cumulativeDistributionFunction[i]) return i ; 
    return cumulativeDistributionFunction.size()-1 ; 
}

template <typename T>
void RouletteWheel<T>::Initialize( vector<T>& seq){
    double sum = 0.0 ; 
    cumulativeDistributionFunction.resize(seq.size()) ; 
    for(int i=0; i<seq.size(); i++){
        sum += evaluteFunction(seq[i]) ;
        cumulativeDistributionFunction[i] = sum ; 
    }
    for(auto& y : cumulativeDistributionFunction) y /= sum ; 
}

template <typename T>
void RouletteWheel<T>::ReversedInitialize( vector<T>& seq){

    vector<pair<double,int>> ithFitness(seq.size()) ; 
    vector<double> reversedFitnesses(seq.size()) ; 
    for(int i=0; i<seq.size(); ++i) ithFitness[i] = {evaluteFunction(seq[i]), i} ; 
    sort(ithFitness.begin(), ithFitness.end()) ;
    
    for(int i=0; i<seq.size()/2; ++i) swap(ithFitness[i].second, ithFitness[seq.size()-1-i].second) ;
    
    for(int i=0; i<seq.size(); ++i) reversedFitnesses[ithFitness[i].second] = ithFitness[i].first ; 

    double sum = 0.0 ; 
    cumulativeDistributionFunction.resize(seq.size()) ; 
    for(int i=0; i<seq.size(); i++){
        sum += reversedFitnesses[i] ; 
        cumulativeDistributionFunction[i] = sum ; 
    }
    for(auto& y : cumulativeDistributionFunction) y /= sum ; 


}