#include "BinaryGA.hpp"

ostream& operator<<(ostream& os, const BinaryChromosome& binaryChromosome){
    for(auto& v : binaryChromosome.phenotype) os << v << " " ; 
    os << "\n" ;
    for(const auto& v : binaryChromosome.genotype) os << v << " " ; 
    return os ; 
}
