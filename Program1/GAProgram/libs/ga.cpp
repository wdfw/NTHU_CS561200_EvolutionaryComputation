#include "ga.h"

using PhenotypeType = GeneticAlgorithm::PhenotypeType ;
using GenotypeType = GeneticAlgorithm::GenotypeType ;
double SchwefelFunction(const vector<int>& x, int n){
    double result = 0 ; //418.98291*n ;
    for(auto& v : x) result += v*sin(abs(v)) ;
    return result ;
}

void GeneticAlgorithm::Solve(){
    // Initialize() ; 
    // for(auto& candidate : candidates){
    //     candidate.fitness = Evaluate(candidate.individual) ; 
    // }

    // for(;;){

    // }

}
ostream& operator<<(ostream& os, const GeneticAlgorithm::CandidateType& candidate){
    for(auto& v : candidate.phenotype) os << v << " " ; 
    os << "\n" ;
    for(const auto& v : candidate.genotype) os << v ; 
    return os ; 
}

GeneticAlgorithm::CandidateType::CandidateType(const PhenotypeType& phenotype, function<double(const PhenotypeType&)> fn){
    this->fitnessFunction = fn ; 
    (*this) = phenotype ; 
}

GeneticAlgorithm::CandidateType::CandidateType(const GenotypeType& genotype, function<double(const PhenotypeType&)> fn){
    this->fitnessFunction = fn ; 
    (*this) = genotype ; 
}

void GeneticAlgorithm::CandidateType::G2P(const GenotypeType& genotype, PhenotypeType& phenotype){
    phenotype = PhenotypeType(genotype.size()/10) ;
    for(int i=0; i<phenotype.size(); i++){
        for(int j=0; j<10; j++){
            phenotype[i] <<= 1 ;
            phenotype[i] += genotype[10*i+j] ;
        }
    }
}
void GeneticAlgorithm::CandidateType::P2G(const PhenotypeType& phenotype, GenotypeType& genotype){
    genotype = GenotypeType(10*phenotype.size()) ;
    for(int i=0; i<phenotype.size(); i++){
        for(int n=phenotype[i], j=0; j<10; j++){
            genotype[10*i+9-j] = n%2 ; n >>= 1 ; 
        }
    }
}

PhenotypeType& GeneticAlgorithm::CandidateType::operator=(const PhenotypeType& phenotype){
    this->phenotype = phenotype ;
    P2G(this->phenotype, this->genotype) ;
    return this->phenotype ; 
}

GenotypeType& GeneticAlgorithm::CandidateType::operator=(const GenotypeType& genotype){
    this->genotype = genotype ;
    G2P(this->genotype, this->phenotype) ;
    return this->genotype ; 
}

void GeneticAlgorithm::CandidateType::random(int minNum, int maxNum){
    for(int i=0; i<phenotype.size(); i++) phenotype[i] = rand()%(maxNum-minNum+1) + minNum ;
    P2G(phenotype, genotype) ;
}

double GeneticAlgorithm::CandidateType::fitness(){
    // if(_changeFlag){
    _fitness = fitnessFunction(phenotype) ;
        // _changeFlag = false ;
    // }
    return _fitness ; 
}

void GeneticAlgorithm::Initialize(){
    candidates = vector<CandidateType>(config.population_size, CandidateType(fitnessFunction)) ;

    for(int i=0; i<config.population_size; i++){
        candidates[i] = PhenotypeType(config.dimension) ;
        candidates[i].random(-1,2) ; 
    }
}

double GeneticAlgorithm::Evaluate(CandidateType& candidate){
    return candidate.fitness() ;
}

