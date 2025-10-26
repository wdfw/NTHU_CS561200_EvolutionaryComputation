#include "RealValueGA.hpp"

 ostream& operator<<(ostream& os, const RealValueChromosome& candidate){
    for(auto& v : candidate.phenotype) os << v << " " ; 
    return os ; 
}