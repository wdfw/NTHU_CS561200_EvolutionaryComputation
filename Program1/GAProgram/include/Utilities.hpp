#include <cmath>
#include <vector>

using namespace std ; 

template<typename T>
double SchwefelFunction(const vector<T>& x, int n){
    double result = 418.98291*(n) ;

    // for(int i=0; i<x.size(); i++){
    //     T v = x[i] ; 
    //     result += v*sin(sqrt(abs(v))) ;
    // }
    for(auto& v : x) result -= v*sin(sqrt(abs(v))) ;
    return result ;
}